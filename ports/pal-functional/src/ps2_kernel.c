#include "ps2_kernel.h"
#include "core/sce_compat.h"
#include "system.h"     // Required to access g_GraphicsSemaphore and g_GraphicsSemaphoreID
#include <SDL.h>
#include "graphics.h"   // Required to check target_fps
#include <string.h>
#include <stdlib.h>     // Required for atoi() in ee_atoi

int ee_memcmp(const void* ptr1, const void* ptr2, size_t num) {
	// On PC, the standard 'memcmp' from string.h is optimized at the level
	// of modern assembly (SSE/AVX) and produces exactly the same
	// mathematical result as the PS2 vector loop, natively and safely.
	return memcmp(ptr1, ptr2, num);
}

#if !defined(PLATFORM_PS2)
// Simulated internal control structure that emulates Sony's kernel semaphores on PC
typedef struct {
	s32 count;
	s32 max_count;
} PS2_Simulated_Semaphore;

// Static table of virtual semaphores for the portable port environment
static PS2_Simulated_Semaphore g_virtual_semaphores[16] = {
	{0, 1}, // ID 0: general / system
	{1, 1}, // ID 1: IO Lock Sema
	{1, 1}, // ID 2: IO Wait Sema
	{1, 1}  // ID 3: IO DMA Sema
};
#endif

/**
 * @brief Pauses execution of the current thread in the PS2 kernel...
 */
s32 sceWaitSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// On the real console this suspends the CPU through inline assembly:
	// __asm__ volatile("li $v0, 68 \n syscall");
	return 0;
#else
	// For the PC port, the wait is emulated passively and safely.

	// If the user set the FPS to '0' (unlimited), do not block.
	// Otherwise, honour the semaphore to synchronize the engine.
	if (g_GraphicsCanvasData.target_fps != 0.0f) {
		// In a real PS2 engine, sema_id usually maps to a specific semaphore.
		// Since this is the main graphics synchronization semaphore:
		if (g_GraphicsSemaphore != NULL) {
			SDL_SemWait(g_GraphicsSemaphore);
		}
	}

	return 0;
#endif
}

/**
 * @brief Signals a kernel semaphore to increment its count and wake waiting threads...
 * Original Ghidra address: syscall stub sector (0x42 MIPS syscall) (PAL)
 *
 * @param sema_id Unique identifier of the semaphore to signal.
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 sceSignalSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// On the real console this runs as MIPS inline assembly:
	// __asm__ volatile("li $v0, 66 \n syscall");
	return 0;
#else
	// For the modern PC port, the release is emulated actively.
	// If the ID is the graphics semaphore, notify the modern system
	if (sema_id == g_GraphicsSemaphoreID && g_GraphicsSemaphore != NULL) {
		SDL_SemPost(g_GraphicsSemaphore);
	}

	return 0;
#endif
}

/**
 * @brief Checks without blocking whether a semaphore is available in the PS2 kernel.
 * Original Ghidra address: syscall stub sector (0x45 MIPS syscall) (PAL)
 *
 * @param sema_id Unique identifier of the semaphore to query.
 * @return s32 The current semaphore count on success, or a negative value if it is locked.
 */
s32 scePollSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// On the real console this translates to the native instruction:
	// __asm__ volatile("li $v0, 69 \n syscall"); // 69 decimal is 0x45
	return 0;
