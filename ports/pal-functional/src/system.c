#include "system.h"
#include "graphics.h" // Requerido para leer la configuración de FPS extendida
#include "ps2_kernel.h"

unsigned int Sys_CheckConsoleVersion(void) {
	/* --- COMPORTAMIENTO ORIGINAL DE HARDWARE SIMULADO (COMENTADO EN PC) ---
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

	// En el port nativo de PC, al no existir la ROM "rom0:ROMVER",
	// puenteamos la comprobación de seguridad forzando un retorno exitoso (1).
	// Esto garantiza que el motor asuma que corre en un entorno apto y prosiga con el arranque estable.
	return 1;
}

void Sys_ReleaseGraphicsSemaphore(void) {
	// Invocamos activamente la función del kernel pasándole el ID 
	// del semáforo de gráficos que rastreamos en las funciones anteriores.
	sceSignalSema(g_GraphicsSemaphoreID);
}

// Definición de las variables globales compartidas con el kernel
int g_GraphicsSemaphoreID = SYS_SEMAPHORE_INVALID;
SDL_sem* g_GraphicsSemaphore = NULL;

void Sys_InitGraphicsSemaphore(void) {
	if (g_GraphicsSemaphoreID == SYS_SEMAPHORE_INVALID) {
		// Delegamos la creación al comportamiento del kernel simulado
		g_GraphicsSemaphoreID = sceCreateSema();
	}
}

// ... tus variables globales ...

long long Sys_WaitGraphicsFrame(void) {
	// 1. Garantiza que el semáforo esté creado en PC
	Sys_InitGraphicsSemaphore();

	// 2. Llama directamente a tu función del kernel de PS2 que acabamos de corregir
	sceWaitSema(g_GraphicsSemaphoreID);

	return 0;
}

/*
// Inicializamos las variables en el estado original de la PS2 (-1)
int g_GraphicsSemaphoreID = SYS_SEMAPHORE_INVALID;
SDL_sem* g_GraphicsSemaphore = NULL;

void Sys_InitGraphicsSemaphore(void) {
	if (g_GraphicsSemaphoreID == SYS_SEMAPHORE_INVALID) {
		// En PC creamos un semáforo binario o con contador inicial en 0
		g_GraphicsSemaphore = SDL_CreateSemaphore(0);

		if (g_GraphicsSemaphore != NULL) {
			// Asignamos un ID ficticio diferente de -1 para que la lógica del juego
			// sepa que la inicialización fue exitosa y proceda correctamente.
			g_GraphicsSemaphoreID = 1;
		}
	}
}
*/

// Inicializamos el primer par de variables en -1
int g_RenderSemaphoreID_A = SYS_SEMAPHORE_INVALID;
SDL_sem* g_RenderSemaphore_A = NULL;

// Inicializamos el segundo par de variables en -1
int g_RenderSemaphoreID_B = SYS_SEMAPHORE_INVALID;
SDL_sem* g_RenderSemaphore_B = NULL;

void Sys_InitRenderBuffers(void) {
	if (g_RenderSemaphoreID_A == SYS_SEMAPHORE_INVALID) {
		// Al llamar a sceCreateSema, tu kernel emulado en PC creará el semáforo de SDL 
		// automáticamente y le asignará su ID virtual único.
		g_RenderSemaphoreID_A = sceCreateSema();

		// Asignamos el objeto SDL correspondiente guardándolo en la estructura de control
		// Nota: En tu kernel modificaste sceCreateSema para enlazar g_GraphicsSemaphore.
		// Para soportar múltiples semáforos dinámicos en PC de manera robusta sin romper nada,
		// capturamos la creación aquí con un fallback seguro de SDL:
		g_RenderSemaphore_A = SDL_CreateSemaphore(0);

		// Hacemos exactamente lo mismo para el segundo buffer/semáforo
		g_RenderSemaphoreID_B = sceCreateSema();
		g_RenderSemaphore_B = SDL_CreateSemaphore(0);
	}
}
