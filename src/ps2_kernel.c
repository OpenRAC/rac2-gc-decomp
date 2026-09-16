#include "ps2_kernel.h"
#include "system.h"     // Necesario para acceder a g_GraphicsSemaphore y g_GraphicsSemaphoreID
#include <SDL.h>
#include "graphics.h"   // Necesario para verificar target_fps
#include <string.h>
#include <stdlib.h>     // Requerido para atoi() en ee_atoi

int ee_memcmp(const void* ptr1, const void* ptr2, size_t num) {
	// En PC, la función estándar 'memcmp' de string.h está optimizada a nivel 
	// de ensamblador moderno (SSE/AVX) y produce exactamente el mismo resultado
	// matemático que el bucle vectorial de la PS2, pero de forma nativa y segura.
	return memcmp(ptr1, ptr2, num);
}

#if !defined(PLATFORM_PS2)
// Estructura de control interna simulada para emular los semáforos del Kernel de Sony en PC
typedef struct {
	s32 count;
	s32 max_count;
} PS2_Simulated_Semaphore;

// Tabla estática de semáforos virtuales para el entorno portátil del port
static PS2_Simulated_Semaphore g_virtual_semaphores[16] = {
	{0, 1}, // ID 0: General / Sistema
	{1, 1}, // ID 1: IO Lock Sema
	{1, 1}, // ID 2: IO Wait Sema
	{1, 1}  // ID 3: IO DMA Sema
};
#endif

/**
 * @brief Pausa la ejecución del hilo actual en el Kernel de la PS2...
 */
s32 sceWaitSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// En la consola real, esto suspende la CPU mediante ensamblador inline:
	// __asm__ volatile("li $v0, 68 \n syscall");
	return 0;
#else
	// Para el port a PC, emulamos la espera de forma pasiva y segura.

	// Si el usuario configuró los FPS como '0' (ilimitados), no bloqueamos.
	// De lo contrario, respetamos el semáforo para sincronizar el motor.
	if (g_GraphicsCanvasData.target_fps != 0.0f) {
		// En un motor real de PS2, sema_id suele mapearse a un semáforo específico.
		// Dado que este es el semáforo principal de sincronización gráfica:
		if (g_GraphicsSemaphore != NULL) {
			SDL_SemWait(g_GraphicsSemaphore);
		}
	}

	return 0;
#endif
}

/**
 * @brief Envía una señal a un semáforo del Kernel para incrementar su conteo y despertar hilos en...
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (0x42 MIPS Syscall) (PAL)
 *
 * @param sema_id Identificador único del semáforo al que se le enviará la señal de liberación.
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 sceSignalSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// En la consola real, esto se ejecuta mediante ensamblador inline de MIPS:
	// __asm__ volatile("li $v0, 66 \n syscall");
	return 0;
#else
	// Para el port moderno a PC, emulamos la liberación de forma activa.
	// Si el ID corresponde al semáforo gráfico, notificamos al sistema moderno
	if (sema_id == g_GraphicsSemaphoreID && g_GraphicsSemaphore != NULL) {
		SDL_SemPost(g_GraphicsSemaphore);
	}

	return 0;
#endif
}

/**
 * @brief Verifica de forma no bloqueante si un semáforo está disponible en el Kernel de la PS2.
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (0x45 MIPS Syscall) (PAL)
 *
 * @param sema_id Identificador único del semáforo a consultar.
 * @return s32 El conteo actual del semáforo si tuvo éxito, o un valor negativo si está bloqueado.
 */
s32 scePollSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// En la consola real, esto se traduce a la instrucción nativa:
	// __asm__ volatile("li $v0, 69 \n syscall"); // 69 en decimal es 0x45
	return 0;
