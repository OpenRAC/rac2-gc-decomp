// src/math_util.h
#ifndef MATH_UTIL_H
#define MATH_UTIL_H

#include "types.h"
#include <stdarg.h>  // va_list
#include <stddef.h>  // size_t
#include <stdbool.h> // Defines 'bool'
#include <stdint.h>  // Required for the fixed-width integer types

#ifdef __cplusplus
extern "C" {
#endif

	/* ------------------------------------------------------------------------
	 * External functions of other project units. Their original headers
	 * are not available, so they are declared here.
	 * ------------------------------------------------------------------------ */

	 // Type aliases so the compiler understands the math signatures
	typedef uint32_t u32;
	typedef int32_t  s32;
	typedef uint64_t u64;
	typedef int64_t  s64;
	typedef uint8_t  u8;

	void  sys_safe_exit_stub(void);
	s32   game_sprintf(s32* p_buffer_struct, const char* p_format_str, ...);
	u64   ee_atoll_wrapper(const char* p_srcString, char** p_end_ptr, s32 base); // Not used in math_util.c; kept in case another unit needs it
	void* ee_memcpy(void* dest, const void* src, u32 num); // Used in math_double_to_digits() without a declaration in the original file; type corrected to u32 to agree with kernel_sys.c (its real definition)

	/* CROSS-FILE CONFLICT: in ps2_sif.c/ps2_kernel.h this function is used
	 * as "void kernel_system_sync_release(void)", but here in math_util.c
	 * the original declared it as "bool". Both files ignore the return
	 * value when calling it, so leaving it as "void" here breaks nothing
	 * (to avoid clashing with ps2_sif.h) — but confirm the real signature. */
	 /* Signatures confirmed against their real definition in kernel_sys.c: both return bool. */
	bool kernel_system_sync_guard(void);
	bool kernel_system_sync_release(void);

	/* ------------------------------------------------------------------------
	 * Public API of this module (conversion, IEEE 754 packing, 64-bit
	 * arithmetic and scientific-text utilities for the HUD).
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

	void* math_add_sub_double64(u32* p_unpack1, u32* p_unpack2, u32* p_out_unpack); // See the bug note further below
	s32    math_compare_double64_wrapper(u64 param_1, u64 param_2);
	s32    math_compare_double64(u32* p_unpack1, u32* p_unpack2);
	void   math_unpack_double64(u64* p_double_bits, u32* p_output_struct);

	double math_float_to_double(f32 param_1); // Real signature: returns double, NOT u64 (the original .c had a conflicting prototype)

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
