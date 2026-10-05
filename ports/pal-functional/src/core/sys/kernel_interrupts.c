/*
 * kernel_interrupts.c – PC implementation of _EnableIntc / _DisableIntc
 *
 * PS2 original (MIPS R5900):
 *   FUN_0011b5f0  →  lee Status[16], lock, _EnableIntc(), sync 0, unlock
 *
 * PC (x86-64):
 *   IRQs are the responsibility of the OS (IDT, LAPIC/IOAPIC).
 *   User space cannot toggle IE.
 *   The function is a no-op. The u64(void) signature is kept
 *   for compatibility with the decompiled code.
 */

#include "core/sys/kernel_interrupts.h"

uint64_t kernel_enable_interrupts(void)
{
    /* PS2: mtc0 Status, (Status | 0x2)  →  IE = 1
           sync 0
           return old_ie;
       PC:  no-op.
       If an instruction barrier is ever needed
       (equivalent to 'sync 0' on MIPS), on x86 it is a no-op.
       On ARM/AArch64 it would be: __asm__ volatile("isb" ::: "memory"); */

    (void)0;   /* suppresses the unused-variable warning if the fence is removed */
    return 0;
}

uint64_t kernel_disable_interrupts(void)
{
    /* PS2: mtc0 Status, (Status & ~0x2) →  IE = 0
           sync 0
           return old_ie;
       PC:  no-op. */

    (void)0;
    return 0;
}