#else
	// For the PC port, first check that the ID is within our virtual range
	if (sema_id == SYS_SEMAPHORE_INVALID || sema_id >= 16) {
		return -113; // Invalid ID per the Sony SDK
	}

	// Find which of our SDL2 semaphores the engine wants to inspect
	SDL_sem* target_sem = NULL;
	if (sema_id == g_GraphicsSemaphoreID)    target_sem = g_GraphicsSemaphore;
	else if (sema_id == g_RenderSemaphoreID_A) target_sem = g_RenderSemaphore_A;
	else if (sema_id == g_RenderSemaphoreID_B) target_sem = g_RenderSemaphore_B;

	if (target_sem != NULL) {
		// SDL_SemTryWait tries to take the semaphore immediately:
		// returns 0 if it was free (success), SDL_MUTEX_TIMEDOUT if it was busy.
		if (SDL_SemTryWait(target_sem) == 0) {
			return 0; // Success: the semaphore was free and was taken without blocking
		}
		else {
			return -489; // Official Sony code for "semaphore locked/closed" (signaled/wait state)
		}
	}

	// A secondary semaphore not linked yet returns success by default so the flow does not hang
	return 0;
#endif
}

/**
 * @brief Safely signals a semaphore from an interrupt context.
 * Original Ghidra address: internal hooks sector (MIPS syscall -67 / 0xFFFFFFFFFFFFFFBD) (PAL)
 *
 * @param sema_id Unique identifier of the semaphore assigned by the kernel to the channel.
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 iSignalSema(s32 sema_id) {
#if defined(PLATFORM_PS2)
	// On the real console this invokes the kernel hook through assembly:
	// __asm__ volatile("li $v0, -67 \n syscall");
	return 0;
#else
	// For the modern PC port, since there are no physical MIPS interrupts,
	// the release is emulated by calling the same active semaphore logic:
	if (sema_id == g_GraphicsSemaphoreID && g_GraphicsSemaphore != NULL) {
		SDL_SemPost(g_GraphicsSemaphore);
	}

	return 0;
#endif
}

int ee_atoi(const char* str) {
	if (str == NULL) return 0;

	// On native PC, 'atoi' performs the base-10 conversion identically
	// to the behaviour expected from the Emotion Engine's 32-bit truncation.
	return atoi(str);
}

/**
 * @brief Creates a kernel semaphore and returns its ID (sceCreateSema).
 * On PC the ID is a virtual handle from the sceSemaCreate stub; the semaphores
 * that really block are the SDL ones registered by Sys_InitGraphicsSemaphore.
 */
s32 sceCreateSema(void) {
	return (s32)sceSemaCreate(NULL);
}

/**
 * @brief Deletes a kernel semaphore (sceDeleteSema).
 * Virtual IDs own no PC resource, so deleting one only reports success.
 */
s32 sceDeleteSema(s32 sema_id) {
	return sema_id >= 0 ? 0 : -1;
}

/**
 * @brief Returns the ID of the calling thread (sceGetThreadId).
 * The engine compares it against a stored owner; on PC a stable non-zero
 * value derived from the SDL thread ID serves the same purpose.
 */
s32 sceGetThreadId(void) {
	return (s32)(SDL_ThreadID() & 0x7FFFFFFF);
}

