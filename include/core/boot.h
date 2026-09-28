#ifndef CORE_BOOT_H
#define CORE_BOOT_H

/**
 * Inicialización completa del display al arrancar.
 *
 * Port de FUN_002848C0 (0x002848C0, PS2 ELF).
 *
 * Secuencia:
 *   1. gpu_init_context()         ← reset GS/VU1/GIF
 *   2. Limpiar flag dev si aplica
 *   3. core_display_set(...)      ← configurar CRT + screen on
 *
 * Llamar UNA VEZ al inicio, después de SDL_Init + GL context.
 * No es re-entrante.
 *
 * XREFs en el ELF:
 *   FUN_00286078 (main init)
 *   FUN_002914E0 (re-init tras save/load)
 */
void boot_display_init(void);

#endif /* CORE_BOOT_H */
