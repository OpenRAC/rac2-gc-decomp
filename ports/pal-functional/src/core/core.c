#include "core.h"
#include "sce_compat.h"
#include "ps2_kernel.h"
#include "engine/motor_regs.h"
#include "engine/render_init.h"
#include <SDL.h>
#include <pthread.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>

static int g_sys_keyboard_buffer_index = 0;

static int DAT_00134710 = 0;
static int DAT_0013c680 = 0;
static int DAT_0013c688 = 0;

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

/* ------------------------------------------------------------------ */
/*  State global (DAT_00134e20 in the ELF)                            */
/*  [MOD-PENDING] Identify who reads this global and give it the      */
/*  real Insomniac engine name.                                        */
/* ------------------------------------------------------------------ */
int g_sync_state = 0;
uint32_t g_rcnt3_mode = 0;   /* display channel register, see engine/motor_regs.h */

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
	sceSifCheckM_S(0);   /* kick 1 */
	sceSifCheckM_S(0);   /* kick 2 */

	head_a = sceSifSetM_S() - 0x20c;
	head_b = sceSifSetM_S() - 0x168;

	/* Spin-wait: re-poll the head that lags "behind" until they converge */
	while (head_a != head_b) {
		if (head_a < head_b)
			head_a = sceSifSetM_S() - 0x20c;
		else
			head_b = sceSifSetM_S() - 0x168;
	}
#else
	/* PC: both stubs return 0, the while loop is never entered.
	   The code is kept so that the logic is 1:1 with the ELF
	   and serves as a reference for an emulator. */
	(void)head_a;
	(void)head_b;
	head_a = sceSifSetM_S() - 0x20c;  /* 0 - 0x20c */
	head_b = sceSifSetM_S() - 0x168;  /* 0 - 0x168 */
	/* head_a != head_b → the while loop is entered → converges in 1 iteration */
	while (head_a != head_b) {
		if (head_a < head_b)
			head_a = sceSifSetM_S() - 0x20c;
		else
			head_b = sceSifSetM_S() - 0x168;
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
		sceSifCheckM_S(0);      /* kick  */
		sceSifSetRpcQueue(0, 0, 0);       /* wait  */
		sceSifSetRpcQueue(0, 0, 0);       /* wait  */
		sceFlushCache(0, 0, 0);     /* flush */
		sceFlushCache(0, 0, 0);     /* flush */
		sceSifCheckM_S(0);      /* kick  */

		/* Phase 2: per-buffer handshake (6 iterations) */
		do {
			cnt = cnt + 1u;
			sceSifInitRpc(0);      /* signal: releases the previous buffer */
			sceSifCheckM_S(0);     /* kick:   starts the next buffer */
		} while (cnt < DISPLAY_INIT_BUFFERS);

		/* [CONFIRM] Mark the channel as initialized (idempotence guard).
		 * The ELF probably does it through a register write that Ghidra
		 * did not resolve; it is set here so a second call does not re-initialize. */
		g_rcnt3_mode |= RCNT3_FLAG_INIT;
	}
	/* If it was already set, nothing happens: idempotent. */
}

int InitializeThreadManagement(void) {
	/* PS2: create the keyboard semaphore, create and start the keyboard
	 * thread and raise its priority. The thread entry was not recovered
	 * (the draft passed NULL), so on PC only the bookkeeping is kept and the
	 * current thread is recorded as the owner. */
	if (DAT_00134710 < 1) {
		s32 sema_id = sceCreateSema();
		DAT_0013c680 = (int)sema_id;
		if (sema_id >= 0) {
			DAT_00134710 = sceGetThreadId();
			DAT_0013c688 = 0;
			g_sys_keyboard_buffer_index = 0;
			ChangeThreadPriority(pthread_self(), PRIORITY_NORMAL);
			return DAT_00134710;
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
		sceSifCheckM_S(0x5A);                       /* kick, op=0x5A */
		sceSifSetRpcQueue((int)0x80074000u, 0x134e40, 0x7a8);/* wait, a2=1960 */
		sceFlushCache(0, 0, 0);
		sceFlushCache(2, 0, 0);
		sceSifCheckM_S(0x5B);                       /* kick, op=0x5B */

		/* Loop: 1 iteration (cnt 2→3, exits on 3<3 false) */
		do {
			cnt = cnt + 1u;
			sceSifInitRpc(0x5A);                    /* signal */
			sceSifCheckM_S(0x5A);                   /* next kick */
		} while (cnt < 3u);

		/* Does not store a global at the end (unlike channel B).
		   Ends directly with jr ra. */
	}
	/* if sys_config_init() == 0: does nothing (advanced mode, another channel renders) */
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
	if (DAT_001a7308 == NULL) return 0;   /* no PS2 transfer list on PC */
	puVar6 = (uint32_t*)((uintptr_t)DAT_001a7308 + *DAT_001a7308);
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
		if ((((uVar1 & 7) == 0) && (((uintptr_t)puVar7 & 7) == 0)) && (((uintptr_t)puVar4 & 7) == 0)) {
			puVar5 = (uint64_t*)((uintptr_t)puVar4 + uVar1);
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
			puVar5 = (uint64_t*)((uintptr_t)puVar4 + uVar1);
			puVar3 = puVar7;
			if (puVar4 == puVar5) {
				iVar2 = puVar6[1];
			}
			else {
				do {
					*(uint32_t*)puVar4 = *(uint32_t*)puVar3;
					puVar4 = (uint64_t*)((uintptr_t)puVar4 + 4);
					puVar3 = (uint64_t*)((uintptr_t)puVar3 + 4);
				} while (puVar4 != puVar5);
			LAB_00131dbc:
				iVar2 = puVar6[1];
			}
		}
		puVar6 = (uint32_t*)((uintptr_t)puVar7 + iVar2);

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
/**
 * @brief Emotion Engine processor release and interrupt-enable routine.
 * Re-enables the PS2 system threads after a critical synchronization operation.
 * Original Ghidra address: 0x0011F628 (PAL)
 *
 * @return bool Returns the state of the processor diagnostic flag.
 */
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
void cacheOp(uint32_t op, int index) {
	// MIPS cache operations have no PC equivalent: the hardware keeps caches coherent
	(void)op;
	(void)index;
}

// Simulated SYNC operation
// Defines the TagLo variable
static uint32_t TagLo = 0x12345678;  // Make sure this is initialized appropriately

void ProcessCache(uint32_t param_1, uint32_t param_2) {
	uint32_t uVar1;
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

void InitializeStruct(uintptr_t param_1) {
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
static void (*PTR_LAB_002560d0[1])(void) = { NULL };   /* the recovered table is not available */

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
	/* The state handler table was not recovered; an empty table dispatches nothing. */
	if ((size_t)DAT_001a74a0 < sizeof(PTR_LAB_002560d0) / sizeof(PTR_LAB_002560d0[0]))
		PTR_LAB_002560d0[DAT_001a74a0]();
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

