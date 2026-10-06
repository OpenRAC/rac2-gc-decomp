"""Fresh static catalogue of EE body extents and explicit boundary quality.

Generated labels and exact bytes are not original-function certificates. Strict
C extents plus fresh STT_FUNC metadata form a separate qualified tier. Every EE
byte and the excluded VU scope remain accounted for without instruction exports.
"""
from __future__ import annotations
import argparse
from bisect import bisect_right
from collections import Counter, defaultdict, deque
import hashlib
import json
from pathlib import Path
import re
import struct
import rabbitizer
import elf_tools
import region

TARGET="SCUS_972.68"
DECL=re.compile(r"^nonmatching (\S+), (0x[0-9A-Fa-f]+)")
COMMENT=re.compile(r"/\* [0-9A-Fa-f]+ ([0-9A-Fa-f]{8}) ([0-9A-Fa-f]{8}) \*/")
CPU={".text","core.text"}


def sha(data):return hashlib.sha256(data).hexdigest()

def image(path):
    data=Path(path).read_bytes();elf=elf_tools._parse(data)
    loads=[s for s in elf["segments"] if s["type"]==1]
    def read(address,size):
        owners=[s for s in loads if s["address"]<=address and address+size<=s["address"]+s["filesz"]]
        if len(owners)!=1:raise ValueError("Body not uniquely file-backed")
        s=owners[0];off=s["offset"]+address-s["address"]
        return data[off:off+size]
    return data,elf,read


def stt_functions(data,elf):
    result=defaultdict(list);shoff=struct.unpack_from("<I",data,32)[0]
    stride,count=struct.unpack_from("<HH",data,46)
    headers=[struct.unpack_from("<10I",data,shoff+i*stride) for i in range(count)]
    for s in headers:
        if s[1] not in (2,11):continue
        if s[9]<16 or s[5]%s[9] or s[6]>=count:raise ValueError("Invalid symbol table")
        strings=headers[s[6]]
        if strings[1]!=3:raise ValueError("Symbol names not STRTAB")
        names=data[strings[4]:strings[4]+strings[5]]
        for off in range(s[4],s[4]+s[5],s[9]):
            name,address,size,info,other,index=struct.unpack_from("<IIIBBH",data,off)
            if info&15!=2 or not size:continue
            if name>=len(names) or names.find(b"\0",name)<0:raise ValueError("Bad symbol name")
            result[(address,size)].append(names[name:names.find(b"\0",name)].decode("utf8",errors="replace"))
    return dict(result)


def parse_maps(path,program,read):
    rows=[];pending=None;active=None
    with Path(path).open(encoding="utf8") as source:
        for line_number,line in enumerate(source,1):
            match=DECL.match(line)
            if match:pending=(match[1],int(match[2],16));continue
            if line.startswith("glabel ") and pending and line.split()[1]==pending[0]:
                if active:raise ValueError("Nested declared function")
                active={"declared_symbol":pending[0],"declared_size":pending[1],"address":None,
                        "last":None,"raw":bytearray(),"line":line_number,"aliases":[]}
                continue
            if line.startswith("alabel ") and active:
                name=line.split()[1]
                if re.fullmatch(r"func_[0-9A-Fa-f]{8}",name):active["aliases"].append(int(name[5:],16))
            if line.startswith("endlabel ") and active:
                if line.split()[1]!=active["declared_symbol"] or active["address"] is None:raise ValueError("Bad endlabel")
                size=active["last"]+4-active["address"]
                if size!=len(active["raw"]) or size!=active["declared_size"]:raise ValueError("Declared extent mismatch")
                if bytes(active["raw"])!=read(active["address"],size):raise ValueError("ASM comments differ from reference")
                rows.append({"program":program,"address":active["address"],"size":size,
                             "raw_sha256":sha(active["raw"]),"declared_symbol":active["declared_symbol"],
                             "inner_declared_entries":active["aliases"],"source_line":active["line"],
                             "source_file":path.name})
                active=None;pending=None;continue
            if active:
                match=COMMENT.search(line)
                if match:
                    address=int(match[1],16)
                    if active["address"] is None:active["address"]=address
                    elif address!=active["last"]+4:raise ValueError("Noncontiguous body")
                    active["last"]=address;active["raw"].extend(bytes.fromhex(match[2]))
    if active:raise ValueError("Unterminated body")
    return rows


