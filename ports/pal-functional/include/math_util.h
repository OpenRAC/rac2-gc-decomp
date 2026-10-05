// src/math_util.h
#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "types.h"
#include <stdarg.h>  // va_list
#include <stddef.h>  // size_t
#include <stdbool.h> // ¡ESTO ENSEÑA QUÉ ES 'bool'!
#include <stdint.h>  // ¡REQUERIDO PARA TIPOS DE ENTEROS FIJOS!

#ifdef __cplusplus
extern "C" {
#endif

	/* ------------------------------------------------------------------------
	 * Funciones externas de otras unidades del proyecto. No se cuenta con sus
	 * cabeceras originales, así que se declaran aquí.
	 * ------------------------------------------------------------------------ */

	 // Definimos los alias de tipo para que el compilador entienda tus firmas matemáticas
	typedef uint32_t u32;
	typedef int32_t  s32;
	typedef uint64_t u64;
	typedef int64_t  s64;
	typedef uint8_t  u8;

	void  sys_safe_exit_stub(void);
	s32   game_sprintf(s32* p_buffer_struct, const char* p_format_str, ...);
	u64   ee_atoll_wrapper(const char* p_srcString, char** p_end_ptr, s32 base); // No se usa en math_util.c; se conserva por si otra unidad la necesita
	void* ee_memcpy(void* dest, const void* src, u32 num); // Usada en math_double_to_digits() sin declarar en el archivo original; tipo corregido a u32 para coincidir con kernel_sys.c (su definición real)

	/* CONFLICTO ENTRE ARCHIVOS: en ps2_sif.c/ps2_kernel.h esta función se usa
	 * como "void kernel_system_sync_release(void)", pero aquí en math_util.c
	 * el original la declaraba como "bool". Ambos archivos ignoran el valor
	 * de retorno al llamarla, así que no rompe nada dejarla en "void" aquí
	 * (para no chocar con ps2_sif.h) — pero confirma cuál es la firma real. */
	 /* Firmas confirmadas contra su definición real en kernel_sys.c: ambas devuelven bool. */
	bool kernel_system_sync_guard(void);
	bool kernel_system_sync_release(void);

	/* ------------------------------------------------------------------------
	 * API pública de este módulo (conversión, empaquetado IEEE 754, aritmética
	 * de 64 bits y utilidades de texto científico para el HUD).
	 * ------------------------------------------------------------------------ */
	s32    math_double_to_int32_signed(double param_1);
	double math_fmod_double64(double x, double y);
	double math_floor_double64(double x);
	double math_log10_double64(double x);
	double math_log_double64(double x);

	void sys_assert_fail(const char* p_assertion, const char* p_file, s32 line);

	bool txt_render_scientific_string(const u8* format_ptr, va_list args_list);
	bool txt_format_scientific_wrapper(const u8* p_format_label, ...);
	void math_double_to_scientific_digits(double param_1);

	s32    math_double_to_digits(double param_1);
	s64    math_double_to_int64(double param_1);
	double math_int64_to_double(s64 param_1);
	double math_add_double64(double a, double b);
	double math_int_to_double(s32 param_1);
	s32    math_double_to_int(double param_1);
	double math_mul_double64(double a, double b);
	s64    math_mul64(s64 a, s64 b);
	double math_sub_double64(double minuend, double subtrahend);
	double math_div_double64(double dividend, double divisor);

	void* math_add_sub_double64(u32* p_unpack1, u32* p_unpack2, u32* p_out_unpack); // Ver nota de bug más abajo
	s32    math_compare_double64_wrapper(u64 param_1, u64 param_2);
	s32    math_compare_double64(u32* p_unpack1, u32* p_unpack2);
	void   math_unpack_double64(u64* p_double_bits, u32* p_output_struct);

	double math_float_to_double(f32 param_1); // Firma real: devuelve double, NO u64 (el .c original tenía un prototipo en conflicto)

	void   math_pack_double64_wrapper(u32 param_1, u32 param_2, u32 param_3, u64 param_4);
	u64    math_pack_double64(u32* p_input_struct);
	s64    math_mod64(s64 dividend, s64 divisor);
	void   math_unpack_float32(u32* p_float_bits, u32* p_output_struct);
	s64    math_div64(s64 dividend, s64 divisor);
	u64    math_udiv64(u64 dividend, u64 divisor);

#ifdef __cplusplus
}
#endif

#endif // MATH_UTIL_H
