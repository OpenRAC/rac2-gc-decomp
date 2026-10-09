# ThorpenCodes: five boot function reservations

This document records an acknowledged reservation request. It is a receipt,
not an alternative task queue or ownership ledger. Current ownership comes
from the shared upstream ledger through `scripts/reservations.py list` and
`check`; campaign decisions remain in `config/campaign-register.json`.

- Upstream base: `09024f4b5577f6676b785c797b1c56f402b42fee` (`RAC2`).
- Owner: `ThorpenCodes`.
- Fork: `ThorpenCodes/rac2-gc-decomp`.
- Branch: `reserve/boot-five-functions-20261009`.
- Reservation issue: <https://github.com/OpenRAC/rac2-gc-decomp/issues/65>.
- Claim: `e2f541e73b3cf391839c01054d219294f8d2ab4868b914532324871bcc87d80a`.
- Acknowledged command: `6073138256`; ledger revision: `4`; state: `reserved`.
- Lease deadline from acknowledgement: Unix timestamp `1791686379`.
- Target: Going Commando USA v1.01, `SCUS_972.68`, program `boot`.
- Pinned executable SHA-256:
  `36d5814d8d95328d5839612ccdf7a2e7ecac0b6f868d3ad4bb2e98411f734b4a`.
- Claim kind: `functions`; only these five explicit extents are reserved.

| Symbol | Address | Complete size |
| --- | --- | --- |
| `FUN_00282C10` | `0x00282C10` | 32 bytes |
| `FUN_00282D90` | `0x00282D90` | 32 bytes |
| `FUN_002833F0` | `0x002833F0` | 32 bytes |
| `FUN_00283410` | `0x00283410` | 32 bytes |
| `FUN_00283658` | `0x00283658` | 32 bytes |

## Selection and verification

The current campaign status and queue were inspected. Each selected boot
extent is `flow_supported_inferred` in the pinned function catalogue and does
not overlap current integrated C. A conservative search of the complete
campaign register for each decimal address, eight-digit hexadecimal address
and structural family ID found no prior task or refusal referring to these
targets. This is an identifier search, not proof that no historical alias exists.
The generated nonmatching shelf contained no published attempts.

The maintained upstream `describe` command resolved each complete target.
The atomic five-target `claim` command then returned `ok: true` and
`state: reserved` for the exact membership above. The reservation tests passed:
`python -m unittest discover -s tests -p test_reservation*.py -v`
ran 62 tests, all successful.

## Scope and next steps

This lot stops at reservation. No game C, catalogue, campaign decisions or
matching proofs are changed, and no matching credit is claimed. Function
boundaries remain inferred and original signatures remain unverified.

Complete setup qualification, Ghidra analysis access, PCSX2 usability and a
baseline for the then-current source must be verified before decompilation.
Those checks were not rerun for this reservation-only request. Before any
future compiler trial, check the exact claim membership and current lease;
renew through the shared CLI when required. An expired lease continues to
exclude other contributors until inspected, but does not authorize work.
The draft PR records this receipt; opening or merging it does not establish
ownership independently of the shared ledger.
