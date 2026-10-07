#ifndef MOTOR_REGS_H
#define MOTOR_REGS_H

#include <stdint.h>

/* [CONFIRM] Display channel register of the Insomniac engine.
 *   Bit 0x100 = "channel initialized" (idempotence guard).
 *   The other bits will be revealed as more functions are decoded. */
extern uint32_t g_rcnt3_mode;

#define RCNT3_FLAG_INIT   0x100u

/* Number of display buffers the handshake initializes (2..7 = 6 buffers).
 * [MOD-PENDING] Hard-coded in the ELF. For mods → load from data/levels or
 * config. Meanwhile it is a macro so the change stays local. */
#define DISPLAY_INIT_BUFFERS  8u

#endif