def decode_flow(word,address):
    ins=rabbitizer.Instruction(word,address,rabbitizer.InstrCategory.R5900)
    valid=ins.isValid();record=None;writes_gp=False
    if valid:
        for suffix in ("Rs","Rt","Rd"):
            if getattr(ins,"modifies"+suffix)() and getattr(ins,suffix.lower()).value==28:writes_gp=True
        if ins.isFunctionCall():
            target=(ins.getBranchVramGeneric() if ins.isBranch() else ins.getInstrIndexAsVram()
                    if ins.isJumpWithAddress() else None)
            record={"kind":"call","target":target,"delay":ins.hasDelaySlot()}
        elif ins.isReturn():record={"kind":"return","target":None,"delay":ins.hasDelaySlot()}
        elif ins.isBranch():
            record={"kind":"branch","target":ins.getBranchVramGeneric(),"delay":ins.hasDelaySlot(),
                    "conditional":not ins.isUnconditionalBranch()}
        elif ins.isJump():
            record={"kind":"jump" if ins.isJumpWithAddress() else "indirect_jump",
                    "target":ins.getInstrIndexAsVram() if ins.isJumpWithAddress() else None,"delay":ins.hasDelaySlot()}
    return valid,record,writes_gp


def cfg_check(address,size,controls,invalid,entry_set):
    end=address+size;work=[address];seen=set();escaped=set();missing_delay=set();unknown=set();terminal=[]
    while work:
        pc=work.pop()
        if pc in seen:continue
        if pc<address or pc>=end:escaped.add(pc);continue
        seen.add(pc);control=controls.get(pc)
        if control is None:
            work.append(pc+4);continue
        if control["delay"]:
            slot=pc+4
            if slot>=end:missing_delay.add(slot)
            else:
                seen.add(slot)
                if slot in controls:unknown.add(slot)
        after=pc+8 if control["delay"] else pc+4
        kind,target=control["kind"],control["target"]
        if kind=="return":terminal.append((pc,"return"))
        elif kind=="call":work.append(after)
        elif kind=="indirect_jump":unknown.add(pc)
        elif kind in ("jump","branch"):
            if address<=target<end:work.append(target)
            elif target in entry_set:
                terminal.append((pc,"tail_or_cross_function_branch"))
            else:escaped.add(target)
            if kind=="branch" and control.get("conditional"):work.append(after)
    unreachable_non_nop=[]
    # All-word validity is checked separately; unused tails remain unresolved.
    return {"reachable":seen,"escaped_targets":sorted(escaped),"missing_delay_slots":sorted(missing_delay),
            "unresolved_indirect_or_delay_control":sorted(unknown),"terminal_exits":terminal,
            "invalid_words":sorted(x for x in invalid if address<=x<end),
            "closed":not (escaped or missing_delay or unknown) and bool(terminal)}


