#include "core/ee_memory.h"
#include "types.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h> // Required for timespec structures on modern systems
#include <assert.h>
#include <stdbool.h>
#include "ps2_kernel.h"
#include "core/sce_compat.h"

int sys_sif_rpc_init_client(void);   /* defined further below */

// Definitions of the static audio and interrupt offsets in PS2 RAM
#define IO_WAIT_SEMA_ID             (*(s32*)EE_ADDR(0x0013642C))
#define CURRENT_AUDIO_CMD_ID        (*(s32*)EE_ADDR(0x00136418))
#define AUDIO_SESSION_STATUS_FLAG   (*(s32*)EE_ADDR(0x00136448))
#define DEBUG_NET_LOG_LEVEL         (*(s32*)EE_ADDR(0x00136410))

// Definition of the global stage-transition variables in PS2 RAM
#define BOOT_INTRO_DELAY_STATE       (*(s32*)EE_ADDR(0x001A642C))
#define BOOT_INTRO_TRANSITION_FLAG   (*(u8*)EE_ADDR(0x001A642E))
#if defined(PLATFORM_PS2)
#define NEXT_GAME_STAGE_CALLBACK     (*(void**)0x001A6440)
#else
/* A code pointer: on PC it holds a host function installed by PC code, never the
 * MIPS address the boot executable's data may hold at 0x001A6440. */
static void* g_next_game_stage_callback = NULL;
#define NEXT_GAME_STAGE_CALLBACK     g_next_game_stage_callback
#endif
#define NEXT_GAME_STAGE_ARGUMENT     (*(u32*)EE_ADDR(0x001A6444))

// Definition of the Memory Card state variables in PS2 RAM
#define MC_ACTIVE_COMMAND_ID        (*(s32*)EE_ADDR(0x00137EE8))
#define MC_CHANNEL_WIDGET_HANDLE    (*(u32*)EE_ADDR(0x00141B80))
#define MC_RESULT_METADATA_VAL      (*(u32*)EE_ADDR(0x00143140))

// Definition of the global Memory Card driver version variables on the PS2
#define MC_MUTEX_SEMA_ID            (*(s32*)EE_ADDR(0x00137EEC))
#define MC_IS_BOUND_FLAG            (*(s32*)EE_ADDR(0x00141BA4))
#define MC_MCSERV_VERSION           (*(u32*)EE_ADDR(0x00143144))
#define MC_MCMAN_VERSION            (*(u32*)EE_ADDR(0x00143148))

// Definitions of the mapped physical registers and global buffers of the Memory Card
#define MC_SLOT_INPUT_BUFFER_PTR    (*(u32*)EE_ADDR(0x00141C00))

// Definition of the open-descriptor variables mapped in PS2 RAM
#define MC_OPEN_PATH_PTR            (*(u32*)EE_ADDR(0x00141C10))
#define MC_OPEN_FLAGS_MASK          (*(u32*)EE_ADDR(0x00141C14))

// References to the external global callback tables already defined in the repository
extern u32 g_sys_sif_general_callback_table;
extern u32 g_sys_sif_system_callback_table;

// References to the consolidated helpers and infrastructure of ps2_kernel.c and the boot
s32  scePollSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);

// Make sure the prototypes or headers above declare it exactly like this:
bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

extern s32 g_sys_mc_is_bound_flag;
extern s32 g_sys_mc_mutex_sema_id;
extern s32 g_sys_mc_active_command_id;
extern u32 g_sys_mc_channel_widget_handle;

/**
 * @brief Sends the open command of a Memory Card file (command 4) to the hardware bus.
 * Configures the path, slot and binary control flags, applying mutual exclusion through semaphores.
 * Original Ghidra address: 0x00127720 (PAL)
 *
 * @param slot_index Card slot to query (0 = slot 1, 1 = slot 2) (param_1).
 * @param p_file_path Text string with the formatted path of the save file (param_2).
 * @param open_flags Bit mask with the SDK read/write access modes (param_3).
 * @return s32 Status code (0 if the bus accepted the command, negative values on error).
 */
s32 sceMcOpen(u32 slot_index, const char* p_file_path, u32 open_flags) {
	s32 status_code;

	// 1. Check that the Memory Card subsystem is formally up
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protect the bus with a non-blocking poll of the channel semaphore
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	if (sema_status < 0) {
		return -200;
	}

	// 3. Write the open parameters contiguously into the static data section
	extern u32 g_sys_mc_slot_input_buffer_val;
	*(u32*)0x00141C00 = slot_index; // Reuses the slot ID buffer
	MC_OPEN_PATH_PTR = (u32)(long)p_file_path;
	MC_OPEN_FLAGS_MASK = open_flags;

	// Dispatch the order with the command 4 burst (priority synchronous = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		4, 1, 0x141C00, 0x30, 0x143140, 4, 0, 0
	);

	// 4. If the SIF bus injection succeeded, sign the active command in RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 4;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}


// References to the consolidated helpers and infrastructure
s32  scePollSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);

// External global variables from the boot module
extern s32 g_sys_mc_is_bound_flag;
extern s32 g_sys_mc_mutex_sema_id;
extern s32 g_sys_mc_active_command_id;
extern u32 g_sys_mc_channel_widget_handle;

// References to the interlinked kernel and HUD components and infrastructure
s32  sceCreateSema(void);
s32  sceWaitSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
u32  sys_mc_io_sync_command_guard(long command_type, long p_out_cmd_ptr, long p_out_meta_ptr);
s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

extern s32 g_sys_mc_result_metadata_val;

/**
 * @brief Fully initializes the physical Memory Card subsystem and validates the versions of the IOP IRX modules.
 * Opens control session 0x80000400 and applies mutual-exclusion barriers to secure the reader.
 * Original Ghidra address: 0x00127348 (PAL)
 *
 * @return s32 General status code of the save system (0 or metadata on success, negative values on error).
 */
s32 sys_mc_init_subsystem(void) {
	s32 status_code;

	// 1. Initialize and reserve the mutual-exclusion semaphore for the card slots
	if (MC_MUTEX_SEMA_ID < 0) {
		MC_MUTEX_SEMA_ID = sceCreateSema();
	}

	// Make sure the physical channel is free and idle before reconfiguring
	sys_mc_io_sync_command_guard(0, 0, 0);

#if defined(PLATFORM_PS2)
	sceWaitSema(MC_MUTEX_SEMA_ID);
#endif

	// Initialize the RPC client environment
	sys_sif_rpc_init_client();

	// 2. LINK WAIT LOOP: opens the card control session (command 0x80000400)
	extern u32 g_sys_mc_channel_widget_handle;
	while (1) {
		while (1) {
			s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_mc_channel_widget_handle, 0x80000400, 0);

			if (session_status >= 0) {
				break;
			}

			// If the hardware link with the IOP fails, emit the panic log
			boot_txt_render_extended_string((const u8*)"bind error libmc \n", 0, 0, 0, 0, 0, 0, 0);

			// Safety infinite loop on the original console in case of a serious hardware error
#if defined(PLATFORM_PS2)
			while (1) {}
#else
			return -1; // Safe escape for the portable PC environment
#endif
		}

		if (MC_IS_BOUND_FLAG != 0) {
			break;
		}

		s32 delay_counter = 0x100000;
		while (delay_counter != 0) { delay_counter--; }
	}

	// 3. INTEGRITY QUERY: requests the save driver versions from the IOP (command 0xFE)
	s32 transaction_status = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		0xFE, 0, 0x141C00, 0x30, 0x143140, 0x0C, 0, 0
	);

#if defined(PLATFORM_PS2)
	sceSignalSema(MC_MUTEX_SEMA_ID);
#endif

	if (transaction_status < 0) {
		MC_IS_BOUND_FLAG = 0;
		status_code = transaction_status - 100;
	}
	// 4. SONY COMPILER VERSION VALIDATION (safety checks)
	else if (MC_MCSERV_VERSION < 0x20A) {
		boot_txt_render_extended_string((const u8*)"libmc: too old release of mcserv.irx\n", 0, 0, 0, 0, 0, 0, 0);
		MC_IS_BOUND_FLAG = 0;
		status_code = -0x78;
	}
	else {
		status_code = g_sys_mc_result_metadata_val;
		if (MC_MCMAN_VERSION < 0x20E) {
			boot_txt_render_extended_string((const u8*)"libmc: too old release of mcman.irx\n", 0, 0, 0, 0, 0, 0, 0);
			MC_IS_BOUND_FLAG = 0;
			status_code = -0x79;
		}
	}

	// For the modern PC port, overwrite with forced success to enable
	// direct local file writes without depending on the PS2 modules:
#if !defined(PLATFORM_PS2)
	MC_IS_BOUND_FLAG = 1;
	status_code = 0;
#endif

	return status_code;
}

// References to the consolidated kernel helpers and infrastructure
s32  hud_validate_widget_node_state(const u32* p_widget_handle);
void mc_io_wait_delay_ms(void);
s32  sceSignalSema(s32 sema_id);

// Reference to the global wait-semaphore variable of ps2_kernel.c
extern s32 g_sys_io_wait_sema_id;

/**
 * @brief Guards the synchronization of Memory Card I/O operations (read/write).
 * Blocks the execution thread with mc_io_wait_delay_ms while the hardware reports activity.
 * Original Ghidra address: 0x00127B88 (PAL)
 */
