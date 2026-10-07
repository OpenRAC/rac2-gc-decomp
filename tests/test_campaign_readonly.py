"""Exact functions cannot hide missing or differing generated switch data."""
import json
from pathlib import Path
import sys
import unittest
import test_campaign as fixtures

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "scripts"))


class ReadonlyBackend(fixtures.Backend):
    def __init__(self, tools, table_state):
        super().__init__(tools)
        self.table_state = table_state

    def measure(self, obj, catalog, reference, toolchain, work):
        result = super().measure(obj, catalog, reference, toolchain, work)
        if self.table_state != "missing":
            item = catalog["read_only_sections"][0]
            result["read_only_sections"] = [{"section": item["section"], "address": item["address"],
                                            "size": item["size"], "matched": self.table_state == "exact",
                                            "different_bytes": 0 if self.table_state == "exact" else 1,
                                            "reference_sha256": item["sha256"],
                                            "candidate_sha256": item["sha256"] if self.table_state == "exact" else "b" * 64}]
        return result


class CampaignReadonlyTests(unittest.TestCase):
    def setUp(self):
        self.fixture = fixtures.CampaignTests("test_one_compile_many_children_preserves_inputs_and_no_credit")
        self.fixture.setUp()
        self.addCleanup(self.fixture.tearDown)
        catalog = json.loads(self.fixture.catalog.read_bytes())
        catalog["read_only_sections"] = [{"section": ".rodata", "address": 0x2000,
                                         "size": 8, "sha256": "a" * 64}]
        self.fixture.dump(self.fixture.catalog, catalog)
        fixtures.campaign.plan(self.fixture.store, self.fixture.task)

    def test_missing_table_is_refused_even_with_exact_functions(self):
        result = self.fixture.attempt(ReadonlyBackend(self.fixture.tools, "missing"))
        self.assertTrue(all(f["matched"] for f in result["children"][0]["functions"]))
        self.assertEqual(result["state"], "mismatch")
        self.assertEqual(result["integration_credit"], 0)

    def test_one_data_byte_difference_is_refused_even_with_exact_functions(self):
        result = self.fixture.attempt(ReadonlyBackend(self.fixture.tools, "different"))
        self.assertTrue(all(f["matched"] for f in result["children"][0]["functions"]))
        self.assertEqual(result["state"], "mismatch")

    def test_exact_code_and_data_remain_private_until_integration(self):
        result = self.fixture.attempt(ReadonlyBackend(self.fixture.tools, "exact"))
        self.assertEqual(result["state"], "exact_private")
        self.assertEqual(result["integration_credit"], 0)


if __name__ == "__main__":
    unittest.main()