def qualified_rows(repo,program,read,candidate_read,symbols):
    slug=program.removeprefix("levels/")
    path=repo/"progress/integration.json" if program=="boot" else repo/"progress/levels"/f"{slug}.json"
    proof=json.loads(path.read_bytes());gate=proof.get("full_boot_gate") or proof["full_level_gate"]
    if not gate["matched"]:raise ValueError("Current C proof full gate not matched")
    functions=proof["functions"]+proof.get("native",{}).get("object_qualification",{}).get("functions",[])
    unique={};failures=[]
    for f in functions:
        key=(f["address"],f["size"]);raw=read(*key)
        if not f["matched"] or f["different_bytes"] or sha(raw)!=f["reference_sha256"] or f["candidate_sha256"]!=sha(raw):
            raise ValueError("Current complete C body proof differs from reference")
        if candidate_read(*key)!=raw:raise ValueError("Fresh candidate C span differs")
        if key not in symbols:
            failures.append({"program":program,"address":key[0],"size":key[1],"symbol":f["symbol"],"reason":"missing_fresh_STT_FUNC_extent"})
            continue
        unique[key]={"program":program,"address":key[0],"size":key[1],"raw_sha256":sha(raw),
                     "qualified_symbol":f["symbol"],"candidate_func_symbols":symbols[key],
                     "proof_source":"progress/integration.json" if program=="boot" else f"progress/levels/{slug}.json",
                     "proof_sha256":sha(path.read_bytes())}
    return list(unique.values()),failures


def reconcile(declared,qualified):
    """Qualified compiler extents win; excluded prefixes/tails remain fragments."""
    out=[];qs=sorted(qualified,key=lambda x:x["address"])
    for first,second in zip(qs,qs[1:]):
        if first["address"]+first["size"]>second["address"]:raise ValueError("Overlapping qualified extents")
    for row in declared:
        start,end=row["address"],row["address"]+row["size"]
        overlaps=[q for q in qs if start<q["address"]+q["size"] and q["address"]<end]
        if not overlaps:out.append(dict(row));continue
        cursor=start
        for q in overlaps:
            if cursor<q["address"]:
                out.append({**row,"address":cursor,"size":q["address"]-cursor,"force_fragment":True})
            cursor=max(cursor,q["address"]+q["size"])
        if cursor<end:out.append({**row,"address":cursor,"size":end-cursor,"force_fragment":True})
    for q in qs:
        enriched=dict(q,qualified=True)
        parents=[r for r in declared if r["address"]<=q["address"]<r["address"]+r["size"]]
        if len(parents)==1:
            parent=parents[0]
            enriched["declared_container"]={"address":parent["address"],"size":parent["size"],"symbol":parent["declared_symbol"]}
            enriched["declared_symbol"]=(parent["declared_symbol"] if q["address"]==parent["address"] else
                f"func_{q['address']:08X}" if q["address"] in parent.get("inner_declared_entries",[]) else None)
            enriched["source_file"],enriched["source_line"]=parent["source_file"],parent["source_line"]
        out.append(enriched)
    return sorted(out,key=lambda x:x["address"])


