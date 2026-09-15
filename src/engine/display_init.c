#include "engine/display_init.h"
#include "engine/motor_regs.h"
#include "core/sce_compat.h"

/* Nº de buffers de display que el handshake inicializa (2..7 = 6 buffers).
 * [MOD-PENDING] Hardcode del ELF. Para mods → cargar de data/levels o
 * config. Mientras, se define como macro para que el cambio sea local. */
#define DISPLAY_INIT_BUFFERS  8u

void display_init_channel(void)
{
	uint32_t cnt;

	if ((g_rcnt3_mode & RCNT3_FLAG_INIT) == 0u)
	{
		cnt = 2u;

		/* Fase 1: kicks + waits + flushes (handshake inicial EE<->IOP) */
		sce_stub_syscall116();      /* kick  */
		sce_stub_syscall90();       /* wait  */
		sce_stub_syscall90();       /* wait  */
		sceFlushCache(0, 0, 0);     /* flush */
		sceFlushCache(0, 0, 0);     /* flush */
		sce_stub_syscall116();      /* kick  */

		/* Fase 2: handshake por buffer (6 iteraciones) */
		do {
			cnt = cnt + 1u;
			sce_stub_syscall91();      /* signal: libera el buffer anterior */
			sce_stub_syscall116();     /* kick:   arranca el buffer siguiente */
		} while (cnt < DISPLAY_INIT_BUFFERS);

		/* [CONFIRM] Marcar canal como inicializado (guard de idempotencia).
		 * El ELF probablemente lo hace vía write a un registro que Ghidra
		 * no resolvió; lo seteo aquí para que la 2ª llamada no re-inicie. */
		g_rcnt3_mode |= RCNT3_FLAG_INIT;
	}
	/* Si ya estaba set, no se hace nada: idempotente. */
}

void display_b_vu_config(void)
{
#if defined(PLATFORM_PS2)
	/* 1) Aislar bits de excepción del CP0 (Status &= ~0x1c) */
	unsigned int st;
	__asm__ __volatile__(
		"mfzc  %0, 0x0000\n"
		"and   %0, %0, %1\n"
		"mtc0  %0, 0x0000\n"
		"sync\n"
		: "=&r"(st) : "i"(0xfffffffe4u) : "memory");

	/* 2) Cargar constantes del VU desde 0x141840.. (ld + mthi/mtlo/mt1/mtsa)
	   [CONFIRM] Mapear cada DAT_00141xxx -> registro vf al re-analizar
	   el body completo en Ghidra. */

	   /* 3) Restaurar contexto GPR desde 0x141650..0x141830 (lq en s4..ra)
		  [CONFIRM] Es un context-restore; en PC no se replica (PC gestiona
		  su propia pila/registros). */

		  /* 4) Status |= 0x13 (ERL|EXL) ; sync ; eret (regresa al punto de falla) */
	__asm__ __volatile__(
		"sync\n"
		"mfc0  $0, 0x0000\n"   /* Status */
		"ori   $0, $0, 0x13\n"
		"mtc0  $0, 0x0000\n"
		"sync\n"
		"eret\n"
		: : : "memory");

	g_display_b_state = *(int*)0x141660;   /* [CONFIRM] */
#else
	/* PC: no hay VU, CP0 ni excepciones del EE. No-op seguro.
	   [MOD-PENDING] Hook de uniforms: cuando exista render real del
	   canal B, cargar aquí los datos de 0x141650..0x141868 como
	   gl*Uniform del shader (matrices/paleta/contexto). */
	(void)g_display_b_state;
#endif
}