u32 sys_mc_io_sync_command_guard(long command_type, long p_out_cmd_ptr, long p_out_meta_ptr) {
	s32 is_mc_node_active;
	u32 query_status;

	// 1. Initialization control or idle state of the reader
	if (MC_ACTIVE_COMMAND_ID == 0) {
		return 0xFFFFFFFF;
	}

	is_mc_node_active = hud_validate_widget_node_state(&MC_CHANNEL_WIDGET_HANDLE);

	// 2. Blocking loop: safely freezes the CPU while the card I/O bus is busy
	if (command_type == 0 && is_mc_node_active != 0) {
		while (1) {
			s32 is_busy = hud_validate_widget_node_state(&MC_CHANNEL_WIDGET_HANDLE);
			is_mc_node_active = 0;
			if (is_busy == 0) {
				break;
			}
			// Call the hardware delay function from the previous step
			mc_io_wait_delay_ms();
		}
	}

	// Compute the resulting logical state of the channel occupation
	query_status = (u32)(is_mc_node_active == 0);

	if (p_out_cmd_ptr != 0) {
		*(s32*)p_out_cmd_ptr = MC_ACTIVE_COMMAND_ID;
	}

	// 3. If the transaction finished, release the mutual exclusion and dump the metadata
	if (query_status != 0) {
		MC_ACTIVE_COMMAND_ID = 0;

		if (p_out_meta_ptr != 0) {
			*(u32*)p_out_meta_ptr = MC_RESULT_METADATA_VAL;
		}

		// Atomically release the kernel semaphore, passing it the global wait ID
		sceSignalSema(g_sys_io_wait_sema_id);
	}

	return query_status;
}

// References to the already ported master functions
u32  sys_sound_dispatch_iop_query_filter(void);

// References to the kernel functions already integrated portably in ps2_kernel.c
s32 sceGetThreadId(void);
s32 sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg);
s32 sceSleepThread(void);

/**
 * @brief Pauses the active Memory Card execution thread by setting a kernel alarm and putting the CPU to sleep.
 * Used by the engine to give physical data-settling margins during I/O operations.
 * Original Ghidra address: 0x00127B40 (PAL)
 */
void mc_io_wait_delay_ms(void) {
	// 1. Query the identifier of the current thread that owns the operation
	sceGetThreadId();

	// 2. Schedule the hardware alarm and put the game thread into voluntary sleep.
	// For the modern native PC port, both calls resolve passively and smoothly
	// because reads on modern disks (SSD/NVMe) take nanoseconds:
	sceSetAlarm(1000, NULL, NULL); // Simulated safety hardware delay (1000 ms/1 s maximum)
	sceSleepThread();
}

/**
 * @brief Root state machine coordinating the transition from the intro to the main playable menu.
 * Monitors the status of the sound IOP and atomically runs the callback of the next game stage.
 * Original Ghidra address: 0x002B8940 (PAL)
 *
 * @param execution_stage Control parameter defining the loop phase (param_1).
 */
void sys_boot_intro_state_machine(s32 execution_stage) {
	// 1. Check whether the active phase requires the software readiness poll
	if (execution_stage == 1) {

		// Ask whether the sound hardware and IOP finished the asynchronous load
		s32 audio_status = (s32)sys_sound_dispatch_iop_query_filter();

		if (audio_status == 0) {
			// Clear the instruction cache before the atomic jump
			sceFlushCache(0, NULL, 0);

			// Keep the descriptors of the next game stage in local variables
			u32 stage_arg = NEXT_GAME_STAGE_ARGUMENT;
			void (*p_stage_callback)(u32, bool) = (void (*)(u32, bool))NEXT_GAME_STAGE_CALLBACK;

			// Reset the global boot flags to clean the RAM
			BOOT_INTRO_DELAY_STATE = 0;
			bool is_clean_boot = (BOOT_INTRO_TRANSITION_FLAG == '\0');
			BOOT_INTRO_TRANSITION_FLAG = '\0';

			// 2. MASTER GAME TRIGGER: if a subroutine is linked, jump to it immediately
			if (p_stage_callback != NULL) {
				NEXT_GAME_STAGE_ARGUMENT = 0;
				NEXT_GAME_STAGE_CALLBACK = NULL;

				// Redirect the native execution thread to the main menu / gameplay screen
				p_stage_callback(stage_arg, is_clean_boot);
			}
		}
		else {
			// If the audio bus is still busy loading tracks, configure an active wait state
			BOOT_INTRO_DELAY_STATE = 2;
		}
	}
}

// References to the interlinked repository components
void sys_io_init_kernel_semaphores(void);
s32  sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8);
s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

// Additional PS2 SDK support stubs emulated portably
s32  scePollSema(s32 sema_id);
//void sceSignalSema(s32 sema_id);

// Definitions of the static audio and interrupt offsets in PS2 RAM
#define IO_WAIT_SEMA_ID             (*(s32*)EE_ADDR(0x0013642C))
#define CURRENT_AUDIO_CMD_ID        (*(s32*)EE_ADDR(0x00136418))
#define AUDIO_SESSION_STATUS_FLAG   (*(s32*)EE_ADDR(0x00136448))
#define DEBUG_NET_LOG_LEVEL         (*(s32*)EE_ADDR(0x00136410))
#define AUDIO_HARDWARE_READY_FLAG   (*(s32*)EE_ADDR(0x00137E6C))

// References to the low-level components and kernel infrastructure
void sys_io_init_kernel_semaphores(void);
s32  scePollSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
s32  sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8);
s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

// References to the interlinked repository components
u32  hud_allocate_linear_node_slot(s32* p_master_alloc_struct);
void sys_kernel_flush_dcache_range(u32 start_addr, long block_size);
void hud_disable_widget_node(u32* p_internal_node);
u64  sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Official Sony kernel API stubs emulated in ps2_kernel.c
s32 sceCreateSema(void);
s32 sceWaitSema(s32 sema_id);
s32 sceDeleteSema(s32 sema_id);

// Physical destination address of the audio buffer in PS2 RAM
#define SOUND_IOP_STATUS_BUFFER_PTR    ((void*)EE_ADDR(0x00137600))

// Definition of the audio state reserve variable in PS2 RAM
#define SIF_SOUND_BACKUP_IOP_STATUS     (*(u32*)EE_ADDR(0x001A7190))

// Reference to the global lock flag of the external IO queue
extern s32 g_sys_io_queue_lock_flag;

// Reference to the already integrated audio polling function
s32 sys_sound_query_iop_status(void);

/**
 * @brief Filters and intercepts sound chip poll requests, checking the general queue lock.
 * Returns the previous reserve state immediately if the hardware bus is congested.
 * Original Ghidra address: 0x001336E8 (PAL)
 *
 * @return u32 Game audio synchronization status code (or backup buffer).
 */
u32 sys_sound_dispatch_iop_query_filter(void) {
	u32 current_status = SIF_SOUND_BACKUP_IOP_STATUS;

	// If the global queue lock is free, dispatch the audio query immediately
	if (g_sys_io_queue_lock_flag == 0) {
		current_status = (u32)sys_sound_query_iop_status();
	}

	return current_status;
}

// References to the consolidated kernel components and infrastructure
s32  sys_sound_init_audio_stream_session(long current_cmd_param);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr,
	long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);
s32  sceSignalSema(s32 sema_id);

// Reference to the global IO wait-semaphore variable of ps2_kernel.c
extern s32 g_sys_io_wait_sema_id;

/**
 * @brief Polls the state of the IOP coprocessor's audio firmware by running RPC transaction command 4.
 * Initializes the stream session under token 3 and atomically releases the mutual-exclusion semaphore.
 * Original Ghidra address: 0x00125588 (PAL)
 *
 * @return s32 General status code (0 for full success, 0xFFFFFFFF for bus failures).
 */
s32 sys_sound_query_iop_status(void) {
	// 1. Wake and initialize the streaming channel under audio command ID 3
	s32 session_init_status = sys_sound_init_audio_stream_session(3);
	s32 final_status_code;

	if (session_init_status == 0) {
		final_status_code = 0xFFFFFFFF; // Error: could not raise the audio loop
	}
	else {
		// 2. Dispatch the command 4 transaction, querying hardware buffer 0x137600 (size 4 bytes)
		extern u32 g_sys_sound_channel_widget_handle;
		s32 transaction_status = sys_sif_rpc_send_transaction_data(
			&g_sys_sound_channel_widget_handle,
			4, 0, 0, 0,
			(long)SOUND_IOP_STATUS_BUFFER_PTR,
			4, 0, 0
		);

		// 3. Atomically release the kernel semaphore, passing it the global wait ID
		if (transaction_status < 0) {
			sceSignalSema(g_sys_io_wait_sema_id);
			final_status_code = 0xFFFFFFFF; // Error: critical transmission failure on the SIF bus
		}
		else {
			final_status_code = 0; // Audio and IOP synchronization completed successfully
			sceSignalSema(g_sys_io_wait_sema_id);
		}
	}

	return final_status_code;
}

/**
 * @brief Sends, synchronizes and manages an extended data transfer burst through SIF RPC transactions.
 * Applies dcache physical coherence barriers and orchestrates suspending the control thread through kernel semaphores.
 * Original Ghidra address: 0x0011D620 (PAL)
 */
