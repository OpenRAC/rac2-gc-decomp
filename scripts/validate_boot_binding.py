"""Validate pinned combined-reference boot edges, without a runtime theorem.

Retail bytes are checked by the pinned producer. This consumer checks freshness,
current catalogue membership and the complete static mapping/nonoverlap receipt.
Section names alone never authorize a cross-program edge.
"""
from dataclasses import dataclass, replace
import gzip
import io
import hashlib
import json
from pathlib import Path
import re
from types import MappingProxyType

POLICY = 'combined-pinned-reference-images'
SUPPORTED = {'qualified_complete', 'flow_supported_inferred'}


def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def hash_value(value):
    require(isinstance(value, str) and re.fullmatch('[0-9a-f]{64}', value), 'Invalid boot proof hash')


def chunks_digest(chunks):
    require(isinstance(chunks, list) and bool(chunks), 'Missing pinned function chunks')
    return sha(json.dumps(chunks, sort_keys=True, separators=(',', ':')).encode())


def number(value, *, positive=False):
    require(type(value) is int and (value > 0 if positive else value >= 0) and value <= 0xffffffff,
            'Invalid boot mapping integer')
    return value


def span(address, size):
    address, size = number(address), number(size)
    require(address + size <= 0x100000000, 'Boot mapping exceeds ELF32 address space')
    return address, address + size


def overlaps(address, size, other, other_size):
    a, b = span(address, size)
    c, d = span(other, other_size)
    return a < d and c < b


@dataclass(frozen=True)
class ValidatedBootBindings:
    edges: object
    receipt_sha256: str
    row_pins: object
    scope: str = POLICY
    runtime_preservation_proven: bool = False
    artifact_sha256: str | None = None
    source_catalog_sha256: str | None = None


