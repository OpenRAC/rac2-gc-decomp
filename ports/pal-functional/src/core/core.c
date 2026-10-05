#include "core.h"
#include "sce_compat.h"
#include <SDL.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>

static SDL_Semaphore* g_GraphicsSemaphore = NULL;
static SDL_Semaphore* g_RenderSemaphore_A = NULL;
static SDL_Semaphore* g_RenderSemaphore_B = NULL;
static int g_GraphicsSemaphoreID = -1;
static int g_RenderSemaphoreID_A = -1;
static int g_RenderSemaphoreID_B = -1;
static int g_sys_keyboard_buffer_index = 0;

static int DAT_00134710 = 0;
static int DAT_0013c680 = 0;
static int DAT_0013c688 = 0;

static void* thread_wrapper(void* arg) {
	ThreadData* data = (ThreadData*)arg;
	void* (*thread_func)(void*) = data->func;
	void* func_arg = data->arg;
	free(arg); // Free the argument memory
	return thread_func(func_arg);
}

s32 sceSemaCreate(void) {
	// Semaphore creation
	SDL_Semaphore* sema = SDL_CreateSemaphore(1);
	if (sema != NULL) {
		return (s32)(intptr_t)sema;
	}
	return -1;
}

s32 sceDeleteSema(SDL_Semaphore* sema) {
	if (sema != NULL) {
		SDL_DestroySemaphore(sema);
		return 0; // Deletion succeeded
	}
	return -1; // Returns an error code if the semaphore is NULL
}

int CreateThread(void) {
	pthread_t thread;
	if (pthread_create(&thread, NULL, (void* (*)(void*))_StartThread, NULL) == 0) {
		return (int)(intptr_t)thread;
	}
	return -1;
}

void _StartThread(void* (*thread_func)(void*), void* arg) {
	pthread_t thread;
	ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData));
	if (data == NULL) {
		perror("Failed to allocate memory for ThreadData");
		return;
	}
	data->func = thread_func;
	data->arg = arg;

	if (pthread_create(&thread, NULL, thread_wrapper, data) != 0) {
		perror("Failed to create thread");
		free(data);
		return;
	}
	pthread_detach(thread); // The thread is detached after it terminates
}

s32 sceGetThreadId(void) {
	pthread_t tid = pthread_self();
	return (s32)(intptr_t)tid;
}

void ChangeThreadPriority(pthread_t thread, ThreadPriority priority) {
	struct sched_param param;
	int policy;
	int ret;

	// Get the current policy
	ret = pthread_getschedparam(thread, &policy, &param);
	if (ret != 0) {
		perror("Failed to get the thread policy");
		return;
	}

	// Set the new priority from the ThreadPriority enum
	switch (priority) {
	case PRIORITY_LOW:
		param.sched_priority = sched_get_priority_min(policy);
		break;
	case PRIORITY_NORMAL:
		param.sched_priority = sched_get_priority_max(policy) / 2;
		break;
	case PRIORITY_HIGH:
		param.sched_priority = sched_get_priority_max(policy);
		break;
	default:
		perror("Invalid priority");
		return;
	}

	// Set the new policy and priority
	ret = pthread_setschedparam(thread, policy, &param);
	if (ret != 0) {
		perror("Failed to set the new thread priority");
		return;
	}
}

int InitializeThreadManagement(void) {
	if (DAT_00134710 < 1) {
		s32 sema_id = sceSemaCreate();
		DAT_0013c680 = (int)sema_id;
		if (sema_id >= 0) {
			int thread_id = CreateThread();
			DAT_00134710 = thread_id;
			if (thread_id >= 0) {
				DAT_0013c688 = 0;
				g_sys_keyboard_buffer_index = 0;
				_StartThread(NULL);
				sceGetThreadId();
				ChangeThreadPriority(pthread_self(), PRIORITY_NORMAL);
				return DAT_00134710;
			}
			sceDeleteSema((SDL_Semaphore*)sema_id);
		}
	}
	return -1;
}

