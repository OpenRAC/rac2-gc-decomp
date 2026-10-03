# PCSX2-MCP validation for RAC2

On 2026-10-03, the pinned USA v1.01 disc and the lot 24 rebuilt boot were launched
in a dedicated portable PCSX2 debugger profile. Both PINE probes identified
`SCUS-97268`, game version `1.01`, on `PCSX2 d75a0ad`. DebugServer and PINE
connections, pause/continue control, EE register responses, memory reads and
native disassembly were exercised. Memory cards were disabled, and the controller
stopped only its own emulator processes after the observations.

The rebuilt run used the same pinned ISO and the `-elf` override. Its ELF SHA-256
matches the complete boot gate. The eight bytes at `FUN_00282C88` agree across the
retail run, rebuilt run, pinned reference and compiler-produced linked image.
Only hashes and structural metadata are published in the
[observation record](../progress/pcsx2/boot-lot24.json); raw memory, emulator profiles,
BIOS files, game images and logs remain private.

The function's breakpoint was installed and listed, but no hit was observed in
this probe. No call/return or memory-write effect is therefore claimed. These
observations qualify the emulator route and loaded code sample; visual gameplay,
level traversal, the full runtime state and the native PC runtime remain unverified.
PINE reports different UUID values for the two ELF routes; use the pinned image
identity and loaded-byte gates for matching claims.

For reproducible observations, start the owned RAC2 profile, connect explicitly
to DebugServer port 21512 and PINE port 28012, verify game identity, then inspect
known EE/IOP addresses. Use breakpoints and verified continuation addresses.
The connector's instruction stepping and step-over remain unqualified. Preserve
existing breakpoints and remove only those created by the experiment. Record
source/ELF/image identities and distinguish an observed result from an unvisited
path. Emulator observations supplement byte matching; they add no C progress.
