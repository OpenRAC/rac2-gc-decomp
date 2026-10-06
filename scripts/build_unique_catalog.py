"""Recheck private reference spans and build metadata-only global catalogue chunks."""
from __future__ import annotations
import argparse
import collections
import gzip
import hashlib
import io
import json
from pathlib import Path
import sys
import struct

import elf_tools
import relocation_identity as identity
import unique_code_report

ROOT = Path(__file__).resolve().parents[1]

def sha(data):
    return hashlib.sha256(data).hexdigest()

def encoded(value):
    return (json.dumps(value, sort_keys=True, separators=(',', ':')) + '\n').encode()

def body(data, elf, address, size):
    owners = [s for s in elf['sections'] if s['type'] == 1 and s['flags'] & 6 == 6
              and s['name'] in ('.text', 'core.text') and s['address'] <= address
              and address + size <= s['address'] + s['size']]
    if len(owners) != 1:
        raise ValueError('Complete catalogue interval is outside one EE code section')
    section = owners[0]
    offset = section['offset'] + address - section['address']
    result = data[offset:offset + size]
    if len(result) != size:
        raise ValueError('Truncated catalogue interval')
    return result


def static_dependencies(original, address):
    """Record every external direct control target, including unmasked branches."""
    words = struct.unpack('<' + 'I' * (len(original) // 4), original)
    result = []
    for index, word in enumerate(words):
        control = identity._control(word, index, address, len(words))
        # The frozen relocation decoder deliberately omits COP0 branches.
        # Catalogue completeness also includes the R5900 BC0F/T/FL/TL family.
        if control is None and word >> 26 == 0x10 and (word >> 21) & 31 == 8 and (word >> 16) & 31 in (0, 1, 2, 3):
            immediate = word & 0xffff
            displacement = immediate - 0x10000 if immediate & 0x8000 else immediate
            control = ('branch', address + index * 4 + 4 + displacement * 4, bool((word >> 16) & 2))
        if control is None:
            continue
        kind, target, _likely = control
        if type(target) is int and not address <= target < address + len(original):
            result.append({'offset': index * 4, 'kind': kind, 'target': target})
    return result

def build(repo, boundary_path, references, destination, pointer_evidence=None):
    repo, references, destination = Path(repo).resolve(), Path(references).resolve(), Path(destination).resolve()
    if references.is_relative_to(repo) or repo.is_relative_to(references):
        raise ValueError('References must remain private outside repository')
    if destination.exists():
        raise ValueError('Catalogue destination exists; preserve it and use a fresh path')
    if destination.is_relative_to(repo) and destination.parent != repo / 'config':
        raise ValueError('Public metadata catalogue must be a direct config subdirectory')
    raw_boundary = Path(boundary_path).read_bytes()
    boundary = json.loads(raw_boundary)
    if type(boundary.get('schema')) is not int or boundary.get('schema') != 1 or boundary.get('target') != 'SCUS_972.68' or boundary.get('qualified_extent_failures'):
        raise ValueError('Boundary catalogue identity or qualification failure')
    scope = json.loads((repo / 'config/progress-scope.json').read_bytes())
    required = ['config/target.json','config/overlays.json','config/progress-scope.json',
                'progress/report.json','progress/integration.json','progress/candidates.json',
                'scripts/decomp_report.py','scripts/unique_code_report.py','scripts/relocation_identity.py',
                'scripts/build_unique_catalog.py']
    if (repo / 'scripts/global_function_catalog.py').exists():
        required.append('scripts/global_function_catalog.py')
    required.append('scripts/call_graph_refinement.py')
    pointer_records, pointer_provenance, pointer_payload_pin = {}, None, None
    if pointer_evidence is not None:
        import pointer_evidence_loader
        pointer_evidence = Path(pointer_evidence).resolve()
        payload = pointer_evidence.read_bytes()
        pointer_payload_pin = sha(payload)
        expanded = payload
        if pointer_evidence.suffix == '.gz':
            with gzip.GzipFile(fileobj=io.BytesIO(payload), mode='rb') as stream:
                expanded = stream.read(256 * 1024 * 1024 + 1)
        if len(expanded) > 256 * 1024 * 1024:
            raise ValueError('Pointer evidence exceeds metadata size limit')
        report = json.loads(expanded)
        if type(report.get('schema')) is not int or report['schema'] != 1 or report.get('target') != boundary['target']:
            raise ValueError('Pointer evidence schema or target mismatch')
        proof_file, loader_file = repo / 'scripts/data_role_evidence_v2.py', repo / 'scripts/pointer_evidence_loader.py'
        proof_sha, loader_sha = sha(proof_file.read_bytes()), sha(loader_file.read_bytes())
        dependency_sha = sha((repo / 'scripts/relocation_identity.py').read_bytes())
        if (report.get('proof_decoder_sha256') != proof_sha
                or report.get('decoder_dependency_sha256') != dependency_sha
                or sha(Path(pointer_evidence_loader.__file__).read_bytes()) != loader_sha
                or sha(Path(pointer_evidence_loader.data_role_evidence.__file__).read_bytes()) != proof_sha):
            raise ValueError('Pointer evidence decoder or replay source pin mismatch')
        required += ['scripts/data_role_evidence_v2.py', 'scripts/pointer_evidence_loader.py']
        if (repo / 'scripts/scan_pointer_roles.py').exists():
            required.append('scripts/scan_pointer_roles.py')
        if pointer_evidence.is_relative_to(repo):
            required.append(pointer_evidence.relative_to(repo).as_posix())
        # Independent replay reads pinned complete callee bytes. A subset of
        # recorded dereferences cannot qualify an incoming argument theorem.
        pointer_provenance = {'artifact_sha256': pointer_payload_pin, 'expanded_sha256': sha(expanded),
                              'proof_decoder_sha256': proof_sha, 'decoder_dependency_sha256': dependency_sha,
                              'replay_loader_sha256': loader_sha}
    required += ['progress/levels/' + p['name'].split('/')[1] + '.json' for p in scope['programs'] if p['name'] != 'boot']
    frozen_inputs = {name:sha((repo/name).read_bytes()) for name in sorted(set(required))}
    if pointer_evidence is not None:
        pointer_records = pointer_evidence_loader.load_pointer_evidence(
            pointer_evidence, boundary, references, identity, replay=True)
        pointer_provenance['replayed_records'] = sum(len(records) for records in pointer_records.values())
    normalizer_sha = sha((repo / 'scripts/relocation_identity.py').read_bytes())
    groups = collections.defaultdict(list)
    for row in boundary['functions']:
        groups[row['program']].append(row)
    contexts = {ctx['program']: ctx for ctx in boundary['contexts']}
    if len(contexts) != len(boundary['contexts']) or set(groups) != {p['name'] for p in scope['programs']} or set(contexts) != set(groups):
        raise ValueError('All pinned programs must be present exactly once')
    for program in scope['programs']:
        if boundary['reference_pins'].get(program['name']) != program['sha256'] or contexts[program['name']].get('reference_sha256') != program['sha256']:
            raise ValueError('Boundary reference identity differs from pinned progress scope')
    staged = []
    summary = {'functions': 0, 'reconstruction_checks': 0, 'relocations': collections.Counter(),
               'unresolved': collections.Counter(), 'boundary_status': collections.Counter()}
    normalizer_failures = []
    for program in sorted(groups):
        path = references / ('boot.elf' if program == 'boot' else program + '/overlay.elf')
        data = path.read_bytes()
        if sha(data) != boundary['reference_pins'][program]:
            raise ValueError('Changed pinned reference: ' + program)
        elf = elf_tools._parse(data)
        context = dict(contexts[program])
        if pointer_evidence is not None:
            context['function_pins'] = {str(row['address']): {
                'size': row['size'], 'raw_sha256': row['raw_sha256'], 'boundary_tier': row['boundary']['status']}
                for row in groups[program] if row['boundary']['status'] in ('qualified_complete', 'flow_supported_inferred')}
            context['pointer_argument_roles'] = pointer_records.get(program, [])
        prepared = identity.prepare_context(context)
        compact = []
        previous = -1
        for row in sorted(groups[program], key=lambda r:r['address']):
            address, size = row['address'], row['size']
            if type(address) is not int or type(size) is not int or size <= 0 or address % 4 or size % 4 or address < previous:
                raise ValueError('Invalid/overlapping boundary interval')
            previous = address + size
            original = body(data, elf, address, size)
            digest = sha(original)
            if digest != row['raw_sha256']:
                raise ValueError('Boundary span differs from reference')
            status = row['boundary']['status']
            try:
                result = identity.normalize(original, address, prepared)
                restored = identity.reconstruct(result['template'], address, result['relocations'])
                if restored != original or result['certificate']['raw_sha256'] != digest or result['certificate']['exact'] is not True:
                    raise ValueError('Normalizer round trip is not exact')
                if result['certificate']['normalizer_sha256'] != normalizer_sha:
                    raise ValueError('Normalizer instrument changed')
                role_schema = [{k:v for k,v in relocation.items() if k in
                    ('offset','kind','high_offset','low_offset','lo_mode','role','internal','relative_target')}
                    for relocation in result['relocations']]
                expected_signature = sha(b'ee-relocation-template-v1\0' + result['template'] +
                    json.dumps(role_schema, sort_keys=True, separators=(',', ':')).encode())
                if result['certificate']['normalized_sha256'] != sha(result['template']) or result['signature_sha256'] != expected_signature or result['certificate']['reconstructed_sha256'] != digest:
                    raise ValueError('Normalizer receipt hashes contradict the checked bytes')
                signature, relocations = result['signature_sha256'], result['relocations']
                cert = {key: result['certificate'][key] for key in
                        ('exact', 'normalized_sha256', 'reconstructed_sha256')}
                summary['relocations'].update(r['kind'] for r in relocations)
                summary['unresolved'].update(str(u.get('reason', u.get('kind', 'retained'))) if isinstance(u, dict) else str(u) for u in result['unresolved'])
                summary['reconstruction_checks'] += 1
            except (ValueError, KeyError, TypeError) as error:
                # No guessed identity and no fake receipt. An unsupported interval
                # remains an uncollapsed raw unit and a preserved diagnostic.
                signature, relocations, cert = None, [], None
                if status != 'ambiguous_fragment':
                    status = 'inferred'
                normalizer_failures.append({'program': program, 'address': address, 'size': size, 'error': str(error)})
            aliases = [row['qualified_symbol']] if row.get('qualified_symbol') else []
            evidence = row['boundary']['evidence']
            compact.append([address, size, digest, status, signature, relocations, aliases, evidence, cert,
                            static_dependencies(original, address)])
            summary['functions'] += 1
            summary['boundary_status'][status] += 1
        payload = b''.join(encoded(row) for row in compact)
        buffer = io.BytesIO()
        with gzip.GzipFile(filename='', mode='wb', fileobj=buffer, mtime=0) as stream:
            stream.write(payload)
        compressed = buffer.getvalue()
        filename = ('boot' if program == 'boot' else program.split('/')[1]) + '.ndjson.gz'
        staged.append((filename, compressed, {'path': filename, 'format': 'compact-ndjson-v1',
                      'program': program, 'compression': 'gzip', 'sha256': sha(compressed),
                      'expanded_sha256': sha(payload), 'max_expanded_bytes': len(payload) + 1}))
        print(json.dumps({'program':program,'functions':len(compact),'metadata_bytes':len(compressed)}), flush=True)
    if any(sha((repo/name).read_bytes()) != pin for name,pin in frozen_inputs.items()):
        raise ValueError('Catalogue inputs changed during global normalization')
    if pointer_evidence is not None and sha(pointer_evidence.read_bytes()) != pointer_payload_pin:
        raise ValueError('Pointer evidence changed during global normalization')
    manifest = {'schema':1,'target':'SCUS_972.68','normalizer':{'id':'conservative-ee-relocation-template-v1','sha256':normalizer_sha},
                'section_identity_policy':'named-pinned-ee-sections-v1',
                'group_policy':'graph-refined-structural-templates',
                'dependency_policy':'complete-static-control-dependencies',
                'data_policy':'retain-unowned-data-address-operands',
                'dependency_extraction':'pinned-r5900-static-control-including-cop0-branches',
                'input_pins':[{'path':name,'sha256':pin} for name,pin in frozen_inputs.items()],
                'programs':[{'program':p['name'],'reference_sha256':p['sha256'],
                            'ee_sections':[{'name':s['name'],'address':s['address'],'size':s['size']} for s in p['sections'] if s['flags'] & 4 and s['name'] != '.vutext'],
                            'excluded_vu_bytes':sum(s['size'] for s in p['sections'] if s['name'] == '.vutext')} for p in scope['programs']],
                'function_chunks':[entry for _,_,entry in staged],
                'provenance':{'boundary_catalogue_sha256':sha(raw_boundary),'boundary_decoder':boundary['decoder'],
                              'method':'Private pinned-byte/CFG extraction, conservative address-flow checks, full exact reconstruction; no original-source-boundary claim'},
                'generation':{**summary,'normalizer_failures':len(normalizer_failures)}}
    if pointer_provenance is not None:
        manifest['provenance']['pointer_evidence'] = pointer_provenance
    destination.mkdir(parents=True, exist_ok=False)
    for filename, compressed, _ in staged:
        (destination/filename).write_bytes(compressed)
    (destination/'catalog.json').write_bytes(encoded(manifest))
    # Validate the exact encoded public metadata through the independent loader.
    expanded, _ = unique_code_report.read_catalog(destination/'catalog.json')
    unique_code_report.validate_catalog(expanded)
    return manifest, normalizer_failures

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=ROOT)
    parser.add_argument('--boundaries', required=True, type=Path)
    parser.add_argument('--references', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--diagnostics', required=True, type=Path)
    parser.add_argument('--pointer-evidence', type=Path,
                        help='optional pinned JSON/gzip pointer metadata; every theorem is replayed before use')
    args = parser.parse_args()
    if args.diagnostics.exists() or args.diagnostics.resolve().is_relative_to(args.repo.resolve()):
        raise ValueError('Diagnostics must be a new private file')
    manifest, failures = build(args.repo,args.boundaries,args.references,args.output,args.pointer_evidence)
    args.diagnostics.parent.mkdir(parents=True,exist_ok=True)
    with args.diagnostics.open('xb') as stream:
        stream.write(encoded({'generation':manifest['generation'],'failures':failures}))
    print(json.dumps(manifest['generation']), flush=True)

if __name__ == '__main__':
    main()
