# English

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

## Falta **0x001A9138** 

# Español

# Subsistema Gráfico y Control de Cuadros (GS / V-Sync Sync)

Este documento registra los puntos de control del Sintetizador Gráfico (GS) y la estrategia para desligar el motor lúdico del límite físico original de la consola.

## Logs de Integridad del Bus DMA Gráfico (VIF/GIF Channels)

| Dirección de Memoria | Tipo de Dato | Valor de Cadena Estática | Propósito / Uso en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x0013B563`** | `char[]` | `"sceGsExecLoadImage: DMA Ch.2 does not terminate...\r\n"` | Error crítico cuando el canal DMA 2 (GIF) se congela al transferir texturas a la VRAM. |
| **`0x0013B5D1`** | `char[]` | `"sceGsExecStoreImage: GS does not terminate...\r\n"` | Error crítico cuando el pipeline gráfico colapsa al intentar volcar el buffer de dibujado. |

## Estrategia de FPS Dinámicos para el Port de PC
* **Lazo Lúdico (Engine Tick):** Fijado lógicamente a pasos fijos equivalentes a la tasa original (16.66ms para emular 60Hz nativos).
* **Lazo Visual (Render Frame Rate):** Desacoplado mediante interpolación lineal de variables lúdicas (`DeltaTime` en PC), permitiendo tasas de refresco escalables (60Hz, 144Hz, Uncapped) configurables por el usuario.


| Dirección de Memoria | Tipo de Dato | Valor de Cadena Estática | Propósito / Uso en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001A9138`** | `char[]` | `"Unsupported language\n"` | Mensaje de pánico impreso por el selector si el índice devuelto por la BIOS no coincide con los assets del disco. |

