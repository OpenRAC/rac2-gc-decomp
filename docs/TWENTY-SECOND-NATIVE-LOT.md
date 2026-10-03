# Twenty-second RAC2 native C lot: resident selection helper

Add the 72-byte body `LVL_24_SHIP_SHACK_FUN_002D1570` to the 24th level
overlay. It selects a table entry only when the selection is non-negative and
the entry index is below the stored limit. The authored C source compares
byte-for-byte with the complete function in the pinned USA v1.01 overlay.

The function reads the resident boot `.bss` object at `0x00189E20`. Ghidra
memory and section ownership identify that address in the boot program; the
level overlay has no section covering it. The catalogue therefore records a
reference to the resident object, not a copied data definition. The C view only
covers the observed fields: `selected` at `+0x0C40`, `index` at `+0x0C44`, and
the object pointer at `+0x2290`. The complete historical type and runtime
values remain unknown.

| Measure | Before | After |
| --- | ---: | ---: |
| Boot functions / bytes | 177 / 9,328 | 177 / 9,328 |
| Overlay placements / bytes | 4,129 / 204,648 | 4,130 / 204,720 |
| Native functions / bytes | 56 / 2,752 | 57 / 2,824 |
| Integrated C bytes | 213,976 | 214,048 |
| Executable-code proportion | 0.4386% | 0.4387% |

Validation: the native source and catalogue pass the complete-symbol gate under
the pinned `8bed6eae` C profile. The build matches all loaded bytes and metadata
in the reconstructed boot and the `24_ship_shack` overlay: 2,521,763 and
2,616,168 bytes, respectively. The other 26 overlay sources and their proofs
were unchanged. Native gameplay and the complete resident-object layout remain
unverified.
