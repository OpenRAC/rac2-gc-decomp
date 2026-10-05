# English

# Memory Card Subsystem Documentation - Ratchet & Clank 2 (PAL)

This document compiles the text strings and calls to the Sony SDK (`libmc`) used to manage game progress.

## File Format Templates (Memory Card Path Template)

| Memory Address | Data Type | Static String Value | Purpose / Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001A9AA2`** | `char[]` (String) | `“BESCES-50916RATCHET/save%d.bin”` | Format mask used by vsnprintf to define the path of the game’s binary file in the slot (`mc0:` / `mc1:`). |

# Español

# Documentación del Subsistema de Memory Card - Ratchet & Clank 2 (PAL)

Este documento centraliza las cadenas de texto y las llamadas al SDK de Sony (`libmc`) utilizadas para gestionar el progreso de la partida.

## Plantillas de Formato de Archivo (Memory Card Path Template)

| Dirección de Memoria | Tipo de Dato | Valor de Cadena Estática | Propósito / Uso en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001A9AA2`** | `char[]` (String) | `"BESCES-50916RATCHET/save%d.bin"` | Máscara de formato utilizada por vsnprintf para definir la ruta del archivo binario de la partida en el slot (`mc0:` / `mc1:`). |

