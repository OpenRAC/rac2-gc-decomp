#ifndef CORE_DISPLAY_H
#define CORE_DISPLAY_H

#include <stdint.h>

/**
 * Display configuration commands.
 * Direct mapping of "param_1" of FUN_001257d0.
 */
typedef enum {
	DISPLAY_CMD_CONFIGURE = 0,  /* Full setup + screen on  */
	DISPLAY_CMD_DISABLE = 1,  /* Screen off (GS_CSR=0x100) */
	DISPLAY_CMD_RECONFIGURE = 5   /* Update + apply CRT, without clearing the IRQ */
} display_command_t;

/**
 * Configures or controls the video output.
 *
 * Port of FUN_001257d0 (0x001257D0, PS2 ELF).
 *
 * @param command    What to do (DISPLAY_CMD_*)
 * @param mode       Resolution / display mode (u16, PS2: GS_CSR bits + timing)
 * @param timing     Timing parameter (u16, PS2: HSYNC/VSYNC lines)
 * @param interlace  1 = interlaced, 0 = progressive
 *
 * On PS2:
 *   - Escribe REG_GS_CSR (0x100 off, 0x200 on)
 *   - Fills g_GraphicsCanvasData with mode/timing/flags
 *   - Configures the IMR (glColorMask equivalent)
 *   - If it was a reconfigure (cmd=0) and an IRQ was active:
 *     critical_enter → RemoveIntcHandler(2) → clear flags
 *   - SetGsCrt() → applies the timings to the hardware
 *
 * On PC (SDL2):
 *   - DISPLAY_CMD_CONFIGURE  → SDL_SetWindowMode + glViewport + glEnable
 *   - DISPLAY_CMD_DISABLE    → SDL_HideWindow / glClear + no render
 *   - DISPLAY_CMD_RECONFIGURE → SDL_SetWindowMode (resize) without IRQ re-init
 *
 * Thread safety: call only from the render thread.
 */
void core_display_set(display_command_t command,
	uint16_t mode,
	uint16_t timing,
	int      interlace);

#endif /* CORE_DISPLAY_H */