def validate_boot_binding(proof, rows, programs, function_chunks, expected_source_pins):
    """Return immutable per-edge targets after current metadata/provenance checks."""
    require(type(proof.get('schema')) is int and proof['schema'] == 1
            and proof.get('target') == 'SCUS_972.68' and proof.get('policy') == POLICY
            and proof.get('runtime_preservation_proven') is False, 'Invalid combined-reference boot proof policy')
    for key in ('proof_source_sha256', 'reader_sha256', 'decoder_source_sha256'):
        hash_value(proof.get(key))
        hash_value(expected_source_pins.get(key))
        require(proof[key] == expected_source_pins[key], 'Stale boot proof source pin: ' + key)
    hash_value(proof.get('catalog_sha256'))
    if 'source_catalog_sha256' in proof:
        hash_value(proof['source_catalog_sha256'])
        require(proof['source_catalog_sha256'] == proof['catalog_sha256'], 'Original boot proof source context mismatch')
    require(proof.get('source_function_chunks_sha256') == chunks_digest(function_chunks), 'Stale boot proof function chunks')
    if isinstance(programs, list):
        require(len({p['program'] for p in programs}) == len(programs), 'Duplicate current program')
        programs = {p['program']: p for p in programs}
    by_id = {r['id']: r for r in rows}
    require(len(by_id) == len(rows), 'Duplicate current function id')
    entries = {(r['program'], r['address']): r for r in rows}
    require(len(entries) == len(rows), 'Duplicate current program entry')
    images = {}
    for image in proof.get('programs', []):
        program = image.get('program')
        require(program in programs and program not in images, 'Unknown or duplicate boot proof program')
        require(image.get('reference_sha256') == programs[program]['reference_sha256'], 'Boot proof reference pin mismatch')
        sections, segments = image.get('allocated_sections'), image.get('load_segments')
        require(isinstance(sections, list) and isinstance(segments, list) and bool(segments), 'Incomplete boot mapping receipt')
        for section in sections:
            require(isinstance(section.get('name'), str) and section['name'], 'Missing pinned section identity')
            span(section.get('address'), section.get('size'))
            number(section.get('type')); number(section.get('flags')); number(section.get('offset'))
            require(section['flags'] & 2, 'Nonallocated section in boot proof inventory')
        for segment in segments:
            require(segment.get('type') == 1 and type(segment.get('type')) is int, 'Boot proof contains a non-load segment')
            span(segment.get('address'), segment.get('memsz'))
            number(segment.get('filesz')); number(segment.get('flags')); number(segment.get('offset'))
            require(segment['filesz'] <= segment['memsz'], 'Invalid boot load extent')
        expected = programs[program].get('ee_sections', [])
        require(expected and all(isinstance(s.get('name'), str) and s['name'] for s in expected),
                'Boot binding requires current named pinned EE sections')
        actual = [(s['name'], s['address'], s['size']) for s in sections
                  if s['type'] == 1 and s['flags'] & 6 == 6 and 'vutext' not in s['name'].lower()]
        require(sorted(actual) == sorted((s['name'], s['address'], s['size']) for s in expected),
                'Boot proof named executable section identities mismatch')
        images[program] = image
    require('boot' in images, 'Missing pinned boot mapping')
    core = proof.get('boot_core', {})
    require(core in images['boot']['allocated_sections'], 'Boot core selector differs from pinned section inventory')
    require(any(s['name'] == core.get('name') and s['address'] == core.get('address')
                and s['size'] == core.get('size') for s in programs['boot']['ee_sections']),
            'Boot core is not current named executable scope')
    require(core.get('type') == 1 and core.get('flags', 0) & 6 == 6, 'Boot core must be executable file-backed code')

    def mapping(row, program='boot', require_core=True):
        start, end = span(row['address'], row['size'])
        require(type(row['size']) is int and row['size'] > 0 and row['size'] % 4 == 0 and row['address'] % 4 == 0,
                'Invalid complete boot binding body extent')
        owners = [s for s in images[program]['allocated_sections'] if s['type'] == 1 and s['flags'] & 6 == 6
                  and s['address'] <= start and end <= s['address'] + s['size']]
        require(len(owners) == 1 and (not require_core or owners[0] == core), 'Boot binding body crosses or lacks exact section ownership')
        owner = owners[0]
        offset = owner['offset'] + start - owner['address']
        loads = [s for s in images[program]['load_segments'] if s['flags'] & 1 and s['address'] <= start
                 and end <= s['address'] + s['filesz'] and s['offset'] + start - s['address'] == offset]
        require(len(loads) == 1, 'Boot target lacks unambiguous executable file-backed segment')

    targets = {}
    for target in proof.get('target_bodies', []):
        identity = target.get('id')
        row = by_id.get(identity)
        require(row is not None and identity not in targets and row['program'] == 'boot', 'Unknown or duplicate supported boot body')
        require(row['boundary']['status'] in SUPPORTED and target.get('boundary_status') == row['boundary']['status']
                and all(target.get(key) == row[key] for key in ('address', 'size', 'raw_sha256')),
                'Boot target body or supported boundary pin mismatch')
        certificate = (row.get('normalization') or {}).get('certificate') or {}
        require(certificate.get('exact') is True and certificate.get('raw_sha256') == row['raw_sha256']
                and certificate.get('reconstructed_sha256') == row['raw_sha256'],
                'Boot target lacks current exact reconstruction certificate')
        hash_value(row['raw_sha256']); number(row['size'], positive=True)
        mapping(row)
        targets[identity] = row
    validated, pinned_rows = {}, {}
    for binding in proof.get('bindings', []):
        caller = by_id.get(binding.get('caller_id'))
        target = targets.get(binding.get('target_id'))
        program = binding.get('program')
        require(caller is not None and caller['program'] == program and program != 'boot'
                and program in images and target is not None and binding.get('target_program') == 'boot',
                'Boot binding caller/program/target membership mismatch')
        require(binding.get('scope') == POLICY and binding.get('runtime_preservation_proven') is False,
                'Boot binding scope must remain static combined references')
        require(all(binding.get('caller_' + key) == caller[key] for key in ('address', 'size', 'raw_sha256')),
                'Boot binding caller body pin mismatch')
        mapping(caller, program, require_core=False)
        require(binding.get('target') == target['address'] and binding.get('target_size') == target['size']
                and binding.get('target_raw_sha256') == target['raw_sha256'], 'Boot binding target body pin mismatch')
        offset, kind, address = binding.get('offset'), binding.get('kind'), binding.get('target')
        require(type(offset) is int and offset % 4 == 0 and 0 <= offset <= caller['size'] - 4,
                'Invalid boot binding edge offset')
        require(any(edge.get('offset') == offset and edge.get('kind') == kind and edge.get('target') == address
                    and edge.get('target_program', 'boot') == 'boot' for edge in caller.get('call_dependencies', [])),
                'Boot binding is not a current exact static caller edge')
        require((program, address) not in entries, 'Boot binding would override a local program entry')
        for section in images[program]['allocated_sections']:
            require(not overlaps(core['address'], core['size'], section['address'], section['size']), 'Overlay section overlaps boot core')
        for segment in images[program]['load_segments']:
            require(not overlaps(core['address'], core['size'], segment['address'], segment['memsz']), 'Overlay load/BSS extent overlaps boot core')
        key = (caller['id'], offset, kind, address)
        require(key not in validated, 'Duplicate boot binding edge')
        validated[key] = target['id']
        for row in (caller, target):
            pinned_rows[row['id']] = (row['program'], row['address'], row['size'], row['raw_sha256'], row['boundary']['status'])
    return ValidatedBootBindings(MappingProxyType(validated), sha(json.dumps(proof, sort_keys=True, separators=(',', ':')).encode()),
                                 MappingProxyType(pinned_rows), source_catalog_sha256=proof['catalog_sha256'])


def load_boot_binding(path, expected_sha256, rows, programs, function_chunks, expected_source_pins):
    data = Path(path).read_bytes()
    hash_value(expected_sha256)
    require(sha(data) == expected_sha256, 'Boot binding artifact pin mismatch')
    if Path(path).suffix == '.gz':
        try:
            with gzip.GzipFile(fileobj=io.BytesIO(data), mode='rb') as stream:
                data = stream.read(64 * 1024 * 1024 + 1)
        except (OSError, EOFError) as error:
            raise ValueError('Malformed gzip boot binding metadata') from error
    require(len(data) <= 64 * 1024 * 1024, 'Boot binding metadata exceeds 64 MiB limit')
    result = validate_boot_binding(json.loads(data), rows, programs, function_chunks, expected_source_pins)
    return replace(result, artifact_sha256=expected_sha256)
