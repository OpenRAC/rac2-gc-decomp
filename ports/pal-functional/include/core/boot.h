#ifndef CORE_BOOT_H
#define CORE_BOOT_H

/**
 * Complete display initialization at startup.
 *
 * Port of FUN_002848C0 (0x002848C0, PS2 ELF).
 *
 * Sequence:
 *   1. gpu_init_context()         ← reset GS/VU1/GIF
 *   2. Clear the dev flag if applicable
 *   3. core_display_set(...)      ← configure the CRT + screen on
 *
 * Call ONCE at startup, after SDL_Init + GL context.
 * Not re-entrant.
 *
 * XREFs in the ELF:
 *   FUN_00286078 (main init)
 *   FUN_002914E0 (re-init tras save/load)
 */
void boot_display_init(void);

#endif /* CORE_BOOT_H */
