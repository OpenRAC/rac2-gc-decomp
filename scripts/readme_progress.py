"""Render the README progress bar from all currently validated integration proofs."""
from __future__ import annotations

import argparse
import json
from pathlib import Path

from decomp_report import generate, validate_object_proof

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "progress/decompilation.svg"


def render(matched: int, total: int) -> str:
    if type(matched) is not int or type(total) is not int or not 0 <= matched <= total or total <= 0:
        raise ValueError("Invalid integrated code byte counts")
    percent = matched / total * 100
    width = matched / total * 700
    return f'''<svg xmlns="http://www.w3.org/2000/svg" width="760" height="126" viewBox="0 0 760 126" role="img" aria-labelledby="title description">
  <title id="title">RAC2 matching C/C++ progress: {percent:.4f}%</title>
  <desc id="description">{matched:,} of {total:,} code bytes validated across the boot and 27 overlays. Target: 100 percent.</desc>
  <rect width="760" height="126" rx="12" fill="#0d1117"/>
  <g font-family="Segoe UI, Arial, sans-serif">
    <text x="30" y="30" fill="#c9d1d9" font-size="14" font-weight="600">MATCHING C / C++</text>
    <text x="730" y="32" text-anchor="end" fill="#e89b35" font-size="23" font-weight="700">{percent:.4f}%</text>
    <rect x="30" y="47" width="700" height="18" rx="9" fill="#30363d"/>
    <clipPath id="rail"><rect x="30" y="47" width="700" height="18" rx="9"/></clipPath>
    <rect x="30" y="47" width="{width:.6f}" height="18" fill="#e89b35" clip-path="url(#rail)"/>
    <text x="30" y="88" fill="#c9d1d9" font-size="14">{matched:,} / {total:,} validated code bytes</text>
    <text x="30" y="109" fill="#8b949e" font-size="12">Boot + 27 overlays | Full game target: 100%</text>
  </g>
</svg>
'''


def current_svg() -> str:
    read = lambda name: json.loads((ROOT / name).read_bytes())
    levels = [read(f"progress/levels/{row['level']}.json") for row in read("config/overlays.json")["levels"]]
    if len(levels) != 27:
        raise ValueError("README progress requires all 27 overlays")
    integration = read("progress/integration.json")
    report = generate(read("config/progress-scope.json"), read("config/target.json"),
                      read("config/overlays.json"), read("progress/report.json"), integration, levels)
    validate_object_proof(integration, read("progress/candidates.json"))
    return render(int(report["measures"]["matchedCode"]), int(report["measures"]["totalCode"]))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="reject a stale bar without writing")
    args = parser.parse_args()
    data = current_svg().encode("utf-8")
    if args.check:
        if not OUTPUT.exists() or OUTPUT.read_bytes() != data:
            raise ValueError("Stale README progress bar; run python scripts/readme_progress.py")
    else:
        OUTPUT.write_bytes(data)
    print("README progress bar is current: progress/decompilation.svg")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print(f"README progress failed: {error}")
        raise SystemExit(2)
