/*
 * kernel_cache.h – Coherencia de caché (adaptación PS2 → PC)
 *
 * En PS2 (MIPS/R5900):
 *   __cache(addr, size) emite cache operations (c0,k,c0) sobre
 *   un rango alineado a 64B antes/después de cada DMA del EIC.
 *
 * En PC (x86-64):
 *   La coherencia L1/L2/L3 es transparente.
 *   Esta función se reduce a un memory barrier para preservar
 *   el orden de memoria en multi-thread, y retornar siempre true.
 *
 * Mantener la firma original para que las funciones descompiladas
 * que la llaman (FUN_00128578, FUN_00132318, etc.) no requieran
 * cambios de llamada.
 */

#ifndef CORE_SYS_KERNEL_CACHE_H
#define CORE_SYS_KERNEL_CACHE_H

#include <stdbool.h>
#include "../types.h"

 /*
  * Sincroniza caché sobre el rango [addr, addr+size).
  *
  * PS2:  ProcessCache(addr & 0xFFFFFFC0, (addr+size) & 0xFFFFFFC0)
  * PC:   Memory barrier (no-op en la mayoría de los casos).
  *
  * @param addr  Dirección base (se alinea internamente a 64B)
  * @param size  Tamaño en bytes
  * @return      true si la sincronización "completó" (siempre true en PC)
  */
bool kernel_cache_sync(u32 addr, u32 size);

/*
 * Lock de sistema (equivalente a kernel_system_sync_guard en PS2).
 * En PC: no-op. Se deja para que el código descompilado compile.
 *
 * @return  0 si se pudo adquirir (siempre 0 en PC)
 */
int kernel_system_sync_guard(void);

/*
 * Unlock de sistema (equivalente a kernel_system_sync_release en PS2).
 * En PC: no-op.
 *
 * @return  0 si se pudo liberar (siempre 0 en PC)
 */
int kernel_system_sync_release(void);

#endif /* CORE_SYS_KERNEL_CACHE_H */
