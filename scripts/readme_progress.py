"""Render the README bar and status table from validated integration proofs."""
from __future__ import annotations

import argparse
import json
from datetime import datetime
from pathlib import Path

from decomp_report import generate, validate_object_proof

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / "progress/decompilation.svg"
README = ROOT / "README.md"
START = "<!-- generated-progress:start -->"
END = "<!-- generated-progress:end -->"


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


def current_progress() -> tuple[dict, list[dict], str]:
    read = lambda name: json.loads((ROOT / name).read_bytes())
    levels = [read(f"progress/levels/{row['level']}.json") for row in read("config/overlays.json")["levels"]]
    if len(levels) != 27:
        raise ValueError("README progress requires all 27 overlays")
    integration = read("progress/integration.json")
    gates = read("progress/report.json")
    report = generate(read("config/progress-scope.json"), read("config/target.json"),
                      read("config/overlays.json"), gates, integration, levels)
    validate_object_proof(integration, read("progress/candidates.json"))
    native = [item for proof in levels for item in proof["functions"]
              if item.get("origin") == "level-native"]
    return report, native, gates["verified_at"]


def current_svg() -> str:
    report, _, _ = current_progress()
    return render(int(report["measures"]["matchedCode"]), int(report["measures"]["totalCode"]))


def render_table(report: dict, native: list[dict], verified_at: str) -> str:
    categories = {row["id"]: row["measures"] for row in report["categories"]}
    boot, levels = categories["boot"], categories["levels"]
    matched, total = (int(report["measures"][key]) for key in ("matchedCode", "totalCode"))
    date = datetime.fromisoformat(verified_at)
    months = ("January", "February", "March", "April", "May", "June", "July",
              "August", "September", "October", "November", "December")
    return f'''Recorded validation on **{date.day} {months[date.month - 1]} {date.year}**:

| Scope | Integrated C functions / placements | Matched C bytes |
| --- | ---: | ---: |
| Boot | {int(boot["completeUnits"]):,} functions | {int(boot["matchedCode"]):,} |
| 27 level overlays | {int(levels["completeUnits"]):,} placements | {int(levels["matchedCode"]):,} |
| Native overlay subset, included above | {len(native):,} placements | {sum(item["size"] for item in native):,} |
| **Total C coverage** | **Boot + all 27 overlays** | **{matched:,} / {total:,} ({matched / total * 100:.4f}%)** |
'''


def update_readme(text: str, table: str) -> str:
    if text.count(START) != 1 or text.count(END) != 1:
        raise ValueError("README requires exactly one generated progress block")
    before, rest = text.split(START)
    _, after = rest.split(END)
    return before + START + "\n" + table + END + after


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="reject a stale bar or table without writing")
    args = parser.parse_args()
    report, native, verified_at = current_progress()
    data = render(int(report["measures"]["matchedCode"]), int(report["measures"]["totalCode"])).encode("utf-8")
    original = README.read_text(encoding="utf-8")
    updated = update_readme(original, render_table(report, native, verified_at))
    if args.check:
        if not OUTPUT.exists() or OUTPUT.read_bytes() != data:
            raise ValueError("Stale README progress bar; run python scripts/readme_progress.py")
        if original != updated:
            raise ValueError("Stale README progress table; run python scripts/readme_progress.py")
    else:
        OUTPUT.write_bytes(data)
        if original != updated:
            README.write_text(updated, encoding="utf-8", newline="\n")
    print("README progress bar and table are current")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print(f"README progress failed: {error}")
        raise SystemExit(2)
