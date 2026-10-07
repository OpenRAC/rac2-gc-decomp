"""Resolve the pinned identities of one game region.

The USA v1.01 matching target keeps its historical identity files,
``config/target.json`` and ``config/overlays.json``, so every existing proof
and hash stays byte-identical. Further regions are declared in
``config/regions.json``. A region whose disc, boot or overlays have not been
measured is unpinned: ``setup.py --measure-identity`` may measure it and
``build.py`` may round-trip its assembly, but no tool accepts it as a pinned
identity, and only a region with ``matching`` set has C catalogues, reviews
and progress proofs.

Selection order: an explicit ``--region``, then ``RAC2_REGION``, then the
registry default. A manifest or catalogue names its serial; ``by_serial``
resolves the region that owns it.
"""
from __future__ import annotations

import json
import os
import re
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
REGISTRY = "config/regions.json"
ENVIRONMENT = "RAC2_REGION"
# A checkout or test fixture without the registry has exactly this one region.
FALLBACK = {"schema": 1, "default": "ntsc-u", "aliases": {"ntsc": "ntsc-u"},
            "regions": {"ntsc-u": {"label": "USA v1.01", "serial": "SCUS_972.68",
                                   "target": "config/target.json", "overlays": "config/overlays.json",
                                   "matching": True}}}


@dataclass(frozen=True)
class Region:
    name: str
    label: str
    serial: str
    target: dict
    overlays: dict | None
    matching: bool

    @property
    def pinned(self) -> bool:
        """Disc, boot and every overlay identity are measured and recorded."""
        return (all(isinstance(self.target.get(key), dict) and self.target[key].get("sha256")
                    for key in ("iso", "boot"))
                and type(self.target.get("expected_levels")) is int
                and self.overlays is not None
                and len(self.overlays["levels"]) == self.target["expected_levels"])

    @property
    def expected_levels(self) -> int | None:
        return self.target.get("expected_levels")

    def overlay_pins(self) -> dict:
        self.require_pinned("overlay identities")
        return {row["level"]: row["sha256"] for row in self.overlays["levels"]}

    def program_pins(self) -> dict:
        """Pinned SHA-256 per program, named as catalogues and campaign tasks name them.

        Only programs whose identity is recorded are returned; a catalogue for any
        other program is refused by the caller."""
        if not isinstance(self.target.get("boot"), dict) or not self.target["boot"].get("sha256"):
            self.require_pinned("boot identity")
        levels = [] if self.overlays is None else self.overlays["levels"]
        return {"boot": self.target["boot"]["sha256"],
                **{"levels/" + row["level"]: row["sha256"] for row in levels}}

    def require_pinned(self, what: str = "identity") -> None:
        if not self.pinned:
            raise ValueError(f"{self.label} ({self.serial}) {what} not pinned yet; measure a legally acquired disc "
                             f"with scripts/setup.py --region {self.name} --measure-identity and review the proposal")

    def require_matching(self, what: str) -> None:
        if not self.matching:
            raise ValueError(f"{what} exists only for a matching region; {self.label} ({self.serial}) "
                             "has no C catalogues, reviews or progress proofs yet")


def registry(root: Path = ROOT) -> dict:
    path = root / REGISTRY
    document = json.loads(path.read_bytes()) if path.is_file() else FALLBACK
    if document.get("schema") != 1 or document.get("default") not in document.get("regions", {}):
        raise ValueError(f"Invalid region registry: {REGISTRY}")
    return document


def names(root: Path = ROOT) -> list[str]:
    document = registry(root)
    return sorted(document["regions"]) + sorted(document.get("aliases", {}))


def canonical(name: str | None, root: Path = ROOT) -> str:
    document = registry(root)
    name = name or os.environ.get(ENVIRONMENT) or document["default"]
    name = document.get("aliases", {}).get(name.lower(), name.lower())
    if name not in document["regions"]:
        raise ValueError(f"Unknown region {name!r}; choose one of {', '.join(names(root))}")
    return name


def read_identity(root: Path, relative: str | None) -> dict | None:
    if relative is None:
        return None
    path = (root / relative).resolve()
    if not path.is_relative_to(root.resolve()):
        raise ValueError(f"Region identity escapes the repository: {relative}")
    return json.loads(path.read_bytes())


def load(name: str | None = None, root: Path = ROOT) -> Region:
    name = canonical(name, root)
    entry = registry(root)["regions"][name]
    target = read_identity(root, entry["target"])
    overlays = read_identity(root, entry.get("overlays"))
    serial = entry["serial"]
    if not re.fullmatch(r"[A-Z]{4}_\d{3}\.\d{2}", serial) or target.get("serial") != serial:
        raise ValueError(f"Region {name} target does not declare its serial {serial}")
    if overlays is not None and overlays.get("target", serial) != serial:
        raise ValueError(f"Region {name} overlays belong to another target")
    return Region(name, entry["label"], serial, target, overlays, entry.get("matching") is True)


def by_serial(serial: object, root: Path = ROOT) -> Region:
    matches = [name for name, entry in registry(root)["regions"].items() if entry["serial"] == serial]
    if len(matches) != 1:
        raise ValueError(f"No registered region owns target {serial!r}")
    return load(matches[0], root)


def matching(root: Path = ROOT) -> Region:
    """The one region whose catalogues and proofs this repository publishes."""
    found = [name for name, entry in registry(root)["regions"].items() if entry.get("matching") is True]
    if len(found) != 1:
        raise ValueError("The region registry must declare exactly one matching region")
    return load(found[0], root)


def add_argument(parser, root: Path = ROOT) -> None:
    parser.add_argument("--region", choices=names(root),
                        help=f"game region (default: ${ENVIRONMENT} or {registry(root)['default']}); "
                             "see config/regions.json")
