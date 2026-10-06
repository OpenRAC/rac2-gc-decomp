"""Synthetic combined-image receipt checks; no retail images or runtime claim."""
import copy
import gzip
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'scripts'))
import validate_boot_binding as validator
import call_graph_refinement as graph


def fixture():
    pins = {key: character * 64 for key, character in [('proof_source_sha256', 'c'), ('reader_sha256', 'd'), ('decoder_source_sha256', 'e')]}
    chunks = [{'path': 'boot.ndjson.gz', 'sha256': 'f' * 64}]
    def row(identity, program, address, signature):
        raw = validator.sha(identity.encode())
        return {'id': identity, 'program': program, 'address': address, 'size': 8, 'raw_sha256': raw,
                'boundary': {'status': 'qualified_complete'}, 'call_dependencies': [],
                'normalization': {'signature_sha256': signature, 'relocations': [],
                                  'certificate': {'exact': True, 'raw_sha256': raw, 'reconstructed_sha256': raw}}}
    rows = [row('boot-body', 'boot', 0x1000, 'return-one'), row('caller-a', 'levels/a', 0x2000, 'caller'),
            row('caller-b', 'levels/b', 0x3000, 'caller')]
    for r in rows[1:]:
        r['call_dependencies'] = [{'offset': 0, 'kind': 'call', 'target': 0x1000}]
    programs, images = [], []
    for program, address, pin in [('boot', 0x1000, 'a' * 64), ('levels/a', 0x2000, 'b' * 64), ('levels/b', 0x3000, 'b' * 64)]:
        name = 'core.text' if program == 'boot' else '.text'
        section = {'name': name, 'address': address, 'size': 16, 'flags': 6, 'type': 1, 'offset': 0}
        programs.append({'program': program, 'reference_sha256': pin,
                         'ee_sections': [{'name': name, 'address': address, 'size': 16}]})
        images.append({'program': program, 'reference_sha256': pin, 'allocated_sections': [section],
                       'load_segments': [{'type': 1, 'flags': 7, 'address': address, 'offset': 0, 'filesz': 16, 'memsz': 32}]})
    target = rows[0]
    bodies = [{'id': target['id'], 'address': target['address'], 'size': target['size'],
               'raw_sha256': target['raw_sha256'], 'boundary_status': target['boundary']['status']}]
    bindings = [{'caller_id': r['id'], 'caller_address': r['address'], 'caller_size': r['size'], 'caller_raw_sha256': r['raw_sha256'],
                 'program': r['program'], 'offset': 0, 'kind': 'call', 'target_program': 'boot', 'target': target['address'],
                 'target_id': target['id'], 'target_size': target['size'], 'target_raw_sha256': target['raw_sha256'],
                 'scope': validator.POLICY, 'runtime_preservation_proven': False} for r in rows[1:]]
    proof = {'schema': 1, 'target': 'SCUS_972.68', 'policy': validator.POLICY, 'runtime_preservation_proven': False,
             'catalog_sha256': '1' * 64, 'source_function_chunks_sha256': validator.chunks_digest(chunks), **pins,
             'programs': images, 'boot_core': images[0]['allocated_sections'][0], 'target_bodies': bodies, 'bindings': bindings}
    return proof, rows, programs, chunks, pins


