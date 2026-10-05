#ifndef MOTOR_REGS_H
#define MOTOR_REGS_H

#include <stdint.h>

/* [CONFIRM] Registro del canal de display del motor Insomniac.
 *   Bit 0x100 = "canal inicializado" (guard de idempotencia).
 *   Los demás bits se irán revelando al descifrar más funciones. */
extern uint32_t g_rcnt3_mode;

#define RCNT3_FLAG_INIT   0x100u

#endif