s32 sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr,
	long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg) {
	if (p_session_handle == NULL) {
		return -1;
	}

	// Simulated structure so the compiler understands the type
	typedef struct {
		int dummy[16]; // Padding buffer for size compatibility
	} SifRpcClientData;

	// The original code now compiles:
	extern u32 g_sys_sif_rpc_client_struct[16];   /* SifRpcClientData storage */
	u32* p_node_slot = (u32*)hud_allocate_linear_node_slot((s32*)&g_sys_sif_rpc_client_struct);

	if (p_node_slot == NULL) {
		return 0xFFFFFFFF; // Error: buffer full
	}

	u32 structural_signature = p_node_slot[0x06]; // Validation offset 0x18
	p_session_handle[8] = extra_arg;
	*p_session_handle = (u32)(long)p_node_slot;
	p_session_handle[1] = structural_signature;
	p_session_handle[7] = (s32)p8;

	p_node_slot[8] = command_id;
	p_node_slot[9] = (u32)src_size;
	p_node_slot[10] = (u32)dest_addr;
	p_node_slot[11] = (u32)dest_size;
	p_node_slot[5] = (u32)(long)p_node_slot;

	u32 callback_val = p_session_handle[9];
	p_node_slot[7] = (u32)(long)p_session_handle;
	p_node_slot[13] = callback_val;

	u32 u_src_addr = (u32)src_addr;

	// 2. CPU CACHE COHERENCE BARRIERS (D-CACHE FLUSH)
	if ((sync_flags & 2) == 0) {
		if (src_addr == dest_addr) {
			long final_size = dest_size;
			if (dest_size <= src_size) {
				final_size = src_size;
			}
			sys_kernel_flush_dcache_range(u_src_addr, final_size);
		}
		else {
			if (src_size > 0) {
				sys_kernel_flush_dcache_range(u_src_addr, src_size);
			}
			if (dest_size > 0) {
				sys_kernel_flush_dcache_range((u32)dest_addr, dest_size);
			}
		}
	}

	s32 status_code = 0xFFFFFFFF;

	// 3. CASE A: mandatory synchronous transmission with kernel semaphores
	if ((sync_flags & 1) == 0) {
		s32 sema_id = sceCreateSema();
		p_session_handle[2] = sema_id;

		if (sema_id < 0) {
			hud_disable_widget_node(p_node_slot);
			return 0xFFFFFFFD;
		}

		p_node_slot[12] = 1; // Active state flag

		// Send the asynchronous block transfer command (0x8000000a)
		long dma_status = (long)sys_sif_submit_dma_packet_simple(0x8000000A, p_node_slot, 0x40, u_src_addr, p_session_handle[5], src_size);

		if (dma_status != 0) {
#if defined(PLATFORM_PS2)
			sceWaitSema(sema_id);
			sceDeleteSema(sema_id);
#endif
			return 0; // Synchronous transaction succeeded
		}

#if defined(PLATFORM_PS2)
		sceDeleteSema(sema_id);
#endif
	}
	// 4. CASE B: free asynchronous dispatch (fire and forget)
	else {
		if (p8 == 0) {
			p_node_slot[12] = 0;
		}
		else {
			p_node_slot[12] = 1;
		}
		p_session_handle[2] = 0xFFFFFFFF;

		long dma_status = (long)sys_sif_submit_dma_packet_simple(0x8000000A, p_node_slot, 0x40, u_src_addr, p_session_handle[5], src_size);
		if (dma_status != 0) {
			return 0;
		}
	}

	// Escape control if the descriptor queue experienced hardware failures
	hud_disable_widget_node(p_node_slot);
	return 0xFFFFFFFE;
}

/**
 * @brief Initializes the streaming session and orchestrates the atomic link of the audio bus for the intro and menus.
 * Queries the synchronization guards and safely opens RPC transaction 0x80000593.
 * Original Ghidra address: 0x00124C98 (PAL)
 *
 * @param current_cmd_param Identifier of the game audio command or order to dispatch (param_1).
 * @return s32 Status code (1 on success, 0 on failure or thread collision).
 */
s32 sys_sound_init_audio_stream_session(long current_cmd_param) {
	// 1. Make sure the base I/O semaphores are registered in the Sony kernel
	sys_io_init_kernel_semaphores();

	// Non-blocking status poll of the IO wait semaphore
	s32 current_kernel_sema = scePollSema(IO_WAIT_SEMA_ID);

	// 2. If the ID matches, the thread safely takes control of the audio session
	if (IO_WAIT_SEMA_ID == current_kernel_sema) {
		CURRENT_AUDIO_CMD_ID = (s32)current_cmd_param;

		// If the original game only passively queries the audio thread state:
		sceReferThreadStatus(1, NULL);

		// Ask the S (sound) channel guard whether the physical bus is available
		s32 is_sound_busy = sys_sound_sync_command_guard(1, 0, 0, 0, 0, 0, 0, 0);

		if (is_sound_busy == 0) {
			// Safely initialize the RPC client environment
			sys_sif_rpc_init_client();

			if (AUDIO_SESSION_STATUS_FLAG > -1) {
				return 1;
			}

			// 3. ACTIVE WAIT LOOP: opens the engine's asynchronous audio session (command 0x80000593)
			extern u32 g_sys_sound_channel_widget_handle;
			while (1) {
				while (1) {
					s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_sound_channel_widget_handle, 0x80000593, 0);

					if (session_status >= 0) {
						break;
					}

					// If a hardware error occurs on the bus, print the alert with the typographic interceptor
					if (DEBUG_NET_LOG_LEVEL > 0) {
						boot_txt_render_extended_string((const u8*)"Libcdvd bind err S cmd\n", 0, 0, 0, 0, 0, 0, 0);
					}

					// Fine delay of physical wait cycles to retry the transaction on the console
					s32 delay_counter = 0x100000;
					while (delay_counter != -1) { delay_counter--; }
				}

				// Break condition once the streaming bridge with the IOP firmware is established
				if (AUDIO_HARDWARE_READY_FLAG != 0) {
					break;
				}

				s32 delay_counter = 0x100000;
				while (delay_counter != -1) { delay_counter--; }
			}

			AUDIO_SESSION_STATUS_FLAG = 0;
			return 1;
		}

		// If the audio guard reports it is busy, release the semaphore so the system does not hang
		sceSignalSema(IO_WAIT_SEMA_ID);
	}
	// Error case in the main semaphore recorded by network debugging
	else if (DEBUG_NET_LOG_LEVEL > 0) {
		boot_txt_render_extended_string((const u8*)"Scmd fail sema cur_cmd:%d keep_cmd:%d\n", current_cmd_param, (long)CURRENT_AUDIO_CMD_ID, 0, 0, 0, 0, 0);
		return 0;
	}

	return 0;
}

// References to the repository functions required for linking
u32  hud_alloc_ring_buffer_node(u32* p_ring_struct);
s32* hud_find_node_by_id(s32 target_node_id, void* p_hud_container);
void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// References to the repository components required for linking
u32  hud_alloc_ring_buffer_node(u32* p_ring_struct);
void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Reference to the keyboard interrupt handler of the previous step
void sys_hardware_keyboard_interrupt_handler(u64 expected_thread_id);

// References to the unified kernel synchronization functions
extern bool kernel_system_sync_guard(void);
extern void kernel_system_sync_release(void);

// References to the repository components needed for interconnection
u32  hud_allocate_linear_node_slot(s32* p_master_alloc_struct);
void hud_disable_widget_node(u32* p_internal_node);
u64  sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Official Sony kernel API stubs, emulated
s32 sceCreateSema(void);
s32 sceWaitSema(s32 sema_id);
s32 sceDeleteSema(s32 sema_id);

/**
 * @brief Opens and manages an asynchronous command session through SIF RPC transactions.
 * Reserves a linear slot, orchestrates synchronization with kernel semaphores and dispatches the metadata.
 * Original Ghidra address: 0x0011D450 (PAL)
 *
 * @param p_session_handle Pointer to the local structure of the session handle (param_1).
 * @param command_id Identifier of the game command or order to send to the IOP (param_2).
 * @param sync_flags Bit mask that decides whether the wait is synchronous or in the background (param_3).
 * @return s32 Status code (0 on success, negative values for hardware errors).
 */
s32 sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags) {
	if (p_session_handle == NULL) {
		return -1;
	}

	// Initialize the state variables at the handle's contiguous offsets
	p_session_handle[4] = 0; // Offset +16
	p_session_handle[9] = 0; // Offset +36

	// Simulated structure so the compiler understands the type
	typedef struct {
		int dummy[16]; // Padding buffer for size compatibility
	} SifRpcClientData;

	// The original code now compiles:
	extern u32 g_sys_sif_rpc_client_struct[16];   /* SifRpcClientData storage */
	u32* p_node_slot = (u32*)hud_allocate_linear_node_slot((s32*)&g_sys_sif_rpc_client_struct);

	s32 status_code = 0xFFFFFFFF; // Default error if the buffer is full

	if (p_node_slot != NULL) {
		u32 structural_signature = p_node_slot[6]; // Validation offset 0x18

		*p_session_handle = (u32)(long)p_node_slot; // Stores the node address
		p_session_handle[1] = structural_signature;  // Stores the control signature

		p_node_slot[8] = command_id;         // Inserts the command ID into the packet
		p_node_slot[5] = (u32)(long)p_node_slot; // Self-addressing of the header
		p_node_slot[7] = (u32)(long)p_session_handle;

		// Case A: mandatory synchronous hardware wait through semaphores
		if ((sync_flags & 1) == 0) {
			s32 sema_id = sceCreateSema();
			p_session_handle[2] = sema_id;

			if (sema_id < 0) {
				hud_disable_widget_node(p_node_slot);
				status_code = 0xFFFFFFFD; // Error: semaphore creation failed
			}
			else {
				// Send the asynchronous open command (0x80000009)
				u64 dma_status = sys_sif_submit_dma_packet_simple(0x80000009, p_node_slot, 0x40, 0, 0, 0);

				if (dma_status == 0) {
					hud_disable_widget_node(p_node_slot);
#if defined(PLATFORM_PS2)
					sceDeleteSema(sema_id);
#endif
					status_code = 0xFFFFFFFE; // Error: the DMA bus failed
				}
				else {
					// Put the CPU to sleep while waiting for the bus response
#if defined(PLATFORM_PS2)
					sceWaitSema(sema_id);
					sceDeleteSema(sema_id);
#endif
					status_code = 0; // Transaction completed with full success
				}
			}
		}
		// Case B: free asynchronous dispatch (fire and forget)
		else {
			p_session_handle[2] = 0xFFFFFFFF;
			u64 dma_status = sys_sif_submit_dma_packet_simple(0x80000009, p_node_slot, 0x40, 0, 0, 0);
			status_code = 0;

			if (dma_status == 0) {
				hud_disable_widget_node(p_node_slot);
				status_code = 0xFFFFFFFE;
			}
		}
	}

	return status_code;
}

