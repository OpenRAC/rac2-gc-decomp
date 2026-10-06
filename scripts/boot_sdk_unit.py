"""Two explicitly qualified SDK boot owners; legacy compiler admission is unchanged."""
from __future__ import annotations
import hashlib, json, re, struct
from pathlib import Path, PurePosixPath
import compiler_profiles as cp
from elf_tools import read_elf
UNIT = 'sdk-sysbit-flush'
SOURCE = 'candidates/sdk/sysbit_flush.c'
MODULE = 'src/sdk/sysbit_flush.c'
CATALOG = 'config/boot-units/sdk-sysbit-flush.json'
REVIEW = 'progress/boot-units/sdk-sysbit-flush.json'
SOURCE_SHA = 'b6921af8b6d1fb1b6d65860b0f6130f5a8f6e6bc57777439e6844df8460391f4'
BODY = '70928de6af250170c95f9516617cbc936385634206ffe953ad7dbe2007698ce0'
FUNCTION = {'symbol': '_sysbitFlush', 'address': 1239272, 'size': 152}
PROFILE = 'config/compiler-profiles/owned-sdk-b9-single-text-controls-v1.json'
CONTROLS = 'progress/compiler-profiles/owned-sdk-b9-leaf-controls.json'
PROFILE_SHA = 'b6274a824c321458200375da3012c3cd35163f1994402caf6f9c78bfb71023b6'
CONTROL_SHA = 'd28fdadf81b63778c25592200771dff28964e4b8f9084818cbd37db2178b40d2'
ACTUAL_UNIT_OUTCOME_SHA = '1b6766c61b67f6e418f3ea46be919c0609f7564f4deaa741d33fb5919d636679'
CPR8 = 'sdk-cpr8'
UNITS = {
    UNIT: {'source': SOURCE, 'module': MODULE, 'catalog': CATALOG, 'review': REVIEW,
           'source_sha256': SOURCE_SHA, 'body_sha256': BODY, 'function': FUNCTION,
           'basename': 'sysbit_flush.c', 'actual_anchor': ACTUAL_UNIT_OUTCOME_SHA,
           'object_sha256': 'e0a9a1a83aed6b86d94cd1b0e0f71ea6021d64f190c842ea56aa83cef9f3845c',
           'externals': {}, 'relocations': []},
    CPR8: {'source': 'candidates/sdk/cpr8_source_unit.c', 'module': 'src/sdk/cpr8_source_unit.c',
           'catalog': 'config/boot-units/sdk-cpr8.json', 'review': 'progress/boot-units/sdk-cpr8.json',
           'source_sha256': '526e6888e54dea5025a27a013101d88024c434cc99ce0121c06d67d84c19a66e',
           'body_sha256': '3e43b8eb74cf2d19ba065bc68d58b8bda52bfa053bb06588bbe9dc0f38c0fad8',
           'function': {'symbol': 'FUN_0012D808', 'address': 1234952, 'size': 656},
           'basename': 'cpr8_source_unit.c',
           'actual_anchor': 'ff648481b24e38076a550b7fd7d6d70e1395149cda3e4315f44bb9f2684002ba',
           'object_sha256': 'd92c545c53c0f2c0550fca7c42fa2db1dd37b6d9374e0ab435702f59622b467b',
           'externals': {'FUN_0011F5E0': 1177056, 'FUN_0011F628': 1177128},
           'relocations': [
               {'offset': 248, 'type': 4, 'symbol': 'FUN_0011F5E0', 'target_address': 1177056},
               {'offset': 316, 'type': 4, 'symbol': 'FUN_0011F628', 'target_address': 1177128},
               {'offset': 372, 'type': 4, 'symbol': 'FUN_0011F5E0', 'target_address': 1177056},
               {'offset': 440, 'type': 4, 'symbol': 'FUN_0011F628', 'target_address': 1177128}]},
}

def unit_spec(unit):
    require(type(unit) is str and unit in UNITS, 'Unknown source-specific SDK unit')
    return UNITS[unit]

def admitted_units(root):
    return [unit for unit, spec in UNITS.items() if (Path(root) / spec['catalog']).exists()]

ROW_FIELDS = {'symbol', 'address', 'size', 'matched', 'different_bytes', 'reference_sha256', 'candidate_sha256', 'state', 'integrated', 'program', 'candidate_source', 'origin', 'unit_id'}

def digest(data):
    return hashlib.sha256(data).hexdigest()

def file_hash(p):
    return digest(Path(p).read_bytes())

def read(p):
    return json.loads(Path(p).read_bytes())

def require(v, m):
    if not v:
        raise ValueError(m)

def fields(v, keys):
    require(type(v) is dict and set(v) == set(keys), 'Owner schema fields mismatch')
    return v

