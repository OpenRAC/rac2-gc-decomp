"""Render paired unique-code and loaded-code metrics from validated objdiff reports.

The exporters own proof validation and grouping. This renderer never estimates a
denominator or derives a unique numerator from the loaded-code numerator.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
START = "<!-- unique-code-progress:start -->"
END = "<!-- unique-code-progress:end -->"


def counts(report: dict) -> tuple[int, int]:
    m = report["measures"]
    values = []
    for key in ("matchedCode", "totalCode"):
        value = m[key]
        if isinstance(value, str):
            if not value.isascii() or not value.isdigit():
                raise ValueError(f"Invalid {key}")
            value = int(value)
        if type(value) is not int:
            raise ValueError(f"Invalid {key}")
        values.append(value)
    done, total = values
    if not 0 <= done <= total or total <= 0:
        raise ValueError("Invalid code byte ratio")
    return done, total


def paired_metrics(unique: dict, physical: dict) -> dict:
    unique_done, unique_total = counts(unique)
    physical_done, physical_total = counts(physical)
    if unique_total > physical_total or unique_done > physical_done:
        raise ValueError("Unique-code counts exceed loaded-code counts")
    return {
        "schema_version": 1,
        "unique_code": {"matched_bytes": unique_done, "total_bytes": unique_total,
                        "matched_percent": unique_done / unique_total * 100},
        "loaded_code": {"matched_bytes": physical_done, "total_bytes": physical_total,
                        "matched_percent": physical_done / physical_total * 100},
    }


def catalogue_metrics(catalogue: dict, physical: dict) -> dict:
    """Retain incomplete coverage explicitly instead of promoting a subset to global."""
    complete = catalogue["catalogue_complete"]
    if type(complete) is not bool:
        raise ValueError("Invalid catalogue completeness")
    subset = catalogue["validated_subset"]
    global_counts = catalogue.get("metrics")
    metrics = paired_metrics({"measures": {
        "matchedCode": global_counts["unique_matched_bytes"] if global_counts else subset["matched_c_unique_bytes"],
        "totalCode": global_counts["unique_total_bytes"] if global_counts else subset["total_unique_bytes"],
    }}, physical)
    coverage = catalogue["coverage"]
    fields = ("scoped_ee_bytes", "catalogued_interval_bytes", "verified_boundary_bytes",
              "unresolved_gap_bytes", "excluded_vu_bytes")
    for key in fields:
        if type(coverage[key]) is not int or coverage[key] < 0:
            raise ValueError(f"Invalid coverage field: {key}")
    if complete and coverage["unresolved_gap_bytes"]:
        raise ValueError("Complete catalogue cannot contain unresolved gaps")
    partition = catalogue["provisional_partition"]
    if type(partition["representative_bytes"]) is not int or partition["representative_bytes"] < 0:
        raise ValueError("Invalid provisional partition")
    metrics.update({"catalogue_complete": complete,
                    "coverage": {key: coverage[key] for key in fields},
                    "provisional_partition": {"representative_bytes": partition["representative_bytes"],
                                              "certified": partition["certified"]}})
    metrics["unique_code"]["scope"] = "conservative_global_ee" if global_counts else "global" if complete else "verified_boundary_subset"
    if global_counts:
        metrics["validated_subset"] = subset
        metrics["quality"] = catalogue["quality"]
    return metrics


def unique_label(metrics: dict) -> str:
    if metrics["unique_code"].get("scope") == "conservative_global_ee":
        return "Conservative unique EE code (unsupported extents uncollapsed)"
    return "Unique code (verified grouping policy)" if metrics.get("catalogue_complete", True) else "Verified boundary subset (unique code)"


def render_table(metrics: dict) -> str:
    rows = []
    for key, label in (("unique_code", unique_label(metrics)),
                       ("loaded_code", "Loaded code (boot + 27 overlays)")):
        m = metrics[key]
        rows.append(f"| {label} | {m['matched_bytes']:,} | {m['total_bytes']:,} | {m['matched_percent']:.4f}% |")
    result = "| Metric | Matched C bytes | Total code bytes | Progress |\n| --- | ---: | ---: | ---: |\n" + "\n".join(rows) + "\n"
    if "coverage" in metrics:
        c = metrics["coverage"]
        result += f"\nStructurally supported function extents cover {c['verified_boundary_bytes']:,} loaded EE bytes; {c['unresolved_gap_bytes']:,} EE bytes remain unresolved. VU code excluded: {c['excluded_vu_bytes']:,} bytes.\n"
        p = metrics["provisional_partition"]
        result += f"Provisional representative partition: {p['representative_bytes']:,} bytes (certified: {str(p['certified']).lower()}); no global progress percentage is inferred from this partition.\n"
    if "quality" in metrics:
        q, subset = metrics["quality"], metrics["validated_subset"]
        result += f"Conservative global partition retains unknown extents and gaps without deduplication: {q['unknown_bytes']:,} loaded EE bytes have unsupported boundaries. The total follows the stated grouping policy and is not a certified original-source size. Supported subset: {subset['matched_c_unique_bytes']:,} / {subset['total_unique_bytes']:,} unique bytes.\n"
    return result


def render(metrics: dict) -> str:
    body = []
    primary = "CONSERVATIVE UNIQUE EE CODE" if metrics["unique_code"].get("scope") == "conservative_global_ee" else "UNIQUE CODE" if metrics.get("catalogue_complete", True) else "VERIFIED BOUNDARY SUBSET"
    for index, (key, label) in enumerate((("unique_code", primary), ("loaded_code", "LOADED CODE"))):
        m = metrics[key]
        y = 28 + index * 112
        width = m['matched_bytes'] / m['total_bytes'] * 700
        unit_label = "grouped bytes under the stated policy" if key == "unique_code" else "loaded bytes across all placements"
        body.append(f'''<text x="30" y="{y}" fill="#c9d1d9" font-size="14" font-weight="600">{label}</text>
<text x="730" y="{y}" text-anchor="end" fill="#e89b35" font-size="20">{m['matched_percent']:.4f}%</text>
<rect x="30" y="{y + 15}" width="700" height="18" rx="9" fill="#30363d"/>
<clipPath id="rail{index}"><rect x="30" y="{y + 15}" width="700" height="18" rx="9"/></clipPath>
<rect x="30" y="{y + 15}" width="{width:.6f}" height="18" fill="#e89b35" clip-path="url(#rail{index})"/>
<text x="30" y="{y + 59}" fill="#c9d1d9" font-size="14">{m['matched_bytes']:,} / {m['total_bytes']:,} {unit_label}</text>''')
    return '''<svg xmlns="http://www.w3.org/2000/svg" width="760" height="242" viewBox="0 0 760 242" role="img" aria-labelledby="title description">
<title id="title">RAC2 unique and loaded code progress</title>
<desc id="description">Two separate measured ratios. Unique code follows the documented structural grouping policy; loaded code counts every executable placement across the boot and 27 overlays.</desc>
<rect width="760" height="242" rx="12" fill="#0d1117"/>
<g font-family="Segoe UI, Arial, sans-serif">''' + "\n".join(body) + '''
<text x="30" y="230" fill="#8b949e" font-size="12">Matching C / C++ | Target: 100% | See the metric policy and its remaining limitations</text>
</g></svg>
'''


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--catalogue-report", type=Path, required=True)
    parser.add_argument("--physical-report", type=Path, required=True)
    parser.add_argument("--repo", type=Path, default=ROOT)
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    physical = json.loads(args.physical_report.read_bytes())
    result = catalogue_metrics(json.loads(args.catalogue_report.read_bytes()), physical)
    output = args.output_dir or args.repo / "progress"
    expected = {
        output / "paired-code-metrics.json": json.dumps(result, indent=2) + "\n",
        output / "unique-decompilation.svg": render(result),
    }
    if args.output_dir is None:
        readme = args.repo / "README.md"
        original = readme.read_text(encoding="utf8")
        if original.count(START) != 1 or original.count(END) != 1:
            raise ValueError("README needs exactly one unique-code marker pair")
        block = START + "\n" + render_table(result).rstrip() + "\n" + END
        expected[readme] = re.sub(re.escape(START) + r".*?" + re.escape(END), lambda _: block, original, flags=re.S)
    else:
        expected[output / "progress-table.md"] = render_table(result)
    for path, text in expected.items():
        if args.check:
            if not path.exists() or path.read_text(encoding="utf8") != text:
                raise ValueError("Stale paired display: " + path.name)
        else:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text, encoding="utf8", newline="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