#else
	// Para el port de PC, validamos primero que el ID sea correcto dentro de nuestro rango virtual
	if (sema_id == SYS_SEMAPHORE_INVALID || sema_id >= 16) {
		return -113; // ID inválido del SDK de Sony
	}

	// Buscamos cuál de nuestros semáforos de SDL2 quiere inspeccionar el motor
	SDL_sem* target_sem = NULL;
	if (sema_id == g_GraphicsSemaphoreID)    target_sem = g_GraphicsSemaphore;
	else if (sema_id == g_RenderSemaphoreID_A) target_sem = g_RenderSemaphore_A;
	else if (sema_id == g_RenderSemaphoreID_B) target_sem = g_RenderSemaphore_B;

	if (target_sem != NULL) {
		// SDL_SemTryWait intenta tomar el semáforo de inmediato:
		// Retorna 0 si estaba libre (éxito). Retorna SDL_MUTEX_TIMEDOUT si estaba ocupado.
		if (SDL_SemTryWait(target_sem) == 0) {
			return 0; // Éxito: El semáforo estaba libre y lo tomamos sin bloquear
		}
		else {
			return -489; // Código oficial de Sony para "Semáforo bloqueado/Cerrado" (Signaled/Wait state)
		}
	}

	// Si es un semáforo secundario que aún no enlazamos, devolvemos éxito por defecto para no colgar el flujo
	return 0;
#endif
}

