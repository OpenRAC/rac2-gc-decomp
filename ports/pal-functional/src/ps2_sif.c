#include "types.h"
#include "ps2_kernel.h"
#include "ps2_sif.h"

// Global state variables of the SIF bus tables mapped in PS2 RAM
#define SIF_GENERAL_CALLBACK_TABLE     (*(u32*)0x0013CFEC)
#define SIF_SYSTEM_CALLBACK_TABLE      (*(u32*)0x0013CFE4)

// Definition of the global interrupt variables mapped in PS2 RAM
#define IO_INTERRUPT_CALLBACK       (*(void(**)(u32))(long)0x001418C4)
#define IO_INTERRUPT_ARGUMENT       (*(u32*)0x001418C8)

/**
 * @brief Low-level interrupt handler (callback) of the SIF IO bus.
 * Evaluates the reconfiguration flags and runs the registered dynamic callback, passing its metadata.
 * Original Ghidra address: 0x001248B8 (PAL)
 */
void sys_io_iop_interrupt_handler(void) {
	// 1. Protective filter: if the channel is being reconfigured, abort the dispatch
	if (IO_INTERRUPT_CALLBACK != NULL && g_sys_io_reconfig_flag == 0) {

		// Safely store the descriptors in local variables before the jump
		void (*p_callback)(u32) = IO_INTERRUPT_CALLBACK;
		u32 callback_arg = IO_INTERRUPT_ARGUMENT;

		// 2. MASTER IO TRIGGER: runs the background response subroutine portably
		p_callback(callback_arg);
	}
}

// Simulated SIF state variables for the portable port environment
u8   g_sys_sif_is_initialized = 0;
u32  g_sys_sif_handler_id = 0;
u32  g_sys_sif_reg_status = 0;

/**
 * @brief Packs and dispatches an asynchronous data transfer transaction over the SIF hardware bus (DMA).
 * Original Ghidra address: 0x0011CBE8 (PAL)
 */
u64 sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len) {
	if (packet_size - 16 > 0x60) {
		return 0;
	}

	s32 calculated_offset = 0;
	/* SIF command header (PS2SDK SifCmdHeader): [0] psize:8 | dsize:24, [1] dest, [2] cid, [3] opt */
	if (transfer_len < 1) {
		p_packet_header[1] = 0;
		*p_packet_header = (u32)(u8)(*p_packet_header);
	}
	else {
		p_packet_header[1] = dest_addr;
		calculated_offset = 1;
		*p_packet_header = (u32)(u8)(*p_packet_header) | ((u32)transfer_len << 8);

		if ((sync_flags & 4) != 0) {
			calculated_offset = 0x10;
			goto finalize_packet;
		}
	}
	calculated_offset = calculated_offset << 4;

finalize_packet:
	p_packet_header[2] = command_type;
	*(u8*)p_packet_header = (u8)packet_size;

	u64 transaction_id;
	if ((sync_flags & 1) == 0) {
#if defined(PLATFORM_PS2)
		transaction_id = sceSifSetDma();
#else
		transaction_id = 1;
#endif
	}
	else {
#if defined(PLATFORM_PS2)
		transaction_id = isceSifSetDma();
#else
		transaction_id = 1;
#endif
	}
	return transaction_id;
}

/**
 * @brief Simplified convenience wrapper that dispatches asynchronous packets on the SIF DMA bus with flags set to 0.
 * Original Ghidra address: 0x0011CD20 (PAL)
 */
void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len) {
	sys_sif_submit_dma_packet(command_type, 0, p_packet_header, packet_size, src_addr, dest_addr, transfer_len);
}

/**
 * @brief Convenience wrapper that dispatches priority synchronous packets on the SIF DMA bus with flags set to 1.
 * Original Ghidra address: 0x0011CD60 (PAL)
 */
void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len) {
	sys_sif_submit_dma_packet(command_type, 1, p_packet_header, packet_size, src_addr, dest_addr, transfer_len);
}

/**
 * @brief Fully configures and initializes the SIF subsystem manager and the asynchronous DMA channels.
 * Original Ghidra address: 0x0011C8D8 (PAL)
 */
