"""Derive metadata-only combined-reference boot bindings; no runtime theorem."""
import argparse
import hashlib
import json
from pathlib import Path
import sys
import struct

def digest(data):
    return hashlib.sha256(data).hexdigest()

def function_chunks_digest(chunks):
    """Pin unchanged function payload descriptors while allowing proof/header attachment."""
    return digest(json.dumps(chunks,sort_keys=True,separators=(',',':')).encode('utf-8'))

def overlap(address,size,other,other_size):
    return address < other+other_size and other < address+size

def section_mapping(elf,address,size):
    """Exact code ownership and matching file/memory segment mapping, not names."""
    owners=[s for s in elf['sections'] if s['type']==1 and s['flags']&6==6
            and s['address']<=address and address+size<=s['address']+s['size']]
    if len(owners)!=1:
        raise ValueError('Ambiguous or missing complete executable section')
    owner=owners[0]
    offset=owner['offset']+address-owner['address']
    segments=[s for s in elf['segments'] if s['type']==1 and s['flags']&1
              and s['address']<=address and address+size<=s['address']+s['filesz']
              and s['offset']+address-s['address']==offset]
    if len(segments)!=1:
        raise ValueError('Ambiguous or inconsistent executable file-backed PT_LOAD')
    return owner,segments[0],offset

def disjoint_overlay(elf,address,size):
    # Include zero-fill/memsz and every allocated section, not only .text.
    return not any(overlap(address,size,s['address'],s['memsz']) for s in elf['segments'] if s['type']==1) and not any(
        overlap(address,size,s['address'],s['size']) for s in elf['sections'] if s['flags']&2)

def direct_dependencies(body,address):
    """Independent opcode extraction, including COP0 branches; no role masking."""
    if not body or len(body)%4:raise ValueError('Complete aligned body required')
    result=[]
    for offset in range(0,len(body),4):
        word=struct.unpack_from('<I',body,offset)[0]
        op,rs,rt=word>>26,(word>>21)&31,(word>>16)&31
        if op in (2,3):
            target=((address+offset+4)&0xf0000000)|((word&0x03ffffff)<<2)
            kind='call' if op==3 else 'jump'
        elif (op in (4,5,6,7,20,21,22,23)
              or op==1 and rt in (0,1,2,3,16,17,18,19)
              or op in (0x10,0x11,0x12) and rs==8 and rt in (0,1,2,3)):
            immediate=word&0xffff
            signed=immediate-0x10000 if immediate&0x8000 else immediate
            target=address+offset+4+signed*4;kind='branch'
        else:continue
        if not address<=target<address+len(body):
            result.append({'offset':offset,'kind':kind,'target':target})
    return result

def pinned_body(image,elf,row):
    _,_,offset=section_mapping(elf,row['address'],row['size'])
    body=image[offset:offset+row['size']]
    if len(body)!=row['size'] or digest(body)!=row['raw_sha256']:
        raise ValueError('Complete reference body differs from catalogue')
    return body