/**
 * @brief Wakes a specific execution thread that was suspended in the PS2 kernel.
 * Original Ghidra address: syscall stub sector (0x33 MIPS syscall) (PAL)
 *
 * @param thread_id Unique identifier of the thread to reactivate.
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 sceWakeupThread(s32 thread_id) {
#if defined(PLATFORM_PS2)
	// On the real console it is invoked through inline assembly:
	// __asm__ volatile("li $v0, 51 \n syscall");
	return 0;
#else
	// For the PC port, thread wake-up is emulated passively.
	// Since modern operating system multitasking manages the background threads,
	// the wake-up signal is confirmed immediately to keep the flow clean:
	(void)thread_id;
	return 0;
#endif
}

/**
 * @brief Safely resumes a suspended execution thread from an interrupt context (ISR).
 * Original Ghidra address: internal hooks sector (MIPS syscall -52 / 0xFFFFFFFFFFFFFFCC) (PAL)
 *
 * @param thread_id Unique identifier of the thread to wake urgently.
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 iWakeupThread(s32 thread_id) {
#if defined(PLATFORM_PS2)
	// On the real console this invokes the kernel hook through assembly:
	// __asm__ volatile("li $v0, -52 \n syscall");
	return 0;
#else
	// For the modern PC port, resumption is emulated passively
	// by confirming the thread's immediate success signal:
	(void)thread_id;
	return 0;
#endif
}

/**
 * @brief Queries the current state of a specific thread in the PlayStation 2 kernel.
 * Original Ghidra address: syscall stub sector (0x30 MIPS syscall) (PAL)
 *
 * @param thread_id Unique identifier of the thread to inspect.
 * @param status_ptr Pointer to the structure where the kernel writes the thread state (sceThreadStatus).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 sceReferThreadStatus(s32 thread_id, void* status_ptr) {
#if defined(PLATFORM_PS2)
	// On the real console it is invoked through inline assembly:
	// __asm__ volatile("li $v0, 48 \n syscall");
	return 0;
#else
	// For the PC port, the query is emulated by returning immediate success (0).
	// This tells the Insomniac engine that the threads are running optimally

	// Avoid unused-parameter warnings in the modern PC compiler
	(void)thread_id;

	if (status_ptr != NULL) {
		// On a real PS2 the clean structure has a status field.
		// The value '1' typically represents the "RUN" state.
		// The first 4 bytes are safely filled with 1 in case the game checks that the thread is alive.
		*(s32*)status_ptr = 1;
	}

	return 0;
#endif
}

/**
 * @brief Schedules a time-based interrupt alarm in the PlayStation 2 kernel.
 * Original Ghidra address: syscall stub sector (252 MIPS syscall / 0xFC) (PAL)
 *
 * @param microseconds Exact time in microseconds before the hardware alarm fires.
 * @param alarm_callback Pointer to the function that handles the interrupt when the time expires.
 * @param callback_arg Optional control argument passed to the handler.
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg) {
#if defined(PLATFORM_PS2)
	// On the real console this runs through the inline instruction:
	// __asm__ volatile("li $v0, 252 \n syscall");
	return 0;
#else
	// For the PC port, since the modern operating system's file system
	// resolves transactions instantly without physical memory-card hardware waits,
	// the virtual alarm confirms the scheduling immediately:
	(void)microseconds;
	(void)alarm_callback;
	(void)callback_arg;
	return 0;
#endif
}

/**
 * @brief Voluntarily suspends execution of the active control thread in the PS2 kernel.
 * Original Ghidra address: syscall stub sector (0x32 MIPS syscall) (PAL)
 *
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 sceSleepThread(void) {
#if defined(PLATFORM_PS2)
	// On the real console it is invoked through inline assembly:
	// __asm__ volatile("li $v0, 50 \n syscall");
	return 0;
#else
	// For the PC port, suspension is emulated passively by returning immediate success.
	// Since the PC file system needs no physical 8MB hardware delays,
	// the virtual thread continues without freezing or slowing the engine's frame rate:
	return 0;
#endif
}

// 1. Simulation of the main menu function and the intro cycle
void game_main_menu_and_intro_loop(void) {
	// This is where the main game loop will eventually hook in
}

// 2. Simulation of Sony SDK registers
uint32_t sceSifGetReg(void) {
	return 1; // Return 1 to simulate that the IOP coprocessor answered the handshake
}

// 3. Simulation of hardware timer delays (native equivalent of nanosleep on Windows)
void nanosleep(void* req, void* rem) {
	// On Windows it is simulated very precisely with the native tools:
	// (It can be left empty or mapped to a sub-delay if the engine requires it)
	(void)req; (void)rem;
}

// 4. Decoder of custom HUD characters (for the extended text converter)
void custom_hud_glyph_decoder(void* param_1, void* param_2) {
	(void)param_1; (void)param_2;
}

// 5. Variables and stubs of the memory card subsystem (Memory Card - sceMc)
// The game checks the card state before loading the intro. They are created empty:
int g_sys_mc_is_bound_flag = 0;
int g_sys_mc_mutex_sema_id = -1;
int g_sys_mc_active_command_id = 0;
int g_sys_mc_channel_widget_handle = 0;
s32 g_sys_mc_result_metadata_val = 0;   /* metadata returned with the driver version query */