class BootBindingTests(unittest.TestCase):
    def validate(self, fixture_values):
        return validator.validate_boot_binding(*fixture_values)

    def test_only_exact_validated_edges_resolve_boot_and_no_runtime_claim(self):
        values = fixture()
        proof, rows, programs, _, _ = values
        unresolved = graph.refine_call_groups(rows, programs)
        self.assertNotEqual(unresolved['class_by_id']['caller-a'], unresolved['class_by_id']['caller-b'])
        checked = self.validate(values)
        result = graph.refine_call_groups(rows, programs, checked)
        self.assertEqual(result['class_by_id']['caller-a'], result['class_by_id']['caller-b'])
        self.assertEqual(result['combined_reference_boot_edges'], 2)
        self.assertFalse(result['runtime_preservation_proven'])
        with self.assertRaises(TypeError):
            checked.edges[('forged', 0, 'call', 0x1000)] = 'boot-body'
        with self.assertRaises(ValueError):
            graph.refine_call_groups(rows, programs, proof)

    def test_explicit_target_program_boot_does_not_bypass_proof(self):
        values = fixture()
        _, rows, programs, _, _ = values
        for row in rows[1:]:
            row['call_dependencies'][0]['target_program'] = 'boot'
        result = graph.refine_call_groups(rows, programs)
        self.assertEqual(result['unresolved_edge_count'], 2)
        self.assertNotEqual(result['class_by_id']['caller-a'], result['class_by_id']['caller-b'])
        checked = self.validate(values)
        self.assertEqual(graph.refine_call_groups(rows, programs, checked)['combined_reference_boot_edges'], 2)

    def test_partial_receipt_resolves_only_covered_edge(self):
        values = fixture()
        values[0]['bindings'] = values[0]['bindings'][:1]
        checked = self.validate(values)
        result = graph.refine_call_groups(values[1], values[2], checked)
        self.assertEqual(result['combined_reference_boot_edges'], 1)
        self.assertEqual(result['unresolved_edge_count'], 1)
        self.assertNotEqual(result['class_by_id']['caller-a'], result['class_by_id']['caller-b'])

    def test_forged_reference_source_chunks_or_body_pin_is_refused(self):
        for mutate in (lambda p: p['programs'][1].update(reference_sha256='4' * 64),
                       lambda p: p.update(proof_source_sha256='4' * 64),
                       lambda p: p.update(source_function_chunks_sha256='4' * 64),
                       lambda p: p['target_bodies'][0].update(raw_sha256='4' * 64),
                       lambda p: p['bindings'][0].update(caller_raw_sha256='4' * 64),
                       lambda p: p['bindings'][0].update(target_size=12),
                       lambda p: p['bindings'][0].update(target_id='missing'),
                       lambda p: p.update(runtime_preservation_proven=True),
                       lambda p: p.update(schema=True)):
            values = fixture()
            mutate(values[0])
            with self.assertRaises(ValueError):
                self.validate(values)

    def test_changed_edge_and_duplicate_binding_are_refused(self):
        values = fixture()
        values[0]['bindings'][0]['offset'] = 4
        with self.assertRaisesRegex(ValueError, 'current exact static caller edge'):
            self.validate(values)
        values = fixture()
        values[0]['bindings'].append(copy.deepcopy(values[0]['bindings'][0]))
        with self.assertRaisesRegex(ValueError, 'Duplicate boot binding'):
            self.validate(values)

    def test_whole_boot_body_and_overlay_bss_ranges_are_checked(self):
        values = fixture()
        values[0]['programs'][0]['load_segments'][0]['filesz'] = 4
        with self.assertRaisesRegex(ValueError, 'executable file-backed segment'):
            self.validate(values)
        values = fixture()
        values[0]['programs'][1]['load_segments'][0]['filesz'] = 4
        with self.assertRaisesRegex(ValueError, 'executable file-backed segment'):
            self.validate(values)
        values = fixture()
        values[0]['programs'][1]['load_segments'].append({'type': 1, 'flags': 6, 'address': 0xf00,
                                                       'offset': 0, 'filesz': 0, 'memsz': 0x110})
        with self.assertRaisesRegex(ValueError, 'load/BSS extent overlaps'):
            self.validate(values)
        values = fixture()
        values[0]['programs'][1]['allocated_sections'].append({'name': '.bss', 'type': 8, 'flags': 3,
                                                             'address': 0x1000, 'size': 8, 'offset': 16})
        with self.assertRaisesRegex(ValueError, 'section overlaps'):
            self.validate(values)

    def test_names_alone_and_changed_target_section_identity_are_not_proof(self):
        values = fixture()
        values[0]['boot_core']['name'] = '.fake-core'
        with self.assertRaisesRegex(ValueError, 'section identities mismatch'):
            self.validate(values)
        values = fixture()
        values[2][0]['ee_sections'][0].pop('name')
        with self.assertRaisesRegex(ValueError, 'named pinned EE sections'):
            self.validate(values)

    def test_body_mutation_after_validation_does_not_reuse_old_mapping(self):
        values = fixture()
        checked = self.validate(values)
        values[1][1]['raw_sha256'] = '4' * 64
        with self.assertRaisesRegex(ValueError, 'body changed after validation'):
            graph.refine_call_groups(values[1], values[2], checked)

    def test_gzip_artifact_pins_and_malformed_payload(self):
        values = fixture()
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'boot-proof.json.gz'
            payload = gzip.compress(json.dumps(values[0]).encode(), mtime=0)
            path.write_bytes(payload)
            loaded = validator.load_boot_binding(path, validator.sha(payload), *values[1:])
            self.assertEqual(loaded.artifact_sha256, validator.sha(payload))
            path.write_bytes(payload + b'changed')
            with self.assertRaisesRegex(ValueError, 'artifact pin mismatch'):
                validator.load_boot_binding(path, validator.sha(payload), *values[1:])
            path.write_bytes(b'not gzip')
            with self.assertRaisesRegex(ValueError, 'Malformed gzip'):
                validator.load_boot_binding(path, validator.sha(b'not gzip'), *values[1:])


if __name__ == '__main__':
    unittest.main()
