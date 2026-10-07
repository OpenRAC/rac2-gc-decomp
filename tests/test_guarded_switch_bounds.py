"""Positive bounded-switch replay plus guard/bypass/target negative controls."""
import importlib.util
from pathlib import Path
import struct
import sys
import unittest
sys.path.insert(0,str(Path(__file__).resolve().parents[1] / 'scripts'))
import guarded_switch_bounds as m
path=Path(__file__).resolve().parents[1] / 'scripts/global_function_catalog.py'
spec=importlib.util.spec_from_file_location('boundary_static',path);b=importlib.util.module_from_spec(spec);spec.loader.exec_module(b)
BASE=0x1000;TABLE=0x180100

def fixture(*,selector_clobber=False,predicate_clobber=False,bypass=False):
    items=[]
    if bypass:items+=[('entry',('bne',5,'branch_delay')),('entry_delay',0)]
    items += [('guard',0x2c830002)]
    if predicate_clobber:items += [('clobber',0x24030001)]
    items += [('guard_branch',('beq',3,'default')),('branch_delay',0x24840001 if selector_clobber else 0),
              ('lookup',0x3c020018),('scale',0x00041880),('low',0x24420100),('sum',0x00621821),
              ('load',0x8c640000),('jump',0x00800008),('jump_delay',0),
              ('case0',0x2402000b),('ret0',0x03e00008),('slot0',0),
              ('case1',0x24020016),('ret1',0x03e00008),('slot1',0),
              ('default',0x24020000),('retd',0x03e00008),('slotd',0)]
    labels={label:BASE+i*4 for i,(label,word) in enumerate(items)};words=[]
    for label,word in items:
        if isinstance(word,tuple):
            kind,reg,target=word;op=4 if kind=='beq' else 5
            word=op<<26|reg<<21|(((labels[target]-(labels[label]+4))//4)&65535)
        words.append(word)
    raw=struct.pack('<'+'I'*len(words),*words);controls={}
    for i,w in enumerate(words):
        valid,c,gp=b.decode_flow(w,BASE+i*4)
        assert valid
        if c:controls[BASE+i*4]=c
    blob=struct.pack('<II',labels['case0'],labels['case1'])
    sections=[{'name':'.data','type':1,'flags':3,'address':0x180000,'size':0x1000}]
    return raw,controls,labels,blob,sections

class SwitchProofTests(unittest.TestCase):
    def prove(self,**kwargs):
        raw,c,l,blob,sections=fixture(**kwargs)
        return raw,c,l,m.prove_tables(raw,BASE,c,lambda address,size:blob,sections)

    def test_guarded_switch_reaches_all_words_and_closes(self):
        raw,c,l,(proofs,failures)=self.prove()
        self.assertEqual(failures,[]);self.assertEqual(len(proofs),1)
        proof=proofs[0];self.assertEqual(proof['bound'],2);self.assertEqual(proof['table_address'],TABLE)
        old=m.cfg_replay(BASE,len(raw),c);new=m.cfg_replay(BASE,len(raw),c,{proof['jump']:proof['targets']})
        self.assertTrue(old['unknown']);self.assertFalse(new['unknown']);self.assertFalse(new['escapes'])
        self.assertEqual(len(new['reachable']),len(raw)//4)
        self.assertTrue(proof['table_writable']);self.assertFalse(proof['runtime_immutability_proven'])

    def test_selector_write_after_guard_rejects(self):
        self.assertFalse(self.prove(selector_clobber=True)[3][0])

    def test_predicate_write_before_branch_rejects(self):
        self.assertFalse(self.prove(predicate_clobber=True)[3][0])

    def test_branch_into_delay_slot_bypasses_guard_and_rejects(self):
        raw,c,l,(proofs,failures)=self.prove(bypass=True)
        self.assertFalse(proofs);self.assertTrue(any('dominate' in f['reason'] for f in failures))

    def test_external_or_unaligned_table_target_rejects(self):
        raw,c,l,blob,sections=fixture()
        for target in [0x2000,BASE+1]:
            bad=struct.pack('<II',l['case0'],target)
            proofs,failures=m.prove_tables(raw,BASE,c,lambda address,size:bad,sections)
            self.assertFalse(proofs)

    def test_no_bound_guard_rejects(self):
        raw,c,l,blob,sections=fixture();bad=bytearray(raw);struct.pack_into('<I',bad,0,0x24830002)
        proofs,failures=m.prove_tables(bytes(bad),BASE,c,lambda address,size:blob,sections)
        self.assertFalse(proofs)

    def test_index_scale_and_target_use_must_match(self):
        raw,c,l,blob,sections=fixture();bad=bytearray(raw);struct.pack_into('<I',bad,l['scale']-BASE,0x00041840)
        proofs,failures=m.prove_tables(bytes(bad),BASE,c,lambda address,size:blob,sections)
        self.assertFalse(proofs)

    def test_table_must_be_allocated_progbits_data(self):
        raw,c,l,blob,sections=fixture()
        for flags,kind in [(0,1),(6,1),(3,8)]:
            bad=[dict(sections[0],flags=flags,type=kind)]
            proofs,failures=m.prove_tables(raw,BASE,c,lambda address,size:blob,bad)
            self.assertFalse(proofs)

if __name__=='__main__':unittest.main(verbosity=2)