bool sys_sif_init_manager(void) {
	bool interrupt_status = kernel_system_sync_guard();

	if (g_sys_sif_is_initialized != 0) {
		if (interrupt_status) {
			kernel_system_sync_release();
		}
		return true;
	}
	g_sys_sif_is_initialized = 1;

	if (interrupt_status) {
		kernel_system_sync_release();
	}

#if defined(PLATFORM_PS2)
	g_sys_sif_handler_id = sceAddDmacHandler(5, (void*)0x0011C8A0, 0);
#endif
	sys_kernel_enable_dmac();

	u32 handshake_reg = 1;
	if (handshake_reg != 0) {
		g_sys_sif_reg_status = handshake_reg;
		u32 dummy_payload = 0;
		sys_sif_submit_dma_packet_simple(0x80000000, &dummy_payload, 0x14, 0, 0, 0);
		return true;
	}
	return true;
}

/**
 * @brief Registers a function pointer (callback) and its arguments in the indexed SIF bus event table.
 * Original Ghidra address: 0x0011CB90 (PAL)
 */
void sys_sif_register_callback(long command_id, void* callback_ptr, void* callback_arg) {
	u32 table_base_address = SIF_GENERAL_CALLBACK_TABLE;
	if (command_id < 0) {
		table_base_address = SIF_SYSTEM_CALLBACK_TABLE;
	}
	u32* p_callback_slot = (u32*)(long)((s32)command_id * 8 + table_base_address);
	p_callback_slot[0] = (u32)(long)callback_ptr;
	p_callback_slot[1] = (u32)(long)callback_arg;
}

/**
 * @brief Removes and unregisters a callback from the indexed SIF bus event table by storing a null pointer.
 * Original Ghidra address: 0x0011CBC0 (PAL)
 */
void sys_sif_unregister_callback(long command_id) {
	u32 table_base_address = SIF_GENERAL_CALLBACK_TABLE;
	if (command_id < 0) {
		table_base_address = SIF_SYSTEM_CALLBACK_TABLE;
	}
	u32* p_callback_slot = (u32*)(long)((s32)command_id * 8 + table_base_address);
	*p_callback_slot = 0;
}

// Definition of the global IO control flags mapped in PS2 RAM
#define IO_INTERRUPT_ACTIVE_FLAG    (*(s32*)0x00136414)
#define IO_THREAD_RESET_DESCRIPTOR  (*(s32*)0x00136454)

/**
 * @brief Shuts down, dismantles and fully releases the resources and semaphores of the Input/Output (IO) subsystem.
 * Removes the -0x7fffffee callback from the SIF bus and destroys the three physical synchronization semaphores.
 * Original Ghidra address: 0x00124818 (PAL)
 *
 * @return bool Returns true if the atomic dismantling in the kernel succeeded.
 */
bool sys_io_shutdown_subsystem(void) {
	// 1. Pre-emptively wake the blocked threads before shutting down
	if (IO_INTERRUPT_ACTIVE_FLAG != 0) {
		IO_THREAD_RESET_DESCRIPTOR = 0xFFFFFFFF;
		sceSignalSema(g_sys_io_wait_sema_id);
	}

	// 2. Chained physical destruction of the three kernel subsystem semaphores
	sceDeleteSema(g_sys_io_lock_sema_id);
	sceDeleteSema(g_sys_io_wait_sema_id);
	sceDeleteSema(g_sys_io_dma_sema_id);

	// Reset the local global control identifiers to mark the inactive state
	g_sys_io_lock_sema_id = -1;
	g_sys_io_wait_sema_id = -1;
	g_sys_io_dma_sema_id = -1;

	// 3. Atomic mutual exclusion to unregister the SIF callback from the IOP queue
	bool sync_status = kernel_system_sync_guard();

	sys_sif_unregister_callback(-0x7FFFFFEE);

	if (!sync_status) {
		return false;
	}

	kernel_system_sync_release();
	return true;
}

// Definition of the global IO variables mapped in PS2 RAM
#define IO_RECONFIG_FLAG            (*(s32*)0x00136424)
#define IO_IS_READY_FLAG            (*(s32*)0x0013643C)

/**
 * @brief Initializes and configures the asynchronous Input/Output (IO) service channel in the kernel.
 * Registers the -0x7fffffee callback in the SIF bus table under atomic mutual exclusion.
 * Original Ghidra address: 0x001248F8 (PAL)
 *
 * @return s32 Success status code (1).
 */
