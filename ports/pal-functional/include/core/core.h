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

/* Shared global state. 0x20c/0x168 were head
 * offsets in the SIF ring. On PC it is fixed to 0. */
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

void display_init_channel_b(void);
void InitializeDisplayAndThreads(void);

void ResetInitializationFlag(void);

void NoOperation(void);

bool WaitForInterruptStatus(SDL_Window* window);

bool kernel_system_sync_guard(void);
bool kernel_system_sync_release(void);

void InitializePointers(void);

void ProcessCache(uint param_1, uint param_2);

void InitializeStruct(int param_1);

void ProcessFunction(void);

void SetVSyncFlag(SDL_Window* window);

/* [CONFIRM] Engine VSync handshake: sets the VSync flag,
 *   enables the EE interrupt line (INTSTAT bit 2 = 0x4),
 *   waits for the FIRST vblank (bit 2 clear OR the handler writing to
 *   buffer[0]), acknowledges it (INTCONT) and returns the VSync
 *   handle/state (buffer[8], uStack_18 in Ghidra's C).
 *   PS2:  vent. 0x0010f000 (INTSTAT) + 0x001000000 (ack) +
 *         kernel_system_sync_guard/release (IRQ protection).
 *   PC:   there is no EE/INTSTAT. No-op, returns 0 (null handle).
 *   [MOD-PENDING] Once real rendering exists, this maps to the
 *         native vertical synchronization (glXSwapInterval / DWM /
 *         SDL_WaitEvent) of the active channel. */
int vsync_wait_first(void);


#endif // CORE_H
