"""Synthetic graph and real-normalizer regressions for static target refinement."""
from pathlib import Path
import copy
import hashlib
import importlib
import random
import struct
import sys
import unittest
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/"scripts"))
m=importlib.import_module("call_graph_refinement")

def row(identity,program,address,signature,size=8,edges=(),supported=True):
    raw=hashlib.sha256(identity.encode()).hexdigest()
    return {"id":identity,"program":program,"address":address,"size":size,"raw_sha256":raw,
      "boundary":{"status":"qualified_complete" if supported else "inferred"},
      "normalization":{"signature_sha256":signature,"certificate":{"exact":True,"raw_sha256":raw,"reconstructed_sha256":raw},"relocations":[]},
      "call_dependencies":[{"offset":offset,"kind":kind,"target":target} for offset,kind,target in edges]}

class RefinementTests(unittest.TestCase):
    def test_callee_constant_difference_splits_callers(self):
        rows=[row('a','p',0x100,'caller',edges=[(0,'call',0x200)]),row('b','p',0x110,'caller',edges=[(0,'call',0x210)]),
              row('x','p',0x200,'constant1'),row('y','p',0x210,'constant2')]
        result=m.refine_call_groups(rows)
        self.assertNotEqual(result['class_by_id']['a'],result['class_by_id']['b'])

    def test_equivalent_callee_copies_different_addresses_group(self):
        rows=[row('a','p',0x100,'caller',edges=[(0,'call',0x200)]),row('b','q',0x120,'caller',edges=[(0,'call',0x240)]),
              row('x','p',0x200,'same'),row('y','q',0x240,'same')]
        result=m.refine_call_groups(rows)
        self.assertEqual(result['class_by_id']['a'],result['class_by_id']['b'])

    def test_equal_numeric_unmasked_targets_in_different_programs_split(self):
        rows=[row('a','p',0x100,'rawsame',edges=[(0,'jump',0x200)]),row('b','q',0x100,'rawsame',edges=[(0,'jump',0x200)]),
              row('x','p',0x200,'one'),row('y','q',0x200,'two')]
        self.assertNotEqual(m.refine_call_groups(rows)['class_by_id']['a'],m.refine_call_groups(rows)['class_by_id']['b'])

    def test_equal_relative_branch_targets_different_bodies_split(self):
        rows=[row('a','p',0x100,'branchsame',edges=[(0,'branch',0x200)]),row('b','q',0x300,'branchsame',edges=[(0,'branch',0x400)]),
              row('x','p',0x200,'one'),row('y','q',0x400,'two')]
        result=m.refine_call_groups(rows)
        self.assertNotEqual(result['class_by_id']['a'],result['class_by_id']['b'])

    def test_recursive_cycles_are_stable_deterministic(self):
        rows=[row('a','p',0x100,'left',edges=[(0,'call',0x200)]),row('b','p',0x200,'right',edges=[(0,'call',0x100)]),
              row('c','q',0x300,'left',edges=[(0,'call',0x400)]),row('d','q',0x400,'right',edges=[(0,'call',0x300)])]
        first=m.refine_call_groups(rows)
        random.Random(4).shuffle(rows)
        self.assertEqual(first,m.refine_call_groups(rows))
        self.assertEqual(first['class_by_id']['a'],first['class_by_id']['c'])
        self.assertLessEqual(first['iterations'],len(rows))

    def test_difference_propagates_through_multiple_call_levels(self):
        rows=[row('a','p',0x100,'outer',edges=[(0,'call',0x200)]),row('b','p',0x110,'outer',edges=[(0,'call',0x210)]),
              row('c','p',0x200,'middle',edges=[(0,'call',0x300)]),row('d','p',0x210,'middle',edges=[(0,'call',0x310)]),
              row('x','p',0x300,'one'),row('y','p',0x310,'two')]
        result=m.refine_call_groups(rows)
        self.assertGreaterEqual(result['iterations'],3)
        self.assertNotEqual(result['class_by_id']['a'],result['class_by_id']['b'])

    def test_unsupported_and_unresolved_remain_conservative(self):
        rows=[row('a','p',0x100,'same',supported=False),row('b','q',0x100,'same',supported=False),
              row('c','p',0x300,'caller',edges=[(0,'call',0x9990)]),row('d','q',0x300,'caller',edges=[(0,'call',0x9990)])]
        result=m.refine_call_groups(rows)
        self.assertNotEqual(result['class_by_id']['a'],result['class_by_id']['b'])
        self.assertNotEqual(result['class_by_id']['c'],result['class_by_id']['d'])
        self.assertEqual(result['unresolved_edge_count'],2)

    def test_explicit_empty_list_does_not_use_legacy_relocations(self):
        rows=[row('a','p',0x100,'same'),row('b','q',0x200,'same')]
        rows[0]['normalization']['relocations']=[{'kind':'j26','offset':0,'target':0x500}]
        self.assertEqual(m.refine_call_groups(rows)['final_class_count'],1)

    def test_legacy_fallback_is_not_complete_graph_claim(self):
        rows=[row('a','p',0x100,'same')];del rows[0]['call_dependencies']
        rows[0]['normalization']['relocations']=[{'kind':'j26','offset':0,'target':0x500}]
        result=m.refine_call_groups(rows)
        self.assertFalse(result['complete_graph_coverage_claimed'])
        self.assertEqual(result['external_static_edge_count'],1)

    def test_internal_targets_do_not_leave_body(self):
        rows=[row('a','p',0x100,'same',edges=[(0,'jump',0x104)]),row('b','q',0x200,'same',edges=[(0,'jump',0x204)])]
        self.assertEqual(m.refine_call_groups(rows)['final_class_count'],1)

    def test_invalid_or_duplicate_entries_rejected(self):
        with self.assertRaises(ValueError):m.refine_call_groups([row('a','p',0x100,'x'),row('b','p',0x100,'x')])
        rows=[row('a','p',0x100,'same',edges=[(0,'call',0x501)])]
        with self.assertRaises(ValueError):m.refine_call_groups(rows)

    def test_same_raw_data_binding_across_programs_can_group(self):
        rows=[row('a','p',0x100,'same',size=16),row('b','q',0x200,'same',size=16)]
        binding={'kind':'hi16_lo16','offset':0,'target':0x180004,'high_offset':0,'low_offset':4,'lo_mode':'addiu'}
        for item in rows:item['normalization']['relocations']=[dict(binding)]
        self.assertEqual(m.refine_call_groups(rows)['final_class_count'],1)
        rows[1]['normalization']['relocations'][0]['target']=0x180008
        self.assertEqual(m.refine_call_groups(rows)['final_class_count'],2)

    def test_same_gp_target_different_base_operands_are_retained(self):
        rows=[row('a','p',0x100,'same'),row('b','q',0x200,'same')]
        rows[0]['normalization']['relocations']=[{'kind':'gp16','offset':0,'target':0x1aeff4,'gp':0x1aeff0}]
        rows[1]['normalization']['relocations']=[{'kind':'gp16','offset':0,'target':0x1aeff4,'gp':0x1aefec}]
        self.assertEqual(m.refine_call_groups(rows)['final_class_count'],2)

    def test_actual_normalizer_unknown_var_plus4_plus8_split(self):
        norm=importlib.import_module('relocation_identity')
        def pack(*values):return struct.pack('<'+'I'*len(values),*values)
        context={'program':'synthetic','reference_sha256':'a'*64,
          'sections':[{'address':0x100000,'size':0x10000,'flags':6,'type':1,'name':'.text'},
                      {'address':0x180000,'size':0x80000,'flags':3,'type':1,'name':'.data'}],
          'function_entries':[],'data_symbols':[]}
        items=[]
        for identity,address,addend in [('a',0x100000,4),('b',0x100100,8)]:
            raw=pack(0x3c080018,0x25080000|addend,0x8d020000,0x03e00008,0)
            normalized=norm.normalize(raw,address,context)
            self.assertTrue(normalized['certificate']['exact'])
            item=row(identity,'synthetic',address,normalized['signature_sha256'],len(raw))
            item['raw_sha256']=hashlib.sha256(raw).hexdigest();item['normalization']=normalized;items.append(item)
        self.assertEqual(items[0]['normalization']['signature_sha256'],items[1]['normalization']['signature_sha256'])
        self.assertEqual([r['target'] for r in items[0]['normalization']['relocations'] if r['kind']=='hi16_lo16'],[0x180004])
        result=m.refine_call_groups(items)
        self.assertNotEqual(result['class_by_id']['a'],result['class_by_id']['b'])

    def test_actual_normalizer_gp_data_addends_remain_primary_distinct(self):
        norm=importlib.import_module('relocation_identity')
        def pack(*values):return struct.pack('<'+'I'*len(values),*values)
        context={'program':'synthetic','reference_sha256':'a'*64,
          'sections':[{'address':0x100000,'size':0x10000,'flags':6,'type':1,'name':'.text'},
                      {'address':0x180000,'size':0x80000,'flags':3,'type':1,'name':'.data'}],
          'function_entries':[],'data_symbols':[],'gp':0x1aeff0,'gp_verified':True,
          'gp_evidence':'synthetic fixed retail initialization theorem fixture'}
        items=[]
        for identity,address,addend in [('a',0x100000,4),('b',0x100100,8)]:
            raw=pack(0x8f820000|addend,0x03e00008,0)
            normalized=norm.normalize(raw,address,context)
            self.assertTrue(normalized['certificate']['exact'])
            item=row(identity,'synthetic',address,normalized['signature_sha256'],len(raw))
            item['raw_sha256']=hashlib.sha256(raw).hexdigest();item['normalization']=normalized;items.append(item)
        self.assertEqual(items[0]['normalization']['signature_sha256'],items[1]['normalization']['signature_sha256'])
        self.assertNotEqual(m.refine_call_groups(items)['class_by_id']['a'],m.refine_call_groups(items)['class_by_id']['b'])

    def test_actual_normalizer_roundtrip_equal_callers_still_split(self):
        norm=importlib.import_module('relocation_identity')
        def pack(*values):return struct.pack('<'+'I'*len(values),*values)
        def caller(target):return pack(0x27bdffe0,0xffbf0010,0x0c000000|(target>>2),0,0xdfbf0010,0x03e00008,0x27bd0020)
        context={'program':'synthetic','reference_sha256':'a'*64,'sections':[{'address':0x100000,'size':0x10000,'flags':6,'type':1,'name':'.text'}],
                 'function_entries':[0x101000,0x102000]}
        items=[]
        for identity,address,raw,edges in [('a',0x100000,caller(0x101000),[(8,'call',0x101000)]),
                ('b',0x100100,caller(0x102000),[(8,'call',0x102000)]),
                ('x',0x101000,pack(0x24020001,0x03e00008,0),[]),('y',0x102000,pack(0x24020002,0x03e00008,0),[])]:
            normalization=norm.normalize(raw,address,context)
            self.assertTrue(normalization['certificate']['exact'])
            item=row(identity,'synthetic',address,normalization['signature_sha256'],len(raw),edges)
            item['raw_sha256']=hashlib.sha256(raw).hexdigest();item['normalization']=normalization;items.append(item)
        self.assertEqual(items[0]['normalization']['signature_sha256'],items[1]['normalization']['signature_sha256'])
        result=m.refine_call_groups(items)
        self.assertNotEqual(result['class_by_id']['a'],result['class_by_id']['b'])

if __name__=='__main__':unittest.main(verbosity=2)
