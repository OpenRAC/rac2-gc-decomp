"""One explicitly qualified SDK boot owner; legacy compiler admission is unchanged."""
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

def load_catalog(root):
    root = Path(root)
    c = fields(read(root / CATALOG), ['schema', 'kind', 'unit_id', 'target', 'program', 'source', 'module', 'source_sha256', 'module_sha256', 'reference_sha256', 'profile_id', 'profile_sha256', 'control_qualification_sha256', 'pipeline', 'admission', 'flags', 'strip_options', 'input_section', 'functions', 'externals', 'read_only_sections', 'traits_scope'])
    integer(c['schema'])
    require(c['schema'] == 1 and c['kind'] == 'source-specific-sdk-boot-unit' and (c['unit_id'] == UNIT) and (c['target'] == 'SCUS_972.68') and (c['program'] == 'boot'), 'Unknown SDK owner')
    require(c['source'] == SOURCE and c['module'] == MODULE and (c['source_sha256'] == c['module_sha256'] == SOURCE_SHA) and (file_hash(root / SOURCE) == file_hash(root / MODULE) == SOURCE_SHA), 'SDK exact source/module drift')
    require(c['reference_sha256'] == read(root / 'config/target.json')['boot']['sha256'] == read(root / PROFILE)['reference_sha256'] == '36d5814d8d95328d5839612ccdf7a2e7ecac0b6f868d3ad4bb2e98411f734b4a', 'SDK reference drift')
    require(c['profile_id'] == cp.SDK_PROFILE and c['profile_sha256'] == PROFILE_SHA == file_hash(root / PROFILE) and (c['control_qualification_sha256'] == CONTROL_SHA == file_hash(root / CONTROLS)), 'SDK foundation drift')
    require(c['pipeline'] == cp.PIPELINE and c['admission'] == 'qualified_exact_source_unit_only' and (c['traits_scope'] == 'matched_this_source_only_not_general_SDK64_or_original_types'), 'SDK scope changed')
    require(c['flags'] == list(cp.FLAGS) and c['strip_options'] == list(cp.STRIP) and (c['input_section'] == '.text') and (c['functions'] == [FUNCTION]) and (c['externals'] == {}) and (c['read_only_sections'] == []), 'SDK fixed whole-unit contract changed')
    return c

def validate_review(root, c, review):
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
    require(r['actual_unit_outcome_sha256'] == ACTUAL_UNIT_OUTCOME_SHA, 'Actual source-specific qualification anchor changed')
    require(r['object_sha256'] == 'e0a9a1a83aed6b86d94cd1b0e0f71ea6021d64f190c842ea56aa83cef9f3845c', 'Unqualified SDK object variant')
    contract = fields(r['object_contract'], ['section', 'size', 'alignment', 'symbol_count', 'padding', 'relocations', 'helpers', 'GP', 'data'])
    for name in ['size', 'alignment', 'symbol_count', 'padding', 'relocations', 'helpers']:
        integer(contract[name])
    require(contract['GP'] is False and contract['data'] == [] and (type(contract['data']) is list) and (contract == {'section': '.text', 'size': 152, 'alignment': 8, 'symbol_count': 1, 'padding': 0, 'relocations': 0, 'helpers': 0, 'GP': False, 'data': []}) and (r['read_only_sections'] == []) and (r['integration_credit'] == 0), 'SDK ownership/trait/credit drift')
    require(type(r['functions']) is list and len(r['functions']) == 1, 'SDK complete function omitted')
    f = fields(r['functions'][0], ['symbol', 'address', 'size', 'matched', 'different_bytes', 'reference_sha256', 'candidate_sha256', 'state'])
    integer(f['address'])
    integer(f['size'])
    integer(f['different_bytes'])
    require({k: f[k] for k in FUNCTION} == FUNCTION and f['matched'] is True and (f['different_bytes'] == 0) and (f['reference_sha256'] == f['candidate_sha256'] == BODY) and (f['state'] == 'matched_unintegrated'), 'SDK complete unmasked review refused')
    return r

def current_input_paths(integration, repo):
    """Fixed relative dependency set; unknown SDK descriptors never authorize paths."""
    fields(integration['sdk_units'], [UNIT])
    owner = integration['sdk_units'][UNIT]
    descriptor(owner)
    load_catalog(repo)
    validate_review(repo, load_catalog(repo), read(Path(repo) / REVIEW))
    return {SOURCE, MODULE, CATALOG, REVIEW, PROFILE, CONTROLS, 'scripts/boot_sdk_unit.py', 'scripts/compiler_profiles.py', 'scripts/check_candidates.py', 'scripts/elf_tools.py', 'src/boot/16-dual-prime-motion-vector.cfrag', 'src/boot/17-track-temporary-data.cfrag', 'src/boot/18-ipu-synchronization.cfrag'}

