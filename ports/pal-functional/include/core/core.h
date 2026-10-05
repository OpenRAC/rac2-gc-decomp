#ifndef CORE_H
#define CORE_H

#include <SDL.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>
#include "core/sys/kernel_cache.h"

typedef enum {
	PRIORITY_LOW,
	PRIORITY_NORMAL,
	PRIORITY_HIGH
} ThreadPriority;

/* Shared global state. 0x20c/0x168 were head
 * offsets in the SIF ring. On PC it is fixed to 0. */
extern int g_sync_state;

void ChangeThreadPriority(pthread_t thread, ThreadPriority priority);
int InitializeThreadManagement(void);

int ProcessData(void);

/* EE<->IOP synchronization barrier (original spin-wait).
 *   On PC: no-op + write g_sync_state so that the
 *   later readers do not see 0. */
void sce_sync_barrier(void);

/* Initializes the engine's display channel (GS).
 *   Idempotent: only runs if RCNT3_FLAG_INIT is not set.
 *   [MOD-PENDING] The number of buffers (6) is hard-coded; for render
 *   mods (triple/quad buffering) move it out to data/. */
void display_init_channel(void);

/* [CONFIRM] Minimal engine channel (1 buffer), gated by MODE.
 *   Unlike channel A (idempotent guard) and channel B (no guard),
 *   this one runs only if sys_config_init() == 1 (default mode).
 *   On PC it always runs (default config). On PS2, if the mode is
 *   advanced, another channel already covers rendering and this one does not run. */
void display_init_channel_min(void);

void InitializeDisplayAndThreads(void);

void ResetInitializationFlag(void);

void NoOperation(void);

bool WaitForInterruptStatus(SDL_Window* window);

void InitializePointers(void);

void ProcessCache(uint32_t param_1, uint32_t param_2);

void InitializeStruct(uintptr_t param_1);

void ProcessFunction(void);

void SetVSyncFlag(SDL_Window* window);

/* vsync_wait_first() is declared in engine/display_init.h. */


#endif // CORE_H
