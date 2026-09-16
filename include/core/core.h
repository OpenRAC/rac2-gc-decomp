#ifndef CORE_H
#define CORE_H

#include <SDL.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	PRIORITY_LOW,
	PRIORITY_NORMAL,
	PRIORITY_HIGH
} ThreadPriority;

/* Global de estado compartido. 0x20c/0x168 eran offsets
 * de cabezal en el anillo SIF. En PC se fija a 0. */
int  g_sync_state;

s32 sceSemaCreate(void);
s32 sceDeleteSema(SDL_Semaphore* sema);
int CreateThread(void);
void _StartThread(void* arg);
s32 sceGetThreadId(void);
void ChangeThreadPriority(pthread_t thread, ThreadPriority priority);
int InitializeThreadManagement(void);

int ProcessData(void);

void render_init_semas(void);

/* Barrera de sincronización EE<->IOP (spin-wait original).
 *   En PC: no-op + escribir g_sync_state para que los
 *   readers posteriores no vean 0. */
void sce_sync_barrier(void);

/* Inicializa el canal de display (GS) del motor.
 *   Idempotente: solo corre si RCNT3_FLAG_INIT no está set.
 *   [MOD-PENDING] El nº de buffers (6) es hardcode; para mods
 *   de render (triple/quad buffer) externalizar a data/. */
void display_init_channel(void);

/* [CONFIRM] Canal mínimo (1 buffer) del motor, gateado por MODO.
 *   A diferencia del canal A (guard idempotente) y del B (sin guard),
 *   esta entra solo si sys_config_init() == 1 (modo default).
 *   En PC siempre entra (config default). En PS2, si el modo es
 *   avanzado, otro canal ya cubre el render y este no corre. */
void display_init_channel_min(void);

void display_init_channel_b(void);
void InitializeDisplayAndThreads(void);

void ResetInitializationFlag(void);

void NoOperation(void);

bool WaitForInterruptStatus(void);

bool kernel_system_sync_guard(void);
bool kernel_system_sync_release(void);

void InitializePointers(void);

void ProcessCache(uint param_1, uint param_2);

void InitializeStruct(int param_1);

#endif // CORE_H
