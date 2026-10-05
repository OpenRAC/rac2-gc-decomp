/*
 * kernel_interrupts.h – Gestión de interrupciones (adaptación PS2 → PC)
 *
 * En PS2 (MIPS R5900):
 *   _EnableIntc() / _DisableIntc()
 *   Togan el bit IE del CP0 Status para habilitar/deshabilitar
 *   IRQs a nivel de hardware. Retornan el valor anterior.
 *
 * En PC (x86-64):
 *   Las interrupciones las controla el SO (IDT, IRQ controller).
 *   En user-space no existe equivalente.
 *   Esta función es un no-op que preserva la firma para que
 *   el código descompilado compile sin modificación.
 *
 * XREF original: FUN_00126dc0 (llamada a FUN_0011b5f0)
 */

#ifndef CORE_SYS_KERNEL_INTERRUPTS_H
#define CORE_SYS_KERNEL_INTERRUPTS_H

#include <stdint.h>

 /*
  * Habilita interrupciones del CPU (equivalente a _EnableIntc en PS2).
  *
  * PS2:  Status.IE = 1; return old_ie;
  * PC:   no-op. Retorna 0.
  *
  * @return  Valor anterior del bit IE (siempre 0 en PC)
  */
uint64_t kernel_enable_interrupts(void);

/*
 * Deshabilita interrupciones del CPU (equivalente a _DisableIntc en PS2).
 * Se anticipa porque probablemente aparezca en otra FUN_.
 *
 * PS2:  Status.IE = 0; return old_ie;
 * PC:   no-op. Retorna 0.
 */
uint64_t kernel_disable_interrupts(void);

#endif /* CORE_SYS_KERNEL_INTERRUPTS_H */
