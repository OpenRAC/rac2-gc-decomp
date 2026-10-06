"""Adversarial SDK owner acceptance; synthetic legacy callbacks, no compiler."""
import copy
import json
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'scripts'))
import boot_sdk_unit as sdk
import compiler_profiles as cp
import integration
import decomp_report

ACTUAL_UNIT = '1b6766c61b67f6e418f3ea46be919c0609f7564f4deaa741d33fb5919d636679'
REFERENCE = '36d5814d8d95328d5839612ccdf7a2e7ecac0b6f868d3ad4bb2e98411f734b4a'
OBJECT = 'e0a9a1a83aed6b86d94cd1b0e0f71ea6021d64f190c842ea56aa83cef9f3845c'


class BootObjectOwnersAdversarial(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(); self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for relative in (sdk.SOURCE,sdk.MODULE,sdk.PROFILE,sdk.CONTROLS,'scripts/boot_sdk_unit.py'):
            destination = self.root/relative; destination.parent.mkdir(parents=True,exist_ok=True)
            source = ROOT/relative if (ROOT/relative).exists() else ROOT/sdk.MODULE
            shutil.copyfile(source,destination)
        self.write('config/target.json',{'serial':'SCUS_972.68','boot':{'sha256':REFERENCE}})
        self.catalog = {'schema':1,'kind':'source-specific-sdk-boot-unit','unit_id':sdk.UNIT,'target':'SCUS_972.68',
                        'program':'boot','source':sdk.SOURCE,'module':sdk.MODULE,'source_sha256':sdk.SOURCE_SHA,
                        'module_sha256':sdk.SOURCE_SHA,'reference_sha256':REFERENCE,'profile_id':cp.SDK_PROFILE,
                        'profile_sha256':sdk.PROFILE_SHA,'control_qualification_sha256':sdk.CONTROL_SHA,
                        'pipeline':cp.PIPELINE,'admission':'qualified_exact_source_unit_only','flags':list(cp.FLAGS),
                        'strip_options':list(cp.STRIP),'input_section':'.text','functions':[sdk.FUNCTION.copy()],
                        'externals':{},'read_only_sections':[],
                        'traits_scope':'matched_this_source_only_not_general_SDK64_or_original_types'}
        self.write(sdk.CATALOG,self.catalog)
        self.review = {'schema':1,'kind':'source-specific-sdk-unit-review','state':'matched_unintegrated',
                       'unit_id':sdk.UNIT,'target':'SCUS_972.68','reference_sha256':REFERENCE,'source_sha256':sdk.SOURCE_SHA,
                       'catalog_sha256':sdk.file_hash(self.root/sdk.CATALOG),'profile_id':cp.SDK_PROFILE,
                       'profile_sha256':sdk.PROFILE_SHA,'control_qualification_sha256':sdk.CONTROL_SHA,
                       'pipeline':cp.PIPELINE,'admission':'qualified_exact_source_unit_only','flags':list(cp.FLAGS),
                       'strip_options':list(cp.STRIP),'tools':cp.TOOLS.copy(),'validator_sha256':sdk.helper_hash(self.root),
                       'actual_unit_outcome_sha256':ACTUAL_UNIT,'object_sha256':OBJECT,'candidate_elf_sha256':'7'*64,
                       'functions':[{**sdk.FUNCTION,'matched':True,'different_bytes':0,'reference_sha256':sdk.BODY,
                                     'candidate_sha256':sdk.BODY,'state':'matched_unintegrated'}],
                       'read_only_sections':[],'object_contract':{'section':'.text','size':152,'alignment':8,
                       'symbol_count':1,'padding':0,'relocations':0,'helpers':0,'GP':False,'data':[]},'integration_credit':0}
        self.write(sdk.REVIEW,self.review)
        defaults = [{'symbol':f'FUN_{0x1000+i*8:08X}','address':0x1000+i*8,'size':4,'matched':True,'different_bytes':0,
                     'reference_sha256':'a'*64,'candidate_sha256':'a'*64,'state':'integrated','integrated':True,
                     'program':'boot','candidate_source':'candidates/boot.c','origin':'boot-default','unit_id':'default-gnu8bed'} for i in range(187)]
        sdk_row = {**sdk.FUNCTION,'matched':True,'different_bytes':0,'reference_sha256':sdk.BODY,'candidate_sha256':sdk.BODY,
                   'state':'integrated','integrated':True,'program':'boot','candidate_source':sdk.SOURCE,'origin':'boot-sdk','unit_id':sdk.UNIT}
        gate = {'matched':True,'reference_sha256':REFERENCE,'integrated_c_functions':188,'integrated_c_bytes':900}
        proof = {key:self.review[key] for key in ['source_sha256','catalog_sha256','profile_id','pipeline','tools',
                                                'object_sha256','candidate_elf_sha256','validator_sha256','functions','read_only_sections']}
        proof.update(unit_id=sdk.UNIT,review_sha256=sdk.file_hash(self.root/sdk.REVIEW))
        owner = {'unit_id':sdk.UNIT,'source':sdk.SOURCE,'module':sdk.MODULE,'catalog_path':sdk.CATALOG,'review_path':sdk.REVIEW,
                 'review_sha256':sdk.file_hash(self.root/sdk.REVIEW),'profile_id':cp.SDK_PROFILE,'input_section':'.text','object_proof':proof}
        self.union = {'schema':3,'kind':'boot-c-owner-integration','target':'SCUS_972.68','program':'boot',
                      'reference_sha256':REFERENCE,'state':'integrated','functions':defaults+[sdk_row],
                      'matched_code_bytes':900,'full_boot_gate':gate,
                      'default':{'target':'SCUS_972.68','reference_sha256':REFERENCE,'candidate_source':'candidates/boot.c',
                                 'source_sha256':'1'*64,'object_sha256':'2'*64,'matched_code_bytes':748,'full_boot_gate':gate},
                      'sdk_units':{sdk.UNIT:owner}}
        self.default_review = {'source_sha256':'1'*64,'object_sha256':'2'*64}
        self.default_expected = {(r['symbol'],r['address'],r['size']) for r in defaults}
        self.calls = []

    def write(self,name,value):
        destination = self.root/name; destination.parent.mkdir(parents=True,exist_ok=True)
        destination.write_text(json.dumps(value,sort_keys=True)+'\n')

    def validate_default(self,legacy):
        if len(legacy['functions']) != 187 or any(r['symbol']=='_sysbitFlush' for r in legacy['functions']):
            raise ValueError('Default187 owner must remain complete and isolated')
        if {(r['symbol'],r['address'],r['size']) for r in legacy['functions']} != self.default_expected:
            raise ValueError('Unknown default body ownership')
        if legacy['matched_code_bytes'] != sum(r['size'] for r in legacy['functions']):
            raise ValueError('Default subset count/bytes inconsistent')
        if legacy['source_sha256'] != self.default_review['source_sha256']:
            raise ValueError('Default source drift')
        self.calls.append(('default',len(legacy['functions'])))

    def validate_default_object(self,legacy,review):
        if legacy['object_sha256'] != review['object_sha256']:
            raise ValueError('Default object drift')
        self.calls.append(('object',legacy['object_sha256']))

    def accept(self,value=None):
        return sdk.validate_union(value or self.union,self.default_review,self.root,
                                  self.validate_default,self.validate_default_object)

    def test_valid_union_dispatches187_default_and_one_sdk_owner_once(self):
        default,sdk_rows = self.accept()
        self.assertEqual((len(default),len(sdk_rows)),(187,1))
        self.assertEqual(self.calls,[('default',187),('object','2'*64)])
        self.assertEqual(sum(r['size'] for r in default)+sdk_rows[0]['size'],900)

    def test_forged_owner_source_profile_section_receipt_object_and_reference_refused(self):
        edits = [('source','candidates/boot.c'),('module','src/boot/other.cfrag'),('profile_id','other-profile'),
                 ('input_section','.text._sysbitFlush'),('review_sha256','0'*64),('unit_id','other-unit')]
        for key,value in edits:
            bad = copy.deepcopy(self.union); bad['sdk_units'][sdk.UNIT][key] = value
            with self.subTest(key=key),self.assertRaises(ValueError): self.accept(bad)
        for key in ('source_sha256','catalog_sha256','object_sha256','validator_sha256'):
            bad = copy.deepcopy(self.union); bad['sdk_units'][sdk.UNIT]['object_proof'][key] = '0'*64
            with self.subTest(proof=key),self.assertRaises(ValueError): self.accept(bad)
        bad = copy.deepcopy(self.union); bad['reference_sha256'] = '0'*64
        with self.assertRaises(ValueError): self.accept(bad)

    def test_sdk_final_row_mismatch_flags_unknown_fields_and_bool_integers_refused(self):
        for key,value in [('different_bytes',1),('different_bytes',False),('state','mismatch'),('program','levels/4_barlow'),
                          ('matched',1),('integrated',1),('address',True),('size',True),('unexpected_owner_field',True)]:
            bad = copy.deepcopy(self.union); bad['functions'][-1][key] = value
            with self.subTest(key=key,value=value),self.assertRaises(ValueError): self.accept(bad)

    def test_missing_duplicate_renamed_overlap_and_incorrect_counter_refused(self):
        cases = []
        bad = copy.deepcopy(self.union); bad['functions'].pop(); cases.append(bad)
        bad = copy.deepcopy(self.union); bad['functions'].append(copy.deepcopy(bad['functions'][-1])); cases.append(bad)
        bad = copy.deepcopy(self.union); bad['functions'].append({**bad['functions'][-1],'symbol':'RENAMED_ALIAS'}); cases.append(bad)
        bad = copy.deepcopy(self.union); bad['sdk_units'] = {}; cases.append(bad)
        bad = copy.deepcopy(self.union); bad['functions'].pop(0); bad['matched_code_bytes'] -= 4; cases.append(bad)
        bad = copy.deepcopy(self.union); bad['matched_code_bytes'] += 152; cases.append(bad)
        for bad in cases:
            with self.subTest(value=bad),self.assertRaises(ValueError): self.accept(bad)

    def test_nested_contract_types_unknown_fields_and_actual_unit_anchor_refused(self):
        for key,value in [('padding',False),('relocations',False),('helpers',False),('GP',0),('symbol_count',True),
                          ('unexpected_field',0),('size',156),('data',[{}])]:
            bad = copy.deepcopy(self.review); bad['object_contract'][key] = value
            with self.subTest(key=key),self.assertRaises(ValueError): sdk.validate_review(self.root,self.catalog,bad)
        bad = copy.deepcopy(self.review); bad['actual_unit_outcome_sha256'] = '0'*64
        with self.assertRaises(ValueError): sdk.validate_review(self.root,self.catalog,bad)

    def test_changed_sdk_sources_catalogue_foundation_and_unknown_owner_maps_refused(self):
        for relative in (sdk.SOURCE,sdk.MODULE,sdk.PROFILE,sdk.CONTROLS):
            path = self.root/relative; original = path.read_bytes(); path.write_bytes(original+b'\n')
            with self.subTest(path=relative),self.assertRaises(ValueError): self.accept()
            path.write_bytes(original)
        bad = copy.deepcopy(self.union); bad['sdk_units']['extra-owner'] = copy.deepcopy(bad['sdk_units'][sdk.UNIT])
        with self.assertRaises(ValueError): self.accept(bad)
        bad = copy.deepcopy(self.union); bad['sdk_units'][sdk.UNIT]['object_proof']['unknown'] = 1
        with self.assertRaises(ValueError): self.accept(bad)

    def test_existing_overlay_catalogue_uses_only_default_boot_bodies(self):
        legacy_functions = [{k:r[k] for k in ('symbol','address','size')} for r in self.union['functions'][:-1]]
        self.write('config/candidate-catalog.json',{'target':'SCUS_972.68','functions':legacy_functions,'flags':['-O2','-G0','-ffunction-sections']})
        self.write('config/overlays.json',{'levels':[{'level':'0_aranos_tutorial','sha256':'3'*64}]})
        placement = {'reference_sha256':'3'*64,'functions':[{**legacy_functions[0],'address':0x20000}],'externals':{}}
        self.write('config/level-catalog.json',{'target':'SCUS_972.68','levels':{'0_aranos_tutorial':placement}})
        with mock.patch.object(integration,'ROOT',self.root):
            before = integration.level_catalog('0_aranos_tutorial')
            self.accept()
            self.assertEqual(integration.level_catalog('0_aranos_tutorial'),before)
            placement['functions'].append(sdk.FUNCTION.copy())
            self.write('config/level-catalog.json',{'target':'SCUS_972.68','levels':{'0_aranos_tutorial':placement}})
            with self.assertRaises(ValueError): integration.level_catalog('0_aranos_tutorial')

    def test_actual_unit_reference_cannot_be_repinned_independently_of_qualification(self):
        self.write('config/target.json',{'serial':'SCUS_972.68','boot':{'sha256':'0'*64}})
        changed = copy.deepcopy(self.catalog); changed['reference_sha256'] = '0'*64
        self.write(sdk.CATALOG,changed)
        with self.assertRaises(ValueError): sdk.load_catalog(self.root)

    def test_physical_exporter_keeps_outer188_count_and_validates_default187_subset(self):
        boot_source = self.root/'candidates/boot.c'; boot_source.parent.mkdir(parents=True,exist_ok=True)
        boot_source.write_bytes(b'int FUN_00001000(void) { return 0; }\n')
        defaults = self.union['functions'][:-1]
        catalog = {'target':'SCUS_972.68','reference_sha256':REFERENCE,
                   'functions':[{k:r[k] for k in ('symbol','address','size')} for r in defaults]}
        self.write('config/candidate-catalog.json',catalog)
        legacy = self.union['default']
        legacy.update(state='integrated',catalog_sha256=sdk.file_hash(self.root/'config/candidate-catalog.json'),
                      source_sha256=sdk.file_hash(boot_source),tools={name:'4'*64 for name in ('cc1','cpp','as','ld.exe')})
        review = {'target':'SCUS_972.68','reference_sha256':REFERENCE,'source_sha256':legacy['source_sha256'],
                  'catalog_sha256':legacy['catalog_sha256'],'tools':legacy['tools'],'object_sha256':'2'*64,
                  'candidate_elf_sha256':'5'*64,'checker_sha256':'6'*64,
                  'functions':[{k:v for k,v in r.items() if k not in ('unit_id','origin','candidate_source')} for r in defaults]}
        self.write('progress/candidates.json',review)
        gate = self.union['full_boot_gate']
        gate.update(bytes_compared=decomp_report.BOOT_LOAD_BYTES,segments=decomp_report.BOOT_LOAD_SEGMENTS)
        progress = {'target':'SCUS_972.68','integrated_functions':188,'decompiled_functions':188,
                    'g1':{'matched':True,'reference_sha256':REFERENCE,'bytes_compared':decomp_report.BOOT_LOAD_BYTES}}
        with mock.patch.object(decomp_report,'ROOT',self.root):
            accepted = decomp_report.validate_integration(self.union,{'serial':'SCUS_972.68','boot':{'sha256':REFERENCE}},progress)
            self.assertEqual(len(accepted),188)
            self.assertEqual(sum(r['size'] for r in accepted),900)
            for count in (187,189,True):
                wrong = copy.deepcopy(progress); wrong['integrated_functions'] = wrong['decompiled_functions'] = count
                with self.subTest(count=count),self.assertRaises(ValueError):
                    decomp_report.validate_integration(self.union,{'serial':'SCUS_972.68','boot':{'sha256':REFERENCE}},wrong)

    def test_overlay_export_dispatches_only_the_validated_default_boot_owner(self):
        class DispatchObserved(Exception):
            pass

        target = {'serial': 'SCUS_972.68', 'boot': {'sha256': REFERENCE}}
        levels = [{'level': 'level' + str(i), 'sha256': '3' * 64} for i in range(27)]
        overlays = {'target': 'SCUS_972.68', 'levels': levels}
        programs = [{'name': 'boot', 'sha256': REFERENCE, 'sections': []}]
        programs += [{'name': 'levels/' + row['level'], 'sha256': row['sha256'],
                      'sections': []} for row in levels]
        scope = {'target': 'SCUS_972.68', 'programs': programs}
        self.write('config/level-catalog.json', {})
        self.write('config/candidate-catalog.json', {})

        def observe(*args):
            shared = args[4]
            self.assertEqual(shared['source_sha256'], '1' * 64)
            self.assertEqual(len(shared['functions']), 187)
            self.assertTrue(all(row['origin'] == 'boot-default' for row in shared['functions']))
            self.assertNotIn('sdk_units', shared)
            raise DispatchObserved()

        with mock.patch.object(decomp_report, 'ROOT', self.root), \
                mock.patch.object(decomp_report, 'validate_integration', return_value=self.union['functions']), \
                mock.patch.object(decomp_report, 'validate_native_level_proof', side_effect=observe):
            with self.assertRaises(DispatchObserved):
                decomp_report.generate(scope, target, overlays,
                                       {'integrated_functions': 188, 'decompiled_functions': 188},
                                       self.union, [{'kind': 'level-c-integration', 'program': 'level0'}])


if __name__ == '__main__': unittest.main()