s32 sys_io_init_subsystem(void) {
	// 1. Mark the physical reconfiguration state in RAM
	IO_RECONFIG_FLAG = 1;

	// 2. Protect the bus by registering the IOP interrupt handler thread-safely
	bool sync_status = kernel_system_sync_guard();

	sys_sif_register_callback(-0x7FFFFFEE, (void*)sys_io_iop_interrupt_handler, NULL);

	if (sync_status) {
		kernel_system_sync_release();
	}

	// 3. Release the configuration flag and set the channel availability flag
	IO_RECONFIG_FLAG = 0;
	IO_IS_READY_FLAG = 1;

	return 1; // Pipeline initialized successfully
}

// Definition of the static DVD reader registers and buffers mapped in PS2 RAM
#define CDVD_INIT_MODE_BUFFER_PTR   (*(u32*)0x00141B40)
#define CDVD_BACKUP_METADATA_1      (*(u32*)0x00136440)
#define CDVD_BACKUP_METADATA_2      (*(u32*)0x00136438)
#define CDVD_BACKUP_METADATA_3      (*(u32*)0x00136448)
#define CDVD_BACKUP_METADATA_4      (*(u32*)0x00136444)
#define CDVD_BACKUP_STATUS_1        (*(u32*)0x00136434)
#define CDVD_BACKUP_STATUS_2        (*(s32*)0x0013644C)
#define DEBUG_NET_LOG_LEVEL         (*(s32*)0x00136410)

// Global DVD subsystem variables mapped from Ghidra
s32 g_sys_cdvd_thread_owner_id = 0;
s32 g_sys_cdvd_init_attempts_count = 0;
u32 g_sys_cdvd_channel_widget_handle = 0;
s32 g_sys_cdvd_is_bound_flag = 0;

/**
 * @brief Initializes and mounts the DVD reader file system (libcdvd) through SIF RPC transactions.
 * Registers channel 0x80000592 and orchestrates the hot start or shutdown of the Input/Output plumbing.
 * Original Ghidra address: 0x00124E08 (PAL)
 */
u32 sys_cdvd_init_filesystem(s32 init_mode) {
	// 1. Check the availability of the synchronization bus
	s32 is_sound_busy = sys_sound_sync_command_guard(1, 0, 0, 0, 0, 0, 0, 0);
	u32 status_code = 0;

	if (is_sound_busy == 0) {
		sys_sif_rpc_init_client();

		g_sys_cdvd_thread_owner_id = sceGetThreadId();
		g_sys_io_reconfig_flag = 1;
		g_sys_cdvd_init_attempts_count = g_sys_cdvd_init_attempts_count + 1;

		// Contiguous burst initialization of the factory control masks
		g_sys_io_is_ready_flag = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_1 = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_2 = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_3 = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_4 = 0xFFFFFFFF;
		CDVD_BACKUP_STATUS_1 = 0;
		CDVD_BACKUP_STATUS_2 = 0xFFFFFFFF;

		// 2. LINK WAIT LOOP: binds the reader's exclusive channel (command 0x80000592)
		while (1) {
			while (1) {
				s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_cdvd_channel_widget_handle, 0x80000592, 0);
				if (session_status >= 0) {
					break;
				}

				// If the physical reader is delayed, emit the panic log
				if (DEBUG_NET_LOG_LEVEL > 0) {
					boot_txt_render_extended_string((const u8*)"Libcdvd bind err %d CD_Init %d\n", session_status, g_sys_cdvd_init_attempts_count, 0, 0, 0, 0, 0);
				}

				s32 delay_counter = 0x100000;
				while (delay_counter != -1) { delay_counter--; }
			}

			if (g_sys_cdvd_is_bound_flag != 0) {
				break;
			}

			s32 delay_counter = 0x100000;
			while (delay_counter != -1) { delay_counter--; }
		}

		CDVD_BACKUP_STATUS_2 = 0;
		CDVD_INIT_MODE_BUFFER_PTR = (u32)init_mode;

		// Ensure physical coherence of the mode buffer before dispatching
		sys_kernel_flush_dcache_range(0x00141B40, 4);

		// 3. MOUNT COMMAND DISPATCH: transfers the reader initialization block (command 0)
		s32 transaction_status = sys_sif_rpc_send_transaction_data(
			&g_sys_cdvd_channel_widget_handle,
			0, 0, 0x00141B40, 4, 0x00137600, 0x10, 0, 0
		);

		// 4. PHASE BRANCH OF THE INPUT/OUTPUT ENVIRONMENT
		if (transaction_status < 0) {
			g_sys_io_reconfig_flag = 0;
			status_code = 0;
		}
		else {
			g_sys_io_reconfig_flag = 0;
			status_code = 2;

			// If the mode is not a hot shutdown (exit mode = 5), raise the IO gates
			if ((init_mode < 0 || init_mode < 2) || init_mode != 5) {
				sys_io_init_kernel_semaphores();
				sys_io_init_subsystem();
			}
			// If the engine orders ejecting or shutting down the reader service, unmount the system
			else {
				if (DEBUG_NET_LOG_LEVEL > 0) {
					boot_txt_render_extended_string((const u8*)"Libcdvd Exit\n", 0, 0xFFFFFFFF, 0, 0, 0, 0, 0);
				}
				sys_io_shutdown_subsystem();
				g_sys_io_lock_sema_id = 0xFFFFFFFF;
				g_sys_io_wait_sema_id = 0xFFFFFFFF;
				g_sys_io_dma_sema_id = 0xFFFFFFFF;
			}
		}
	}

	return status_code;
}

