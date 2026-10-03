# Native C for levels

This support adds an overlay-specific source, catalogue and review alongside
the boot C bodies already placed in that overlay. It preserves the complete-body
checks and the final check of every PT_LOAD.
No game ELF, object or byte may be committed.

For an identifier from `config/overlays.json`, the paths are:

| Artifact | Path |
| --- | --- |
| Hand-written source | `candidates/levels/<id>.c` |
| Boundary and identity catalogue | `config/level-native/<id>.json` |
| Body and object review | `progress/level-candidates/<id>.json` |

`scripts/level_native.py` is the shared API. Symbols use
`LVL_<UPPERCASE_ID>_FUN_<8_HEX_ADDRESS>`. The catalogue contains schema 1,
kind `level-native-catalog`, target, level, program `levels/<id>`, reference_sha256
(the overlay pin), entry (ELF entry point), canonical source, flags, functions
(symbol/address/size/meaning), externals and optionally gp. Sizes and addresses
are integers divisible by four; bodies must not overlap.
The `24_ship_shack` catalogue provides a real example, with no address hard-coded in the API.

Qualification requires an ET_EXEC ELF with the correct entry point and exact pin.
Each complete body must lie within an executable PROGBITS section.
It compiles in a private runtime, checks freshness, links the selected sections,
checks the complete symbol size and compares every byte.
The review records hashes of the source, catalogue, checker, object,
candidate ELF, reference ELF and bodies, as well as cpp/cc1/as/ld and the flags.
Body hashes are distinct from ELF file hashes.

Run qualification from the repository root, using the appropriate private paths:

```powershell
python -B scripts/check_level_candidates.py --level 24_ship_shack `
  --reference <private-overlay.elf> --toolchain <C-toolchain> `
  --runtime <runtime-outside-repository> --write-review
```

Reconstruction with `build.py` keeps the existing options. `--c-level <id>`
selects that level's shared and native C when its catalogue exists.
`--all-levels --c-all-levels --c-toolchain <C-toolchain>` selects them everywhere.
`--toolchain` identifies the ASM reconstruction toolchain; `--c-toolchain`
identifies the C toolchain. These two instruments must remain distinct.
A requalified boot review can be supplied explicitly through
`--candidate-review <json>` without replacing the previous provenance.

Integration requalifies the native object and requires the same hash as the
reviewed object. Shared bodies retain their `candidates/boot.c` source, review
and checks. Each linker fragment selects its own object; no placement correction
is added. The union rejects overlaps and conflicting externals.
The schema 2 proof, kind `level-c-integration`, stores the union's functions
only once, both qualifications, the reconstruction instrument hashes
and the complete loaded-byte gate.

`decomp_report.py` accepts `--level-proof <integration.json>` (repeatable),
`--candidate-review`, `--integration-proof` and `--progress-proof`. It validates
each proof against its program, source, review, object and gate, then derives
the counters. Native units have their own `sourcePath`.
A boot proof, partial body or smoke check earns no credit.

A level's dependency key contains its three native files, shared placement
entry, selected boot source/review and checker. Changing another level's C
does not invalidate it. No global index of native proofs is required;
supply only the proofs that are still valid to the exporter.
Changing the shared boot source invalidates the levels that reuse it.

Current limitations: native sources are standalone; includes and date/time
macros are rejected until header dependencies are pinned. Mixed integration
still requires a valid, nonempty shared catalogue.
The initial demonstration covered one native body of 24 bytes in one overlay.
[Lot 18](EIGHTEENTH-C-LOT.md) now validates the boot and all 27 overlays,
with two native bodies totaling 104 bytes in `24_ship_shack`.
The 10 % objective remains open. Publishable JSON reviews contain only
identities and measurements, not private objects.
