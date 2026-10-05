/*
 * kernel_cache.c – Implementación PC de __cache / coherencia de caché
 *
 * PS2 original (MIPS R5900):
 *   FUN_0011b3d0  →  lee Status[16], lock, ProcessCache(a&0x..C0, b&0x..C0), unlock
 *
 * PC (x86-64):
 *   La coherencia la garantiza el hardware. Solo emitimos un
 *   __atomic_thread_fence para preservar el orden en caso de
 *   que en el futuro haya threads compartiendo buffers (audio,
 *   input, render).
 */

#include "core/sys/kernel_cache.h"

 /* Alineación a 64B (tamaño de caché line de la EE).
	En PC es cosmético, pero mantiene la semántica si algún día
	se usa para mapear regiones de mmap(MAP_SHARED). */
#define CACHE_LINE_ALIGN_MASK  0xFFFFF000u   /* & 0xFFFFF000 = alinea a 4KB (página) */
#define CACHE_LINE_SIZE        64u

bool kernel_cache_sync(u32 addr, u32 size)
{
	/* Alineamos como en PS2 (aunque no tenga efecto funcional) */
	(void)(addr & CACHE_LINE_ALIGN_MASK);
	(void)(size & ~(CACHE_LINE_SIZE - 1u));

	/* Memory barrier: garantiza que todos los accesos de memoria
	   antes de esta línea sean visibles después.
	   En x86 es un no-op (store buffer ya es total-order),
	   en ARM/AArch64 emitiría un DMB. */
	__atomic_thread_fence(__ATOMIC_SEQ_CST);

	return true;
}

int kernel_system_sync_guard(void)
{
	/* PS2: lock del EIC / kernel.
	   PC: no-op. Si en el futuro se usa un mutex para
	   multi-thread rendering, va aquí. */
	return 0;
}

int kernel_system_sync_release(void)
{
	/* PS2: unlock del EIC / kernel.
	   PC: no-op. */
	return 0;
}
