import copy
from dataclasses import replace
import json
import os
from pathlib import Path, PureWindowsPath
import struct
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "scripts"))
import compiler_profiles as cp

PROFILE = ROOT / "config/compiler-profiles/owned-sdk-b9-single-text-controls-v1.json"
RECEIPT = ROOT / "progress/compiler-profiles/owned-sdk-b9-leaf-controls.json"
MODULES = {"dual-prime388": "16-dual-prime-motion-vector.cfrag", "track116": "17-track-temporary-data.cfrag", "IPUsync104": "18-ipu-synchronization.cfrag"}


def fixture():
    return json.loads(PROFILE.read_bytes()), json.loads(RECEIPT.read_bytes())


def context(profile):
    return {"current_checker_sha256": cp.digest((ROOT / "scripts/check_candidates.py").read_bytes()),
            "current_reference_sha256": profile["reference_sha256"],
            "current_modules": {n:cp.digest((ROOT / "src/boot" / file).read_bytes()) for n,file in MODULES.items()},
            "observed_tools": cp.TOOLS.copy(), "observed_headers": {}}


def repin(profile, receipt):
    profile = copy.deepcopy(profile)
    profile["qualification_sha256"] = cp.digest(cp.encoded(receipt))
    return cp.validate_profile(profile)


def binding(name="one", profile_id=cp.SDK_PROFILE):
    roles = cp.TOOLS if profile_id == cp.SDK_PROFILE else {"cc1":None,"cpp":None,"as":None,"ld.exe":None}
    return cp.RuntimeBinding.create(profile_id=profile_id,runtime_id="owned-tools",job_id=name,distro="Ubuntu",
                                    cwd=f"/private/{name}/config/us",output_root=f"/private/output-{name}",
                                    tool_paths={r:f"/private/{name}/tools/{r}" for r in roles})


def object_fixture(path, *, size=8, text_size=8, extra_function=False, extra_data=False, reloc=False):
    """Small valid ET_REL fixture: adversarial ownership tests, no compiler."""
    names = b"\0.text\0.strtab\0.symtab\0.shstrtab\0.rodata\0.rel.text\0"
    strings = b"\0FUN_1\0EXTRA\0"
    symbols = bytes(16) + struct.pack("<IIIBBH",1,0,size,0x12,0,1)
    if extra_function: symbols += struct.pack("<IIIBBH",7,0,4,0x12,0,1)
    chunks = [(".text",1,6,bytes(text_size),0,0,8,0), (".strtab",3,0,strings,0,0,1,0),
              (".symtab",2,0,symbols,2,1,4,16), (".shstrtab",3,0,names,0,0,1,0)]
    if extra_data: chunks.append((".rodata",1,2,b"1234",0,0,4,0))
    if reloc: chunks.append((".rel.text",9,0,bytes(8),3,1,4,8))
    data = bytearray(52); headers = [bytes(40)]
    for name,kind,flags,payload,link,info,alignment,entrysize in chunks:
        data.extend(bytes((-len(data)) % alignment)); offset = len(data); data.extend(payload)
        headers.append(struct.pack("<10I",names.index(name.encode()+b"\0"),kind,flags,0,offset,len(payload),link,info,alignment,entrysize))
    data.extend(bytes((-len(data)) % 4)); section_offset = len(data); data.extend(b"".join(headers))
    ident = b"\x7fELF\x01\x01\x01" + bytes(9)
    data[:52] = struct.pack("<16sHHIIIIIHHHHHH",ident,1,8,1,0,0,section_offset,0,52,32,0,40,len(headers),4)
    path.write_bytes(data)


