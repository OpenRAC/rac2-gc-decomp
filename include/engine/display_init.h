#ifndef DISPLAY_INIT_H
#define DISPLAY_INIT_H

/* Inicializa el canal de display (GS) del motor.
 *   Idempotente: solo corre si RCNT3_FLAG_INIT no está set.
 *   [MOD-PENDING] El nº de buffers (6) es hardcode; para mods
 *   de render (triple/quad buffer) externalizar a data/. */
void display_init_channel(void);

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

#endif
