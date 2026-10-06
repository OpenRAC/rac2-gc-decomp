"""Replay bounded address witnesses and all pinned GP-write owner bodies."""
import argparse
import collections
import json
from pathlib import Path
import struct
import sys
from data_gp_audit import sha,public_member

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('repo','audit','boundaries','references','pointer-evidence','output'):p.add_argument('--'+name,type=Path,required=True)
    args=p.parse_args()
    if args.output.exists():p.error('Fresh output required')
    sys.path.insert(0,str(args.repo/'scripts'))
    import relocation_identity as norm
    import rabbitizer
    from pointer_evidence_loader import ReferenceImage,load_pointer_evidence
    audit=json.loads(args.audit.read_bytes());boundaries=json.loads(args.boundaries.read_bytes())
    pointers=load_pointer_evidence(args.pointer_evidence,boundaries,args.references,norm)
    contexts={c['program']:dict(c) for c in boundaries['contexts']};fp=collections.defaultdict(dict)
    for row in boundaries['functions']:
        if row['boundary']['status'] in ('qualified_complete','flow_supported_inferred'):
            fp[row['program']][row['address']]={'size':row['size'],'raw_sha256':row['raw_sha256'],'boundary_tier':row['boundary']['status']}
    prepared={}
    for program,context in contexts.items():
        context.update(function_pins=fp[program],pointer_argument_roles=pointers.get(program,[]));prepared[program]=norm.prepare_context(context)
    images={}
    def body(row):
        program=row['program']
        if program not in images:images[program]=ReferenceImage(args.references,program,contexts[program]['reference_sha256'])
        return images[program].body(row['address'],row['size'],row['raw_sha256'])
    families=[]
    for family in audit['families']:
        if family['rank'] not in (2,4,16):continue
        witnesses=[]
        for member in family['members'][:2]:
            raw=body(member);words=struct.unpack('<'+'I'*(len(raw)//4),raw)
            record,controls,reachable=norm._analyze(words,member['address'],prepared[member['program']])
            uses=[];aliases={}
            for root,consumers in sorted(record['memory'].items()):
                for consumer in consumers:
                    offset,expr,target,section,width=consumer[:5]
                    if len(uses)>=40:break
                    uses.append({'LUI_origin_offset':root*4,'consumer_offset':offset*4,'target':target,'access_width':width,
                        'mapped_section':section,'definition_mode':expr.mode,'completed_anchor':expr.anchor,
                        'consumer_delta_from_encoded_anchor':target-expr.anchor,
                        'target_alias':aliases.setdefault(target,len(aliases)),
                        'scalar_or_escape_blockers':sorted(record['blocked'].get(root,[])),
                        'original_object_base_verified':False,'normalization_acceptance_claimed':False})
                if len(uses)>=40:break
            witnesses.append(dict(public_member(member),mapped_effective_address_uses=uses,
                reachable_word_count=len(reachable),fullbody_words=len(words)))
        left,right=witnesses;rightmap={(u['LUI_origin_offset'],u['consumer_offset']):u for u in right['mapped_effective_address_uses']}
        relationships=[]
        for use in left['mapped_effective_address_uses']:
            other=rightmap.get((use['LUI_origin_offset'],use['consumer_offset']))
            if other is None:continue
            relationships.append({'LUI_origin_offset':use['LUI_origin_offset'],'consumer_offset':use['consumer_offset'],
                'left_target':use['target'],'right_target':other['target'],'target_delta':other['target']-use['target'],
                'left_encoded_anchor_addend':use['consumer_delta_from_encoded_anchor'],
                'right_encoded_anchor_addend':other['consumer_delta_from_encoded_anchor'],
                'same_encoded_addend':use['consumer_delta_from_encoded_anchor']==other['consumer_delta_from_encoded_anchor'],
                'original_object_relationship_verified':False})
        families.append({'rank':family['rank'],'size':family['size'],'witnesses':witnesses,'cross_program_operand_relationships':relationships})
    gp_records=[]
    for owner in audit['GP_attribution']['body_pinned_functions']:
        raw=body(owner);words=struct.unpack('<'+'I'*(len(raw)//4),raw);writes=[];stores=[];loads=[];calls=[]
        for i,word in enumerate(words):
            ins=rabbitizer.Instruction(word,owner['address']+i*4,rabbitizer.InstrCategory.R5900)
            if ins.isValid() and any(getattr(ins,'modifies'+suffix)() and getattr(ins,suffix.lower()).value==28 for suffix in ('Rs','Rt','Rd')):
                writes.append(i*4)
            if ins.isFunctionCall():calls.append(i*4)
            op=word>>26;rt=(word>>16)&31;rs=(word>>21)&31
            if op in norm.MEMORY and norm.MEMORY[op][1] and rt==28:
                site={'offset':i*4,'base_register':rs,'signed_displacement':norm.signed16(word&65535),'width':norm.MEMORY[op][2]}
                (stores if norm.MEMORY[op][0]=='store' else loads).append(site)
        if len(writes)!=owner['GP_write_count']:raise ValueError('Actual decoder GP write count drift')
        gp_records.append({k:v for k,v in owner.items() if k in ('program','address','size','raw_sha256','classification')}
            |{'actual_GP_write_offsets':writes,'GPR_GP_store_sites':stores,'GPR_GP_load_sites':loads,'call_offsets':calls,
              'entry_GP_verified':False,'call_GP_preservation_verified':False})
    negative=[]
    catalogue_rows={(r['program'],r['address'],r['size']):r for r in boundaries['functions']}
    for label,program,address,size in [('packet26','levels/0_aranos_tutorial',0x2f08d0,92),
        ('packet29','levels/0_aranos_tutorial',0x2f0930,92),('GS-Aranos','levels/0_aranos_tutorial',0x2e5890,136),
        ('GS-Oozla','levels/1_oozla',0x2dafd8,136)]:
        source=catalogue_rows[(program,address,size)]
        row=dict(source,boundary_tier=source['boundary']['status']);raw=body(row)
        normalized=norm.normalize(raw,address,prepared[program]);words=struct.unpack('<'+'I'*(size//4),raw)
        template=struct.unpack('<'+'I'*(size//4),normalized['template']);scalar_fields=[];GS_fields=[]
        for i,word in enumerate(words):
            op=word>>26
            if op in (9,25,13) and (word&65535) in (0x26,0x29):
                scalar_fields.append({'offset':i*4,'scalar_value':word&65535,'retained_exactly':word==template[i]})
            if op!=15:continue
            register=(word>>16)&31;high=(word&65535)<<16
            for j in range(i+1,min(i+17,len(words))):
                nxt=words[j];n_op=nxt>>26
                if n_op in (9,25,13) and ((nxt>>21)&31)==register:
                    value=high|(nxt&65535) if n_op==13 else (high+norm.signed16(nxt&65535))&0xffffffff
                    if value==0x3ff000:GS_fields.append({'high_offset':i*4,'low_offset':j*4,'scalar_value':value,
                        'fields_retained_exactly':words[i]==template[i] and words[j]==template[j]})
                if register in norm._uses_defs(nxt,None,prepared[program])[1]:break
        negative.append(dict(public_member(row),label=label,signature_sha256=normalized['signature_sha256'],
            packet_scalar_fields=scalar_fields,GS_literal_fields=GS_fields,
            exact_fullbody_reconstruction=normalized['certificate']['exact'],unresolved=normalized['unresolved']))
    if not all(f['retained_exactly'] for row in negative for f in row['packet_scalar_fields']):raise ValueError('Packet scalar field lost')
    if not all(f['fields_retained_exactly'] for row in negative for f in row['GS_literal_fields']):raise ValueError('GS scalar field lost')
    if negative[0]['signature_sha256']==negative[1]['signature_sha256']:raise ValueError('Packet counterexamples collapsed')
    original_symbols=[]
    for program,context in contexts.items():
        if program not in images:images[program]=ReferenceImage(args.references,program,context['reference_sha256'])
        data=images[program].data;offset=struct.unpack_from('<I',data,32)[0]
        stride,count=struct.unpack_from('<HH',data,46);objects=functions=tables=0
        if stride<40 or offset+stride*count>len(data):raise ValueError('Invalid original ELF section table')
        for i in range(count):
            section=struct.unpack_from('<10I',data,offset+i*stride)
            if section[1] not in (2,11):continue
            tables+=1
            if section[9]<16 or section[5]%section[9] or section[4]+section[5]>len(data):raise ValueError('Invalid original symbol table')
            for pos in range(section[4],section[4]+section[5],section[9]):
                symbol=struct.unpack_from('<IIIBBH',data,pos);kind=symbol[3]&15
                if kind==1 and symbol[2]>0:objects+=1
                if kind==2 and symbol[2]>0:functions+=1
        original_symbols.append({'program':program,'reference_sha256':context['reference_sha256'],
            'original_symbol_tables':tables,'sized_STT_OBJECT_symbols':objects,'sized_STT_FUNC_symbols':functions})
    report={'schema':1,'target':audit['target'],'input_pins':{'audit':sha(args.audit.read_bytes()),
        'boundaries':sha(args.boundaries.read_bytes()),'normalizer':sha(Path(norm.__file__).read_bytes()),
        'script':sha(Path(__file__).read_bytes())},'bounded_address_families':families,
        'original_ELF_object_metadata':original_symbols,'current_negative_controls':negative,
        'actual_GP_write_revalidation':{'decoder':'rabbitizer '+rabbitizer.__version__+' R5900',
            'functions':len(gp_records),'actual_GP_writes':sum(len(r['actual_GP_write_offsets']) for r in gp_records),
            'owners':gp_records,'new_verified_entries':0,'new_preservation_theorems':0},
        'limits':['Addresses/addends are machine-effective operand witnesses, not original allocation bases',
            'A structural same-slot save/restore lacks global-slot alias/lifetime and caller-entry provenance proofs',
            'Scratch GP mutation families remain concrete counterexamples to universal fixed-GP assumptions',
            'No raw words or disassembly are included in this structural report']}
    with args.output.open('x',encoding='utf8') as stream:json.dump(report,stream,separators=(',',':'))
    print('Witness families',len(families),'GP owners',len(gp_records),'actualwrites',report['actual_GP_write_revalidation']['actual_GP_writes'])
if __name__=='__main__':main()
