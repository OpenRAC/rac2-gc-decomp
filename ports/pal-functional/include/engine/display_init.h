#ifndef DISPLAY_INIT_H
#define DISPLAY_INIT_H

/* [CONFIRM] Epílogo de handler de excepción del EE para el canal B.
 *   PS2:  Status &= ~0x1c (aislar excepciones)
 *         cargar VU desde 0x141840.. (mthi/mtlo/mt1/mtsa)
 *         restaurar GPRs desde 0x141650..0x141830 (lq en s4..ra)
 *         Status |= 0x13 (ERL/EXL) ; sync ; eret
 *   PC:   no-op (no hay VU/CP0/excepciones del EE).
 *   [MOD-PENDING] Los datos de 0x141650..0x141868 son contexto de
 *   render; cuando exista shader del canal B, volcar a gl*Uniform. */
void display_b_vu_config(void);

/* [CONFIRM] Retorno contextual del ELF (DAT_00141660). Verificar
 *   si algún caller lo consume antes de fijar valor. */
int g_display_b_state;

/* [CONFIRM] Handshake de VSync del motor: setea la flag de VSync,
 *   habilita la línea de interrupción del EE (INTSTAT bit 2 = 0x4),
 *   espera al PRIMER vblank (bit 2 limpio O handler escribiendo en
 *   buffer[0]), hace el ack (INTCONT) y devuelve el handle/estado del
 *   VSync (buffer[8], el uStack_18 del C de Ghidra).
 *   PS2:  vent. 0x0010f000 (INTSTAT) + 0x001000000 (ack) +
 *         kernel_system_sync_guard/release (protección de IRQ).
 *   PC:   no hay EE/INTSTAT. No-op, devuelve 0 (handle nulo).
 *   [MOD-PENDING] Cuando exista render real, esto se mapea a la
 *         sincronización vertical nativa (glXSwapInterval / DWM /
 *         SDL_WaitEvent) del canal activo. */
int vsync_wait_first(void);

#endif