/**
 * @brief Sequentially scans the reserved HUD memory looking for an empty 64-byte (0x40) node slot.
 * If found, sets its activation flags and safely inserts the validation signatures.
 * Original Ghidra address: 0x0011D140 (PAL)
 *
 * @param p_master_alloc_struct Pointer to the global allocation control structure (param_1).
 * @return u32 Physical memory address of the reserved and configured widget slot, or 0 if full.
 */
u32 hud_allocate_linear_node_slot(s32* p_master_alloc_struct) {
	if (p_master_alloc_struct == NULL) {
		return 0;
	}

	bool interrupt_status = kernel_system_sync_guard();

	s32 current_index = 0;

	// Index 1 (1 * 4 = 4 bytes) holds the base pointer of the physical node array
	u32 p_node_array_base = (u32)p_master_alloc_struct[1];

	// Index 2 (2 * 4 = 8 bytes) holds the limit or maximum slot capacity of the array
	s32 max_slots_limit = p_master_alloc_struct[2];

	if (max_slots_limit > 0) {
		u8* p_node_cursor = (u8*)EE_ADDR(p_node_array_base);

		do {
			// Offset 0x10 (index 4 as u32) holds the widget state flags
			u32* p_node_flags = (u32*)(p_node_cursor + 0x10);

			// If the least significant bit is zero, the slot is completely free
			if ((*p_node_flags & 1) == 0) {

				// Set the flag by writing 5 (live enable bit) and keep the index in the upper part
				*p_node_flags = (u32)(current_index << 0x10) | 5;

				// Extract, increment and update the master global counter of unique signatures
				s32 old_signature = *p_master_alloc_struct;
				s32 new_signature = old_signature + 1;
				*p_master_alloc_struct = new_signature;

				// Engine safeguard rule: if the signature reaches 1, force it to 2
				if (new_signature == 1) {
					new_signature = old_signature + 2;
					*p_master_alloc_struct = new_signature;
				}

				// Offset 0x14 (index 5 as u32) points to the widget's own base address
				*(u32*)(p_node_cursor + 0x14) = (u32)(long)p_node_cursor;

				// Offset 0x18 (index 6 as u32) holds the computed unique validation signature
				*(s32*)(p_node_cursor + 0x18) = new_signature;

				if (interrupt_status) {
					kernel_system_sync_release();
				}

				return (u32)(long)p_node_cursor; // Returns the assigned slot, ready to use
			}

			current_index++;
			p_node_cursor += 0x40; // Exact offset to the next sibling node (64 bytes)
		} while (current_index < max_slots_limit);
	}

	if (interrupt_status) {
		kernel_system_sync_release();
	}

	return 0; // The dynamic buffer is completely full
}

/**
 * @brief SIF RPC interrupt callback that receives, packs and asynchronously queues a request from the IOP.
 * Interleaves the FIFO pointers on the bus and fires the hardware interrupt handler if the channel is free.
 * Original Ghidra address: 0x0011D590 (PAL)
 *
 * @param p_packet_req Base address of the interrupt packet with the load order sent by the IOP (param_1).
 */
void sys_sif_rpc_on_queue_request(void* p_packet_req) {
	if (p_packet_req == NULL) {
		return;
	}

	u8* p_pkt = (u8*)p_packet_req;

	// Offset 0x34 holds the physical address of the internal data node to process (int)
	s32 p_node_addr = *(s32*)(p_pkt + 0x34);
	u8* p_node = (u8*)EE_ADDR(p_node_addr);

	// Offset 0x40 inside the node points to the master SIF channel descriptor (int**)
	s32** pp_channel_master = *(s32***)(p_node + 0x40);
	s32* p_channel = *pp_channel_master;

	// Index 3 (3 * 4 = 12 bytes) is the pointer to the front node of the FIFO queue
	if (p_channel[3] == 0) {
		p_channel[3] = p_node_addr;
	}
	else {
		// Index 4 (4 * 4 = 16 bytes) is the pointer to the previous rear output node
		u8* p_last_rear_node = (u8*)EE_ADDR(p_channel[4]);
		*(s32*)(p_last_rear_node + 0x3C) = p_node_addr; // FIFO bridge link
	}

	// Update the rear queue register with the address of the newly inserted element
	p_channel[4] = p_node_addr;

	// Contiguous 32-bit burst dump of the metadata into the internal node
	*(u32*)(p_node + 0x20) = *(u32*)(p_pkt + 0x14); // Command ID
	*(u32*)(p_node + 0x1C) = *(u32*)(p_pkt + 0x1C); // Source buffer pointer
	*(u32*)(p_node + 0x24) = *(u32*)(p_pkt + 0x20); // Destination buffer pointer
	*(u32*)(p_node + 0x0C) = *(u32*)(p_pkt + 0x24); // Burst size in bytes
	*(u32*)(p_node + 0x28) = *(u32*)(p_pkt + 0x28); // Control attributes
	*(u32*)(p_node + 0x2C) = *(u32*)(p_pkt + 0x2C); // Synchronization mask
	*(u32*)(p_node + 0x30) = *(u32*)(p_pkt + 0x30); // Secondary parameters
	*(u32*)(p_node + 0x34) = *(u32*)(p_pkt + 0x10); // Owner thread ID

	// Evaluation of the cross-thread hardware trigger
	s32 active_thread_id = p_channel[0]; // Index 0 (+0 bytes)
	s32 channel_busy_flag = p_channel[1]; // Index 1 (+4 bytes)

	if (active_thread_id > -1 && channel_busy_flag == 0) {
		// Call the lab routine to inject and process the hardware entry
		sys_hardware_keyboard_interrupt_handler((u64)active_thread_id);
	}
}

// Definition of the global keyboard buffer variables in PS2 RAM
#define KEYBOARD_BUFFER_INDEX       (*(s32*)EE_ADDR(0x0013C68C))
#define KEYBOARD_STATIC_BUFFER_PTR  ((u16*)EE_ADDR(0x0013C690))

// Reference to the already integrated console character counter
extern s32 g_debug_console_char_count;

// Official Sony kernel API prototypes, emulated
s32 iSignalSema(s32 sema_id);
s32 iWakeupThread(s32 thread_id);

/**
 * @brief Hardware interrupt handler that captures keystrokes of the development keyboard.
 * Stores the ASCII characters cyclically in a 512-entry ring buffer.
 * Original Ghidra address: 0x0011B8D8 (PAL)
 *
 * @param expected_thread_id Identifier of the execution thread expected for validation (param_1).
 */
void sys_hardware_keyboard_interrupt_handler(u64 expected_thread_id) {
	u64 hardware_input_char = 0;

	// On a real PS2 this runs syscall(-47) to query the Sony hardware.
	// For the native PC port, the operating system handles keyboard events in the background.
	// The behaviour of the in_v0 register is simulated by extracting the character:
#if defined(PLATFORM_PS2)
	__asm__ volatile("syscall" : "=r"(hardware_input_char) : "r"(-47));
#else
	hardware_input_char = 0; // Simulated empty input to avoid garbage loops on PC
#endif

	// 1. If the key matches the expected thread, insert it into the 512-byte ring
	if (hardware_input_char == expected_thread_id) {
		if (hardware_input_char < 0x100 && g_debug_console_char_count != 0) {

			// Apply the binary mask & 0x1FF to limit the cyclic index to 512 words (9 bits)
			s32 ring_index = KEYBOARD_BUFFER_INDEX & 0x1FF;
			KEYBOARD_BUFFER_INDEX = (KEYBOARD_BUFFER_INDEX & 0x1FF) + 1;

			// Insert the character contiguously into the global static buffer
			u16* p_buffer_cell = KEYBOARD_STATIC_BUFFER_PTR + ring_index;
			*p_buffer_cell = (u16)hardware_input_char;

			// Wake the control threads at once through logical semaphores
#if defined(PLATFORM_PS2)
			iSignalSema(0); // Operating system semaphore ID
#endif
		}
	}
	// 2. If the thread is out of step, force an immediate CPU wake-up
	else {
#if defined(PLATFORM_PS2)
		iWakeupThread((s32)expected_thread_id);
#endif
	}
}

/**
 * @brief SIF RPC interrupt callback that processes and dispatches direct DMA data transfer (streaming) commands.
 * Prepares the slot in the ring buffer and forwards the physical PS2 burst pointers.
 * Original Ghidra address: 0x0011D2F0 (PAL)
 *
 * @param p_incoming_packet Address of the packet with the burst addresses (param_1).
 * @param p_hud_container Base address of the global graphical interface container (param_2).
 */