// Definition of the static disc-check registers and buffers mapped in PS2 RAM
#define CDVD_READY_MODE_BUFFER_VAL  (*(u32*)0x00141B50)
#define CDVD_READY_STATUS_BACKUP    (*(s32*)0x00136444)

// Global variables of the secondary libcdvd channel mapped from Ghidra
u32 g_sys_cdvd_ready_channel_handle = 0;
s32 g_sys_cdvd_ready_is_bound_flag = 0;

/**
 * @brief Queries the readiness and physical presence of the disc in the reader (sceCdDiskReady wrapper).
 * Opens the asynchronous channel 0x8000059A and synchronously dispatches the status command to the IOP.
 * Original Ghidra address: 0x001250E8 (PAL)
 *
 * @param check_mode Hardware check mode sent to the Sony reader (param_1).
 * @return u32 Read status (0 for full success / media ready, 6 for active wait, 0xFFFFFFFF for error).
 */
u32 sys_cdvd_check_disk_ready(long check_mode) {
	// 1. Diagnostic log for the sensor start
	if (DEBUG_NET_LOG_LEVEL > 0) {
		boot_txt_render_extended_string((const u8*)"DiskReady 0\n", 0, 0, 0, 0, 0, 0, 0);
	}

	sys_io_init_kernel_semaphores();
	s32 sema_status = scePollSema(g_sys_io_wait_sema_id);
	u32 return_value = 6; // Default state: active wait / retry

	if (g_sys_io_wait_sema_id == sema_status) {
		s32 is_sound_busy = sys_sound_sync_command_guard(1, 0, 0, 0, 0, 0, 0, 0);

		if (is_sound_busy == 0) {
			sys_sif_rpc_init_client();

			// 2. LINK WAIT LOOP: binds the reader's secondary channel (command 0x8000059A)
			if (CDVD_READY_STATUS_BACKUP < 0) {
				while (1) {
					while (1) {
						s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_cdvd_ready_channel_handle, 0x8000059A, 0);
						if (session_status >= 0) {
							break;
						}

						if (DEBUG_NET_LOG_LEVEL > 0) {
							boot_txt_render_extended_string((const u8*)"Libcdvd bind err CdDiskReady\n", 0, 0, 0, 0, 0, 0, 0);
						}

						s32 delay_counter = 0x100000;
						while (delay_counter != -1) { delay_counter--; }
					}

					if (g_sys_cdvd_ready_is_bound_flag != 0) {
						break;
					}

					s32 delay_counter = 0x100000;
					while (delay_counter != -1) { delay_counter--; }
				}
				CDVD_READY_STATUS_BACKUP = 0;
			}

			// 3. SIF COMMAND DISPATCH: transfers the mode code (offset 0x141B50)
			CDVD_READY_MODE_BUFFER_VAL = (u32)check_mode;
			sys_kernel_flush_dcache_range(0x00141B50, 4);

			s32 transaction_status = sys_sif_rpc_send_transaction_data(
				&g_sys_cdvd_ready_channel_handle,
				0, 0, 0x00141B50, 4, 0x00137600, 4, 0, 0
			);

			// 4. If the reader confirms the physical media is spinning and intact, return success
			if (transaction_status > -1) {
				if (DEBUG_NET_LOG_LEVEL > 0) {
					boot_txt_render_extended_string((const u8*)"DiskReady ended\n", 0, 0, 0, 0, 0, 0, 0);
				}
				sceSignalSema(g_sys_io_wait_sema_id);
				return 0; // The disc is ready to transfer data
			}
		}

		// Escape case or congested channel: release the semaphore to avoid deadlocks
		sceSignalSema(g_sys_io_wait_sema_id);
		return_value = 6;
		if (check_mode == 8) {
			return_value = 0xFFFFFFFF; // Critical error code of the Sony reader
		}
	}

	// For the modern native PC port, where the local files are on the hard disk,
	// intercept and force full success automatically to go straight to the reads:
