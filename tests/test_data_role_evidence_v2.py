import importlib.util
import os
from pathlib import Path
import struct
import sys
import unittest

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'scripts'))
from data_role_evidence_v2 import incoming_pointer_roles
from pointer_evidence_loader import load_decoder
norm=load_decoder(Path(os.environ.get('POINTER_DECODER_SOURCE',ROOT/'scripts/relocation_identity.py')))
def I(op,rs,rt,imm):return op<<26|rs<<21|rt<<16|(imm&65535)
def R(rs,rt,rd,fn):return rs<<21|rt<<16|rd<<11|fn
def roles(*words):return incoming_pointer_roles(struct.pack('<'+'I'*len(words),*words),0x100000,norm)
RET=R(31,0,0,8)
class Tests(unittest.TestCase):
    def test_load_pointer(self):self.assertEqual([r['argument_register'] for r in roles(I(35,4,2,0),RET,0)],[4])
    def test_copy_addend(self):self.assertEqual(len(roles(R(4,0,8,33),I(9,8,8,24),I(35,8,2,4),RET,0)),1)
    def test_return_pointer_rejected(self):self.assertEqual(roles(I(35,4,8,0),R(4,0,2,33),RET,0),[])
    def test_store_pointer_rejected(self):self.assertEqual(roles(I(35,4,8,0),I(43,29,4,8),RET,0),[])
    def test_store_same_pointer_base_rejected(self):self.assertEqual(roles(I(43,4,4,0),RET,0),[])
    def test_scalar_rejected(self):self.assertEqual(roles(I(35,4,8,0),I(12,4,2,15),RET,0),[])
    def test_call_preservation_rejected(self):self.assertEqual(roles(I(35,4,8,0),3<<26|(0x110000>>2),0,RET,0),[])
    def test_constant_dereference_not_argument(self):self.assertEqual(roles(I(15,0,4,0x3f),I(35,4,2,-4096),RET,0),[])
    def test_external_branch_rejected(self):self.assertEqual(roles(I(35,4,8,0),I(4,0,0,50),0,RET,0),[])
    def test_regimm_link_call_rejected(self):self.assertEqual(roles(I(1,0,17,3),0,I(35,4,2,0),RET,0,I(9,0,4,1),RET,0),[])
    def test_cop2_use_rejected(self):self.assertEqual(roles(I(35,4,8,0),I(18,4,4,0),RET,0),[])
    def test_delay_load_witness(self):self.assertEqual(roles(RET,I(35,4,2,0))[0]['dereferences'][0]['dereference_instruction_offset'],4)
    def test_addend_preserved(self):self.assertEqual(roles(I(9,4,8,-16),I(35,8,2,12),RET,0)[0]['dereferences'][0]['dereference_offset'],-4)
    def test_incrementing_loop_bounded(self):self.assertEqual(roles(I(9,4,4,4),I(35,4,2,0),I(4,0,0,-3),0),[])
if __name__=='__main__':unittest.main()
