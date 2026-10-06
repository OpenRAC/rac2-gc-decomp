# Bounded data and GP attribution audit

All 109,725 current catalogue intervals (48,470,076 placement bytes) were checked against their complete pinned retail bodies. This accounts for catalogue intervals only; the separate 231,732-byte EE residual scope is unchanged. No compiler, matching gate or source experiment was run.

The 20 largest multilevel discovery families contain 681 placements and 3,786,240 bytes: 509 flow-supported inferred placements (2,055,392 bytes) and 172 weaker inferred placements (1,730,848 bytes). Discovery deliberately erases immediates and register operands, including scalar values. It supplies candidate pairs only, adds zero matching credit and never feeds accepted normalization. All 681 selected entire intervals passed exact reconstruction; original function boundaries are still separately qualified.

## Ranked discovery families

| Rank | Body bytes | Placements | Placement bytes | First concrete pair | Boundary tier |
| --- | ---: | ---: | ---: | --- | --- |
| 1 | 34,316 | 11 | 377,476 | levels/0_aranos_tutorial@002BA7D8 / levels/10_hrugis_cloud@002D5558 | inferred |
| 2 | 11,348 | 27 | 306,396 | levels/0_aranos_tutorial@00300618 / levels/10_hrugis_cloud@0031B0F0 | flow_supported_inferred |
| 3 | 26,432 | 11 | 290,752 | levels/0_aranos_tutorial@002CB3A8 / levels/10_hrugis_cloud@002E6128 | inferred |
| 4 | 8,440 | 27 | 227,880 | levels/0_aranos_tutorial@003AE900 / levels/10_hrugis_cloud@003C63A8 | flow_supported_inferred |
| 5 | 7,840 | 28 | 219,520 | boot@00298F00 / levels/0_aranos_tutorial@00309FA8 | inferred |
| 6 | 18,632 | 11 | 204,952 | levels/0_aranos_tutorial@002C42E0 / levels/10_hrugis_cloud@002DF060 | inferred |
| 7 | 3,680 | 54 | 198,720 | levels/0_aranos_tutorial@003C6B98 / levels/0_aranos_tutorial@00406150 | flow_supported_inferred |
| 8 | 6,104 | 28 | 170,912 | boot@002A4E68 / levels/0_aranos_tutorial@00317D70 | inferred |
| 9 | 6,048 | 28 | 169,344 | boot@0034B2C8 / levels/0_aranos_tutorial@00440BF8 | flow_supported_inferred |
| 10 | 5,980 | 28 | 167,440 | boot@002E1228 / levels/0_aranos_tutorial@0035AA58 | inferred |
| 11 | 5,868 | 28 | 164,304 | boot@0030A348 / levels/0_aranos_tutorial@00393148 | flow_supported_inferred |
| 12 | 5,760 | 28 | 161,280 | boot@002E9268 / levels/0_aranos_tutorial@00362FF8 | flow_supported_inferred |
| 13 | 5,796 | 27 | 156,492 | levels/0_aranos_tutorial@003FA858 / levels/10_hrugis_cloud@004020F8 | flow_supported_inferred |
| 14 | 5,628 | 27 | 151,956 | levels/0_aranos_tutorial@002A7E60 / levels/10_hrugis_cloud@002C2BE0 | inferred |
| 15 | 5,280 | 28 | 147,840 | boot@00286738 / levels/0_aranos_tutorial@002F1D30 | inferred |
| 16 | 5,060 | 28 | 141,680 | boot@00274D90 / levels/0_aranos_tutorial@002DD048 | flow_supported_inferred |
| 17 | 4,972 | 27 | 134,244 | levels/0_aranos_tutorial@0041CB80 / levels/10_hrugis_cloud@00436EB8 | flow_supported_inferred |
| 18 | 4,788 | 28 | 134,064 | boot@00295F60 / levels/0_aranos_tutorial@003069C8 | flow_supported_inferred |
| 19 | 1,204 | 109 | 131,236 | boot@0032BE30 / levels/0_aranos_tutorial@003A2668 | flow_supported_inferred |
| 20 | 1,324 | 98 | 129,752 | levels/0_aranos_tutorial@003F8280 / levels/0_aranos_tutorial@00415210 | flow_supported_inferred |

Every family retained identical register-operand selectors across its selected members. Register allocation differences therefore contribute zero observed differing-word bytes in this bounded sample; this says nothing about unselected families. Complete member pins and pair receipts are retained in the structural JSON bank.

## Overlapping verification obligations

