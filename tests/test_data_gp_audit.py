from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'scripts'))
from data_gp_audit import difference_categories,shape,reason_tags
MEM={35:('load',True,4),43:('store',True,4)}
def I(op,rs,rt,imm):return op<<26|rs<<21|rt<<16|(imm&65535)
class Tests(unittest.TestCase):
    def test_shape_erases_scalar_only_for_discovery(self):
        self.assertEqual(shape(struct.pack('<I',I(9,0,2,0x26)),True),shape(struct.pack('<I',I(9,0,2,0x29)),True))
        self.assertIn('LO_address_or_scalar_immediate',difference_categories(I(9,0,2,0x26),I(9,0,2,0x29),MEM))
    def test_GP_requires_GPR_memory_base(self):
        self.assertIn('GP_memory_displacement',difference_categories(I(35,28,2,1),I(35,28,2,2),MEM))
        self.assertNotIn('GP_memory_displacement',difference_categories(I(18,28,2,1),I(18,28,2,2),MEM))
    def test_register_difference_stays_visible(self):
        a=I(35,4,2,0);b=I(35,5,2,0)
        self.assertNotEqual(shape(struct.pack('<I',a),False),shape(struct.pack('<I',b),False))
        self.assertEqual(shape(struct.pack('<I',a),True),shape(struct.pack('<I',b),True))
        self.assertIn('register_operand_difference',difference_categories(a,b,MEM))
    def test_lui_class_is_not_allocation_proof(self):self.assertEqual(difference_categories(I(15,0,2,10),I(15,0,2,11),MEM),{'LUI_HI16_value'})
    def test_control_target_only(self):self.assertEqual(difference_categories(3<<26|1,3<<26|2,MEM),{'static_control_target'})
    def test_internal_rebase_is_not_callee_split(self):
        self.assertEqual(difference_categories(2<<26|(0x100010>>2),2<<26|(0x200010>>2),MEM,0x100000,0x200000,64,0),{'internal_control_rebase_only'})
    def test_external_callee_keeps_unknown_identity(self):
        self.assertEqual(difference_categories(3<<26|(0x300010>>2),3<<26|(0x400010>>2),MEM,0x100000,0x200000,64,0),{'external_static_control_target'})
    def test_reason_overlap_explicit(self):self.assertEqual(reason_tags('call argument; unqualified scalar; Real GP value/lifetime'),{'callee_argument_or_target','scalar_or_escaped_value','unproved_GP'})
if __name__=='__main__':unittest.main()