/**
 * @brief Envía una señal de liberación a un semáforo de forma segura desde un contexto de interrupción.
 * Dirección original en Ghidra: Sector de Internal Hooks (Syscall MIPS -67 / 0xFFFFFFFFFFFFFFBD) (PAL)
 *
 * @param sema_id Identificador único del semáforo asignado por el Kernel al canal.
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 iSignalSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// En la consola real, esto se ejecuta invocando el hook del Kernel mediante assembly:
	// __asm__ volatile("li $v0, -67 \n syscall");
	return 0;
#else
	// Para el port moderno a PC, al no existir interrupciones físicas de MIPS,
	// emulamos la liberación llamando a la misma lógica activa del semáforo:
	if (sema_id == g_GraphicsSemaphoreID && g_GraphicsSemaphore != NULL) {
		SDL_SemPost(g_GraphicsSemaphore);
	}

	return 0;
#endif
}

int ee_atoi(const char* str) {
	if (str == NULL) return 0;

	// En PC nativo, 'atoi' realiza la conversión en base 10 de forma idéntica
	// al comportamiento esperado por el truncado de 32 bits del Emotion Engine.
	return atoi(str);
}

/**
 * @brief Despierta un hilo de ejecución específico que se encontraba en estado de suspensión en el Kernel de la PS2.
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (0x33 MIPS Syscall) (PAL)
 *
 * @param thread_id Identificador único del hilo de ejecución que se desea reactivar.
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 sceWakeupThread(s32 thread_id) {
#if defined(PLATFORM_PS2)
	// En la consola real, se invoca mediante ensamblador inline:
	// __asm__ volatile("li $v0, 51 \n syscall");
	return 0;
#else
	// Para el port a PC, emulamos la reactivación de hilos de forma pasiva.
	// Como la multitarea moderna de los sistemas operativos gestiona los hilos de fondo,
	// confirmamos la señal de reactivación inmediatamente para mantener el flujo limpio:
	(void)thread_id;
	return 0;
#endif
}

/**
 * @brief Reactiva y despierta de forma segura un hilo de ejecución suspendido desde un contexto de interrupción (ISR).
 * Dirección original en Ghidra: Sector de Internal Hooks (Syscall MIPS -52 / 0xFFFFFFFFFFFFFFCC) (PAL)
 *
 * @param thread_id Identificador único del hilo de ejecución que se va a despertar de urgencia.
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 iWakeupThread(s32 thread_id) {
#if defined(PLATFORM_PS2)
	// En la consola real, esto se ejecuta invocando el hook del Kernel mediante assembly:
	// __asm__ volatile("li $v0, -52 \n syscall");
	return 0;
#else
	// Para el port moderno a PC, emulamos la reanudación de forma pasiva
	// confirmando la señal de éxito inmediato del hilo:
	(void)thread_id;
	return 0;
#endif
}

/**
 * @brief Consulta el estado actual de un hilo de ejecución específico en el Kernel de la PlayStation 2.
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (0x30 MIPS Syscall) (PAL)
 *
 * @param thread_id Identificador único del hilo a inspeccionar.
 * @param status_ptr Puntero a la estructura donde el Kernel vuelca el estado del hilo (sceThreadStatus).
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 sceReferThreadStatus(s32 thread_id, void* status_ptr) {
#if defined(PLATFORM_PS2)
	// En la consola real, se invoca mediante ensamblador inline:
	// __asm__ volatile("li $v0, 48 \n syscall");
	return 0;
#else
	// Para el port a PC, emulamos la consulta devolviendo éxito inmediato (0).
	// Esto le indica al motor de Insomniac que los hilos están corriendo de forma óptima

	// Evitamos advertencias de parámetros no utilizados en el compilador moderno de PC
	(void)thread_id;

	if (status_ptr != NULL) {
		// En la PS2 real, la estructura limpia tiene un campo de estado (status).
		// El valor '1' típicamente representa el estado "RUN" (Corriendo).
		// Llenamos los primeros 4 bytes con 1 de forma segura por si el juego valida que el hilo no esté muerto.
		*(s32*)status_ptr = 1;
	}

	return 0;
#endif
}

/**
 * @brief Programa una alarma por interrupción basada en tiempo dentro del Kernel de la PlayStation 2.
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (252 MIPS Syscall / 0xFC) (PAL)
 *
 * @param microseconds Tiempo exacto en microsegundos antes de disparar la alarma por hardware.
 * @param alarm_callback Puntero a la función que actuará como manejador de la interrupción al expirar el tiempo.
 * @param callback_arg Argumento opcional de control que se le pasará a la función manejadora.
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg) {
#if defined(PLATFORM_PS2)
	// En la consola real, esto se ejecuta mediante la instrucción inline:
	// __asm__ volatile("li $v0, 252 \n syscall");
	return 0;
#else
	// Para el port a PC, dado que el sistema de archivos del sistema operativo moderno
	// resuelve las transacciones instantáneamente sin esperas físicas de hardware de tarjetas,
	// la alarma virtual confirma el agendamiento de forma inmediata:
	(void)microseconds;
	(void)alarm_callback;
	(void)callback_arg;
	return 0;
#endif
}

/**
 * @brief Suspende voluntariamente la ejecución del hilo de control activo en el Kernel de la PS2.
 * Dirección original en Ghidra: Sector de Stubs de Syscalls (0x32 MIPS Syscall) (PAL)
 *
 * @return s32 Código de estado del Kernel (0 para éxito, o valor negativo si ocurre un error).
 */
s32 sceSleepThread(void) {
#if defined(PLATFORM_PS2)
	// En la consola real, se invoca mediante ensamblador inline:
	// __asm__ volatile("li $v0, 50 \n syscall");
	return 0;
#else
	// Para el port a PC, emulamos la suspensión de forma pasiva devolviendo éxito inmediato.
	// Dado que el sistema de archivos de PC no requiere retardos de hardware físicos de 8MB,
	// el hilo virtual fluye de largo sin congelar o ralentizar la tasa de cuadros del motor:
	return 0;
#endif
}

// 1. Simulación de la función del menú principal y ciclo de la intro
void game_main_menu_and_intro_loop(void) {
	// Aquí es donde eventualmente se quedará enganchado el bucle lúdico principal
}

// 2. Simulación de registros de la SDK de Sony
uint32_t sceSifGetReg(void) {
	return 1; // Devolvemos 1 para simular que el coprocesador IOP respondió al saludo
}

