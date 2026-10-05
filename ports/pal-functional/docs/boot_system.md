# Boot System Initialization Register - Ratchet & Clank 2 (PAL)

This document lists the addresses of global variables initialized by the graphics engine core during the binary's startup.

## Static Memory Initialization Segment (0x001A6464 - 0x001A64B4)

| Memory Address | Data Type | Initialization Value | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001A6464`** | `uint32_t` | `0xFFFFFFFF` | Main control loop state mask or ID of the active game thread. |
| **`0x001A6468`** | `uint32_t` | `0x20` (32) | Control factor or frame rate per second (PAL refresh rate alignment). |
| **`0x001A646C`** | `uint32_t` | `0` | Global pointer to the intro and menu flow state manager. |
| **`0x001A6470`** | `uint32_t` | `0` | Planet dynamic data loading control flag. |
| **`0x001A64B4`** | `uint32_t` | `0` | Base pointer for the global economy and inventory manager. |
