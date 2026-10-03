"""Qualify one overlay's own C source; ELF/object outputs stay private."""
from __future__ import annotations
import argparse
import json
import uuid
from pathlib import Path
from level_native import ROOT, paths, qualify


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--level", required=True)
    parser.add_argument("--reference", required=True, type=Path)
    parser.add_argument("--toolchain", required=True, type=Path)
    parser.add_argument("--runtime", required=True, type=Path)
    parser.add_argument("--write-review", action="store_true")
    args = parser.parse_args()
    runtime = args.runtime.resolve()
    if runtime == ROOT or ROOT in runtime.parents or runtime in ROOT.parents or ROOT in args.reference.resolve().parents:
        raise ValueError("Native ELF/object artifacts must remain outside sources")
    work = runtime / "native-candidates" / uuid.uuid4().hex[:8]
    _, _, proof = qualify(args.reference.resolve(), work, args.toolchain.resolve(), args.level)
    if args.write_review:
        destination = ROOT / paths(args.level)[2]
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(json.dumps(proof, indent=2) + "\n", encoding="utf-8", newline="")
    print(json.dumps({"report": str(work / "object-qualification.json"), "functions": proof["functions"]}, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print(f"Native candidate gate failed: {error}")
        raise SystemExit(2)
