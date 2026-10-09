"""Group the current verified physical report without changing matching proofs."""
from __future__ import annotations

import argparse
import json
from pathlib import Path

import decomp_report


ROOT = Path(__file__).resolve().parents[1]


def verified_report(repo: Path) -> dict:
    def read(relative: str) -> dict:
        return json.loads((repo / relative).read_bytes())
    overlays = read("config/overlays.json")
    integration = read("progress/integration.json")
    levels = [read("progress/levels/" + row["level"] + ".json") for row in overlays["levels"]]
    report = decomp_report.generate(read("config/progress-scope.json"), read("config/target.json"),
                                    overlays, read("progress/report.json"), integration, levels)
    decomp_report.validate_object_proof(integration, read("progress/candidates.json"))
    return report


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--source-module-summary", type=Path)
    args = parser.parse_args()
    # Complete the original validators before loading the presentation mapper.
    report = verified_report(ROOT)
    from progress_modules import group_report
    grouped, summary = group_report(report, ROOT)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(grouped, indent=2) + "\n", encoding="utf-8", newline="")
    if args.source_module_summary is not None:
        args.source_module_summary.parent.mkdir(parents=True, exist_ok=True)
        args.source_module_summary.write_text(json.dumps(summary, indent=2) + "\n",
                                              encoding="utf-8", newline="")
    print(json.dumps({"output": str(args.output), "units": len(grouped["units"]),
                      "total_code": grouped["measures"]["totalCode"],
                      "matched_code": grouped["measures"]["matchedCode"]}))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError) as error:
        print("Grouped report export failed: " + str(error))
        raise SystemExit(2)