def integer(v):
    require(type(v) is int and v >= 0, 'Owner integer/schema required')
    return v

def helper_hash(root):
    return file_hash(Path(root) / 'scripts/boot_sdk_unit.py')

def load_catalog(root, unit=UNIT):
    spec = unit_spec(unit)
    UNIT = unit
    SOURCE, MODULE, CATALOG, REVIEW = (spec[k] for k in ('source', 'module', 'catalog', 'review'))
    SOURCE_SHA, BODY, FUNCTION = (spec[k] for k in ('source_sha256', 'body_sha256', 'function'))
    root = Path(root)
    c = fields(read(root / CATALOG), ['schema', 'kind', 'unit_id', 'target', 'program', 'source', 'module', 'source_sha256', 'module_sha256', 'reference_sha256', 'profile_id', 'profile_sha256', 'control_qualification_sha256', 'pipeline', 'admission', 'flags', 'strip_options', 'input_section', 'functions', 'externals', 'read_only_sections', 'traits_scope'] + (['relocations', 'helper_ownership'] if unit == CPR8 else []))
    integer(c['schema'])
    require(c['schema'] == 1 and c['kind'] == 'source-specific-sdk-boot-unit' and (c['unit_id'] == UNIT) and (c['target'] == 'SCUS_972.68') and (c['program'] == 'boot'), 'Unknown SDK owner')
    require(c['source'] == SOURCE and c['module'] == MODULE and (c['source_sha256'] == c['module_sha256'] == SOURCE_SHA) and (file_hash(root / SOURCE) == file_hash(root / MODULE) == SOURCE_SHA), 'SDK exact source/module drift')
    require(c['reference_sha256'] == read(root / 'config/target.json')['boot']['sha256'] == read(root / PROFILE)['reference_sha256'] == '36d5814d8d95328d5839612ccdf7a2e7ecac0b6f868d3ad4bb2e98411f734b4a', 'SDK reference drift')
    require(c['profile_id'] == cp.SDK_PROFILE and c['profile_sha256'] == PROFILE_SHA == file_hash(root / PROFILE) and (c['control_qualification_sha256'] == CONTROL_SHA == file_hash(root / CONTROLS)), 'SDK foundation drift')
    require(c['pipeline'] == cp.PIPELINE and c['admission'] == 'qualified_exact_source_unit_only' and (c['traits_scope'] == 'matched_this_source_only_not_general_SDK64_or_original_types'), 'SDK scope changed')
    require(c['flags'] == list(cp.FLAGS) and c['strip_options'] == list(cp.STRIP) and (c['input_section'] == '.text') and (c['functions'] == [FUNCTION]) and (c['externals'] == spec['externals']) and (c['read_only_sections'] == []), 'SDK fixed whole-unit contract changed')
    for f in c['functions']:
        fields(f, ['symbol', 'address', 'size'])
        integer(f['address'])
        integer(f['size'])
    for address in c['externals'].values():
        integer(address)
    if unit == CPR8:
        require(type(c['relocations']) is list, 'Explicit CPR8 relocations required')
        for row in c['relocations']:
            fields(row, ['offset', 'type', 'symbol', 'target_address'])
            for name in ['offset', 'type', 'target_address']:
                integer(row[name])
        require(c['relocations'] == spec['relocations'], 'CPR8 relocation binding changed')
        require(c['helper_ownership'] == 'opaque_absolute_reference_bindings_zero_C_credit',
                'Opaque helper ownership changed')
    return c

