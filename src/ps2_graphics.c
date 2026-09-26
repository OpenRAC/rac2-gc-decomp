#include "ps2_graphics.h"
#include "graphics.h"
#include "system.h"
#include "ps2_kernel.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

// Referencia a tu variable estática virtual compartida en esta misma suite
extern u64 g_virtual_gs_imr_mask;

/**
 * @brief Recupera el valor actual del registro de máscara de interrupciones de hardware (IMR) del chip gráfico.
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (112 MIPS Syscall / 0x70) (PAL)
 *
 * @return u64 La máscara de bits activa de 64 bits que controla el subsistema de video.
 */
u64 GsGetIMR(void) {
#if defined(PLATFORM_PS2)
	// En la PlayStation 2 real, se ejecuta la instrucción ensamblador inline:
	// __asm__ volatile("li $v0, 112 \n syscall");
	return 0;
#else
	// Para el port nativo de PC, retornamos el valor de la máscara simulada de largo.
	// Esto garantiza que el motor gráfico de Insomniac lea un estado consistente:
	return g_virtual_gs_imr_mask;
#endif
}

/**
 * @brief Inicializa y configura el modo de pantalla y señal de video (PAL/NTSC) en el Sintetizador Gráfico (GS).
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (2 MIPS Syscall / 0x02) (PAL)
 *
 * @param interlace Tipo de entrelazado de la imagen (0 = No entrelazado, 1 = Entrelazado).
 * @param omode Modo de video original de la consola (2 = PAL 50Hz, 3 = NTSC 60Hz, etc.).
 * @param ffmd Modo de cuadro/campo de lectura de memoria gráfica.
 */
void SetGsCrt(s16 interlace, s16 omode, s16 ffmd) {
#if defined(PLATFORM_PS2)
	// En la PlayStation 2 real, se ejecuta la instrucción ensamblador inline:
	// __asm__ volatile("li $v0, 2 \n syscall");
#else
	// Para tu port nativo de PC, el tamaño de la ventana y los hz ya los controla 
	// SDL2 de forma nativa en tu main.c. Interceptamos la llamada para registrar 
	// si el motor original de Insomniac Games solicita arrancar en modo PAL:
	printf("[GRAPHICS HAL] SetGsCrt invocado -> Entrelazado: %d, Modo original: %d (PAL Target), Campo: %d\n",
		interlace, omode, ffmd);
#endif
}

// Variable interna virtual para simular el registro de máscara de interrupciones del chip gráfico
static u64 g_virtual_gs_imr_mask = 0;

/**
 * @brief Modifica el registro de máscara de interrupciones de hardware (IMR) del Sintetizador Gráfico (GS).
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (113 MIPS Syscall / 0x71) (PAL)
 *
 * @param imr_mask Máscara de bits de 64 bits para enmascarar o desenmascarar interrupciones de video.
 * @return u64 El valor previo almacenado en la máscara de interrupciones del sistema.
 */
u64 GsPutIMR(u64 imr_mask) {
#if defined(PLATFORM_PS2)
	// En la PlayStation 2 real, se ejecuta la instrucción ensamblador inline:
	// __asm__ volatile("li $v1, 113 \n syscall");
	return 0;
#else
	// Para el port nativo de PC, respaldamos el estado lógico de la máscara de Insomniac.
	// Esto evita colisiones lógicas si el motor comprueba qué interrupciones apagó:
	u64 old_mask = g_virtual_gs_imr_mask;
	g_virtual_gs_imr_mask = imr_mask;

	return old_mask;
#endif
}

/*
 * gputimr() — Port de GsPutIMR
 * En PS2: escribe el IMR (quién "pasa" en la mezcla RGBA)
 * En PC: glColorMask + glBlendFunc
 */
void gputimr(void)
{
	/* Por default: RGBA completos, blending off (la presentación no mezcla) */
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_BLEND);

	/*
	 * Si un mod necesita canal alfa parcial:
	 *   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	 *   glEnable(GL_BLEND);
	 *   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 */
}

/*
 * set_gs_crt() — Port de SetGsCrt
 * En PS2: configura HSYNC/VSYNC/total lines/resolution del CRT
 * En PC: resize de ventana + glViewport
 */
void set_gs_crt(void)
{
	/*
	 * Para la presentación: 640×448 (NTSC) o 640×512 (PAL) → 16:9 en PC
	 * El "CRT" original era 4:3. Para fidelidad:
	 *   - Ventana 640×448 con aspect ratio lock
	 *   - O: ventana full-screen con letterbox (pillarbox)
	 *
	 * Por ahora: 640×448 (resolución base de R&C2 PS2)
	 */
	SDL_Window* win = SDL_GL_GetWindow();
	if (win) {
		SDL_SetWindowMode(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetWindowMinimumSize(win, 640, 448);
		/* SDL_SetWindowAspectRatio(win, 640, 448); ← SDL 2.0.18+ */
	}

	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 * remove_intc_handler() — Port de RemoveIntcHandler
 * En PS2: desregistra el callback de la línea INTc (2 = VS)
 * En PC: no-op (SDL maneja vsync en el driver)
 */
void remove_intc_handler(int intc_id, uint32_t handler_data)
{
	(void)intc_id;
	(void)handler_data;
	/* No-op en PC. En PS2 real: syscall 26 (sDeleteIntcHandler) */
}