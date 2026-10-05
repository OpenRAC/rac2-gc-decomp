/*
 * kernel_interrupts.h – Interrupt management (PS2 → PC adaptation)
 *
 * On PS2 (MIPS R5900):
 *   _EnableIntc() / _DisableIntc()
 *   Toggle the CP0 Status IE bit to enable/disable
 *   IRQs at hardware level. They return the previous value.
 *
 * On PC (x86-64):
 *   Interrupts are controlled by the OS (IDT, IRQ controller).
 *   There is no user-space equivalent.
 *   This function is a no-op that preserves the signature so that
 *   the decompiled code compiles unmodified.
 *
 * Original XREF: FUN_00126dc0 (call to FUN_0011b5f0)
 */

#ifndef CORE_SYS_KERNEL_INTERRUPTS_H
#define CORE_SYS_KERNEL_INTERRUPTS_H

#include <stdint.h>

 /*
  * Enables CPU interrupts (equivalent to _EnableIntc on PS2).
  *
  * PS2:  Status.IE = 1; return old_ie;
  * PC:   no-op. Returns 0.
  *
  * @return  Previous value of the IE bit (always 0 on PC)
  */
uint64_t kernel_enable_interrupts(void);

/*
 * Disables CPU interrupts (equivalent to _DisableIntc on PS2).
 * Declared ahead because it will probably appear in another FUN_.
 *
 * PS2:  Status.IE = 0; return old_ie;
 * PC:   no-op. Returns 0.
 */
uint64_t kernel_disable_interrupts(void);

#endif /* CORE_SYS_KERNEL_INTERRUPTS_H */
