# Twenty-third RAC2 native C lot: ship-shack bank selection

Add the 72-byte level-only body `LVL_24_SHIP_SHACK_FUN_0035E348` to the
`24_ship_shack` overlay. When the resident context is in state 9, it updates the
context's current pointer to the level's bank table and copies the selected
bank field to `+0x68`. The table and resident structure are accessed through
partial C views of fields observed in the pinned level ELF; their complete
historical types and runtime values are not recovered.

Data ownership was resolved by program and section. The context at
`0x001BC3C0` belongs to the level overlay's `.bss`; the table at `0x0028AAC0`
belongs to its `.data`. Boot places the same numeric table address in `.text`,
so the level data is identified by its own program and section. No table bytes
or initialized values are copied into the C source.

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 177 / 9,328 | 177 / 9,328 |
| Overlay placements / bytes | 4,130 / 204,720 | 4,131 / 204,792 |
| Native functions / bytes | 57 / 2,824 | 58 / 2,896 |
| Integrated C bytes | 214,048 | 214,120 |
| Executable-code proportion | 0.4387% | 0.4389% |

Validation: all six `24_ship_shack` native bodies match their complete pinned
function boundaries under the current `8bed6eae` C profile. The rebuilt boot
matches 2,521,763 loaded bytes, and the full overlay matches all 2,616,168
loaded bytes and metadata. The other 26 overlay sources and proofs are
unchanged. Native gameplay remains unverified.
