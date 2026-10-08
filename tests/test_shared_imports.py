"""Synthetic ELF mutation tests: no game bytes, toolchain or compiler needed."""
import copy, hashlib, struct, unittest, sys, json, tempfile
from unittest import mock
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'scripts'))
from shared_imports import adapt_shared_imports, validate_import_owner_union, validate_shared_import_receipt

def fixture():
    caller='FUN_00001000'; provider='FUN_00002000'
    section_names=['','.text.'+caller,'.text.'+provider,'.rel.text.'+caller,'.symtab','.strtab','.shstrtab']
    shstrings=b'\0'; offsets={}
    for name in section_names[1:]:offsets[name]=len(shstrings); shstrings+=name.encode()+b'\0'
    strings=b'\0'+caller.encode()+b'\0'+provider.encode()+b'\0'; cn=1;pn=2+len(caller)
    symbol=lambda name,value,size,info,section:struct.pack('<IIIBBH',name,value,size,info,0,section)
    syms=symbol(0,0,0,0,0)+symbol(0,0,0,3,1)+symbol(0,0,0,3,2)+symbol(cn,0,12,18,1)+symbol(pn,0,8,18,2)
    chunks=[b'',struct.pack('<III',0x0c000000,0x03e00008,0),struct.pack('<II',0x03e00008,0),struct.pack('<II',0,(2<<8)|4),syms,strings,shstrings]
    blob=bytearray(52); locations=[0]*7
    for i in range(1,7):
        while len(blob)%4:blob+=b'\0'
        locations[i]=len(blob);blob+=chunks[i]
    while len(blob)%4:blob+=b'\0'
    shoff=len(blob);headers=[(0,0,0,0,0,0,0,0,0,0)]
    for i in range(1,7):
        typ,flags,link,info,align,entry={1:(1,6,0,0,4,0),2:(1,6,0,0,4,0),3:(9,0,4,1,4,8),4:(2,0,5,3,4,16),5:(3,0,0,0,1,0),6:(3,0,0,0,1,0)}[i]
        headers.append((offsets[section_names[i]],typ,flags,0,locations[i],len(chunks[i]),link,info,align,entry))
    for h in headers:blob+=struct.pack('<10I',*h)
    ident=b'\x7fELF\x01\x01\x01'+b'\0'*9;struct.pack_into('<16sHHIIIIIHHHHHH',blob,0,ident,1,8,1,0,0,shoff,0,52,0,0,40,7,6)
    placed=[dict(symbol=caller,address=0x1000,size=12)];boot=[dict(symbol=provider,address=0x2000,size=8)]
    proof=[dict(symbol=provider,address=0x2000,size=8,matched=True,different_bytes=0,reference_sha256='a'*64,candidate_sha256='a'*64)]
    return bytes(blob),placed,{provider:0x2000},boot,proof,{},lambda address:0x2000,locations

