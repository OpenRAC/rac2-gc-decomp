# Tools: what to obtain and what each one does

**Every contributor needs their own legally acquired matching ISO and the full
tool suite.** Start with the [beginner AI contribution guide](../docs/CONTRIBUTOR-QUICKSTART.md).
Your AI helps prepare and verify the complete environment before choosing a task.
Repository tests can run during setup without game data; that technical capability
does not waive the project's contribution prerequisites.

This directory contains documentation and [requirements metadata](requirements.json),
not SDKs or executables. Download public software from its own upstream project.
Keep installed binaries outside Git: use an external tools directory, or the
ignored `toolchain/local/` folder. Game references and build outputs must stay in
a private runtime outside this checkout.

## Required suite and component roles

| Role | Needed | Validation |
| --- | --- | --- |
| Repository management and checks | Git + Python 3.12 + pinned dependencies + GitHub login | Tests may run during preparation; not contribution readiness alone |
| Prepare your own game reference | Above + pinned dependencies, Wrench, matching disc | Follow [START-HERE](../docs/START-HERE.md) |
| Check actual game C candidates | Prepared reference + qualified GNU EE tools in WSL + SN EE linker | Current runner uses Windows + WSL |
| Integrate C into complete images | Above + qualified SN reconstruction assembler/linker | Current runner uses Windows + WSL |
| Analyze the game | Ghidra with verified R5900 support and analysis access | Required; verify target identity and instruction spans |
| Debug/validate execution | PCSX2, appropriate configuration and permitted local BIOS | Required; observations never replace the byte gates |

## Public tools and official sources