int sceMcGetInfo(int channel, int slot, void* type, void* free, void* format) {
	(void)channel; (void)slot; (void)type; (void)free; (void)format;
	return 0; // Return 0 (memory card not inserted, or passively simulated)
}

// ============================================================================
// FINAL PLATFORM STUBS TO COMPLETE LINKING (PC PORT)
// ============================================================================

// 1. Global variables of the IOP Input/Output (I/O) and sound system
int g_sys_io_wait_sema_id = -1;
int g_sys_io_queue_lock_flag = 0;
int g_sys_sound_channel_widget_handle = 0;
int g_sys_io_reconfig_flag = 0;

// 2. Physical interrupt control functions of the Emotion Engine chip (MIPS)
// On PC there are no direct hardware interrupt registers; return immediate success.
u32 Status = 0;
void DI(void) {}
void EI(void) {}
void SYNC(int type) { (void)type; }

// 3. Control variables of the hardware paging table (TLB) initializer
// The game clears the original TLB when starting RAM. On PC dummy indices are created:
int g_tlb_wired_index = 0;
int g_tlb_bound_index = 0;
int g_tlb_status_sync = 0;
int g_tlb_extra_flags = 0;

// 4. Synchronization function of the PS2 bus graphics packet
void ps2_sync(void) {
	// On a real PS2 this waited for the GIF/VIF bus to drain.
	// On PC, since processing is synchronous on the thread, it is instantaneous.
}

// ============================================================================
// FINAL INPUT/OUTPUT AND MEMORY CARD STUBS (PC PORT)
// ============================================================================

// 1. Read-state variables of the PS2 file system
int g_sys_io_is_ready_flag = 1; // 1 = the virtual reader is always ready on PC
int g_sys_io_lock_sema_id = -1;
int g_sys_io_dma_sema_id = -1;

// 2. File descriptors and sizes of the memory card subsystem (sceMc)
// The engine uses them to save/load games. They are initialized to safe zeros.
int g_sys_mc_read_fd = -1;
int g_sys_mc_read_size = 0;
int g_sys_mc_write_fd = -1;
void* g_sys_mc_write_src_ptr = NULL;
int g_sys_mc_write_size = 0;

// 3. Complementary stubs for memory card calls found in ps2_sif
int sceMcChdir(int channel, int slot, const char* path, char* current_dir) {
	(void)channel; (void)slot; (void)path; (void)current_dir;
	return 0;
}

int sceMcWriteExtended(int channel, int slot, const char* filename, void* buffer, int size) {
	(void)channel; (void)slot; (void)filename; (void)buffer; (void)size;
	return 0;
}

/**
 * @brief Registers an event or interrupt handler (callback) for a specific DMA controller (DMAC) channel.
 * Original Ghidra address: syscall stub sector (18 MIPS syscall / 0x12) (PAL)
 *
 * @param dma_channel Corresponding PS2 DMA channel (e.g. 5 for the SIF bus).
 * @param p_handler Pointer to the function that handles the interrupt.
 * @param arg Optional control argument passed to the handler.
 * @return s32 ID of the assigned slot or handler (positive on success, or negative on error).
 */
s32 sceAddDmacHandler(s32 dma_channel, void* p_handler, s32 arg) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v1, 18 \n syscall");
	return 0;
#else
	// For the native PC port, being a high-level software emulation,
	// the call is intercepted to confirm that the simulated channel was linked.
	// A fixed positive slot ID (e.g. 1) is returned to give the green light:
	(void)dma_channel;
	(void)p_handler;
	(void)arg;

	return 1;
#endif
}

