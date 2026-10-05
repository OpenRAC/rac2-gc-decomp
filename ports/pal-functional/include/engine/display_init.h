#ifndef DISPLAY_INIT_H
#define DISPLAY_INIT_H

/* [CONFIRM] EE exception handler epilogue for channel B.
 *   PS2:  Status &= ~0x1c (isolate exceptions)
 *         load VU from 0x141840.. (mthi/mtlo/mt1/mtsa)
 *         restore GPRs from 0x141650..0x141830 (lq into s4..ra)
 *         Status |= 0x13 (ERL/EXL) ; sync ; eret
 *   PC:   no-op (there is no VU/CP0/EE exception).
 *   [MOD-PENDING] The data at 0x141650..0x141868 is render context;
 *   once a channel B shader exists, upload it with gl*Uniform. */
void display_b_vu_config(void);

/* [CONFIRM] Contextual return value of the ELF (DAT_00141660). Check
 *   whether any caller consumes it before fixing a value. */
int g_display_b_state;

/* [CONFIRM] Engine VSync handshake: sets the VSync flag,
 *   enables the EE interrupt line (INTSTAT bit 2 = 0x4),
 *   waits for the FIRST vblank (bit 2 clear OR the handler writing to
 *   buffer[0]), acknowledges it (INTCONT) and returns the VSync
 *   handle/state (buffer[8], uStack_18 in Ghidra's C).
 *   PS2:  vent. 0x0010f000 (INTSTAT) + 0x001000000 (ack) +
 *         kernel_system_sync_guard/release (IRQ protection).
 *   PC:   there is no EE/INTSTAT. No-op, returns 0 (null handle).
 *   [MOD-PENDING] Once real rendering exists, this maps to the
 *         native vertical synchronization (glXSwapInterval / DWM /
 *         SDL_WaitEvent) of the active channel. */
int vsync_wait_first(void);

#endif
