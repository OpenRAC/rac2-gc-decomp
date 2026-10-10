"""Run explicitly targeted local tests or select bounded maintainer PR tests.

This is a focused check, never a full-suite or matching-gate receipt. The
protected merge queue still runs unittest discovery over every test module.
"""
import argparse
import ast
import json
from pathlib import Path
import re
import subprocess
import sys
import unittest

from maintainer_test_policy import identity, REPOSITORY


def authenticate(repo):
    """Use GitHub's immutable authenticated user ID, not local Git names."""
    remote = subprocess.check_output(["git", "-C", str(repo), "remote", "get-url", "origin"], text=True).strip()
    if remote not in {"https://github.com/" + REPOSITORY + ".git", "https://github.com/" + REPOSITORY,
                      "git@github.com:" + REPOSITORY + ".git"}:
        raise ValueError("Maintainer testing requires the maintained GitHub repository")
    result = subprocess.run(["gh", "api", "--hostname", "github.com", "user"], capture_output=True, text=True)
    if result.returncode or not identity(json.loads(result.stdout)):
        raise ValueError("Maintainer testing requires the authenticated primary maintainer")
    user = json.loads(result.stdout)
    return {"id": user["id"], "login": user["login"]}


def validate_modules(repo, modules):
    values = sorted(set(modules))
    if not values:
        raise ValueError("Specify at least one test module")
    for name in values:
        if not re.fullmatch(r"test_[a-z0-9_]+", name) or not (repo / "tests" / (name + ".py")).is_file():
            raise ValueError("Unknown maintained test module: " + name)
    return values


def check_changed_python(repo, paths):
    """Syntax-only owner precheck; never import executable contributors here."""
    checked = []
    for relative in sorted(set(paths)):
        if not relative.endswith(".py"):
            continue
        path = repo / relative
        if path.is_absolute() and not path.resolve().is_relative_to(repo.resolve()):
            raise ValueError("Changed Python path escapes the repository")
        if path.is_file():
            ast.parse(path.read_bytes(), filename=relative)
            checked.append(relative)
    return checked


def select_modules(repo, paths):
    """Owner PR precheck: smoke + directly changed test modules + syntax.

    This deliberately does not infer transitive coverage or run a heavy suite
    for an orphan executable. It is limited pre-queue feedback; every queued
    contribution still runs unconditional full discovery in the workflow.
    """
    check_changed_python(repo, paths)
    selected = {"test_maintainer_test_policy", "test_maintainer_tests", "test_decomp_report_cli"}
    for relative in paths:
        if Path(relative).parent == Path("tests") and relative.endswith(".py"):
            name = Path(relative).stem
            if name.startswith("test_") and (repo / "tests" / (name + ".py")).is_file():
                selected.add(name)
    return validate_modules(repo, selected)


def run_modules(repo, modules):
    sys.path.insert(0, str(repo / "scripts"))
    sys.path.insert(0, str(repo / "tests"))
    if modules is None:
        print("Unmapped executable change: running the complete suite.", flush=True)
        suite = unittest.defaultTestLoader.discover(str(repo / "tests"))
    else:
        print("Targeted test modules: " + ", ".join(modules), flush=True)
        suite = unittest.defaultTestLoader.loadTestsFromNames(modules)
    if not suite.countTestCases():
        print("No tests were selected; refusing a successful test receipt.", flush=True)
        return 1
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    return 0 if result.wasSuccessful() else 1


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repo", type=Path, default=Path(__file__).resolve().parents[1])
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--test", action="append", help="Maintained test module; repeat for a local focused check")
    group.add_argument("--ci-base", help="Verified protected-base SHA supplied by the CI policy")
    args = parser.parse_args(argv)
    repo = args.repo.resolve()
    if args.test:
        authenticate(repo)
        modules = validate_modules(repo, args.test)
    else:
        if not re.fullmatch(r"[0-9a-f]{40}", args.ci_base):
            parser.error("CI base must be an exact commit SHA")
        paths = subprocess.check_output(["git", "-C", str(repo), "diff", "--name-only", "-z",
                                         args.ci_base, "HEAD"], text=True).split("\0")
        modules = select_modules(repo, [p for p in paths if p])
    return run_modules(repo, modules)


if __name__ == "__main__":
    raise SystemExit(main())
