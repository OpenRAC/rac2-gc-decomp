#ifndef MOTOR_REGS_H
#define MOTOR_REGS_H

#include <stdint.h>

/* [CONFIRM] Display channel register of the Insomniac engine.
 *   Bit 0x100 = "channel initialized" (idempotence guard).
 *   The other bits will be revealed as more functions are decoded. */
extern uint32_t g_rcnt3_mode;

#define RCNT3_FLAG_INIT   0x100u

#endif
