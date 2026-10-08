"""Derive link metadata for reviewed resident imports; never change compiled code."""
from __future__ import annotations
import hashlib, json, re, struct
from pathlib import Path
from elf_tools import _parse, address_bytes

def _need(ok,message):
    if not ok: raise ValueError(message)
def _sha(data): return hashlib.sha256(data).hexdigest()

def adapt_shared_imports(data, placed, externals, boot_functions, boot_review, provider_union, reference_target):
    """Return a derived ET_REL and honest receipt; reference_target(address) decodes JAL."""
    parsed=_parse(data); _need(parsed['type']==1,'Shared import adaptation needs ELF32 MIPS ET_REL')
    header=struct.unpack_from('<16sHHIIIIIHHHHHH',data)
    base,stride,count,names=header[6],header[11],header[12],header[13]
    _need(stride==40 and count>0 and names<count,'Extended/unsupported object section table')
    raw=[struct.unpack_from('<10I',data,base+i*stride)for i in range(count)]
    strings=data[raw[names][4]:raw[names][4]+raw[names][5]]
    def string(blob,index):
        _need(index<len(blob),'Symbol/section string out of bounds'); end=blob.find(b'\0',index); _need(end>=index,'Unterminated object string'); return blob[index:end].decode('ascii')
    sections=[dict(index=i,name=string(strings,h[0]),type=h[1],flags=h[2],offset=h[4],size=h[5],link=h[6],info=h[7],entry_size=h[9])for i,h in enumerate(raw)]
    tables={}
    for s in sections:
        if s['type']!=2:continue
        _need(s['entry_size']==16 and s['size']%16==0 and s['link']<count,'Malformed symbol table')
        st=sections[s['link']]; _need(st['type']==3,'Symbol table lacks string table'); blob=data[st['offset']:st['offset']+st['size']]; table=[]
        for off in range(s['offset'],s['offset']+s['size'],16):
            name,value,size,info,other,ndx=struct.unpack_from('<IIIBBH',data,off)
            table.append(dict(index=len(table),name=string(blob,name),value=value,size=size,type=info&15,binding=info>>4,other=other,section=ndx))
        tables[s['index']]=table
    owners={'.text.'+f['symbol']:f for f in placed}; _need(len(owners)==len(placed),'Duplicate placed function')
    boot={f['symbol']:f for f in boot_functions}; reviewed={f['symbol']:f for f in boot_review}; _need(len(boot)==len(boot_functions) and len(reviewed)==len(boot_review),'Duplicate boot provider')
    result=bytearray(data); changes=[]
    for rel in sections:
        if rel['type']not in (4,9):continue
        _need(rel['info']<count,'Relocation owner out of bounds'); caller=sections[rel['info']]
        if caller['name']not in owners:continue
        _need(rel['type']==9 and rel['entry_size']==8 and rel['size']%8==0 and rel['link']in tables,'Unsupported placed relocation table')
        _need(caller['type']==1 and caller['flags']&6==6 and caller['size']==owners[caller['name']]['size'],'Placed function section extent differs')
        table=tables[rel['link']]
        for pos in range(rel['offset'],rel['offset']+rel['size'],8):
            offset,info=struct.unpack_from('<II',data,pos); _need(info>>8<len(table),'Relocation symbol out of bounds'); target=table[info>>8]
            if target['type']!=3:continue
            _need(target['section']<count,'Section relocation target out of bounds'); provider=sections[target['section']]
            # Only this exact wildcard is discarded by the maintained shared link.
            if not provider['name'].startswith('.text.FUN_') or provider['name']in owners:continue
            name=provider['name'][6:]
            _need(name in externals and name in boot and name in reviewed,'Unproven discarded provider import: '+name)
            def overlaps_loaded(value):
                address=value if type(value)is int else value.get('address')if isinstance(value,dict)else None
                size=value.get('size',1)if isinstance(value,dict)else 1
                return type(address)is int and type(size)is int and address<externals[name]+boot[name]['size'] and externals[name]<address+size
            _need(name not in provider_union and not any(overlaps_loaded(value)for value in provider_union.values()),'Discarded boot provider conflicts with complete loaded-owner union: '+name)
            fact=boot[name]; proof=reviewed[name]
            _need(externals[name]==fact['address'] and proof.get('address')==fact['address'] and proof.get('size')==fact['size'],'Provider binding/extent conflicts with reviewed boot owner')
            _need(proof.get('matched')is True and proof.get('different_bytes')==0 and re.fullmatch('[0-9a-f]{64}',proof.get('reference_sha256','')) and proof['reference_sha256']==proof.get('candidate_sha256'),'Provider lacks complete unmasked boot proof')
            globals_=[s for s in table if s['name']==name and s['type']==2 and s['binding']==1 and s['section']==provider['index']]
            _need(len(globals_)==1,'Provider lacks one existing global function symbol'); global_=globals_[0]
            _need(provider['type']==1 and provider['flags']&6==6 and target['binding']==0 and target['value']==0 and global_['value']==0 and global_['size']==provider['size']==fact['size'],'Unsupported provider symbol/section shape')
            _need(info&255==4,'Unsupported relocation to discarded reviewed provider')
            _need(offset%4==0 and offset+4<=caller['size'],'Provider call offset out of bounds')
            word=struct.unpack_from('<I',data,caller['offset']+offset)[0]
            _need(word==0x0c000000,'Only zero-addend direct JAL entry imports can be adapted')
            address=owners[caller['name']]['address']+offset
            _need(reference_target(address)==externals[name],'Reference call does not prove the existing external binding')
            replacement=(global_['index']<<8)|4; struct.pack_into('<I',result,pos+4,replacement)
            changes.append(dict(caller=owners[caller['name']]['symbol'],caller_address=address,provider=name,provider_address=externals[name],provider_size=fact['size'],provider_reference_sha256=proof['reference_sha256'],old_symbol_index=target['index'],new_symbol_index=global_['index'],r_info_offset=pos+4,old_r_info=info,new_r_info=replacement))
    allowed={p for c in changes for p in range(c['r_info_offset'],c['r_info_offset']+4)}
    _need(len(result)==len(data) and all(a==b or i in allowed for i,(a,b)in enumerate(zip(data,result))),'Adapter changed non-relocation bytes')
    _parse(bytes(result))
    sections_preserved=[]
    for s in sections:
        if s['type']in (0,8,9):continue
        before=data[s['offset']:s['offset']+s['size']]; after=result[s['offset']:s['offset']+s['size']]
        _need(before==after,'Adapter changed section/code/symbol bytes: '+s['name']); sections_preserved.append(dict(index=s['index'],name=s['name'],sha256=_sha(before)))
    receipt=dict(schema=1,kind='reviewed-shared-import-relocation-adapter',compiled_object_sha256=_sha(data),link_object_sha256=_sha(result),relocation_changes=changes,unchanged_sections=sections_preserved,compiled_code_unchanged=True,symbol_table_unchanged=True,new_physical_bytes=0,integration_credit=0)
    return bytes(result),receipt

