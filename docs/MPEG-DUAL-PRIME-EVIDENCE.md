# MPEG dual-prime motion vectors

The boot function `FUN_0012B3E0` computes MPEG dual-prime motion vectors from
picture structure, field order, two displacement corrections and signed motion
components. Its complete inferred extent is 388 bytes. The source is mechanically
adapted from Lombyte's `_dualPrimeVector`; it is attributed third-party C rather
than a newly recovered original translation unit.

## Source provenance

- [Pinned Lombyte source](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/src/sdk/library/dual_prime_vector.c).
- [Pinned MIT licence](https://github.com/mateuszklysz/Lombyte/blob/2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61/LICENSE), copyright 2026 Mateusz Kłysz.
- Source SHA-256: `e6df19f23bb23e15cf030b7d3c5222fae6d1fc4ba4411cbff4a03f31fc014b51`.
- Git source blob: `ef8a62a5677152d818667ae3e37f98cba3022ac5`; last source change
  `f18ea57e965bf7e88630871cc3b230b05b07c587`.

The adaptation removes the include supplying already-identical `u8`/`s32`
declarations and renames the function to its measured RAC2 symbol. The structure,
parameter types, arithmetic, expressions, statement order and complete MIT notice
are preserved. The accepted boot source prefix is retained byte for byte.

## Independent evidence and acceptance

The actual compiled RAC1 source object, RAC1 original body and RAC2 original body
have the same full 388-byte machine-code hash:
`cbd826f112e4e0f877738933fb36cb67db882ce5f8cc497b759686bc476cc949`.
That identity established a source hypothesis. The donor used its SDK compiler,
which differs from the qualified RAC2 compiler; donor object equality alone did
not establish RAC2 C acceptance.

The first RAC2 trial, `d1f7858eb1be4dadbd9443c9b9a36240`, compiled the unchanged
adapted body together with all 184 current boot controls under the qualified
`-O2 -G0 -ffunction-sections` profile. All 185 complete functions matched, with
zero different bytes for the new body. The actual object hash is
`afafc4632bd5563778da7ce814ef7f73cf661c7ed651e5a2cb285ff0f479fc3a`.
Object extents, linked symbol sizes and full raw bodies were independently
replayed. There are no readonly sections for this candidate.

Both call sites in the complete 1,796-byte caller at `0x001296A0` support five
ordinary arguments: decoder pointer in A0, a 16-byte output array in A1,
displacement pair in A2 and signed motion components in A3/T0. The caller ignores
the return value. Fields at decoder offsets `0x174` and `0x178` agree with the
preserved structure. No GP, helper call, stack frame, floating-point operation or
VU primitive is required by the leaf function.

The verified global catalogue contains only the boot extent with this exact
388-byte body. No overlay copy is credited. Complete original object boundaries
remain inferred; source organization does not establish an original module.
Trial equality adds no integrated credit on its own: current source reviews and
the boot plus 27 overlay full-image proofs govern publication and progress.
