"""Address proofs, CFG/register kills, literal preservation and reconstruction."""
import importlib.util
from pathlib import Path
import struct
import sys
import unittest

ROOT=Path(__file__).resolve().parents[1]
spec=importlib.util.spec_from_file_location('relocation_identity',ROOT/'scripts/relocation_identity.py')
m=importlib.util.module_from_spec(spec);sys.modules[spec.name]=m;spec.loader.exec_module(m)
BASE=0x100000


def i(op,rs,rt,imm):return op<<26|rs<<21|rt<<16|(imm&0xffff)
def r(rs,rt,rd,fn):return rs<<21|rt<<16|rd<<11|fn
def j(op,target):return op<<26|((target>>2)&0x3ffffff)
def body(*words):return struct.pack('<'+'I'*len(words),*words)
RET=r(31,0,0,8)


def context(**overrides):
    result={'program':'levels/test','reference_sha256':'a'*64,
            'sections':[{'address':0x100000,'size':0x20000,'flags':6,'type':1,'name':'.text'},
                        {'address':0x180000,'size':0x80000,'flags':3,'type':1,'name':'.data'},
                        {'address':0x3f0000,'size':0x10000,'flags':3,'type':8,'name':'.bss'}],
            'function_entries':[0x101000,0x102000,0x103000],
            'gp':0x1aeff0,'gp_verified':True,'gp_evidence':'pinned retail LUI/ADDIU + DADDU initialization'}
    result.update(overrides);return result


def pointer(address,reg=8):
    return i(15,0,reg,((address+0x8000)>>16)&0xffff),i(9,reg,reg,address&0xffff)


def pointer_context(callee=0x101000,register=4,addend=16,width=4):
    proof={'callee_address':callee,'argument_register':register,'callee_size':16,
           'callee_raw_sha256':'b'*64,'boundary_tier':'flow_supported_inferred',
           'program':'levels/test','reference_sha256':'a'*64,
           'dereference_instruction_offset':0,'dereference_offset':addend,'width':width,
           'action':'load','evidence':'Replayed pinned incoming-only GPR memory dereference theorem',
           'proof_status':'verified_incoming_dereference','proof_decoder_sha256':'c'*64,
           'decoder_dependency_sha256':'d'*64}
    return {'pointer_argument_roles':[proof],
            'function_pins':{callee:{'size':16,'raw_sha256':'b'*64,'boundary_tier':'flow_supported_inferred'}}}


