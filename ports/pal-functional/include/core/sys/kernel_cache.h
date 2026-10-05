/*
 * kernel_cache.h – Cache coherence (PS2 → PC adaptation)
 *
 * On PS2 (MIPS/R5900):
 *   __cache(addr, size) issues cache operations (c0,k,c0) over
 *   a 64B-aligned range before/after each EIC DMA.
 *
 * On PC (x86-64):
 *   L1/L2/L3 coherence is transparent.
 *   This function reduces to a memory barrier that preserves
 *   memory ordering across threads, and always returns true.
 *
 * Keep the original signature so that the decompiled functions
 * that call it (FUN_00128578, FUN_00132318, etc.) need no
 * call-site changes.
 */

#ifndef CORE_SYS_KERNEL_CACHE_H
#define CORE_SYS_KERNEL_CACHE_H

#include <stdbool.h>
#include "../types.h"

 /*
  * Synchronizes the cache over the range [addr, addr+size).
  *
  * PS2:  ProcessCache(addr & 0xFFFFFFC0, (addr+size) & 0xFFFFFFC0)
  * PC:   Memory barrier (a no-op in most cases).
  *
  * @param addr  Base address (aligned internally to 64B)
  * @param size  Size in bytes
  * @return      true if the synchronization "completed" (always true on PC)
  */
bool kernel_cache_sync(u32 addr, u32 size);

/*
 * System lock (equivalent to kernel_system_sync_guard on PS2).
 * On PC: no-op. Kept so the decompiled code compiles.
 *
 * @return  0 if it could be acquired (always 0 on PC)
 */
bool kernel_system_sync_guard(void);

/*
 * System unlock (equivalent to kernel_system_sync_release on PS2).
 * On PC: no-op.
 *
 * @return  0 if it could be released (always 0 on PC)
 */
bool kernel_system_sync_release(void);

#endif /* CORE_SYS_KERNEL_CACHE_H */