#if !defined(PLATFORM_PS2)
	return_value = 0;
#endif

	return return_value;
}

// Definition of the read descriptor variables mapped in PS2 RAM
#define MC_READ_FD_VAL              (*(u32*)0x00141C04)
#define MC_READ_SIZE_VAL            (*(u32*)0x00141C08)
#define MC_READ_BUFFER_PTR          (*(u32*)0x00141BA8)
#define MC_READ_LEN_VAL             (*(u32*)0x00141BAC)
#define MC_READ_OFFSET_VAL          (*(u32*)0x00141BB0)

/**
 * @brief Sends the block-read command of a Memory Card file (command 1) to the hardware bus.
 * Configures the destination buffers, offsets and sizes, applying dcache flushes and synchronous interrupts.
 * Original Ghidra address: 0x00127CC0 (PAL)
 *
 * @param file_descriptor File identifier (FD) previously obtained with sceMcOpen (param_1).
 * @param read_size Maximum number of bytes requested for reading the save game (param_2).
 * @param p_dest_buffer RAM pointer where the read data is stored (param_3).
 * @param block_len Length in bytes of the physical burst block (param_4).
 * @param offset_pos Byte offset or alignment of the cursor within the file (param_5).
 * @return s32 Status code (0 if the command was injected into the bus successfully, negative values on error).
 */
s32 sceMcRead(u32 file_descriptor, u32 read_size, long p_dest_buffer, long block_len, long offset_pos) {
	s32 status_code;

	// 1. Check that the Memory Card subsystem is formally up
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protect the bus with a non-blocking poll of the mutual-exclusion semaphore
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	if (sema_status < 0) {
		return -200;
	}

	// 3. Write the read parameters contiguously into the static data section
	*(u32*)0x00141C1C = 0x00142080; // Base address of the control table
	*(u32*)0x00141C14 = (u32)(p_dest_buffer != 0);
	*(u32*)0x00141C10 = (u32)(block_len != 0);
	*(u32*)0x00141C0C = (u32)(offset_pos != 0);

	MC_READ_BUFFER_PTR = (u32)p_dest_buffer;
	MC_READ_LEN_VAL = (u32)block_len;
	MC_READ_OFFSET_VAL = (u32)offset_pos;
	MC_READ_FD_VAL = file_descriptor;
	MC_READ_SIZE_VAL = read_size;

	// Synchronize the physical control buffer to main memory (0xC0 = 192 bytes)
	sys_kernel_flush_dcache_range(0x00142080, 0xC0);

	// Dispatch the order with the command 1 burst (priority synchronous = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		1, 1, 0x141C00, 0x30, 0x143140, 4, 0x127C68, (u32)0x00142080
	);

	// 4. If the SIF bus injection succeeded, sign the active command in RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 1;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}

// Definition of the write descriptor variables mapped in PS2 RAM
#define MC_WRITE_FD_VAL             (*(u32*)0x00141C00)
#define MC_WRITE_SRC_PTR            (*(u32*)0x00141C18)
#define MC_WRITE_SIZE_VAL           (*(u32*)0x00141C0C)

