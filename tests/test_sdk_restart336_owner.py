"""Finite third SDK owner synthetic metadata checks; no assets or compiler."""
import copy,struct,sys,tempfile,unittest
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT/"scripts"))
import boot_sdk_unit as sdk
UNIT=sdk.RESTART336

def fixture(path,linked=False):
    spec=sdk.unit_spec(UNIT);f=spec["function"]
    names=b"\0.text\0.strtab\0.symtab\0.shstrtab\0.rel.text\0"
    strings=b"\0"+f["symbol"].encode()+b"\0";offsets={}
    for name in spec["externals"]:
        offsets[name]=len(strings);strings+=name.encode()+b"\0"
    symbols=bytes(16)+struct.pack("<IIIBBH",1,f["address"] if linked else 0,f["size"],18,0,1)
    for name,address in spec["externals"].items():
        symbols+=struct.pack("<IIIBBH",offsets[name],address if linked else 0,0,17 if linked else 16,0,65521 if linked else 0)
    text=bytearray(f["size"]);rels=bytearray()
    for row,index,opcode in zip(spec["relocations"],(2,3),(3,2)):
        struct.pack_into("<I",text,row["offset"],opcode<<26)
        rels.extend(struct.pack("<II",row["offset"],index<<8|4))
    chunks=[(".text",1,6,bytes(text),0,0,8,0),(".strtab",3,0,strings,0,0,1,0),
            (".symtab",2,0,symbols,2,1,4,16),(".shstrtab",3,0,names,0,0,1,0)]
    if not linked:chunks.append((".rel.text",9,0,bytes(rels),3,1,4,8))
    data=bytearray(52);headers=[bytes(40)]
    for name,kind,flags,payload,link,info,align,stride in chunks:
        data.extend(bytes((-len(data))%align));at=len(data);data.extend(payload)
        headers.append(struct.pack("<10I",names.index(name.encode()+b"\0"),kind,flags,
            f["address"] if linked and name==".text" else 0,at,len(payload),link,info,align,stride))
    data.extend(bytes((-len(data))%4));shoff=len(data);data.extend(b"".join(headers))
    data[:52]=struct.pack("<16sHHIIIIIHHHHHH",b"\x7fELF\x01\x01\x01"+bytes(9),2 if linked else 1,8,1,
        f["address"] if linked else 0,0,shoff,0,52,32,0,40,len(headers),4)
    path.write_bytes(data)

class RestartOwnerTests(unittest.TestCase):
    def test_exact_synthetic_object_and_linked_scope(self):
        with tempfile.TemporaryDirectory() as tmp:
            p=Path(tmp)/"fixture.o";fixture(p)
            self.assertEqual(sdk.inspect_unit_object(p,UNIT)["text_size"],336)
            fixture(p,linked=True);sdk.inspect_linked_helpers(p,UNIT)
    def test_three_finite_catalogs_reviews(self):
        self.assertEqual(sdk.admitted_units(ROOT),[sdk.UNIT,sdk.CPR8,UNIT])
        for unit in sdk.admitted_units(ROOT):
            c=sdk.load_catalog(ROOT,unit);sdk.validate_review(ROOT,c,sdk.read(ROOT/sdk.unit_spec(unit)["review"]))
    def test_second_owner_contracts_remain_exact(self):
        self.assertEqual(sdk.unit_spec(sdk.UNIT)["function"]["size"],152)
        self.assertEqual(sdk.unit_spec(sdk.CPR8)["function"]["size"],656)
        self.assertEqual(sdk.unit_spec(sdk.CPR8)["relocations"][0]["offset"],248)
    def test_unknown_owner_refused(self):
        with self.assertRaises(ValueError):sdk.unit_spec("sdk-other-tail")
    def test_tail_opcode_addend_and_relocation_forgery_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            p=Path(tmp)/"forged.o";fixture(p);original=p.read_bytes()
            _,e,hs,_=sdk._symbol_tables(p)
            text=next(s for s in e["sections"] if s["name"]==".text")
            rel=next(h for h in hs if h[1]==9 and h[5])
            for offset,value in [(text["offset"]+304,0x0c000000),(text["offset"]+304,0x08000001),
                                 (text["offset"]+124,0x08000000),(rel[4]+8,300)]:
                raw=bytearray(original);struct.pack_into("<I",raw,offset,value);p.write_bytes(raw)
                with self.subTest(offset=offset,value=value),self.assertRaises(ValueError):
                    sdk.inspect_unit_object(p,UNIT)
    def test_source_specific_review_cannot_change_object_traits(self):
        c=sdk.load_catalog(ROOT,UNIT);r=sdk.read(ROOT/sdk.unit_spec(UNIT)["review"])
        for key,value in [("helpers",1),("relocations",4),("size",340),("GP",True)]:
            bad=copy.deepcopy(r);bad["object_contract"][key]=value
            with self.subTest(key=key),self.assertRaises(ValueError):sdk.validate_review(ROOT,c,bad)
    def test_source_specific_tail_never_promotes_generic_profile(self):
        c=sdk.load_catalog(ROOT,UNIT)
        self.assertEqual(c["admission"],"qualified_exact_source_unit_only")
        self.assertEqual(c["traits_scope"],"matched_this_source_only_not_general_SDK64_or_original_types")
        self.assertEqual(sdk.unit_spec(UNIT)["externals"],{"SetD3Chcr":0x130AB0,"SetD4Chcr":0x130B18})

if __name__=="__main__":unittest.main()