/**
 * @brief Removes a DMA channel handler previously registered in the PS2 hardware controller (DMAC).
 * Original Ghidra address: syscall stub sector (19 MIPS syscall / 0x13) (PAL)
 *
 * @param dma_channel Corresponding DMA channel (param_1).
 * @param handler_id Numeric identifier or slot assigned to the handler to release (param_2).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 RemoveDmacHandler(s32 dma_channel, s32 handler_id) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v1, 19 \n syscall");
	return 0;
#else
	// For the native PC port, the modern operating system manages burst queues
	// transparently in nanoseconds, so success is confirmed immediately:
	(void)dma_channel;
	(void)handler_id;
	return 0;
#endif
}

/**
 * @brief Removes a physical interrupt handler previously registered in the Emotion Engine CPU.
 * Original Ghidra address: syscall stub sector (17 MIPS syscall / 0x11) (PAL)
 *
 * @param intc_id Identifier of the physical interrupt (param_1).
 * @param handler_id Identifier or slot of the handler to remove (param_2).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 RemoveIntcHandler(s32 intc_id, s32 handler_id) {
#if defined(PLATFORM_PS2)
	// On the real console this runs through the inline instruction:
	// __asm__ volatile("li $v1, 17 \n syscall");
	return 0;
#else
	// For the PC port, since the modern operating system manages the
	// background hardware natively, removal is emulated by returning immediate success:
	(void)intc_id;
	(void)handler_id;
	return 0;
#endif
}

/**
 * @brief Enables a specific hardware interrupt line in the Emotion Engine processor.
 * Original Ghidra address: syscall stub sector (20 MIPS syscall / 0x14) (PAL)
 *
 * @param intc_id Identifier of the physical interrupt to enable (param_1).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 _EnableIntc(s32 intc_id) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v1, 20 \n syscall");
	return 0;
#else
	// For the native PC port, the modern operating system manages the
	// background hardware natively, so success is confirmed immediately:
	(void)intc_id;
	return 0;
#endif
}

/**
 * @brief Disables a specific hardware interrupt line in the Emotion Engine processor.
 * Original Ghidra address: syscall stub sector (21 MIPS syscall / 0x15) (PAL)
 *
 * @param intc_id Identifier of the physical interrupt to disable (param_1).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 _DisableIntc(s32 intc_id) {
#if defined(PLATFORM_PS2)
	// On the real console this runs through the inline instruction:
	// __asm__ volatile("li $v1, 21 \n syscall");
	return 0;
#else
	// For the PC port, since the modern operating system manages the
	// background hardware natively, disabling is emulated by returning immediate success:
	(void)intc_id;
	return 0;
#endif
}

/**
 * @brief Enables a specific DMA controller (DMAC) channel in the PlayStation 2 kernel.
 * Original Ghidra address: syscall stub sector (22 MIPS syscall / 0x16) (PAL)
 *
 * @param dma_channel Identifier of the DMA channel to enable (param_1).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 sceEnableDmac(s32 dma_channel) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v1, 22 \n syscall");
	return 0;
#else
	// For the native PC port with SDL 2.32.2, the operating system manages the
	// background hardware immediately and automatically, so success is confirmed:
	(void)dma_channel;
	return 0;
#endif
}

/**
 * @brief Disables a specific DMA controller (DMAC) channel in the PlayStation 2 kernel.
 * Original Ghidra address: syscall stub sector (23 MIPS syscall / 0x17) (PAL)
 *
 * @param dma_channel Identifier of the DMA channel to disable (param_1).
 * @return s32 Kernel status code (0 on success, or a negative value on error).
 */
s32 _DisableDmac(s32 dma_channel) {
#if defined(PLATFORM_PS2)
	// On a real PlayStation 2 the inline assembly instruction runs:
	// __asm__ volatile("li $v1, 23 \n syscall");
	return 0;
#else
	// For the native PC port, since modern hardware needs no manual console
	// bus synchronization, the simulated shutdown is confirmed immediately:
	(void)dma_channel;
	return 0;
#endif
}
