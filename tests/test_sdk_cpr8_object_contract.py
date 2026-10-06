"""Synthetic CPR8 object admission; no compiler or reference asset required."""
from pathlib import Path
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'scripts'))
import boot_sdk_unit as sdk
import integration


def fixture(path, *, size=656, extra_data=False, relocation_type=4, addend=0,
            wrong_offset=False, missing_call=False, helper_info=16, linked=False,
            linked_helper_info=17, wrong_target=False, extra_function=False):
    spec = sdk.unit_spec(sdk.CPR8)
    names = b'\0.text\0.strtab\0.symtab\0.shstrtab\0.rodata\0.rel.text\0'
    strings = b'\0FUN_0012D808\0FUN_0011F5E0\0FUN_0011F628\0EXTRA\0'
    text = bytearray(size)
    rels = bytearray()
    rows = spec['relocations'][:-1] if missing_call else spec['relocations']
    for i, row in enumerate(rows):
        at = row['offset'] + (4 if wrong_offset and i == 0 else 0)
        if at + 4 <= size:
            struct.pack_into('<I', text, at, (3 << 26) | addend)
        symbol = 2 if row['symbol'] == 'FUN_0011F5E0' else 3
        rels.extend(struct.pack('<II', at, (symbol << 8) | relocation_type))
    address = spec['function']['address'] if linked else 0
    symbols = bytes(16) + struct.pack('<IIIBBH', 1, address, size, 18, 0, 1)
    for name, target in spec['externals'].items():
        symbols += struct.pack('<IIIBBH', strings.index(name.encode()),
                               target + (4 if wrong_target else 0) if linked else 0,
                               0, linked_helper_info if linked else helper_info,
                               0, 0xfff1 if linked else 0)
    if extra_function:
        symbols += struct.pack('<IIIBBH', strings.index(b'EXTRA'), 0, 4, 18, 0, 1)
    chunks = [('.text', 1, 6, text, 0, 0, 8, 0),
              ('.strtab', 3, 0, strings, 0, 0, 1, 0),
              ('.symtab', 2, 0, symbols, 2, 1, 4, 16),
              ('.shstrtab', 3, 0, names, 0, 0, 1, 0)]
    if not linked:
        chunks.append(('.rel.text', 9, 0, rels, 3, 1, 4, 8))
    if extra_data:
        chunks.append(('.rodata', 1, 2, b'1234', 0, 0, 4, 0))
    data = bytearray(52)
    headers = [bytes(40)]
    for name, kind, flags, payload, link, info, alignment, entrysize in chunks:
        data.extend(bytes((-len(data)) % alignment))
        offset = len(data)
        data.extend(payload)
        headers.append(struct.pack('<10I', names.index(name.encode() + b'\0'), kind, flags,
                                   address if name == '.text' else 0, offset, len(payload),
                                   link, info, alignment, entrysize))
    data.extend(bytes((-len(data)) % 4))
    section_offset = len(data)
    data.extend(b''.join(headers))
    ident = b'\x7fELF\x01\x01\x01' + bytes(9)
    data[:52] = struct.pack('<16sHHIIIIIHHHHHH', ident, 2 if linked else 1, 8, 1,
                           address, 0, section_offset, 0, 52, 32, 0, 40, len(headers), 4)
    path.write_bytes(data)


