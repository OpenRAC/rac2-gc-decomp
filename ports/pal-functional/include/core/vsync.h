#ifndef CORE_VSYNC_H
#define CORE_VSYNC_H

#include <stdint.h>

/**
 * Result of a vsync wait.
 * bit 0 → frame parity (0 = even, 1 = odd)
 *        On PS2 it was the "interlaced field". On PC it is frame_parity.
 */
typedef uint32_t vsync_result_t;

/**
 * Canvas/graphics state that the engine updates every frame.
 * On PS2 it was a global struct managed by the SIF/GS driver.
 * On PC it is maintained by the render module.
 */
typedef struct {
	uint16_t field_mode;   /* 1 = return the real parity, 0 = always 1 */
	uint16_t _reserved;    /* padding / flag futuro */
	uint32_t vsync_type;   /* 0 = blocking (SDL default), !=0 = polling (swap buffer + spin) */
	uint16_t display_mode;      /* offset 0: param_2 (resolution hint)     */
	uint16_t timing_param;      /* offset 2: param_3 (timing/refresh)      */
	uint16_t flag_interlace;    /* offset 4: (param_4 != 0)                */
	uint16_t csr_high_byte;     /* offset 6: (GS_CSR >> 16) & 0xFF         */
	uint32_t irq_active;        /* offset 8: 0 = no IRQ, !=0 = handler on  */
	uint32_t intc_handler_data; /* offset 12: data for RemoveIntcHandler   */
} graphics_canvas_data_t;

/**
 * Global pointer to the canvas state.
 * On PS2 it was g_GraphicsCanvasData() (a function returning the pointer).
 * On PC it is a singleton in core/renderer.
 */
const graphics_canvas_data_t* graphics_canvas_data(void);

/**
 * Waits for the next vertical blank.
 * Blocks until the display finishes scanning out the current frame.
 *
 * Returns vsync_result_t:
 *   - bit 0: frame parity (useful for interlaced mods / shaders)
 *   - rest: 0
 *
 * On PS2: read GS_CSR bit 13 or used the VS IRQ.
 * On PC:  SDL_SwapWindow() + SDL_GL_SwapWindow() + frame counter.
 *
 * NOTE: This function must NOT be called from the audio thread.
 */
vsync_result_t vsync_wait(void);

#endif /* CORE_VSYNC_H */
