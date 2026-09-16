// src/kernel_sys.h
#ifndef KERNEL_SYS_H
#define KERNEL_SYS_H

#include "types.h"
#include <stdarg.h>  // va_list
#include <stdbool.h> // ¡ESTA LÍNEA ENSEÑA QUÉ ES 'bool', 'true' y 'false'!
#include <stdint.h>  // ¡ESTA LÍNEA DEBE ESTAR PARA PROCESAR ENTEROS ESTÁNDAR!

#ifdef __cplusplus
extern "C" {
#endif

	/* ------------------------------------------------------------------------
	 * Registros y syscalls de hardware reales de la PS2 (solo bajo
	 * PLATFORM_PS2). "Status" es el registro Status del Coprocesador 0 MIPS.
	 * OJO: "FlushCache" aquí solo tenía firma void(void), pero el PS2SDK real
	 * la define como void FlushCache(int mode) -- se corrigió abajo y en la
	 * única llamada del .c (antes "FlushCache();", ahora "FlushCache(0);").
	 * Confírmalo si tienes la documentación original a mano.
	 * ------------------------------------------------------------------------ */
	extern u32 Status;
	void DI(void);
	void EI(void);
	void SYNC(int type);
	void RFU086_WaitEvnetFlag(void);
	long GetMemorySize(void);
	void _InitTLB(void);
	void FlushCache(int mode);

	// Si no tienes definidos globalmente tus alias abreviados u32, s32, u8 en otro .h incluido, 
	// agrégalos aquí de forma segura para blindar la cabecera:
	typedef uint32_t u32;
	typedef int32_t  s32;
	typedef uint8_t  u8;

	// ... A partir de aquí tus firmas de funciones como:
	// bool kernel_system_sync_guard(void);
	// void kernel_system_sync_release(void);
	// ... ya serán 100% válidas y legales para el compilador de Windows ...

	/* BUG CORREGIDO: estas 4 variables las usa kernel_tlb_cache_sync() pero no
	 * estaban declaradas en ningún lado del .c original (ni siquiera "extern") -
	 * el archivo no compilaba. El tipo s32 es una suposición razonable según su
	 * uso (se suman, se comparan con 0x30, se desplazan) pero no se pudo
	 * verificar contra su definición real, que debe vivir en otro módulo del
	 * kernel (gestión de TLB) que no hemos visto todavía. */
	extern s32 g_tlb_wired_index;
	extern s32 g_tlb_bound_index;
	extern s32 g_tlb_status_sync;
	extern s32 g_tlb_extra_flags;

	/* ------------------------------------------------------------------------
	 * API pública de este módulo (aserciones, printf/vsnprintf del motor,
	 * utilidades de string, formateo científico, memoria TLB y logs DECI2).
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
	u32  sys_log_write_buffered(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag); // MISMATCH: antes declarada como "s32" (la definición real devuelve u32)
	s32  sys_log_dispatch_message(u32 log_level, const char* p_message, s32 message_len);

	void* sys_queue_initialize(u32 param_1);

	bool sys_deci2_subsystem_init(void);
	s32  sys_deci2_print_log(const char* p_message, s32 max_len);
	void sys_deci2_call_wrapper(void);
	s32  sys_deci2_call_channel_a(void); // BUG CORREGIDO: estaba definida como "void" pero se usaba su valor de retorno (ver nota en el .c)
	void sys_deci2_call_channel_c(void);

#ifdef __cplusplus
}
#endif

#endif // KERNEL_SYS_H
