#ifndef PS2_GRAPHICS_H
#define PS2_GRAPHICS_H

/**
 * Wrappers of the PS2 GS (Graphics Synthesizer) functions.
 * On PC: they map to OpenGL / SDL calls.
 * On real PS2: they would be syscalls or mfc0/mtc0 on mapped registers.
 */

 /**
  * GsPutIMR() → Image Mask Register
  * Configures which RGBA channels are included in the transfer.
  * PC: glColorMask() + glBlendFunc()
  */
void gputimr(void);

/**
 * SetGsCrt() → Set CRT timing
 * Applies the resolution, refresh rate and interlace to the display.
 * PC: SDL_SetWindowMode() + glViewport()
 */
void set_gs_crt(void);

/**
 * RemoveIntcHandler(intc_id, handler_data)
 * Registers/unregisters a handler in the INTC (Interrupt Controller).
 * intc_id 2 = Vertical Sync interrupt.
 * PC: no-op (SDL handles vsync internally).
 */
void remove_intc_handler(int intc_id, uint32_t handler_data);

#endif /* PS2_GRAPHICS_H */
