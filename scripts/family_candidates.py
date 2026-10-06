"""Prepare reviewed shared-C controls; never compile, register or add credit."""
from __future__ import annotations

import argparse
import copy
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys

SAFE = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]*\Z")
IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
KEYWORDS = set('auto break case char const continue default do double else enum extern float for goto if inline int long register restrict return short signed sizeof static struct switch typedef union unsigned void volatile while _Bool _Complex _Imaginary'.split())
TOKEN = re.compile(rb"@@[A-Za-z_][A-Za-z0-9_]*@@")
HASH = re.compile(r"[0-9a-f]{64}\Z")
FLAGS = {("-O2", "-G0", "-ffunction-sections"): "g0",
         ("-O2", "-G8", "-ffunction-sections"): "g8"}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    return (json.dumps(value, sort_keys=True, indent=2) + "\n").encode("utf-8")


def contained(root, relative):
    if (not isinstance(relative, str) or "\\" in relative
            or Path(relative).is_absolute() or any(p in {".", ".."} for p in relative.split("/"))):
        raise ValueError("Expected a contained repository-relative path")
    path = (root / relative).resolve()
    if not path.is_relative_to(root) or path == root:
        raise ValueError("Repository path escaped its root")
    return path


def _scrub(data):
    """Keep positions while excluding C comments and string/character contents."""
    pattern = rb'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    return re.sub(pattern, lambda m: re.sub(rb"[^\n]", b" ", m.group()), data, flags=re.S)


def _source_scope(data, function, externals):
    if re.search(rb"\b(?:asm|__asm__|__asm|INCLUDE_ASM)\b|(?m:^[ \t]*(?:(?:[A-Za-z_.$][A-Za-z0-9_.$]*|[0-9]+):[ \t]*)?\.(?:byte|word)\b(?:[ \t]+(?![ \t]*=)\S|[ \t]*$))|\b__(?:DATE|TIME|TIMESTAMP)__\b", data):
        raise ValueError("Family source must contain reproducible C, never assembly or retail bytes")
    if re.search(rb"(?m)^\s*#\s*include\b", data):
        raise ValueError("Family context must be explicit; includes are unsupported")
    clean = _scrub(data)
    definitions = [(m.group(1).decode(), m) for m in
                   re.finditer(rb"\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", clean)
                   if m.group(1) not in {b"if", b"for", b"while", b"switch"}]
    if len(definitions) != 1 or definitions[0][0] != function:
        raise ValueError("Family source must define exactly the reviewed generic function")
    match = definitions[0][1]
    start = clean.rfind(b";", 0, match.start()) + 1
    opening = match.end() - 1
    depth, end = 0, None
    for index in range(opening, len(clean)):
        if clean[index:index + 1] == b"{":
            depth += 1
        elif clean[index:index + 1] == b"}":
            depth -= 1
            if not depth:
                end = index + 1
                break
    if end is None:
        raise ValueError("Unterminated family function")
    calls = {m.group(1).decode() for m in re.finditer(rb"\b([A-Za-z_]\w*)\s*\(", clean[opening:end])}
    if calls - ({function} | externals | {'if','for','while','switch','sizeof'}):
        raise ValueError("Family body calls an unreviewed helper or macro")
    outside = clean[:start] + clean[end:]
    outside = re.sub(rb"(?m)^\s*#[^\n]*", b"", outside)
    # Context is declarations/types only. Reject unreviewed file-scope data.
    statements, depth, beginning = [], 0, 0
    for index, byte in enumerate(outside):
        depth += (byte == 123) - (byte == 125)
        if byte == 59 and depth == 0:
            statements.append(outside[beginning:index].strip())
            beginning = index + 1
    if outside[beginning:].strip():
        raise ValueError("Unrecognized declaration context")
    for statement in statements:
        if not statement:
            continue
        if statement.startswith(b"typedef "):
            continue
        if statement.startswith(b"extern ") and any(
                re.search(rb"\b" + name.encode() + rb"\b", statement) for name in externals):
            if b"=" not in statement:
                continue
        raise ValueError("Context contains an unknown definition, external or data object")