def derive_shared_import_object(original,destination,receipt_path,catalog,boot,review,reference,provider_union):
    original,destination,receipt_path=map(Path,(original,destination,receipt_path)); data=original.read_bytes()
    _need(_sha(data)==review['object_sha256'],'Compiled boot object differs from original review')
    def reference_target(address):
        word=struct.unpack('<I',address_bytes(reference,address,4))[0]
        _need(word>>26==3,'Reference provider edge is not direct JAL')
        return ((address+4)&0xf0000000)|((word&0x3ffffff)<<2)
    result,receipt=adapt_shared_imports(data,catalog['functions'],catalog['externals'],boot['functions'],review['functions'],provider_union,reference_target)
    _need(not destination.exists() and not receipt_path.exists(),'Never overwrite link-adapter evidence')
    destination.parent.mkdir(parents=True,exist_ok=True)
    with destination.open('xb')as stream:stream.write(result)
    with receipt_path.open('xb')as stream:stream.write((json.dumps(receipt,sort_keys=True,indent=2)+'\n').encode())
    _need(original.read_bytes()==data,'Original compiled object changed during adaptation')
    return receipt

def validate_import_owner_union(receipt,definitions):
    """Recheck the actual final owner union; an import must never shadow a definition."""
    for edge in receipt['relocation_changes']:
        addresses=[v if type(v)is int else v.get('address')for v in definitions.values()if type(v)is int or isinstance(v,dict)]
        _need(edge['provider'] not in definitions and edge['provider_address']not in addresses,'Adapted resident import became a loaded C definition: '+edge['provider'])