/**
 * @brief Sends the block-write command of a file to the Memory Card (command 5) over the hardware bus.
 * Configures the source RAM addresses and burst lengths, and applies two safety dcache flushes.
 * Original Ghidra address: 0x00127888 (PAL)
 *
 * @param file_descriptor File identifier (FD) previously obtained with sceMcOpen (param_1).
 * @param src_ram_addr Game RAM address from which the data to save is read (param_2).
 * @param write_size Exact number of binary bytes to inject and write to the card (param_3).
 * @return s32 Status code (0 if the bus accepted the command, negative values on error).
 */
s32 sceMcWrite(u32 file_descriptor, u32 src_ram_addr, long write_size) {
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

	// 3. Write the write parameters contiguously into the static data section
	*(u32*)0x00141C1C = 0x00142080; // Address of the control superstructure
	MC_WRITE_SIZE_VAL = (u32)write_size;
	MC_WRITE_FD_VAL = file_descriptor;
	MC_WRITE_SRC_PTR = src_ram_addr;

	// DOUBLE MEMORY COHERENCE BARRIER (D-CACHE FLUSH)
	// Synchronizes the game's source data buffer
	sys_kernel_flush_dcache_range(src_ram_addr, write_size);
	// Synchronizes the kernel's own structural control buffer (0xC0 = 192 bytes)
	sys_kernel_flush_dcache_range(0x00142080, 0xC0);

	// Dispatch the order with the command 5 burst (priority synchronous = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		5, 1, 0x141C00, 0x30, 0x143140, 4, 0x1277F8, (u32)0x00142080
	);

	// 4. If the SIF bus injection succeeded, sign the active command in RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 5;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}

// Definition of the extended write descriptor variables in PS2 RAM
#define MC_EXT_ALIGNMENT_OFFSET    (*(u32*)0x00141C14)
#define MC_EXT_ALIGNED_BUFFER_PTR  ((u8*)0x00141C20)

/**
 * @brief Creates a new directory or active working folder on the Memory Card (command 0x11).
 * Configures the slot and the folder path, sending the order to the IOP as a priority synchronous request.
 * Original Ghidra address: 0x00128180 (PAL)
 *
 * @param slot_index Card slot to query (0 = slot 1, 1 = slot 2) (param_1).
 * @param p_dir_path Text string with the name of the folder to create (param_2).
 * @return s32 Status code (0 if the bus accepted the command, negative values on error).
 */
s32 sceMcMkdir(u32 slot_index, const char* p_dir_path) {
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

	// 3. Write the creation parameters contiguously into the static data section
	g_sys_mc_read_fd = slot_index;
	g_sys_mc_read_size = (u32)(uintptr_t)p_dir_path; // Reuses the DAT_00141c08 buffer for the text pointer

	// Dispatch the order with the command 0x11 burst (priority synchronous = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		0x11, 1, 0x141C00, 0x30, 0x143140, 4, 0, 0
	);

	// 4. If the SIF bus injection succeeded, sign the active command in RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 0x11;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}

// Definition of the delete descriptor variables mapped in PS2 RAM
#define MC_DELETE_SLOT_VAL          (*(u32*)0x00141C30)
#define MC_DELETE_CONTEXT_VAL       (*(u32*)0x00141C34)
#define MC_DELETE_FILENAME_BUFFER   ((u8*)0x00141C44)

/**
 * @brief Sends the delete command of a Memory Card file (command 0x0F) to the hardware bus.
 * Validates the file name, performs a safe copy and dispatches the transaction to the IOP.
 * Original Ghidra address: 0x00128068 (PAL)
 *
 * @param slot_index Card slot to query (0 = slot 1, 1 = slot 2) (param_1).
 * @param context_val Secondary numeric context parameter of the SDK (param_2).
 * @param p_filename_path Text string with the name of the binary file to delete from the card (param_3).
 * @return s32 Status code (0 if the bus accepted the command, negative values on error).
 */