def validate_review(root, c, review):
    unit = c['unit_id']
    spec = unit_spec(unit)
    UNIT = unit
    SOURCE, MODULE, CATALOG, REVIEW = (spec[k] for k in ('source', 'module', 'catalog', 'review'))
    SOURCE_SHA, BODY, FUNCTION = (spec[k] for k in ('source_sha256', 'body_sha256', 'function'))
    root = Path(root)
    r = fields(review, ['schema', 'kind', 'state', 'unit_id', 'target', 'reference_sha256', 'source_sha256', 'catalog_sha256', 'profile_id', 'profile_sha256', 'control_qualification_sha256', 'pipeline', 'admission', 'flags', 'strip_options', 'tools', 'validator_sha256', 'actual_unit_outcome_sha256', 'object_sha256', 'candidate_elf_sha256', 'functions', 'read_only_sections', 'object_contract', 'integration_credit'])
    integer(r['schema'])
    integer(r['integration_credit'])
    require(r['schema'] == 1 and r['kind'] == 'source-specific-sdk-unit-review' and (r['state'] == 'matched_unintegrated') and (r['unit_id'] == UNIT) and (r['target'] == c['target']), 'Unknown SDK review')
    for name in ['reference_sha256', 'source_sha256', 'profile_id', 'profile_sha256', 'control_qualification_sha256', 'pipeline', 'admission', 'flags', 'strip_options']:
        require(r[name] == c[name], 'SDK review context drift')
    require(r['catalog_sha256'] == file_hash(root / CATALOG) and r['validator_sha256'] == helper_hash(root), 'SDK review catalog/validator drift')
    cp.tools(r['tools'])
    for name in ['actual_unit_outcome_sha256', 'object_sha256', 'candidate_elf_sha256']:
        cp.sha(r[name])
    require(r['actual_unit_outcome_sha256'] == spec['actual_anchor'], 'Actual source-specific qualification anchor changed')
    require(r['object_sha256'] == spec['object_sha256'], 'Unqualified SDK object variant')
    contract = fields(r['object_contract'], ['section', 'size', 'alignment', 'symbol_count', 'padding', 'relocations', 'helpers', 'GP', 'data'])
    for name in ['size', 'alignment', 'symbol_count', 'padding', 'relocations', 'helpers']:
        integer(contract[name])
    require(contract['GP'] is False and contract['data'] == [] and (type(contract['data']) is list) and (contract == {'section': '.text', 'size': FUNCTION['size'], 'alignment': 8, 'symbol_count': 1, 'padding': 0, 'relocations': len(spec['relocations']), 'helpers': 0, 'GP': False, 'data': []}) and (r['read_only_sections'] == []) and (r['integration_credit'] == 0), 'SDK ownership/trait/credit drift')
    require(type(r['functions']) is list and len(r['functions']) == 1, 'SDK complete function omitted')
    f = fields(r['functions'][0], ['symbol', 'address', 'size', 'matched', 'different_bytes', 'reference_sha256', 'candidate_sha256', 'state'])
    integer(f['address'])
    integer(f['size'])
    integer(f['different_bytes'])
    require({k: f[k] for k in FUNCTION} == FUNCTION and f['matched'] is True and (f['different_bytes'] == 0) and (f['reference_sha256'] == f['candidate_sha256'] == BODY) and (f['state'] == 'matched_unintegrated'), 'SDK complete unmasked review refused')
    return r

def current_input_paths(integration, repo):
    """Fixed relative closure for every currently admitted source-specific owner."""
    fields(integration['sdk_units'], admitted_units(repo))
    result = {PROFILE, CONTROLS, 'scripts/boot_sdk_unit.py', 'scripts/compiler_profiles.py',
              'scripts/check_candidates.py', 'scripts/elf_tools.py',
              'src/boot/16-dual-prime-motion-vector.cfrag',
              'src/boot/17-track-temporary-data.cfrag', 'src/boot/18-ipu-synchronization.cfrag'}
    for unit, owner in integration['sdk_units'].items():
        descriptor(owner)
        require(owner['unit_id'] == unit, 'SDK owner map key/descriptor mismatch')
        spec = unit_spec(unit)
        c = load_catalog(repo, unit)
        validate_review(repo, c, read(Path(repo) / spec['review']))
        result.update(spec[k] for k in ('source', 'module', 'catalog', 'review'))
    return result

def descriptor(owner):
    fields(owner, ['unit_id', 'source', 'module', 'catalog_path', 'review_path', 'review_sha256',
                   'profile_id', 'input_section', 'object_proof'])
    spec = unit_spec(owner['unit_id'])
    require(owner['source'] == spec['source'] and owner['module'] == spec['module']
            and owner['catalog_path'] == spec['catalog'] and owner['review_path'] == spec['review']
            and owner['profile_id'] == cp.SDK_PROFILE and owner['input_section'] == '.text',
            'Unknown/forged SDK owner')
    cp.sha(owner['review_sha256'])

def owner_rows(functions):
    require(type(functions) is list and functions, 'Missing boot union functions')
    defaults = []
    sdks = []
    seen = set()
    ordered = []
    for row in functions:
        fields(row, ROW_FIELDS)
        require(type(row['unit_id']) is str and type(row['symbol']) is str, 'Invalid owner identity')
        integer(row['address'])
        integer(row['size'])
        integer(row['different_bytes'])
        require(row['program'] == 'boot' and row['state'] == 'integrated' and (row['matched'] is True) and (row['integrated'] is True) and (row['different_bytes'] == 0), 'Invalid final boot owner state')
        require(row['size'] > 0 and row['address'] % 4 == row['size'] % 4 == 0, 'Invalid full owner span')
        key = (row['symbol'], row['address'], row['size'])
        require(key not in seen, 'Duplicate boot owner function')
        seen.add(key)
        ordered.append(row)
        if row.get('unit_id') == 'default-gnu8bed' and row.get('origin') == 'boot-default' and (row.get('candidate_source') == 'candidates/boot.c'):
            defaults.append(row)
        elif row.get('unit_id') in UNITS and row.get('origin') == 'boot-sdk' and (row.get('candidate_source') == unit_spec(row['unit_id'])['source']):
            sdks.append(row)
        else:
            raise ValueError('Unknown boot object owner')
    ordered.sort(key=lambda r: r['address'])
    require(all((a['address'] + a['size'] <= b['address'] for a, b in zip(ordered, ordered[1:]))), 'Boot owner overlap')
    require(sdks, 'SDK body missing')
    seen_units = set()
    for row in sdks:
        spec = unit_spec(row['unit_id'])
        require(row['unit_id'] not in seen_units and {k: row[k] for k in spec['function']} == spec['function'], 'SDK body duplicated/misplaced')
        seen_units.add(row['unit_id'])
    return (defaults, sdks)

