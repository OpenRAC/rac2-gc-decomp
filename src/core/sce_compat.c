#include "core/sce_compat.h"

/* ------------------------------------------------------------------ */
/*  Stub: syscall 116 – "kick" de transferencia SIF/DMA               */
/*  PS2:  li v1, 0x74 ; syscall                                       */
/*  PC:   no-op                                                       */
/* ------------------------------------------------------------------ */
int sce_stub_syscall116(void)
{
#if defined(PLATFORM_PS2)
	__asm__ volatile ("li v0, 0x74\n\t syscall\n\t" : : : "memory", "v0");
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------ */
/*  Stub: syscall 131 – "poll" de estado del anillo                   */
/*  PS2:  li v1, 0x83 ; syscall                                       */
/*  PC:   no-op, devuelve 0                                           */
/* ------------------------------------------------------------------ */
int sce_stub_syscall131(void)
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
/*  PS2:  li v0, 0x40 ; syscall   (param en $a0)                     */
/*  PC:   devuelve un ID virtual estable. No crea SDL_Semaphore:     */
/*  eso lo hace la capa engine (Sys_InitGraphicsSemaphore).          */
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
	static int virtual_sema_counter = 4;   /* 4 = 1-based; 0 reservado como invalid */
	return virtual_sema_counter++;
#endif
}

/*
 * sceFlushCache
 * ------------------------------------------------------------------
 * PS2:  li $v0, 100 ; syscall   (coherencia de i/d-cache sobre [addr,addr+size])
 * PC:   la coherencia la garantiza el hardware (MESI/MOESI). No-op seguro.
 *
 * Se deja el cuerpo vacío a propósito: en x86_64/ARM no hay que forzar
 * un wbinvd/clflush por región como en PS2, y hacerlo penalizaría sin
 * aportar nada. Si en el futuro se detecta un bug de coherencia al
 * compartir memoria con la GPU, se rellena aquí y en ningún otro sitio.
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
/*  Stub: syscall 90 – event-set / cola de eventos del kernel         */
/*  PS2:  li v0, 0x5a ; syscall                                       */
/*  PC:   no-op (no hay event-sets del IOP que esperar)               */
/* ------------------------------------------------------------------ */
int sce_stub_syscall90(void)
{
#if defined(PLATFORM_PS2)
	__asm__ volatile ("li v0, 0x5a\n\t syscall\n\t" : : : "memory", "v0");
	return 0;
#else
	return 0;
#endif
}

/* ------------------------------------------------------------------ */
/*  Stub: syscall 91 – signal de event-set (libera el evento)         */
/*  PS2:  li v0, 0x5b ; syscall                                       */
/*  PC:   no-op (no hay event-sets del IOP que liberar)               */
/* ------------------------------------------------------------------ */
int sce_stub_syscall91(void)
{
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
	sce_stub_syscall116(0x5A);          /* kick, op=90 */
	sce_stub_syscall90(0x80075000, 0x1347d0, 0x330);  /* wait con flags */
	sceFlushCache(0, 0, 0);
	sceFlushCache(2, 0, 0);
	sce_stub_syscall116(0x5B);          /* kick, op=91 */
	sce_stub_syscall116(0x54);          /* kick, op=84 */

	/* Bucle: 5 iteraciones, cada una: signal(op) + kick(op+1) */
	do {
		cnt = cnt + 1u;
		sce_stub_syscall91(0x55 + (cnt - 3));   /* op avanza 0x55→0x56 */
		sce_stub_syscall116(0x55 + (cnt - 3));  /* kick siguiente */
	} while (cnt < 8u);

	/* [Corregido] DAT_00134b48 = 3 (literal del asm), no el retorno del signal */
	g_display_channel_b = 3;

}

int GetOsdConfigParam(void* out_buf)
{
#if defined(PLATFORM_PS2)
	register void* r __asm__("a0") = out_buf;
	__asm__ volatile ("li v0, 0x4b\n\t syscall\n\t" : : "r"(r) : "memory", "v0");
#else
	/* PC: no hay kernel. El buffer queda a 0 (estado "default"). */
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
	/* PC: no hay kernel que escribir. No-op seguro. */
	(void)in_buf;
#endif
	return 0;
}

/* ------------------------------------------------------------------ */
/*  sys_config_init  (FUN_0011f8d0)                                    */
/*                                                                     */
/*  PS2 original:                                                     */
/*    v   = Get(sp);        v |= 0x2000;   Set(sp+4 = v);             */
/*    v   = Get(sp+4);      campo = (v>>13) & 7;                      */
/*    return (campo < 1);                                                     */
/*                                                                     */
/*  PC: config empieza a 0 → campo = 0 → devuelve 1 (modo default).    */
/*  El bit 13 forzado no tiene efecto observable (no hay kernel).      */
/* ------------------------------------------------------------------ */
int sys_config_init(void)
{
	unsigned int v;

#if defined(PLATFORM_PS2)
	GetOsdConfigParam(&v);          /* 1) leer estado actual        */
	v |= 0x2000u;                   /* 2) forzar el bit 13          */
	SetOsdConfigParam(&v);          /* 3) escribir de vuelta        */
	GetOsdConfigParam(&v);          /* 4) re-leer                   */
	v = (v >> 13) & 0x7u;           /* 5) extraer campo 13-15 (0..7)*/
	return (v < 1) ? 1 : 0;         /* 6) == 0 ? 1 : 0              */
#else
	(void)v;
	/* PC: sin kernel. El "campo de modo" termina en 0 (default) y la
	   función devuelve 1 (== modo default OK). Idéntico resultado al
	   original en la vía "PAL / config no forzada". */
	return 1;
#endif
}