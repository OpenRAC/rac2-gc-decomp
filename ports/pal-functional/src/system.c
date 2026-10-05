#include "system.h"
#include "graphics.h" // Required to read the extended FPS configuration
#include "ps2_kernel.h"

unsigned int Sys_CheckConsoleVersion(void) {
	/* --- ORIGINAL SIMULATED HARDWARE BEHAVIOUR (COMMENTED OUT ON PC) ---
	char acStack_140[256];
	int slot_id = Graphics_SetupCanvasEnvironment("rom0:ROMVER", 1, 0);

	if (slot_id < 0) return 0xffffffff;

	unsigned int bytes_read = 0;
	for (; bytes_read < 0x100; bytes_read++) {
		Graphics_DispatchCanvasTransaction(slot_id, (unsigned int)&acStack_140[bytes_read], 1);
		if (acStack_140[bytes_read] == '\0') break;
	}
	Graphics_CloseCanvasTransaction(slot_id);
	*/

	// In the native PC port there is no "rom0:ROMVER" ROM, so
	// the safety check is bridged by forcing a successful return (1).
	// This guarantees that the engine assumes a suitable environment and continues a stable boot.
	return 1;
}

void Sys_ReleaseGraphicsSemaphore(void) {
	// Actively call the kernel function, passing it the ID
	// of the graphics semaphore tracked in the previous functions.
	sceSignalSema(g_GraphicsSemaphoreID);
}

// Definition of the global variables shared with the kernel
int g_GraphicsSemaphoreID = SYS_SEMAPHORE_INVALID;
SDL_sem* g_GraphicsSemaphore = NULL;

void Sys_InitGraphicsSemaphore(void) {
	if (g_GraphicsSemaphoreID == SYS_SEMAPHORE_INVALID) {
		// Delegate creation to the simulated kernel behaviour
		g_GraphicsSemaphoreID = sceCreateSema();
	}
}

// ... global variables ...

long long Sys_WaitGraphicsFrame(void) {
	// 1. Make sure the semaphore exists on PC
	Sys_InitGraphicsSemaphore();

	// 2. Call the PS2 kernel function directly
	sceWaitSema(g_GraphicsSemaphoreID);

	return 0;
}

/*
// Initialize the variables to the original PS2 state (-1)
int g_GraphicsSemaphoreID = SYS_SEMAPHORE_INVALID;
SDL_sem* g_GraphicsSemaphore = NULL;

void Sys_InitGraphicsSemaphore(void) {
	if (g_GraphicsSemaphoreID == SYS_SEMAPHORE_INVALID) {
		// On PC create a binary semaphore, or one with an initial count of 0
		g_GraphicsSemaphore = SDL_CreateSemaphore(0);

		if (g_GraphicsSemaphore != NULL) {
			// Assign a dummy ID other than -1 so that the game logic
			// knows initialization succeeded and proceeds correctly.
			g_GraphicsSemaphoreID = 1;
		}
	}
}
*/

// Initialize the first pair of variables to -1
int g_RenderSemaphoreID_A = SYS_SEMAPHORE_INVALID;
SDL_sem* g_RenderSemaphore_A = NULL;

// Initialize the second pair of variables to -1
int g_RenderSemaphoreID_B = SYS_SEMAPHORE_INVALID;
SDL_sem* g_RenderSemaphore_B = NULL;

void Sys_InitRenderBuffers(void) {
	if (g_RenderSemaphoreID_A == SYS_SEMAPHORE_INVALID) {
		// When sceCreateSema is called, the emulated PC kernel creates the SDL semaphore
		// automatically and assigns it its unique virtual ID.
		g_RenderSemaphoreID_A = sceCreateSema();

		// Assign the corresponding SDL object by storing it in the control structure
		// Note: the kernel's sceCreateSema was modified to link g_GraphicsSemaphore.
		// To support several dynamic semaphores on PC robustly without breaking anything,
		// creation is captured here with a safe SDL fallback:
		g_RenderSemaphore_A = SDL_CreateSemaphore(0);

		// Do exactly the same for the second buffer/semaphore
		g_RenderSemaphoreID_B = sceCreateSema();
		g_RenderSemaphore_B = SDL_CreateSemaphore(0);
	}
}