void sys_sif_rpc_on_data_transfer(void* p_incoming_packet, void* p_hud_container) {
	if (p_incoming_packet == NULL || p_hud_container == NULL) {
		return;
	}

	u32* p_pkt_in = (u32*)p_incoming_packet;

	// 1. Reserve the cyclic command slot in Insomniac's ring buffer
	u32* p_response_packet = (u32*)hud_alloc_ring_buffer_node((u32*)p_hud_container);

	// 2. Extract and pack the base PS2 synchronization identifiers
	u32 val_1c = p_pkt_in[7]; // Offset 0x1C (index 7)
	p_response_packet[5] = p_pkt_in[5]; // Offset 0x14 (index 5)
	p_response_packet[7] = val_1c;
	p_response_packet[8] = 0x8000000C; // Command ID of the active transfer execution

	// 3. Extract the physical pointers and lengths of the asynchronous transaction
	u32 src_dma_addr = p_pkt_in[8];  // Offset 0x20 (index 8)
	u32 dest_dma_addr = p_pkt_in[9]; // Offset 0x24 (index 9)
	long block_len = (long)p_pkt_in[10]; // Offset 0x28 (index 10)

	// 4. Call the synchronous wrapper at once, sending the physical descriptors to the SIF bus (size 64 bytes = 0x40)
	sys_sif_submit_dma_packet_sync(0x80000008, p_response_packet, 0x40, src_dma_addr, dest_dma_addr, block_len);
}

/**
 * @brief SIF RPC interrupt callback that answers the IOP coprocessor about the physical state of a HUD node.
 * Reserves a slot in the ring buffer, queries the tree with hud_find_node_by_id and dispatches a synchronous packet.
 * Original Ghidra address: 0x0011D3A0 (PAL)
 *
 * @param p_incoming_packet Address of the query packet sent by the IOP (param_1).
 * @param p_hud_container Base address of the global graphical interface container (param_2).
 */
void sys_sif_rpc_on_query_node_status(void* p_incoming_packet, void* p_hud_container) {
	if (p_incoming_packet == NULL || p_hud_container == NULL) {
		return;
	}

	u32* p_pkt_in = (u32*)p_incoming_packet;

	// 1. Reserve a burst slot in the circular buffer with the allocator function
	u32* p_response_packet = (u32*)hud_alloc_ring_buffer_node((u32*)p_hud_container);

	// 2. Copy and pack the base control metadata at the contiguous offsets
	u32 val_14 = p_pkt_in[5]; // Offset 0x14
	p_response_packet[7] = p_pkt_in[7]; // Offset 0x1C
	p_response_packet[5] = val_14;
	p_response_packet[8] = 0x80000009; // Response status ID

	// 3. Call the finder to query whether the widget exists by ID (offset 0x20)
	s32 target_id = (s32)p_pkt_in[8];
	s32* p_found_node = hud_find_node_by_id(target_id, p_hud_container);

	if (p_found_node == NULL) {
		// Empty safety slots if the node was not found
		p_response_packet[9] = 0;
		p_response_packet[10] = 0;
		p_response_packet[11] = 0;
	}
	else {
		// Insert the real RAM addresses, semaphores and states
		p_response_packet[9] = (u32)(long)p_found_node;
		p_response_packet[10] = (u32)p_found_node[2]; // Internal attribute or semaphore
		p_response_packet[11] = (u32)p_found_node[5]; // Secondary status flag
	}

	// 4. Priority dispatch back to the SIF bus with the synchronous wrapper (64-byte packet = 0x40)
	sys_sif_submit_dma_packet_sync(0x80000008, p_response_packet, 0x40, 0, 0, 0);
}

// Reference to the master SIF function already integrated in the repository
u64 sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len);

/**
 * @brief Cyclically assigns and indexes the next available slot in a HUD ring buffer.
 * Applies modulo arithmetic to reuse fixed 64-byte (0x40) slots and prevents divisions by zero.
 * Original Ghidra address: 0x0011D208 (PAL)
 *
 * @param p_ring_struct Base address of the circular buffer control structure (param_1).
 * @return u32 Physical memory address of the computed slot, ready to use.
 */
u32 hud_alloc_ring_buffer_node(u32* p_ring_struct) {
	if (p_ring_struct == NULL) {
		return 0;
	}

	// Offset 0x18 is index 6 in 32-bit integers (6 * 4 = 24 bytes) - maximum capacity
	s32 max_capacity = (s32)p_ring_struct[0x06];

	// Safety check equivalent to the PS2 hardware instruction trap(7)
	assert(max_capacity != 0 && "Critical HUD error: ring buffer capacity is zero.");
	if (max_capacity == 0) {
		return 0;
	}

	// Offset 0x24 is index 9 (9 * 4 = 36 bytes) - cumulative insertion counter
	s32 total_inserts = (s32)p_ring_struct[0x09];

	// Compute the safe circular index with the modulo operation
	s32 circular_index = total_inserts % max_capacity;

	// Increment the cumulative cursor for the next allocation
	p_ring_struct[0x09] = (u32)(circular_index + 1);

	// Offset 0x14 is index 5 (5 * 4 = 20 bytes) - base address of the node array
	u32 p_array_base = p_ring_struct[0x05];

	// Each node is strictly 64 bytes (0x40 in hexadecimal)
	return p_array_base + (u32)(circular_index * 0x40);
}

// Definition of the global command filter variables in PS2 RAM
#define IO_QUEUE_LOCK_FLAG         (*(s32*)EE_ADDR(0x001A750C))
#define IO_BACKUP_COMMAND_ID       (*(u32*)EE_ADDR(0x001A7510))

// Definition of the semaphore identifiers in PS2 RAM
#define IO_LOCK_SEMA_ID             (*(s32*)EE_ADDR(0x00136428))
#define IO_WAIT_SEMA_ID             (*(s32*)EE_ADDR(0x0013642C))
#define IO_DMA_SEMA_ID              (*(s32*)EE_ADDR(0x00136420))

// Reference to the already integrated global pending-command variable
#define IO_PENDING_COMMANDS_COUNT   (*(s32*)EE_ADDR(0x00136430))

// Definition of the sound channel offset in PS2 RAM
#define SOUND_CHANNEL_WIDGET_HANDLE    (*(u32*)EE_ADDR(0x00137E48))
#define DEBUG_NET_LOG_LEVEL            (*(s32*)EE_ADDR(0x00136410))

// Simulated RPC subsystem state control variables for the PC port
u8  g_sys_sif_rpc_is_initialized = 0;
u32 g_sys_sif_rpc_client_struct[16] = { 0 };
u32 g_sys_sif_rpc_dummy_packet = 0;

// References to the helpers and initializers already integrated in the repository
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
bool sys_sif_init_manager(void);
u32  sys_sif_get_channel_descriptor_ptr(s32 channel_index);
//void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Additional native stubs of the Sony SDK
u32  sceSifSetReg(void);

// Reference to the HUD node deactivator already integrated in the repository
void hud_disable_widget_node(u32* p_internal_node);

// Official Sony kernel SDK stub for semaphore interrupts
s32 iSignalSema(s32 sema_id);

/**
 * @brief Performs an exhaustive two-dimensional search of the tree and linked lists to locate a HUD node by its ID.
 * Walks the container branches (offset 0x24) and the list siblings (offset 0x38) until a match is found.
 * Original Ghidra address: 0x0011D350 (PAL)
 *
 * @param target_node_id Unique identifier of the component to search for (param_1).
 * @param p_hud_container Base address of the interface container structure (param_2).
 * @return s32* Pointer to the physical node of the structure found in RAM, or NULL if absent.
 */
s32* hud_find_node_by_id(s32 target_node_id, void* p_hud_container) {
	if (p_hud_container == NULL) {
		return NULL;
	}

	u8* p_container_bytes = (u8*)p_hud_container;

	// Offset 0x28 (index 10 as u32) points to the first branch or control block of the container
	s32 p_current_branch = *(s32*)(p_container_bytes + 0x28);

	while (p_current_branch != 0) {
		u8* p_branch_bytes = (u8*)p_current_branch;

		// Offset +8 (index 2 as u32) inside the branch holds the pointer to the start of the node list
		s32* p_node_cursor = *(s32**)(p_branch_bytes + 8);

		while (p_node_cursor != NULL) {
			// Index 0 (+0 bytes) always holds the identifier of the current node
			s32 current_node_id = *p_node_cursor;

			if (current_node_id == target_node_id) {
				return p_node_cursor; // Found: returns the physical RAM pointer
			}

			// Offset 0x38 (index 14 / 0x0E as s32) holds the pointer to the next sibling node
			p_node_cursor = (s32*)p_node_cursor[0x0E];
		}

		// When the current branch's list is exhausted, jump to offset 0x14 (index 5) to move to the next branch
		p_current_branch = *(s32*)(p_branch_bytes + 0x14);
	}

	return NULL; // No match was found in the whole superstructure
}

/**
 * @brief Hardware interrupt handler (callback) invoked when a transaction on the SIF RPC bus completes.
 * Runs the associated user callback, updates the buffer metadata and wakes the paused system threads.
 * Original Ghidra address: 0x0011D238 (PAL)
 *
 * @param p_packet_res Base address of the interrupt packet received from the IOP (param_1).
 */
