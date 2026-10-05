// src/kernel_sys.h
#ifndef KERNEL_SYS_H
#define KERNEL_SYS_H

#include "types.h"
#include <stdarg.h>  // va_list
#include <stdbool.h> // Defines 'bool', 'true' and 'false'
#include <stdint.h>  // Required for the standard fixed-width integers

#ifdef __cplusplus
extern "C" {
#endif

	/* ------------------------------------------------------------------------
	 * Real PS2 hardware registers and syscalls (only under
	 * PLATFORM_PS2). "Status" is the MIPS Coprocessor 0 Status register.
	 * NOTE: "FlushCache" here only had a void(void) signature, but the real PS2SDK
	 * defines it as void FlushCache(int mode) -- corrected below and in the
	 * single call in the .c (previously "FlushCache();", now "FlushCache(0);").
	 * Confirm it if the original documentation is at hand.
	 * ------------------------------------------------------------------------ */
	extern u32 Status;
	void DI(void);
	void EI(void);
	void SYNC(int type);
	void RFU086_WaitEvnetFlag(void);
	long GetMemorySize(void);
	void _InitTLB(void);
	void FlushCache(int mode);

	// If the short aliases u32, s32, u8 are not defined globally in another included .h,
	// add them here safely to harden the header:
	typedef uint32_t u32;
	typedef int32_t  s32;
	typedef uint8_t  u8;

	// ... From here on, function signatures such as:
	// bool kernel_system_sync_guard(void);
	// void kernel_system_sync_release(void);
	// ... are fully valid for the Windows compiler ...

	/* FIXED BUG: kernel_tlb_cache_sync() uses these 4 variables but they
	 * were not declared anywhere in the original .c (not even "extern") -
	 * the file did not compile. The s32 type is a reasonable assumption from
	 * their use (added, compared with 0x30, shifted) but it could not be
	 * verified against their real definition, which must live in another
	 * kernel module (TLB management) not examined yet. */
	extern s32 g_tlb_wired_index;
	extern s32 g_tlb_bound_index;
	extern s32 g_tlb_status_sync;
	extern s32 g_tlb_extra_flags;

	/* ------------------------------------------------------------------------
	 * Public API of this module (assertions, engine printf/vsnprintf,
	 * string utilities, scientific formatting, TLB memory and DECI2 logs).
	 * ------------------------------------------------------------------------ */
	void sys_assert_dispatch(const char* p_file, s32 line, const char* p_assertion,
		long p4, long p5, long p6, long p7, long p8);

	s32  txt_vsnprintf_internal(char* p_dest_buffer, const char* p_format_str, va_list args_list);
	s32  txt_sprintf_channel_dispatcher(s32* p_buffer_struct, const char* p_format_str, va_list args_list);
	s32  custom_vsprintf_engine(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list);
	s32  custom_vsprintf_engine_alt(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list);
	s32  txt_sprintf_wrapper(s32* p_buffer_struct, const char* p_format_str, va_list args_list);
	s32  game_sprintf(s32* p_buffer_struct, const char* p_format_str, ...);

	const char* math_dtoa_format(double value, s32 precision, char format_char, s32 flags);

	char* ee_strrev(char* p_str);
	char* ee_itoa(s32 value, char* p_dest_buffer, s64 base);
	s32   txt_round_ascii_digits(char* p_str_buffer, s64 precision_index);
	int   ee_strlen(const char* str);
	int   ee_strcmp(const char* str1, const char* str2);
	void* ee_memcpy(void* dest, const void* src, u32 size);
	void* ee_memchr(const void* ptr, int value, u32 num);
	u64   ee_atoll_wrapper(const char* p_srcString, char** p_end_ptr, s32 base);
	s64   ee_strtoll(s32* p_error_out, const char* p_srcString, char** p_end_ptr, s32 base);

	const void** ee_get_ctype_table_ptr(void);
	const void** ee_ctype_interface_wrapper(void);

	s32  txt_decode_multibyte_char(u8* p_localeContext, u32* p_outChar, const u8* p_srcString, u32 max_bytes, s32* p_state);

	void sys_safe_exit_stub(void);
	void sys_kernel_panic_abort(s32 exit_code);
	void kernel_hardware_memory_init(void);
	long kernel_tlb_cache_sync(void);

	u32  sys_log_write_buffered_alt(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag);
	u32  sys_log_write_buffered(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag); // MISMATCH: previously declared as "s32" (the real definition returns u32)
	s32  sys_log_dispatch_message(u32 log_level, const char* p_message, s32 message_len);

	void* sys_queue_initialize(u32 param_1);

	bool sys_deci2_subsystem_init(void);
	s32  sys_deci2_print_log(const char* p_message, s32 max_len);
	void sys_deci2_call_wrapper(void);
	s32  sys_deci2_call_channel_a(void); // FIXED BUG: it was defined as "void" but its return value was used (see the note in the .c)
	void sys_deci2_call_channel_c(void);

#ifdef __cplusplus
}
#endif

#endif // KERNEL_SYS_H
