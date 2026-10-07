#include "core/ee_memory.h"
#include "engine/display_init.h"
#include "engine/motor_regs.h"
#include "core/sce_compat.h"

int g_display_b_state = 0;   /* DAT_00141660 */

void display_b_vu_config(void)
{
#if defined(PLATFORM_PS2)
	/* 1) Isolate the CP0 exception bits (Status &= ~0x1c) */
	unsigned int st;
	__asm__ __volatile__(
		"mfzc  %0, 0x0000\n"
		"and   %0, %0, %1\n"
		"mtc0  %0, 0x0000\n"
		"sync\n"
		: "=&r"(st) : "i"(0xfffffffe4u) : "memory");

	/* 2) Load the VU constants from 0x141840.. (ld + mthi/mtlo/mt1/mtsa)
	   [CONFIRM] Map each DAT_00141xxx -> vf register when re-analysing
	   the complete body in Ghidra. */

	   /* 3) Restore the GPR context from 0x141650..0x141830 (lq into s4..ra)
		  [CONFIRM] It is a context restore; PC does not replicate it (PC manages
		  its own stack/registers). */

		  /* 4) Status |= 0x13 (ERL|EXL) ; sync ; eret (returns to the faulting point) */
	__asm__ __volatile__(
		"sync\n"
		"mfc0  $0, 0x0000\n"   /* Status */
		"ori   $0, $0, 0x13\n"
		"mtc0  $0, 0x0000\n"
		"sync\n"
		"eret\n"
		: : : "memory");

	g_display_b_state = *(int*)EE_ADDR(0x141660);   /* [CONFIRM] */
#else
	/* PC: no VU, CP0 or EE exceptions. Safe no-op.
	   [MOD-PENDING] Uniform hook: once real channel B rendering exists,
	   load the data at 0x141650..0x141868 here as
	   gl*Uniform values of the shader (matrices/palette/context). */
	(void)g_display_b_state;
#endif
}

/* [CONFIRM] EE interrupt registers (memory windows).
 *   INTSTAT = 0x0010f000 (line status) ; bit 2 = VSync (0x4).
 *   INTCONT = 0x001000000 (control / ack). */
#define EE_INTSTAT   (*(volatile unsigned int *)0x0010f000u)
#define EE_INTCONT   (*(volatile unsigned int *)0x001000000u)
#define EE_INT_VSYNC 0x4u

int vsync_wait_first(void)
{
	unsigned int buf0 = 0;   /* buffer[0]: flag written by the IRQ handler */
	unsigned int handle = 0; /* buffer[8]: returned handle/state            */
	int guard;

#if defined(PLATFORM_PS2)
	/* 1) SetVSyncFlag(sp, sp+8): passes both buffer slots to the engine */
	SetVSyncFlag(&buf0, &handle);

	/* 2) Protect IRQs + enable the VSync line in INTSTAT */
	guard = kernel_system_sync_guard();
	EE_INTSTAT |= EE_INT_VSYNC;
	__asm__ __volatile__("sync\n" ::: "memory");
	if (guard) kernel_system_sync_release();

	/* 3) Wait for the first vblank: bit 2 clear OR handler wrote buf0 */
	for (;;)
	{
		if ((EE_INTSTAT & EE_INT_VSYNC) != 0) break;   /* vblank arrived */
		if (buf0 != 0) break;                          /* handler wrote */
	}

	/* 4) Acknowledge the interrupt (with IRQs protected) */
	guard = kernel_system_sync_guard();
	EE_INTCONT |= EE_INT_VSYNC;
	__asm__ __volatile__("sync\n" ::: "memory");
	if (guard) kernel_system_sync_release();

	return (int)handle;
#else
	/* PC: no EE, no INTSTAT/INTCONT, no engine IRQ handler.
	   [MOD-PENDING] Once real rendering exists, this maps to the
	   native vertical synchronization of the active channel
	   (glXSwapInterval / DWM / SDL_WaitEvent) and returns a native
	   handle instead of 0. */
	(void)buf0;
	(void)handle;
	return 0;
#endif
}
