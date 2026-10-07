"""Second fixed SDK boot owner: adversarial metadata/ELF tests, no compiler."""
import copy
from pathlib import Path
import struct
import sys
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'scripts'))
import test_boot_object_owners_adversarial as first_owner_tests
import boot_sdk_unit as sdk
import compiler_profiles as cp
import integration


def elf_fixture(path, *, linked=False, size=656, extra_function=False, helper_info=None,
                helper_owner=None, helper_value=None, helper_size=0, relocation=None, addend=0):
    """Synthetic zero text with generic JAL words; no game object/byte fixture."""
    spec = sdk.unit_spec(sdk.CPR8); function = spec['function']
    strings = b'\0' + function['symbol'].encode() + b'\0'; helper_names = {}
    for name in spec['externals']:
        helper_names[name] = len(strings); strings += name.encode()+b'\0'
    extra_name = len(strings); strings += b'EXTRA\0'
    symbols = bytes(16)+struct.pack('<IIIBBH',1,function['address'] if linked else 0,size,18,0,1)
    helper_indices = {}
    for index,(name,address) in enumerate(spec['externals'].items(),2):
        helper_indices[name] = index
        symbols += struct.pack('<IIIBBH',helper_names[name],address if linked and helper_value is None else helper_value or 0,
                               helper_size,helper_info if helper_info is not None else 17 if linked else 16,0,
                               helper_owner if helper_owner is not None else 0xfff1 if linked else 0)
    if extra_function: symbols += struct.pack('<IIIBBH',extra_name,0,4,18,0,1)
    text = bytearray(size); rels = []
    for index,row in enumerate(spec['relocations']):
        word = 0x0c000000 | (row['target_address']>>2 if linked else addend)
        struct.pack_into('<I',text,row['offset'],word)
        offset,info = row['offset'],helper_indices[row['symbol']]<<8|row['type']
        if relocation and index == 0: offset,info = relocation
        rels.append(struct.pack('<II',offset,info))
    names = b'\0.text\0.strtab\0.symtab\0.shstrtab\0.rel.text\0'
    chunks = [('.text',1,6,bytes(text),0,0,8,0),('.strtab',3,0,strings,0,0,1,0),
              ('.symtab',2,0,symbols,2,1,4,16),('.shstrtab',3,0,names,0,0,1,0)]
    if not linked: chunks.append(('.rel.text',9,0,b''.join(rels),3,1,4,8))
    data = bytearray(52); headers = [bytes(40)]
    for name,kind,flags,payload,link,info,alignment,entsize in chunks:
        data.extend(bytes((-len(data))%alignment)); offset = len(data); data.extend(payload)
        headers.append(struct.pack('<10I',names.index(name.encode()+b'\0'),kind,flags,
                                   function['address'] if linked and name=='.text' else 0,
                                   offset,len(payload),link,info,alignment,entsize))
    data.extend(bytes((-len(data))%4)); shoff = len(data); data.extend(b''.join(headers))
    data[:52] = struct.pack('<16sHHIIIIIHHHHHH',b'\x7fELF\x01\x01\x01'+bytes(9),2 if linked else 1,8,1,
                           function['address'] if linked else 0,0,shoff,0,52,32,0,40,len(headers),4)
    path.write_bytes(data)


