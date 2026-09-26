#include "core/vsync.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

/* ---- singleton interno ---- */

static graphics_canvas_data_t g_canvas_data = {
	.field_mode = 1,   /* por default sí reportamos paridad */
	.vsync_type = 0,   /* modo blocking (SDL vsync on) */
	.display_mode = 0x0500,  /* 640×448 NTSC (default R&C2) */
	.timing_param = 0x0100,  /* 60Hz progressive */
	.flag_interlace = 0,
	.csr_high_byte = 0,
	.irq_active = 0,
	.intc_handler_data = 0,
};

static uint32_t g_frame_parity = 0;  /* alterna 0/1 cada swap */

const graphics_canvas_data_t* graphics_canvas_data(void)
{
	return &g_canvas_data;
}

/*
 * ------------------------------------------------------------------
 *  PORT de FUN_001261f0 (0x001261F0, PS2 ELF)
 * ------------------------------------------------------------------
 *  Original:
 *    - if vsync_type == 0 → WaitForInterruptStatus() + read GS_CSR bit13
 *    - if vsync_type != 0 → vsync_wait_first() >> 13 & 1
 *    - return field bit si field_mode == 1, else return 1
 *
 *  PC:
 *    - SDL_GL_SwapWindow() ya sincroniza con el VBLANK del driver
 *    - la "paridad" la generamos nosotros con un contador
 *    - el modo poll vs interrupt se mapea a:
 *        vsync_type == 0 → SDL_SetHint(VIDEODRIVER_VSYNC, "1")  (blocking)
 *        vsync_type != 0 → SDL_SetHint(VIDEODRIVER_VSYNC, "0") + spin (raro, pero fiel)
 * ------------------------------------------------------------------
 */
vsync_result_t vsync_wait(void)
{
	const graphics_canvas_data_t* canvas = graphics_canvas_data();
	vsync_result_t result = 1;  /* default: "no field info" → 1 (igual que el original) */

	if (canvas->field_mode == 1) {
		result = g_frame_parity;  /* 0 o 1, alternado */
	}

	/*
	 * La swap en sí la hace el caller (renderer) DESPUÉS de dibujar.
	 * Aquí solo "esperamos" si estamos en modo poll (vsync_type != 0).
	 * En modo blocking (vsync_type == 0), SDL ya bloqueó en la swap anterior.
	 *
	 * Si vsync_type != 0 (poll mode, fiel a PS2):
	 *   spin hasta que el frame anterior esté visible
	 */
	if (canvas->vsync_type != 0) {
		/*
		 * En PS2 esto era: leer un registro de estado del GS hasta que
		 * el flag de "frame visible" se pusiera.
		 * En PC: SDL_GL_SwapWindow con vsync=0 no bloquea, así que
		 * hacemos un yield corto para no quemar CPU.
		 * En la práctica vas a dejar vsync_type == 0 siempre.
		 */
		SDL_Delay(1);  /* ~1ms, evita 100% CPU en poll mode */
	}

	/* alternar paridad para el próximo frame */
	g_frame_parity ^= 1;

	return result;
}
