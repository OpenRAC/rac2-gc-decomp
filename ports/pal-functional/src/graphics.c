#include "graphics.h"
#include "system.h"
#include "ps2_kernel.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

extern int g_GraphicsSifInitialized;
extern unsigned int g_GraphicsVideoFormat;
static unsigned char g_Ps2ScratchpadMemory[0x200] = { 0 };

// Agrega esta línea en la sección de variables globales arriba del todo:
unsigned char g_GraphicsIopCommandBuffers[0x440 * 4] = { 0 };

// Definición de variables globales identificadas en el SIF gráfico
unsigned int g_SifClientStructure[16] = { 0 }; // Mapea DAT_00140100
int g_SifSessionReady = 1;                   // Mapea DAT_00140124 (Forzamos '1' para romper el bucle en PC)
int g_GraphicsSifInitialized = 0;            // Mapea DAT_001347ac
unsigned int g_GraphicsVideoFormat = 0;      // Mapea DAT_001347b0

// Inicializamos los modos de video virtuales (puedes usar números de control para PC)
unsigned int g_VideoMode_Current = 0;
unsigned int g_VideoMode_Target = 0;
unsigned int g_VideoMode_Fallback = 0;

// Instanciamos las variables globales identificadas
char g_GraphicsResourcePath[1024] = { 0 }; // Mapea DAT_0013ea14
unsigned int g_GraphicsCanvasFlags = 0;   // Mapea DAT_0013ea0c
unsigned int g_GraphicsCanvasParam3 = 0;  // Mapea DAT_0013ea10
int g_GraphicsScratchpadIndex = 0;        // Mapea DAT_0013ee14

int g_GraphicsTempSemaID = 0;             // Mapea DAT_0013ea00
int g_GraphicsContextState = 0;           // Mapea DAT_0013ea08
void* g_GraphicsStackBufferPtr = NULL;     // Mapea DAT_0013ea04

unsigned long long sceGsDefDispEnv(unsigned long long* out_env, short mode_flags, short width, short height, short dx, short dy) {
	LOG_SUCCESS("GRAPHICS", "Sintetizador Gráfico interceptado: %dx%d (Modo original: %d)", width, height, mode_flags);
	if (out_env == NULL) return 0;

	// 1. Ejecutamos los guardianes de sincronización que limpiamos en los pasos anteriores
	Sys_CheckConsoleVersion();
	sceFlushCache(0);

	// 2. Interceptamos las dimensiones que el motor de Ratchet & Clank 2 solicita de origen
	g_GraphicsCanvasData.width_native = (int)width;
	g_GraphicsCanvasData.height_native = (int)height;

	// --- MEJORA PARA PC MODERNA (ALTA RESOLUCIÓN) ---
	// Si el usuario no ha configurado una resolución personalizada, escalamos dinámicamente.
	// Evitamos los límites físicos de la TV de tubo (CRT) de la PS2.
	if (g_GraphicsCanvasData.width_modern == 512 && g_GraphicsCanvasData.height_modern == 288) {
		g_GraphicsCanvasData.width_modern = 1920;  // Forzamos 1080p por defecto en PC
		g_GraphicsCanvasData.height_modern = 1080;
	}

	// 3. Replicamos el llenado estructural básico que el juego espera leer en memoria
	out_env[0] = 0x66; // Código identificador interno del buffer de despliegue
	out_env[1] = (mode_flags == 0) ? 1 : 3;
	out_env[2] = ((unsigned long long)(mode_flags & 0xF) << 15) | ((unsigned long long)((width + 0x3F) >> 6) << 9);

	// 4. Simulamos el empaquetado matemático de 64 bits para el registro GS DISPLAY
	// Esto previene que las funciones secundarias del juego hagan un "trap" (crasheen) por leer 0.
	unsigned long long calculated_display_reg = 0;
	int frame_calc = (width + 0x9FF) / (width == 0 ? 1 : width);

	calculated_display_reg = ((unsigned long long)(frame_calc - 1) << 23) |
		((unsigned long long)(height - 1) << 44) |
		((unsigned long long)(dy & 0xFFF) << 12) |
		(dx & 0xFFF);

	out_env[3] = calculated_display_reg;
	out_env[4] = 0; // Registro de control superior en cero

	// Impresión de depuración en la terminal de la PC para verificar que todo fluye en tiempo real
	printf("[Graphics] Entorno de Pantalla Configurado Nativo: %dx%d | Escalado en PC a: %dx%d (%s)\n",
		width, height, g_GraphicsCanvasData.width_modern, g_GraphicsCanvasData.height_modern,
		(g_GraphicsCanvasData.target_fps == 0.0f) ? "FPS Desbloqueados" : "FPS Limitados");

	return calculated_display_reg;
}

