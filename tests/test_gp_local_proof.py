"""Finite local GP synthetic proofs: no assets or compiler."""
import copy
from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))
import gp_local_proof as gp

BASE = 0x100000
REF = "a" * 64
SECTIONS = [{"name": ".text", "type": 1, "flags": 6, "address": BASE, "size": 0x1000},
            {"name": ".data", "type": 1, "flags": 3, "address": 0x180000, "size": 0x1000}]


def i(op, rs, rt, imm):
    return op << 26 | rs << 21 | rt << 16 | imm & 0xffff


def r(rs, rt, rd, fn, shift=0):
    return rs << 21 | rt << 16 | rd << 11 | shift << 6 | fn


def j(op, target):
    return op << 26 | target >> 2 & 0x3ffffff


def body(*words):
    return struct.pack("<" + "I" * len(words), *words)


RET = r(31, 0, 0, 8)
DEFINE = (i(15, 0, 28, 0x18), i(13, 28, 28, 0x100))
LOAD = i(0x23, 28, 2, 4)


def proof(*words, sections=SECTIONS):
    return gp.prove(body(*words), BASE, program="synthetic", reference_sha256=REF,
                    sections=sections, boundary_evidence="supplied synthetic complete extent")


def valid_memory(result):
    return [m for m in result["GP_memory"] if m["locally_proved_mapped_target"]]


