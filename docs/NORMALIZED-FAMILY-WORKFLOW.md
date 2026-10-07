# Discover and qualify shared C families

Use this workflow to find candidate copies and qualify a reviewed canonical C
body with explicit per-program bindings. Discovery and preparation add no C
credit. Only the existing exact complete-symbol and full-image checks can accept
matching C. The [validated GS pilot](NORMALIZED-FAMILY-VALIDATION.md) demonstrates
both successful reuse and a wrong binding that a masked comparison would hide.

## Discover candidates from measured intervals

Keep reference ELF files and the function scope outside the repository. The
reference root contains `boot.elf` and `levels/<id>/overlay.elf`, with identities
pinned by the target configuration. Supply complete aligned, non-overlapping
intervals and their boundary evidence in a private scope JSON:

```json
{
  "schema": 1,
  "target": "SCUS_972.68",
  "functions": [
    {
      "program": "levels/0_aranos_tutorial",
      "address": 3037328,
      "size": 136,
      "boundary_evidence": "Current complete native catalogue and unit review: LVL_0_ARANOS_TUTORIAL_FUN_002E5890"
    }
  ]
}
```

Optional `reference_sha256` and `body_sha256` fields add explicit input pins.
Boundary descriptions remain supplied evidence; this tool does not recover or
certify original function boundaries. References must be pinned executable EE
`.text` or `core.text`, with unambiguous loaded/file-backed mappings. VU payload,
overlapping intervals, aliases, wrong references and truncated extents are refused.

```powershell
python scripts/family_discovery.py --references <private-reference-root> --functions <private-scope.json> --output <new-private-discovery.json>
```

The original stdlib normalizer masks only the 26-bit destination of J/JAL. It
retains the opcode, every other instruction field, constants, GP offsets,
address halves, COP2 operands and trailing NOPs. Every input is validated before
an output is created; existing outputs are never replaced. Reports contain
hashes, explicit placements and evidence metadata, with `matching: false` and
zero credit. A shared shape can call different functions and must be reviewed.
It is not a unique-source denominator or a new progress percentage.

## Prepare one reviewed C body and explicit bindings

The checked [GS spec](../config/family-candidates/gs-buffer-setup.json) names the
canonical authored fragment, its exact hash, identifier-only substitutions and
three helper roles. Other specs can supply explicit hashed declaration/type
context fragments. All placeholders and roles must be accounted for; includes,
assembly, implicit helpers and unreviewed file-scope data are refused.

Preparation checks each current source-layout recipe, native catalogue and
complete-unit review. It verifies source/context/catalogue hashes, current tool
identities and private reference pins before writing. It reuses each placement's
qualified flags and GP; it does not invent an ABI, data address or new profile.

The runtime has the same private references at
`references/levels/<id>/overlay.elf`. Choose a fresh bank id each time:

```powershell
python scripts/family_candidates.py --repo . --runtime <private-runtime> --spec config/family-candidates/gs-buffer-setup.json --batch gs-review-001
```

Use repeated `--level <id>` arguments for a bounded subset. The fresh bank is
`<private-runtime>/bank/family-candidates/gs-review-001/`. It contains one
standalone `source.c`, separate binding catalogues, `tasks.json`, a dependency
receipt and `discovery-input.json`. Task pointers use `runtime:`; no machine
paths or retail bytes belong in the public register. G0 and G8 placements become
separate tasks, so one compilation cannot silently mix profiles. The tool neither
compiles nor registers a task. Existing or partial banks are preserved and cannot
be reused as new evidence.

Inspect the scope with `family_discovery.py`, then use the maintained campaign:

```powershell
python scripts/campaign.py --runtime <private-runtime> plan <private-runtime>/bank/family-candidates/gs-review-001/tasks.json
python scripts/campaign.py --runtime <private-runtime> trial gs-buffer-setup-control-gs-review-001-g0 --toolchain <qualified-C-linker-toolchain>
python scripts/campaign.py --runtime <private-runtime> trial gs-buffer-setup-control-gs-review-001-g8 --toolchain <qualified-C-linker-toolchain>
```

The register retains immutable compilation inputs and every child result. The
same object is linked with each child's measured bindings and compared against
every byte of its complete reference function, without masking. Review children
individually; do not substitute a process exit code or discovery hash for proof.
These generated tasks are known-family controls and must not be integrated as
additional native aliases. Keep the public/native per-program symbol rules intact.

## Retain negative controls and meaningful variants

For the GS rejection test, add this option to preparation with a fresh bank id:

```powershell
--negative-binding 0_aranos_tutorial:FamilyGSFourWords:FamilyGSTwoWords
```

It clones one private catalogue and deliberately binds one role to another
measured helper. Positive inputs remain intact. The affected task is expected to
report an aggregate mismatch while retaining exact positive children and the
wrong-binding refusal. Never execute that deliberately incorrect linked candidate.

Preserve constant/layout variants rather than merging them through address
guessing. The retail counterexample in the pilot is separated by this narrower
normalizer. For actual source integration, author canonical fragments and explicit
placements under `src/`, qualify the intended complete units and run the full
boot/all-27-overlay loaded-byte and metadata gates through the
[campaign workflow](CAMPAIGN-WORKFLOW.md). Only those integration proofs update
the existing progress metric.