| Tool | What it means | Obtain it here |
| --- | --- | --- |
| Git | Keeps the project's files and changes organized | [Git downloads](https://git-scm.com/downloads/) |
| Python | Runs the project scripts and tests | [Python downloads](https://www.python.org/downloads/); use the project-tested 3.12 line |
| GitHub CLI, optional | Lets the AI manage your fork and pull request from a terminal | [Official GitHub CLI](https://cli.github.com/) |
| WSL, advanced matching route | Runs the historical Linux compiler on Windows | [Microsoft installation guide](https://learn.microsoft.com/en-us/windows/wsl/install) |
| Wrench | Extracts level references from your disc | [Author's releases](https://github.com/chaoticgd/wrench/releases) and [build instructions](https://github.com/chaoticgd/wrench) |
| Ghidra | Helps inspect instructions and function boundaries | [Official releases](https://github.com/NationalSecurityAgency/ghidra/releases) |
| PCSX2 | Runs the PS2 game for controlled observations | [Official downloads](https://pcsx2.net/downloads/) |

Use Wrench's `wrenchbuild`, not its graphical editor, for `setup.py`. Record the
selected version/hash with preparation evidence. Ghidra analysis also needs
verified R5900 language support; a standard MIPS selection must not be assumed
equivalent. Follow each project's own prerequisites and license instructions.
The AI should explain installation steps that require your login, admin rights
or a restart, and obtain the needed permission instead of silently changing the system.

### Analysis access for your AI

For PS2 processor support, evaluate the author's
[Emotion Engine: Reloaded extension](https://github.com/chaoticgd/ghidra-emotionengine-reloaded)
against your Ghidra version and known pinned instructions. Language identifiers
may differ across extensions; verify R5900 decoding rather than copying a name.
For direct AI access, an integration such as
[GhidraMCP](https://github.com/LaurieWired/GhidraMCP) can be configured according to
its own upstream instructions. An installed bridge is not a qualified analysis:
confirm the explicit program, addresses and memory against the reference.

PCSX2's [official debugger guide](https://pcsx2.net/docs/advanced/debugger/)
describes manual access. The author's [PCSX2-MCP bridge](https://github.com/hkmodd/PCSX2-MCP)
offers AI debugging integration; review its license, release/source provenance
and compatibility before installing. See [our validation limits](../docs/PCSX2-VALIDATION.md).
Keep the emulator, BIOS, ISO, bridge configuration and session dumps private.
These upstream links identify projects to configure; they do not establish that
their latest versions reproduce this repository's recorded observations.

## The exact matching compiler is a separate requirement

Current C compilation uses **GNU EE 2.9-ee-991111b** with measured RAC2 changes.
Linking uses the SN EE `ld.exe`. Reconstructed retail assembly separately uses
`Ps2EeAs.exe`. These are different roles; ordinary modern GCC and the old SN
`ee-gcc2953.exe` frontend are not replacements for the currently qualified profile.

Authoritative hashes come from `progress/candidates.json` (`tools`) for C and
`progress/report.json` (`tools`) for reconstruction. Flags come from
`config/candidate-catalog.json`. The JSON manifest references those sources instead
of maintaining another hand-edited hash table. Availability or agreeing hashes
do not prove a candidate: actual complete-symbol/image gates still have to pass.

The reconstructed GNU profile starts from `gnu-ee-binutils-gcc-1.1.tar.gz`
(base archive identity recorded in `requirements.json`) and an inherited patch
stack plus the public adjustments under `scripts/compiler/`. See
[compiler provenance](../docs/COMPILER-NOTES.md). **The exact base acquisition and
complete public rebuild recipe are not yet a self-contained beginner setup.**
The [upstream GNU GCC project](https://gcc.gnu.org/) provides background and public
GNU releases, not a download of our byte-matching EE profile. Do not substitute
another compiler and report it as qualified. Documenting a verified, licensed,
reproducible acquisition/build route is useful work in its own right.

## Legacy SN components

You must supply an authorized copy of the qualified legacy assembler/linker.
We offer no verified public download URL for these exact components and do not
host SDK kits. The current [PlayStation Partners portal](https://partners.playstation.net/)
is a rights-holder information channel, not a promise that a legacy PS2 kit is
available to new applicants. Ask the rights holder about access where necessary.

An existing mirror or GitHub repository alone is not proof of distribution
permission. Component licenses must be checked separately; GNU-derived code
inside a package does not establish permission for the entire SDK. Do not add
unverified SDK downloads, license keys or bypass instructions to this project.
If these prerequisites are unavailable, complete the permitted setup work before
contributing. State the exact missing component and acquisition gap; do not
substitute another route or claim an untested game-code match.

## Let the AI check your setup

Basic diagnostics and tests may be run during preparation:

```powershell
python scripts/doctor.py
python -m unittest discover -s tests
```

Before contribution, check your own ISO and every required tool:

```powershell
python scripts/doctor.py --contributor-check --iso <your-iso> --runtime <private-runtime> --wrench <wrenchbuild> --toolchain <ASM-toolchain-root> --c-toolchain <C-linker-root> --ghidra <ghidra-launcher> --pcsx2 <pcsx2-executable> --bios <your-bios>
```

The C check requires only `ee/bin/ld.exe` from the supplied C toolchain root;
it probes the actual GNU `cpp`/`cc1`/`as` hashes through `wsl_chain.py`. Configure
these variables in the same terminal before launching Python when needed:

```powershell
$env:RAC2_WSL_DISTRO = 'Ubuntu'
$env:RAC2_WSL_TOOLS = '/opt/rac2/ee-tools'
$env:RAC2_WSL_TMP = '/opt/rac2/scratch'
```

These are example locations, not installed files. The historical 32-bit tools
compile in WSL's Linux filesystem; do not compile directly under `/mnt/c` or
`/mnt/d`. Retain short, simple paths and reproducible bare source filenames.
Store your actual paths in ignored `.local/ENVIRONMENT.md`, never public docs.
The strict check returns nonzero for missing prerequisites. File/hash presence
does not establish legal acquisition, R5900 language support or usable emulator
configuration: the AI and contributor must verify those separately. Prepare the
pinned references and pass the full baseline gates before contribution work.

## Source-specific SDK boot owner

The boot links the separately qualified sysbit and CPR8 SDK objects alongside
the unchanged default GNU object. Use the maintained campaign integration route
with `--sdk-binding <private-sdk-binding.json>`, as shown in
[CAMPAIGN-WORKFLOW.md](../docs/CAMPAIGN-WORKFLOW.md). The file is private and has
exactly three fields: `distro`, `workspace_root` and `tool_paths`.

`workspace_root` names a private native Linux directory under `/root/` or `/home/`.
`tool_paths` supplies the seven instrument roles in
[`owned-sdk-b9-single-text-controls-v1.json`](../config/compiler-profiles/owned-sdk-b9-single-text-controls-v1.json):
`driver`, `cc1`, `as`, `strip`, `linker`, `cpp_available` and `cc1plus_available`.
The linker uses its host path; the other instruments use absolute WSL paths.
Their hashes must match the qualified owned tools. Flags and stripping arguments
are fixed by the source-specific unit, rather than configurable in the binding.

The ordinary doctor C probe covers the default GNU chain. It does not supply
or qualify the owned SDK instruments. This additional admission is limited to
the exact standalone source and complete object documented in
[SDK-SYSBIT-EVIDENCE.md](../docs/SDK-SYSBIT-EVIDENCE.md) and
[SDK-CPR8-EVIDENCE.md](../docs/SDK-CPR8-EVIDENCE.md); the existing three-control
foundation remains leaf-only. The complete boot and all 27 overlay gates remain
mandatory, and private tools and runtime bindings are never published.

## Keeping this folder current

Verify upstream ownership, license, applicable version and download provenance
before adding a source. Keep missing or unverified acquisition routes explicit.
Update the qualified proofs after a measured profile change, then this metadata
if the component roles change. Never update hashes simply to accept an unrelated tool.
