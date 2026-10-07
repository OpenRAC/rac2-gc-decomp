# Shared native family batch: measured evidence

This lot extends the shared-body practise already used by the clear family and
the resident wrappers: one authored body, reviewed per-program placements, and
the existing complete-symbol and full-image gates.

## What was matched

Every body below was recovered from the packed retail instructions, compiled
with the unchanged qualified GNU profile (`-O2 -G0 -ffunction-sections`) and
compared byte for byte against the pinned overlay. A family is accepted only
when one C body reproduces the same measured bytes at **every** placement; the
placements differ only by the definition symbol (and, where a body reads data,
by that program's reviewed data alias).

The families were found by clustering the pinned overlays on a normalizer that
masks only the 26-bit destination of `J`/`JAL`; a family is kept only when its
members are byte-identical, so no masked comparison ever enters the proof.

## Method notes that carry forward

* **Statement order is the scheduler's input.** For a straight-line body of
  constant stores, cc1's first scheduler hoists to the front of the block: the
  last statement, the store of the multi-instruction 64-bit constant, and the
  last store of each floating constant group (in source order), then emits the
  rest in source order and drops the final instruction into the `jr $ra` delay
  slot. Writing the source with those statements moved to the end reproduces the
  retail order.
* **Literal addresses and symbolic names generate different code.** A numeric
  address lets the assembler fold the constant into the destination register
  (`lui $at` reuse), while an `extern` symbol produces a two-register
  `%hi`/`%lo` pair. Which one matches is a measured property of each body.
* **Small data (`$gp`) is out of reach under the pinned `-G0` profile.** The
  retail overlays contain thousands of `$gp` accesses; the qualified profile
  emits the absolute form instead. Those families are parked with their measured
  census rather than approximated.
* **VU0 macro-mode bodies are not expressible in standalone C.** The
  reconstructed `cc1` has no VU0 mode in this toolchain, and inline assembly is
  refused by the campaign's admission rules.

## Limits

* The batch adds C integration bytes only; it claims no original source, object
  boundary, data ownership or runtime behaviour.
* Families whose bodies need small-data addressing, VU0 macro mode or a
  different assembler convention remain parked with their measured refusals.
* Negative results are retained privately with the source, object and diff that
  produced them.
