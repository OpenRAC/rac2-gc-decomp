# Game regions

The matching target remains USA v1.01, `SCUS_972.68`. The European PAL release,
`SCES_516.07`, is registered as a second region so the preparation and
reconstruction tools can measure and round-trip it. It has no C catalogues,
reviews or progress proofs, and it adds nothing to the README progress.

| Region | Serial | Identity files | State |
| --- | --- | --- | --- |
| `ntsc-u` (aliases `ntsc`, `usa`; default) | `SCUS_972.68` | [target](../config/target.json), [overlays](../config/overlays.json) | pinned; matching proofs |
| `pal` (alias `europe`) | `SCES_516.07` | [target](../config/regions/pal/target.json); overlays not yet recorded | unmeasured; no catalogues |

[`config/regions.json`](../config/regions.json) is the registry and
[`scripts/region.py`](../scripts/region.py) resolves it. A region is selected by
`--region`, then the `RAC2_REGION` environment variable, then the registry default.
A manifest or catalogue names its serial and `region.by_serial` resolves the
region that owns it. The USA identity files keep their original paths and bytes,
so no existing proof changes.

A region is *pinned* when its disc, boot and every overlay SHA-256 and its level
count are recorded. Only one region is *matching*: the one whose catalogues and
proofs this repository publishes.

## Tool behaviour

| Tool | Region behaviour |
| --- | --- |
| `setup.py` | `--region`; verifies every pinned identity. `--measure-identity` records unpinned ones in a private `identity-proposal.json` |
| `doctor.py` | `--region`; reports pinned or unmeasured identity and whether C catalogues exist; the next command names the region |
| `build.py` | Resolves the region from the manifest serial. A pinned region requires its pinned boot and overlays. An unpinned region accepts only a measurement manifest and runs the assembly round trip; C integration requires the matching region |
| `campaign_build.py` | Full C campaigns are refused for a non-matching region; the overlay count comes from the target |
| `campaign.py` | Trial references are checked against the pins of the region named by each catalogue |
| `ApplyMobyNames.java` | Refuses an overlay whose SHA-256 is not the pinned USA overlay, because its dispatch addresses are USA addresses |
| `ApplyAssertMessageNames.java` | Unchanged; its `.text` fingerprints already refuse every other build |
| `elf_tools.py`, `expand_asm.py`, `wsl_chain.py`, compiler patches | Region-neutral |
| `check_candidates.py`, `check_level_candidates.py`, `level_native.py`, `integration.py`, `source_layout.py` | Unchanged and USA-only. `level_native.checker_hash` binds the first group into all 27 native reviews and `source-layout.json` binds the generator; editing them requires regenerating those reviews with the private compiler |
| `decomp_report.py`, `readme_progress.py` | Unchanged; they publish the matching region's proofs |

## Measuring and pinning PAL

1. Use your own legally acquired PAL disc and the normal tool suite:

   ```powershell
   python scripts/doctor.py --region pal --iso <pal.iso> --runtime <runtime>
   python scripts/setup.py --region pal --measure-identity --iso <pal.iso> --runtime <runtime> --wrench <wrenchbuild.exe>
   python scripts/build.py --manifest <runtime>\runs\<id>\manifest.json --toolchain <ProDG-2.0> --all-levels
   ```

   The manifest is marked `"pinned": false`. A successful build is a round trip
   against an unreviewed reference, not a match and not a proof.
2. Compare the ISO hashes in `identity-proposal.json` with a public dump catalogue
   such as Redump. Do not pin a dump that disagrees with the catalogue.
3. Record the reviewed values in `config/regions/pal/target.json`, add
   `config/regions/pal/overlays.json` with its `target` and `levels`, and set
   `overlays` in the registry. Rerun setup without `--measure-identity`; it must
   pass every pinned comparison.

PAL addresses differ from USA addresses. Catalogues, Ghidra annotations and the
dispatch and diagnostic tables must be re-derived for PAL; none may be copied by
address. Making PAL a second matching region also requires region-aware proof
paths and regenerated reviews for the checker group above.

## The functional PAL port

[`ports/pal-functional/`](../ports/pal-functional/) holds the imported history of
[platypet2217-star/RAC2Decomp](https://github.com/platypet2217-star/RAC2Decomp): a
functional, non-matching reconstruction of `SCES_516.07` and a native-port
skeleton under its original MIT notice. Its `RAC2_REGION` CMake option selects
NTSC or PAL boot file, frame rate and display height (see its README). The
function addresses it quotes are PAL addresses. Its RAM-address macros are
PAL-only until a PAL-to-USA address map exists.