class CompilerProfileTests(unittest.TestCase):
    def test_actual_sanitized_receipt_qualifies_only_three_leaf_controls(self):
        p,q = fixture()
        result = cp.validate_qualification(cp.validate_profile(p),q,**context(p))
        self.assertEqual(result.scope,"leaf_controls_only")
        self.assertEqual(len(q["runs"]),6)
        self.assertEqual(q["credit_added"],0)
        def strings(value):
            if isinstance(value,str): yield value
            elif isinstance(value,dict):
                for item in value.values(): yield from strings(item)
            elif isinstance(value,list):
                for item in value: yield from strings(item)
        for value in (p,q):
            for item in strings(value):
                self.assertFalse(Path(item).is_absolute() or PureWindowsPath(item).is_absolute())

    def test_unknown_or_incomplete_profile_cannot_authorize_any_job(self):
        p,q = fixture()
        p["state"] = "unknown_unqualified"; p["qualification_sha256"] = None
        with self.assertRaisesRegex(ValueError,"Unknown/unqualified"):
            cp.validate_qualification(cp.validate_profile(p),q,**context(p))
        for key,value in [("id","other-compiler"),("scope","full_boot"),("schema",True),
                          ("pipeline","run-shell"),("state","qualified_general")]:
            bad,_ = fixture(); bad[key] = value
            with self.subTest(key=key),self.assertRaises(ValueError): cp.validate_profile(bad)

    def test_unknown_fields_flags_and_tool_pins_fail_closed(self):
        p,_ = fixture()
        cases = []
        for where in ("profile","control","tools"):
            bad = copy.deepcopy(p)
            target = bad if where == "profile" else bad["controls"]["track116"] if where == "control" else bad["tools"]
            target["unexpected"] = "x"; cases.append(bad)
        for role in cp.TOOLS:
            bad = copy.deepcopy(p); bad["tools"][role] = "0"*64; cases.append(bad)
        for key,value in [("flags",p["flags"]+["-G0"]),("strip_options",[]),("headers",{"hidden.h":"a"*64}),
                          ("validator_sha256","0"*64)]:
            bad = copy.deepcopy(p); bad[key] = value; cases.append(bad)
        bad = copy.deepcopy(p); bad["controls"]["track116"]["source_sha256"] = "0"*64; cases.append(bad)
        bad = copy.deepcopy(p); bad["controls"]["track116"]["size"] = True; cases.append(bad)
        for bad in cases:
            with self.subTest(value=bad),self.assertRaises(ValueError): cp.validate_profile(bad)

    def test_missing_duplicate_or_nondeterministic_runs_are_rejected_even_when_repinned(self):
        p,q = fixture()
        cases = []
        bad = copy.deepcopy(q); bad["runs"].pop(); cases.append(bad)
        bad = copy.deepcopy(q); bad["runs"][-1] = copy.deepcopy(bad["runs"][0]); cases.append(bad)
        bad = copy.deepcopy(q); bad["runs"][-1]["object_sha256"] = "1"*64; cases.append(bad)
        for key,value in [("produced_size",736),("text_size",400),("symbol_count",2),("padding_bytes",4),
                          ("text_offset",4),("relocation_count",1),("different_bytes",1),("pass_id",True),
                          ("readonly_sections",[{}]),("data_sections",[{}]),("executed_roles",["driver","cpp","cc1","as","strip","linker"])]:
            bad = copy.deepcopy(q); bad["runs"][0][key] = value; cases.append(bad)
        bad = copy.deepcopy(q); bad["extra"] = True; cases.append(bad)
        for bad in cases:
            with self.subTest(value=bad),self.assertRaises(ValueError):
                cp.validate_qualification(repin(p,bad),bad,**context(p))

    def test_current_source_module_reference_header_and_instrument_drift_refused(self):
        p,q = fixture(); profile = cp.validate_profile(p)
        ctx = context(p)
        for key in ("current_checker_sha256","current_reference_sha256"):
            changed = copy.deepcopy(ctx); changed[key] = "0"*64
            with self.subTest(key=key),self.assertRaises(ValueError): cp.validate_qualification(profile,q,**changed)
        changed = copy.deepcopy(ctx); changed["current_modules"]["track116"] = "0"*64
        with self.assertRaises(ValueError): cp.validate_qualification(profile,q,**changed)
        changed = copy.deepcopy(ctx); changed["observed_headers"] = {"hidden.h":"a"*64}
        with self.assertRaises(ValueError): cp.validate_qualification(profile,q,**changed)
        for role in cp.TOOLS:
            changed = copy.deepcopy(ctx); changed["observed_tools"][role] = "0"*64
            with self.subTest(role=role),self.assertRaises(ValueError): cp.validate_qualification(profile,q,**changed)

    def test_two_explicit_jobs_are_immutable_and_do_not_swap_global_environment(self):
        p,q = fixture(); ctx = context(p)
        qualified = cp.validate_qualification(cp.validate_profile(p),q,**ctx)
        source = b"typedef float f32;\ntypedef int s32;\ntypedef unsigned char u8;\ntypedef unsigned int u32;\n\n" + (ROOT/"src/boot"/MODULES["dual-prime388"]).read_bytes()
        before = dict(os.environ)
        first = cp.control_job(qualified,binding("one"),"dual-prime388",source,**ctx)
        second = cp.control_job(qualified,binding("two"),"dual-prime388",source,**ctx)
        self.assertNotEqual(first.argv[0],second.argv[0]); self.assertNotEqual(first.argv[-1],second.argv[-1])
        self.assertNotEqual(dict(first.environment)["PATH"],dict(second.environment)["PATH"])
        self.assertEqual(tuple(first.argv[3:-3]),cp.FLAGS)
        self.assertEqual(dict(os.environ),before)
        with self.assertRaises(ValueError): cp.control_job(qualified,binding("legacy",cp.LEGACY_PROFILE),"dual-prime388",source,**ctx)
        with self.assertRaises(ValueError): cp.control_job(qualified,binding(),"_sysbitFlush",source,**ctx)
        with self.assertRaises(ValueError): cp.control_job(qualified,binding(),"dual-prime388",source+b"\n",**ctx)

    def test_direct_binding_cannot_hide_duplicate_roles_or_noncanonical_order(self):
        p,q = fixture(); ctx = context(p)
        qualified = cp.validate_qualification(cp.validate_profile(p),q,**ctx)
        source = b"typedef float f32;\ntypedef int s32;\ntypedef unsigned char u8;\ntypedef unsigned int u32;\n\n" + (ROOT/"src/boot"/MODULES["dual-prime388"]).read_bytes()
        valid = binding()
        for paths in (valid.tool_paths + (valid.tool_paths[0],), tuple(reversed(valid.tool_paths)), valid.tool_paths[:-1]):
            with self.subTest(paths=paths),self.assertRaises(ValueError):
                cp.control_job(qualified,replace(valid,tool_paths=paths),"dual-prime388",source,**ctx)

    def test_boundaries_require_one_complete_text_no_extra_function_padding_or_data(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory)/"candidate.o"
            object_fixture(path)
            self.assertEqual(cp.inspect_single_text_object(path,"FUN_1",8)["text_size"],8)
            for kwargs in ({"text_size":12},{"extra_function":True},{"extra_data":True},{"reloc":True},{"size":4}):
                with self.subTest(kwargs=kwargs),self.assertRaises(ValueError):
                    object_fixture(path,**kwargs); cp.inspect_single_text_object(path,"FUN_1",8)


if __name__ == "__main__": unittest.main()