s32 sceMcDelete(u32 slot_index, u32 context_val, const char* p_filename_path) {
	s32 status_code;

	// 1. Check that the Memory Card subsystem is formally up
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protect the bus with a non-blocking poll of the channel semaphore
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	s32 is_busy_err = -200;

	if (sema_status > -1) {
		// 3. Strict check of the file string (SCE_MC_ERR_NAME safety filter)
		if (p_filename_path == NULL || p_filename_path[0] == '\0') {
			sceSignalSema(g_sys_mc_mutex_sema_id);
			return -0xD2; // Error: invalid or null file name
		}

		// Perform the safe copy with the local strncpy clone (limit 0x3FF bytes)
		sys_strncpy_safe((u32)(uintptr_t)MC_DELETE_FILENAME_BUFFER, p_filename_path, 0x3FF);

		// Clear the contiguous metadata of the physical Sony kernel structure
		*(u8*)0x00142043 = 0; // DAT_00142043
		*(u32*)0x00141C38 = 0; // DAT_00141c38

		// Write the delete descriptors into the static data section
		MC_DELETE_SLOT_VAL = slot_index;
		MC_DELETE_CONTEXT_VAL = context_val;

		// Dispatch the order with the command 0x0F burst (priority synchronous = 1, block size 0x414)
		is_busy_err = sys_sif_rpc_send_transaction_data(
			&g_sys_mc_channel_widget_handle,
			0x0F, 1, 0x141C30, 0x414, 0x143140, 4, 0, 0
		);

		// 4. If the SIF bus injection succeeded, sign the active command in RAM
		if (is_busy_err == 0) {
			g_sys_mc_active_command_id = 0x0F;
		}
		else {
			sceSignalSema(g_sys_mc_mutex_sema_id);
		}
	}

	return is_busy_err;
}

// Definitions of the shared Memory Card buffer offsets (already mapped in the suite)
#define MC_GETDIR_SLOT_VAL          (*(u32*)0x00141C30)
#define MC_GETDIR_CONTEXT_VAL       (*(u32*)0x00141C34)
#define MC_GETDIR_MAX_ENTRIES       (*(u32*)0x00141C38)
#define MC_GETDIR_PATTERN_BUFFER    ((u8*)0x00141C44)

/**
 * @brief Sends the Memory Card directory scan and listing command (command 2) to the hardware bus.
 * Configures the search patterns and entry limits and dispatches the transaction synchronously to the IOP.
 * Original Ghidra address: 0x00127508 (PAL)
 *
 * @param slot_index Card slot to query (0 = slot 1, 1 = slot 2) (param_1).
 * @param context_val Secondary numeric context parameter of the SDK (param_2).
 * @param p_search_pattern Text string with the pattern or filter of files to scan (param_3).
 * @param max_entries Maximum number of records or entries to list in the transaction (param_4).
 * @return s32 Status code (0 if the command was injected into the bus successfully, negative values on error).
 */
s32 sceMcGetDir(u32 slot_index, u32 context_val, const char* p_search_pattern, u32 max_entries) {
	s32 status_code;

	// 1. Check that the Memory Card subsystem is formally up
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protect the bus with a non-blocking poll of the channel semaphore
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	s32 is_busy_err = -200;

	if (sema_status > -1) {
		// 3. Safety check of the string (SCE_MC_ERR_NAME filter)
		if (p_search_pattern == NULL || p_search_pattern == '\0') {
			sceSignalSema(g_sys_mc_mutex_sema_id);
			return -0xD2; // Error: invalid or empty search pattern
		}

		// Perform the safe copy into the shared buffer with the vector utility
		sys_strncpy_safe(0x00141C44, p_search_pattern, 0x3FF);

		// Clear the contiguous metadata of the physical Sony control structure
		*(u8*)0x00142043 = 0;

		// Write the scan descriptors into the static data section
		MC_GETDIR_SLOT_VAL = slot_index;
		MC_GETDIR_CONTEXT_VAL = context_val;
		MC_GETDIR_MAX_ENTRIES = max_entries;

		// Dispatch the order with the command 2 burst (priority synchronous = 1, size 0x414)
		is_busy_err = sys_sif_rpc_send_transaction_data(
			&g_sys_mc_channel_widget_handle,
			2, 1, 0x141C30, 0x414, 0x143140, 4, 0, 0
		);

		// 4. If the SIF bus injection succeeded, sign the active command in RAM
		if (is_busy_err == 0) {
			g_sys_mc_active_command_id = 2;
		}
		else {
			sceSignalSema(g_sys_mc_mutex_sema_id);
		}
	}

	return is_busy_err;
}