def descriptor(owner):
    fields(owner, ['unit_id', 'source', 'module', 'catalog_path', 'review_path', 'review_sha256', 'profile_id', 'input_section', 'object_proof'])
    require({k: owner[k] for k in ['unit_id', 'source', 'module', 'catalog_path', 'review_path', 'profile_id', 'input_section']} == {'unit_id': UNIT, 'source': SOURCE, 'module': MODULE, 'catalog_path': CATALOG, 'review_path': REVIEW, 'profile_id': cp.SDK_PROFILE, 'input_section': '.text'}, 'Unknown/forged SDK owner')
    cp.sha(owner['review_sha256'])

def owner_rows(functions):
    require(type(functions) is list and functions, 'Missing boot union functions')
    defaults = []
    sdks = []
    seen = set()
    ordered = []
    for row in functions:
        fields(row, ROW_FIELDS)
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
        elif row.get('unit_id') == UNIT and row.get('origin') == 'boot-sdk' and (row.get('candidate_source') == SOURCE):
            sdks.append(row)
        else:
            raise ValueError('Unknown boot object owner')
    ordered.sort(key=lambda r: r['address'])
    require(all((a['address'] + a['size'] <= b['address'] for a, b in zip(ordered, ordered[1:]))), 'Boot owner overlap')
    require(len(sdks) == 1 and {k: sdks[0][k] for k in FUNCTION} == FUNCTION, 'SDK body missing/duplicated/misplaced')
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
    fields(integration['sdk_units'], [UNIT])
    owner = integration['sdk_units'][UNIT]
    descriptor(owner)
    c = load_catalog(root)
    r = validate_review(root, c, read(Path(root) / REVIEW))
    require(owner['review_sha256'] == file_hash(Path(root) / REVIEW), 'SDK reviewed owner pin drift')
    proof = fields(owner['object_proof'], ['unit_id', 'source_sha256', 'catalog_sha256', 'review_sha256', 'profile_id', 'pipeline', 'tools', 'object_sha256', 'candidate_elf_sha256', 'validator_sha256', 'functions', 'read_only_sections'])
    for key in ['source_sha256', 'catalog_sha256', 'profile_id', 'pipeline', 'tools', 'object_sha256', 'validator_sha256', 'functions', 'read_only_sections']:
        require(proof[key] == r[key], 'Fresh SDK owner proof mismatches qualified source-specific review')
    require(proof['unit_id'] == UNIT and proof['review_sha256'] == owner['review_sha256'], 'SDK source owner freshness drift')
    cp.sha(proof['candidate_elf_sha256'])
    require(sdks[0]['reference_sha256'] == sdks[0]['candidate_sha256'] == BODY and sdks[0]['matched'] is True and (sdks[0]['integrated'] is True), 'SDK final raw/state mismatch')
    integer(integration['matched_code_bytes'])
    require(integration['matched_code_bytes'] == sum((f['size'] for f in integration['functions'])), 'Boot union byte count mismatch')
    return (defaults, sdks)

