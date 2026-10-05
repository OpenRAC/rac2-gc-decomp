#include "ps2_graphics.h"
#include "graphics.h"
#include "system.h"
#include "ps2_kernel.h"
#include "core/region.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>

// Reference to the shared virtual static variable in this same suite
extern u64 g_virtual_gs_imr_mask;

/**
 * @brief Returns the current value of the graphics chip's hardware interrupt mask register (IMR).
 * Original Ghidra address: syscall stub sector (112 MIPS syscall / 0x70) (PAL)
 *
 * @return u64 The active 64-bit mask that controls the video subsystem.
 */
u64 GsGetIMR(void) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v0, 112 \n syscall");
	return 0;
#else
	// For the native PC port, return the value of the simulated mask.
	// This guarantees that the Insomniac graphics engine reads a consistent state:
	return g_virtual_gs_imr_mask;
#endif
}

/**
 * @brief Initializes and configures the screen mode and video signal (PAL/NTSC) in the Graphics Synthesizer (GS).
 * Original Ghidra address: syscall stub sector (2 MIPS syscall / 0x02) (PAL)
 *
 * @param interlace Image interlace type (0 = non-interlaced, 1 = interlaced).
 * @param omode Original console video mode (2 = NTSC 60Hz, 3 = PAL 50Hz; SCE_GS_NTSC / SCE_GS_PAL).
 * @param ffmd Frame/field mode for reading graphics memory.
 */
void SetGsCrt(s16 interlace, s16 omode, s16 ffmd) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v0, 2 \n syscall");
#else
	// For the native PC port, the window size and refresh rate are already controlled
	// natively by SDL2 in main.c. The call is intercepted to log
	// which video mode the original Insomniac Games engine requests:
	printf("[GRAPHICS HAL] SetGsCrt called -> interlace: %d, original mode: %d (%s build expects %d), field: %d\n",
		interlace, omode, RAC2_REGION_NAME, RAC2_GS_OMODE, ffmd);
#endif
}

// Internal virtual variable simulating the graphics chip's interrupt mask register
static u64 g_virtual_gs_imr_mask = 0;

/**
 * @brief Modifies the hardware interrupt mask register (IMR) of the Graphics Synthesizer (GS).
 * Original Ghidra address: syscall stub sector (113 MIPS syscall / 0x71) (PAL)
 *
 * @param imr_mask 64-bit mask that masks or unmasks video interrupts.
 * @return u64 The previous value stored in the system interrupt mask.
 */
u64 GsPutIMR(u64 imr_mask) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v1, 113 \n syscall");
	return 0;
#else
	// For the native PC port, keep the logical state of the Insomniac mask.
	// This avoids logical collisions if the engine checks which interrupts it disabled:
	u64 old_mask = g_virtual_gs_imr_mask;
	g_virtual_gs_imr_mask = imr_mask;

	return old_mask;
#endif
}

/*
 * gputimr() — Port of GsPutIMR
 * On PS2: writes the IMR (what "passes" in the RGBA mix)
 * On PC: glColorMask + glBlendFunc
 */
void gputimr(void)
{
	/* By default: full RGBA, blending off (the intro does not blend) */
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_BLEND);

	/*
	 * If a mod needs a partial alpha channel:
	 *   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
	 *   glEnable(GL_BLEND);
	 *   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	 */
}

/*
 * set_gs_crt() — Port of SetGsCrt
 * On PS2: configures the CRT HSYNC/VSYNC/total lines/resolution
 * On PC: window resize + glViewport
 */
void set_gs_crt(void)
{
	/*
	 * For the intro: 640×448 (NTSC) or 640×512 (PAL) → 16:9 on PC
	 * The original "CRT" was 4:3. For fidelity:
	 *   - a window of the region's base size with an aspect-ratio lock
	 *   - or: a full-screen window with letterbox (pillarbox)
	 *
	 * For now: the region's base PS2 resolution (see core/region.h)
	 */
	SDL_Window* win = SDL_GL_GetWindow();
	if (win) {
		SDL_SetWindowMode(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetWindowMinimumSize(win, RAC2_DISPLAY_WIDTH, RAC2_DISPLAY_HEIGHT);
		/* SDL_SetWindowAspectRatio(win, RAC2_DISPLAY_WIDTH, RAC2_DISPLAY_HEIGHT); ← SDL 2.0.18+ */
	}

	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*
 * remove_intc_handler() — Port of RemoveIntcHandler
 * On PS2: unregisters the INTC line callback (2 = VS)
 * On PC: no-op (SDL handles vsync in the driver)
 */
void remove_intc_handler(int intc_id, uint32_t handler_data)
{
	(void)intc_id;
	(void)handler_data;
	/* No-op on PC. On a real PS2: syscall 26 (sDeleteIntcHandler) */
}