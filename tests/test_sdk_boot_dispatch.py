import json
"""SDK boot dispatch and full assembly ownership; no compiler execution."""
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
import integration
import boot_sdk_unit as sdk


class SDKBootDispatch(unittest.TestCase):
    def test_valid_sdk_assembly_alias_requires_exact_owner(self):
        function = {**sdk.FUNCTION, 'unit_id': sdk.UNIT, 'origin': 'boot-sdk',
                    'candidate_source': sdk.SOURCE, 'input_section': '.text'}
        address = function['address']
        text = f'.globl func_{address:08X}\nfunc_{address:08X}:\n'
        text += ''.join(f'/* 000000 {address + i:08X} 00000000 */ nop\n'
                        for i in range(0, 152, 4))
        pieces = integration.split_assembly(text, [function])
        self.assertEqual(len(pieces), 1)
        self.assertEqual(pieces[0]['kind'], 'c')
        for key, value in [('unit_id', 'default-gnu8bed'), ('candidate_source', 'other.c'),
                           ('input_section', '.text.fake'), ('symbol', 'OTHER')]:
            wrong = dict(function, **{key: value})
            with self.assertRaises(ValueError):
                integration.split_assembly(text, [wrong])

    def test_default_boot_symbol_guard_remains(self):
        with self.assertRaises(ValueError):
            integration.split_assembly('', [{'symbol': 'ARBITRARY', 'address': 0x1000, 'size': 8}])

    def test_restart_sdk_alias_requires_exact_finite_owner(self):
        spec = sdk.unit_spec(sdk.RESTART336)
        function = {**spec['function'], 'unit_id': sdk.RESTART336, 'origin': 'boot-sdk',
                    'candidate_source': spec['source'], 'input_section': '.text'}
        address = function['address']
        text = f'.globl func_{address:08X}\nfunc_{address:08X}:\n'
        text += ''.join(f'/* 000000 {address + i:08X} 00000000 */ nop\n'
                        for i in range(0, function['size'], 4))
        pieces = integration.split_assembly(text, [function])
        self.assertEqual([p['kind'] for p in pieces], ['c'])
        for key, value in [('unit_id', sdk.UNIT), ('unit_id', 'sdk-unknown'),
                           ('candidate_source', sdk.SOURCE), ('origin', 'level-native'),
                           ('input_section', '.text.fake'), ('symbol', 'OTHER'),
                           ('size', 332), ('address', address + 4)]:
            wrong = dict(function, **{key: value})
            with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                integration.split_assembly(text, [wrong])

    def test_no_sdk_catalog_preserves_exact_default_dispatch(self):
        expected = ({'functions': []}, Path('default.o'), {'cc1': 'old'})
        with patch.object(integration, 'compile_c', return_value=expected) as default:
            with patch.object(Path, 'exists', return_value=False):
                result = integration.compile_boot_c(Path('reference'), Path('private'), Path('tools'))
        self.assertEqual(result, expected)
        default.assert_called_once()

    def test_overlay_dispatch_does_not_invoke_the_present_sdk_boot_owner(self):
        tools = {'cc1': 'default'}
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            catalog = root / sdk.CATALOG
            catalog.parent.mkdir(parents=True)
            catalog.write_text('{}', encoding='utf8')
            shared_object = root / 'shared.o'
            shared_object.write_bytes(b'raw shared compiler fixture')
            link_object = root / 'shared-link.o'
            link_object.write_bytes(b'derived shared link fixture')
            adapter = root / 'adapter.json'
            adapter.write_text(json.dumps({'compiled_object_sha256': integration.file_hash(shared_object),
                'link_object_sha256': integration.file_hash(link_object), 'relocation_changes': []}))
            shared_catalog = {'functions': [], 'shared_link_object': {
                'path': link_object.name, 'sha256': integration.file_hash(link_object),
        'adapter_path': adapter.name, 'adapter_sha256': integration.file_hash(adapter),
        'compiled_path': shared_object.name, 'compiled_object_sha256': integration.file_hash(shared_object)}}
            review = root / 'progress/candidates.json'
            review.parent.mkdir()
            review.write_text(json.dumps({'object_sha256': integration.file_hash(shared_object)}))
            expected = (shared_catalog, shared_object, tools)
            with patch.object(integration, 'ROOT', root), \
                    patch.object(integration, '_compile_shared_level_c', return_value=expected) as shared, \
                    patch.object(sdk, 'compile_reviewed', side_effect=AssertionError('SDK is boot-only')) as sdk_compile:
                result = integration.compile_level_c(Path('reference'), root, Path('tools'), '0_aranos_tutorial')
            self.assertEqual(result, (shared_catalog, link_object, tools))
            shared.assert_called_once()
            sdk_compile.assert_not_called()


if __name__ == '__main__':
    unittest.main()
