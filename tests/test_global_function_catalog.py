"""Boundary tiers: flow support is not original-function certification."""
import importlib
from pathlib import Path
import struct
import sys
import tempfile
import unittest

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/"scripts"))
m=importlib.import_module("global_function_catalog")

class FlowTests(unittest.TestCase):
    def flow(self,words,address=0x1000,entries=()):
        controls={};invalid=set()
        for i,word in enumerate(words):
            valid,control,gp=m.decode_flow(word,address+i*4)
            if control:controls[address+i*4]=control
            if not valid:invalid.add(address+i*4)
        return m.cfg_check(address,len(words)*4,controls,invalid,set(entries))

    def test_return_requires_real_delay_slot(self):
        self.assertTrue(self.flow([0x03e00008,0])["closed"])
        result=self.flow([0x03e00008])
        self.assertFalse(result["closed"])
        self.assertEqual(result["missing_delay_slots"],[0x1004])

    def test_indirect_nonreturn_jump_stays_unresolved(self):
        self.assertFalse(self.flow([0x03200008,0])["closed"])

    def test_known_tail_target_vs_unknown_target(self):
        self.assertTrue(self.flow([0x08000800,0],entries=[0x2000])["closed"])
        self.assertFalse(self.flow([0x08000800,0])["closed"])

    def test_call_returns_inside_local_extent(self):
        result=self.flow([0x0c000800,0,0x03e00008,0])
        self.assertTrue(result["closed"])
        self.assertEqual(len(result["terminal_exits"]),1)

    def test_regimm_is_branch_not_fake_register_operation(self):
        valid,control,gp=m.decode_flow(0x04a10001,0x1000)
        self.assertTrue(valid);self.assertEqual(control["target"],0x1008)
        self.assertFalse(gp)

    def test_real_gp_writes_and_cop2_fields_distinguished(self):
        self.assertTrue(m.decode_flow(0x8fbc0010,0x1000)[2])
        self.assertFalse(m.decode_flow(0x4b8208c0,0x1000)[2])
        self.assertFalse(m.decode_flow(0xc7bc0140,0x1000)[2])

    def test_invalid_instruction_retained_as_failure(self):
        self.assertFalse(m.decode_flow(0x00000005,0x1000)[0])

    def test_control_in_delay_slot_stays_unresolved(self):
        self.assertFalse(self.flow([0x03e00008,0x03e00008])["closed"])

class PartitionTests(unittest.TestCase):
    def row(self,address,size):
        return {"program":"boot","address":address,"size":size,"raw_sha256":"a"*64,
                "declared_symbol":"func_00001000","inner_declared_entries":[address+16],
                "source_file":"fixture.s","source_line":4}

    def test_qualified_leaf_split_preserves_prefix_not_padding(self):
        original=self.row(0x1000,28)
        qualified={"program":"boot","address":0x1010,"size":12,"raw_sha256":"b"*64,"qualified_symbol":"leaf"}
        rows=m.reconcile([original],[qualified])
        self.assertEqual([(x["address"],x["size"]) for x in rows],[(0x1000,16),(0x1010,12)])
        self.assertTrue(rows[0]["force_fragment"])
        self.assertTrue(rows[1]["qualified"])
        self.assertEqual(rows[1]["declared_container"]["size"],28)

    def test_exact_extent_replacement_no_alias_double_count(self):
        original=self.row(0x1000,8)
        q={"program":"boot","address":0x1000,"size":8,"raw_sha256":"b"*64}
        self.assertEqual(len(m.reconcile([original],[q])),1)

    def test_overlapping_qualified_claims_rejected(self):
        with self.assertRaises(ValueError):
            m.reconcile([self.row(0x1000,28)],[{"address":0x1000,"size":20},{"address":0x1010,"size":12}])

    def test_generated_words_must_match_reference(self):
        text="nonmatching func_00001000, 0x8\nglabel func_00001000\n/* 000000 00001000 0800E003 */ jr\n/* 000004 00001004 00000000 */ nop\nendlabel func_00001000\n"
        with tempfile.TemporaryDirectory() as temporary:
            path=Path(temporary)/"fixture.s";path.write_text(text,encoding="utf8")
            raw=struct.pack("<II",0x03e00008,0)
            rows=m.parse_maps(path,"boot",lambda address,size:raw)
            self.assertEqual(rows[0]["raw_sha256"],m.sha(raw))
            with self.assertRaises(ValueError):
                m.parse_maps(path,"boot",lambda address,size:b"x"*size)

if __name__=="__main__":unittest.main(verbosity=2)
