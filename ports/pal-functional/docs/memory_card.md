# Memory Card Subsystem Documentation - Ratchet & Clank 2 (PAL)

This document compiles the text strings and calls to the Sony SDK (`libmc`) used to manage game progress.

## File Format Templates (Memory Card Path Template)

| Memory Address | Data Type | Static String Value | Purpose / Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001A9AA2`** | `char[]` (String) | `“BESCES-50916RATCHET/save%d.bin”` | Format mask used by vsnprintf to define the path of the game’s binary file in the slot (`mc0:` / `mc1:`). |