int g_GraphicsCanvasActiveIndex = 0; // Mapea DAT_0013ea1c

int Graphics_CloseCanvasTransaction(unsigned long slot_index) {
	// 1. Localizamos la dirección de la ranura del Scratchpad virtual a partir del índice
	unsigned int scratchpad_addr = Graphics_GetScratchpadSlotAddress(slot_index);
	unsigned int* slot_ptr = (unsigned int*)(uintptr_t)scratchpad_addr;

	Sys_WaitGraphicsFrame();

	// 2. Validación: Si el sistema SIF gráfico general no está activo
	if (g_GraphicsSifInitialized == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -1;
	}

	// 3. Validación: Si la ranura no es válida o ya estaba vacía (puVar1[1] == 0)
	if (scratchpad_addr == 0 || slot_ptr == NULL || slot_ptr[1] == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -9;
	}

	// 4. Mapeo de parámetros globales imitando el flujo original
	g_GraphicsCanvasFlags = slot_ptr[0];
	g_GraphicsCanvasParam3 = (int)((scratchpad_addr - 0x13ff00) / 0x10);

	// Simulamos variables de contexto de la llamada por consistencia estructural
	g_GraphicsTempSemaID = g_GraphicsSemaphoreID;
	g_GraphicsContextState = 4;

	// 5. El paso clave: Marcamos la ranura como LIBRE (puVar1[1] = 0)
	// En PC modificamos directamente la posición correcta en nuestro arreglo virtual
	int local_offset = scratchpad_addr - 0x13ff00;

	// Corregido: Obtenemos el puntero sumando el offset en bytes a la base del arreglo
	unsigned int* local_slot = (unsigned int*)(g_Ps2ScratchpadMemory + local_offset);
	local_slot[1] = 0;

	// 6. Simulación de la Transacción SIF 1 (Cierre de Entorno en IOP)
	// Forzamos la respuesta positiva del hardware simulado
	int simulated_iop_status = 1; // 1 = Éxito devuelto en DAT_2013f640

	Sys_ReleaseGraphicsSemaphore();

	if (simulated_iop_status == 0) {
		return -11; // Error en la comunicación virtual (-0xb)
	}

	// Espera del semáforo y borrado del contexto temporal de sincronización
	sceWaitSema(g_RenderSemaphoreID_A);
	sceDeleteSema(g_GraphicsTempSemaID);

	return 0; // Retorna éxito limpio: Ranura liberada y lista en PC
}