def compile_reviewed(reference, directory, root, binding_path):
    """Fresh SDK source compilation; the admitted source and controls stay separate."""
    import shlex
    import shutil
    import subprocess
    import uuid
    from check_candidates import compare_function, run
    root, directory = (Path(root), Path(directory))
    catalog = load_catalog(root)
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
    snapshot = directory / 'sysbit_flush.c'
    snapshot.write_bytes((root / SOURCE).read_bytes())
    native = workspace_root.rstrip('/') + '/unit-' + uuid.uuid4().hex
    cwd, output = (native + '/config/us', native + '/output')
    environment = {'PATH': ':'.join([str(PurePosixPath(paths[role]).parent) for role in ('cc1', 'as', 'driver')] + ['/usr/bin', '/bin']), 'LANG': 'C', 'LC_ALL': 'C', 'TZ': 'UTC', 'HOME': cwd, 'TMPDIR': cwd + '/tmp'}
    object_path = directory / 'sysbit_flush.c.o'
    driver = [paths['driver'], '-v', '-c', *cp.FLAGS, 'sysbit_flush.c', '-o', output + '/qualified.o']
    strip = [paths['strip'], output + '/qualified.o', *cp.STRIP]
    unix_destination = '/mnt/' + object_path.resolve().drive[0].lower() + object_path.resolve().as_posix()[2:]
    source_bytes = snapshot.read_bytes()
    tool_paths = {name: path for name, path in paths.items() if name != 'linker'}
    script = '\n'.join(['from pathlib import Path', 'import hashlib,json,shutil,subprocess', 'def sha(p): return hashlib.sha256(Path(p).read_bytes()).hexdigest()', 'tools=' + repr(tool_paths), 'expected=' + repr({k: cp.TOOLS[k] for k in tool_paths}), 'assert {k:sha(v) for k,v in tools.items()} == expected', 'native=Path(' + repr(native) + '); native.mkdir(parents=True,exist_ok=False)', 'cwd=Path(' + repr(cwd) + '); cwd.mkdir(parents=True)', '[p.mkdir() for p in [native/"src",native/"include",native/"output",cwd/"include",cwd/"tmp"]]', '(cwd/"sysbit_flush.c").write_bytes(' + repr(source_bytes) + ')', 'result=subprocess.run(' + repr(driver) + ',cwd=cwd,env=' + repr(environment) + ',capture_output=True,timeout=120)', '(native/"output/compile.log").write_bytes(result.stdout+result.stderr)', 'assert result.returncode == 0', 'shutil.copyfile(native/"output/qualified.o",native/"output/before-strip.o")', 'result=subprocess.run(' + repr(strip) + ',env=' + repr(environment) + ',capture_output=True,timeout=30)', '(native/"output/strip.log").write_bytes(result.stdout+result.stderr)', 'assert result.returncode == 0', 'assert sha(cwd/"sysbit_flush.c") == ' + repr(SOURCE_SHA), 'assert {k:sha(v) for k,v in tools.items()} == expected', 'destination=Path(' + repr(unix_destination) + ')', 'shutil.copyfile(native/"output/qualified.o",destination)', '[shutil.copyfile(native/"output"/name,destination.parent/name) for name in ["compile.log","strip.log","before-strip.o"]]'])
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
    cp.inspect_single_text_object(object_path, FUNCTION['symbol'], FUNCTION['size'])
    require(file_hash(object_path) == review['object_sha256'], 'Fresh SDK object differs from admitted source unit')
    require(file_hash(paths['linker']) == cp.TOOLS['linker'], 'SDK qualification linker changed')
    qualification_script = directory / 'qualification.ld'
    qualification_script.write_text('ENTRY(_sysbitFlush)\nSECTIONS { .text._sysbitFlush 0x0012E8E8 : { *(.text) } .data : { *(.data) *(.rodata) *(.rdata) *(.lit4) *(.lit8) *(.sdata) } .bss : { *(.bss) *(.sbss) *(COMMON) } /DISCARD/ : { *(.reginfo) } }\n', encoding='ascii', newline='\n')
    linked = directory / 'qualification.elf'
    run([paths['linker'], '-T', str(qualification_script), '-o', str(linked), str(object_path)], directory / 'link.log')
    observation_after_link = observe_tools()
    (directory / 'inputs-after-link.json').write_text(json.dumps(observation_after_link, indent=2, sort_keys=True) + '\n', encoding='utf8', newline='\n')
    require(observation_after_link == observation_before, 'SDK link input closure drift')
    comparison = compare_function(reference, linked, **FUNCTION)
    require(comparison == review['functions'][0], 'Fresh SDK full body qualification refused')
    require(file_hash(snapshot) == SOURCE_SHA and file_hash(root / SOURCE) == SOURCE_SHA, 'SDK source changed during compilation')
    proof = {'unit_id': UNIT, 'source_sha256': SOURCE_SHA, 'catalog_sha256': file_hash(root / CATALOG), 'review_sha256': file_hash(root / REVIEW), 'profile_id': cp.SDK_PROFILE, 'pipeline': cp.PIPELINE, 'tools': cp.TOOLS.copy(), 'object_sha256': file_hash(object_path), 'candidate_elf_sha256': file_hash(linked), 'validator_sha256': helper_hash(root), 'functions': [comparison], 'read_only_sections': []}
    (directory / 'object-qualification.json').write_text(json.dumps(proof, indent=2, sort_keys=True) + '\n', encoding='utf8', newline='\n')
    return (catalog, object_path, proof)


def check_final_tool_closure(directory, root):
    """Reobserve SDK binaries after the full boot link; private binding stays private."""
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
