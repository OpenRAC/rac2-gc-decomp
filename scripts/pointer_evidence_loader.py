"""Load pinned metadata and replay its complete incoming-pointer theorems."""
import collections
import gzip
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import struct
import sys
import data_role_evidence_v2 as data_role_evidence

DECODER_SHA256='0b0fc7cd33595440549ad82b650c6a99c6843dad2752d4af32673db22b3dda86'
PROGRAM=re.compile(r'(?:boot|levels/[0-9]+_[a-z0-9_]+)\Z')
def sha(data):return hashlib.sha256(data).hexdigest()

def load_decoder(path):
    path=Path(path)
    if sha(path.read_bytes())!=DECODER_SHA256:raise ValueError('Frozen V2 decoder source pin mismatch')
    spec=importlib.util.spec_from_file_location('pointer_evidence_b80',path)
    module=importlib.util.module_from_spec(spec);sys.modules[spec.name]=module;spec.loader.exec_module(module)
    return module

class ReferenceImage:
    def __init__(self,root,program,pin):
        if not isinstance(program,str) or not PROGRAM.fullmatch(program):raise ValueError('Invalid program path')
        path=Path(root)/('boot.elf' if program=='boot' else program+'/overlay.elf')
        self.data=path.read_bytes()
        if sha(self.data)!=pin:raise ValueError('Reference pin changed')
        if len(self.data)<52 or self.data[:6]!=b'\x7fELF\x01\x01':raise ValueError('32-bit little-endian ELF required')
        phoff=struct.unpack_from('<I',self.data,28)[0];stride,count=struct.unpack_from('<HH',self.data,42)
        if stride<32 or not count or phoff+stride*count>len(self.data):raise ValueError('Invalid ELF segment table')
        self.segments=[struct.unpack_from('<8I',self.data,phoff+i*stride) for i in range(count)]
        if any(s[1]+s[4]>len(self.data) or s[4]>s[5] for s in self.segments if s[0]==1):raise ValueError('Invalid backed segment')
    def body(self,address,size,pin):
        if type(address) is not int or type(size) is not int or address<0 or address%4 or size<=0 or size%4:
            raise ValueError('Invalid body extent')
        owners=[s for s in self.segments if s[0]==1 and s[2]<=address and address+size<=s[2]+s[4]]
        if len(owners)!=1:raise ValueError('Unique file-backed body required')
        s=owners[0];off=s[1]+address-s[2];body=self.data[off:off+size]
        if sha(body)!=pin:raise ValueError('Body pin changed')
        return body

def flattened_roles(body,row,normalizer,proof_pin):
    roles=data_role_evidence.incoming_pointer_roles(body,row['address'],normalizer)
    result=[]
    for role in roles:
        for witness in role.pop('dereferences'):
            result.append(dict(role,**witness,program=row['program'],reference_sha256=row['reference_sha256'],
                boundary_tier=row['boundary']['status'],proof_decoder_sha256=proof_pin,
                decoder_dependency_sha256=DECODER_SHA256,proof_status='verified_incoming_dereference'))
    return result

def load_pointer_evidence(report_path,catalogue,reference_root,normalizer,replay=True):
    """Return records by program after complete theorem replay using frozen V2.

    Replay=False is forbidden for promotion: pins alone cannot prove truthful
    incoming-pointer witnesses. The caller must qualify every consumer too.
    """
    if not replay:raise ValueError('Incoming-pointer evidence requires theorem replay')
    if sha(Path(normalizer.__file__).read_bytes())!=DECODER_SHA256:raise ValueError('Frozen V2 decoder dependency mismatch')
    path=Path(report_path);payload=path.read_bytes()
    report=json.loads(gzip.decompress(payload) if path.suffix=='.gz' else payload)
    proof_pin=sha(Path(data_role_evidence.__file__).read_bytes())
    if (report.get('schema')!=1 or report.get('target')!=catalogue['target'] or
        report.get('proof_decoder_sha256')!=proof_pin or report.get('decoder_dependency_sha256')!=DECODER_SHA256):
        raise ValueError('Evidence schema/target/source dependency pin mismatch')
    rows={(r['program'],r['address']):r for r in catalogue['functions']
          if r['boundary']['status'] in ('qualified_complete','flow_supported_inferred')}
    grouped=collections.defaultdict(list)
    for record in report['records']:
        key=(record.get('program'),record.get('callee_address'));row=rows.get(key)
        if row is None:raise ValueError('Evidence callee is not a current eligible boundary')
        if (record.get('callee_size')!=row['size'] or record.get('callee_raw_sha256')!=row['raw_sha256'] or
            record.get('boundary_tier')!=row['boundary']['status'] or
            record.get('reference_sha256')!=catalogue['reference_pins'][key[0]] or
            record.get('proof_decoder_sha256')!=proof_pin or record.get('decoder_dependency_sha256')!=DECODER_SHA256):
            raise ValueError('Evidence callee/reference/decoder pin mismatch')
        grouped[key].append(record)
    images={};result=collections.defaultdict(list)
    for (program,address),records in grouped.items():
        if program not in images:images[program]=ReferenceImage(reference_root,program,catalogue['reference_pins'][program])
        row=rows[(program,address)];body=images[program].body(address,row['size'],row['raw_sha256'])
        replay_row=dict(row,reference_sha256=catalogue['reference_pins'][program])
        expected=flattened_roles(body,replay_row,normalizer,proof_pin)
        if sorted(json.dumps(r,sort_keys=True) for r in records)!=sorted(json.dumps(r,sort_keys=True) for r in expected):
            raise ValueError('Stored pointer-role witnesses differ from full theorem replay')
        result[program].extend(records)
    return dict(result)
