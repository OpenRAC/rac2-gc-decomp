# Maintained C campaign workflow

Use the maintained campaign commands instead of creating a runner for each
function. The campaign owns scheduling and experiment bookkeeping; the existing
compiler, complete-symbol comparison and full-image integration gates remain
the acceptance criteria. This organization change adds no matching credit.

## One register, derived views

`config/campaign-register.json` is the canonical experiment and task register.
Queue, status, function packets and the historical Markdown register are views
of that data. Do not maintain another queue or manually edit a generated view.
The migration retains historical target rows and operational observations,
including failures and missing proofs. It does not convert an old exact result
into a current compiler qualification or integration proof.

Record one compilation event per immutable trial, with child results for the
selected targets. If compilation fails, those targets are unmeasured; they are
not independent compiler failures. Keep the exact source, catalog, compiler
flags, reference and tool hashes, object, assembly, logs and result in a private
runtime directory outside this repository. A repeated source/catalog/profile
requires an explicit new reason. Never reuse an output directory as a new trial.

A parked target requires a concrete reopening condition: new ABI evidence, a
measured type/layout, a changed compiler hypothesis, or a different algorithm.
Renaming locals or cycling equivalent expressions does not supply that evidence.
Ghidra holds analysis and annotations; the register holds experiment history
and next actions. PCSX2 observations remain runtime evidence, separate from
compiler equality and C integration credit.

## Source organization

Authored boot modules retain their order and shared declaration context when
assembled into the standalone `candidates/boot.c` compilation unit. A module
boundary is an organizational boundary, not a recovered original object file.
Splitting into independently compiled objects requires a separate byte proof.

Native shared families use a canonical authored body and explicit per-program
placements. A generator produces the standalone level units consumed by the
existing checker. The pilot preserves every output byte, including source
spelling, whitespace and declarations; object provenance therefore stays valid.
Other native bodies remain explicit authored source until equivalence is proven.

The commands and authoring safeguards are documented in
[`SOURCE-LAYOUT.md`](SOURCE-LAYOUT.md). The checked source inventory is
[`progress/source-inventory.json`](../progress/source-inventory.json).

## Commands and task packets

Run commands from the repository with an explicit private runtime. That runtime
holds immutable trial/action directories, registry revisions and view backups.
`--registry` is a test/bootstrap override; the live default remains the one
versioned `config/campaign-register.json`.

```powershell
python scripts/campaign.py --runtime <private-runtime> status
python scripts/campaign.py --runtime <private-runtime> queue
python scripts/campaign.py --runtime <private-runtime> packet 24_ship_shack-fun_002a4468
python scripts/campaign.py --runtime <private-runtime> plan <task-spec.json>
python scripts/campaign.py --runtime <private-runtime> amend <task-id> <changes.json> --reason "Measured new evidence"
python scripts/campaign.py --runtime <private-runtime> trial <task-id> --toolchain <C-linker-toolchain>
python scripts/campaign.py --runtime <private-runtime> views
python scripts/campaign.py --runtime <private-runtime> views --check
```

The generated [queue view](CAMPAIGN-QUEUE.md) includes the retained parked and
closed tasks. The `queue` command returns only active queued work, ordered by
priority. A packet combines program, address and reference pin, planning/ABI/type
context, best source and remaining-difference pointers where recorded, recent
historical results and the latest trial. Extracted assembly belongs in private
evidence, not in the public packet. Add a measured context pointer with `amend`
when it becomes available; an empty field does not establish a type or ABI.

A candidate spec names one source compilation unit and one or more child
reference/catalog targets. Unprefixed paths are repository-relative; `runtime:`
paths bind to the chosen private runtime and cannot escape it. For example:

```json
{
  "id": "ship-shack-new-unit",
  "kind": "candidate",
  "source": "runtime:bank/24_ship_shack.c",
  "symbols": ["LVL_24_SHIP_SHACK_FUN_002A4468"],
  "hypothesis": "One measured ABI or source-shape change",
  "next_action": "Compare every complete symbol in the unit",
  "reopen_condition": "New ABI, type or compiler evidence",
  "budget": 6,
  "targets": [{"id": "ship-shack", "reference": "runtime:references/24_ship_shack.elf",
               "catalog": "runtime:bank/24_ship_shack.json"}]
}
```

The catalog defines measured scope; `symbols` filters packet history only. Source
must remain standalone C. The default profile is `progress/candidates.json`, and
actual cc1/cpp/as/linker hashes must agree. Research tasks carry analysis or naming
work without compiler targets. Historical source rows were conservatively seeded
as research tasks: accepted functions are closed by current validated proofs,
trial-only winners retain no credit before integration, and unresolved trials stay parked.

A native catalog may pin one whole compiler-generated `.rodata` section with
`read_only_sections`: `section`, `address`, `size` and `sha256` are required.
The unchanged compiled object must contain exactly that allocated non-executable
data section, with readonly flags and the complete reviewed size. The linked
table must occupy the reviewed address and match every pinned reference byte.
Reference tables may reside in the original writable `.data`; this does not
change their loaded metadata. A missing table, other generated data, a truncated
extent or one different byte is a refusal even when every function matches.
Data adds no C code coverage. This placement qualifies an object only; source
publication still requires replacement of the original data extent and the
complete loaded-image and metadata gates. Do not embed a retail table in C or
rewrite the switch to avoid its generated data.