Each row counts the full body once for every applicable unresolved category. These figures overlap and must not be summed or presented as a causal decomposition of the unique-code denominator.

| Obligation | Overlapping placement bytes |
| --- | ---: |
| unmapped_memory_target | 3,190,616 |
| scalar_or_escaped_value | 3,786,240 |
| callee_argument_or_target | 2,575,940 |
| CFG_or_decoder_unresolved | 2,970,172 |
| unowned_data_template_retained_by_primary_policy | 2,435,512 |
| unproved_GP | 2,402,004 |
| bare_LUI_missing_exact_data_symbol | 1,828,640 |

The actual pair comparisons contain 379,720 differing-word bytes. Field categories count complete four-byte words, with overlap allowed:

| Observed difference | Differing-word bytes |
| --- | ---: |
| LUI_HI16_value | 22,704 |
| LO_address_or_scalar_immediate | 52,792 |
| external_static_control_target | 241,108 |
| memory_displacement_or_object_addend | 29,832 |
| GP_memory_displacement | 28,588 |
| internal_control_rebase_only | 4,696 |

The 4,696 internal-control rebase bytes preserve the same internal destination offsets. They are distinct from the 241,108 external static-target bytes, whose callee identity requires the separate graph/target proof. The 52,792 LO-or-scalar bytes remain explicitly ambiguous.

## Bounded address and GP witnesses

- Aranos 00300618 / Hrugis 0031B0F0 (11,348 bytes): both entire 2,837-word bodies are reachable in the current conservative CFG. At LUI origin +4, consumers +68 and +80 retain addends +12 and +72 while effective targets move by +2,240. This is an encoded address relationship, not an original global-object binding.
- Aranos 003AE900 / Hrugis 003C63A8 (8,440 bytes): both entire 2,110-word bodies are reachable. A bare-high origin +1,100 consumed at +1,104 has low addends +6,600 versus +8,264. Masking that low field without ownership evidence would erase an unqualified object-relative distinction. Forty and twenty-one bounded cross-program address-use relationships are retained for these two families.
- Boot 00274D90 / Aranos 002DD048 (5,060 bytes), replicated across all 28 programs: the actual R5900 decoder verifies 25 GP writes per body, no GPR memory store of GP and no calls. This remains a concrete scratch-GP counterexample to using one fixed GP value throughout every body.
- Boot 002A4D88 (224 bytes): GP is stored at +48 and loaded at +200 with the same r1/+64 fields; two calls intervene at +80 and +96. The structural global-slot restore requires callee effects, alias/lifetime and caller-entry provenance before it can become a preservation theorem.
- All 58 pinned GP-write owner bodies were independently decoded again: 731 actual GPR GP writes agree with the frozen evidence. No new entry GP value or call-preservation theorem was verified. Startup 001AEFF0 remains initialization-local.
- All 28 original reference ELFs contain zero symbol tables and zero sized STT_OBJECT/STT_FUNC records. No original object allocation metadata was recovered. The 110 current synthetic C binding views remain excluded from original allocation claims.
- Current pinned negative controls were replayed: packet routines Aranos 002F08D0 and 002F0930 retain scalar 0x26/0x29 at +12 and +40 and stay distinct. GS Aranos 002E5890 and Oozla 002DAFD8 retain the 0x003FF000 high/low literal fields at +24/+32 exactly.

## Proof still required

The largest dispatch extents need the independent boundary/jump-table investigation; writable table bounds do not establish original data-object ownership. Supported address families need allocation/base provenance and preserved alias/addend relationships. GP readers need scoped entry value, actual callee preservation and restore-slot lifetime proofs. Scalar payload values, packet constants and GS address arithmetic remain raw. No broad masking, guessed ownership, C identity or matching credit follows from this audit.

## Reproduction and pins

Run data_gp_audit.py with --repo, --catalog, --boundaries, --references, --pointer-evidence, --gp-evidence and a fresh --output. Then run inspect_role_witnesses.py with the same pinned inputs and the first report as --audit, using the maintained R5900 Rabbitizer runtime. Both reports contain structural metadata only; private reference files remain outside version control. Eight focused discovery/classification regressions pass.

Audit SHA-256: `290cdd4ac21ba9e2b01de07186fd992323e6cb092f3fd7f76380346a9e580984`.
Witness SHA-256: `428916909b3b5083c81cd88757385f3d43db6f2654992bf464810587579e6385`.
Normalizer SHA-256: `0b0fc7cd33595440549ad82b650c6a99c6843dad2752d4af32673db22b3dda86`.