void sys_sif_rpc_on_transaction_complete(void* p_packet_res) {
	if (p_packet_res == NULL) {
		return;
	}

	u8* p_pkt = (u8*)p_packet_res;

	// Offset 0x20 holds the identifier or type of the received SIF event (uint)
	u32 transaction_event_id = *(u32*)(p_pkt + 0x20);

	// Offset 0x1C holds the pointer to the channel's internal control structure (int*)
	s32** pp_channel_struct = *(s32***)(p_pkt + 0x1C);
	s32* p_channel = *pp_channel_struct;

	if (transaction_event_id == 0x8000000A) {
		// Index 7 (7 * 4 = 28 bytes) holds the user callback function pointer
		void (*user_callback)(s32) = (void (*)(s32))p_channel[7];

		if (user_callback != NULL) {
			// Index 8 (8 * 4 = 32 bytes) holds the associated argument passed to the callback
			user_callback(p_channel[8]);

			// Reload the structure in case it was altered during the user call
			pp_channel_struct = *(s32***)(p_pkt + 0x1C);
			p_channel = *pp_channel_struct;
		}
	}
	else if (transaction_event_id == 0x80000009) {
		// Contiguously synchronize the size metadata and pointers returned by the hardware
		p_channel[9] = *(s32*)(p_pkt + 0x24); // Offset 0x24: new address or ID
		p_channel[5] = *(s32*)(p_pkt + 0x28); // Offset 0x28: block size
		p_channel[6] = *(s32*)(p_pkt + 0x2C); // Offset 0x2C: secondary attributes
	}

	// Index 2 (2 * 4 = 8 bytes) holds the ID of the semaphore assigned to the channel thread
	s32 sema_id = p_channel[2];

	if (sema_id > -1) {
		// On a real PS2 this increments the hardware semaphore from the interrupt.
		// Functionally on PC, the modern multitasking environment handles it natively:
#if defined(PLATFORM_PS2)
		iSignalSema(sema_id);
#endif
	}

	// Call the lab function to switch off the HUD node safely
	hud_disable_widget_node((u32*)*pp_channel_struct);

	// Cut the RAM link by zeroing the pointer reference
	*pp_channel_struct = NULL;
}

/**
 * @brief Deactivates a HUD widget node in memory, clearing its enable bit and its signature.
 * Original Ghidra address: 0x0011D1E8 (PAL)
 *
 * @param p_internal_node Base address of the widget node's internal block (param_1).
 */
void hud_disable_widget_node(u32* p_internal_node) {
	if (p_internal_node == NULL) {
		return;
	}

	// Offset 0x18 is index 6 in 32-bit integers (6 * 4 = 24 bytes)
	// Clears the structural validation signature
	p_internal_node[0x06] = 0;

	// Offset 0x10 is index 4 (4 * 4 = 16 bytes), which holds the state flags
	// Apply the binary mask & 0xFFFFFFFE to clear the "active node" bit at once
	p_internal_node[0x04] = p_internal_node[0x04] & 0xFFFFFFFE;
}

// Real physical definition of the SifRpcClientData structure, taken from the Sony SDK offsets
typedef struct {
	u32  is_active;       // Offset +0x00 (DAT_0013e980)
	u32  p_iop_buffer;     // Offset +0x04 (DAT_0013e984)
	s32  buffer_size;     // Offset +0x08 (DAT_0013e988)
	u32  p_gp_register;   // Offset +0x0C (DAT_0013e98c)
	u32  packet_id;       // Offset +0x10 (DAT_0013e990)
	u32  p_channel_desc;  // Offset +0x14 (DAT_0013e994)
	u32  server_id;       // Offset +0x18 (DAT_0013e998)
	u32  p_command_buff;  // Offset +0x1C (DAT_0013e99c)
	s32  command_size;    // Offset +0x20 (DAT_0013e9a0)
	u32  callback_arg;    // Offset +0x24 (DAT_0013e9a4)
} SifRpcClientData;

// The original code now compiles:
//extern SifRpcClientData g_sys_sif_rpc_client_struct;
extern u32 g_sys_sif_rpc_dummy_packet;

// References to the functions and initializers already consolidated in the repository
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
bool sys_sif_init_manager(void);
void sys_sif_register_callback(long command_id, void* callback_ptr, void* callback_arg);
u32  sys_sif_get_channel_descriptor_ptr(s32 channel_index);
//void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Native PS2 SDK support prototypes
u32  sceSifGetReg(void);
u32  sceSifSetReg(void);

// Callbacks of the asynchronous circuit
void sys_sif_rpc_on_transaction_complete(void* p_packet_res);
void sys_sif_rpc_on_query_node_status(void* p_incoming_packet, void* p_hud_container);
void sys_sif_rpc_on_queue_request(void* p_packet_req);
void sys_sif_rpc_on_data_transfer(void* p_incoming_packet, void* p_hud_container);

/**
 * @brief Fully initializes the SIF bus remote procedure call (RPC) client and server subsystem.
 * Fills in the client information exactly and registers the four interrupt callbacks.
 * Original Ghidra address: 0x0011CF78 (PAL)
 */
 // 1. Return type changed from 'bool' to 'int'
int sys_sif_rpc_init_client(void) {
	bool interrupt_status;

	interrupt_status = kernel_system_sync_guard();

	// 1. Guard against duplicate initialization
	if (g_sys_sif_rpc_is_initialized != 0) {
		if (interrupt_status) {
			kernel_system_sync_release();
		}
		return 1; // Changed true to 1
	}

	g_sys_sif_rpc_is_initialized = 1;
	kernel_system_sync_release();

	// 2. Wake the master manager of the physical DMA channels
	sys_sif_init_manager();

	kernel_system_sync_guard();

	// 3. Contiguous configuration of the physical structure
	SifRpcClientData* p_sif_client = (SifRpcClientData*)&g_sys_sif_rpc_client_struct;
	p_sif_client->is_active = 1;
	p_sif_client->p_iop_buffer = 0x2013D180;
	p_sif_client->buffer_size = 0x20;
	p_sif_client->p_gp_register = 0;
	p_sif_client->packet_id = 0;
	p_sif_client->p_channel_desc = 0x2013D980;
	p_sif_client->server_id = 0x20;
	p_sif_client->p_command_buff = 0x2013E180;
	p_sif_client->command_size = 0x20;
	p_sif_client->callback_arg = 0;

	// 4. Formal registration of the complete asynchronous circuit with auto-translated names
	sys_sif_register_callback(-0x7FFFFFF8, (void*)sys_sif_rpc_on_transaction_complete, &g_sys_sif_rpc_client_struct);
	sys_sif_register_callback(-0x7FFFFFF7, (void*)sys_sif_rpc_on_query_node_status, &g_sys_sif_rpc_client_struct);
	sys_sif_register_callback(-0x7FFFFFF6, (void*)sys_sif_rpc_on_queue_request, &g_sys_sif_rpc_client_struct);
	sys_sif_register_callback(-0x7FFFFFF4, (void*)sys_sif_rpc_on_data_transfer, &g_sys_sif_rpc_client_struct);

	kernel_system_sync_release();

	// 5. Final handshake loop with the IOP coprocessor
	long register_check = (long)sceSifGetReg();

	if (register_check == 0) {
		// --- CRASH AVOIDANCE ON PC ---
		// Writing directly to the fixed physical PS2 address 0x0013D1CC
		// would cause an immediate access violation (segmentation fault) on Windows.
		// The hard-coded pointer is replaced with a simulated global variable.
		static u32 pc_simulated_dat_0013d1cc = 0;
		pc_simulated_dat_0013d1cc = 1;

		// Send the remote RPC initialization command (0x80000002)
		sys_sif_submit_dma_packet_simple(0x80000002, &g_sys_sif_rpc_dummy_packet, 0x10, 0, 0, 0);

		// --- INFINITE LOOP AVOIDANCE ON PC ---
		// The original 'while(1)' loop waited for a physical PS2 DMA channel to set a flag.
		// On PC, without that hardware, it would freeze at 100% CPU.
		// Force the exit by simulating immediate success.
		while (1) {
			u32 channel_desc = 1; // Simulate that the channel is ready (non-zero)
			if (channel_desc != 0) {
				break;
			}
		}

#if defined(PLATFORM_PS2)
		return (int)sceSifSetReg();
#else
		return 1; // Returns success on PC
#endif
	}

	// Equivalent of the MIPS SUB81 macro, extracting the least significant byte portably
	return (int)(register_check & 0xFF);
}

// Simulated SIF state variables for the port
extern int  g_sys_sif_is_initialized;
extern int  g_sys_sif_handler_id;
extern int  g_sys_sif_reg_status;

// References to the low-level functions mapped in the lab
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
u64  sys_kernel_enable_dmac(void);
//void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Emulation stubs for native Sony SDK APIs
s32 sceAddDmacHandler(s32 channel, void* handler, s32 arg);

// Definition of the global callback descriptor tables in PS2 RAM
#define SIF_GENERAL_CALLBACK_TABLE     (*(u32*)EE_ADDR(0x0013CFEC))
#define SIF_SYSTEM_CALLBACK_TABLE      (*(u32*)EE_ADDR(0x0013CFE4))

// Definition of the physical address of the channel descriptor table in PS2 RAM
#define SIF_CHANNEL_DESCRIPTOR_TABLE_PTR   ((const u32*)EE_ADDR(0x0013D100))

/**
 * @brief Returns, by index, the pointer to the control descriptor of a SIF subsystem channel.
 * Original Ghidra address: 0x0011C8B0 (PAL)
 *
 * @param channel_index Numeric index of the SIF channel to query (param_1).
 * @return u32 Physical address or control word of the linked channel, or 0 if the table is invalid.
 */
u32 sys_sif_get_channel_descriptor_ptr(s32 channel_index) {
	if (SIF_CHANNEL_DESCRIPTOR_TABLE_PTR == NULL) {
		return 0;
	}

	// Compute the exact offset with the indexed 4-byte step (1 MIPS word)
	const u32* p_descriptor_slot = SIF_CHANNEL_DESCRIPTOR_TABLE_PTR + channel_index;

	// Return the value stored in the slot directly
	return *p_descriptor_slot;
}