int Graphics_DispatchCanvasTransaction(unsigned long slot_index, unsigned int param_2, long param_3) {	// 1. Obtenemos la dirección del Scratchpad virtual a partir del índice
	unsigned int scratchpad_addr = Graphics_GetScratchpadSlotAddress(slot_index);
	unsigned int* slot_ptr = (unsigned int*)(uintptr_t)scratchpad_addr;

	Sys_WaitGraphicsFrame();

	// 2. Validación de inicialización del subsistema
	if (g_GraphicsSifInitialized == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -1; // Error: SIF no inicializado
	}

	// 3. Validación de la ranura de comando
	if (scratchpad_addr == 0 || slot_ptr == NULL || slot_ptr[1] == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -9; // Error: Ranura inválida o inactiva (0xfffffff7)
	}

	unsigned int uVar1 = slot_ptr[1];

	// 4. Mapeo de parámetros globales replicando la aritmética original
	g_GraphicsCanvasFlags = slot_ptr[0];

	// Originalmente calculaba: (int)(puVar2 + -0x4ffc0) >> 4;
	g_GraphicsCanvasActiveIndex = (int)((scratchpad_addr - 0x13ff00) / 0x10);

	// Corregido: Si param_3 es un puntero o dirección, aplicamos el casteo seguro de PC
	*(uintptr_t*)&g_GraphicsResourcePath = (uintptr_t)param_3;
	g_GraphicsCanvasParam3 = param_2;

	// 5. Gestión asíncrona original (Flags de control de hilos en PS2)
	// En PC no necesitamos enmascarar transacciones en la tabla g_GraphicsActiveTransactions
	// ya que nuestra ejecución moderna es síncrona y determinista a nivel de hilos de software.
	if ((uVar1 & 0x8000) != 0) {
		// Simulación pasiva de la sección asíncrona si el motor lo requiere en sus banderas
		sceWaitSema(g_RenderSemaphoreID_A);
		sceSignalSema(g_RenderSemaphoreID_A);
	}

	// 6. Omitimos las llamadas de hardware específicas de MIPS:
	// sys_kernel_flush_dcache_range(param_2, param_3);
	// sys_kernel_flush_dcache_range(0x13ea00, 0x20);

	// 7. Simulación de la Transacción SIF 2 (Dibujado/Render)
	// Forzamos la respuesta de éxito instantáneo del coprocesador virtual
	int simulated_iop_status = 1; // 1 = Éxito devuelto en DAT_2013f640

	Sys_ReleaseGraphicsSemaphore();

	if (simulated_iop_status == 0) {
		return -11; // Fallo en transacción (0xfffffff5)
	}

	// Si el modo requiere sincronización explícita inmediata (síncrono)
	if ((uVar1 & 0x8000) == 0) {
		sceWaitSema(g_RenderSemaphoreID_A);
		sceDeleteSema(g_GraphicsTempSemaID); // Limpieza virtual segura
	}

	return 0; // Éxito total: El cuadro se despachó de forma correcta
}

unsigned int Graphics_GetScratchpadSlotAddress(unsigned long slot_index) {
	int calculated_address = 0;

	// 1. Pide acceso exclusivo al semáforo de control de renderizado A
	Sys_InitRenderBuffers();
	sceWaitSema(g_RenderSemaphoreID_A);

	// 2. Validación de límites: El Scratchpad de Insomniac soporta un máximo de 32 ranuras (0x20)
	if (slot_index < 0x20) {
		// Replicamos el cálculo original: index * 16 + Base_Scratchpad
		calculated_address = (int)slot_index * 0x10 + 0x13ff00;
		sceSignalSema(g_RenderSemaphoreID_A);
	}
	else {
		// Índice fuera de rango seguro
		sceSignalSema(g_RenderSemaphoreID_A);
		calculated_address = 0;
	}

	return calculated_address;
}