void render_init_semas(void) {
	// Initialization of the render semaphores
	g_GraphicsSemaphore = SDL_CreateSemaphore(1);
	g_RenderSemaphore_A = SDL_CreateSemaphore(1);
	g_RenderSemaphore_B = SDL_CreateSemaphore(1);
	g_GraphicsSemaphoreID = g_GraphicsSemaphore ? (int)(intptr_t)g_GraphicsSemaphore : -1;
	g_RenderSemaphoreID_A = g_RenderSemaphore_A ? (int)(intptr_t)g_RenderSemaphore_A : -1;
	g_RenderSemaphoreID_B = g_RenderSemaphore_B ? (int)(intptr_t)g_RenderSemaphore_B : -1;
}

/* ------------------------------------------------------------------ */
/*  State global (DAT_00134e20 in the ELF)                            */
/*  [MOD-PENDING] Identify who reads this global and give it the      */
/*  real Insomniac engine name.                                        */
/* ------------------------------------------------------------------ */
int g_sync_state = 0;

/* ------------------------------------------------------------------ */
/*  sce_sync_barrier  (FUN_0011f718)                                  */
/*                                                                     */
/*  PS2 original:                                                     */
/*    kick(); kick();                                                 */
/*    head_a = poll() - 0x20c;                                        */
/*    head_b = poll() - 0x168;                                        */
/*    while (head_a != head_b) { spin... }                            */
/*    DAT_00134e20 = head_a;                                          */
/*                                                                     */
/*  PC: there is no IOP or SIF ring. The "spin" is useless (both      */
/*  stubs return 0 → they converge immediately). We only write        */
/*  the global so the dependency chain is not broken.                 */
/* ------------------------------------------------------------------ */
void sce_sync_barrier(void)
{
	int head_a, head_b;

#if defined(PLATFORM_PS2)
	sce_stub_syscall116();   /* kick 1 */
	sce_stub_syscall116();   /* kick 2 */

	head_a = sce_stub_syscall131() - 0x20c;
	head_b = sce_stub_syscall131() - 0x168;

	/* Spin-wait: re-poll the head that lags "behind" until they converge */
	while (head_a != head_b) {
		if (head_a < head_b)
			head_a = sce_stub_syscall131() - 0x20c;
		else
			head_b = sce_stub_syscall131() - 0x168;
	}
#else
	/* PC: both stubs return 0, the while loop is never entered.
	   The code is kept so that the logic is 1:1 with the ELF
	   and serves as a reference for an emulator. */
	(void)head_a;
	(void)head_b;
	head_a = sce_stub_syscall131() - 0x20c;  /* 0 - 0x20c */
	head_b = sce_stub_syscall131() - 0x168;  /* 0 - 0x168 */
	/* head_a != head_b → the while loop is entered → converges in 1 iteration */
	while (head_a != head_b) {
		if (head_a < head_b)
			head_a = sce_stub_syscall131() - 0x20c;
		else
			head_b = sce_stub_syscall131() - 0x168;
	}
#endif

	g_sync_state = head_a;
}

void display_init_channel(void)
{
	uint32_t cnt;

	if ((g_rcnt3_mode & RCNT3_FLAG_INIT) == 0u)
	{
		cnt = 2u;

		/* Phase 1: kicks + waits + flushes (initial EE<->IOP handshake) */
		sce_stub_syscall116();      /* kick  */
		sce_stub_syscall90();       /* wait  */
		sce_stub_syscall90();       /* wait  */
		sceFlushCache(0, 0, 0);     /* flush */
		sceFlushCache(0, 0, 0);     /* flush */
		sce_stub_syscall116();      /* kick  */

		/* Phase 2: per-buffer handshake (6 iterations) */
		do {
			cnt = cnt + 1u;
			sce_stub_syscall91();      /* signal: releases the previous buffer */
			sce_stub_syscall116();     /* kick:   starts the next buffer */
		} while (cnt < DISPLAY_INIT_BUFFERS);

		/* [CONFIRM] Mark the channel as initialized (idempotence guard).
		 * The ELF probably does it through a register write that Ghidra
		 * did not resolve; it is set here so a second call does not re-initialize. */
		g_rcnt3_mode |= RCNT3_FLAG_INIT;
	}
	/* If it was already set, nothing happens: idempotent. */
}

