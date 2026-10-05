#include "core/sce_compat.h"

int g_display_channel_b = 0;   /* DAT_00134b48 */

/* ------------------------------------------------------------------ */
/*  Stub: syscall 116 – SIF/DMA transfer "kick"                       */
/*  PS2:  li v1, 0x74 ; syscall                                       */
/*  PC:   no-op                                                       */
/* ------------------------------------------------------------------ */
int sceSifCheckM_S(int op_code)
{
	(void)op_code;
#if defined(PLATFORM_PS2)
	__asm__ volatile ("li v0, 0x74\n\t syscall\n\t" : : : "memory", "v0");
	return 1;
#else
	return 1;
#endif
}

/* ------------------------------------------------------------------ */
/*  Stub: syscall 131 – ring status "poll"                            */
/*  PS2:  li v1, 0x83 ; syscall                                       */
/*  PC:   no-op, returns 0                                            */
/* ------------------------------------------------------------------ */
int sceSifSetM_S(void)
{
#if defined(PLATFORM_PS2)
	int r;
	__asm__ volatile ("li v0, 0x83\n\t syscall\n\t"
		: "=v"(r)
		: : "memory", "v0");
	return r;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------ */
/*  Stub: syscall 64 – sceSemaCreate (kernel semaphore)              */
/*  PS2:  li v0, 0x40 ; syscall   (param in $a0)                     */
/*  PC:   returns a stable virtual ID. Does not create SDL_Semaphore: */
/*  the engine layer does that (Sys_InitGraphicsSemaphore).          */
/* ------------------------------------------------------------------ */
int sceSemaCreate(void* param)
{
#if defined(PLATFORM_PS2)
	register void* r_param __asm__("a0") = param;
	int r;
	__asm__ volatile ("li v0, 0x40\n\t syscall\n\t"
		: "=v"(r)
		: "r"(r_param)
		: "memory", "v0");
	return r;
#else
	(void)param;
	static int virtual_sema_counter = 4;   /* 4 = 1-based; 0 reserved as invalid */
	return virtual_sema_counter++;
#endif
}

/*
 * sceFlushCache
 * ------------------------------------------------------------------
 * PS2:  li $v0, 100 ; syscall   (i/d-cache coherence over [addr,addr+size])
 * PC:   coherence is guaranteed by the hardware (MESI/MOESI). Safe no-op.
 *
 * The body is intentionally empty: x86_64/ARM do not need a per-region
 * wbinvd/clflush as the PS2 does, and doing it would cost time without
 * any benefit. If a coherence bug is ever found when sharing memory
 * with the GPU, fill it in here and nowhere else.
 */
int sceFlushCache(int mode, void* addr, int size)
{
#if defined(PLATFORM_PS2)
	/* li $v0, 100 ; syscall  -- $a0=mode $a1=addr $a2=size */
	register int   r_mode __asm__("a0") = mode;
	register void* r_addr __asm__("a1") = addr;
	register int   r_size __asm__("a2") = size;
	__asm__ volatile ("li v0, 100\n\t syscall\n\t"
		: /* out */
	: "r"(r_mode), "r"(r_addr), "r"(r_size)
		: "memory", "v0");
	return 0;
#else
	(void)mode;
	(void)addr;
	(void)size;
	return 0;
#endif
}


/* ------------------------------------------------------------------ */
/*  Stub: syscall 90 – kernel event-set / event queue                */
/*  PS2:  li v0, 0x5a ; syscall                                       */
/*  PC:   no-op (there are no IOP event-sets to wait for)            */
/* ------------------------------------------------------------------ */
int sceSifSetRpcQueue(int a0, int a1, int a2)
{
	(void)a0; (void)a1; (void)a2;
#if defined(PLATFORM_PS2)
	__asm__ volatile ("li v0, 0x5a\n\t syscall\n\t" : : : "memory", "v0");
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------ */
/*  Stub: syscall 91 – event-set signal (releases the event)         */
/*  PS2:  li v0, 0x5b ; syscall                                       */
/*  PC:   no-op (there are no IOP event-sets to release)             */
/* ------------------------------------------------------------------ */
int sceSifInitRpc(int op_code)
{
	(void)op_code;
#if defined(PLATFORM_PS2)
	__asm__ volatile ("li v0, 0x5b\n\t syscall\n\t" : : : "memory", "v0");
	return 0;
#else
	return 0;
#endif
}

void display_init_channel_b(void)
{
	uint32_t cnt = 3u;

	/* Setup: 116(0x5A) → 90 → flush(0) → flush(2) → 116(0x5B) → 116(0x54) */
	sceSifCheckM_S(0x5A);          /* kick, op=90 */
	sceSifSetRpcQueue((int)0x80075000u, 0x1347d0, 0x330);  /* wait with flags */
	sceFlushCache(0, 0, 0);
	sceFlushCache(2, 0, 0);
	sceSifCheckM_S(0x5B);          /* kick, op=91 */
	sceSifCheckM_S(0x54);          /* kick, op=84 */

	/* Loop: 5 iterations, each one: signal(op) + kick(op+1) */
	do {
		cnt = cnt + 1u;
		sceSifInitRpc(0x55 + (cnt - 3));   /* op advances 0x55→0x56 */
		sceSifCheckM_S(0x55 + (cnt - 3));  /* next kick */
	} while (cnt < 8u);

	/* [Fixed] DAT_00134b48 = 3 (literal from the asm), not the signal's return value */
	g_display_channel_b = 3;

}

int GetOsdConfigParam(void* out_buf)
{
#if defined(PLATFORM_PS2)
	register void* r __asm__("a0") = out_buf;
	__asm__ volatile ("li v0, 0x4b\n\t syscall\n\t" : : "r"(r) : "memory", "v0");
#else
	/* PC: there is no kernel. The buffer stays 0 ("default" state). */
	if (out_buf) *(unsigned int*)out_buf = 0;
#endif
	return 0;
}

int SetOsdConfigParam(const void* in_buf)
{
#if defined(PLATFORM_PS2)
	register const void* r __asm__("a0") = in_buf;
	__asm__ volatile ("li v0, 0x4a\n\t syscall\n\t"
		: : "r"(r) : "memory", "v0");
#else
	/* PC: there is no kernel to write to. Safe no-op. */
	(void)in_buf;
#endif
	return 0;
}

/* ------------------------------------------------------------------ */
/*  sys_config_init  (FUN_0011f8d0)                                    */
/*                                                                     */
/*  PS2 original:                                                     */
/*    v   = Get(sp);        v |= 0x2000;   Set(sp+4 = v);             */
/*    v   = Get(sp+4);      field = (v>>13) & 7;                      */
/*    return (campo < 1);                                                     */
/*                                                                     */
/*  PC: config starts at 0 → field = 0 → returns 1 (default mode).     */
/*  The forced bit 13 has no observable effect (there is no kernel).  */
/* ------------------------------------------------------------------ */
int sys_config_init(void)
{
	unsigned int v;

#if defined(PLATFORM_PS2)
	GetOsdConfigParam(&v);          /* 1) read the current state    */
	v |= 0x2000u;                   /* 2) force bit 13              */
	SetOsdConfigParam(&v);          /* 3) write it back             */
	GetOsdConfigParam(&v);          /* 4) read it again             */
	v = (v >> 13) & 0x7u;           /* 5) extract field 13-15 (0..7) */
	return (v < 1) ? 1 : 0;         /* 6) == 0 ? 1 : 0              */
#else
	(void)v;
	/* PC: no kernel. The "mode field" ends up 0 (default) and the
	   function returns 1 (== default mode OK). Same result as the
	   original on the "PAL / config not forced" path. */
	return 1;
#endif
}