class RelocationIdentityTests(unittest.TestCase):
    def norm(self,data,**kwargs):
        result=m.normalize(data,BASE,context(**kwargs))
        self.assertEqual(m.reconstruct(result['template'],BASE,result['relocations']),data)
        self.assertEqual(result['certificate']['raw_sha256'],result['certificate']['reconstructed_sha256'])
        self.assertTrue(result['certificate']['exact']);return result

    def test_signed_carry_and_mapped_address_normalize(self):
        first=body(*pointer(0x188123),i(0x23,8,2,0),RET,0)
        second=body(*pointer(0x198123),i(0x23,8,2,0),RET,0)
        a=self.norm(first);b=self.norm(second)
        self.assertEqual(a['signature_sha256'],b['signature_sha256'])
        rel=a['relocations'][0];self.assertEqual(rel['kind'],'hi16_lo16')
        self.assertEqual(rel['target'],0x188123);self.assertEqual(rel['lo_mode'],'addiu')

    def test_ori_address_with_only_memory_uses(self):
        a=body(i(15,0,8,0x18),i(13,8,8,0x1234),i(0x23,8,2,0),RET,0)
        result=self.norm(a);self.assertEqual(result['relocations'][0]['lo_mode'],'ori')

    def test_structure_memory_offset_is_retained(self):
        a=body(*pointer(0x181234),i(0x23,8,2,12),RET,0)
        b=body(*pointer(0x191234),i(0x23,8,2,16),RET,0)
        x=self.norm(a);y=self.norm(b)
        self.assertEqual(struct.unpack_from('<I',x['template'],8)[0]&0xffff,12)
        self.assertNotEqual(x['signature_sha256'],y['signature_sha256'])

    def test_small_bare_lui_offset_without_symbol_is_unproved(self):
        data=body(i(15,0,8,0x18),i(0x23,8,2,12),RET,0)
        result=self.norm(data);self.assertEqual(result['template'],data)
        self.assertTrue(result['unresolved'])

    def test_exact_base_symbol_keeps_struct_offset(self):
        data=body(i(15,0,8,0x18),i(0x23,8,2,12),RET,0)
        result=self.norm(data,data_symbols=[{'address':0x180000,'size':64}])
        rel=result['relocations'][0];self.assertIsNone(rel['low_offset'])
        self.assertEqual(struct.unpack_from('<I',result['template'],4)[0]&0xffff,12)

    def test_addiu_after_exact_global_base_is_a_field_constant(self):
        data=body(i(15,0,8,0x18),i(9,8,8,12),i(0x23,8,2,0),RET,0)
        result=self.norm(data,data_symbols=[{'address':0x180000,'size':64}])
        self.assertIsNone(result['relocations'][0]['low_offset'])
        self.assertEqual(struct.unpack_from('<I',result['template'],4)[0]&0xffff,12)

    def test_vu_payload_is_not_an_ee_function_allocation(self):
        with self.assertRaises(ValueError):
            self.norm(body(RET,0),sections=[{'name':'.vutext','address':BASE,'size':64,'flags':6,'type':1}])

    def test_direct_memory_signed_lo_requires_precise_symbol(self):
        data=body(i(15,0,8,0x19),i(0x23,8,2,0x8120),RET,0)
        self.assertEqual(self.norm(data)['template'],data)
        result=self.norm(data,data_symbols=[{'address':0x188120,'size':4}])
        self.assertEqual(result['relocations'][0]['lo_mode'],'memory')

    def test_ram_numeric_value_returned_is_not_address(self):
        data=body(*pointer(0x181234,2),RET,0)
        result=self.norm(data);self.assertEqual(result['template'],data)
        self.assertTrue(result['unresolved'])

    def test_scalar_packet_ori_constants_remain_distinct(self):
        a=body(i(15,0,8,0x3000),i(13,8,8,0x26),i(0x2b,28,8,0),RET,0)
        b=body(i(15,0,8,0x3000),i(13,8,8,0x29),i(0x2b,28,8,0),RET,0)
        x=self.norm(a);y=self.norm(b)
        self.assertNotEqual(x['signature_sha256'],y['signature_sha256'])
        self.assertEqual(struct.unpack_from('<I',x['template'],4)[0]&0xffff,0x26)
        self.assertEqual(struct.unpack_from('<I',y['template'],4)[0]&0xffff,0x29)

    def test_stored_ram_pointer_with_unknown_ownership_stays_raw(self):
        data=body(*pointer(0x1a6600),i(0x2b,28,8,0),RET,0)
        result=self.norm(data)
        self.assertEqual(result['template'][:8],data[:8])
        self.assertFalse(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))

    def test_gs_3ff000_scalar_argument_does_not_gain_pointer_type(self):
        data=body(*pointer(0x3ff000,4),j(3,0x101000),0,RET,0)
        result=self.norm(data)
        self.assertEqual(result['template'][:8],data[:8])
        self.assertEqual([rel['kind'] for rel in result['relocations']],['j26'])

    def test_pointer_used_locally_then_tail_passed_stays_unqualified(self):
        data=body(*pointer(0x181234,4),i(0x23,4,8,0),j(2,0x101000),0)
        result=self.norm(data)
        self.assertFalse(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))
        self.assertTrue(any('tail-transfer' in item['reason'] for item in result['unresolved']))

    def test_verified_callee_pointer_use_accepts_constructed_address(self):
        data=body(*pointer(0x181234,4),j(3,0x101000),0,RET,0)
        result=self.norm(data,**pointer_context())
        self.assertTrue(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))
        self.assertTrue(any('pointer_evidence' in rel for rel in result['relocations']))

    def test_pointer_evidence_does_not_allow_scalar_return_or_store(self):
        data=body(*pointer(0x181234,4),r(4,0,2,0x2d),i(0x2b,28,4,0),j(3,0x101000),0,RET,0)
        result=self.norm(data,**pointer_context())
        self.assertFalse(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))

    def test_pinned_aligned_pointer_keeps_memory_field_offset(self):
        data=body(i(15,0,4,0x18),i(0x23,4,8,12),j(3,0x101000),0,RET,0)
        result=self.norm(data,**pointer_context())
        addresses=[rel for rel in result['relocations'] if rel['kind']=='hi16_lo16']
        self.assertTrue(addresses);self.assertTrue(all(rel['low_offset'] is None for rel in addresses))
        self.assertEqual(struct.unpack_from('<I',result['template'],4)[0]&0xffff,12)

    def test_pointer_certificate_stale_callee_reference_or_tier_rejected(self):
        data=body(*pointer(0x181234,4),j(3,0x101000),0,RET,0)
        for key,value in [('callee_raw_sha256','0'*64),('reference_sha256','0'*64),
                          ('boundary_tier','inferred'),('proof_decoder_sha256','bad'),
                          ('argument_register',True)]:
            enriched=pointer_context();enriched['pointer_argument_roles'][0][key]=value
            with self.assertRaises(ValueError):self.norm(data,**enriched)

    def test_unmapped_effective_callee_dereference_keeps_address_raw(self):
        data=body(*pointer(0x181234,4),j(3,0x101000),0,RET,0)
        result=self.norm(data,**pointer_context(addend=0x100000))
        self.assertFalse(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))

    def test_delay_slot_argument_definition_is_checked_after_execution(self):
        data=body(i(15,0,4,0x18),j(3,0x101000),i(9,4,4,0x1234),RET,0)
        result=self.norm(data,**pointer_context())
        self.assertTrue(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))

    def test_regimm_link_flow_cannot_certify_pointer_addresses(self):
        data=body(*pointer(0x181234,4),i(1,0,17,2),0,i(0x23,4,2,0),RET,0)
        result=self.norm(data,**pointer_context())
        self.assertEqual(result['template'],data)
        self.assertTrue(any('REGIMM' in item['reason'] for item in result['unresolved']))

    def test_gpr_kill_and_no_backward_taint(self):
        killed=body(i(15,0,8,0x19),r(4,0,8,0x2d),i(0x23,8,2,0x8120),RET,0)
        self.assertEqual(self.norm(killed,data_symbols=[{'address':0x188120,'size':4}])['template'],killed)
        earlier_scalar=body(i(9,0,8,7),i(9,8,9,5),i(15,0,8,0x19),i(0x23,8,2,0x8120),RET,0)
        result=self.norm(earlier_scalar,data_symbols=[{'address':0x188120,'size':4}])
        self.assertEqual(result['relocations'][0]['high_offset'],8)

    def test_cfg_join_with_different_lui_values_is_unresolved(self):
        data=body(i(4,4,0,4),0,i(15,0,8,0x18),j(2,BASE+28),0,
                  i(15,0,8,0x19),0,i(0x23,8,2,0x120),RET,0)
        result=self.norm(data,data_symbols=[{'address':0x180120,'size':4},{'address':0x190120,'size':4}])
        self.assertFalse(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))
        self.assertTrue(any('join' in item['reason'] for item in result['unresolved']))

    def test_gp_requires_retail_initialization_proof(self):
        data=body(i(0x23,28,2,0x100),RET,0)
        self.assertEqual(self.norm(data,gp_verified=False)['template'],data)
        self.assertEqual(self.norm(data)['relocations'][0]['kind'],'gp16')
        with self.assertRaises(ValueError):self.norm(data,gp_evidence='')

    def test_gp_is_invalidated_by_real_register_write(self):
        data=body(i(9,0,28,0),i(0x23,28,2,0x100),RET,0)
        self.assertFalse(self.norm(data)['relocations'])

    def test_gp_is_not_assumed_preserved_across_unknown_call(self):
        data=body(j(3,0x101000),0,i(0x23,28,2,0x100),RET,0)
        self.assertFalse(any(r['kind']=='gp16' for r in self.norm(data)['relocations']))
        self.assertTrue(any(r['kind']=='gp16' for r in self.norm(data,gp_call_preserved=True,
            gp_call_preservation_evidence='Synthetic pinned callee GP preservation theorem')['relocations']))

    def test_s_register_after_call_is_not_preserved_by_an_abi_guess(self):
        data=body(*pointer(0x181234,16),j(3,0x101000),0,i(0x23,16,2,0),RET,0)
        result=self.norm(data)
        self.assertFalse(any(rel['kind']=='hi16_lo16' for rel in result['relocations']))

    def test_cop2_rs28_vf_operands_are_not_gp_address_fields(self):
        data=body(i(0x12,28,17,0x1234),RET,0)
        result=self.norm(data);self.assertEqual(result['template'],data)
        self.assertFalse(result['relocations'])

    def test_cop_vector_memory_keeps_vf_operand_bits(self):
        data=body(i(0x36,28,17,0),RET,0)
        result=self.norm(data)
        self.assertEqual(struct.unpack_from('<I',result['template'])[0]>>16,
                         struct.unpack_from('<I',data)[0]>>16)

    def test_jal_needs_mapped_supported_entry(self):
        data=body(j(3,0x104000),0,RET,0)
        self.assertEqual(self.norm(data)['template'],data)

    def test_external_call_alias_pattern_is_retained(self):
        a=body(j(3,0x101000),0,j(3,0x101000),0,RET,0)
        b=body(j(3,0x102000),0,j(3,0x103000),0,RET,0)
        self.assertNotEqual(self.norm(a)['signature_sha256'],self.norm(b)['signature_sha256'])
        c=body(j(3,0x102000),0,j(3,0x102000),0,RET,0)
        self.assertEqual(self.norm(a)['signature_sha256'],self.norm(c)['signature_sha256'])

    def test_internal_j_target_is_relative_and_cfg_preserved(self):
        a=body(j(2,BASE+16),0,0,0,RET,0)
        result=self.norm(a)
        self.assertEqual(struct.unpack_from('<I',result['template'])[0]&0x3ffffff,4)
        moved=body(j(2,BASE+0x100+16),0,0,0,RET,0)
        shifted=m.normalize(moved,BASE+0x100,context())
        self.assertEqual(result['signature_sha256'],shifted['signature_sha256'])
        different=body(j(2,BASE+12),0,0,0,RET,0)
        self.assertNotEqual(result['signature_sha256'],self.norm(different)['signature_sha256'])

    def test_tail_nop_and_stack_offsets_remain_part_of_identity(self):
        a=body(i(0x23,29,2,16),RET,0)
        b=body(i(0x23,29,2,20),RET,0)
        self.assertNotEqual(self.norm(a)['signature_sha256'],self.norm(b)['signature_sha256'])
        self.assertEqual(len(self.norm(a)['template']),len(a))

    def test_unsupported_mmi_kills_address_proof(self):
        data=body(*pointer(0x181234),i(0x1c,8,9,0),i(0x23,8,2,0),RET,0)
        self.assertFalse(any(r['kind']=='hi16_lo16' for r in self.norm(data)['relocations']))

    def test_public_metadata_contains_no_instruction_words_or_raw_bytes(self):
        result=self.norm(body(j(3,0x101000),0,RET,0))
        self.assertFalse(any(key in rel for rel in result['relocations'] for key in ('word','raw','bytes','instruction')))
        self.assertEqual(result['certificate']['normalizer_sha256'],m.NORMALIZER_SHA256)

    def test_context_preparation_reuses_verified_entries(self):
        prepared=m.prepare_context(context());self.assertIs(m.prepare_context(prepared),prepared)
        self.assertTrue(m.normalize(body(RET,0),BASE,prepared)['certificate']['exact'])


if __name__=='__main__':unittest.main()
