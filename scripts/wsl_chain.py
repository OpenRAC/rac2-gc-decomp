"""RAC2 C compilation through the reconstructed 2.9-ee chain, hosted in WSL.

The 1999-era tools are 32-bit Linux binaries: they run under WSL, but they
cannot stat files on /mnt/d (EOVERFLOW on 64-bit inodes), so each source is
copied into the WSL filesystem, compiled there under its bare name (the
compiler writes the spelling it received into `.file`, which ends up in the
object), and the results are copied back. The linker stays the SDK `ld.exe`
used by the previous profile.

Environment (defaults match this machine):
    RAC2_WSL_DISTRO   WSL distribution           (default "Ubuntu")
    RAC2_WSL_TOOLS    directory holding cpp/cc1/as in WSL
                      (default "/root/essai-rac2/outils-rac2")
    RAC2_WSL_TMP      parent for the temporary build dirs in WSL
                      (default "/root/essai-rac2")

The tool hashes recorded in the proofs are computed from the actual files:
cc1, cpp and as through `sha256sum` in WSL, `ld.exe` from the toolchain
directory on the Windows side.
"""

from __future__ import annotations

import hashlib
import os
import subprocess
from pathlib import Path

DISTRO = os.environ.get("RAC2_WSL_DISTRO", "Ubuntu")
TOOLS = os.environ.get("RAC2_WSL_TOOLS", "/root/essai-rac2/outils-rac2")
TMPROOT = os.environ.get("RAC2_WSL_TMP", "/root/essai-rac2")
LINKER = ("ee", "bin", "ld.exe")


def wsl_path(path: Path) -> str:
    """D:\\a\\b -> /mnt/d/a/b (the WSL spelling of a Windows path)."""
    resolved = Path(path).resolve()
    drive = resolved.drive.rstrip(":").lower()
    if not drive:
        raise ValueError(f"Not a Windows path: {path}")
    rest = resolved.as_posix().split(":", 1)[1]
    return f"/mnt/{drive}{rest}"


def _run(script: str, log: Path | None) -> None:
    result = subprocess.run(["wsl.exe", "-d", DISTRO, "-e", "bash", "-lc", script],
                            capture_output=True, text=True, timeout=900)
    if log is not None:
        Path(log).write_text(result.stdout + result.stderr, encoding="utf-8")
    if result.returncode:
        raise ValueError(f"Reconstructed chain failed ({result.returncode}); see {log}")


def compile_c(source: Path, flags: list[str], object_out: Path,
              assembly_out: Path | None = None, log: Path | None = None) -> None:
    """cpp + cc1, then `as`; writes the object (and optionally the assembly)."""
    source = Path(source).resolve()
    steps = [
        "set -e",
        f"T={TOOLS}",
        f"tmp=$(mktemp -d {TMPROOT}/tmp.XXXXXX)",
        "cd $tmp",
        f"cp '{wsl_path(source)}' {source.name}",
        f"$T/cpp {source.name} > {source.stem}.i",
        f"$T/cc1 -quiet {' '.join(flags)} {source.stem}.i -o {source.stem}.s",
    ]
    if assembly_out is not None:
        steps.append(f"cp {source.stem}.s '{wsl_path(assembly_out)}'")
    steps += [
        f"$T/as -o {source.stem}.o {source.stem}.s",
        f"cp {source.stem}.o '{wsl_path(object_out)}'",
        "rm -rf $tmp",
    ]
    _run("\n".join(steps) + "\n", log)


def tool_hashes(toolchain: Path) -> dict:
    """The instruments that produced the object, exactly as the proofs need."""
    noms = ("cc1", "cpp", "as")
    script = " ; ".join(f"sha256sum {TOOLS}/{nom}" for nom in noms)
    result = subprocess.run(["wsl.exe", "-d", DISTRO, "-e", "bash", "-lc", script],
                            capture_output=True, text=True, timeout=300)
    if result.returncode:
        raise ValueError("Cannot hash the WSL chain tools")
    hashes = {}
    for ligne in result.stdout.splitlines():
        empreinte, chemin = ligne.split()
        hashes[Path(chemin).name] = empreinte
    linker = Path(toolchain).resolve().joinpath(*LINKER)
    hashes[LINKER[-1]] = hashlib.sha256(linker.read_bytes()).hexdigest()
    return hashes
