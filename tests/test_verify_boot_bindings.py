import copy
import unittest
import struct
from pathlib import Path
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'scripts'))
from verify_boot_bindings import section_mapping,disjoint_overlay,direct_dependencies,function_chunks_digest

class MappingTests(unittest.TestCase):
    def setUp(self):
        self.boot={'sections':[{'name':'arbitrary','type':1,'flags':6,'address':0x1000,'offset':0x100,'size':0x100}],
                   'segments':[{'type':1,'flags':7,'address':0x1000,'offset':0x100,'filesz':0x100,'memsz':0x100}]}
    def test_section_name_is_not_mapping_proof(self):
        self.assertEqual(section_mapping(self.boot,0x1020,0x20)[2],0x120)
    def test_section_segment_file_offset_disagreement_refused(self):
        self.boot['segments'][0]['offset']+=4
        with self.assertRaises(ValueError):section_mapping(self.boot,0x1020,0x20)
    def test_full_extent_required(self):
        with self.assertRaises(ValueError):section_mapping(self.boot,0x10f0,0x20)
    def test_zero_fill_overlap_refused(self):
        overlay={'sections':[],'segments':[{'type':1,'address':0x1080,'filesz':0,'memsz':0x20}]}
        self.assertFalse(disjoint_overlay(overlay,0x1000,0x100))
    def test_allocated_non_code_overlap_refused(self):
        overlay={'segments':[],'sections':[{'flags':2,'address':0x1080,'size':0x20}]}
        self.assertFalse(disjoint_overlay(overlay,0x1000,0x100))
    def test_adjacent_is_disjoint(self):
        overlay={'segments':[{'type':1,'address':0x1100,'memsz':0x100}],'sections':[]}
        self.assertTrue(disjoint_overlay(overlay,0x1000,0x100))
    def test_ambiguous_executable_ownership_refused(self):
        self.boot['sections'].append(copy.deepcopy(self.boot['sections'][0]))
        with self.assertRaises(ValueError):section_mapping(self.boot,0x1020,0x20)
    def test_memory_only_boot_tail_refused(self):
        self.boot['segments'][0]['filesz']=0x40
        with self.assertRaises(ValueError):section_mapping(self.boot,0x1060,0x20)
    def test_direct_call_target_is_independent_of_role(self):
        body=struct.pack('<II',(3<<26)|(0x1200>>2),0)
        self.assertEqual(direct_dependencies(body,0x1000),[{'offset':0,'kind':'call','target':0x1200}])
    def test_internal_jump_is_not_external_binding(self):
        self.assertEqual(direct_dependencies(struct.pack('<III',(2<<26)|(0x1008>>2),0,0),0x1000),[])
    def test_cop0_branch_is_recorded(self):
        body=struct.pack('<II',(0x10<<26)|(8<<21)|(1<<16)|16,0)
        self.assertEqual(direct_dependencies(body,0x1000),[{'offset':0,'kind':'branch','target':0x1044}])
    def test_regimm_trap_is_not_branch(self):
        self.assertEqual(direct_dependencies(struct.pack('<II',(1<<26)|(8<<16)|16,0),0x1000),[])
    def test_function_payload_descriptor_pin_is_order_independent_in_objects(self):
        self.assertEqual(function_chunks_digest([{'sha256':'a','path':'x'}]),function_chunks_digest([{'path':'x','sha256':'a'}]))
    def test_changed_payload_descriptor_pin_refused_by_comparison(self):
        self.assertNotEqual(function_chunks_digest([{'sha256':'a','path':'x'}]),function_chunks_digest([{'sha256':'b','path':'x'}]))

if __name__=='__main__':unittest.main()