def catalogue(repo,reference_root,build_root):
    repo,reference_root,build_root=map(lambda x:Path(x).resolve(),(repo,reference_root,build_root))
    owner=region.by_serial(TARGET,repo);owner.require_pinned();owner.require_matching("global static catalogue")
    programs={};all_calls=[];global_entries=set();qualified_failures=[]
    for program,pin in owner.program_pins().items():
        slug=program.removeprefix("levels/");name="boot.elf" if program=="boot" else "overlay.elf"
        ref=reference_root/name if program=="boot" else reference_root/program/name
        data,elf,read=image(ref)
        if sha(data)!=pin:raise ValueError("Reference is not region-pinned")
        directory=build_root/slug;gate_path=directory/"gate.json";gate=json.loads(gate_path.read_bytes())
        candidate_data,candidate_elf,candidate_read=image(directory/"build"/name)
        if not gate["matched"] or gate["reference_sha256"]!=pin or sha(candidate_data)!=gate["candidate_sha256"]:
            raise ValueError("Fresh build recorded gate pin mismatch")
        if sha((directory/"assets"/name).read_bytes())!=pin:raise ValueError("Fresh build input asset mismatch")
        declared=[];asm_pins=[]
        for path in sorted((directory/"asm").glob("*.s")):
            declared+=parse_maps(path,program,read)
            asm_pins.append({"file":path.name,"sha256":sha(path.read_bytes())})
        qualified,failures=qualified_rows(repo,program,read,candidate_read,stt_functions(candidate_data,candidate_elf))
        qualified_failures+=failures;rows=reconcile(declared,qualified)
        sections=[{k:s[k] for k in ("name","type","flags","address","size")} for s in elf["sections"] if s["flags"]&2 and s["size"]]
        cpu=[s for s in sections if s["name"] in CPU and s["type"]==1 and s["flags"]&4]
        controls={};invalid=set();gp_writes=[];valid_words=0
        for section in cpu:
            raw=read(section["address"],section["size"])
            for i,(word,) in enumerate(struct.iter_unpack("<I",raw)):
                address=section["address"]+i*4;valid,control,gp=decode_flow(word,address)
                valid_words+=valid
                if not valid:invalid.add(address)
                if gp:gp_writes.append(address)
                if control:
                    controls[address]=control
                    if control["kind"]=="call" and control["target"] is not None:
                        all_calls.append({"caller_program":program,"caller":address,"target":control["target"]})
        global_entries.update((program,x["address"]) for x in rows)
        programs[program]={"reference_sha256":pin,"candidate_sha256":sha(candidate_data),"gate_sha256":sha(gate_path.read_bytes()),
           "sections":sections,"entry":elf["entry"],"asm_pins":asm_pins,"rows":rows,"read":read,
           "controls":controls,"invalid":invalid,"valid_words":valid_words,"gp_writes":gp_writes,
           "generated_maps":len(declared),"qualified_extents":len(qualified)}
        print("decoded",program,len(rows),"invalid",len(invalid),flush=True)
    boot=programs["boot"]
    def in_cpu(program,address):return any(s["name"] in CPU and s["address"]<=address<s["address"]+s["size"] for s in programs[program]["sections"])
    def in_resident_boot(address):return any(s["name"]=="core.text" and s["address"]<=address<s["address"]+s["size"] for s in boot["sections"])
    callers=defaultdict(list)
    interior_calls=defaultdict(list);interior_branches=defaultdict(list)
    indices={pr:([r["address"] for r in value["rows"]],value["rows"]) for pr,value in programs.items()}
    def owner_row(program,address):
        starts,rows=indices[program];index=bisect_right(starts,address)-1
        if index>=0 and address<rows[index]["address"]+rows[index]["size"]:return rows[index]
        return None
    for call in all_calls:
        program=call["caller_program"];target=call["target"]
        destination=program if in_cpu(program,target) else "boot" if in_resident_boot(target) else None
        if destination:
            callers[(destination,target)].append({"program":program,"address":call["caller"]})
            row=owner_row(destination,target)
            if row and target!=row["address"]:
                interior_calls[(destination,row["address"])].append({"program":program,"source":call["caller"],"target":target})
    for program,p in programs.items():
        for source,control in p["controls"].items():
            if control["kind"] not in ("jump","branch") or control["target"] is None:continue
            target=control["target"]
            destination=program if in_cpu(program,target) else "boot" if in_resident_boot(target) else None
            if destination is None:continue
            row=owner_row(destination,target);source_row=owner_row(program,source)
            if row and target!=row["address"] and (destination!=program or source_row is None or source_row["address"]!=row["address"]):
                interior_branches[(destination,row["address"])].append({"program":program,"source":source,"target":target})
    all_rows=[];contexts=[];coverage=[];residuals=[];tier_counts=Counter();tier_bytes=Counter()
    for program,p in programs.items():
        read=p["read"];entries={x["address"] for x in p["rows"]}|{x for pr,x in global_entries if pr=="boot" and in_resident_boot(x)}
        rows=p["rows"]
        for row in rows:
            address,size=row["address"],row["size"];raw=read(address,size);row["raw_sha256"]=sha(raw)
            flow=cfg_check(address,size,p["controls"],p["invalid"],entries)
            unreachable=sum(1 for offset in range(0,size,4) if address+offset not in flow["reachable"] and raw[offset:offset+4]!=b"\0"*4)
            first=struct.unpack_from("<I",raw)[0]
            prologue=(first>>26 in (9,25) and (first>>21)&31==29 and (first>>16)&31==29 and first&0x8000!=0)
            direct=callers[(program,address)]
            inside_calls=interior_calls[(program,address)];inside_branches=interior_branches[(program,address)]
            if row.get("qualified"):status="qualified_complete"
            elif row.get("force_fragment"):status="ambiguous_fragment"
            elif (flow["closed"] and not flow["invalid_words"] and not unreachable and not inside_calls and not inside_branches
                  and (prologue or direct or address==p["entry"])):status="flow_supported_inferred"
            else:status="inferred"
            evidence=["Reference body and generated emitted bytes agree"]
            if row.get("qualified"):evidence=["Complete C body matches reference and fresh candidate","Fresh candidate STT_FUNC has the same address and complete size"]
            elif status=="flow_supported_inferred":evidence+=["Decoded local flow closes at returns/declared tail targets with complete delay slots","Entry has a static call or frame-opening witness; original boundary remains inferred"]
            elif status=="ambiguous_fragment":evidence+=["Residual from a generated extent split by a separately qualified C entry; not a certified function"]
            row["boundary"]={"status":status,"evidence":evidence,
              "sources":[{"kind":"fresh_generated_map","file":row.get("source_file"),"line":row.get("source_line")}]
                if not row.get("qualified") else [{"kind":"current_C_proof","path":row["proof_source"],"sha256":row["proof_sha256"]}],
              "entry_checks":{"static_direct_calls":len(direct),"caller_samples":direct[:3],"frame_opener":prologue,"matches_ELF_entry":address==p["entry"],
                              "declared_inner_entries":row.get("inner_declared_entries",[]),"candidate_STT_FUNC":row.get("candidate_func_symbols",[]),
                              "interior_direct_call_count":len(inside_calls),"interior_direct_call_samples":inside_calls[:3],
                              "incoming_cross_boundary_interior_count":len(inside_branches),"incoming_cross_boundary_interior_samples":inside_branches[:3]},
              "exit_checks":{"closed_cfg":flow["closed"],"return_or_tail_exits":len(flow["terminal_exits"]),
                             "escaped_targets":flow["escaped_targets"],"missing_delay_slots":flow["missing_delay_slots"],
                             "unresolved_indirect_or_delay_control":flow["unresolved_indirect_or_delay_control"],
                             "invalid_word_addresses":flow["invalid_words"],"unreachable_nonzero_words":unreachable}}
            row["id"]=f"{program}:{address:08x}:{size}:{row['raw_sha256']}"
            for key in ("qualified","force_fragment","source_file","source_line","proof_source","proof_sha256"):
                row.pop(key,None)
            tier_counts[status]+=1;tier_bytes[status]+=size;all_rows.append(row)
        # Exact byte partition; residuals are retained, never counted as functions.
        cpu=[s for s in p["sections"] if s["name"] in CPU and s["type"]==1 and s["flags"]&4]
        program_residual=0
        for section in cpu:
            cursor=section["address"];end=cursor+section["size"]
            owners=sorted([r for r in rows if cursor<=r["address"]<end],key=lambda r:r["address"])
            for row in owners:
                if row["address"]<cursor or row["address"]+row["size"]>end:raise ValueError("Catalogue interval overlap/section escape")
                if cursor<row["address"]:
                    raw=read(cursor,row["address"]-cursor);kind="zero_padding_or_delay_residual" if not any(raw) else "unassigned_EE_code_or_data_gap"
                    residuals.append({"program":program,"address":cursor,"size":len(raw),"raw_sha256":sha(raw),"kind":kind})
                    program_residual+=len(raw)
                cursor=row["address"]+row["size"]
            if cursor<end:
                raw=read(cursor,end-cursor);residuals.append({"program":program,"address":cursor,"size":len(raw),"raw_sha256":sha(raw),
                          "kind":"zero_padding_or_delay_residual" if not any(raw) else "unassigned_EE_code_or_data_gap"});program_residual+=len(raw)
        vu=[s for s in p["sections"] if s["name"]==".vutext" and s["type"]==1]
        for section in vu:
            raw=read(section["address"],section["size"])
            residuals.append({"program":program,"address":section["address"],"size":section["size"],"raw_sha256":sha(raw),"kind":"VU_microcode_excluded_from_EE_normalization"})
        cpu_bytes=sum(s["size"] for s in cpu);function_bytes=sum(r["size"] for r in rows)
        if cpu_bytes!=function_bytes+program_residual:raise ValueError("EE coverage partition is incomplete")
        coverage.append({"program":program,"ee_scope_bytes":cpu_bytes,"row_bytes":function_bytes,"ee_residual_bytes":program_residual,
                         "vu_bytes":sum(s["size"] for s in vu),"valid_decoded_words":p["valid_words"],"invalid_word_addresses":sorted(p["invalid"]),
                         "generated_intervals":p["generated_maps"],"qualified_extents":p["qualified_extents"]})
        contexts.append({"program":program,"reference_sha256":p["reference_sha256"],"sections":p["sections"],
                         "function_entries":[r["address"] for r in rows if r["boundary"]["status"] in ("qualified_complete","flow_supported_inferred")],
                         "gp":None,"gp_verified":False,"gp_evidence":["No universal function-entry GP proof supplied; G0 catalogue value is not retail GP"],
                         "gpr_gp_write_addresses":p["gp_writes"]})
    return {"schema":1,"target":TARGET,"kind":"global-static-boundary-catalogue","decoder":"rabbitizer "+rabbitizer.__version__+" R5900",
            "functions":all_rows,"contexts":contexts,"coverage":coverage,"residuals":residuals,
            "reference_pins":{pr:p["reference_sha256"] for pr,p in programs.items()},
            "build_pins":{pr:{k:p[k] for k in ("candidate_sha256","gate_sha256","asm_pins")} for pr,p in programs.items()},
            "qualified_extent_failures":qualified_failures,"summary":{"rows":len(all_rows),"boundary_status_counts":dict(tier_counts),
                  "boundary_status_bytes":dict(tier_bytes),"EE_scope_bytes":sum(x["ee_scope_bytes"] for x in coverage),
                  "EE_residual_bytes":sum(x["ee_residual_bytes"] for x in coverage),"VU_bytes":sum(x["vu_bytes"] for x in coverage)},
            "limits":["Qualified means a complete compiled C extent, not recovery of an original source/object boundary.",
                       "Flow-supported boundaries remain inferred: static entry/exit structure is not runtime or semantic identity.",
                       "Indirect transfers, missing delay slots, undecoded/unreachable words and unsupplied GP context remain explicit."]}


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo",type=Path,required=True);parser.add_argument("--references",type=Path,required=True)
    parser.add_argument("--build",type=Path,required=True);parser.add_argument("--output",type=Path,required=True)
    args=parser.parse_args()
    root=args.repo.resolve()
    for path in (args.references,args.build,args.output):
        resolved=path.resolve()
        if resolved==root or resolved.is_relative_to(root) or root.is_relative_to(resolved):
            parser.error("Runtime inputs/output must be separate from the repository")
    if args.output.exists() or args.output.is_symlink():parser.error("Output already exists")
    report=catalogue(args.repo,args.references,args.build)
    with args.output.open("x",encoding="utf8") as out:json.dump(report,out,separators=(",",":"));out.write("\n")
    print("SUMMARY",json.dumps(report["summary"]),flush=True)

if __name__=="__main__":main()
