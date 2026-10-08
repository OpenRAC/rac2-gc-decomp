"""Shared link-view proof wiring/tamper tests; no compiler, linker or game bytes."""
import ast,copy,hashlib,importlib.util,json,os,struct,sys,tempfile,types,unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path
from unittest import mock
ROOT=Path(__file__).resolve().parents[1]
REPO=Path(os.environ.get('RAC2_PROOF_TEST_ROOT',ROOT))
spec=importlib.util.spec_from_file_location('shared_link_private_report',ROOT/'scripts/decomp_report.py');report=importlib.util.module_from_spec(spec);spec.loader.exec_module(report)

def sha(data):return hashlib.sha256(data).hexdigest()
def encoded(value):return(json.dumps(value,sort_keys=True,indent=2)+'\n').encode()
def receipt(raw,linked,caller='Caller',address=0x1000):
 return {'schema':1,'kind':'reviewed-shared-import-relocation-adapter','compiled_object_sha256':sha(raw),'link_object_sha256':sha(linked),'compiled_code_unchanged':True,'symbol_table_unchanged':True,'new_physical_bytes':0,'integration_credit':0,'unchanged_sections':[{'index':1,'name':'.text.'+caller,'sha256':sha(raw[:16])}],'relocation_changes':[{'caller':caller,'caller_address':address,'provider':'Resident','provider_address':0x2000,'provider_size':8,'provider_reference_sha256':'a'*64,'old_symbol_index':2,'new_symbol_index':4,'r_info_offset':32,'old_r_info':(2<<8)|4,'new_r_info':(4<<8)|4}]}
def load_build():
 tree=ast.parse((ROOT/'scripts/build.py').read_bytes());nodes=[n for n in tree.body if isinstance(n,ast.FunctionDef)and n.name in {'shared_link_object_proof','rebuild'}]
 ns={'Path':Path,'json':json,'hashlib':hashlib,'sys':sys,'ThreadPoolExecutor':ThreadPoolExecutor,'__name__':'private_build_proof_fixture'};exec(compile(ast.Module(body=nodes,type_ignores=[]),'actual-build-functions','exec'),ns);return ns

