#include "engine/display_init.h"
#include "engine/motor_regs.h"
#include "core/sce_compat.h"

/* Nº de buffers de display que el handshake inicializa (2..7 = 6 buffers).
 * [MOD-PENDING] Hardcode del ELF. Para mods → cargar de data/levels o
 * config. Mientras, se define como macro para que el cambio sea local. */
#define DISPLAY_INIT_BUFFERS  8u

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

/* [CONFIRM] Registros de interrupciones del EE (ventanas de memoria).
 *   INTSTAT = 0x0010f000 (estado de líneas) ; bit 2 = VSync (0x4).
 *   INTCONT = 0x001000000 (control / ack). */
#define EE_INTSTAT   (*(volatile unsigned int *)0x0010f000u)
#define EE_INTCONT   (*(volatile unsigned int *)0x001000000u)
#define EE_INT_VSYNC 0x4u

int vsync_wait_first(void)
{
	unsigned int buf0 = 0;   /* buffer[0]: flag escrita por el handler de IRQ */
	unsigned int handle = 0; /* buffer[8]: handle/estado devuelto           */
	int guard;

#if defined(PLATFORM_PS2)
	/* 1) SetVSyncFlag(sp, sp+8): le pasa al motor los dos slots del buffer */
	SetVSyncFlag(&buf0, &handle);

	/* 2) Protege IRQ + habilita la línea de VSync en INTSTAT */
	guard = kernel_system_sync_guard();
	EE_INTSTAT |= EE_INT_VSYNC;
	__asm__ __volatile__("sync\n" ::: "memory");
	if (guard) kernel_system_sync_release();

	/* 3) Espera al primer vblank: bit 2 limpio O handler en buf0 */
	for (;;)
	{
		if ((EE_INTSTAT & EE_INT_VSYNC) != 0) break;   /* vblank llegó */
		if (buf0 != 0) break;                          /* handler escribió */
	}

	/* 4) Ack de la interrupción (con IRQ protegidas) */
	guard = kernel_system_sync_guard();
	EE_INTCONT |= EE_INT_VSYNC;
	__asm__ __volatile__("sync\n" ::: "memory");
	if (guard) kernel_system_sync_release();

	return (int)handle;
#else
	/* PC: sin EE, sin INTSTAT/INTCONT, sin handler de IRQ del motor.
	   [MOD-PENDING] Cuando exista render real, esto se mapea a la
	   sincronización vertical nativa del canal activo
	   (glXSwapInterval / DWM / SDL_WaitEvent) y devuelve un handle
	   nativo en lugar de 0. */
	(void)buf0;
	(void)handle;
	return 0;
#endif
}

