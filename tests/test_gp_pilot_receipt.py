"""Asset-free freshness and strict no-credit checks for the measured GP pilot."""
import hashlib,json,sys,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'scripts'))

class GPPilotReceiptTests(unittest.TestCase):
    def test_receipt_is_current_complete_scope_and_adds_no_credit(self):
        proof=json.loads((ROOT/'progress/gp-local-pilot.json').read_bytes())
        self.assertEqual(proof['current_verifier_sha256'],hashlib.sha256((ROOT/'scripts/gp_local_proof.py').read_bytes()).hexdigest())
        self.assertEqual(proof['verifier_sha256'],'7d604c9a56c225d53e542276b6a628f8b7c9f6c6b43f1a1bfcb3e9f72c1ce8d4')
        self.assertFalse(proof['historical_observations_are_current_GP_lifetime_evidence'])
        self.assertEqual(proof['current_accepted_GP_facts'],0)
        self.assertEqual(proof['current_producer_refusals'],28)
        self.assertEqual(proof['current_historical_consumer_refusals'],28)
        self.assertEqual(proof['body_count'],28)
        self.assertEqual(proof['raw_complete_bytes_checked'],141680)
        self.assertIs(proof['normalizer_changed'],False)
        self.assertIs(proof['global_GP_ABI_proven'],False)
        self.assertEqual(proof['C_credit_added'],0)
        self.assertEqual(proof['eligible_GP16_fields'],0)
        scope=json.loads((ROOT/'config/progress-scope.json').read_bytes())
        refs={p['name']:p['sha256'] for p in scope['programs']}
        seen=set()
        for member in proof['members']:
            self.assertNotIn(member['program'],seen);seen.add(member['program'])
            self.assertEqual(member['reference_sha256'],refs[member['program']])
            self.assertEqual(member['entry_GP'],'unknown')
            self.assertEqual(member['size'],5060)
            self.assertEqual(member['positive_offset'],0x9b0)
            self.assertEqual(member['gp_value'],0x10010000)
            self.assertEqual(member['effective_address'],0x1000d400)
            self.assertEqual(member['unsafe_join_offsets'],[0x244,0xa20])
            self.assertEqual(member['eligible_GP16_fields'],0)
            self.assertEqual(member['C_credit'],0)
            self.assertEqual(member['address_class'],'MMIO')
            self.assertEqual(member['known_GP_instruction_count'],6)
            for key in ('raw_sha256','proof_sha256'):
                self.assertRegex(member[key],r'^[0-9a-f]{64}$')
        self.assertEqual(seen,set(refs))
        refused=set()
        historical={member['program']:member for member in proof['members']}
        for member in proof['current_refusals']:
            self.assertNotIn(member['program'],refused);refused.add(member['program'])
            self.assertTrue(member['producer_refused']);self.assertTrue(member['consumer_refused'])
            self.assertEqual(member['reference_sha256'],refs[member['program']])
            self.assertEqual(member['raw_sha256'],historical[member['program']]['raw_sha256'])
            self.assertEqual(member['size'],5060)
            self.assertIn('closed direct CFG',member['reason'])
        self.assertEqual(refused,set(refs))

if __name__=='__main__':unittest.main()