int Graphics_SetupCanvasEnvironment(const char* resource_path, unsigned int flags, unsigned int param_3) {
	int result_code = 0;

	// 1. Sincronización e inicialización perezosa de la capa SIF
	Sys_WaitGraphicsFrame();
	if (g_GraphicsSifInitialized == 0) {
		Graphics_InitSifInterface();
	}

	// 2. Validación de cambios en el modo de video
	if (Graphics_CheckVideoModeChange()) {
		Sys_ReleaseGraphicsSemaphore();
		return -0x10004; // Error: Modo de video inconsistente
	}

	// 3. Reserva de ranura de comandos en el Scratchpad Virtual
	// Obtenemos la dirección simulada compatible con PS2 (0x13ff00 + offset)
	unsigned int scratchpad_addr = Graphics_AllocateScratchpadSlot();
	int* slot_ptr = (int*)(uintptr_t)scratchpad_addr;

	if (scratchpad_addr == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -0x13; // Error: Cola de comandos rápidos llena
	}

	// 4. Copiado seguro de la ruta del recurso (Reemplaza el bucle For original de Ghidra)
	// Garantizamos que no sobrepase los 1024 bytes y termine con carácter nulo
	strncpy(g_GraphicsResourcePath, resource_path, 1023);
	g_GraphicsResourcePath[1023] = '\0';

	// 5. Traducción de la aritmética de punteros original de MIPS
	// En la PS2 real, calculaba el índice de la ranura basándose en la distancia a la RAM base
	// iVar6 = (int)(piVar4 + -0x4ffc0) >> 4;
	g_GraphicsScratchpadIndex = (int)((scratchpad_addr - 0x13ff00) / 0x10);

	// Guardamos los metadatos en las variables de control globales
	g_GraphicsCanvasFlags = flags & 0x6FFFFFFF;
	g_GraphicsCanvasParam3 = param_3;
	g_GraphicsScratchpadIndex = g_GraphicsScratchpadIndex;

	// 6. Simulación de la Transacción SIF en PC
	// Omitimos la creación de semáforos temporales y llamadas RPC de Sony.
	// Forzamos un comportamiento exitoso simulando que el IOP respondió con éxito.
	int simulated_stack_response = 0; // Simulamos que aiStack_120[0] devolvió 0 (Éxito)

	Sys_ReleaseGraphicsSemaphore();

	// Replicamos la lógica del bloque de éxito original:
	// Pide acceso exclusivo para escribir en la ranura del Scratchpad
	sceWaitSema(g_RenderSemaphoreID_A);

	// Aplicamos la máscara de bits solicitada por el juego sobre el estado de la ranura
	// piVar4[1] = piVar4[1] | param_2;
	// *piVar4 = aiStack_120[0];
	if (slot_ptr != NULL) {
		// Como estamos operando sobre nuestra memoria mapeada, modificamos los offsets correctos
		// En nuestro arreglo virtual g_Ps2ScratchpadMemory
		int local_offset = scratchpad_addr - 0x13ff00;
		int* local_slot = (int*)&g_Ps2ScratchpadMemory[local_offset];

		local_slot[1] |= flags;
		local_slot[0] = simulated_stack_response;
	}

	sceSignalSema(g_RenderSemaphoreID_A);
	result_code = simulated_stack_response;

	return result_code;
}

// Recordatorio: g_Ps2ScratchpadMemory ya fue declarado previamente en este archivo como:
// static unsigned char g_Ps2ScratchpadMemory[0x200]; 

unsigned int Graphics_AllocateScratchpadSlot(void) {
	// 1. Asegura la inicialización y pide acceso exclusivo al semáforo de renderizado
	Sys_InitRenderBuffers();
	sceWaitSema(g_RenderSemaphoreID_A);

	// Mapeamos los rangos físicos originales a offsets locales de nuestro arreglo en PC
	// Base original PS2: 0x13ff00 -> Offset PC: 0
	// Límite original PS2: 0x1400ff -> Offset PC: 0x1FF
	int local_offset = 0;

	while (true) {
		// Leemos el estado de la ranura actual en el offset correspondiente
		// Originalmente leía el offset +4 del puntero de control (iVar1 = DAT_0013ff04 en la primera iteración)
		unsigned int slot_status = *(unsigned int*)&g_Ps2ScratchpadMemory[local_offset + 4];

		if (slot_status == 0) {
			// Encontramos ranura libre: Escribimos el flag de control de ocupado
			*(unsigned int*)&g_Ps2ScratchpadMemory[local_offset + 4] = 0x10000000;

			// Liberamos el semáforo y devolvemos la dirección simulada compatible con PS2
			sceSignalSema(g_RenderSemaphoreID_A);
			return 0x13ff00 + local_offset;
		}

		// Condición de parada si el siguiente incremento de 16 bytes supera el límite del búfer
		if (0x1400ff < (0x13ff00 + local_offset + 0x10)) {
			break;
		}

		// Avanzamos a la siguiente ranura (16 bytes adelante)
		local_offset += 0x10;
	}

	// Si salimos del ciclo, el búfer de comandos rápidos está lleno
	sceSignalSema(g_RenderSemaphoreID_A);
	return 0;
}