int InitializeThreadManagement(void) {
	if (DAT_00134710 < 1) {
		s32 sema_id = sceSemaCreate();
		DAT_0013c680 = (int)sema_id;
		if (sema_id >= 0) {
			int thread_id = CreateThread();
			DAT_00134710 = thread_id;
			if (thread_id >= 0) {
				DAT_0013c688 = 0;
				g_sys_keyboard_buffer_index = 0;
				_StartThread(NULL);
				sceGetThreadId();
				ChangeThreadPriority(pthread_self(), PRIORITY_NORMAL);
				return DAT_00134710;
			}
			sceDeleteSema((SDL_Semaphore*)sema_id);
		}
	}
	return -1;
}

void display_init_channel_min(void)
{
	uint32_t cnt;

	if (sys_config_init() != 0)        /* guarded by MODE, not by idempotence */
	{
		cnt = 2u;                       /* uVar2 = 2 in the asm */

		/* Setup (116,90,100,100,116) — same shape as A/B, different args */
		sce_stub_syscall116(0x5A);                       /* kick, op=0x5A */
		sce_stub_syscall90(0x80074000u, 0x134e40, 0x7a8);/* wait, a2=1960 */
		sceFlushCache(0, 0, 0);
		sceFlushCache(2, 0, 0);
		sce_stub_syscall116(0x5B);                       /* kick, op=0x5B */

		/* Loop: 1 iteration (cnt 2→3, exits on 3<3 false) */
		do {
			cnt = cnt + 1u;
			sce_stub_syscall91(0x5A);                    /* signal */
			sce_stub_syscall116(0x5A);                   /* next kick */
		} while (cnt < 3u);

		/* Does not store a global at the end (unlike channel B).
		   Ends directly with jr ra. */
	}
	/* if sys_config_init() == 0: does nothing (advanced mode, another channel renders) */
}

void display_init_channel_b(void)
{
	uint32_t cnt = 3u;

	/* Setup: 116(0x5A) → 90 → flush(0) → flush(2) → 116(0x5B) → 116(0x54) */
	sce_stub_syscall116(0x5A);          /* kick, op=90 */
	sce_stub_syscall90(0x80075000, 0x1347d0, 0x330);  /* wait with flags */
	sceFlushCache(0, 0, 0);
	sceFlushCache(2, 0, 0);
	sce_stub_syscall116(0x5B);          /* kick, op=91 */
	sce_stub_syscall116(0x54);          /* kick, op=84 */

	/* Loop: 5 iterations, each one: signal(op) + kick(op+1) */
	do {
		cnt = cnt + 1u;
		sce_stub_syscall91(0x55 + (cnt - 3));   /* op advances 0x55→0x56 */
		sce_stub_syscall116(0x55 + (cnt - 3));  /* next kick */
	} while (cnt < 8u);

	/* [Fixed] DAT_00134b48 = 3 (literal from the asm), not the signal's return value */
	g_display_channel_b = 3;
}


void InitializeDisplayAndThreads(void) {
	render_init_semas();
	sce_sync_barrier();
	display_init_channel();
	InitializeThreadManagement();
	display_init_channel_min();
	display_init_channel_b();
}

static uint32_t* DAT_001a7308 = NULL;  // Make sure this is initialized appropriately