class LocalGPTests(unittest.TestCase):
    def test_unknown_entry_never_uses_metadata_global_gp(self):
        result = proof(LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])
        self.assertFalse(result["entry_GP_assumed"])
        with self.assertRaises(TypeError):
            gp.prove(body(LOAD), BASE, program="synthetic", reference_sha256=REF,
                sections=SECTIONS, boundary_evidence="extent", gp_verified=True, gp=0x180100)

    def test_lui_ori_complete_target_and_unmasked_reconstruction(self):
        result = proof(*DEFINE, LOAD, RET, 0)
        memory = valid_memory(result)
        self.assertEqual([(m["offset"], m["gp_lower64"], m["target"]) for m in memory],
                         [(8, 0x180100, 0x180104)])
        self.assertEqual(result["certificate"]["reconstructed_sha256"], result["raw_sha256"])
        gp.verify(result, body(*DEFINE, LOAD, RET, 0), BASE, program="synthetic", reference_sha256=REF,
                  sections=SECTIONS, boundary_evidence="supplied synthetic complete extent")

    def test_addiu_daddiu_move_chain(self):
        for op in (9, 25):
            result = proof(i(15, 0, 8, 0x18), i(op, 8, 8, 0x100), r(8, 0, 28, 0x2d), LOAD, RET, 0)
            self.assertEqual(valid_memory(result)[0]["target"], 0x180104)

    def test_negative_sign_extensions_are_not_low_pointer_proofs(self):
        for sequence in [(i(15, 0, 28, 0xffff),),
                         (i(9, 0, 28, -1),), (i(25, 0, 28, -1),)]:
            self.assertEqual(valid_memory(proof(*sequence, LOAD, RET, 0)), [])
        self.assertEqual(gp.sx32(0xffffffff), 0xffffffffffffffff)

    def test_gpr_load_to_gp_kills_and_float_vector_fields_do_not_write_gp(self):
        result = proof(*DEFINE, i(0x23, 28, 28, 0), LOAD, RET, 0)
        self.assertEqual([m["offset"] for m in valid_memory(result)], [8])
        for opcode in (0x31, 0x35, 0x36, 0x39, 0x3d, 0x3e):
            result = proof(*DEFINE, i(opcode, 28, 28, 0), LOAD, RET, 0)
            self.assertEqual([m["offset"] for m in valid_memory(result)], [8, 12])

    def test_call_slot_uses_prior_gp_and_return_kills_all_facts(self):
        result = proof(*DEFINE, j(3, 0x101000), LOAD, LOAD, RET, 0)
        self.assertEqual([m["offset"] for m in valid_memory(result)], [12])
        self.assertFalse(result["call_GP_preservation_assumed"])
        result = proof(j(3, 0x101000), i(15, 0, 28, 0x18), LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])

    def test_indirect_call_and_saved_register_assumption_refused(self):
        result = proof(*DEFINE, r(28, 0, 16, 0x25), r(8, 0, 31, 9), 0,
                       r(16, 0, 28, 0x25), LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])

    def test_delay_definition_before_internal_jump_is_valid(self):
        result = proof(j(2, BASE + 8), i(15, 0, 28, 0x18), LOAD, RET, 0)
        self.assertEqual(valid_memory(result)[0]["gp_lower64"], 0x180000)

    def test_divergent_join_and_likely_bypass_slot_destroy_gp(self):
        # GP initially known, one branch arm replaces it with another constant.
        result = proof(*DEFINE, i(4, 4, 0, 2), 0, i(13, 28, 28, 0x200), LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])
        # Taken branch runs the LUI slot; not-taken likely branch annuls it.
        result = proof(i(20, 4, 0, 1), i(15, 0, 28, 0x18), LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])

    def test_equal_join_retains_gp_and_unreachable_write_does_not_matter(self):
        result = proof(*DEFINE, i(4, 4, 0, 2), 0, 0, LOAD, RET, 0)
        self.assertEqual(len(valid_memory(result)), 1)
        result = proof(*DEFINE, j(2, BASE + 16), 0, LOAD, RET, 0,
                       i(15, 0, 28, 0x77))
        self.assertEqual(len(valid_memory(result)), 1)
        self.assertNotIn(28, [f["offset"] for f in result["locally_known_GP"]])

    def test_unknown_mmi_and_opcode_destroy_gp(self):
        for instruction in (0x70000000, 0x4c000000, i(15, 1, 28, 0x18)):
            result = proof(*DEFINE, instruction, LOAD, RET, 0)
            self.assertEqual(valid_memory(result), [])

    def test_two_qualified_mmi_forms_write_only_the_destination(self):
        pmfhl = 0x7000b930; psrah = 0x7017b9f7
        result = proof(*DEFINE, pmfhl, psrah, LOAD, RET, 0)
        self.assertEqual(len(valid_memory(result)), 1)
        self.assertEqual(len(result["finite_MMI_decoder_receipt"]["instructions"]), 2)
        for word in ((pmfhl & ~0xf800) | 28 << 11, (psrah & ~0xf800) | 28 << 11):
            self.assertEqual(valid_memory(proof(*DEFINE, word, LOAD, RET, 0)), [])
        read_gp = (psrah & ~0x1f0000) | 28 << 16
        self.assertEqual(len(valid_memory(proof(*DEFINE, read_gp, LOAD, RET, 0))), 1)
        for word in (pmfhl ^ 1, pmfhl ^ (1 << 6), psrah ^ 1, psrah ^ (1 << 6)):
            self.assertEqual(valid_memory(proof(*DEFINE, word, LOAD, RET, 0)), [])

    def test_qualified_mmi_decoder_drift_refused(self):
        from unittest.mock import patch
        import rabbitizer
        with patch.object(rabbitizer, "__version__", "unqualified"), self.assertRaises(ValueError):
            proof(*DEFINE, 0x7000b930, LOAD, RET, 0)

    def test_mmio_gp_is_known_but_no_mapped_data_relocation(self):
        result = proof(i(15, 0, 28, 0x1001), 0x7000b930, 0x7017b9f7,
                       i(0x23, 28, 25, -0x2c00), i(0x23, 28, 28, -0x3000), LOAD, RET, 0)
        self.assertEqual(result["GP_memory"][0]["gp_lower64"], 0x10010000)
        self.assertEqual(result["GP_memory"][0]["target"], 0x1000d400)
        self.assertEqual(result["GP_memory"][1]["gp_lower64"], 0x10010000)
        self.assertIsNone(result["GP_memory"][2]["gp_lower64"])
        self.assertEqual(result["eligible_scoped_relocations"], [])

    def test_join_path_bypassing_local_definition_is_unknown(self):
        result = proof(i(4, 4, 0, 2), 0, i(15, 0, 28, 0x18), LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])

    def test_consumer_binds_producing_mmi_decoder_artifact(self):
        result = proof(*DEFINE, 0x7000b930, LOAD, RET, 0)
        forged = copy.deepcopy(result)
        forged["finite_MMI_decoder_receipt"]["decoder_sha256"] = "c" * 64
        with self.assertRaises(ValueError):
            gp.verify(forged, body(*DEFINE, 0x7000b930, LOAD, RET, 0), BASE,
                program="synthetic", reference_sha256=REF, sections=SECTIONS,
                boundary_evidence="supplied synthetic complete extent")

    def test_cross_mapping_width_and_duplicate_owner_refused(self):
        result = proof(i(15, 0, 28, 0x18), i(13, 28, 28, 0xffe), LOAD, RET, 0)
        self.assertEqual(valid_memory(result), [])
        result = proof(*DEFINE, LOAD, RET, 0, sections=SECTIONS + [dict(SECTIONS[1])])
        self.assertEqual(valid_memory(result), [])
        with self.assertRaises(ValueError):
            proof(*DEFINE, LOAD, RET, 0, sections=[dict(SECTIONS[0], size=4), SECTIONS[1]])

    def test_partial_or_unaligned_access_remains_raw(self):
        for opcode in (0x22, 0x26, 0x2a, 0x2e, 0x1a, 0x1b, 0x2c, 0x2d):
            result = proof(*DEFINE, i(opcode, 28, 2, 0), RET, 0)
            self.assertEqual(valid_memory(result), [])
        result = proof(*DEFINE, i(0x1e, 28, 2, 4), RET, 0)
        self.assertEqual(valid_memory(result), [])

    def test_fake_executable_section_is_not_an_EE_code_owner(self):
        for name in (".fake-exec", ".vutext", ".DVP.overlay"):
            sections = [dict(SECTIONS[0], name=name), SECTIONS[1]]
            with self.subTest(name=name), self.assertRaises(ValueError):
                proof(*DEFINE, LOAD, RET, 0, sections=sections)

    def test_incomplete_slot_or_branch_into_delay_refused(self):
        with self.assertRaises(ValueError):
            proof(j(3, 0x101000))
        with self.assertRaises(ValueError):
            proof(j(2, BASE + 4), 0, RET, 0)
        with self.assertRaises(ValueError):
            proof(i(1, 4, 16, 1), 0, RET, 0)

    def test_system_control_and_exception_do_not_create_a_false_cfg(self):
        for instruction in (0x42000018, 0x0000000c, 0x0000000d):
            with self.assertRaises(ValueError):
                proof(*DEFINE, instruction, LOAD, RET, 0)

    def test_boolean_integer_and_float_substitutions_are_not_equal_proofs(self):
        original = proof(i(9, 0, 28, 0), LOAD, RET, 0)
        variants = []
        bad = copy.deepcopy(original); bad["locally_known_GP"][0]["gp_lower64"] = False; variants.append(bad)
        bad = copy.deepcopy(original); bad["certificate"]["exact"] = 1; variants.append(bad)
        bad = copy.deepcopy(original); bad["address"] = float(BASE); variants.append(bad)
        for bad in variants:
            with self.assertRaises(ValueError):
                gp.verify(bad, body(i(9, 0, 28, 0), LOAD, RET, 0), BASE,
                    program="synthetic", reference_sha256=REF, sections=SECTIONS,
                    boundary_evidence="supplied synthetic complete extent")

    def test_spoof_fact_target_tool_source_and_unknown_field_refused(self):
        original = proof(*DEFINE, LOAD, RET, 0)
        mutations = []
        for key, value in [("entry_GP_assumed", True), ("address", BASE + 4), ("reference_sha256", "b" * 64),
                           ("raw_sha256", "b" * 64), ("cfg_decoder_sha256", "b" * 64),
                           ("caller_gp_verified", True)]:
            forged = copy.deepcopy(original); forged[key] = value; mutations.append(forged)
        forged = copy.deepcopy(original); forged["GP_memory"][0]["target"] += 4; mutations.append(forged)
        for forged in mutations:
            with self.assertRaises(ValueError):
                gp.verify(forged, body(*DEFINE, LOAD, RET, 0), BASE, program="synthetic", reference_sha256=REF,
                    sections=SECTIONS, boundary_evidence="supplied synthetic complete extent")
        # Constants, destination GPR and load opcode are part of the full raw proof.
        for words in [(*DEFINE, i(0x23, 28, 2, 8), RET, 0),
                      (i(15, 0, 27, 0x18), DEFINE[1], LOAD, RET, 0),
                      (*DEFINE, i(0x24, 28, 2, 4), RET, 0)]:
            with self.assertRaises(ValueError):
                gp.verify(original, body(*words), BASE, program="synthetic", reference_sha256=REF,
                    sections=SECTIONS, boundary_evidence="supplied synthetic complete extent")


if __name__ == "__main__":
    unittest.main()