// 3. Simulación de retrasos de temporizador de hardware (Equivalente nativo a nanosleep en Windows)
void nanosleep(void* req, void* rem) {
	// En Windows se simula de forma ultra-precisa usando las herramientas nativas:
	// (Puedes dejarlo vacío o mapearlo con un sub-retraso si el motor lo exige)
	(void)req; (void)rem;
}

// 4. Decodificador de caracteres personalizados del HUD (Para el conversor de texto extendido)
void custom_hud_glyph_decoder(void* param_1, void* param_2) {
	(void)param_1; (void)param_2;
}

// 5. Variables y stubs del subsistema de la Tarjeta de Memoria (Memory Card - sceMc)
// El juego consulta el estado de la tarjeta antes de cargar la intro. Las creamos vacías:
int g_sys_mc_is_bound_flag = 0;
int g_sys_mc_mutex_sema_id = -1;
int g_sys_mc_active_command_id = 0;
int g_sys_mc_channel_widget_handle = 0;

int sceMcGetInfo(int channel, int slot, void* type, void* free, void* format) {
	(void)channel; (void)slot; (void)type; (void)free; (void)format;
	return 0; // Devolvemos 0 (Tarjeta de memoria no insertada o simulada pasivamente)
}

// ============================================================================
// STUBS FINALES DE PLATAFORMA PARA CIERRE DE ENLAZADO (PC PORT)
// ============================================================================

// 1. Variables globales del sistema de Entrada/Salida (I/O) y Sonido del IOP
int g_sys_io_wait_sema_id = -1;
int g_sys_io_queue_lock_flag = 0;
int g_sys_sound_channel_widget_handle = 0;
int g_sys_io_reconfig_flag = 0;

// 2. Funciones de control de interrupciones físicas del chip Emotion Engine (MIPS)
// En PC no hay registros de interrupción de hardware directo; devolvemos éxito inmediato.
int DI(void) { return 0; }
int EI(void) { return 0; }
int SYNC(void) { return 0; }
int Status(void) { return 0; }

// 3. Variables de control del inicializador de la tabla de paginación de hardware (TLB)
// El juego limpia la TLB original al arrancar la RAM. En PC creamos los índices ficticios:
int g_tlb_wired_index = 0;
int g_tlb_bound_index = 0;
int g_tlb_status_sync = 0;
int g_tlb_extra_flags = 0;

// 4. Función de sincronización del paquete gráfico del bus de la PS2
void ps2_sync(void) {
	// En la PS2 real, esto esperaba que el bus GIF/VIF se vaciara. 
	// En PC, al procesarse de forma síncrona en el hilo, es una operación instantánea.
}

// ============================================================================
// STUBS FINALES ABSOLUTOS DE ENTRADA/SALIDA Y MEMORY CARD (PC PORT)
// ============================================================================

// 1. Variables del estado de lectura del sistema de archivos de la PS2
int g_sys_io_is_ready_flag = 1; // 1 = El lector virtual siempre está listo en PC
int g_sys_io_lock_sema_id = -1;
int g_sys_io_dma_sema_id = -1;

// 2. Descriptores de archivo y tamaños del subsistema de Tarjeta de Memoria (sceMc)
// El motor los utiliza para guardar/cargar partidas. Los inicializamos en cero seguros.
int g_sys_mc_read_fd = -1;
int g_sys_mc_read_size = 0;
int g_sys_mc_write_fd = -1;
void* g_sys_mc_write_src_ptr = NULL;
int g_sys_mc_write_size = 0;

// 3. Stubs complementarios para llamadas de tarjeta de memoria detectadas en ps2_sif
int sceMcChdir(int channel, int slot, const char* path, char* current_dir) {
	(void)channel; (void)slot; (void)path; (void)current_dir;
	return 0;
}

int sceMcWriteExtended(int channel, int slot, const char* filename, void* buffer, int size) {
	(void)channel; (void)slot; (void)filename; (void)buffer; (void)size;
	return 0;
}
