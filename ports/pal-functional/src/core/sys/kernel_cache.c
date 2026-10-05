/*
 * kernel_cache.c – PC implementation of __cache / cache coherence
 *
 * PS2 original (MIPS R5900):
 *   FUN_0011b3d0  →  reads Status[16], lock, ProcessCache(a&0x..C0, b&0x..C0), unlock
 *
 * PC (x86-64):
 *   Coherence is guaranteed by the hardware. We only emit an
 *   __atomic_thread_fence to preserve ordering in case
 *   threads share buffers in the future (audio,
 *   input, render).
 */

#include "core/sys/kernel_cache.h"

 /* 64B alignment (EE cache line size).
	On PC it is cosmetic, but it keeps the semantics if it is ever
	used to map mmap(MAP_SHARED) regions. */
#define CACHE_LINE_ALIGN_MASK  0xFFFFF000u   /* & 0xFFFFF000 = align to 4KB (page) */
#define CACHE_LINE_SIZE        64u

bool kernel_cache_sync(u32 addr, u32 size)
{
	/* Align as on the PS2 (even though it has no functional effect) */
	(void)(addr & CACHE_LINE_ALIGN_MASK);
	(void)(size & ~(CACHE_LINE_SIZE - 1u));

	/* Memory barrier: guarantees that every memory access
	   before this line is visible afterwards.
	   On x86 it is a no-op (the store buffer is already total-order),
	   on ARM/AArch64 it would emit a DMB. */
	__atomic_thread_fence(__ATOMIC_SEQ_CST);

	return true;
}

bool kernel_system_sync_guard(void)
{
	/* PS2: EIC / kernel lock.
	   PC: no-op. If a mutex is ever used for
	   multi-threaded rendering, it goes here. */
	return false;
}

bool kernel_system_sync_release(void)
{
	/* PS2: EIC / kernel unlock.
	   PC: no-op. */
	return false;
}