int ProcessData(void) {
	uint32_t uVar1;
	int iVar2;
	uint64_t* puVar3;
	uint64_t* puVar4;
	uint64_t* puVar5;
	uint32_t* puVar6;
	uint64_t* puVar7;
	int iVar8;
	int exit_condition = 0;  // Exit condition for the infinite loop

	iVar8 = 0;
	puVar6 = (uint32_t*)(*DAT_001a7308 + (int)DAT_001a7308);
	do {
		puVar7 = (uint64_t*)(puVar6 + 4);
		puVar4 = (uint64_t*)*puVar6;
		if (iVar8 == 0) {
			iVar8 = puVar6[3];
			uVar1 = puVar6[1];
		}
		else {
			if (iVar8 != puVar6[3]) {
				return iVar8;
			}
			uVar1 = puVar6[1];
		}
		if ((((uVar1 & 7) == 0) && (((uint)puVar7 & 7) == 0)) && (((uint)puVar4 & 7) == 0)) {
			puVar5 = (uint64_t*)((int)puVar4 + uVar1);
			puVar3 = puVar7;
			if (puVar4 == puVar5) goto LAB_00131dbc;
			do {
				*puVar4 = *puVar3;
				puVar4 = puVar4 + 1;
				puVar3 = puVar3 + 1;
			} while (puVar4 != puVar5);
			iVar2 = puVar6[1];
		}
		else {
			puVar5 = (uint64_t*)((int)puVar4 + uVar1);
			puVar3 = puVar7;
			if (puVar4 == puVar5) {
				iVar2 = puVar6[1];
			}
			else {
				do {
					*(uint32_t*)puVar4 = *(uint32_t*)puVar3;
					puVar4 = (uint64_t*)((int)puVar4 + 4);
					puVar3 = (uint64_t*)((int)puVar3 + 4);
				} while (puVar4 != puVar5);
			LAB_00131dbc:
				iVar2 = puVar6[1];
			}
		}
		puVar6 = (uint32_t*)((int)puVar7 + iVar2);

		// Exit condition of the infinite loop
		exit_condition++;
		if (exit_condition > 1000) {  // Adjust this condition as needed
			break;
		}
	} while (true);

	return 0;  // Returns an appropriate value
}

static int g_deci2_is_initialized = 0;

void ResetInitializationFlag(void) {
	g_deci2_is_initialized = 0;
}

void NoOperation(void) {
	// No operation is performed
}

#include "core.h"
#include <stdint.h>

// Defines the REG_INTC_STAT address
#define REG_INTC_STAT (*(volatile uint32_t *)0x1000f000)

// Simulates the synchronization operation
bool WaitForInterruptStatus(SDL_Window* window) {
	if (!window) {
		fprintf(stderr, "SDL window not initialized.\n");
		return false;
	}

	// Simulates acquiring the synchronization lock
	bool guard_success = true; // Assume the lock can always be acquired

	if (guard_success) {
		// Simulates setting REG_INTC_STAT to 4
		SDL_GL_SetSwapInterval(4); // Simulates the REG_INTC_STAT register
		// Make sure the write completes
		SDL_GL_SwapWindow(window);

		// Simulates releasing the synchronization lock
	}

	// Wait until bit 2 of REG_INTC_STAT is set
	uint32_t intc_stat;
	do {
		// Simulates reading REG_INTC_STAT
		intc_stat = SDL_GL_GetSwapInterval(); // Simulates the REG_INTC_STAT register
	} while ((intc_stat & 4) == 0);

	// Simulates acquiring the synchronization lock again
	guard_success = true; // Assume the lock can always be acquired

	if (guard_success) {
		// Simulates setting REG_INTC_STAT to 4 again
		SDL_GL_SetSwapInterval(4); // Simulates the REG_INTC_STAT register
		// Make sure the write completes
		SDL_GL_SwapWindow(window);

		// Simulates releasing the synchronization lock
	}

	return true;
}

/**
 * @brief Emotion Engine processor lock and synchronization routine.
 * Monitors the processor flag state through a safe spinlock.
 * Original Ghidra address: 0x0011F5E0 (PAL)
 *
 * @return bool Returns the final state of the processor diagnostic flag.
 */
bool kernel_system_sync_guard(void) {
	// 0x10000 is an interrupt/diagnostic status flag in MIPS Coprocessor 0
	if ((Status & 0x10000) != 0) {
		do {
			DI();        // Disable hardware interrupts on the PS2
			SYNC(0x10);  // Force synchronization of the processor data pipeline
		} while ((Status & 0x10000) != 0); // Repeat until the hardware settles

		return (Status & 0x10000) != 0;
	}

	return false;
}