def validate_union(integration, default_review, root, validate_default, validate_default_object):
    fields(integration, ['schema', 'kind', 'target', 'program', 'reference_sha256', 'state', 'functions', 'matched_code_bytes', 'full_boot_gate', 'default', 'sdk_units'])
    require(type(integration['schema']) is int and integration['schema'] == 3 and (integration['kind'] == 'boot-c-owner-integration') and (integration['program'] == 'boot') and (integration['state'] == 'integrated'), 'Unknown boot owner union')
    defaults, sdks = owner_rows(integration['functions'])
    legacy = dict(integration['default'])
    require('functions' not in legacy, 'Duplicate default function rows')
    legacy['functions'] = [{k: v for k, v in row.items() if k not in ['unit_id', 'origin', 'candidate_source']} for row in defaults]
    require(integration['target'] == legacy['target'] and integration['reference_sha256'] == legacy['reference_sha256'] and (integration['full_boot_gate'] == legacy['full_boot_gate']), 'Union default reference/full gate drift')
    validate_default(legacy)
    validate_default_object(legacy, default_review)
    fields(integration['sdk_units'], admitted_units(root))
    require({row['unit_id'] for row in sdks} == set(integration['sdk_units']), 'SDK row/descriptor ownership differs')
    for UNIT, owner in integration['sdk_units'].items():
        spec = unit_spec(UNIT)
        REVIEW = spec['review']
        BODY = spec['body_sha256']
        row = next(row for row in sdks if row['unit_id'] == UNIT)
        descriptor(owner)
        require(owner['unit_id'] == UNIT, 'SDK owner map key/descriptor mismatch')
        c = load_catalog(root, UNIT)
        r = validate_review(root, c, read(Path(root) / REVIEW))
        require(owner['review_sha256'] == file_hash(Path(root) / REVIEW), 'SDK reviewed owner pin drift')
        proof = fields(owner['object_proof'], ['unit_id', 'source_sha256', 'catalog_sha256', 'review_sha256', 'profile_id', 'pipeline', 'tools', 'object_sha256', 'candidate_elf_sha256', 'validator_sha256', 'functions', 'read_only_sections'])
        for key in ['source_sha256', 'catalog_sha256', 'profile_id', 'pipeline', 'tools', 'object_sha256', 'validator_sha256', 'functions', 'read_only_sections']:
            require(proof[key] == r[key], 'Fresh SDK owner proof mismatches qualified source-specific review')
        require(proof['unit_id'] == UNIT and proof['review_sha256'] == owner['review_sha256'], 'SDK source owner freshness drift')
        cp.sha(proof['candidate_elf_sha256'])
        require(row['reference_sha256'] == row['candidate_sha256'] == BODY and row['matched'] is True and (row['integrated'] is True), 'SDK final raw/state mismatch')
    integer(integration['matched_code_bytes'])
    require(integration['matched_code_bytes'] == sum((f['size'] for f in integration['functions'])), 'Boot union byte count mismatch')
    return (defaults, sdks)

def _symbol_tables(path):
    """Inspect bounded ELF symbol/REL tables without trusting a producer summary."""
    path = Path(path)
    data = path.read_bytes()
    elf = read_elf(path)
    offset = struct.unpack_from('<I', data, 32)[0]
    stride = struct.unpack_from('<H', data, 46)[0]
    headers = [struct.unpack_from('<10I', data, offset + i * stride)
               for i in range(len(elf['sections']))]
    tables = {}
    for index, h in enumerate(headers):
        if h[1] != 2:
            continue
        require(h[6] < len(headers) and headers[h[6]][1] == 3
                and h[9] == 16 and h[5] % 16 == 0, 'Invalid SDK symbol table')
        strings_h = headers[h[6]]
        strings = data[strings_h[4]:strings_h[4] + strings_h[5]]
        symbols = []
        for at in range(h[4], h[4] + h[5], 16):
            name, value, size, info, other, owner = struct.unpack_from('<IIIBBH', data, at)
            require(name < len(strings), 'SDK symbol name outside strings')
            end = strings.find(b'\0', name)
            require(end >= 0, 'Unterminated SDK symbol name')
            symbols.append({'name': strings[name:end].decode('ascii'), 'value': value,
                            'size': size, 'info': info, 'other': other, 'owner': owner})
        tables[index] = symbols
    require(len(tables) == 1, 'One SDK symbol table required')
    return data, elf, headers, tables