// References to the helpers already integrated in the repository
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);
s32  hud_validate_widget_node_state(const u32* p_widget_handle);
void sys_kernel_wait_timer(u32 microseconds);

// Engine and SDK synchronization prototypes already mapped
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
s32  sceEnableDmac(s32 channel);

// Prototype of the cache flusher already integrated in the repository
void sys_kernel_flush_dcache_range(u32 start_addr, long block_size);

// Official Sony SDK prototypes, emulated passively
u64 sceSifSetDma(void);
u64 isceSifSetDma(void);

// Reference to the master SIF function already integrated in the repository
u64 sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len);

/**
 * @brief Flushes and synchronizes a range of the CPU data cache (D-cache) to main RAM (writeback invalidate).
 * Portably replaces the PS2's low-level cacheOp(0x18) and SYNC instructions.
 * Original Ghidra address: 0x0011CEC8 (PAL)
 *
 * @param start_addr RAM start address of the block to synchronize (param_1).
 * @param block_size Length or size in bytes of the segment to flush (param_2).
 */
void sys_kernel_flush_dcache_range(u32 start_addr, long block_size) {
	// On a real PlayStation 2 the processor must align the range to 64-byte (0x40) lines,
	// compute the step with bit shifts (>> 6 and >> 3) to run unrolled bursts,
	// and fire cacheOp(0x18, ...) interleaved with SYNC(0) memory barriers.
	// For the modern native PC port, x86_64/ARM hardware handles cache coherence
	// and DMA accesses automatically without forcing manual flushes:
	return;
}

/**
 * @brief Safely and atomically enables the DMA controller (DMAC), temporarily suspending interrupts if required.
 * Original Ghidra address: 0x0011B6C0 (PAL)
 *
 * @return u64 Previous state or configuration of the DMAC controller.
 */
u64 sys_kernel_enable_dmac(void) {
	// Simulate reading the MIPS CPU interrupt status flag
	u32 status_interrupt_flag = 1; // Force a simulated safe state for PC
	bool bVar1 = false;

	// 1. If hardware interrupts are on, take the engine's atomic lock
	if (status_interrupt_flag != 0) {
		bVar1 = kernel_system_sync_guard();
	}

	// 2. Invoke the native enable instruction. On PC, modern render threads
	// operate with native asynchronous parallel virtual memory pipelines by default:
	u64 previous_dma_state = 0;
#if defined(PLATFORM_PS2)
	previous_dma_state = (u64)sceEnableDmac(-1); // Enables every channel of the physical bus
	__asm__ volatile("sync"); // MIPS SYNC(0) instruction to drain the cache buses
#endif

	// 3. Release the CPU lock once the bus transaction has finished
	if (status_interrupt_flag != 0 && bVar1) {
		kernel_system_sync_release();
	}

	return previous_dma_state;
}

/**
 * @brief Guards the synchronization of sound and streaming flow commands. Freezes the thread if the audio hardware is busy.
 * Original Ghidra address: 0x00124C28 (PAL)
 */
s32 sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	s32 is_sound_node_active;

	// Case A: wait for the sound channel to become free before advancing
	if (command_type == 0) {
		if (DEBUG_NET_LOG_LEVEL > 0) {
			boot_txt_render_extended_string((const u8*)"S cmd wait\n", p2, p3, p4, p5, p6, p7, p8);
		}

		// Blocking loop: sleeps the thread while the PS2 sound processor reports activity
		while (1) {
			is_sound_node_active = hud_validate_widget_node_state(&SOUND_CHANNEL_WIDGET_HANDLE);
			if (is_sound_node_active == 0) {
				break;
			}

			// Pause the processor for 1 millisecond (1000 microseconds) to give the audio bus room
			sys_kernel_wait_timer(1000);
		}
		return 0;
	}

	// Case B: quick query of the sound subsystem's busy state
	return hud_validate_widget_node_state(&SOUND_CHANNEL_WIDGET_HANDLE);
}

// Prototype of the official Sony SDK API
s32 sceCreateSema(void);

/**
 * @brief Initializes and reserves the low-level synchronization semaphore objects of the PS2 kernel.
 * Prepares the I/O channel by zeroing the engine's pending-command counter.
 * Original Ghidra address: 0x00124780 (PAL)
 */
void sys_io_init_kernel_semaphores(void) {
	// Check whether the semaphore slots are uninitialized (-1)
	if (IO_LOCK_SEMA_ID == -1 || IO_WAIT_SEMA_ID == -1) {

		// On a real PS2 this calls the Sony kernel API directly.
		// Functionally, the portable PC port assigns valid logical state IDs:
#if defined(PLATFORM_PS2)
		IO_LOCK_SEMA_ID = sceCreateSema();
		IO_WAIT_SEMA_ID = sceCreateSema();
		IO_DMA_SEMA_ID = sceCreateSema();
#else
		IO_LOCK_SEMA_ID = 1;
		IO_WAIT_SEMA_ID = 2;
		IO_DMA_SEMA_ID = 3;
#endif

		// Safely reset the pending IO burst counter to zero
		IO_PENDING_COMMANDS_COUNT = 0;
	}
}

// Prototype of the main menu and intro function identified earlier
void game_main_menu_and_intro_loop(void); // FUN_002b8940

// Reference to the already integrated queue filter
u32 sys_io_queue_command_filter(u32 target_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8);

/**
 * @brief Sends the identifier of the game's intro and main menu stage to the system's logical command queue.
 * Original Ghidra address: 0x002B74F0 (PAL)
 */
u32 sys_boot_dispatch_stage(long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	// Register the address of the menu loop subroutine as the target command to dispatch
	u32 target_stage_command = (u32)((long)game_main_menu_and_intro_loop);

	// Call the conditional filter at once to guarantee a clean injection into the FPU/kernel
	return sys_io_queue_command_filter(target_stage_command, p2, p3, p4, p5, p6, p7, p8);
}

// References to the helpers already integrated in the repository
u32 sys_io_submit_command(u32 new_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8);

/**
 * @brief Safely filters and stores pending IO commands, checking the state of the global lock.
 * Prevents corrupting or losing data transitions by backing requests up in secondary reserve buffers.
 * Original Ghidra address: 0x00133730 (PAL)
 */
u32 sys_io_queue_command_filter(u32 target_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	u32 fallback_command = IO_BACKUP_COMMAND_ID;
	u32 current_command = target_command_id;

	// 1. If the global queue lock is free, dispatch the command immediately
	if (IO_QUEUE_LOCK_FLAG == 0) {
		fallback_command = sys_io_submit_command(target_command_id, p2, p3, p4, p5, p6, p7, p8);
		current_command = IO_BACKUP_COMMAND_ID;
	}

	// 2. Back up the identifier in the reserve variable for the next cycle
	IO_BACKUP_COMMAND_ID = current_command;

	return fallback_command;
}

// Definition of the active command offset in PS2 RAM
#define IO_ACTIVE_COMMAND_ID        (*(u32*)EE_ADDR(0x001418C0))

// Reference to the IO command guard
s32 sys_io_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8);

/**
 * @brief Safely sends and injects a new command into the IO read channel queue (DVD/network).
 * Applies mutual exclusion by temporarily suspending interrupts through generic engine wrappers.
 * Original Ghidra address: 0x001245D0 (PAL)
 *
 * @param new_command_id Identifier or address of the new command to dispatch (param_1).
 * @return u32 The ID of the previous command that was active on the channel.
 */
u32 sys_io_submit_command(u32 new_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	// 1. Ask the guard with type '1' whether the channel is free to receive the transaction
	s32 is_channel_busy = sys_io_sync_command_guard(1, p2, p3, p4, p5, p6, p7, p8);
	u32 previous_command_id = 0;
	bool bVar1;

	// 2. If free, perform an atomic exchange protecting the CPU thread
	if (is_channel_busy == 0) {
		bVar1 = kernel_system_sync_guard(); // Calls the generic lock wrapper

		previous_command_id = IO_ACTIVE_COMMAND_ID;
		IO_ACTIVE_COMMAND_ID = new_command_id; // Inserts the new load command

		if (bVar1) {
			kernel_system_sync_release(); // Calls the generic release wrapper
		}
	}

	return previous_command_id;
}

// Definition of the IO state variables in PS2 RAM
#define DEBUG_NET_LOG_LEVEL         (*(s32*)EE_ADDR(0x00136410))
#define IO_PENDING_COMMANDS_COUNT   (*(s32*)EE_ADDR(0x00136430))
#define IO_CHANNEL_WIDGET_HANDLE    (*(u32*)EE_ADDR(0x001375D0))

// References to the helpers already integrated in the repository
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);
s32  hud_validate_widget_node_state(const u32* p_widget_handle);
void sys_kernel_wait_timer(u32 microseconds);

/**
 * @brief Guards the synchronization of IO command loading (DVD/network). Safely freezes the thread while transfers are in progress.
 * Original Ghidra address: 0x00124B88 (PAL)
 */
s32 sys_io_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	s32 is_node_active;

	// Case A: wait for the IO channel to become completely free
	if (command_type == 0) {
		if (DEBUG_NET_LOG_LEVEL > 0) {
			boot_txt_render_extended_string((const u8*)"N cmd wait\n", p2, p3, p4, p5, p6, p7, p8);
		}

		// Blocking loop: sleeps the thread while commands are pending or the channel reports activity
		while (1) {
			is_node_active = hud_validate_widget_node_state(&IO_CHANNEL_WIDGET_HANDLE);
			if (IO_PENDING_COMMANDS_COUNT == 0 && is_node_active == 0) {
				break;
			}

			// Pause the processor for 1000 microseconds (1 millisecond) so the bus is not saturated
			sys_kernel_wait_timer(1000);
		}
		return 0;
	}

	// Case B: quick, instant query of the channel's busy state
	s32 query_status = 1;
	if (IO_PENDING_COMMANDS_COUNT == 0) {
		is_node_active = hud_validate_widget_node_state(&IO_CHANNEL_WIDGET_HANDLE);
		query_status = 1;
		if (is_node_active == 0) {
			query_status = 0; // The channel is completely free and available
		}
	}

	return query_status;
}