def _reviewed_catalogs(repo, manifest, levels):
    """Isolated stdlib reader reuses native validation, without module-cache bleed."""
    reader = r'''
import hashlib,json,sys
from pathlib import Path
root=Path(sys.argv[1]);sys.path.insert(0,str(root/'scripts'))
import level_native as native
import source_layout
request=json.load(sys.stdin);output={};pins={}
def take(path):
    data=(root/path).read_bytes();pins[path]=hashlib.sha256(data).hexdigest();return data
tools=json.loads(take('progress/candidates.json'))['tools']
for path in ('config/target.json','config/overlays.json','config/candidate-catalog.json',
             'scripts/level_native.py','scripts/check_candidates.py','scripts/elf_tools.py',
             'scripts/wsl_chain.py','scripts/source_layout.py'):
    take(path)
for level in request['levels']:
    source,catalog,review=native.paths(level)
    loaded=native.load_catalog(level,root=root)
    reviewed=json.loads(take(review))
    if type(loaded.get('schema')) is not int or type(reviewed.get('schema')) is not int:
        raise ValueError('Native schema must be an integer')
    native.validate_review(reviewed,loaded,level,root=root)
    if reviewed['tools']!=tools:raise ValueError('Native review tools differ from current profile')
    take(catalog);data=take(source);recipe=request['recipes'][source]
    for piece in recipe['pieces']:take(piece['fragment'])
    rendered,_=source_layout.render(root,{'recipes':{source:recipe}})
    if (rendered[source]!=data or recipe['sha256']!=hashlib.sha256(data).hexdigest()
            or recipe['source_text_bytes']!=len(data)):
        raise ValueError('Stale recipe or generated source')
    output[level]={'catalog':loaded,'review':reviewed}
json.dump({'levels':output,'pins':pins},sys.stdout)
'''
    try:
        result = subprocess.run([sys.executable, "-c", reader, str(repo)],
                                input=encoded({"levels": levels, "recipes": manifest["recipes"]}),
                                capture_output=True, timeout=30)
    except subprocess.TimeoutExpired as error:
        raise ValueError("Native qualification reader exceeded 30 seconds; no bank was created") from error
    if result.returncode:
        detail = result.stderr.decode("utf-8", errors="replace").strip().splitlines()
        raise ValueError("Current native qualification rejected: " + (detail[-1] if detail else "reader failure"))
    return json.loads(result.stdout)


