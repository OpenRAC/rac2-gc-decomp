"""Reviewed family preparation invariants; no compiler or acceptance gate runs."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import tempfile
import unittest
from unittest.mock import patch
import subprocess

ROOT = Path(__file__).resolve().parents[1]
REAL_REPO = Path(os.environ.get('FAMILY_CANDIDATES_TEST_REPO', ROOT))
SPEC = importlib.util.spec_from_file_location('family_candidates', ROOT/'scripts/family_candidates.py')
family = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(family)


class FamilyCandidatesTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.base = Path(self.temp.name)
        self.repo, self.runtime = self.base/'repo', self.base/'runtime'
        for directory in ('scripts','config/level-native','config/family-candidates',
                          'src/shared','candidates/levels','progress/level-candidates'):
            (self.repo/directory).mkdir(parents=True)
        for name in ('level_native.py','check_candidates.py','elf_tools.py','wsl_chain.py','source_layout.py'):
            shutil.copyfile(REAL_REPO/'scripts'/name,self.repo/'scripts'/name)
        self.fragment = b'extern void @@FIRST_DECL@@(int);\nextern void @@SECOND_DECL@@(int);\nint @@FUNCTION@@(int x) { @@FIRST_CALL@@(x); @@SECOND_CALL@@(x); return x; }\n'
        (self.repo/'src/shared/control.cfrag').write_bytes(self.fragment)
        self.spec = {'schema':1,'id':'reviewed-control','target':'SCUS_972.68',
                     'fragment':{'path':'src/shared/control.cfrag','sha256':family.digest(self.fragment)},
                     'contexts':[],'function_token':'@@FUNCTION@@','function':'FamilyControl',
                     'substitutions':{'@@FUNCTION@@':'FamilyControl','@@FIRST_DECL@@':'FamilyFirst',
                                      '@@FIRST_CALL@@':'FamilyFirst','@@SECOND_DECL@@':'FamilySecond',
                                      '@@SECOND_CALL@@':'FamilySecond'},
                     'external_roles':{'@@FIRST_DECL@@':'FamilyFirst','@@SECOND_DECL@@':'FamilySecond'}}
        self.spec_path = self.repo/'config/family-candidates/control.json'
        self.tools = {'cc1':'a'*64,'cpp':'b'*64,'as':'c'*64,'ld.exe':'d'*64}
        self.dump('config/target.json',{'serial':'SCUS_972.68'})
        self.dump('config/candidate-catalog.json',{'flags':list(family.FLAGS)[0]})
        self.dump('progress/candidates.json',{'tools':self.tools})
        self.layout = {'schema':1,'target':'SCUS_972.68','recipes':{}}
        pins = []
        for index,level in enumerate(('0_test','4_test')):
            raw = ('synthetic-reference-'+level).encode()
            reference=self.runtime/f'references/levels/{level}/overlay.elf'
            reference.parent.mkdir(parents=True);reference.write_bytes(raw)
            pin=family.digest(raw);pins.append({'level':level,'sha256':pin})
            address=0x1000+index*0x100
            symbol=f'LVL_{level.upper()}_FUN_{address:08X}'
            mapping={'@@FUNCTION@@':symbol,'@@FIRST_DECL@@':'First'+level.replace('_',''),
                     '@@FIRST_CALL@@':'First'+level.replace('_',''),
                     '@@SECOND_DECL@@':'Second'+level.replace('_',''),
                     '@@SECOND_CALL@@':'Second'+level.replace('_','')}
            source=self.fragment
            for token,value in mapping.items():source=source.replace(token.encode(),value.encode())
            relative=f'candidates/levels/{level}.c';(self.repo/relative).write_bytes(source)
            self.layout['recipes'][relative]={'sha256':family.digest(source),'source_text_bytes':len(source),
                'pieces':[{'fragment':'src/shared/control.cfrag','sha256':family.digest(self.fragment),
                           'source_text_bytes':len(self.fragment),'replacements':mapping}]}
            flags=list(family.FLAGS)[index]
            catalog={'schema':1,'kind':'level-native-catalog','target':'SCUS_972.68','level':level,
                     'program':'levels/'+level,'reference_sha256':pin,'entry':address,'source':relative,
                     'flags':flags,'gp':0 if index==0 else 0x1AEFF0,
                     'functions':[{'symbol':symbol,'address':address,'size':16}],
                     'externals':{mapping['@@FIRST_DECL@@']:0x2000+index*0x100,
                                  mapping['@@SECOND_DECL@@']:0x3000+index*0x100}}
            self.dump(f'config/level-native/{level}.json',catalog)
            self.refresh_review(level)
        self.dump('config/overlays.json',{'target':'SCUS_972.68','levels':pins})
        self.dump('config/source-layout.json',self.layout)
        self.save_spec()

    def tearDown(self):
        self.temp.cleanup()

    def dump(self,relative,value):
        (self.repo/relative).write_bytes(family.encoded(value))

    def save_spec(self):
        self.spec_path.write_bytes(family.encoded(self.spec))

    def refresh_review(self,level):
        catalog=json.loads((self.repo/f'config/level-native/{level}.json').read_bytes())
        symbol=catalog['functions'][0]['symbol'];size=catalog['functions'][0]['size']
        checker=family.digest(b''.join((self.repo/'scripts'/name).read_bytes() for name in
                                      ('level_native.py','check_candidates.py','elf_tools.py','wsl_chain.py')))
        body=family.digest(('synthetic-body-'+level).encode())
        review={'schema':1,'kind':'level-native-candidate','target':catalog['target'],
                'program':catalog['program'],'reference_sha256':catalog['reference_sha256'],
                'reference_entry':catalog['entry'],'candidate_source':catalog['source'],
                'source_sha256':family.digest((self.repo/catalog['source']).read_bytes()),
                'catalog_sha256':family.digest((self.repo/f'config/level-native/{level}.json').read_bytes()),
                'checker_sha256':checker,'flags':catalog['flags'],'state':'matched_unintegrated',
                'object_sha256':'e'*64,'candidate_elf_sha256':'f'*64,'tools':self.tools,
                'matched_code_bytes':size,'functions':[{'symbol':symbol,'address':catalog['functions'][0]['address'],
                     'size':size,'produced_size':size,'matched':True,'different_bytes':0,
                     'reference_sha256':body,'candidate_sha256':body}],'read_only_sections':[]}
        self.dump(f'progress/level-candidates/{level}.json',review)

    def prepare(self,**kwargs):
        return family.prepare(self.repo,self.runtime,self.spec_path,'test-batch',**kwargs)

    def refused(self,**kwargs):
        with self.assertRaises((ValueError,FileExistsError)):
            self.prepare(**kwargs)
        self.assertFalse((self.runtime/'bank/family-candidates/test-batch').exists())

    def test_profiles_share_one_source_and_negative_is_separate_immutable_catalog(self):
        original={p:p.read_bytes() for p in self.repo.rglob('*') if p.is_file()}
        result=self.prepare(negative='0_test:FamilyFirst:FamilySecond')
        bank=Path(result['bank']);tasks=result['tasks']
        self.assertEqual(len(tasks),2)
        self.assertEqual(len({task['source'] for task in tasks}),1)
        self.assertEqual(sorted(len(t['targets']) for t in tasks),[1,2])
        positive=json.loads((bank/'catalogs/0_test.json').read_bytes())
        negative=json.loads((bank/'catalogs/0_test-negative.json').read_bytes())
        self.assertEqual(positive['externals']['FamilyFirst'],0x2000)
        self.assertEqual(negative['externals']['FamilyFirst'],0x3000)
        self.assertEqual(negative['functions'],positive['functions'])
        self.assertEqual(positive['gp'],0)
        self.assertEqual(json.loads((bank/'catalogs/4_test.json').read_bytes())['gp'],0x1AEFF0)
        self.assertEqual(result['receipt']['integration_credit'],0)
        self.assertEqual(original,{p:p.read_bytes() for p in original})
        self.assertEqual(len(result['discovery']['functions']),2)
        self.assertTrue(all(isinstance(f['boundary_evidence'],str) for f in result['discovery']['functions']))
        self.assertTrue(all(t['source'].startswith('runtime:') for t in tasks))
        with self.assertRaises(FileExistsError):self.prepare()

    def test_stale_fragment_refused_before_allocation(self):
        (self.repo/'src/shared/control.cfrag').write_bytes(self.fragment+b' ')
        self.refused()

    def test_boolean_schema_is_not_integer_one(self):
        self.spec['schema']=True;self.save_spec();self.refused()
        self.spec['schema']=1;self.save_spec()
        self.layout['schema']=True;self.dump('config/source-layout.json',self.layout);self.refused()

    def test_reader_timeout_is_a_preallocation_refusal(self):
        with patch.object(family.subprocess,'run',side_effect=subprocess.TimeoutExpired('reader',30)):
            self.refused()

    def test_c_keyword_cannot_be_a_generic_identifier(self):
        self.spec['substitutions']['@@FIRST_CALL@@']='return';self.save_spec();self.refused()

    def test_conflicting_context_and_body_mapping_is_refused(self):
        context=b'extern void @@FIRST_DECL@@(int);\n'
        (self.repo/'src/shared/types.cfrag').write_bytes(context)
        self.spec['contexts']=[{'path':'src/shared/types.cfrag','sha256':family.digest(context)}]
        for level in ('0_test','4_test'):
            rel=f'candidates/levels/{level}.c';recipe=self.layout['recipes'][rel]
            contextual=context.replace(b'@@FIRST_DECL@@',b'OtherKnownHelper')
            source=contextual+(self.repo/rel).read_bytes();(self.repo/rel).write_bytes(source)
            recipe['pieces'].insert(0,{'fragment':'src/shared/types.cfrag','sha256':family.digest(context),
                                       'source_text_bytes':len(context),
                                       'replacements':{'@@FIRST_DECL@@':'OtherKnownHelper'}})
            recipe.update(sha256=family.digest(source),source_text_bytes=len(source))
            path=self.repo/f'config/level-native/{level}.json';cat=json.loads(path.read_bytes())
            cat['externals']['OtherKnownHelper']=0x4000;path.write_bytes(family.encoded(cat))
            self.refresh_review(level)
        self.dump('config/source-layout.json',self.layout);self.save_spec();self.refused()

    def test_stale_context_refused(self):
        (self.repo/'src/shared/types.cfrag').write_bytes(b'typedef int value;\n')
        self.spec['contexts']=[{'path':'src/shared/types.cfrag','sha256':'0'*64}]
        self.save_spec();self.refused()

    def test_context_is_explicit_and_preserved(self):
        context=b'typedef int value;\n'
        (self.repo/'src/shared/types.cfrag').write_bytes(context)
        self.spec['contexts']=[{'path':'src/shared/types.cfrag','sha256':family.digest(context)}]
        for level in ('0_test','4_test'):
            rel=f'candidates/levels/{level}.c';source=context+(self.repo/rel).read_bytes()
            (self.repo/rel).write_bytes(source)
            recipe=self.layout['recipes'][rel]
            recipe['pieces'].insert(0,{'fragment':'src/shared/types.cfrag','sha256':family.digest(context),
                                       'source_text_bytes':len(context)})
            recipe.update(sha256=family.digest(source),source_text_bytes=len(source))
            self.refresh_review(level)
        self.dump('config/source-layout.json',self.layout);self.save_spec()
        result=self.prepare();self.assertTrue((Path(result['bank'])/'source.c').read_bytes().startswith(context))

    def test_stale_recipe_mapping_refused(self):
        piece=self.layout['recipes']['candidates/levels/0_test.c']['pieces'][0]
        piece['replacements']['@@FIRST_CALL@@']='Other'
        self.dump('config/source-layout.json',self.layout);self.refused()

    def test_stale_current_catalog_refused_by_actual_native_review(self):
        path=self.repo/'config/level-native/0_test.json';cat=json.loads(path.read_bytes())
        cat['externals']['First0test']=0x4000;path.write_bytes(family.encoded(cat));self.refused()

    def test_changed_aligned_gp_refused_by_actual_native_review(self):
        path=self.repo/'config/level-native/0_test.json';cat=json.loads(path.read_bytes())
        cat['gp']=4;path.write_bytes(family.encoded(cat));self.refused()

    def test_unsupported_flags_refused(self):
        path=self.repo/'config/level-native/0_test.json';cat=json.loads(path.read_bytes())
        cat['flags']=['-O3','-G0','-ffunction-sections'];path.write_bytes(family.encoded(cat));self.refused()

    def test_stale_generated_unit_refused(self):
        path=self.repo/'candidates/levels/0_test.c';path.write_bytes(path.read_bytes()+b'\n');self.refused()

    def test_stale_reference_refused(self):
        (self.runtime/'references/levels/0_test/overlay.elf').write_bytes(b'wrong release');self.refused()

    def test_incomplete_token_role_refused(self):
        del self.spec['substitutions']['@@FIRST_CALL@@'];self.save_spec();self.refused()

    def test_unknown_role_refused(self):
        self.spec['external_roles']['@@UNKNOWN@@']='Unknown';self.save_spec();self.refused()

    def test_colliding_generic_roles_refused(self):
        self.spec['external_roles']['@@FIRST_DECL@@']='FamilySecond'
        self.spec['substitutions']['@@FIRST_DECL@@']='FamilySecond'
        self.spec['substitutions']['@@FIRST_CALL@@']='FamilySecond'
        self.save_spec();self.refused()

    def test_identifier_substitution_cannot_inject_source(self):
        self.spec['substitutions']['@@FIRST_CALL@@']='FamilyFirst(x);evil'
        self.save_spec();self.refused()

    def test_bad_negative_and_unknown_levels_refused(self):
        for kwargs in ({'negative':'0_test:Unknown:FamilySecond'},
                       {'negative':'0_test:FamilyFirst:FamilyFirst'},
                       {'levels':['unknown']},{'levels':['0_test','0_test']}):
            self.refused(**kwargs)

    def test_path_and_runtime_escape_refused(self):
        self.spec['fragment']['path']='../outside.cfrag';self.save_spec();self.refused()
        for runtime in (self.repo,self.repo/'work',self.base):
            with self.assertRaises(ValueError):family.prepare(self.repo,runtime,self.spec_path,'safe')
        for batch in ('..','../escape','bad/id','.'):
            with self.assertRaises(ValueError):family.prepare(self.repo,self.runtime,self.spec_path,batch)

    def test_unknown_code_data_and_include_are_refused(self):
        for prefix in (b'int hidden;\n',b'int unknown(void) { return 0; }\n',
                       b'#include "unknown.h"\n',b'__asm__("nop");\n'):
            source=prefix+self.fragment
            (self.repo/'src/shared/control.cfrag').write_bytes(source)
            self.spec['fragment']['sha256']=family.digest(source);self.save_spec();self.refused()

    def test_unmapped_implicit_helper_call_is_refused(self):
        source=self.fragment.replace(b'return x;',b'UnknownHelper(x); return x;')
        (self.repo/'src/shared/control.cfrag').write_bytes(source)
        self.spec['fragment']['sha256']=family.digest(source);self.save_spec();self.refused()

    def test_current_review_tool_identity_is_enforced(self):
        path=self.repo/'progress/level-candidates/0_test.json';review=json.loads(path.read_bytes())
        review['tools']['cpp']='0'*64;path.write_bytes(family.encoded(review));self.refused()


if __name__ == '__main__':
    unittest.main()
