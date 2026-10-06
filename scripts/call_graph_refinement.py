"""Deterministic monotone refinement of certified body templates by static targets.

Input catalogues must already have validated intervals/certificates. Complete
call_dependencies metadata is preferred; legacy external j26 relocations are
only a compatibility fallback, never a complete-graph certificate. Refinement
splits classes only and does not prove original source or semantic equivalence.
Raw targets of unowned HI/LO and GP data bindings remain in the primary key.
"""
from collections import defaultdict
import hashlib
import json

SUPPORTED={"qualified_complete","flow_supported_inferred"}
KINDS={"call","jump","branch","external-branch","j","jal","j26"}

def _class_id(members):
    return "cfg:"+hashlib.sha256(json.dumps(sorted(members),separators=(",",":")).encode()).hexdigest()

def _supported(row):
    norm=row.get("normalization") or {};cert=norm.get("certificate") or {}
    return (row.get("boundary",{}).get("status") in SUPPORTED
            and isinstance(norm.get("signature_sha256"),str)
            and cert.get("exact") is True
            and cert.get("raw_sha256")==row.get("raw_sha256")
            and cert.get("reconstructed_sha256")==row.get("raw_sha256"))

def _edges(row):
    complete="call_dependencies" in row
    values=row["call_dependencies"] if complete else (row.get("normalization") or {}).get("relocations",[])
    if not isinstance(values,list):raise ValueError("Static dependencies must be a list")
    edges=set()
    for item in values:
        if not isinstance(item,dict):raise ValueError("Invalid static dependency")
        if not complete and item.get("kind")!="j26":continue
        offset,kind,target=item.get("offset"),item.get("kind"),item.get("target")
        if kind not in KINDS or type(offset) is not int or offset<0 or offset%4 or offset>row["size"]-4:
            raise ValueError("Invalid static dependency offset/kind")
        if type(target) is not int or target<0 or target>0xffffffff or target%4:
            raise ValueError("Static dependency target must be an aligned ELF32 address")
        if row["address"]<=target<row["address"]+row["size"]:continue
        if item.get("internal") is True:raise ValueError("External dependency marked internal")
        program=item.get("target_program")
        if program is not None and (not isinstance(program,str) or not program):raise ValueError("Invalid target program")
        edges.add((offset,kind,target,program or ""))
    return sorted(edges),complete

def _raw_data_bindings(row):
    """Retain unowned data targets; pointer use is not an original object base.

    No ownership exemption is accepted by this API. All HI/LO and GP metadata
    retain their actual numeric targets/field positions, even if a generic role
    or pointer-use certificate exists. This is structural, not semantic identity.
    """
    values=(row.get("normalization") or {}).get("relocations",[])
    if not isinstance(values,list):raise ValueError("Relocations must be a list")
    bindings=set()
    for relocation in values:
        if not isinstance(relocation,dict):raise ValueError("Invalid relocation")
        kind=relocation.get("kind")
        if kind not in ("hi16_lo16","gp16"):continue
        target,offset=relocation.get("target"),relocation.get("offset")
        if type(target) is not int or not 0<=target<=0xffffffff or type(offset) is not int or offset<0 or offset%4 or offset>row["size"]-4:
            raise ValueError("Invalid raw data binding")
        high,low,mode=-1,-1,""
        if kind=="hi16_lo16":
            high=relocation.get("high_offset");lo=relocation.get("low_offset");mode=relocation.get("lo_mode")
            if (type(high) is not int or high<0 or high%4 or high>row["size"]-4
                    or (lo is not None and (type(lo) is not int or lo<0 or lo%4 or lo>row["size"]-4))
                    or mode not in ("addiu","daddiu","ori","memory","none")
                    or (lo is None)!=(mode=="none")):
                raise ValueError("Invalid HI/LO data fields")
            low=-1 if lo is None else lo
        gp=-1
        if kind=="gp16":
            gp=relocation.get("gp")
            if type(gp) is not int or not 0<=gp<=0xffffffff or not -32768<=target-gp<=32767:
                raise ValueError("Invalid GP data binding base/operand")
        bindings.add((target,high,low,mode,kind,offset,gp))
    return tuple(sorted(bindings,key=lambda item:(item[5],item[4],item[1],item[2],item[3],item[0],item[6])))