def verify(repo,references,catalog_path):
    repo,references,catalog_path=map(lambda p:Path(p).resolve(),(repo,references,catalog_path))
    sys.path.insert(0,str(repo/'scripts'))
    import elf_tools
    import unique_code_report
    catalog,_=unique_code_report.read_catalog(catalog_path)
    decoder_sha=digest((repo/'scripts/relocation_identity.py').read_bytes())
    if catalog.get('normalizer',{}).get('sha256')!=decoder_sha:
        raise ValueError('Current normalization source differs from catalogue dependency')
    programs={p['program']:p for p in catalog['programs']}
    refs={};images={}
    for program,p in programs.items():
        path=references/('boot.elf' if program=='boot' else program+'/overlay.elf')
        data=path.read_bytes()
        if digest(data)!=p['reference_sha256']:
            raise ValueError('Changed pinned reference: '+program)
        images[program]=data;refs[program]=elf_tools._parse(data)
    boot=refs['boot']
    # Select the pinned boot core section once; ownership checks below do not
    # trust the label to establish mapping or preservation.
    sections=[s for s in boot['sections'] if s['name']=='core.text']
    if len(sections)!=1:
        raise ValueError('Missing unambiguous pinned core section selector')
    core=sections[0]
    section_mapping(boot,core['address'],core['size'])
    entries={(r['program'],r['address']):r for r in catalog['functions']}
    if len(entries)!=len(catalog['functions']):raise ValueError('Ambiguous programme entry')
    rows={};bindings=[];blocked=[];counts={}
    for program,elf in refs.items():
        if program=='boot':continue
        safe=disjoint_overlay(elf,core['address'],core['size'])
        counts[program]={'static_core_load_disjoint':safe,'bindings':0,'blocked':0}
        for row in (r for r in catalog['functions'] if r['program']==program):
            prospective=[e for e in row.get('call_dependencies',[]) if (program,e['target']) not in entries
                         and ('boot',e['target']) in entries and core['address']<=e['target']<core['address']+core['size']]
            if not prospective:continue
            original=pinned_body(images[program],elf,row)
            if direct_dependencies(original,row['address'])!=row.get('call_dependencies'):
                raise ValueError('Caller static dependency set differs from pinned instructions')
            for edge in row.get('call_dependencies',[]):
                target=edge['target']
                if (program,target) in entries or not core['address']<=target<core['address']+core['size']:continue
                callee=entries.get(('boot',target))
                if callee is None:continue
                try:
                    owner,segment,offset=section_mapping(boot,target,callee['size'])
                    if owner is not core or not safe or not disjoint_overlay(elf,target,callee['size']):
                        raise ValueError('Overlay load overlaps complete boot body')
                    body=images['boot'][offset:offset+callee['size']]
                    if len(body)!=callee['size'] or digest(body)!=callee['raw_sha256']:
                        raise ValueError('Boot body differs from pinned catalogue')
                    if callee['boundary']['status'] not in ('qualified_complete','flow_supported_inferred'):
                        raise ValueError('Boot boundary lacks structural support')
                    receipt={'caller_id':row['id'],'program':program,'offset':edge['offset'],
                             'caller_address':row['address'],'caller_size':row['size'],
                             'caller_raw_sha256':row['raw_sha256'],
                             'kind':edge['kind'],'target_program':'boot','target':target,
                             'target_id':callee['id'],'target_size':callee['size'],
                             'target_raw_sha256':callee['raw_sha256'],
                             'scope':'combined-pinned-reference-images','runtime_preservation_proven':False}
                    bindings.append(receipt);counts[program]['bindings']+=1
                    rows[callee['id']]={'id':callee['id'],'address':target,'size':callee['size'],
                                        'raw_sha256':callee['raw_sha256'],'boundary_status':callee['boundary']['status']}
                except ValueError as exc:
                    blocked.append({'program':program,'caller_id':row['id'],'target':target,'reason':str(exc)})
                    counts[program]['blocked']+=1
    return {'schema':1,'target':catalog['target'],'policy':'combined-pinned-reference-images',
            'runtime_preservation_proven':False,
            'catalog_sha256':digest(catalog_path.read_bytes()),
            'source_catalog_sha256':digest(catalog_path.read_bytes()),
            'function_chunks_sha256':function_chunks_digest(catalog['function_chunks']),
            'source_function_chunks_sha256':function_chunks_digest(catalog['function_chunks']),
            'function_chunks_hash_encoding':'sha256(utf8(json.dumps(function_chunks,sort_keys=True,separators=(comma,colon))))',
            'reader_sha256':digest((repo/'scripts/elf_tools.py').read_bytes()),
            'catalog_reader_sha256':digest((repo/'scripts/unique_code_report.py').read_bytes()),
            'decoder_source_sha256':decoder_sha,
            'decoder_pin_scope':'catalogue-normalization-dependency; direct edges independently decoded by this proof source',
            'dependency_policy':'independently-replayed-complete-direct-control-targets',
            'proof_source_sha256':digest(Path(__file__).read_bytes()),
            'programs':[{'program':p,'reference_sha256':programs[p]['reference_sha256'],
                         'load_segments':refs[p]['segments'],
                         'allocated_sections':[s for s in refs[p]['sections'] if s['flags']&2]} for p in sorted(programs)],
            'boot_core':core,'target_bodies':list(rows.values()),'per_program':counts,
            'bindings':bindings,'blocked':blocked,
            'limits':['PT_LOAD nonoverlap proves only static ELF mapping, not runtime preservation.',
                      'Boot executable segment is writable; loader, arbitrary stores, DMA and dynamic patching are not excluded.',
                      'No original-source, semantic-equivalence or inherited-GP claim is made.']}

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('repo','references','catalog','output'):parser.add_argument('--'+name,required=True,type=Path)
    args=parser.parse_args()
    if args.output.exists():raise ValueError('Preserve existing proof; output must be fresh')
    result=verify(args.repo,args.references,args.catalog)
    args.output.write_text(json.dumps(result,sort_keys=True,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({'bindings':len(result['bindings']),'blocked':len(result['blocked']),
                      'boot_bodies':len(result['target_bodies']),'programs':len(result['programs'])}))

if __name__=='__main__':main()