/**
 * @brief Sends the Memory Card structural verification and format validation command (command 11).
 * Wraps sceMcGetDir with the entry limit fixed at 64 and masks the active command ID to 0x0B.
 * Original Ghidra address: 0x00127630 (PAL)
 *
 * @param slot_index Card slot to query (0 = slot 1, 1 = slot 2) (param_1).
 * @param context_val Secondary numeric context parameter of the SDK (param_2).
 * @param p_dir_path Text string with the base directory path to verify (param_3).
 * @return s32 Status code (0 if the bus accepted the command, negative values on error).
 */
s32 sceMcCheckMc(u32 slot_index, u32 context_val, const char* p_dir_path) {
	// Redirect the parameters, fixing the limit at 64 entries (0x40)
	s32 status_code = sceMcGetDir(slot_index, context_val, p_dir_path, 0x40);

	// If the bus injection succeeded, mask the ID to verification command 11 (0x0B)
	if (status_code == 0) {
		g_sys_mc_active_command_id = 0x0B;
	}

	return status_code;
}

// Definitions of the extended formatting buffer offsets (already mapped in the suite)
#define MC_FORMAT_SLOT_VAL          (*(u32*)0x00141C30)
#define MC_FORMAT_CONTEXT_VAL       (*(u32*)0x00141C34)
#define MC_FORMAT_MAX_ENTRIES       (*(u32*)0x00141C38)
#define MC_FORMAT_CLUSTERS_VAL      (*(s32*)0x00141C3C)
#define MC_FORMAT_FAT_BUFFER_PTR    (*(u32*)0x00141C40)
#define MC_FORMAT_PATTERN_BUFFER    ((u8*)0x00141C44)

/**
 * @brief Sends the Memory Card format and structural initialization command (command 0x0D) to the hardware bus.
 * Applies 64-byte bitwise alignment to the FAT buffer and injects the order into the IOP as a priority synchronous request.
 * Original Ghidra address: 0x00127E48 (PAL)
 */
s32 sceMcFormat(u32 slot_index, u32 context_val, const char* p_dir_path, u32 max_entries, long clusters_count, u32 fat_buffer_addr) {
	s32 status_code;

	// 1. Check that the Memory Card subsystem is formally up
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protect the bus with a non-blocking poll of the channel semaphore
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	s32 is_busy_err = -200;

	if (sema_status > -1) {
		// 3. Safety check of the string (SCE_MC_ERR_NAME safety filter)
		if (p_dir_path == NULL || p_dir_path == '\0') {
			sceSignalSema(g_sys_mc_mutex_sema_id);
			return -0xD2;
		}

		// Write the extended formatting descriptors into the static data section
		MC_FORMAT_SLOT_VAL = slot_index;
		MC_FORMAT_CONTEXT_VAL = context_val;
		MC_FORMAT_MAX_ENTRIES = max_entries;
		MC_FORMAT_CLUSTERS_VAL = (s32)clusters_count;
		MC_FORMAT_FAT_BUFFER_PTR = fat_buffer_addr;

		// Perform the safe copy into the shared buffer with the vector utility
		sys_strncpy_safe(0x00141C44, p_dir_path, 0x3FF);

		// Clear the contiguous metadata of the physical Sony control structure
		*(u8*)0x00142043 = 0;

		// MULTIPLIED COHERENCE BARRIER (capacity << 6 equals multiplying by 64 bytes)
		if (clusters_count > -1) {
			long calculated_bytes_len = (long)((s32)clusters_count << 6);
			sys_kernel_flush_dcache_range(fat_buffer_addr, calculated_bytes_len);
		}

		// Dispatch the order with the command 0x0D burst (priority synchronous = 1, size 0x414)
		is_busy_err = sys_sif_rpc_send_transaction_data(
			&g_sys_mc_channel_widget_handle,
			0x0D, 1, 0x141C30, 0x414, 0x143140, 4, 0, 0
		);

		// 4. If the SIF bus injection succeeded, sign the active command in RAM
		if (is_busy_err == 0) {
			g_sys_mc_active_command_id = 0x0D;
		}
		else {
			sceSignalSema(g_sys_mc_mutex_sema_id);
		}
	}

	return is_busy_err;
}