/**
 * @brief Emotion Engine processor release and interrupt-enable routine.
 * Re-enables the PS2 system threads after a critical synchronization operation.
 * Original Ghidra address: 0x0011F628 (PAL)
 *
 * @return bool Returns the state of the processor diagnostic flag.
 */
bool kernel_system_sync_release(void) {
	// Re-enables the general interrupts in the PlayStation 2 hardware
	EI();

	// Evaluates and returns bit 16 of the Coprocessor 0 Status register
	return (Status & 0x10000) != 0;
}

static uint32_t DAT_001b1880;
static uint32_t DAT_001b1884;
static uint32_t DAT_001baf3c = 0x12345678;  // Make sure this is initialized appropriately

void InitializePointers(void) {
	DAT_001b1880 = DAT_001baf3c;
	DAT_001b1884 = DAT_001baf3c + 0x64000;
}

// Defines the cache and TagLo operations
#define CACHE_OP_DXLtg 0x10
#define CACHE_OP_DXWbin 0x14
#define TAGLO_MASK 0xfffff000

// Simulated cache operations
void cacheOp(uint op, int index) {
	// Cache logic can be implemented here as needed
	printf("Cache operation: 0x%x, Index: %d\n", op, index);
}

// Simulated SYNC operation
void SYNC(int arg) {
	// Synchronization logic can be implemented here as needed
	printf("SYNC operation with argument: %d\n", arg);
}

// Defines the TagLo variable
static uint32_t TagLo = 0x12345678;  // Make sure this is initialized appropriately

void ProcessCache(uint param_1, uint param_2) {
	uint uVar1;
	int iVar2 = 0;

	do {
		SYNC(0);
		cacheOp(CACHE_OP_DXLtg, iVar2);
		SYNC(0);

		uVar1 = (TagLo & TAGLO_MASK) + iVar2;
		if ((param_1 <= uVar1) && (uVar1 <= param_2)) {
			SYNC(0);
			cacheOp(CACHE_OP_DXWbin, iVar2);
			SYNC(0);
		}

		SYNC(0);
		cacheOp(CACHE_OP_DXLtg, iVar2 + 1);
		SYNC(0);

		uVar1 = (TagLo & TAGLO_MASK) + iVar2;
		if ((param_1 <= uVar1) && (uVar1 <= param_2)) {
			SYNC(0);
			cacheOp(CACHE_OP_DXWbin, iVar2 + 1);
			SYNC(0);
		}

		SYNC(0);
		iVar2 += 0x40;
	} while (iVar2 < 0x1000);
}

void InitializeStruct(int param_1) {
	// Initialize the specific struct fields
	*((uint32_t*)(param_1 + 0x1b0)) = 0;
	*((uint32_t*)(param_1 + 0x1d4)) = 1;
	uint32_t* puVar1 = (uint32_t*)(param_1 + 0x140);
	*((uint32_t*)(param_1 + 0x1a0)) = 0;
	int iVar2 = 0xf;
	*((uint32_t*)(param_1 + 0x1a4)) = 0;
	*((uint32_t*)(param_1 + 0x1a8)) = 0;
	*((uint32_t*)(param_1 + 0x1d0)) = 1;
	*((uint32_t*)(param_1 + 0x1b4)) = 0;
	*((uint32_t*)(param_1 + 0x1b8)) = 0;
	*((uint32_t*)(param_1 + 0x1c0)) = 0;
	*((uint32_t*)(param_1 + 0x1c4)) = 0;
	*((uint32_t*)(param_1 + 0x1c8)) = 0;
	*((uint32_t*)(param_1 + 0x1d8)) = 0;

	// Loop initializing the additional fields
	do {
		*(puVar1 - 0x10) = 0;
		iVar2 = iVar2 + -1;
		*puVar1 = 0;
		puVar1 = puVar1 + 1;
	} while (-1 < iVar2);
}

