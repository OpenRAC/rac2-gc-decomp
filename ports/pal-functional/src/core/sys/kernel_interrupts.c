/*
 * kernel_interrupts.c – Implementación PC de _EnableIntc / _DisableIntc
 *
 * PS2 original (MIPS R5900):
 *   FUN_0011b5f0  →  lee Status[16], lock, _EnableIntc(), sync 0, unlock
 *
 * PC (x86-64):
 *   Las IRQs son responsabilidad del SO (IDT, LAPIC/IOAPIC).
 *   En user-space no se puede togglear IE.
 *   La función es un no-op. Se mantiene la firma u64(void)
 *   para compatibilidad con el código descompilado.
 */

#include "core/sys/kernel_interrupts.h"

uint64_t kernel_enable_interrupts(void)
{
    /* PS2: mtc0 Status, (Status | 0x2)  →  IE = 1
           sync 0
           return old_ie;
       PC:  no-op.
       Si en el futuro se necesita una barrier de instrucción
       (equiv. a 'sync 0' en MIPS), en x86 es un no-op.
       En ARM/AArch64 sería: __asm__ volatile("isb" ::: "memory"); */

    (void)0;   /* suppress unused-variable warning si se quita el fence */
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