bool Graphics_CheckVideoModeChange(void) {
	// En PC, en lugar de llamar a memcmp para solo 4 bytes, 
	// comparamos directamente los valores numéricos de los enteros.
	// Esto produce exactamente el mismo resultado lógico pero de forma nativa y ultra-rápida.

	if (g_VideoMode_Current != g_VideoMode_Target) {
		if (g_VideoMode_Current != g_VideoMode_Fallback) {
			if (g_VideoMode_Target != g_VideoMode_Fallback) {
				return true; // Los tres buffers difieren, el modo de video cambió
			}
		}
	}

	return false; // El entorno gráfico se mantiene estable
}

// Estructuras de punteros temporales del motor
void* g_GfxBufferPtrA = NULL; // Mapea DAT_0013e9c0
void* g_GfxBufferPtrB = NULL; // Mapea DAT_0013e9c4

// Simulación del bloque de memoria física 0x13ff00 de la PS2
//static unsigned char g_Ps2ScratchpadMemory[0x200] = { 0 };

int Graphics_InitSifInterface(void) {
	// 1. Omitimos inicializaciones de hardware RPC/SIF de PS2 de forma segura
	// sys_sif_rpc_init_client();
	// kernel_system_sync_guard();
	// sys_sif_register_callback(...);

	// 2. Simulación del bucle de apertura de sesión
	// En PC, al forzar g_SifSessionReady = 1, evitamos congelar la ejecución
	while (true) {
		int session_status = 0; // Simulamos éxito de sys_sif_rpc_open_transaction_session
		if (session_status < 0) {
			return -1;
		}
		if (g_SifSessionReady != 0) break;
	}

	// 3. Inicialización del Double Buffer y Sincronización Inicial
	Sys_InitRenderBuffers();

	// El motor bloquea temporalmente usando el primer semáforo de renderizado
	sceWaitSema(g_RenderSemaphoreID_A);

	// 4. Limpieza del bloque de comandos gráficos (Originalmente entre 0x13ff00 y DAT_00140100)
	// El juego salta de 16 en 16 bytes (0x10) y pone a cero el offset +4
	for (int offset = 0; offset < 0x200; offset += 0x10) {
		*(unsigned int*)&g_Ps2ScratchpadMemory[offset + 4] = 0;
	}

	// Desbloqueamos el semáforo para continuar el flujo
	sceSignalSema(g_RenderSemaphoreID_A);

	// 5. Configuración de buffers de intercambio de comandos
	g_GfxBufferPtrA = &g_GraphicsIopCommandBuffers;
	g_GfxBufferPtrB = NULL; // Mapea tu DAT_0013fac0 de forma segura en PC

	// 6. Configuración de banderas finales de éxito
	// En PC forzamos el modo exitoso e inyectamos el formato PAL (1) o NTSC (0)
	g_GraphicsSifInitialized = 1;
	g_GraphicsVideoFormat = 1; // 1 = El juego asume formato de video válido/activo

	return 0; // Retorna éxito limpio
}

/**
 * @brief Inicializa el canal SIF/RPC virtual para los gráficos en PC.
 * @return 0 para éxito, o código de error negativo.
 */
int Graphics_InitSifInterface(void);

void Graphics_SifCallback_Dispatch(void* param_1, unsigned int* param_2) {
	(void)param_1; // Evitamos advertencia de parámetro no usado

	if (param_2 != NULL) {
		// En tu descompilación original: param_2[0] es la dirección de la función a invocar
		// y param_2[1] es el argumento que se le inyecta a esa función.
		typedef void (*GraphicsSubRoutine)(unsigned int);
		GraphicsSubRoutine funcion_a_ejecutar = (GraphicsSubRoutine)((uintptr_t)param_2[0]);

		if (funcion_a_ejecutar != NULL) {
			funcion_a_ejecutar(param_2[1]);
		}
	}

	// Las instrucciones SYNC(0) y EI() se omiten en PC por ser específicas 
	// del pipeline de ejecución y control de interrupciones de la CPU MIPS.
}

// Inicializamos con valores por defecto (ej. 1080p a 60 FPS por defecto)
GraphicsCanvas g_GraphicsCanvasData = {
	.width_native = 512,
	.height_native = 288,
	.width_modern = 1920,
	.height_modern = 1080,
	.target_fps = 60.0f
};