Before selecting a new matching target, check its complete program/address/size
span against every published function in that program's `progress/levels` proof,
including both `boot-shared` and `level-native` origins. A new native symbol or
an unrecorded trial does not establish an uncovered body. Already integrated C
may be recompiled as a stated control, with zero new credit. Keep duplicate-source
trials and the integration overlap refusal rather than weakening that gate.

`private-work:` pointers bind to the project's private work bank; `prepared-run:`
pointers bind to the preparation-manifest directory used by a build. These are
evidence references, not portable files shipped in the repository. Each machine's
private continuation prompt supplies the exact roots. The import retained all
169 original rows: 158 native records, 9 boot records and 2 runtime observations.

## Complete build and closure

Before compiling, establish function boundaries and ABI from the pinned
instructions and relevant callers/callees. A default Ghidra signature or unused
register residue does not justify an invented return value. Prototype changes
require requalification of affected controls. Reuse existing typedefs and use
the native catalog loader's defaults rather than assuming raw JSON contains `gp`.

Measure complete symbols independently from the same compiled object when an
oversized body would prevent linking the other candidates. Retain every refusal,
then qualify all winners and existing controls together in the intended unit.
Do not trim a section, patch bytes or promote an isolated private match directly.

Write versioned sources and reviews as UTF-8/LF. Install the fresh boot review
before a reconstruction that depends on it; normalizing its line endings after
the build changes dependency hashes. Freeze sources, catalogs, reviews, tools and
flags during a batch. Follow long WSL jobs in a supervised foreground process;
do not rely on a detached `nohup` job surviving the `wsl.exe` session.

For proof publication, first validate the new private boot/level integration
proofs with `decomp_report.py --integration-proof ... --progress-proof ...` and
the complete set of affected `--level-proof` paths. Back up public destinations
before copying reviewed proof metadata. Publish a coherent `progress/report.json`,
boot integration and per-level proofs, retaining unaffected proofs when their
inputs are unchanged. The report's boot counters must agree with its measured
G1 result; neither counters nor a process exit code can substitute for the gates.
Retired batch scripts and finalizers are historical evidence, not the active path.

```powershell
python scripts/campaign.py --runtime <private-runtime> integrate -- --manifest <private-manifest.json> --toolchain <ASM-toolchain> --c-toolchain <C-linker-toolchain> --program-jobs 4 --jobs 2
python scripts/campaign.py --runtime <private-runtime> report -- --level-proof progress/levels/<level>.json
python scripts/campaign.py --runtime <private-runtime> close <candidate-task-id>
```

Pass all 27 `--level-proof` arguments to `report` for the full coverage export.
`build` and `integrate` run the complete boot and 27-overlay C gates through
`campaign_build.py`; they do not edit sources, publish proofs or push Git. Program
parallelism and assembler parallelism are separate and bounded explicitly.
There is no stale-result skip. A fresh UUID, reference manifest, source/checker
inventory and reconstruction tool hashes bind each action to its gate report.

After reviewing and publishing the current integration proofs, `close` validates
them through the existing exporter and requires every complete task function to
be integrated at its pinned program/address/size. Neither manual state changes
nor a private match can mark a candidate integrated. Closure adds zero credit;
the existing progress exporter is the authority for matching bytes.

## Organization pilot validation, 3 October 2026

The source pilot regenerated 28 byte-identical standalone units. Two fresh
complete batches passed the boot and all 27 loaded-byte/metadata gates; the
second ran through the campaign CLI with its UUID-bound artifact checks.
The independent exporter validated that batch's private boot/level integration
proofs and retained **221,760 / 48,788,176** integrated C bytes.
The aggregate exposes boot function counters for the existing progress validator;
these counters derive from the measured G1 result, not trial matches.

**214 tool tests pass**, including authoring, stale fragments/views, overwrite
guards, shared compile failures, duplicate trials, final instrument-read failure,
old artifact rejection, stable proof closure and exporter metadata compatibility.
Both generated-view and separate source-inventory freshness checks pass.
No matching function or compiler profile was changed by this organization lot.

Do not add C includes to the qualification path casually. The current compiler
copies a standalone source into WSL. Source fragments are assembled before that
step, so the checked standalone source contains the entire dependency closure.
A future header-based compilation path must snapshot and hash transitive headers
and reproduce their include paths in WSL before it can replace this arrangement.

## What the metrics mean

The progress exporter continues to count only integrated C bytes proven against
each program's complete loaded image. A shared body placed in 27 programs
contributes 27 measured placements to coverage. The separate source inventory
counts a verified canonical family once and lists its placements explicitly.
Similar size or a matching prefix never establishes family identity.

Neither authored-source metrics, historical exact trials, generated assembly,
nor emulator observations increase the C integration numerator. Organization
pilots keep the existing denominator and progress total unchanged.

## Publication

Run source/view freshness checks and the tool tests before committing. If a
compiled source, catalog, checker or tool changes, regenerate its object review
and every affected complete-image gate before exporting progress. Read the
structured gate results; a subprocess exit code alone is insufficient evidence.
Keep game images, extracted assembly, SDKs, objects, binaries, private paths and
scratch candidates outside Git. Publish coherent tested lots with a scoped
English commit subject and a body describing behavior, validation and limits.

This workflow was informed by the RAC1 project's campaign and source layout at
[`405cddd`](https://github.com/Lynder063/rac1-decomp/tree/405cddd940d700c59aec48c7e162ee229eeb5ea5).
Its relocation-masked trial acceptance and assembly-inclusive counters are not
used here. RAC2 keeps complete symbol equality and all-program loaded-byte gates.