def refine_call_groups(rows,programs=None):
    """Return stable classes after ordered external static control-target splits.

    rows: validated catalogue function rows. programmes: optional exporter dict
    or list of programme metadata, used only to recognize boot core.text as a
    resident fallback. Targets resolve by actual programme+entry address; missing
    targets remain distinct by programme+target. Unsupported rows stay singleton.
    """
    if not isinstance(rows,list) or not rows:raise ValueError("Nonempty function rows required")
    if isinstance(programs,list):programs={p["program"]:p for p in programs}
    programs=programs or {}
    by_id={};entries={};initial=defaultdict(list);dependencies={};complete_rows=0
    for row in rows:
        identity,program,address,size=row.get("id"),row.get("program"),row.get("address"),row.get("size")
        if (not isinstance(identity,str) or not identity or identity in by_id or not isinstance(program,str)
                or not program or type(address) is not int or address<0 or address%4
                or type(size) is not int or size<=0 or size%4):raise ValueError("Invalid or duplicate catalogue row")
        if (program,address) in entries:raise ValueError("Ambiguous programme entry alias")
        by_id[identity]=row;entries[(program,address)]=identity
        eligible=_supported(row)
        key=("supported",size,row["normalization"]["signature_sha256"],_raw_data_bindings(row)) if eligible else ("singleton",identity)
        initial[key].append(identity)
        dependencies[identity],complete=_edges(row);complete_rows+=complete
    def labels(groups):return {identity:_class_id(members) for members in groups for identity in members}
    groups=[sorted(members) for members in initial.values()];groups.sort(key=lambda members:tuple(members))
    current=labels(groups);initial_count=len(groups)
    def resident_boot(target):
        boot=programs.get("boot",{})
        return any(s.get("name")=="core.text" and s["address"]<=target<s["address"]+s["size"]
                   for s in boot.get("ee_sections",boot.get("sections",[])))
    resolved={};unresolved=0
    for identity,edges in dependencies.items():
        row=by_id[identity];result=[]
        for offset,kind,target,explicit_program in edges:
            program=explicit_program or row["program"]
            callee=entries.get((program,target))
            if callee is None and not explicit_program and resident_boot(target):
                callee=entries.get(("boot",target))
            token=("resolved",callee) if callee is not None else ("unresolved",program,target)
            unresolved+=callee is None;result.append((offset,kind,token))
        resolved[identity]=result
    iterations=0
    while True:
        iterations+=1;buckets=defaultdict(list)
        for identity in sorted(by_id):
            edges=tuple((offset,kind,("class",current[token[1]]) if token[0]=="resolved" else token)
                        for offset,kind,token in resolved[identity])
            # Retain the previous class to forbid all accidental later merges.
            buckets[(current[identity],edges)].append(identity)
        next_groups=sorted((sorted(members) for members in buckets.values()),key=lambda members:tuple(members))
        next_labels=labels(next_groups)
        if len(next_groups)<len(groups):raise AssertionError("Refinement merged partitions")
        if next_labels==current:
            groups=next_groups;break
        groups,current=next_groups,next_labels
        if iterations>len(rows):raise AssertionError("Monotone refinement failed to converge")
    return {"class_by_id":current,"groups":[{"id":_class_id(members),"members":members} for members in groups],
            "iterations":iterations,"initial_class_count":initial_count,"final_class_count":len(groups),
            "external_static_edge_count":sum(len(v) for v in resolved.values()),"unresolved_edge_count":unresolved,
            "complete_dependency_rows":complete_rows,"legacy_dependency_rows":len(rows)-complete_rows,
            "complete_graph_coverage_claimed":complete_rows==len(rows),
            "primary_data_binding_policy":"retain-unowned-data-address-operands",
            "limits":["Static graph partition refinement is not semantic or original-source equality.",
                      "Explicit dependency lists need a separate pinned extraction completeness receipt.",
                      "Legacy relocation fallback omits unmodified static targets; never certify it as complete."]}
