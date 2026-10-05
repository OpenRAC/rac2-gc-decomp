#ifndef PS2_GRAPHICS_H
#define PS2_GRAPHICS_H

/**
 * Wrappers de funciones del GS (Graphics Synthesizer) de PS2.
 * En PC: mapean a OpenGL / SDL calls.
 * En PS2 real: serían syscalls o mfc0/mtc0 a registros mapeados.
 */

 /**
  * GsPutIMR() → Image Mask Register
  * Configura qué canales RGBA se incluyen en la transferencia.
  * PC: glColorMask() + glBlendFunc()
  */
void gputimr(void);

/**
 * SetGsCrt() → Set CRT timing
 * Aplica la resolución, refresh rate, interlace al display.
 * PC: SDL_SetWindowMode() + glViewport()
 */
void set_gs_crt(void);

/**
 * RemoveIntcHandler(intc_id, handler_data)
 * Registra/desregistra un handler en el INTC (Interrupt Controller).
 * intc_id 2 = Vertical Sync interrupt.
 * PC: no-op (SDL maneja vsync internamente).
 */
void remove_intc_handler(int intc_id, uint32_t handler_data);

#endif /* PS2_GRAPHICS_H */
