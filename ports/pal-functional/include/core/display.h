#ifndef CORE_DISPLAY_H
#define CORE_DISPLAY_H

#include <stdint.h>

/**
 * Comandos de configuración de display.
 * Mapeo directo del "param_1" de FUN_001257d0.
 */
typedef enum {
	DISPLAY_CMD_CONFIGURE = 0,  /* Full setup + screen on  */
	DISPLAY_CMD_DISABLE = 1,  /* Screen off (GS_CSR=0x100) */
	DISPLAY_CMD_RECONFIGURE = 5   /* Update + apply CRT, sin limpiar IRQ */
} display_command_t;

/**
 * Configura o controla el output de video.
 *
 * Port de FUN_001257d0 (0x001257D0, PS2 ELF).
 *
 * @param command    Qué hacer (DISPLAY_CMD_*)
 * @param mode       Resolución / display mode (u16, PS2: bits de GS_CSR + timing)
 * @param timing     Parámetro de timing (u16, PS2: HSYNC/VSYNC lines)
 * @param interlace  1 = interlaced, 0 = progressive
 *
 * En PS2:
 *   - Escribe REG_GS_CSR (0x100 off, 0x200 on)
 *   - Llena g_GraphicsCanvasData con mode/timing/flags
 *   - Configura IMR (glColorMask equivalente)
 *   - Si era un reconfigure (cmd=0) y había IRQ activo:
 *     critical_enter → RemoveIntcHandler(2) → clear flags
 *   - SetGsCrt() → aplica los timings al hardware
 *
 * En PC (SDL2):
 *   - DISPLAY_CMD_CONFIGURE  → SDL_SetWindowMode + glViewport + glEnable
 *   - DISPLAY_CMD_DISABLE    → SDL_HideWindow / glClear + no render
 *   - DISPLAY_CMD_RECONFIGURE → SDL_SetWindowMode (resize) sin re-init de IRQ
 *
 * Thread-safety: llamar solo desde el thread de render.
 */
void core_display_set(display_command_t command,
	uint16_t mode,
	uint16_t timing,
	int      interlace);

#endif /* CORE_DISPLAY_H */