class SecondSDKOwnerTests(unittest.TestCase):
    def setUp(self):
        self.base = first_owner_tests.BootObjectOwnersAdversarial('test_valid_union_dispatches187_default_and_one_sdk_owner_once')
        self.base.setUp(); self.addCleanup(self.base.temp.cleanup)
        self.root = self.base.root; self.spec = sdk.unit_spec(sdk.CPR8)
        for key in ('source','module'):
            path = self.root/self.spec[key]; path.parent.mkdir(parents=True,exist_ok=True)
            path.write_bytes((ROOT/self.spec['module']).read_bytes())
        catalog = copy.deepcopy(self.base.catalog)
        catalog.update(unit_id=sdk.CPR8,source=self.spec['source'],module=self.spec['module'],
                       source_sha256=self.spec['source_sha256'],module_sha256=self.spec['source_sha256'],
                       functions=[self.spec['function'].copy()],externals=self.spec['externals'].copy(),
                       relocations=copy.deepcopy(self.spec['relocations']),
                       helper_ownership='opaque_absolute_reference_bindings_zero_C_credit')
        self.catalog = catalog; self.base.write(self.spec['catalog'],catalog)
        review = copy.deepcopy(self.base.review)
        review.update(unit_id=sdk.CPR8,source_sha256=self.spec['source_sha256'],
                      catalog_sha256=sdk.file_hash(self.root/self.spec['catalog']),
                      actual_unit_outcome_sha256=self.spec['actual_anchor'],object_sha256=self.spec['object_sha256'],
                      object_contract={'section':'.text','size':656,'alignment':8,'symbol_count':1,'padding':0,
                                       'relocations':4,'helpers':0,'GP':False,'data':[]},
                      functions=[{**self.spec['function'],'matched':True,'different_bytes':0,
                                  'reference_sha256':self.spec['body_sha256'],'candidate_sha256':self.spec['body_sha256'],
                                  'state':'matched_unintegrated'}])
        self.review = review; self.base.write(self.spec['review'],review)
        proof = {key:review[key] for key in self.base.union['sdk_units'][sdk.UNIT]['object_proof'] if key in review}
        proof.update(unit_id=sdk.CPR8,review_sha256=sdk.file_hash(self.root/self.spec['review']))
        self.union = copy.deepcopy(self.base.union)
        self.union['sdk_units'][sdk.CPR8] = {'unit_id':sdk.CPR8,'source':self.spec['source'],'module':self.spec['module'],
                                         'catalog_path':self.spec['catalog'],'review_path':self.spec['review'],
                                         'review_sha256':sdk.file_hash(self.root/self.spec['review']),
                                         'profile_id':cp.SDK_PROFILE,'input_section':'.text','object_proof':proof}
        self.union['functions'].append({**self.spec['function'],'matched':True,'different_bytes':0,
                                       'reference_sha256':self.spec['body_sha256'],'candidate_sha256':self.spec['body_sha256'],
                                       'state':'integrated','integrated':True,'program':'boot',
                                       'candidate_source':self.spec['source'],'origin':'boot-sdk','unit_id':sdk.CPR8})
        self.union['matched_code_bytes'] += 656
        self.union['full_boot_gate']['integrated_c_functions'] = 189
        self.union['full_boot_gate']['integrated_c_bytes'] = 1556
        self.union['default']['full_boot_gate'] = copy.deepcopy(self.union['full_boot_gate'])

    def accept(self,value=None):
        return sdk.validate_union(value or self.union,self.base.default_review,self.root,
                                  self.base.validate_default,self.base.validate_default_object)

    def test_two_owners_add656_once_default187_and_sysbit152_unchanged(self):
        default,owners = self.accept()
        self.assertEqual((len(default),len(owners)),(187,2))
        self.assertEqual(sum(r['size'] for r in default+owners),1556)
        self.assertEqual({r['unit_id']:r['size'] for r in owners},{sdk.UNIT:152,sdk.CPR8:656})
        self.assertEqual(self.base.calls,[('default',187),('object','2'*64)])

    def test_missing_owner_row_descriptor_or_review_never_silently_falls_back(self):
        for mode in ('row','descriptor','review'):
            bad = copy.deepcopy(self.union)
            if mode == 'row': bad['functions'].pop()
            elif mode == 'descriptor': del bad['sdk_units'][sdk.CPR8]
            else: bad['sdk_units'][sdk.CPR8]['review_sha256'] = '0'*64
            with self.subTest(mode=mode),self.assertRaises(ValueError): self.accept(bad)

    def test_duplicate_alias_wrong_owner_and_cpr8_sdk_sysbit_crossbinding_refused(self):
        for mode in ('duplicate','alias','wrong_owner','sysbit_object','sysbit_source'):
            bad = copy.deepcopy(self.union)
            if mode == 'duplicate': bad['functions'].append(copy.deepcopy(bad['functions'][-1]))
            elif mode == 'alias': bad['functions'].append({**bad['functions'][-1],'symbol':'CPR_ALIAS'})
            elif mode == 'wrong_owner': bad['functions'][-1]['unit_id'] = sdk.UNIT
            elif mode == 'sysbit_object': bad['sdk_units'][sdk.CPR8]['object_proof']['object_sha256'] = sdk.UNITS[sdk.UNIT]['object_sha256']
            else: bad['sdk_units'][sdk.CPR8]['source'] = sdk.SOURCE
            with self.subTest(mode=mode),self.assertRaises(ValueError): self.accept(bad)

    def test_source_profile_rootflags_anchor_or656_extent_drift_refused(self):
        for key,value in [('source_sha256','0'*64),('profile_id','unqualified'),('flags',list(cp.FLAGS)+['-G0']),
                          ('input_section','.text.FUN_0012D808'),('helper_ownership','new_helper_C_credit')]:
            c = copy.deepcopy(self.catalog); c[key] = value; self.base.write(self.spec['catalog'],c)
            with self.subTest(key=key),self.assertRaises(ValueError): sdk.load_catalog(self.root,sdk.CPR8)
        self.base.write(self.spec['catalog'],self.catalog)
        for key,value in [('actual_unit_outcome_sha256','0'*64),('object_sha256','0'*64),('integration_credit',True)]:
            r = copy.deepcopy(self.review); r[key] = value
            with self.subTest(key=key),self.assertRaises(ValueError): sdk.validate_review(self.root,self.catalog,r)
        r = copy.deepcopy(self.review); r['object_contract']['size'] = 736
        with self.assertRaises(ValueError): sdk.validate_review(self.root,self.catalog,r)

    def test_catalogue_four_relocations_exact_helpers_offsets_type_and_bool_fields(self):
        for key,value in [('offset',244),('type',5),('symbol','FORGED'),('target_address',0),('offset',False)]:
            c = copy.deepcopy(self.catalog); c['relocations'][0][key] = value
            self.base.write(self.spec['catalog'],c)
            with self.subTest(key=key),self.assertRaises(ValueError): sdk.load_catalog(self.root,sdk.CPR8)
        c = copy.deepcopy(self.catalog); c['relocations'].pop(); self.base.write(self.spec['catalog'],c)
        with self.assertRaises(ValueError): sdk.load_catalog(self.root,sdk.CPR8)
        c = copy.deepcopy(self.catalog); c['unexpected'] = 1; self.base.write(self.spec['catalog'],c)
        with self.assertRaises(ValueError): sdk.load_catalog(self.root,sdk.CPR8)

    def test_object_rel26_undef_helpers_and_zero_jal_addends(self):
        path = self.root/'synthetic.o'; elf_fixture(path)
        observed = sdk.inspect_unit_object(path,sdk.CPR8)
        self.assertEqual(len(observed['relocations']),4); self.assertEqual(observed['helper_C_credit'],0)
        for kwargs in ({'size':736},{'extra_function':True},{'helper_info':18},{'helper_owner':1},
                       {'helper_value':4},{'helper_size':4},{'relocation':(248,(2<<8)|5)},
                       {'relocation':(244,(2<<8)|4)},{'addend':1}):
            with self.subTest(kwargs=kwargs),self.assertRaises(ValueError):
                elf_fixture(path,**kwargs); sdk.inspect_unit_object(path,sdk.CPR8)

    def test_linked_helpers_allow_abs_object17_notype16_but_no_stub_size_or_wrong_target(self):
        path = self.root/'synthetic.elf'
        for info in (16,17):
            elf_fixture(path,linked=True,helper_info=info); sdk.inspect_linked_helpers(path,sdk.CPR8)
        for kwargs in ({'helper_info':18},{'helper_owner':1},{'helper_value':4},{'helper_size':72}):
            with self.subTest(kwargs=kwargs),self.assertRaises(ValueError):
                elf_fixture(path,linked=True,**kwargs); sdk.inspect_linked_helpers(path,sdk.CPR8)

    def test_dependency_set_pins_both_owners_and_overlay_scope_stays_default_only(self):
        paths = sdk.current_input_paths(self.union,self.root)
        for unit in (sdk.UNIT,sdk.CPR8):
            self.assertTrue({sdk.UNITS[unit][key] for key in ('source','module','catalog','review')} <= paths)
        defaults = [{key:row[key] for key in ('symbol','address','size')} for row in self.union['functions'][:187]]
        self.base.write('config/candidate-catalog.json',{'target':'SCUS_972.68','functions':defaults,'flags':['-O2','-G0','-ffunction-sections']})
        self.base.write('config/overlays.json',{'levels':[{'level':'0_aranos_tutorial','sha256':'3'*64}]})
        placement = {'reference_sha256':'3'*64,'functions':[{**defaults[0],'address':0x20000}],'externals':{}}
        self.base.write('config/level-catalog.json',{'target':'SCUS_972.68','levels':{'0_aranos_tutorial':placement}})
        with mock.patch.object(integration,'ROOT',self.root):
            before = integration.level_catalog('0_aranos_tutorial'); self.accept()
            self.assertEqual(before,integration.level_catalog('0_aranos_tutorial'))
            placement['functions'].append(self.spec['function'].copy())
            self.base.write('config/level-catalog.json',{'target':'SCUS_972.68','levels':{'0_aranos_tutorial':placement}})
            with self.assertRaises(ValueError): integration.level_catalog('0_aranos_tutorial')


if __name__ == '__main__': unittest.main()
