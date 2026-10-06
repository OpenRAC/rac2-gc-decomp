"""Bounded, pinned-byte shared-family discovery audit; adds no code credit."""
import argparse
import collections
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import sys

def sha(data):return hashlib.sha256(data).hexdigest()

def operand_mask(word,relaxed):
    """Discovery-only mask. Scalar immediates are intentionally also erased."""
    op=word>>26;rs=(word>>21)&31;rt=(word>>16)&31
    if op in (2,3):return 0xfc000000
    if op==0 or op==0x1c:return 0xfc0007ff if relaxed else 0xffffffff
    if op in (0x10,0x11,0x12):return 0xffe0003f if relaxed else 0xffffffff
    if op==1:return 0xfc1f0000 if relaxed else 0xffff0000
    if op in (4,5,6,7,8,9,10,11,12,13,14,15,20,21,22,23,24,25,
              0x1a,0x1b,0x1e,0x1f,*range(0x20,0x40)):
        return 0xfc000000 if relaxed else 0xffff0000
    return 0xffffffff

def shape(body,relaxed):
    words=struct.unpack('<'+'I'*(len(body)//4),body)
    return sha(struct.pack('<'+'I'*len(words),*(w&operand_mask(w,relaxed) for w in words)))

def difference_categories(a,b,memory,left_address=None,right_address=None,size=None,offset=0):
    if a==b:return set()
    op=a>>26;other=b>>26;diff=a^b;tags=set()
    if op!=other:return {'instruction_selection'}
    if op in (2,3):
        if left_address is None:tags.add('static_control_target')
        else:
            left=((left_address+offset+4)&0xf0000000)|((a&0x03ffffff)<<2)
            right=((right_address+offset+4)&0xf0000000)|((b&0x03ffffff)<<2)
            internal=(left_address<=left<left_address+size and right_address<=right<right_address+size)
            tags.add('internal_control_rebase_only' if internal and left-left_address==right-right_address
                     else 'external_static_control_target' if not internal else 'internal_control_destination_difference')
    elif diff&0xffff:
        if op==15:tags.add('LUI_HI16_value')
        elif op in memory:
            tags.add('GP_memory_displacement' if ((a>>21)&31)==28 and ((b>>21)&31)==28 else 'memory_displacement_or_object_addend')
        elif op in (9,25,13):tags.add('LO_address_or_scalar_immediate')
        elif op in (1,4,5,6,7,20,21,22,23):tags.add('relative_control_displacement')
        else:tags.add('scalar_or_instruction_immediate')
    if op not in (2,3) and (diff&0x03ff0000 or op in (0,0x1c) and diff&0xf800):tags.add('register_operand_difference')
    if not tags:tags.add('instruction_subfield')
    return tags

def reason_tags(reason):
    result=set()
    for name,phrases in {
        'unproved_GP':['Real GP value/lifetime'],
        'bare_LUI_missing_exact_data_symbol':['Bare LUI memory base/LO'],
        'callee_argument_or_target':['call argument','tail-transfer argument','J/JAL target','indirect control target'],
        'scalar_or_escaped_value':['scalar','return value','stored value','stored to non-stack','COP1/COP2'],
        'unmapped_memory_target':['memory target is not mapped'],
        'CFG_or_decoder_unresolved':['CFG','delay','REGIMM','unsupported instruction'],
    }.items():
        if any(phrase in reason for phrase in phrases):result.add(name)
    if not result:result.add('other_unresolved')
    return result

def public_member(row):
    return {k:row[k] for k in ('program','address','size','raw_sha256','boundary_tier')}

def run(args):
    sys.path.insert(0,str(args.repo/'scripts'))
    import relocation_identity as norm
    from pointer_evidence_loader import ReferenceImage,load_pointer_evidence
    catalog_bytes=args.catalog.read_bytes();catalog=json.loads(catalog_bytes)
    boundaries=json.loads(args.boundaries.read_bytes())
    if sha(Path(norm.__file__).read_bytes())!=catalog['normalizer']['sha256']:raise ValueError('Current normalizer pin differs from catalogue')
    contexts={c['program']:dict(c) for c in boundaries['contexts']}
    pointers=load_pointer_evidence(args.pointer_evidence,boundaries,args.references,norm)
    function_pins=collections.defaultdict(dict)
    for row in boundaries['functions']:
        if row['boundary']['status'] in ('qualified_complete','flow_supported_inferred'):
            function_pins[row['program']][row['address']]={'size':row['size'],'raw_sha256':row['raw_sha256'],'boundary_tier':row['boundary']['status']}
    prepared={}
    for program,context in contexts.items():
        if context.get('gp_verified') or context.get('data_symbols'):raise ValueError('Audit requires unchanged GP-unknown/no-original-symbol contexts')
        context['function_pins']=function_pins[program];context['pointer_argument_roles']=pointers.get(program,[])
        prepared[program]=norm.prepare_context(context)
    del boundaries,pointers,function_pins
    rows=[];groups=collections.defaultdict(list);strict=collections.defaultdict(list);images={}
    checked=0;checked_bytes=0;direct_data_template_rows=0;direct_data_template_bytes=0
    for chunk in catalog['function_chunks']:
        packed=(args.catalog.parent/chunk['path']).read_bytes()
        if sha(packed)!=chunk['sha256']:raise ValueError('Chunk compressed pin changed')
        expanded=gzip.decompress(packed)
        if sha(expanded)!=chunk['expanded_sha256']:raise ValueError('Chunk expanded pin changed')
        program=chunk['program'];pin=contexts[program]['reference_sha256']
        image=ReferenceImage(args.references,program,pin)
        for line in expanded.splitlines():
            item=json.loads(line);address,size,raw,tier,signature,relocations=item[:6]
            body=image.body(address,size,raw);checked+=1;checked_bytes+=size
            data_records=[r for r in relocations if isinstance(r,dict) and r['kind']=='hi16_lo16']
            if data_records:direct_data_template_rows+=1;direct_data_template_bytes+=size
            row={'program':program,'address':address,'size':size,'raw_sha256':raw,'boundary_tier':tier,
                'normalizer_signature':signature,'provisional_data_fields':len(data_records),
                'strict_shape':shape(body,False),'relaxed_shape':shape(body,True)}
            rows.append(row)
            if size>=args.minimum_size:
                groups[(size,row['relaxed_shape'])].append(row);strict[(size,row['strict_shape'])].append(row)
        print('Pinned byte scan',program,checked,flush=True)
    candidates=[members for members in groups.values() if len(set(r['program'] for r in members))>=args.minimum_programs]
    candidates.sort(key=lambda members:(len(members)*members[0]['size'],members[0]['size']),reverse=True)
    selected=candidates[:args.top];reason_weight=collections.Counter();diff_weight=collections.Counter();cases=[]
    member_keys=set();selected_bytes=0
    for rank,members in enumerate(selected,1):
        members=sorted(members,key=lambda row:(row['program']!='boot',row['program'],row['address']))
        representative=members[0];body_cache={};normalized={};reasons={};gp_accesses={};gp_writes={}
        for row in members:
            program=row['program'];key=(program,row['address']);member_keys.add(key);selected_bytes+=row['size']
            if program not in images:images[program]=ReferenceImage(args.references,program,contexts[program]['reference_sha256'])
            body=images[program].body(row['address'],row['size'],row['raw_sha256']);body_cache[key]=body
            result=norm.normalize(body,row['address'],prepared[program])
            if not result['certificate']['exact'] or result['certificate']['reconstructed_sha256']!=row['raw_sha256']:raise ValueError('Full-body reconstruction control failed')
            normalized[key]=result;tags=set()
            for item in result['unresolved']:tags.update(reason_tags(item['reason']))
            if any(r['kind']=='hi16_lo16' for r in result['relocations']):tags.add('unowned_data_template_retained_by_primary_policy')
            reasons[key]=tags
            for tag in tags:reason_weight[tag]+=row['size']
            words=struct.unpack('<'+'I'*(row['size']//4),body)
            gp_accesses[key]=[i*4 for i,w in enumerate(words) if w>>26 in norm.MEMORY and ((w>>21)&31)==28]
            gp_writes[key]=[i*4 for i,w in enumerate(words) if 28 in norm._uses_defs(w,None,prepared[program])[1]]
        basekey=(representative['program'],representative['address']);basewords=struct.unpack('<'+'I'*(representative['size']//4),body_cache[basekey])
        pairs=[];familydiff=collections.Counter();sample_targets=[]
        for row in members[1:]:
            key=(row['program'],row['address']);words=struct.unpack('<'+'I'*len(basewords),body_cache[key]);categories=collections.Counter();differing=0
            for i,(a,b) in enumerate(zip(basewords,words)):
                if a==b:continue
                differing+=1
                for tag in difference_categories(a,b,norm.MEMORY,representative['address'],row['address'],row['size'],i*4):categories[tag]+=1;familydiff[tag]+=1;diff_weight[tag]+=4
            pairs.append({'left':public_member(representative),'right':public_member(row),'different_words':differing,
                'different_word_bytes':differing*4,'overlapping_categories_words':dict(categories),
                'raw_equal':row['raw_sha256']==representative['raw_sha256'],
                'register_strict_shape_equal':row['strict_shape']==representative['strict_shape']})
        case={'rank':rank,'discovery_shape_sha256':representative['relaxed_shape'],
            'size':representative['size'],'placements':len(members),'programs':len(set(r['program'] for r in members)),
            'placement_bytes':sum(r['size'] for r in members),'raw_hash_classes':len(set(r['raw_sha256'] for r in members)),
            'strict_register_shape_classes':len(set(r['strict_shape'] for r in members)),
            'stored_normalizer_signature_classes':len(set(r['normalizer_signature'] for r in members)),
            'members':[public_member(r) for r in members],'pairs':pairs,'overlapping_difference_words':dict(familydiff),
            'member_unresolved_categories':[dict(public_member(r),categories=sorted(reasons[(r['program'],r['address'])]),
                GP_memory_site_count=len(gp_accesses[(r['program'],r['address'])]),
                conservative_GPR_GP_write_count=len(gp_writes[(r['program'],r['address'])])) for r in members],
            'exact_fullbody_pin_and_reconstruction_controls':len(members),
            'identity_verified':False,'missing_proof':['Original data object base/boundaries and shared ownership',
                'Per-entry GP provenance and caller/callee lifetime where GP memory occurs',
                'Scalar constants and retained addends must remain distinguished',
                'Static callee graph classes and register allocation differences are not relocation equivalence'],
        }
        # A bounded structural witness bank: addresses/addends only, no words.
        for row in members[:2]:
            key=(row['program'],row['address']);result=normalized[key]
            case.setdefault('bounded_witnesses',[]).append(dict(public_member(row),
                unresolved=result['unresolved'][:12],
                data_relocations=[r for r in result['relocations'] if r['kind']=='hi16_lo16'][:12],
                GP_memory_offsets=gp_accesses[key][:20],conservative_GP_write_offsets=gp_writes[key][:20]))
        cases.append(case);print('Family',rank,case['size'],case['placements'],case['placement_bytes'],flush=True)
    gp=json.loads(args.gp_evidence.read_bytes());gpcases=[]
    index={(r['program'],r['address']):r for r in rows}
    for record in gp['functions']:
        row=index.get((record['program'],record['address']))
        if row is None or row['size']!=record['size'] or row['raw_sha256']!=record['raw_sha256']:raise ValueError('GP evidence body pin differs from current catalogue')
        program=row['program']
        if program not in images:images[program]=ReferenceImage(args.references,program,contexts[program]['reference_sha256'])
        images[program].body(row['address'],row['size'],row['raw_sha256'])
        gpcases.append({k:v for k,v in record.items() if not k.startswith('private_')})
    report={'schema':1,'target':catalog['target'],'kind':'data-GP-suspected-family-audit',
        'input_pins':{'catalog':sha(catalog_bytes),'boundaries':sha(args.boundaries.read_bytes()),
            'pointer_evidence':sha(args.pointer_evidence.read_bytes()),'GP_evidence':sha(args.gp_evidence.read_bytes()),
            'normalizer':catalog['normalizer']['sha256'],'script':sha(Path(__file__).read_bytes())},
        'full_byte_controls':{'placements':checked,'placement_bytes':checked_bytes,'all_raw_hashes_verified':True},
        'discovery':{'method':'Equal-sized instruction-selector shape with register operands and all immediates erased; discovery only, including scalar erasure',
            'minimum_size':args.minimum_size,'minimum_programs':args.minimum_programs,'candidate_families':len(candidates),
            'selected_families':len(cases),'selected_placements':len(member_keys),'selected_placement_bytes':selected_bytes},
        'overlapping_byte_weighted_unresolved_categories':dict(reason_weight),
        'overlapping_actual_different_word_bytes':dict(diff_weight),
        'unowned_data_global_metadata':{'rows_with_provisional_HILO_templates':direct_data_template_rows,
            'overlapping_row_bytes':direct_data_template_bytes,'original_allocations_verified':0},
        'families':cases,'GP_attribution':{'boot_initialization':gp['boot_initialization'],
            'body_pinned_functions':gpcases,'all_contexts_GP_verified':False,'new_verified_GP_entries':0},
        'limits':['No code matching credit or identity claim','Byte-weighted unresolved categories count each whole member once per category and overlap',
            'Difference categories count the full 4-byte differing word per applicable tag and overlap',
            'All broad discovery masks are excluded from the accepted normalization pipeline',
            'Conservative uses_defs GP writes can include unsupported-instruction kills; independently pinned GP evidence supplies actual write counts',
            'Synthetic current-C typed bindings are not original allocation proofs']}
    with args.output.open('x',encoding='utf8') as stream:json.dump(report,stream,separators=(',',':'))

def main():
    p=argparse.ArgumentParser(description=__doc__)
    for name in ('repo','catalog','boundaries','references','pointer-evidence','gp-evidence','output'):p.add_argument('--'+name,type=Path,required=True)
    p.add_argument('--top',type=int,default=20);p.add_argument('--minimum-size',type=int,default=256);p.add_argument('--minimum-programs',type=int,default=5)
    args=p.parse_args()
    if args.output.exists():p.error('Fresh output path required')
    run(args)
if __name__=='__main__':main()
