"""Asset-free freshness and strict no-credit checks for the measured GP pilot."""
import hashlib,json,sys,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'scripts'))

class GPPilotReceiptTests(unittest.TestCase):
    def test_receipt_is_current_complete_scope_and_adds_no_credit(self):
        proof=json.loads((ROOT/'progress/gp-local-pilot.json').read_bytes())
        self.assertEqual(proof['verifier_sha256'],hashlib.sha256((ROOT/'scripts/gp_local_proof.py').read_bytes()).hexdigest())
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

if __name__=='__main__':unittest.main()