/**
 * @brief Pauses the execution thread for a precise number of microseconds through PS2 kernel calls.
 * Portably replaces the console's creation, wait and destruction of semaphores/alarms.
 * Original Ghidra address: 0x00124568 (PAL)
 *
 * @param microseconds Exact amount of time to pause the CPU thread.
 */
void sys_kernel_wait_timer(u32 microseconds) {
	// On a real PlayStation 2 a conditional semaphore must be created with sceCreateSema,
	// an interrupt alarm attached with sceSetAlarm, the thread suspended with sceWaitSema
	// and the kernel resources cleaned up with sceDeleteSema on wake-up.
	// Functionally and portably on modern PCs, the pause is delegated to the operating system:

	if (microseconds == 0) {
		return;
	}

	struct timespec requested_time;
	// Convert microseconds to standard C seconds and nanoseconds
	requested_time.tv_sec = microseconds / 1000000;
	requested_time.tv_nsec = (microseconds % 1000000) * 1000;

	// Pause the current thread natively without consuming PC CPU cycles
	nanosleep(&requested_time, NULL);
}

// Remaining static global variables of PS2 debugging
s32  g_debug_console_char_count = 0;
char g_debug_console_static_buffer[128];
u8   g_debug_console_overflow_flag = 0;

// Prototype of the debug helper from the previous step
void sys_init_deci2_debug_link(void);

/**
 * @brief Writes one character into the static buffer of the debug console.
 * Flushes automatically through the hardware channel when a newline (\n) or an overflow is detected.
 * Original Ghidra address: 0x0011BF18 (PAL)
 *
 * @param character Byte or ASCII character to process (param_1).
 */
void sys_debug_console_write_char(s32 character) {
	s32 current_idx = g_debug_console_char_count;

	// 1. Safety check against buffer overflow (limit 125 characters)
	if (g_debug_console_char_count > 0x7D) {
		g_debug_console_char_count = 0;
		g_debug_console_overflow_flag = 0;
		sys_init_deci2_debug_link();
		current_idx = g_debug_console_char_count;
	}

	// 2. A regular character is accumulated sequentially in the array
	if (character != 10) { // 10 = '\n'
		g_debug_console_char_count = current_idx + 1;
		g_debug_console_static_buffer[current_idx] = (char)character;

		// Native real-time emulation on the PC console for development:
		fputc(character, stderr);
		return;
	}

	// 3. On a newline (\n), close the string and dispatch the complete message
	g_debug_console_char_count = 0;
	g_debug_console_static_buffer[current_idx] = 10;
	g_debug_console_static_buffer[current_idx + 1] = '\0'; // Safety null terminator

	// Portable redirection to the modern standard output channel
	fputc('\n', stderr);
	fflush(stderr);

	sys_init_deci2_debug_link();
}


// Global address definitions mapped from the Ghidra capture
#define GLOBAL_THREAD_STATE_ID     (*(u32*)EE_ADDR(0x001A6464))
#define GLOBAL_PAL_FRAME_RATE      (*(u32*)EE_ADDR(0x001A6468))
#define GLOBAL_INTRO_MANAGER_PTR   (*(u32*)EE_ADDR(0x001A646C))
#define GLOBAL_PLANET_LOAD_FLAG    (*(u32*)EE_ADDR(0x001A6470))
#define GLOBAL_INVENTORY_BASE_PTR  (*(u32*)EE_ADDR(0x001A64B4))

// Internal hardware prototypes of the PS2 compiler
void ee_fpu_setup_init(void); // FUN_0026f438
s32  custom_vsprintf_engine_alt(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list); // approximately FUN_002b74f0
// Definition of the global dynamic typographic pointer in PS2 RAM
void* g_hud_typography_callback_ptr = (void*)EE_ADDR(0x00134718);

// Required prototypes of the mapped string ecosystem
bool txt_render_scientific_string(const u8* p_src_str, float* p_args_stack);
void custom_hud_glyph_decoder(void); // approximately FUN_0011bf18

/**
 * @brief Validates the integrity and activation state of a HUD widget node in memory.
 * Runs a pointer consistency test and queries the active enable bit mask.
 * Original Ghidra address: 0x0011D810 (PAL)
 *
 * @param p_widget_handle Pointer to the handle of the interface component structure (param_1).
 * @return s32 Returns 1 if the node is valid and active in the graphics pipeline, or 0 if it is corrupt or disabled.
 */
s32 hud_validate_widget_node_state(const u32* p_widget_handle) {
	if (p_widget_handle == NULL) {
		return 0;
	}

	// Retrieve the base address of the node's internal block (index 0, +0 bytes)
	u32 p_internal_node = *p_widget_handle;

	if (p_internal_node != 0) {
		// Offset 0x18 is index 6 in 32-bit integers (6 * 4 = 24 bytes)
		u32* p_node_struct = (u32*)p_internal_node;
		u32 structural_signature = p_node_struct[6];

		// Check whether the secondary identifier (+4 bytes) matches the node signature
		if (p_widget_handle[1] == structural_signature) {
			// Offset 0x10 is index 4 (4 * 4 = 16 bytes), which holds the state flags
			u32 node_flags = p_node_struct[4];

			// Use a binary mask to check whether the "active/enabled node" bit is set
			if ((node_flags & 1) != 0) {
				return 1; // The node is fully intact and safe to process
			}
		}
	}

	return 0;
}

/**
 * @brief Initializes the PS2 kernel's DECI2 hardware communication and debug channel.
 * On the real console it opens the link with the development PC to send failure reports.
 * Original Ghidra address: 0x0011BAA0 (PAL)
 */
void sys_init_deci2_debug_link(void) {
	// On a real PlayStation 2 this calls sceDeci2Open() of the Sony SDK.
	// For the native PC port, the physical interfaces of the TOOL kit do not apply,
	// so the link is initialized passively and safely without hanging the thread:
	return;
}

/**
 * @brief Typographic wrapper that temporarily changes the global callback to render strings with special characters/icons.
 * Original Ghidra address: 0x0011C820 (PAL)
 *
 * @param p_src_str Character string to process and format (param_1).
 * @param param_2 Block of variable arguments packed on the stack.
 * @return bool Returns true if the scientific/extended formatting succeeded.
 */
bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4,
	long param_5, long param_6, long param_7, long param_8) {
	if (p_src_str == NULL) {
		return false;
	}

	// Contiguous local stack structure emulating the register dump in uStack_38
	long local_args_stack[7];
	local_args_stack[0] = param_2;
	local_args_stack[1] = param_3;
	local_args_stack[2] = param_4;
	local_args_stack[3] = param_5;
	local_args_stack[4] = param_6;
	local_args_stack[5] = param_7;
	local_args_stack[6] = param_8;

	// 1. Back up the conventional typographic callback on the stack
	void* p_previous_callback = *(void**)g_hud_typography_callback_ptr;

	// 2. Intercept and inject Insomniac's extended glyph decoder (FUN_0011bf18)
	*(void**)g_hud_typography_callback_ptr = (void*)custom_hud_glyph_decoder;

	// 3. Render the string, processing the special tokens with the new glyph set
	bool render_status = txt_render_scientific_string(p_src_str, (float*)local_args_stack);

	// 4. Safely restore the previous callback to clean the graphics pipeline
	*(void**)g_hud_typography_callback_ptr = p_previous_callback;

	return render_status;
}
/**
 * @brief Root static initialization function of the graphics engine (pre-main core setup).
 * Prepares the thread flags and video refresh rates and clears the global control pointers.
 * Original Ghidra address: 0x002B7480 (PAL)
 *
 * @return s32 Status code or result of the engine's execution loop.
 */
s32 engine_boot_static_init(void) {
	// 1. Initialize the low-level FPU hardware configuration
	// ee_fpu_setup_init(); // Equivalent to FUN_0026f438()

	// 2. Set the initial factory conditions of the global variables in RAM
	GLOBAL_THREAD_STATE_ID = 0xFFFFFFFF;
	GLOBAL_PAL_FRAME_RATE = 0x20; // 32 decimal

	// Contiguous zeroing of the interface and level game managers
	GLOBAL_INTRO_MANAGER_PTR = 0;
	GLOBAL_PLANET_LOAD_FLAG = 0;
	*(u32*)0x001A6474 = 0; // DAT_001a6474
	*(u32*)0x001A6478 = 0; // DAT_001a6478
	*(u32*)0x001A6488 = 0; // DAT_001a6488
	*(u32*)0x001A6490 = 0; // DAT_001a6490
	*(u32*)0x001A64A4 = 0; // DAT_001a64a4
	*(u32*)0x001A64B0 = 0; // DAT_001a64b0
	GLOBAL_INVENTORY_BASE_PTR = 0;
	*(u32*)0x002A72B8 = 0; // DAT_002a72b8

	// 3. Conditionally call the alternate engine, passing it the stack arguments
	// For functional purposes on PC/modern platforms, the direct logical return is emulated:
	s32 boot_status = 0;

	// (The call on line 28, uVar1 = FUN_002b74f0, is resolved in a unified way in the main loop)
	return boot_status;
}
