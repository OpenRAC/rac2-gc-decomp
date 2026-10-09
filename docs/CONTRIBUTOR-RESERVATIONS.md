# Shared function reservations

Contributors on separate forks and the owner-operated Qwen runner reserve work
through the same upstream GitHub ledger. Local locks do not coordinate machines.
`config/campaign-register.json` remains the authority for experiments, refusals,
reopening and proofs. Reservations and the [nonmatching shelf](../nonmatching/README.md)
add **zero matching credit**.

## Claim a small lot

Complete [setup](CONTRIBUTOR-QUICKSTART.md) first. Read the current campaign queue,
packet and previous refusals. Use a retained attempt when relevant; reservation
does not authorize reopening a stopped experiment without its required evidence.

```sh
gh auth status
python scripts/reservations.py list
python scripts/reservations.py describe --reference-sha256 <pinned-reference-sha256> --address <0xADDRESS> --size <complete-size>
python scripts/reservations.py claim --targets <targets.json> --repository <you/your-fork> --branch <your-topic-branch>
```

Put up to five returned target objects in a private JSON list for `--targets`.
The CLI always coordinates with `OpenRAC/rac2-gc-decomp`, even from your fork.
It creates a `[Reservation]` issue and waits for the shared ledger to acknowledge
the exact command. Wait for its claim ID before starting; an open issue or a
posted command alone is not a reservation. No per-function maintainer approval
is needed for ordinary contributors managing their own work.

Admission is atomic: if any target is taken, none of the new lot is allocated.
Identity includes game version, executable/overlay, reference SHA, address and
size. Overlapping aliases in one executable conflict; equal addresses in
different overlays do not. Unknown references, ambiguous/unsupported extents and
already integrated C placements are refused. A flow-supported extent is still
inferred and does not establish its original signature.

At most five explicit functions may be claimed per lot and actively reserved
per contributor. Completed handoffs and blocked work retain their exclusion
locks without consuming active dispatch slots. Expired `reserved` claims still
consume their slots until inspected. `--kind family` locks the trusted
structural family of the listed members, including its other placements; all
listed members must share that family ID. This grouping proves no original object
or ABI equivalence; each placement still requires its actual matching proofs.

## Check, hand off and release

```sh
python scripts/reservations.py check --claim <claim-id> --targets <targets.json>
python scripts/reservations.py renew --claim <claim-id>
python scripts/reservations.py review --claim <claim-id>
python scripts/reservations.py block --claim <claim-id>
python scripts/reservations.py release --claim <claim-id>
```

| State | Meaning |
| --- | --- |
| `reserved` | Owner may work while the lease is valid |
| `in_review` | Retained result awaits review; reservation stays held |
| `blocked` | Work stopped for inspection; reservation stays held |
| `released` | Work stopped and the reservation was explicitly returned |
| `integrated` | Maintainer verified a merged upstream PR and current exact C proofs |
| `needs_attention` | Open lease expired; reservation still blocks others |

Leases default to 48 hours. Expiration never silently reassigns work that might
still be running on a disconnected machine. Stop before releasing. Maintainers
override another owner only after checking that their work stopped. Closing an
issue alone does not release it. Missing acknowledgement, changed ownership,
expiry or unavailable coordination stop compiler admission.

Only current upstream `maintain`/`admin` roles can override another owner or run:

```sh
python scripts/reservations.py integrated --claim <claim-id> --pr <merged-pr-number>
```

Integration status in this ledger does not modify campaign proofs. Issues show
owners, commands and `reservation:*` labels. Those views can lag under concurrency;
`list` and `check` read the authoritative ledger. The authenticated GitHub author
determines ownership, never an owner name supplied in command text.

## Qwen and maintainer setup

New autonomous runs require a fixed owner-configured Python/CLI and hashes of
all three reservation modules. Qwen claims one function at a time, checks before
dispatch and before compilation, and skips confirmed taken functions. Uncertain
operations retain their claims for inspection. Exact private results enter review;
partial results may be released after evidence and configured publication are
retained. Matching/tool CI is required before integration, separate from the
short coordination workflow. Tool installation does not start or resume Qwen.
Historical stopped journals and their configuration are preserved.

After the reviewed workflow reaches default `RAC2`, a maintainer initializes:

```sh
python scripts/reservations.py init
```

This creates labels and a parentless `coordination/reservations` branch containing
`reservations.json`, without changing game-source history. Writes use a SHA
compare-and-swap and reevaluate conflicts. Immutable comment IDs make retries
idempotent. No global Actions concurrency group discards pending requests.
Only trusted default-branch code executes; contributor branches and comment
text are never executed. Reservation processing invokes no compiler or model.
Ledger size/record bounds require reviewed maintenance when reached, preserving
history. Private owner bridge configuration is documented in Cockpit's
`collaboration/RATCHET-AUTONOMY.md`.

Organization draws on [RAC1's nonmatching shelf](https://github.com/OpenRAC/rac1-decomp/blob/6ded75da99da1d7dec69bf76af7e46dc24b3c14c/docs/NONMATCHING.md).
Its [claims tool](https://github.com/OpenRAC/rac1-decomp/blob/6ded75da99da1d7dec69bf76af7e46dc24b3c14c/tools/claims.py)
serves a shared checkout; our ledger serves independent forks. RAC2 retains its
own source layout and exact-C acceptance criteria.