def inspect_unit_object(path, unit=UNIT):
    """The leaf inspector remains unchanged; CPR8 has its own four-call contract."""
    spec = unit_spec(unit)
    function = spec['function']
    if unit == UNIT:
        return cp.inspect_single_text_object(Path(path), function['symbol'], function['size'])
    data, elf, headers, tables = _symbol_tables(path)
    require(elf['type'] == 1, 'Expected CPR8 ET_REL object')
    sections = elf['sections']
    texts = [(i, s) for i, s in enumerate(sections)
             if s['size'] and s['type'] == 1 and s['flags'] & 6 == 6]
    require(len(texts) == 1, 'One CPR8 executable section required')
    text_index, section = texts[0]
    require(section['name'] == '.text' and section['size'] == function['size']
            and section['alignment'] == 8, 'Complete CPR8 .text extent refused')
    require(not [s for s in sections if s['size'] and s['flags'] & 2
                 and s['name'] not in ('.text', '.reginfo')], 'Extra CPR8 allocated section')
    require(not [s for s in sections if s['size'] and (s['type'] == 4 or s['name'] == '.mdebug')],
            'Unreviewed RELA/debug metadata')
    symbols = next(iter(tables.values()))
    definitions = [s for s in symbols if s['owner'] != 0 and s['info'] & 15 == 2]
    require(definitions == [{'name': function['symbol'], 'value': 0, 'size': function['size'],
                            'info': 18, 'other': 0, 'owner': text_index}],
            'CPR8 extra/partial function or padding')
    global_symbols = [s for s in symbols if s['info'] >> 4 != 0]
    require(len(global_symbols) == 3 and {s['name'] for s in global_symbols}
            == {function['symbol'], *spec['externals']}, 'Unexpected CPR8 global symbols')
    for name in spec['externals']:
        helper = [s for s in global_symbols if s['name'] == name]
        require(helper == [{'name': name, 'value': 0, 'size': 0, 'info': 16,
                            'other': 0, 'owner': 0}], 'CPR8 helper must remain GLOBAL UNDEF NOTYPE')
    relocations = []
    for h in headers:
        if h[1] != 9 or not h[5]:
            continue
        require(h[6] in tables and h[7] == text_index and h[9] == 8 and h[5] % 8 == 0,
                'Invalid CPR8 REL table ownership')
        for at in range(h[4], h[4] + h[5], 8):
            position, info = struct.unpack_from('<II', data, at)
            symbol_index, relocation_type = info >> 8, info & 255
            require(symbol_index < len(tables[h[6]]) and position % 4 == 0
                    and position + 4 <= section['size'], 'Invalid CPR8 relocation location')
            name = tables[h[6]][symbol_index]['name']
            require(name in spec['externals'], 'Unknown CPR8 relocation symbol')
            word = struct.unpack_from('<I', data, section['offset'] + position)[0]
            require(word >> 26 == 3 and word & ((1 << 26) - 1) == 0,
                    'CPR8 relocation requires JAL with zero addend')
            relocations.append({'offset': position, 'type': relocation_type,
                                'symbol': name, 'target_address': spec['externals'][name]})
    require(relocations == spec['relocations'], 'CPR8 four measured R_MIPS_26 bindings refused')
    return {'object_sha256': digest(data), 'text_size': section['size'], 'text_alignment': 8,
            'symbol_count': 1, 'relocations': relocations, 'helper_C_credit': 0}

def inspect_linked_helpers(path, unit=UNIT):
    """SN may emit ABS bindings as GLOBAL OBJECT; this does not own helper bodies."""
    spec = unit_spec(unit)
    if not spec['externals']:
        return
    _, elf, _, tables = _symbol_tables(path)
    require(elf['type'] == 2, 'Expected CPR8 qualification ET_EXEC')
    function = spec['function']
    sections = elf['sections']
    executable = [s for s in sections if s['size'] and s['flags'] & 6 == 6]
    require(len(executable) == 1 and executable[0]['name'] in ('.text', '.text.' + function['symbol'])
            and executable[0]['address'] == function['address']
            and executable[0]['size'] == function['size'] and executable[0]['alignment'] == 8,
            'Linked CPR8 complete ownership refused')
    require(not [s for s in sections if s['size'] and s['flags'] & 2
                 and not s['flags'] & 4], 'Unexpected linked CPR8 data')
    symbols = next(iter(tables.values()))
    functions = [s for s in symbols if s['owner'] != 0 and s['info'] & 15 == 2]
    require(len(functions) == 1 and functions[0]['name'] == function['symbol']
            and functions[0]['value'] == function['address'] and functions[0]['size'] == function['size']
            and functions[0]['info'] == 18, 'Extra or partial linked CPR8 function')
    for name, address in spec['externals'].items():
        helper = [s for s in symbols if s['name'] == name]
        require(len(helper) == 1 and helper[0]['value'] == address and helper[0]['size'] == 0
                and helper[0]['info'] in (16, 17) and helper[0]['other'] == 0
                and helper[0]['owner'] == 0xfff1, 'Wrong opaque ABS helper binding')