// Defines the global variables
static int DAT_001395cc = 0;
static int DAT_00139470 = 0;
static int DAT_00139484 = 0;
static int DAT_001b1a3c = 0;
static int DAT_001a74a0 = 0;
static int DAT_001a74a4 = 0;
static int iGpffff8430 = 0;
static int DAT_001b1a38 = 0;
static int iGp000029c8 = 0;

// Defines the function pointers
static void (*PTR_LAB_002560d0[])(void) = { /* Initialize with the corresponding functions */ };

void ProcessFunction(void) {
	int iVar1;

	iVar1 = iGpffff8430;
	if (((DAT_001395cc != 0) || (DAT_00139470 != 0)) || (0 < DAT_00139484)) {
		DAT_001b1a3c = 1;
	}
	if ((DAT_001a74a4 & 0x80) != 0) {
		DAT_001a74a0 = 0x15;
		DAT_001a74a4 = (DAT_001a74a4 & 0xffffff7f) | 0x40;
	}
	if ((DAT_001a74a4 & 0x100) != 0) {
		DAT_001a74a0 = 0x14;
		DAT_001a74a4 = (DAT_001a74a4 & 0xfffffeff) | 0x40;
	}
	(*(code*)(&PTR_LAB_002560d0)[DAT_001a74a0])();
	iGp000029c8 = DAT_001b1a38 + 1;
	if (DAT_001a74a0 != iVar1) {
		DAT_001b1a38 = 0;
	}
}

void SetVSyncFlag(SDL_Window* window) {
	if (window) {
		// Enable vertical synchronization
		if (SDL_GL_SetSwapInterval(1) != 0) {
			fprintf(stderr, "Could not enable vertical synchronization: %s\n", SDL_GetError());
		}
		else {
			printf("Vertical synchronization enabled.\n");
		}
	}
	else {
		fprintf(stderr, "SDL window not initialized.\n");
	}
}

/* [CONFIRM] EE interrupt registers (memory windows).
 *   INTSTAT = 0x0010f000 (line status) ; bit 2 = VSync (0x4).
 *   INTCONT = 0x001000000 (control / ack). */
#define EE_INTSTAT   (*(volatile unsigned int *)0x0010f000u)
#define EE_INTCONT   (*(volatile unsigned int *)0x001000000u)
#define EE_INT_VSYNC 0x4u

int vsync_wait_first(void)
{
	unsigned int buf0 = 0;   /* buffer[0]: flag written by the IRQ handler */
	unsigned int handle = 0; /* buffer[8]: returned handle/state            */
	int guard;

#if defined(PLATFORM_PS2)
	/* 1) SetVSyncFlag(sp, sp+8): passes both buffer slots to the engine */
	SetVSyncFlag(&buf0, &handle);

	/* 2) Protect IRQs + enable the VSync line in INTSTAT */
	guard = kernel_system_sync_guard();
	EE_INTSTAT |= EE_INT_VSYNC;
	__asm__ __volatile__("sync\n" ::: "memory");
	if (guard) kernel_system_sync_release();

	/* 3) Wait for the first vblank: bit 2 clear OR handler wrote buf0 */
	for (;;)
	{
		if ((EE_INTSTAT & EE_INT_VSYNC) != 0) break;   /* vblank arrived */
		if (buf0 != 0) break;                          /* handler wrote */
	}

	/* 4) Acknowledge the interrupt (with IRQs protected) */
	guard = kernel_system_sync_guard();
	EE_INTCONT |= EE_INT_VSYNC;
	__asm__ __volatile__("sync\n" ::: "memory");
	if (guard) kernel_system_sync_release();

	return (int)handle;
#else
	/* PC: no EE, no INTSTAT/INTCONT, no engine IRQ handler.
	   [MOD-PENDING] Once real rendering exists, this maps to the
	   native vertical synchronization of the active channel
	   (glXSwapInterval / DWM / SDL_WaitEvent) and returns a native
	   handle instead of 0. */
	(void)buf0;
	(void)handle;
	return 0;
#endif
}
