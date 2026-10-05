/* src/core/critical.c */
#include "core/critical.h"
#include "ps2_kernel.h"       /* ← _DisableIntc, etc. */
#include "core/core.h"
#include <stdatomic.h>

/*
 * ------------------------------------------------------------------
 *  PORT of FUN_0011b588 (0x0011B588)
 * ------------------------------------------------------------------
 *
 *  ASM original:
 *    flag  = (Status & 0x10000) != 0;
 *    if (flag)  kernel_system_sync_guard();
 *    prev  = _DisableIntc(intc_id);
 *    SYNC(0);
 *    if (flag)  kernel_system_sync_release();
 *    return prev;
 *
 *  PC single-thread:  no-op, returns 0.
 *  PC multi-thread:   mutex (future).
 * ------------------------------------------------------------------
 */

static _Atomic int g_kernel_sync_active = 0;

uint32_t core_critical_enter(uint32_t intc_id)
{
	uint32_t prev_state = 0;

	int flag = (g_kernel_sync_active != 0);

	if (flag) {
		kernel_system_sync_guard();
	}

	prev_state = (uint32_t)_DisableIntc((s32)intc_id);

	__atomic_thread_fence(__ATOMIC_SEQ_CST);

	if (flag) {
		kernel_system_sync_release();
	}

	return prev_state;
}

void core_critical_exit(uint32_t prev_state)
{
	(void)prev_state;
	__atomic_thread_fence(__ATOMIC_SEQ_CST);
}