class SharedImportMutations(unittest.TestCase):
    def test_only_relocation_info_changes_and_original_is_preserved(self):
        args=fixture();data=args[0];result,receipt=adapt_shared_imports(*args[:7]); changes=receipt['relocation_changes']
        self.assertEqual(len(changes),1);self.assertEqual(changes[0]['new_symbol_index'],4)
        allowed=set(range(args[7][3]+4,args[7][3]+8));self.assertTrue(all(a==b or i in allowed for i,(a,b)in enumerate(zip(data,result))))
        self.assertTrue(receipt['compiled_code_unchanged']);self.assertTrue(receipt['symbol_table_unchanged']);self.assertEqual(receipt['compiled_object_sha256'],hashlib.sha256(data).hexdigest())
        self.assertEqual(receipt['integration_credit'],0)
        validate_shared_import_receipt(receipt,receipt['compiled_object_sha256'],receipt['link_object_sha256'])
        validate_import_owner_union(receipt,{})
        with self.assertRaises(ValueError):validate_import_owner_union(receipt,{'FUN_00002000':0x2000})
    def test_nonzero_addend_and_unsupported_relocation_fail_closed(self):
        for kind in ('addend','reloc'):
            args=list(fixture());raw=bytearray(args[0]);struct.pack_into('<I',raw,args[7][1]if kind=='addend'else args[7][3]+4,0x0c000001 if kind=='addend'else(2<<8)|5);args[0]=bytes(raw)
            with self.assertRaises(ValueError):adapt_shared_imports(*args[:7])
    def test_unknown_or_conflicting_external_and_provider_union_fail(self):
        for externals,union in [({},{}),({'FUN_00002000':0x2004},{}),({'FUN_00002000':0x2000},{'FUN_00002000':{'address':0x2000,'source':'candidates/levels/other.c'}}),({'FUN_00002000':0x2000},{'L_other':{'address':0x1ffc,'size':8}})]:
            args=list(fixture());args[2]=externals;args[5]=union
            with self.assertRaises(ValueError):adapt_shared_imports(*args[:7])
    def test_provider_incomplete_proof_and_wrong_reference_call_fail(self):
        for kind in ('proof','reference'):
            args=list(fixture())
            if kind=='proof':args[4]=copy.deepcopy(args[4]);args[4][0]['candidate_sha256']='b'*64
            else:args[6]=lambda address:0x2004
            with self.assertRaises(ValueError):adapt_shared_imports(*args[:7])
    def test_owned_provider_is_not_rebound_as_external(self):
        args=list(fixture());args[1]=args[1]+args[3];result,receipt=adapt_shared_imports(*args[:7]);self.assertEqual(result,args[0]);self.assertEqual(receipt['relocation_changes'],[])
    def test_symbol_and_function_extent_mutations_fail(self):
        for kind in ('symbol','extent'):
            args=list(fixture())
            if kind=='symbol':raw=bytearray(args[0]);struct.pack_into('<H',raw,args[7][4]+4*16+14,1);args[0]=bytes(raw)
            else:args[3]=copy.deepcopy(args[3]);args[3][0]['size']=12
            with self.assertRaises(ValueError):adapt_shared_imports(*args[:7])
    def test_public_receipt_rejects_wrong_raw_link_code_credit_or_edge(self):
        args=fixture();_,receipt=adapt_shared_imports(*args[:7])
        for key,value in [('compiled_object_sha256','0'*64),('link_object_sha256','0'*64),('compiled_code_unchanged',False),('symbol_table_unchanged',False),('integration_credit',1),('new_physical_bytes',8)]:
            wrong=copy.deepcopy(receipt);wrong[key]=value
            with self.assertRaises(ValueError):validate_shared_import_receipt(wrong,receipt['compiled_object_sha256'],receipt['link_object_sha256'])
        wrong=copy.deepcopy(receipt);wrong['relocation_changes'][0]['new_r_info']+=1
        with self.assertRaises(ValueError):validate_shared_import_receipt(wrong,receipt['compiled_object_sha256'],receipt['link_object_sha256'])
    def test_actual_shared_qualifier_links_derived_and_proves_raw_object(self):
        import integration, shared_imports
        digest=lambda data:hashlib.sha256(data).hexdigest()
        with tempfile.TemporaryDirectory()as folder:
            root=Path(folder); (root/'config').mkdir();(root/'progress').mkdir();(root/'candidates').mkdir()
            source=b'void FUN_00001000(void) {}\n';(root/'candidates/boot.c').write_bytes(source)
            reference=root/'reference.elf';reference.write_bytes(b'synthetic reference fixture')
            function=dict(symbol='FUN_00001000',address=0x1000,size=8)
            boot=dict(target='fixture',reference_sha256=digest(b'boot'),flags=['-O2','-G0','-ffunction-sections'],functions=[function])
            (root/'config/candidate-catalog.json').write_text(json.dumps(boot))
            (root/'config/level-catalog.json').write_text('{}')
            (root/'scripts').mkdir();(root/'scripts/check_candidates.py').write_text('# synthetic checker identity fixture\n')
            review=dict(target='fixture',reference_sha256=boot['reference_sha256'],source_sha256=digest(source),catalog_sha256=digest((root/'config/candidate-catalog.json').read_bytes()),flags=boot['flags'],functions=[dict(function,matched=True)],tools={'cc1':'a'*64},object_sha256=digest(b'raw compiled object'))
            (root/'progress/candidates.json').write_text(json.dumps(review))
            shared=dict(target='fixture',reference_sha256=digest(reference.read_bytes()),flags=boot['flags'],functions=[function],externals={})
            directory=root/'private';directory.mkdir();cdir=directory/'build/c';cdir.mkdir(parents=True)
            snapshot=cdir/'boot.c';snapshot.write_bytes(source);raw=cdir/'boot.c.o';raw.write_bytes(b'raw compiled object')
            def derive(original,destination,receipt_path,*args):
                self.assertEqual(original,raw);destination.write_bytes(b'derived link object')
                receipt=dict(compiled_object_sha256=digest(raw.read_bytes()),link_object_sha256=digest(destination.read_bytes()))
                receipt_path.write_text(json.dumps(receipt));return receipt
            calls=[]
            def link(arguments,log):
                calls.append(arguments);Path(arguments[arguments.index('-o')+1]).write_bytes(b'linked fixture')
            with mock.patch.object(integration,'ROOT',root),mock.patch.object(integration,'level_catalog',return_value=shared),mock.patch.object(integration,'tool_hashes',return_value=review['tools']),mock.patch.object(integration,'compile_snapshot',return_value=(snapshot,raw)),mock.patch.object(integration,'_shared_provider_union',return_value={}),mock.patch.object(shared_imports,'derive_shared_import_object',side_effect=derive),mock.patch.object(integration,'run',side_effect=link),mock.patch.object(integration,'assert_fresh'),mock.patch.object(integration,'compare_function',return_value=dict(function,matched=True)):
                catalog,returned,tools=integration._compile_shared_level_c(reference,directory,root/'tools','fixture')
            derived=cdir/'boot-shared-link.o';proof=json.loads((directory/'level-object-qualification.json').read_bytes())
            self.assertEqual(returned,raw);self.assertEqual(calls[0][-1],str(derived));self.assertEqual(proof['object_sha256'],review['object_sha256']);self.assertEqual(proof['link_object_sha256'],digest(derived.read_bytes()))
            self.assertEqual(proof['link_object_adapter_sha256'],digest((cdir/'boot-shared-link-adapter.json').read_bytes()))
            self.assertEqual(catalog['shared_link_object']['compiled_path'],'build/c/boot.c.o')

if __name__=='__main__':unittest.main()
