"""Reproducible pinned-body incoming-pointer scan; writes metadata only."""
import argparse
import collections
import json
from pathlib import Path
import data_role_evidence_v2 as data_role_evidence
from pointer_evidence_loader import DECODER_SHA256,ReferenceImage,flattened_roles,load_decoder,sha

def scan(catalogue,reference_root,normalizer):
    groups=collections.defaultdict(list)
    for row in catalogue['functions']:
        if row['boundary']['status'] in ('qualified_complete','flow_supported_inferred'):groups[row['program']].append(row)
    proof_pin=sha(Path(data_role_evidence.__file__).read_bytes());results=[];counts={};failures=collections.Counter()
    for program,rows in groups.items():
        pin=catalogue['reference_pins'][program];image=ReferenceImage(reference_root,program,pin);accepted=[]
        for row in rows:
            body=image.body(row['address'],row['size'],row['raw_sha256'])
            try:accepted.extend(flattened_roles(body,dict(row,reference_sha256=pin),normalizer,proof_pin))
            except ValueError as exc:failures[str(exc)]+=1
        results.extend(accepted);counts[program]={'qualified_callees':len(set(r['callee_address'] for r in accepted)),
            'argument_roles':len(set((r['callee_address'],r['argument_register']) for r in accepted)),
            'dereference_records':len(accepted),'examined_callees':len(rows)}
        print(program,counts[program],flush=True)
    return {'schema':1,'target':catalogue['target'],'proof_decoder_sha256':proof_pin,
        'decoder_dependency_sha256':DECODER_SHA256,'counts':counts,'records':results,'cfg_rejections':dict(failures),
        'limits':['No original global allocation symbols claimed','SP spilled arguments and call preservation are rejected',
        'Scalar/return/store escapes rejected','This is context enrichment potential; no normalizer grouping credit measured']}

def main(argv=None):
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--boundaries',type=Path,required=True)
    parser.add_argument('--references',type=Path,required=True)
    parser.add_argument('--decoder-source',type=Path,required=True,help='Frozen V2 relocation decoder Python source')
    parser.add_argument('--output',type=Path,required=True,help='Fresh metadata JSON path; overwriting is forbidden')
    args=parser.parse_args(argv)
    if args.output.exists():parser.error('Output exists; supply a fresh path')
    if not args.output.parent.is_dir():parser.error('Output parent directory must already exist')
    norm=load_decoder(args.decoder_source);catalogue=json.loads(args.boundaries.read_bytes())
    report=scan(catalogue,args.references,norm)
    with args.output.open('x',encoding='utf8',newline='') as stream:json.dump(report,stream,separators=(',',':'))
    print('TOTAL',len(report['records']),'records',sum(v['argument_roles'] for v in report['counts'].values()),'roles',flush=True)
if __name__=='__main__':main()
