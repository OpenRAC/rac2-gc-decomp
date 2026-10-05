#ifndef CORE_VSYNC_H
#define CORE_VSYNC_H

#include <stdint.h>

/**
 * Resultado de un wait de vsync.
 * bit 0 → paridad del frame (0 = par, 1 = impar)
 *        En PS2 era "campo interlazado". En PC es frame_parity.
 */
typedef uint32_t vsync_result_t;

/**
 * Estado del canvas/gráfico que el motor actualiza por frame.
 * En PS2 era un struct global manejado por el SIF/GS driver.
 * En PC lo mantiene el módulo de render.
 */
typedef struct {
	uint16_t field_mode;   /* 1 = retornar paridad real, 0 = siempre 1 */
	uint16_t _reserved;    /* padding / flag futuro */
	uint32_t vsync_type;   /* 0 = blocking (SDL default), !=0 = polling (swap buffer + spin) */
	uint16_t display_mode;      /* offset 0: param_2 (resolución hint)     */
	uint16_t timing_param;      /* offset 2: param_3 (timing/refresh)      */
	uint16_t flag_interlace;    /* offset 4: (param_4 != 0)                */
	uint16_t csr_high_byte;     /* offset 6: (GS_CSR >> 16) & 0xFF         */
	uint32_t irq_active;        /* offset 8: 0 = sin IRQ, !=0 = handler on */
	uint32_t intc_handler_data; /* offset 12: dato para RemoveIntcHandler  */
} graphics_canvas_data_t;

/**
 * Puntero global al estado del canvas.
 * En PS2 era g_GraphicsCanvasData() (función que retornaba el puntero).
 * En PC es un singleton en core/renderer.
 */
const graphics_canvas_data_t* graphics_canvas_data(void);

/**
 * Espera el próximo vertical blank.
 * Bloqueante hasta que el display termine de escanear el frame actual.
 *
 * Devuelve vsync_result_t:
 *   - bit 0: paridad del frame (útil para mods interlazados / shaders)
 *   - resto: 0
 *
 * En PS2: leía GS_CSR bit 13 o usaba IRQ de VS.
 * En PC:  SDL_SwapWindow() + SDL_GL_SwapWindow() + frame counter.
 *
 * NOTA: Esta función NO debe llamarse desde el thread de audio.
 */
vsync_result_t vsync_wait(void);

#endif /* CORE_VSYNC_H */