def validate_shared_import_receipt(receipt,compiled_object_sha256,link_object_sha256):
    """Validate public structural adapter proof; binary/full-image gates remain separate."""
    is_hash=lambda value:isinstance(value,str) and re.fullmatch('[0-9a-f]{64}',value) is not None
    _need(isinstance(receipt,dict) and receipt.get('schema')==1 and receipt.get('kind')=='reviewed-shared-import-relocation-adapter','Unknown shared import receipt')
    _need(is_hash(compiled_object_sha256) and is_hash(link_object_sha256) and receipt.get('compiled_object_sha256')==compiled_object_sha256 and receipt.get('link_object_sha256')==link_object_sha256,'Shared raw/link object identity differs')
    _need(receipt.get('compiled_code_unchanged')is True and receipt.get('symbol_table_unchanged')is True and type(receipt.get('new_physical_bytes'))is int and receipt['new_physical_bytes']==0 and type(receipt.get('integration_credit'))is int and receipt['integration_credit']==0,'Shared import adaptation cannot change code/symbols or add credit')
    changes=receipt.get('relocation_changes'); sections=receipt.get('unchanged_sections')
    _need(isinstance(changes,list) and isinstance(sections,list) and sections,'Missing relocation/section preservation proof')
    seen=set()
    for edge in changes:
        _need(isinstance(edge,dict),'Malformed import edge')
        for name in ('caller','provider'):_need(isinstance(edge.get(name),str) and re.fullmatch(r'FUN_[0-9A-F]{8}',edge[name]),'Unreviewed function identity')
        for name in ('caller_address','provider_address','provider_size','old_symbol_index','new_symbol_index','r_info_offset','old_r_info','new_r_info'):_need(type(edge.get(name))is int and 0<=edge[name]<1<<32,'Invalid relocation receipt integer')
        _need(edge['caller_address']%4==edge['provider_address']%4==edge['provider_size']%4==edge['r_info_offset']%4==0 and edge['provider_size']>0,'Unaligned import edge')
        _need(edge['old_r_info']==(edge['old_symbol_index']<<8)|4 and edge['new_r_info']==(edge['new_symbol_index']<<8)|4 and edge['old_symbol_index']!=edge['new_symbol_index'],'Adapter must only remap R_MIPS_26 symbol identity')
        _need(is_hash(edge.get('provider_reference_sha256')) and edge['r_info_offset']not in seen,'Missing provider body proof/duplicate relocation');seen.add(edge['r_info_offset'])
    indices=set()
    for section in sections:
        _need(isinstance(section,dict) and type(section.get('index'))is int and section['index']>0 and section['index']not in indices and isinstance(section.get('name'),str) and not any(c in section['name']for c in ('/','\\',':','\0')) and is_hash(section.get('sha256')),'Malformed unchanged-section proof');indices.add(section['index'])
    _need(bool(changes) or compiled_object_sha256==link_object_sha256,'Changed link object has no recorded relocation change')
    return receipt