GraphicsCanvas* Graphics_GetCanvasData(void) {
	return &g_GraphicsCanvasData;
}

void Graphics_SetCustomResolution(int width, int height, float fps) {
	g_GraphicsCanvasData.width_modern = width;
	g_GraphicsCanvasData.height_modern = height;
	g_GraphicsCanvasData.target_fps = fps;
}

// Instanciamos el índice y los buffers globales simulados (ajusta los tamaños si Ghidra te revela más)
int g_GraphicsTransactionIndex = 0;
//unsigned char g_GraphicsIopCommandBuffers[0x440 * 4] = { 0 };
int g_GraphicsActiveTransactions[32] = { 0 };

void Graphics_ProcessIopTransaction(void* packet_ptr) {
	if (packet_ptr == NULL) return;

	// Estructuramos el acceso al puntero del paquete recibido
	unsigned int* rpc_packet = (unsigned int*)packet_ptr;

	g_GraphicsTransactionIndex = 0;
	if (g_GraphicsVideoFormat != 0) {
		// En la PS2 original leía el offset 0xC del paquete SIF
		g_GraphicsTransactionIndex = (int)rpc_packet[3];
	}

	// Calculamos el puntero al bloque de comandos del IOP correspondiente
	// Nota: En PC removemos la máscara de memoria virtual '| 0x20000000' de PS2
	int* cmd_buffer = (int*)(&g_GraphicsIopCommandBuffers[g_GraphicsTransactionIndex * 0x440]);

	int cmd_id = cmd_buffer[0];
	int cmd_type = cmd_buffer[1];

	// Copia inicial si el ID es válido
	if (cmd_id > -1) {
		void* dest = (void*)(uintptr_t)cmd_buffer[2];
		void* src = (void*)&cmd_buffer[4];
		size_t size = (size_t)cmd_buffer[3];
		memcpy(dest, src, size);
	}

	// Procesador de tipos de comando del motor
	switch (cmd_type) {
	case 2: {
		int len_a = cmd_buffer[5];
		int len_b = cmd_buffer[6];

		if (len_a > 0) {
			char* dest_a = (char*)(uintptr_t)cmd_buffer[7];
			char* src_a = (char*)&cmd_buffer[9];
			for (int i = 0; i < len_a; i++) dest_a[i] = src_a[i];
		}
		if (len_b > 0) {
			char* dest_b = (char*)(uintptr_t)cmd_buffer[8];
			char* src_b = (char*)&cmd_buffer[0x19];
			for (int i = 0; i < len_b; i++) dest_b[i] = src_b[i];
		}
		break;
	}

	case 0xB:
	case 0xC: {
		// Simplificación limpia de la copia alineada de 64 bytes (8 quadwords)
		void* dest_bulk = (void*)(uintptr_t)cmd_buffer[5];
		void* src_bulk = (void*)&cmd_buffer[6];
		memcpy(dest_bulk, src_bulk, 64);
		break;
	}

	case 0x17:
	case 0x19:
	case 0x1A: {
		size_t size_cap = (size_t)cmd_buffer[6];
		if (size_cap > 0x400) size_cap = 0x400; // Límite de seguridad original

		void* dest_cap = (void*)(uintptr_t)cmd_buffer[5];
		void* src_cap = (void*)&cmd_buffer[7];
		memcpy(dest_cap, src_cap, size_cap);
		break;
	}
	}

	// Lógica de salida: Control y liberación de hilos del motor
	if (cmd_id < 0) {
		// Limpieza de la tabla de transacciones activas
		if (g_GraphicsActiveTransactions[0] == -cmd_id) {
			g_GraphicsActiveTransactions[0] = -1;
		}
		else {
			for (int i = 1; i < 32; i++) {
				if (g_GraphicsActiveTransactions[i] == -cmd_id) {
					g_GraphicsActiveTransactions[i] = -1;
					break;
				}
			}
		}
	}
	else {
		// ¡Señal activa! Despierta el lazo del juego en PC de manera segura
		iSignalSema(g_GraphicsSemaphoreID);
	}
}