def compile_reviewed(reference, directory, root, binding_path, unit=UNIT):
    """Fresh SDK source compilation; the admitted source and controls stay separate."""
    spec = unit_spec(unit)
    UNIT = unit
    SOURCE, MODULE, CATALOG, REVIEW = (spec[k] for k in ('source', 'module', 'catalog', 'review'))
    SOURCE_SHA, BODY, FUNCTION = (spec[k] for k in ('source_sha256', 'body_sha256', 'function'))
    import shlex
    import shutil
    import subprocess
    import uuid
    from check_candidates import compare_function, run
    root, directory = (Path(root), Path(directory))
    catalog = load_catalog(root, unit)
    review = validate_review(root, catalog, read(root / REVIEW))
    require(binding_path is not None, 'An explicit private SDK runtime binding is required')
    binding = fields(read(binding_path), ['distro', 'workspace_root', 'tool_paths'])
    paths = fields(binding['tool_paths'], cp.TOOLS)
    require(re.fullmatch('[A-Za-z0-9_-]+', binding['distro']) is not None, 'Invalid private SDK distribution')
    workspace_root = binding['workspace_root']
    require(type(workspace_root) is str and workspace_root.startswith(('/root/', '/home/')) and ('..' not in Path(workspace_root).parts), 'Use an explicit private native workspace root')
    for role, path in paths.items():
        require(type(path) is str and '\n' not in path and ('\r' not in path), 'Invalid private SDK instrument path')
        if role != 'linker':
            require(path.startswith('/') and '..' not in Path(path).parts, 'SDK instruments require explicit WSL paths')
    directory.mkdir(parents=True, exist_ok=False)
    (directory / 'private-binding.json').write_text(json.dumps(binding, indent=2) + '\n', encoding='utf8', newline='\n')
    snapshot = directory / spec['basename']
    snapshot.write_bytes((root / SOURCE).read_bytes())
    native = workspace_root.rstrip('/') + '/unit-' + uuid.uuid4().hex
    cwd, output = (native + '/config/us', native + '/output')
    environment = {'PATH': ':'.join([str(PurePosixPath(paths[role]).parent) for role in ('cc1', 'as', 'driver')] + ['/usr/bin', '/bin']), 'LANG': 'C', 'LC_ALL': 'C', 'TZ': 'UTC', 'HOME': cwd, 'TMPDIR': cwd + '/tmp'}
    object_path = directory / (spec['basename'] + '.o')
    driver = [paths['driver'], '-v', '-c', *cp.FLAGS, spec['basename'], '-o', output + '/qualified.o']
    strip = [paths['strip'], output + '/qualified.o', *cp.STRIP]
    unix_destination = '/mnt/' + object_path.resolve().drive[0].lower() + object_path.resolve().as_posix()[2:]
    source_bytes = snapshot.read_bytes()
    tool_paths = {name: path for name, path in paths.items() if name != 'linker'}
    script = '\n'.join(['from pathlib import Path', 'import hashlib,json,shutil,subprocess', 'def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()', 'tools=' + repr(tool_paths), 'expected=' + repr({k: cp.TOOLS[k] for k in tool_paths}), 'assert {k:sha(v) for k,v in tools.items()} == expected', 'native=Path(' + repr(native) + '); native.mkdir(parents=True,exist_ok=False)', 'cwd=Path(' + repr(cwd) + '); cwd.mkdir(parents=True)', '[p.mkdir() for p in [native/"src",native/"include",native/"output",cwd/"include",cwd/"tmp"]]', '(cwd/' + repr(spec['basename']) + ').write_bytes(' + repr(source_bytes) + ')', 'result=subprocess.run(' + repr(driver) + ',cwd=cwd,env=' + repr(environment) + ',capture_output=True,timeout=120)', '(native/"output/compile.log").write_bytes(result.stdout+result.stderr)', 'assert result.returncode == 0', 'shutil.copyfile(native/"output/qualified.o",native/"output/before-strip.o")', 'result=subprocess.run(' + repr(strip) + ',env=' + repr(environment) + ',capture_output=True,timeout=30)', '(native/"output/strip.log").write_bytes(result.stdout+result.stderr)', 'assert result.returncode == 0', 'assert sha(cwd/' + repr(spec['basename']) + ') == ' + repr(SOURCE_SHA), 'assert {k:sha(v) for k,v in tools.items()} == expected', 'destination=Path(' + repr(unix_destination) + ')', 'shutil.copyfile(native/"output/qualified.o",destination)', '[shutil.copyfile(native/"output"/name,destination.parent/name) for name in ["compile.log","strip.log","before-strip.o"]]'])
    observation_code = ('from pathlib import Path;import hashlib,json;tools=' + repr(tool_paths)
                        + ';print(json.dumps({"tools":{k:hashlib.sha256(Path(v).read_bytes()).hexdigest() for k,v in tools.items()}}))')
    def observe_tools():
        process = subprocess.run(['wsl.exe', '-d', binding['distro'], '-e', 'python3', '-c', observation_code],
                                 capture_output=True, text=True, timeout=30)
        require(process.returncode == 0, 'Cannot observe SDK compiler closure')
        observation = json.loads(process.stdout)
        observation['tools']['linker'] = file_hash(paths['linker'])
        observation['source_sha256'] = file_hash(root / SOURCE)
        observation['module_sha256'] = file_hash(root / MODULE)
        observation['catalog_sha256'] = file_hash(root / CATALOG)
        observation['review_sha256'] = file_hash(root / REVIEW)
        observation['validator_sha256'] = helper_hash(root)
        observation['binding_sha256'] = file_hash(binding_path)
        observation['reference_sha256'] = file_hash(reference)
        require(observation['tools'] == cp.TOOLS, 'Observed SDK instruments changed')
        require(observation['reference_sha256'] == catalog['reference_sha256'], 'SDK reference identity changed')
        return observation
    observation_before = observe_tools()
    (directory / 'inputs-before.json').write_text(json.dumps(observation_before, indent=2, sort_keys=True) + '\n',
                                                encoding='utf8', newline='\n')
    invocation = {'driver_argv': driver, 'strip_argv': strip, 'cwd': cwd, 'environment': environment}
    (directory / 'invocation.json').write_text(json.dumps(invocation, indent=2, sort_keys=True) + '\n',
                                              encoding='utf8', newline='\n')
    result = subprocess.run(['wsl.exe', '-d', binding['distro'], '-e', 'python3', '-c', script], capture_output=True, timeout=180)
    (directory / 'SDK-run.log').write_bytes(result.stdout + result.stderr)
    copy_artifacts = ('from pathlib import Path;import shutil;native=Path(' + repr(output)
                      + ');destination=Path(' + repr(unix_destination) + ').parent;'
                      + '[shutil.copyfile(p,destination/p.name) for p in native.iterdir() if p.is_file()] if native.exists() else None')
    copied = subprocess.run(['wsl.exe', '-d', binding['distro'], '-e', 'python3', '-c', copy_artifacts],
                            capture_output=True, timeout=30)
    (directory / 'artifact-copy.log').write_bytes(copied.stdout + copied.stderr)
    observation_after_compile = observe_tools()
    (directory / 'inputs-after-compile.json').write_text(json.dumps(observation_after_compile, indent=2, sort_keys=True) + '\n',
                                                       encoding='utf8', newline='\n')
    require(observation_after_compile == observation_before, 'SDK compile input closure drift')
    require(result.returncode == 0, 'Fresh fixed SDK source/strip execution failed; preserve private log')
    executed = {}
    for line in (directory / 'compile.log').read_text(errors='replace').splitlines():
        try:
            words = shlex.split(line)
        except ValueError:
            continue
        if not words or not words[0].startswith('/'):
            continue
        role = Path(words[0]).name
        require(role in ('cc1', 'as') and role not in executed, 'Unknown SDK producer')
        executed[role] = words[0]
    require(set(executed) == {'cc1', 'as'}, 'Integrated SDK producer trace missing')
    resolve = 'from pathlib import Path;import json;paths=' + repr(executed) + ';print(json.dumps({k:str(Path(v).resolve()) for k,v in paths.items()}))'
    observed = subprocess.run(['wsl.exe', '-d', binding['distro'], '-e', 'python3', '-c', resolve], capture_output=True, text=True, timeout=30)
    require(observed.returncode == 0 and json.loads(observed.stdout) == {k: paths[k] for k in ('cc1', 'as')}, 'Actual SDK producer paths differ')
    inspect_unit_object(object_path, unit)
    require(file_hash(object_path) == review['object_sha256'], 'Fresh SDK object differs from admitted source unit')
    require(file_hash(paths['linker']) == cp.TOOLS['linker'], 'SDK qualification linker changed')
    qualification_script = directory / 'qualification.ld'
    absolute = ''.join(f'{name} = 0x{address:08X};\n' for name, address in spec['externals'].items())
    qualification_script.write_text(absolute + f"ENTRY({FUNCTION['symbol']})\nSECTIONS {{ .text.{FUNCTION['symbol']} 0x{FUNCTION['address']:08X} : {{ *(.text) }} .data : {{ *(.data) *(.rodata) *(.rdata) *(.lit4) *(.lit8) *(.sdata) }} .bss : {{ *(.bss) *(.sbss) *(COMMON) }} /DISCARD/ : {{ *(.reginfo) }} }}\n", encoding='ascii', newline='\n')
    linked = directory / 'qualification.elf'
    run([paths['linker'], '-T', str(qualification_script), '-o', str(linked), str(object_path)], directory / 'link.log')
    observation_after_link = observe_tools()
    (directory / 'inputs-after-link.json').write_text(json.dumps(observation_after_link, indent=2, sort_keys=True) + '\n', encoding='utf8', newline='\n')
    require(observation_after_link == observation_before, 'SDK link input closure drift')
    inspect_linked_helpers(linked, unit)
    comparison = compare_function(reference, linked, **FUNCTION)
    require(comparison == review['functions'][0], 'Fresh SDK full body qualification refused')
    require(file_hash(snapshot) == SOURCE_SHA and file_hash(root / SOURCE) == SOURCE_SHA, 'SDK source changed during compilation')
    proof = {'unit_id': UNIT, 'source_sha256': SOURCE_SHA, 'catalog_sha256': file_hash(root / CATALOG), 'review_sha256': file_hash(root / REVIEW), 'profile_id': cp.SDK_PROFILE, 'pipeline': cp.PIPELINE, 'tools': cp.TOOLS.copy(), 'object_sha256': file_hash(object_path), 'candidate_elf_sha256': file_hash(linked), 'validator_sha256': helper_hash(root), 'functions': [comparison], 'read_only_sections': []}
    (directory / 'object-qualification.json').write_text(json.dumps(proof, indent=2, sort_keys=True) + '\n', encoding='utf8', newline='\n')
    return (catalog, object_path, proof)