class CPR8ObjectContract(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / 'object.o'

    def test_exact_four_call_contract(self):
        fixture(self.path)
        result = sdk.inspect_unit_object(self.path, sdk.CPR8)
        self.assertEqual(result['relocations'], sdk.unit_spec(sdk.CPR8)['relocations'])
        self.assertEqual(result['helper_C_credit'], 0)

    def test_wrong_complete_extent_refused_before_link(self):
        for size in (140, 152, 736):
            with self.subTest(size=size):
                fixture(self.path, size=size)
                with self.assertRaises(ValueError):
                    sdk.inspect_unit_object(self.path, sdk.CPR8)

    def test_rel_type_offset_addend_and_missing_call_refused(self):
        for args in ({'relocation_type': 7}, {'wrong_offset': True}, {'addend': 1},
                     {'missing_call': True}):
            with self.subTest(args=args):
                fixture(self.path, **args)
                with self.assertRaises(ValueError):
                    sdk.inspect_unit_object(self.path, sdk.CPR8)

    def test_extra_data_function_and_wrong_undefined_helper_refused(self):
        for args in ({'extra_data': True}, {'extra_function': True}, {'helper_info': 17}):
            with self.subTest(args=args):
                fixture(self.path, **args)
                with self.assertRaises(ValueError):
                    sdk.inspect_unit_object(self.path, sdk.CPR8)

    def test_sn_absolute_object_and_notype_bindings_both_valid_without_helper_credit(self):
        for info in (16, 17):
            fixture(self.path, linked=True, linked_helper_info=info)
            sdk.inspect_linked_helpers(self.path, sdk.CPR8)

    def test_wrong_absolute_binding_refused(self):
        for args in ({'wrong_target': True}, {'linked_helper_info': 18}):
            fixture(self.path, linked=True, **args)
            with self.assertRaises(ValueError):
                sdk.inspect_linked_helpers(self.path, sdk.CPR8)

    def test_leaf_control_inspector_is_not_relaxed(self):
        fixture(self.path)
        with self.assertRaises(ValueError):
            sdk.inspect_unit_object(self.path, sdk.UNIT)

    def test_unknown_unit_never_selects_a_profile(self):
        for unit in ('other', None, True):
            with self.subTest(unit=unit), self.assertRaises(ValueError):
                sdk.inspect_unit_object(self.path, unit)

    def test_two_sdk_units_preserve_default_owner_and_explicit_externals(self):
        default = {'functions': [{'symbol': 'FUN_00001000', 'address': 0x1000, 'size': 8}],
                   'externals': dict(sdk.unit_spec(sdk.CPR8)['externals']),
                   'compiled_source_sha256': 'a' * 64}
        original = {'functions': [dict(f) for f in default['functions']],
                    'externals': dict(default['externals']),
                    'compiled_source_sha256': default['compiled_source_sha256']}

        def compiled(reference, directory, root, binding, unit):
            spec = sdk.unit_spec(unit)
            catalog = {'functions': [dict(spec['function'])], 'externals': dict(spec['externals']),
                       'source_sha256': spec['source_sha256'], 'profile_id': sdk.cp.SDK_PROFILE}
            return catalog, directory / 'qualified.o', {'unit_id': unit}

        with patch.object(integration, 'compile_c', return_value=(default, Path('default.o'), {'cc1': 'old'})), \
                patch.object(sdk, 'admitted_units', return_value=[sdk.UNIT, sdk.CPR8]), \
                patch.object(sdk, 'compile_reviewed', side_effect=compiled) as compile_sdk, \
                patch.object(sdk, 'file_hash', return_value='b' * 64):
            catalog, objects, tools = integration.compile_boot_c(Path('ref'), Path('private'), Path('tools'),
                                                                 sdk_binding=Path('binding.json'))
        self.assertEqual(default, original)
        self.assertEqual(objects['candidates/boot.c'], Path('default.o'))
        self.assertEqual(tools, {'cc1': 'old'})
        self.assertEqual(set(catalog['sdk_units']), {sdk.UNIT, sdk.CPR8})
        self.assertEqual(sum(f['size'] for f in catalog['functions']), 8 + 152 + 656)
        self.assertEqual(catalog['externals'], default['externals'])
        self.assertEqual([call.args[-1] for call in compile_sdk.call_args_list], [sdk.UNIT, sdk.CPR8])
        self.assertEqual(len(catalog['functions']), 3)  # ABS helpers gain no function rows.

    def test_conflicting_absolute_external_does_not_enter_union(self):
        default = {'functions': [], 'externals': {'FUN_0011F5E0': 0x1234},
                   'compiled_source_sha256': 'a' * 64}
        spec = sdk.unit_spec(sdk.CPR8)
        compiled = ({'functions': [spec['function']], 'externals': spec['externals'],
                     'source_sha256': spec['source_sha256'], 'profile_id': sdk.cp.SDK_PROFILE},
                    Path('sdk.o'), {})
        with patch.object(integration, 'compile_c', return_value=(default, Path('default.o'), {})), \
                patch.object(sdk, 'admitted_units', return_value=[sdk.CPR8]), \
                patch.object(sdk, 'compile_reviewed', return_value=compiled), \
                self.assertRaisesRegex(ValueError, 'external conflicts'):
            integration.compile_boot_c(Path('ref'), Path('private'), Path('tools'))

    def test_valid_descriptor_cannot_be_swapped_under_another_owner_key(self):
        spec = sdk.unit_spec(sdk.CPR8)
        owner = {'unit_id': sdk.CPR8, 'source': spec['source'], 'module': spec['module'],
                 'catalog_path': spec['catalog'], 'review_path': spec['review'],
                 'review_sha256': 'b' * 64, 'profile_id': sdk.cp.SDK_PROFILE,
                 'input_section': '.text', 'object_proof': {}}
        value = {'sdk_units': {sdk.UNIT: owner}}
        with patch.object(sdk, 'admitted_units', return_value=[sdk.UNIT]), \
                self.assertRaisesRegex(ValueError, 'map key/descriptor'):
            sdk.current_input_paths(value, Path('private'))


if __name__ == '__main__':
    unittest.main()
