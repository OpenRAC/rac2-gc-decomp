// src/ps2_sif.h
#ifndef PS2_SIF_H
#define PS2_SIF_H
#include <stdbool.h> // Defines 'bool' for the compiler
#include <stdint.h>  // Required for the fixed-width integers

#include "types.h"
#include "ps2_kernel.h" // sceSignalSema, sceDeleteSema, scePollSema, sceGetThreadId, sceFlushCache

#ifdef __cplusplus
extern "C" {
#endif

	// Type aliases so the compiler understands the SIF bus signatures
	typedef uint32_t u32;
	typedef int32_t  s32;
	typedef uint64_t u64;
	typedef int64_t  s64;
	typedef uint8_t  u8;

	/* ------------------------------------------------------------------------
	 * Official Sony SDK functions (only under PLATFORM_PS2, without
	 * PC emulation in this file).
	 * ------------------------------------------------------------------------ */
	u64 sceSifSetDma(void);
	u64 isceSifSetDma(void);
	s32 sceAddDmacHandler(s32 channel, void* handler, s32 arg);

	/* ------------------------------------------------------------------------
	 * External functions of other project units (kernel, SIF bus RPC,
	 * sound, boot text). Their original headers are not available,
	 * so they are declared here. "kernel_system_sync_guard" and
	 * "kernel_system_sync_release" live in kernel_sys.c (confirmed); their
	 * real signature is "bool" for both. "sys_kernel_enable_dmac" is still without
	 * a known location.
	 * ------------------------------------------------------------------------ */
	bool kernel_system_sync_guard(void);
	bool kernel_system_sync_release(void);
	u64  sys_kernel_enable_dmac(void);
	void sys_kernel_flush_dcache_range(u32 start_addr, long block_size);

	s32  sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4,
		long p5, long p6, long p7, long p8);

	bool sys_sif_rpc_init_client(void);
	s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
	s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags,
		long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);

	bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3,
		long param_4, long param_5, long param_6, long param_7, long param_8);

	void sys_io_init_kernel_semaphores(void);

	/* UNRESOLVED CONFLICT: the original .c declared this function in 3 ways
	 * different and incompatible ways:
	 *   void sys_strncpy_safe(void* dest,   const void* src, size_t max_len);
	 *   u32  sys_strncpy_safe(u32  dest_addr, const char* src_addr, u32 max_len);  <- this one was used (it appeared 2 out of 3 times)
	 * Its real definition is not in ps2_sif.c, so it could not be verified which
	 * one is correct. Confirm it against the file where it actually lives.
	 */
	u32 sys_strncpy_safe(u32 dest_addr, const char* src_addr, u32 max_len);

	/* ------------------------------------------------------------------------
	 * Public API of this module (SIF bus, IO channel, libcdvd and Memory Card).
	 * ------------------------------------------------------------------------ */
	void sys_io_iop_interrupt_handler(void);

	u64  sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header,
		long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);
	void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size,
		u32 src_addr, u32 dest_addr, long transfer_len);
	void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size,
		u32 src_addr, u32 dest_addr, long transfer_len);

	bool sys_sif_init_manager(void);
	void sys_sif_register_callback(long command_id, void* callback_ptr, void* callback_arg);
	void sys_sif_unregister_callback(long command_id);

	s32  sys_io_init_subsystem(void);
	bool sys_io_shutdown_subsystem(void);

	u32  sys_cdvd_init_filesystem(s32 init_mode);
	u32  sys_cdvd_check_disk_ready(long check_mode);

	s32  sceMcRead(u32 file_descriptor, u32 read_size, long p_dest_buffer, long block_len, long offset_pos);
	s32  sceMcWrite(u32 file_descriptor, u32 src_ram_addr, long write_size);
	s32  sceMcWriteExtended(u32 file_descriptor, const u8* p_src_ram_buffer, long raw_write_size);
	s32  sceMcChdir(u32 slot_index, const char* p_dir_path);
	s32  sceMcMkdir(u32 slot_index, const char* p_dir_path);
	s32  sceMcDelete(u32 slot_index, u32 context_val, const char* p_filename_path);
	s32  sceMcGetDir(u32 slot_index, u32 context_val, const char* p_search_pattern, u32 max_entries);
	s32  sceMcCheckMc(u32 slot_index, u32 context_val, const char* p_dir_path);
	s32  sceMcFormat(u32 slot_index, u32 context_val, const char* p_dir_path, u32 max_entries,
		long clusters_count, u32 fat_buffer_addr);

	/* ------------------------------------------------------------------------
	 * Global variables of the subsystem (defined in another unit of the
	 * project; they used to be re-declared as "extern" in almost every function
	 * of this file).
	 * ------------------------------------------------------------------------ */
	extern s32 g_sys_io_reconfig_flag;
	extern s32 g_sys_io_is_ready_flag;
	extern s32 g_sys_io_lock_sema_id;
	extern s32 g_sys_io_wait_sema_id;
	extern s32 g_sys_io_dma_sema_id;

	extern s32 g_sys_mc_is_bound_flag;
	extern s32 g_sys_mc_mutex_sema_id;
	extern s32 g_sys_mc_active_command_id;
	extern u32 g_sys_mc_channel_widget_handle;
	extern u32 g_sys_mc_read_fd;
	extern u32 g_sys_mc_read_size;  // Shared with the DAT_00141c08 buffer
	extern u32 g_sys_mc_write_fd;
	extern u32 g_sys_mc_write_src_ptr;
	extern u32 g_sys_mc_write_size;

#ifdef __cplusplus
}
#endif

#endif // PS2_SIF_H