def _check_final_unit_closure(directory, root, unit):
    """Reobserve SDK binaries after the full boot link; private binding stays private."""
    spec = unit_spec(unit)
    UNIT = unit
    SOURCE, MODULE, CATALOG, REVIEW = (spec[k] for k in ('source', 'module', 'catalog', 'review'))
    SOURCE_SHA, BODY, FUNCTION = (spec[k] for k in ('source_sha256', 'body_sha256', 'function'))
    import subprocess
    record = read(Path(directory) / 'build/c/sdk' / UNIT / 'invocation.json')
    paths = read(Path(directory) / 'build/c/sdk' / UNIT / 'inputs-before.json')
    # Driver and strip arguments identify the already pinned private tools.
    sdk_paths = {'driver': record['driver_argv'][0], 'strip': record['strip_argv'][0]}
    source_root = PurePosixPath(record['driver_argv'][0]).parent.parent
    sdk_paths.update({'cc1': str(source_root / 'lib/gcc-lib/ee/2.9-ee-991111-01/cc1'),
                      'as': str(source_root / 'ee/bin/as'),
                      'cpp_available': str(source_root / 'lib/gcc-lib/ee/2.9-ee-991111-01/cpp'),
                      'cc1plus_available': str(source_root / 'lib/gcc-lib/ee/2.9-ee-991111-01/cc1plus')})
    binding = read(Path(directory) / 'build/c/sdk' / UNIT / 'private-binding.json')
    code = 'from pathlib import Path;import json,hashlib;paths=' + repr(sdk_paths) + ';print(json.dumps({k:hashlib.sha256(Path(v).read_bytes()).hexdigest() for k,v in paths.items()}))'
    result = subprocess.run(['wsl.exe', '-d', binding['distro'], '-e', 'python3', '-c', code],
                            capture_output=True, text=True, timeout=30)
    require(result.returncode == 0, 'Cannot observe SDK tools after full boot')
    actual = json.loads(result.stdout)
    actual['linker'] = file_hash(binding['tool_paths']['linker'])
    require(actual == paths['tools'] == cp.TOOLS, 'SDK instruments changed after full boot link')
    require(file_hash(root / SOURCE) == SOURCE_SHA and file_hash(root / MODULE) == SOURCE_SHA,
            'SDK source changed after full boot link')

def check_final_tool_closure(directory, root):
    for unit in admitted_units(root):
        _check_final_unit_closure(directory, root, unit)