class SharedLinkProofTests(unittest.TestCase):
 def setUp(self):
  temporary=tempfile.TemporaryDirectory();self.addCleanup(temporary.cleanup);self.home=Path(temporary.name);self.raw=bytearray(64);struct.pack_into('<I',self.raw,32,(2<<8)|4);self.link=bytearray(self.raw);struct.pack_into('<I',self.link,32,(4<<8)|4);self.raw=bytes(self.raw);self.link=bytes(self.link);self.receipt=receipt(self.raw,self.link);self.build=load_build()
 def files(self,directory):
  c=directory/'build/c';c.mkdir(parents=True,exist_ok=True);raw=c/'boot.c.o';link=c/'boot-shared-link.o';adapter=c/'adapter.json';raw.write_bytes(self.raw);link.write_bytes(self.link);adapter.write_bytes(encoded(self.receipt))
  descriptor={'compiled_path':'build/c/boot.c.o','compiled_object_sha256':sha(self.raw),'path':'build/c/boot-shared-link.o','sha256':sha(self.link),'adapter_path':'build/c/adapter.json','adapter_sha256':sha(adapter.read_bytes())};catalog={'functions':[{'symbol':'Caller','address':0x1000,'size':16}],'shared_link_object':descriptor};return catalog,raw,link,adapter
 def helper(self,directory,catalog,objects,review):
  with mock.patch.dict(sys.modules,{'decomp_report':report}):return self.build['shared_link_object_proof'](directory,catalog,objects,review)
 def test_actual_build_rebuild_routes_derived_but_records_raw_and_shared_chain(self):
  home=self.home;repo=home/'source';(repo/'candidates').mkdir(parents=True);(repo/'config').mkdir();(repo/'progress').mkdir();(repo/'candidates/boot.c').write_bytes(b'authored fixture C');(repo/'config/level-catalog.json').write_bytes(b'{}');(repo/'progress/candidates.json').write_bytes(encoded({'object_sha256':sha(self.raw)}));directory=home/'build';tool=home/'tools'
  for name in ('ld.exe','Ps2EeAs.exe'):
   p=tool/'ee/bin'/name;p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(name.encode())
  reference=home/'reference';reference.write_bytes(b'synthetic reference');objects={};catalog={};link_commands=[]
  def generate(ref,where,name):
   (where/'asm_pp').mkdir();(where/'asm_pp/source.s').write_text('fixture assembly');(where/'config').mkdir();(where/'config/rac2.ld').write_text('fixture');(where/'config/undefined_symbols.ld').write_text('fixture');return {}
  def compile_level(ref,where,tools,level,*args):
   c,raw,link,adapter=self.files(where);native=where/'build/c/native.o';native.write_bytes(b'native fixture');objects.update({'candidates/boot.c':link,'candidates/levels/0_test.c':native});c.update(native={'source':'candidates/levels/0_test.c','catalog_path':'config/native.json','review_path':'progress/native.json','review_sha256':'a'*64,'object_proof':{}},dependency_sha256='b'*64,reference_entry=0x1000,boot_review_sha256='c'*64);catalog.update(c);return c,objects,{'cc1':'d'*64}
  rows=[{'symbol':'Caller','address':0x1000,'size':16,'origin':'boot-shared','candidate_source':'candidates/boot.c'},{'symbol':'Native','address':0x3000,'size':8,'origin':'level-native','candidate_source':'candidates/levels/0_test.c'}]
  def checked(args,where,log):
   if '-Map'in args:link_commands.append(args);(where/'build/overlay.elf').write_bytes(b'linked synthetic ELF')
  integration=types.ModuleType('integration');integration.compile_boot_c=lambda*a:None;integration.compile_level_c=compile_level;integration.replace_inputs=lambda*d,**kw:([],{'fixture':[]});integration.add_definitions=lambda*a:None;integration.validate_integrated=lambda*a,**k:rows;integration.c_objects=lambda obj:list(obj.values())
  native=types.ModuleType('level_native');native.dependencies=lambda*a:'b'*64;native.file_hash=lambda p:sha(Path(p).read_bytes())
  self.build.update(ROOT=repo,TARGET={'serial':'FIXTURE'},generate_config=generate,checked=checked,adapt_linker=lambda*a:None,assemble_source=lambda*a:None,resolved_symbols=lambda*a:None,assert_fresh=lambda*a:None,read_elf=lambda p:{'entry':0x1000,'segments':[{'type':1}]},compare_loads=lambda*a:{'matched':True,'bytes_compared':64})
  with mock.patch.dict(sys.modules,{'integration':integration,'level_native':native,'decomp_report':report}):self.build['rebuild'](reference,sha(reference.read_bytes()),directory,tool,1,'overlay',tool,'0_test')
  proof=json.loads((directory/'integration.json').read_bytes());self.assertEqual(proof['c_object_sha256'],sha(self.raw));self.assertEqual(proof['shared']['c_object_sha256'],sha(self.raw));self.assertEqual(proof['c_link_object_sha256'],sha(self.link));self.assertEqual(proof['shared']['c_link_object_adapter'],self.receipt);self.assertIn('build/c/boot-shared-link.o',link_commands[0]);self.assertNotIn('build/c/boot.c.o',link_commands[0]);self.assertEqual((directory/'build/c/boot.c.o').read_bytes(),self.raw)
 def test_raw_derived_receipt_tampering_and_path_escape_are_refused(self):
  catalog,raw,link,adapter=self.files(self.home/'case');review={'object_sha256':sha(self.raw)};self.helper(self.home/'case',catalog,link,review)
  for path in (raw,link,adapter):
   original=path.read_bytes();path.write_bytes(original+b'changed')
   with self.subTest(path=path.name),self.assertRaises(ValueError):self.helper(self.home/'case',catalog,link,review)
   path.write_bytes(original)
  bad=copy.deepcopy(catalog);bad['shared_link_object']['compiled_path']='../outside.o'
  with self.assertRaises(ValueError):self.helper(self.home/'case',bad,link,review)
  with self.assertRaises(ValueError):self.helper(self.home/'case',catalog,raw,review)
 def test_changed_nonrelocation_bytes_rejected_even_with_self_consistent_new_hash(self):
  catalog,raw,link,adapter=self.files(self.home/'case');mutated=bytearray(self.link);mutated[0]=1;link.write_bytes(mutated);changed=copy.deepcopy(self.receipt);changed['link_object_sha256']=sha(mutated);adapter.write_bytes(encoded(changed));catalog['shared_link_object']['sha256']=sha(mutated);catalog['shared_link_object']['adapter_sha256']=sha(adapter.read_bytes())
  with self.assertRaisesRegex(ValueError,'non-relocation'):self.helper(self.home/'case',catalog,link,{'object_sha256':sha(self.raw)})
 def proof(self,rec=None):
  rec=copy.deepcopy(self.receipt)if rec is None else rec;return {'c_object_sha256':sha(self.raw),'c_link_object_sha256':rec['link_object_sha256'],'c_link_object_adapter_sha256':sha(encoded(rec)),'c_link_object_adapter':rec,'functions':[{'symbol':'Caller','address':0x1000,'size':16}]}
 def test_legacy_without_adapter_is_unchanged_and_valid_chain_passes(self):
  report.validate_shared_link_proof({'c_object_sha256':'d'*64},None);report.validate_shared_link_proof(self.proof(),sha(self.raw))
 def test_wrong_raw_identity_partial_or_false_receipt_claims_are_refused(self):
  proof=self.proof();proof['c_object_sha256']=sha(self.link)
  with self.assertRaises(ValueError):report.validate_shared_link_proof(proof,sha(self.raw))
  for key in ('c_link_object_sha256','c_link_object_adapter_sha256','c_link_object_adapter'):
   proof=self.proof();proof.pop(key)
   with self.subTest(key=key),self.assertRaises(ValueError):report.validate_shared_link_proof(proof,sha(self.raw))
  for key,value in [('compiled_code_unchanged',False),('symbol_table_unchanged',False),('new_physical_bytes',1),('integration_credit',1),('compiled_object_sha256','0'*64)]:
   rec=copy.deepcopy(self.receipt);rec[key]=value
   with self.subTest(key=key),self.assertRaises(ValueError):report.validate_shared_link_proof(self.proof(rec),sha(self.raw))
 def test_invalid_r_info_scope_duplicate_and_receipt_hash_are_refused(self):
  for key,value in [('new_r_info',(4<<8)|5),('new_symbol_index',5),('r_info_offset',33),('caller_address',0x1010),('provider','Caller')]:
   rec=copy.deepcopy(self.receipt);rec['relocation_changes'][0][key]=value
   with self.subTest(key=key),self.assertRaises(ValueError):report.validate_shared_link_proof(self.proof(rec),sha(self.raw))
  rec=copy.deepcopy(self.receipt);rec['relocation_changes']*=2
  with self.assertRaises(ValueError):report.validate_shared_link_proof(self.proof(rec),sha(self.raw))
  proof=self.proof();proof['c_link_object_adapter_sha256']='0'*64
  with self.assertRaises(ValueError):report.validate_shared_link_proof(proof,sha(self.raw))
 def test_actual_native_validator_keeps_strict_raw_boot_equality_and_union_chain(self):
  path=REPO/'tests/test_level_native.py';spec=importlib.util.spec_from_file_location('existing_native_fixture_for_linkproof',path);module=importlib.util.module_from_spec(spec)
  with mock.patch.dict(sys.modules,{'decomp_report':report}):spec.loader.exec_module(module)
  case=module.NativeTests();case.setUp()
  try:
   proof,progress,boot=case.integrated();rec=copy.deepcopy(self.receipt);rec['compiled_object_sha256']='d'*64;rec['relocation_changes'][0].update(caller=case.shared_fn['symbol'],caller_address=case.shared_fn['address']);fields={'c_link_object_sha256':rec['link_object_sha256'],'c_link_object_adapter_sha256':sha(encoded(rec)),'c_link_object_adapter':rec};proof.update(fields);proof['shared'].update(copy.deepcopy(fields));boot['c_object_sha256']='d'*64
   self.assertEqual(len(case.validate_integration(proof,progress,boot)),2)
   bad=copy.deepcopy(proof);bad['c_object_sha256']=bad['shared']['c_object_sha256']=rec['link_object_sha256']
   with self.assertRaisesRegex(ValueError,'Shared source/object'):case.validate_integration(bad,progress,boot)
   bad=copy.deepcopy(proof);bad['shared']['c_link_object_adapter_sha256']='0'*64
   with self.assertRaises(ValueError):case.validate_integration(bad,progress,boot)
  finally:case.doCleanups()

if __name__=='__main__':unittest.main()
