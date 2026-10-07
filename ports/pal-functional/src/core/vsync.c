#include "core/vsync.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

/* ---- internal singleton ---- */

static graphics_canvas_data_t g_canvas_data = {
	.field_mode = 1,   /* report parity by default */
	.vsync_type = 0,   /* modo blocking (SDL vsync on) */
	.display_mode = 0x0500,  /* 640×448 NTSC (default R&C2) */
	.timing_param = 0x0100,  /* 60Hz progressive */
	.flag_interlace = 0,
	.csr_high_byte = 0,
	.irq_active = 0,
	.intc_handler_data = 0,
};

static uint32_t g_frame_parity = 0;  /* toggles 0/1 on every swap */

const graphics_canvas_data_t* graphics_canvas_data(void)
{
	return &g_canvas_data;
}

/*
 * ------------------------------------------------------------------
 *  PORT of FUN_001261f0 (0x001261F0, PS2 ELF)
 * ------------------------------------------------------------------
 *  Original:
 *    - if vsync_type == 0 → WaitForInterruptStatus() + read GS_CSR bit13
 *    - if vsync_type != 0 → vsync_wait_first() >> 13 & 1
 *    - return the field bit if field_mode == 1, else return 1
 *
 *  PC:
 *    - SDL_GL_SwapWindow() already synchronizes with the driver's VBLANK
 *    - the "parity" is generated here with a counter
 *    - poll vs interrupt mode maps to:
 *        vsync_type == 0 → SDL_SetHint(VIDEODRIVER_VSYNC, "1")  (blocking)
 *        vsync_type != 0 → SDL_SetHint(VIDEODRIVER_VSYNC, "0") + spin (rare, but faithful)
 * ------------------------------------------------------------------
 */
vsync_result_t vsync_wait(void)
{
	const graphics_canvas_data_t* canvas = graphics_canvas_data();
	vsync_result_t result = 1;  /* default: "no field info" → 1 (as in the original) */

	if (canvas->field_mode == 1) {
		result = g_frame_parity;  /* 0 or 1, alternating */
	}

	/*
	 * The swap itself is done by the caller (renderer) AFTER drawing.
	 * Here we only "wait" in poll mode (vsync_type != 0).
	 * In blocking mode (vsync_type == 0), SDL already blocked in the previous swap.
	 *
	 * If vsync_type != 0 (poll mode, faithful to the PS2):
	 *   spin until the previous frame is visible
	 */
	if (canvas->vsync_type != 0) {
		/*
		 * On PS2 this read a GS status register until
		 * the "frame visible" flag was set.
		 * On PC: SDL_GL_SwapWindow with vsync=0 does not block, so
		 * we yield briefly to avoid burning CPU.
		 * In practice vsync_type stays 0.
		 */
		SDL_Delay(1);  /* ~1ms, avoids 100% CPU in poll mode */
	}

	/* toggle the parity for the next frame */
	g_frame_parity ^= 1;

	return result;
}