def prepare(repo, runtime, spec_path, batch_id, levels=None, negative=None):
    """Validate all inputs, then create a fresh immutable private bank and tasks."""
    repo, runtime = Path(repo).resolve(), Path(runtime).resolve()
    if runtime == repo or runtime.is_relative_to(repo) or repo.is_relative_to(runtime):
        raise ValueError("Runtime must be outside and not contain the repository")
    if not isinstance(batch_id, str) or not SAFE.fullmatch(batch_id) or batch_id in {".", ".."}:
        raise ValueError("Use a safe fresh batch id")
    bank = (runtime / "bank" / "family-candidates" / batch_id).resolve()
    if not bank.is_relative_to(runtime) or bank == runtime:
        raise ValueError("Private bank escaped runtime")
    if bank.exists():
        raise FileExistsError("Family bank already exists; use a fresh batch id")
    spec_path = Path(spec_path)
    spec_path = (repo / spec_path).resolve() if not spec_path.is_absolute() else spec_path.resolve()
    if not spec_path.is_relative_to(repo):
        raise ValueError("Reviewed spec must be inside the repository")
    spec_bytes = spec_path.read_bytes()
    spec = json.loads(spec_bytes)
    if (type(spec.get("schema")) is not int or spec.get("schema") != 1 or not SAFE.fullmatch(spec.get("id", ""))
            or spec.get("id") in {".", ".."} or not isinstance(spec.get("target"), str)):
        raise ValueError("Invalid reviewed family spec identity")
    inputs = {spec_path.relative_to(repo).as_posix(): digest(spec_bytes)}
    generator_path = Path(__file__).resolve()
    generator_hash = digest(generator_path.read_bytes())
    if generator_path.is_relative_to(repo):
        inputs[generator_path.relative_to(repo).as_posix()] = generator_hash
    layout_path = contained(repo, "config/source-layout.json")
    layout_bytes = layout_path.read_bytes()
    layout = json.loads(layout_bytes)
    inputs["config/source-layout.json"] = digest(layout_bytes)
    if type(layout.get('schema')) is not int or layout.get('schema') != 1:
        raise ValueError("Source layout schema must be integer 1")
    if layout.get("target") != spec["target"]:
        raise ValueError("Family target differs from source layout")
    fragment = spec.get("fragment", {})
    contexts = spec.get("contexts", [])
    if not isinstance(contexts, list) or any(not isinstance(c, dict) for c in contexts):
        raise ValueError("Invalid explicit context fragments")
    chunks = []
    for item in contexts + [fragment]:
        if not isinstance(item, dict) or not HASH.fullmatch(item.get("sha256", "")):
            raise ValueError("Every source/context needs its reviewed SHA-256")
        path = contained(repo, item.get("path"))
        data = path.read_bytes()
        if digest(data) != item["sha256"]:
            raise ValueError("Stale reviewed source/context: " + item["path"])
        inputs[item["path"]] = digest(data)
        chunks.append(data)
    if len({item['path'] for item in contexts + [fragment]}) != len(chunks):
        raise ValueError("Duplicate reviewed source/context fragment")
    substitutions = spec.get("substitutions", {})
    roles = spec.get("external_roles", {})
    function = spec.get("function")
    function_token = spec.get("function_token")
    tokens = {t.decode() for chunk in chunks for t in TOKEN.findall(chunk)}
    if (not isinstance(substitutions, dict) or set(substitutions) != tokens
            or not isinstance(roles, dict) or function_token not in tokens
            or not isinstance(function, str) or not IDENT.fullmatch(function)
            or substitutions.get(function_token) != function
            or any(not isinstance(v, str) or not IDENT.fullmatch(v) or v in KEYWORDS for v in substitutions.values())
            or any(t not in tokens or substitutions[t] != name for t, name in roles.items())
            or len(set(roles.values())) != len(roles) or function in roles.values()
            or set(substitutions.values()) != {function, *roles.values()}
            or sum(v == function for v in substitutions.values()) != 1):
        raise ValueError("Unknown, incomplete or colliding generic token roles")
    source = b"".join(TOKEN.sub(lambda m: substitutions[m.group().decode()].encode(), chunk) for chunk in chunks)
    if b"@@" in source:
        raise ValueError("Unresolved source placeholder")
    _source_scope(source, function, set(roles.values()))
    discovered = {}
    for relative, recipe in layout["recipes"].items():
        match = re.fullmatch(r"candidates/levels/([0-9]+_[a-z0-9_]+)\.c", relative)
        if not match:
            continue
        pieces = [p for p in recipe['pieces'] if p['fragment'] == fragment['path']]
        if len(pieces) > 1:
            raise ValueError("Repeated family fragment in one placement recipe")
        if pieces:
            discovered[match.group(1)] = pieces[0]
    chosen = sorted(discovered) if levels is None else list(levels)
    if not chosen or len(set(chosen)) != len(chosen) or any(n not in discovered for n in chosen):
        raise ValueError("Unknown or duplicate selected family placement")
    chosen = sorted(chosen)
    verified = _reviewed_catalogs(repo, layout, chosen)
    inputs.update(verified['pins'])
    outputs = {'source.c': source}
    catalogs, dependencies, scopes, runtime_inputs = {}, [], [], {}
    for level in chosen:
        piece = discovered[level]
        if piece['sha256'] != fragment['sha256'] or piece['source_text_bytes'] != len(chunks[-1]):
            raise ValueError("Stale family recipe fragment pin")
        mapping = dict(piece.get('replacements', {}))
        # Context placeholders require explicit per-placement recipe mappings too.
        for context in contexts:
            candidate_pieces = [p for p in layout['recipes'][f'candidates/levels/{level}.c']['pieces']
                                if p['fragment'] == context['path']]
            if len(candidate_pieces) != 1 or candidate_pieces[0]['sha256'] != context['sha256']:
                raise ValueError("Context is not identically reviewed in placement recipe")
            for token,value in candidate_pieces[0].get('replacements', {}).items():
                if token in mapping and mapping[token] != value:
                    raise ValueError("Context/body recipe token bindings conflict")
                mapping[token] = value
        if set(mapping) != tokens or any(not isinstance(v, str) or not IDENT.fullmatch(v) for v in mapping.values()):
            raise ValueError("Recipe token roles differ from reviewed source")
        catalog = verified['levels'][level]['catalog']
        review = verified['levels'][level]['review']
        if catalog['target'] != spec['target'] or tuple(catalog['flags']) not in FLAGS:
            raise ValueError("Unsupported family target/profile")
        actual_function = mapping[function_token]
        found = [f for f in catalog['functions'] if f['symbol'] == actual_function]
        if len(found) != 1:
            raise ValueError("Recipe function lacks a complete native catalogue boundary")
        found = found[0]
        addresses = {**catalog['externals'], **{f['symbol']:f['address'] for f in catalog['functions']}}
        externals, symbols = {}, {}
        for token, generic in roles.items():
            actual = mapping[token]
            if actual == actual_function or actual not in addresses:
                raise ValueError("Role lacks a measured helper/data binding")
            if any(mapping[t] != actual for t,v in substitutions.items() if v == generic):
                raise ValueError("One generic role maps to different native symbols")
            externals[generic] = addresses[actual]
            symbols[generic] = actual
        if len(set(symbols.values())) != len(symbols):
            raise ValueError("Colliding native external roles")
        reference_rel = f'references/levels/{level}/overlay.elf'
        reference = (runtime / reference_rel).resolve()
        if not reference.is_relative_to(runtime):
            raise ValueError("Reference escaped private runtime")
        reference_hash = digest(reference.read_bytes())
        if reference_hash != catalog['reference_sha256']:
            raise ValueError("Stale or wrong private reference")
        runtime_inputs[reference_rel] = reference_hash
        private_catalog = {'schema':1,'kind':'family-candidate-catalog','target':spec['target'],
                           'program':catalog['program'],'reference_sha256':reference_hash,
                           'flags':catalog['flags'],'gp':catalog['gp'],
                           'functions':[{'symbol':function,'address':found['address'],'size':found['size']}],
                           'externals':externals}
        catalogs[level] = private_catalog
        outputs[f'catalogs/{level}.json'] = encoded(private_catalog)
        body = next(f for f in review['functions'] if f['symbol'] == actual_function)
        scopes.append({'program':catalog['program'],'address':found['address'],'size':found['size'],
                       'boundary_evidence':f"Current complete native catalogue config/level-native/{level}.json: {actual_function}; validated complete-unit review",
                       'reference_sha256':reference_hash,'body_sha256':body['reference_sha256']})
        dependencies.append({'level':level,'native_symbol':actual_function,'roles':symbols,
                             'externals':externals,'flags':catalog['flags'],'gp':catalog['gp'],
                             'review_path':f'progress/level-candidates/{level}.json'})
    negative_info = None
    if negative is not None:
        parts = negative.split(':') if isinstance(negative,str) else list(negative)
        if len(parts) != 3:
            raise ValueError("Negative binding needs LEVEL:GENERIC_ROLE:ALTERNATE_ROLE")
        level, role, alternate = parts
        if level not in catalogs or role not in roles.values() or alternate not in roles.values() or role == alternate:
            raise ValueError("Unknown or duplicate negative binding role")
        bad = copy.deepcopy(catalogs[level])
        correct, wrong = bad['externals'][role], bad['externals'][alternate]
        if correct == wrong:
            raise ValueError("Negative binding must change a measured address")
        bad['externals'][role] = wrong
        outputs[f'catalogs/{level}-negative.json'] = encoded(bad)
        negative_info = {'level':level,'role':role,'alternate_role':alternate,
                         'correct_address':correct,'wrong_address':wrong,'expected':'complete unmasked mismatch; zero credit'}
    prefix = 'runtime:' + bank.relative_to(runtime).as_posix()
    groups = {}
    for level,catalog in catalogs.items():
        group = FLAGS[tuple(catalog['flags'])]
        groups.setdefault(group,[]).append({'id':level,'catalog':f'{prefix}/catalogs/{level}.json',
                                           'reference':f'runtime:references/levels/{level}/overlay.elf'})
    if negative_info:
        level = negative_info['level']
        groups[FLAGS[tuple(catalogs[level]['flags'])]].append({
            'id':level+'-negative','catalog':f'{prefix}/catalogs/{level}-negative.json',
            'reference':f'runtime:references/levels/{level}/overlay.elf'})
    tasks = [{'id':f"{spec['id']}-{batch_id}-{group}",'kind':'candidate','state':'queued',
              'source':f'{prefix}/source.c','symbols':[function],'targets':targets,'budget':1,
              'hypothesis':'Reviewed canonical family with explicit measured per-program bindings; control only, zero new credit.',
              'next_action':'Run maintained campaign trial; require every complete positive exact and retain the deliberate negative mismatch where present; never integrate these controls.',
              'reopen_condition':'Independent measured ABI/source/compiler evidence; no profile rescue or equivalent source permutations.',
              'pointers':[f'{prefix}/preparation-receipt.json']} for group,targets in sorted(groups.items())]
    outputs['tasks.json'] = encoded(tasks)
    outputs['discovery-input.json'] = encoded({'schema':1,'target':spec['target'],'functions':scopes})
    receipt = {'schema':1,'kind':'reviewed-family-preparation','family':spec['id'],'batch_id':batch_id,
               'state':'prepared_not_compiled','integration_credit':0,'spec':spec_path.relative_to(repo).as_posix(),
               'input_sha256':inputs,'runtime_input_sha256':runtime_inputs,'source_sha256':digest(source),
               'preparer_sha256':generator_hash,
               'substitutions':substitutions,'placements':dependencies,'negative':negative_info,
               'output_sha256':{name:digest(data) for name,data in outputs.items()}}
    outputs['preparation-receipt.json'] = encoded(receipt)
    # Freeze all inputs once more before allocating the fresh immutable bank.
    for relative,pin in inputs.items():
        if digest(contained(repo,relative).read_bytes()) != pin:
            raise ValueError("Preparation input changed: " + relative)
    for relative,pin in runtime_inputs.items():
        if digest((runtime/relative).read_bytes()) != pin:
            raise ValueError("Reference changed during preparation")
    if digest(generator_path.read_bytes()) != generator_hash:
        raise ValueError("Preparer changed during preparation")
    bank.mkdir(parents=True,exist_ok=False)
    try:
        for name,data in outputs.items():
            path = bank/name
            path.parent.mkdir(parents=True,exist_ok=True)
            if not path.resolve().is_relative_to(bank):
                raise ValueError("Output escaped immutable bank")
            with path.open('xb') as stream:
                stream.write(data)
    except Exception as error:
        with (bank/'PREPARATION-ERROR.json').open('xb') as stream:
            stream.write(encoded({'state':'partial_preparation','error':str(error),'reuse':'Never overwrite this bank; use a fresh batch id.'}))
        raise
    return {'bank':str(bank),'tasks':tasks,'receipt':receipt,'discovery':{'schema':1,'target':spec['target'],'functions':scopes}}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo',required=True,type=Path)
    parser.add_argument('--runtime',required=True,type=Path)
    parser.add_argument('--spec',required=True,type=Path)
    parser.add_argument('--batch',required=True)
    parser.add_argument('--level',action='append')
    parser.add_argument('--negative-binding')
    args=parser.parse_args()
    result=prepare(args.repo,args.runtime,args.spec,args.batch,args.level,args.negative_binding)
    print(json.dumps({'bank':result['bank'],'task_ids':[task['id'] for task in result['tasks']],
                      'integration_credit':0},indent=2))


if __name__ == '__main__':
    try:
        main()
    except (OSError,ValueError,KeyError,TypeError) as error:
        print(f'family preparation refused: {error}',file=sys.stderr)
        raise SystemExit(1)
