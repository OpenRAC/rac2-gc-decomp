import json
import os
from pathlib import Path
import struct
import sys
import tempfile
import unittest
import contextlib
import io
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/'scripts'))
import data_role_evidence_v2
from pointer_evidence_loader import DECODER_SHA256,flattened_roles,load_decoder,load_pointer_evidence,sha
import scan_pointer_roles

class LoaderTests(unittest.TestCase):
    def setUp(self):
        self.temp=tempfile.TemporaryDirectory();self.root=Path(self.temp.name)
        self.norm=load_decoder(Path(os.environ.get('POINTER_DECODER_SOURCE',ROOT/'scripts/relocation_identity.py')))
        self.body=struct.pack('<III',35<<26|4<<21|2<<16,31<<21|8,0)
        image=bytearray(96+len(self.body));image[:6]=b'\x7fELF\x01\x01'
        struct.pack_into('<I',image,28,52);struct.pack_into('<HH',image,42,32,1)
        struct.pack_into('<8I',image,52,1,96,0x100000,0,len(self.body),len(self.body),5,4);image[96:]=self.body
        (self.root/'boot.elf').write_bytes(image);pin=sha(image)
        self.row={'program':'boot','address':0x100000,'size':12,'raw_sha256':sha(self.body),'boundary':{'status':'qualified_complete'}}
        self.catalog={'target':'SCUS_972.68','functions':[self.row],'reference_pins':{'boot':pin}}
        proof_pin=sha(Path(data_role_evidence_v2.__file__).read_bytes())
        records=flattened_roles(self.body,dict(self.row,reference_sha256=pin),self.norm,proof_pin)
        self.report={'schema':1,'target':'SCUS_972.68','proof_decoder_sha256':proof_pin,'decoder_dependency_sha256':DECODER_SHA256,'records':records}
        self.path=self.root/'report.json';self.save()
    def tearDown(self):self.temp.cleanup()
    def save(self):self.path.write_text(json.dumps(self.report),encoding='utf8')
    def load(self):return load_pointer_evidence(self.path,self.catalog,self.root,self.norm)
    def test_verified_full_replay(self):self.assertEqual(len(self.load()['boot']),1)
    def test_forged_witness_rejected(self):
        self.report['records'][0]['dereference_offset']=64;self.save()
        with self.assertRaisesRegex(ValueError,'theorem replay'):self.load()
    def test_duplicate_witness_rejected(self):
        self.report['records']*=2;self.save()
        with self.assertRaisesRegex(ValueError,'theorem replay'):self.load()
    def test_reference_drift_rejected(self):
        (self.root/'boot.elf').write_bytes(b'changed')
        with self.assertRaisesRegex(ValueError,'Reference pin'):self.load()
    def test_source_pin_rejected(self):
        self.report['proof_decoder_sha256']='0'*64;self.save()
        with self.assertRaisesRegex(ValueError,'dependency pin'):self.load()
    def test_boundary_tier_rejected(self):
        self.catalog['functions'][0]['boundary']['status']='inferred'
        with self.assertRaisesRegex(ValueError,'eligible boundary'):self.load()
    def test_replay_required(self):
        with self.assertRaisesRegex(ValueError,'requires theorem replay'):load_pointer_evidence(self.path,self.catalog,self.root,self.norm,False)
    def test_cli_refuses_overwrite(self):
        with contextlib.redirect_stderr(io.StringIO()),self.assertRaises(SystemExit):scan_pointer_roles.main(['--boundaries','unused','--references','unused','--decoder-source','unused','--output',str(self.path)])
    def test_reproducible_scan_witnesses(self):
        with contextlib.redirect_stdout(io.StringIO()):report=scan_pointer_roles.scan(self.catalog,self.root,self.norm)
        self.assertEqual(report['records'],self.report['records'])
        self.assertEqual(report['counts']['boot']['argument_roles'],1)
    def test_cli_fresh_output(self):
        boundaries=self.root/'boundaries.json';boundaries.write_text(json.dumps(self.catalog),encoding='utf8')
        output=self.root/'fresh.json'
        with contextlib.redirect_stdout(io.StringIO()):scan_pointer_roles.main(['--boundaries',str(boundaries),'--references',str(self.root),'--decoder-source',self.norm.__file__,'--output',str(output)])
        self.assertEqual(json.loads(output.read_bytes())['records'],self.report['records'])
if __name__=='__main__':unittest.main()
