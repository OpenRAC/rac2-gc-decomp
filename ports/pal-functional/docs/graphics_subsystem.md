# Graphics Subsystem and Frame Control (GS / V-Sync)

This document documents the control points of the Graphics Synthesizer (GS) and the strategy for decoupling the game engine from the console's original physical limits.

## Graphics DMA Bus Integrity Logs (VIF/GIF Channels)

| Memory Address | Data Type | Static String Value | Purpose / Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x0013B563`** | `char[]` | `“sceGsExecLoadImage: DMA Ch.2 does not terminate...\r\n”` | Critical error when DMA channel 2 (GIF) freezes while transferring textures to VRAM. |
| **`0x0013B5D1`** | `char[]` | `“sceGsExecStoreImage: GS does not terminate...\r\n”` | Critical error when the graphics pipeline crashes while attempting to flush the draw buffer. |

## Dynamic FPS Strategy for the PC Port
* **Game Loop (Engine Tick):** Logically fixed to fixed intervals equivalent to the original rate (16.66 ms to emulate native 60 Hz).
* **Visual Loop (Render Frame Rate):** Decoupled via linear interpolation of gameplay variables (`DeltaTime` on PC), allowing for user-configurable scalable refresh rates (60 Hz, 144 Hz, Uncapped).

## Language Selector Diagnostics

| Memory Address | Data Type | Static String Value | Purpose / Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001A9138`** | `char[]` | `"Unsupported language\n"` | Panic message printed by the selector if the index returned by the BIOS does not match the assets on the disc. |
