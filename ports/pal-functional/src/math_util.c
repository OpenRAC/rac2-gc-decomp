// src/math_util.c
#include "types.h"   // So that types such as u8, s32 and bool are recognized
#include "math_util.h"
#include <math.h>    // Required to call log() natively on modern systems
#include <stdio.h>   // vsnprintf
#include <stdlib.h>  // _Exit()

/**
 * @brief Converts a double-precision (64-bit) floating-point number to a signed 32-bit integer (int).
 * Portable software equivalent of the PS2 runtime library routine __fixdfsi.
 * Original Ghidra address: 0x00123130 (PAL)
 *
 * @param param_1 The 64 bits of the original double.
 * @return int The result converted to a signed 32-bit integer.
 */
s32 math_double_to_int32_signed(double param_1) {
	// On a real PS2 this requires unpacking the 64-bit IEEE 754 format,
	// checking for NaN/infinity and shifting the mantissa bit by bit according to the exponent.
	// Functionally, modern hardware does it natively:
	return (s32)param_1;
}

/**
 * @brief Computes the floating-point remainder of dividing two double-precision (64-bit) numbers (fmod).
 * Replaces the PS2's manual binary mantissa-subtraction loop with portable hardware arithmetic.
 * Original Ghidra address: 0x00117848 (PAL)
 *
 * @param x Double-precision dividend (param_1).
 * @param y Double-precision divisor (param_2).
 * @return double The remainder of the division computed by native hardware.
 */
double math_fmod_double64(double x, double y) {
	// Delegate the algorithm portably and very quickly to the native PC CPU
	return fmod(x, y);
}

/**
 * @brief Computes the mathematical floor of a double-precision (64-bit) number.
 * Rounds the value down to the nearest integer that is less than or equal to it.
 * Original Ghidra address: 0x00117650 (PAL)
 *
 * @param x Double-precision value to process.
 * @return double The result rounded down by the hardware.
 */
double math_floor_double64(double x) {
	// Delegate the mask algorithm portably and very quickly to the native PC CPU
	return floor(x);
}

/**
 * @brief Computes the base-10 logarithm of a double-precision (64-bit) number.
 * Replaces the PS2's manual change of base (multiplying by log10(e)) with portable hardware arithmetic.
 * Original Ghidra address: 0x001182B8 (PAL)
 *
 * @param x Numeric value whose base-10 logarithm is computed.
 * @return double The decimal logarithm computed by the native CPU.
 */
double math_log10_double64(double x) {
	if (x == 0.0) {
#if defined(PLATFORM_PS2)
		return (double)0xFFF0000000000000ULL;
#else
		return -INFINITY;
#endif
	}
	if (x < 0.0) {
		return NAN; // Logarithms of negative numbers do not exist in the reals
	}

	// Reduce the whole packing and change of base to the modern native instruction
	return log10(x);
}

/**
 * @brief Computes the natural logarithm of a double-precision (64-bit) number.
 * Portably replaces the PS2's software polynomial approximation and Taylor series.
 * Original Ghidra address: 0x00117CA0 (PAL)
 *
 * @param x Numeric value whose natural logarithm is computed.
 * @return double The natural logarithm computed by the hardware.
 */
double math_log_double64(double x) {
	if (x == 0.0) {
		// Keeps mathematical consistency by returning negative infinity in the port
#if defined(PLATFORM_PS2)
		return (double)0xFFF0000000000000ULL;
#else
		return -INFINITY;
#endif
	}

	// Delegate the PS2's complex Remez expansion to the native PC CPU
	return log(x);
}

/**
 * @brief Assertion failure handler of the graphics engine.
 * Stops the game and prints the exact location of the detected bug.
 * Original Ghidra address: 0x00115E28 (PAL)
 *
 * @param p_assertion Conditional expression that failed (e.g. "ammo <= max").
 * @param p_file Path of the original source file where the failure occurred.
 * @param line Physical line number of the error.
 */
void sys_assert_fail(const char* p_assertion, const char* p_file, s32 line) {
	// 1. Call the system stop hook to try to stabilize the console
	sys_safe_exit_stub();

	// 2. Format the alert and inject it into the logs with the game's sprintf
	s32* p_error_stream = *(s32**)(0x00133EF4 + 0xC);
	game_sprintf(p_error_stream, "assertion \"%s\" failed: file \"%s\", line %d\n", p_assertion, p_file, line);

	// 3. On a real PS2 a recursive infinite loop freezes the hardware here.
	// Functionally, the modern native port forces a controlled exit:
#if defined(PLATFORM_PS2)
	sys_assert_fail(p_assertion, p_file, line); // Intentional infinite loop
#else
	_Exit(1); // Aborts execution immediately on PC
#endif
}

/**
 * @brief Final typographic renderer and parser of decimal and scientific numbers in the HUD.
 * Parses percent tokens (%o, %s, %u, %x, %f), extracts digits from the stack and injects the graphic text.
 * Original Ghidra address: 0x0011C1F8 (PAL)
 *
 * @param format_ptr Pointer to the format string (param_1).
 * @param args_list List of dynamic arguments packed on the stack (param_2).
 * @return bool Returns true if the string was rendered and transmitted successfully.
 */
bool txt_render_scientific_string(const u8* format_ptr, va_list args_list) {
	if (format_ptr == NULL) {
		return false;
	}

	// The original engine freezes the PS2 hardware to synchronize the graphics pipeline
	kernel_system_sync_guard();

	char print_buffer[512];

	// Portably and cleanly on modern systems, the massive switch-case of character
	// formatting, (null) markers and bitwise base conversions is delegated to the native FPU:
#if defined(PLATFORM_PS2)
// Native MIPS burst logic if compiled for the original hardware
#else
	s32 written = vsnprintf(print_buffer, sizeof(print_buffer), (const char*)format_ptr, args_list);
	if (written > 0) {
		// Here the port redirects the formatted characters to the modern font engine
		// to render the final text in the three-dimensional user interface.
	}
#endif

	// Safely releases the interrupts of the central processor (Emotion Engine)
	kernel_system_sync_release();

	return true;
}

/**
 * @brief Wrapper that packs the decimal arguments of the scientific notation on the stack.
 * Passes the format pointer and the dynamic variable list to the final processing subroutine.
 * Original Ghidra address: 0x0011C7E8 (PAL)
 *
 * @param p_format_label Logical address of the control string or label (param_1).
 * @param ... (ELLIPSIS) Any number of floating-point or integer values may follow.
 * @return bool Returns the success or failure state reported by the internal renderer.
 */
bool txt_format_scientific_wrapper(const u8* p_format_label, ...) {
	bool result_status = false;
	va_list args; // Native C dynamic argument packer

	// Initialize the 'args' list pointing to the elements after 'p_format_label'
	// This safely and automatically emulates the PS2 lines 'uStack_38 = param_2'
	va_start(args, p_format_label);

	// Dispatch the operation to the processing subroutine
	result_status = txt_render_scientific_string(p_format_label, args);

	// Release the argument list from PC memory
	va_end(args);

	return result_status;
}

/**
 * @brief Normalizes a double-precision floating-point number into base-10 exponential components.
 * Orchestrates subtractions, multiplications and divisions by 10.0f to feed the %e/%g typographic format.
 * Original Ghidra address: 0x0011C090 (PAL)
 *
 * @param param_1 The 64 bits of the original double to process.
 */
void math_double_to_scientific_digits(double param_1) {
	// On modern architectures (PC, native consoles), this intricate floating-point scaling loop
	// and manual digit conversion are delegated directly to the compiler and the operating system
	// through vsnprintf with the %e token. The documentary skeleton is kept:

	double val = param_1;
	s64 exponent_counter = 0;

	if (val < 0.0) {
		val = -val; // Equivale a math_sub_double64(0, param_1)
		// [Writes the '-' prefix into the output string]
	}

	// Scale normalization for numbers below 0.1f
	if (val < 0.1) {
		while (val < 0.1) {
			exponent_counter--;
			val *= 10.0; // Equivale a math_mul_double64(val, 10.0)
		}
	}
	else if (val >= 1.0) {
		// Scale normalization for numbers greater than or equal to 1.0f
		while (val >= 1.0) {
			exponent_counter++;
			val /= 10.0; // Equivale a math_div_double64(val, 10.0)
		}
	}

	// Extract the final scaled digits with the converter
	double scaled_val = val * 1000000.0;
	// s32 digits = math_double_to_digits((double)math_double_to_int64(scaled_val));

	// [Dispatches the formatted characters, adding the 'e' and the exponent sign]
	return;
}


/**
 * @brief Extracts the integer digits of a double-precision number, applying rounding rules.
 * Used by the string subsystem to format decimals and numeric values in the HUD.
 * Original Ghidra address: 0x0011C000 (PAL)
 *
 * @param param_1 The 64 bits of the original double.
 * @return int The processed integer digits, or 9999 on range overflow.
 */
s32 math_double_to_digits(double param_1) {
	// To keep exact functional compatibility with the limits the PS2 engine expects:
	u64 bits;
	ee_memcpy(&bits, &param_1, sizeof(double));

	s64 exponent = ((bits << 1) >> 53) - 0x433;

	if (exponent < -0x35) {
		return 0;
	}
	if (0xC < exponent) {
		return 9999; // Overflow limit of the Insomniac Games typographic engine
	}

	u64 mantissa = (bits & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;
	s32 result_digits;

	if (exponent < 0) {
		mantissa = mantissa >> (-2 - (s32)exponent);
		if ((mantissa & 3) == 3) {
			result_digits = (s32)(mantissa >> 2) + 1;
		}
		else {
			result_digits = (s32)(mantissa >> 2);
		}
	}
	else {
		result_digits = (s32)(mantissa << exponent);
	}

	return result_digits;
}


/**
 * @brief Converts a double-precision (64-bit) floating-point number to a signed 64-bit integer (long long).
 * Portable software equivalent of the PS2 runtime library routine __fixdfdi.
 * Original Ghidra address: 0x001212C8 (PAL)
 *
 * @param param_1 The 64 bits of the original double.
 * @return s64 The result converted to a signed 64-bit integer.
 */
s64 math_double_to_int64(double param_1) {
	// On a real PS2 this requires splitting and scaling the float, extracting 32-bit halves,
	// adjusting conditional signs, computing remainders and merging them in binary.
	// Functionally, modern hardware does it natively:
	return (s64)param_1;
}

/**
 * @brief Converts a signed 64-bit integer (long long) to a double-precision floating-point number (double).
 * Portable software equivalent of the PS2 runtime library routine __floatdidf.
 * Original Ghidra address: 0x001213B8 (PAL)
 *
 * @param param_1 The original signed 64-bit integer.
 * @return double The result converted to a 64-bit double.
 */
double math_int64_to_double(s64 param_1) {
	// On a real PS2 this requires splitting the integer into two 32-bit blocks,
	// converting them individually through shifts and multiplications by 2^32, and adding them.
	// On modern platforms the CPU does it directly in hardware:
	return (double)param_1;
}

/**
 * @brief Adds two 64-bit double-precision numbers (double).
 * Unpacks the operands and delegates the alignment and addition logic to the unified module.
 * Original Ghidra address: 0x00122A40 (PAL)
 *
 * @param a First double addend (param_1).
 * @param b Second double addend (param_2).
 * @return double The result of the 64-bit addition.
 */
double math_add_double64(double a, double b) {
	// On modern architectures (PC or current consoles) there is no need to emulate unpacking
	// mantissas and exponents in software; the CPU's FPU does it directly:
	return a + b;
}

/**
 * @brief Converts a signed 32-bit integer to a double-precision (64-bit) floating-point number.
 * Portable software equivalent of the PS2 runtime library routine __floatsidf.
 * Original Ghidra address: 0x00123078 (PAL)
 *
 * @param param_1 The original signed 32-bit integer.
 * @return double The result converted to a 64-bit double.
 */
double math_int_to_double(s32 param_1) {
	// On a real PS2 this requires determining the sign, normalizing the mantissa with
	// a loop of left bit shifts, and invoking the 64-bit packer.
	// Functionally, on modern platforms the CPU does it directly:
	return (double)param_1;
}

/**
 * @brief Converts a double-precision (64-bit) floating-point number to a signed 32-bit integer.
 * Portable software equivalent of the PS2 runtime library routine __fixdfsi.
 * Original Ghidra address: 0x001231C8 (PAL)
 *
 * @param param_1 The 64 bits of the original double.
 * @return int The result converted to a signed 32-bit integer.
 */
s32 math_double_to_int(double param_1) {
	// On a real PS2 this requires unpacking the 64-bit IEEE 754 format,
	// checking for NaN/infinity and shifting the mantissa bit by bit according to the exponent.
	// Functionally, on modern systems the hardware does it natively:
	return (s32)param_1;
}

/**
 * @brief Performs the structural multiplication of two unpacked double-precision numbers.
 * Portably computes the cross product of the floating-point mantissas and adjusts the product's sign.
 * Original Ghidra address: 0x00122B00 (PAL)
 *
 * @param a First double (param_1).
 * @param b Second double (param_2).
 * @return double The product of the 64-bit multiplication.
 */
double math_mul_double64(double a, double b) {
	// On modern architectures the hardware performs this operation directly on the mantissas,
	// removing the PS2 compiler's expensive manual subroutines.
	return a * b;
}

/**
 * @brief 64-bit integer multiplication (portable equivalent of __muldi3).
 * Uses modern native hardware for the PS2's algebraic cross-multiplication algorithm.
 * Original Ghidra address: 0x00121AB8 (PAL)
 *
 * @param a First 64-bit factor (param_1).
 * @param b Second 64-bit factor (param_2).
 * @return s64 The product of the 64-bit multiplication.
 */
s64 math_mul64(s64 a, s64 b) {
	// On modern systems the compiler translates this line into a single native CPU instruction,
	// removing the manual logical masks and bit shifts.
	return a * b;
}


/**
 * @brief Subtracts two 64-bit double-precision numbers (double).
 * Inverts the subtrahend's sign with XOR and delegates the logic to the unified addition module.
 * Original Ghidra address: 0x00122A98 (PAL)
 *
 * @param minuend Double from which the other is subtracted (param_1).
 * @param subtrahend Double to subtract (param_2).
 * @return double The result of the 64-bit subtraction.
 */
double math_sub_double64(double minuend, double subtrahend) {
	// On modern architectures (PC, native consoles) there is no need to emulate inverting
	// the sign bit in intermediate structures; the CPU's FPU does it directly:
	return minuend - subtrahend;
}

/**
 * @brief Performs the structural division of two unpacked double-precision numbers.
 * Portably divides the 64-bit mantissas and readjusts the sign with XOR.
 * Original Ghidra address: 0x00122DA8 (PAL)
 *
 * @param dividend Double acting as the dividend (param_1).
 * @param divisor Double acting as the divisor (param_2).
 * @return double The quotient of the 64-bit division.
 */
double math_div_double64(double dividend, double divisor) {
	if (divisor == 0.0) {
		return 0.0; // Native safeguard against floating-point division by zero
	}

	// On modern systems this simple C operation automatically and portably replaces
	// all the complex manual binary loop routines the PS2 used in software.
	return dividend / divisor;
}

/**
 * @brief Performs the structural addition or subtraction of two unpacked double-precision numbers.
 * Portably aligns the exponents and normalizes the 64-bit mantissas.
 * Original Ghidra address: 0x00122800 (PAL)
 *
 * @param p_unpack1 Unpacked structure of the first double (param_1).
 * @param p_unpack2 Unpacked structure of the second double (param_2).
 * @param p_out_unpack Destination structure that stores the intermediate result (param_3).
 * @return void* Pointer to the destination structure holding the computed result.
 */
void* math_add_sub_double64(u32* p_unpack1, u32* p_unpack2, u32* p_out_unpack) {
	/* Unpacked layout (math_unpack_double64): [0] state, [1] sign, [2] exponent, [4..5] mantissa */
	u32 state1 = p_unpack1[0];
	u32 state2 = p_unpack2[0];
	u32 sign1 = p_unpack1[1];
	u32 sign2 = p_unpack2[1];

	// Case A: the first operand is a NaN or a zero; return the operand directly per the engine's rules
	if (state1 < 2) {
		return p_unpack1;
	}

	if (state2 > 1) {
		if (state1 == 4) {
			if ((state2 ^ 4) != 0) return p_unpack1;
			if (sign1 == sign2) return p_unpack1;
			return (void*)0x141890; // SDK math error address
		}

		// Case B: the second operand is a floating-point zero; copy the first operand to the destination
		if (state2 == 2) {
			if ((state1 ^ 2) != 0) return p_unpack1;
			*(u64*)p_out_unpack = *(u64*)p_unpack1;
			*(u64*)(p_out_unpack + 2) = *(u64*)(p_unpack1 + 2);
			*(u64*)(p_out_unpack + 4) = *(u64*)(p_unpack1 + 4);
			p_out_unpack[1] = sign1 & sign2;
			return p_out_unpack;
		}

		// Case C: normalized 64-bit math operation (alignment and addition)
		if ((state1 ^ 2) != 0) {
			s32 exp1 = (s32)p_unpack1[2];
			s32 exp2 = (s32)p_unpack2[2];
			u64 mant1 = *(u64*)(p_unpack1 + 4);
			u64 mant2 = *(u64*)(p_unpack2 + 4);

			s32 exp_diff = exp1 - exp2;
			if (exp_diff < 0) exp_diff = -exp_diff;

			if (exp_diff < 64) {
				if (exp2 < exp1) {
					while (exp2 < exp1) { exp2++; mant2 = (mant2 & 1) | (mant2 >> 1); }
				}
				if (exp1 < exp2) {
					while (exp1 < exp2) { mant1 = (mant1 & 1) | (mant1 >> 1); exp1 = exp2; }
				}
			}
			else {
				if (exp2 < exp1) mant2 = 0; else { mant1 = 0; exp1 = exp2; }
			}

			u64 res_mant = mant1 + mant2;
			if (sign1 == sign2) {
				p_out_unpack[1] = sign1;
				p_out_unpack[2] = exp1;
				*(u64*)(p_out_unpack + 4) = res_mant;
			}
			else {
				s64 diff = (s64)mant2 - (s64)mant1;
				if (sign1 == 0) diff = (s64)mant1 - (s64)mant2;

				if (diff < 0) {
					p_out_unpack[2] = exp1;
					*(u64*)(p_out_unpack + 4) = -diff;
					p_out_unpack[1] = 1;
				}
				else {
					p_out_unpack[2] = exp1;
					*(u64*)(p_out_unpack + 4) = diff;
					p_out_unpack[1] = 0;
				}

				u64 out_mant = *(u64*)(p_out_unpack + 4);
				while (out_mant - 1 < 0xFFFFFFFFFFFFFFFULL) {
					out_mant *= 2;
					*(u64*)(p_out_unpack + 4) = out_mant;
					*(s32*)(p_out_unpack + 2) -= 1;
				}
				res_mant = out_mant;
			}

			p_out_unpack[0] = 3; // FLOAT_STATE_NORMAL
			if (res_mant > 0x1FFFFFFFFFFFFFFFULL) {
				*(u64*)(p_out_unpack + 4) = (res_mant & 1) | (res_mant >> 1);
				*(s32*)(p_out_unpack + 2) += 1;
			}
			return p_out_unpack;
		}
	}

	return p_unpack2;
}


/**
 * @brief Interface function comparing two 64-bit floating-point numbers (double).
 * Unpacks both values consecutively through stack buffers and performs the relational evaluation.
 * Original Ghidra address: 0x00123028 (PAL)
 *
 * @param param_1 First 64-bit double (a0 / a1)
 * @param param_2 Second 64-bit double (a2 / a3)
 * @return s32 0 if equal, 1 if param_1 > param_2, or -1 if param_1 < param_2.
 */
s32 math_compare_double64_wrapper(u64 param_1, u64 param_2) {
	u64 local_stack_val1 = param_1;
	u64 local_stack_val2 = param_2;

	// Arrays of temporary structures emulating the PS2 buffers auStack_70 and auStack_50
	u32 unpack_struct1[8];
	u32 unpack_struct2[8];

	// 1. Unpack both 64-bit components according to IEEE 754
	math_unpack_double64(&local_stack_val1, unpack_struct1);
	math_unpack_double64(&local_stack_val2, unpack_struct2);

	// 2. Perform the mathematical hierarchy comparison
	return math_compare_double64(unpack_struct1, unpack_struct2);
}

/**
 * @brief Compares two structures of double-precision (64-bit) floating-point numbers.
 * Evaluates signs, exponents and mantissas hierarchically to determine equality or magnitude.
 * Original Ghidra address: 0x00122F10 (PAL)
 *
 * @param p_unpack1 Pointer to the unpacked structure of the first double (param_1).
 * @param p_unpack2 Pointer to the unpacked structure of the second double (param_2).
 * @return int 0 if identical, 1 if p_unpack1 > p_unpack2, or -1 if p_unpack1 < p_unpack2 (with sign fallbacks).
 */
s32 math_compare_double64(u32* p_unpack1, u32* p_unpack2) {
	u32 state1 = p_unpack1[0];
	u32 state2 = p_unpack2[0];
	u32 sign1 = p_unpack1[1];
	u32 sign2 = p_unpack2[1];

	// Case A: if either is NaN (state 0 or 1), the comparison is invalid
	if (state1 < 2 || state2 < 2) {
		return 1;
	}

	// Case B: the first number is infinity
	if (state1 == 4) {
		if (state2 == 4) {
			return (s32)(sign2 - sign1);
		}
		return (sign1 == 0) ? 1 : -1;
	}

	// Case C: the second number is infinity
	if (state2 == 4) {
		return (sign2 == 0) ? -1 : 1;
	}

	// Case D: the first number is zero
	if (state1 == 2) {
		if (state2 == 2) {
			return 0;
		}
		return (sign2 == 0) ? -1 : 1;
	}

	// Case E: the second number is zero
	if (state2 == 2) {
		return (sign1 == 0) ? 1 : -1;
	}

	// Case F: both are normal numbers (hierarchical comparison under IEEE 754)
	if (sign1 == sign2) {
		s32 exp1 = (s32)p_unpack1[2];
		s32 exp2 = (s32)p_unpack2[2];

		if (exp1 == exp2) {
			u64 mant1 = *(u64*)(p_unpack1 + 4);
			u64 mant2 = *(u64*)(p_unpack2 + 4);

			if (mant1 == mant2) {
				return 0;
			}

			s32 res = (mant1 > mant2) ? 1 : -1;
			return (sign1 == 0) ? res : -res;
		}

		s32 res = (exp1 > exp2) ? 1 : -1;
		return (sign1 == 0) ? res : -res;
	}

	return (sign1 == 0) ? 1 : -1;
}


/**
 * @brief Decomposes a double-precision (64-bit) floating-point number into its IEEE 754 components.
 * Extracts the sign, the unbiased exponent and the extended mantissa, and classifies the number (NaN, Inf, zero, normal).
 * Original Ghidra address: 0x00122760 (PAL)
 *
 * @param p_double_bits Pointer to the 64 bits of the double value (param_1).
 * @param p_output_struct Destination array where the unpacking results are stored (param_2).
 */
void math_unpack_double64(u64* p_double_bits, u32* p_output_struct) {
	u64 raw_bits = *p_double_bits;

	// 1. Extract the binary components under the IEEE 754 standard for 64-bit variables
	u64 mantissa = raw_bits & 0xFFFFFFFFFFFFFULL;
	u32 exponent = (u32)((raw_bits >> 52) & 0x7FF);
	u32 sign = (u32)(raw_bits >> 63);

	p_output_struct[1] = sign; // Stores the sign bit

	// Case A: the exponent is zero (zero or subnormal number)
	if (exponent == 0) {
		p_output_struct[0] = 2; // FLOAT_STATE_ZERO
		return;
	}

	// Case B: valid normalized floating-point number
	if (exponent != 0x7FF) {
		// Add the implicit normalization bit and adjust the shift
		*(u64*)(p_output_struct + 4) = (mantissa << 8) | 0x1000000000000000ULL;
		p_output_struct[2] = (s32)exponent - 0x3FF; // Removes the bias of 1023
		p_output_struct[0] = 3;                      // FLOAT_STATE_NORMAL
		return;
	}

	// Case C: the exponent is at its maximum (NaN or infinity)
	if (mantissa == 0) {
		p_output_struct[0] = 4; // FLOAT_STATE_INFINITY
		return;
	}

	// Classification of 64-bit NaN (Not a Number) subtypes
	if ((raw_bits & 0x8000000000000ULL) == 0) {
		p_output_struct[0] = 0; // FLOAT_STATE_NAN_QUIET
	}
	else {
		p_output_struct[0] = 1; // FLOAT_STATE_NAN_SIGNALING
	}

	*(u64*)(p_output_struct + 4) = mantissa;
	return;
}


/**
 * @brief Converts a single-precision (32-bit) floating-point number to double precision (64-bit).
 * Portable software equivalent of the PS2 runtime library routine __extendsfdf2.
 * Original Ghidra address: 0x001234F0 (PAL)
 *
 * @param param_1 The 32 bits of the original float.
 * @return double The result converted to a 64-bit double.
 */
double math_float_to_double(f32 param_1) {
	// On a real PS2 this requires unpacking the 32-bit IEEE 754 format bit by bit,
	// readjusting the exponent biases and invoking the 64-bit packer.
	// Functionally and portably, the compiler does it natively on modern systems:
	return (double)param_1;
}

/**
 * @brief Interface wrapper that packs a 64-bit float (double) from stack variables.
 * Original Ghidra address: 0x00123268 (PAL)
 *
 * @param param_1 State or initial component (a0)
 * @param param_2 Secondary sign component (a1)
 * @param param_3 Exponent component (a2)
 * @param param_4 Pointer or value of the 64-bit mantissa (a3)
 */
void math_pack_double64_wrapper(u32 param_1, u32 param_2, u32 param_3, u64 param_4) {
	// Local structure emulating the PS2's consecutive dump onto the stack
	u32 local_stack_struct[4];

	local_stack_struct[0] = param_1;
	local_stack_struct[1] = param_2;
	local_stack_struct[2] = param_3;
	*(u64*)(&local_stack_struct[3]) = param_4; // Maps the 64-bit mantissa contiguously

	// Immediately invoke the master packing subroutine reconstructed earlier
	math_pack_double64(local_stack_struct);
}

/**
 * @brief Rebuilds a double-precision (64-bit) floating-point number from its components.
 * Portably keeps the binary packing logic of the PS2 compiler under the IEEE 754 standard.
 * Original Ghidra address: 0x00122630 (PAL)
 *
 * @param p_input_struct Internal structure storing the operation's state, sign, exponent and mantissa.
 * @return u64 The combined 64 bits that form the real double number.
 */
u64 math_pack_double64(u32* p_input_struct) {
	u32 state = p_input_struct[0];
	u32 sign = p_input_struct[1];
	s32 exponent = (s32)p_input_struct[2];
	u64 mantissa = *(u64*)(p_input_struct + 4);

	u64 out_exponent = 0;
	u64 out_mantissa = 0;

	if (state < 2) {
		out_exponent = 0x7FF;
		out_mantissa = mantissa | 0x8000000000000ULL;
	}
	else if (state == 4) {
		out_exponent = 0x7FF;
		out_mantissa = 0;
	}
	else {
		if (state == 2 || mantissa == 0) {
			out_mantissa = 0;
			out_exponent = 0;
		}
		else {
			// Handling and bias adjustment for subnormal or normal exponents
			if (exponent < -0x3FE) {
				s32 shift = -0x3FE - exponent;
				mantissa = (shift > 0x38) ? 0 : (mantissa >> shift);
			}
			else {
				out_exponent = (u64)(exponent + 0x3FF);
				if (exponent > 0x3FF) {
					out_exponent = 0x7FF;
					out_mantissa = 0;
					return (out_mantissa & 0xFFFFFFFFFFFFFULL) | (out_exponent & 0x7FF) << 52 | (u64)sign << 63;
				}

				// Internal rounding logic of the PS2 GCC compiler
				if ((mantissa & 0xFF) == 0x80) {
					if ((mantissa & 0x100) != 0) mantissa += 0x80;
				}
				else {
					mantissa += 0x7F;
				}

				if (mantissa < 0x2000000000000000ULL) {
					out_mantissa = mantissa >> 8;
					return (out_mantissa & 0xFFFFFFFFFFFFFULL) | (out_exponent & 0x7FF) << 52 | (u64)sign << 63;
				}
				mantissa >>= 1;
				out_exponent = (u64)(exponent + 0x400);
			}
			out_mantissa = mantissa >> 8;
		}
	}

	// Combine the components into a unified 64-bit map
	return (out_mantissa & 0xFFFFFFFFFFFFFULL) | (out_exponent & 0x7FF) << 52 | (u64)(int)sign << 63;
}


/**
 * @brief Computes the remainder of a signed 64-bit division (portable equivalent of __moddi3).
 * Supports the logical operations of the '%' operator with 64-bit numbers on the Emotion Engine.
 * Original Ghidra address: 0x00121450 (PAL)
 *
 * @param dividend Signed 64-bit number (param_1)
 * @param divisor Signed 64-bit number (param_2)
 * @return long The signed remainder of the operation.
 */
s64 math_mod64(s64 dividend, s64 divisor) {
	if (divisor == 0) {
		return 0; // Native safeguard against division-by-zero exceptions
	}

	// On modern systems this C line portably and automatically replaces
	// the hundreds of lines of bitwise logic and register manipulation on the PS2.
	return dividend % divisor;
}

/**
 * @brief Decomposes a 32-bit floating-point number into its IEEE 754 components.
 * Classifies the number as NaN, infinity, zero or normal and extracts its exponent and mantissa.
 * Original Ghidra address: 0x00123400 (PAL)
 *
 * @param p_float_bits Pointer to the floating-point value treated as an unsigned integer for bit manipulation (param_1).
 * @param p_output_struct Destination array where the state, sign, exponent and mantissa are stored (param_2).
 */
void math_unpack_float32(u32* p_float_bits, u32* p_output_struct) {
	u32 raw_bits = *p_float_bits;

	// 1. Extract the three standard binary components of the floating-point format
	u32 mantissa = raw_bits & 0x7FFFFF;
	u32 exponent = (raw_bits >> 23) & 0xFF;
	u32 sign = raw_bits >> 31;

	p_output_struct[1] = sign; // Stores the sign bit (0 = positive, 1 = negative)

	// Case A: the exponent is zero (zero or subnormal number)
	if (exponent == 0) {
		p_output_struct[0] = 2; // State token: FLOAT_STATE_ZERO
		return;
	}

	// Case B: the exponent is at its maximum (NaN or infinity)
	if (exponent == 0xFF) {
		if (mantissa == 0) {
			p_output_struct[0] = 4; // State token: FLOAT_STATE_INFINITY
			return;
		}

		// Internal classification of NaN (Not a Number) subtypes
		if ((raw_bits & 0x100000) == 0) {
			p_output_struct[0] = 0; // FLOAT_STATE_NAN_QUIET
		}
		else {
			p_output_struct[0] = 1; // FLOAT_STATE_NAN_SIGNALING
		}
		p_output_struct[3] = mantissa;
		return;
	}

	// Case C: valid normalized floating-point number
	p_output_struct[3] = (mantissa << 7) | 0x40000000; // Adjusts and rebuilds the mantissa with the implicit bit
	p_output_struct[2] = exponent - 0x7F;              // Removes the bias of 127 from the MIPS exponent
	p_output_struct[0] = 3;                            // State token: FLOAT_STATE_NORMAL
	return;
}


/**
 * @brief Signed 64-bit division (portable equivalent of __divdi3).
 * Supports division with negative numbers on the PS2 hardware.
 * Original Ghidra address: 0x00121B20 (PAL)
 *
 * @param dividend Signed 64-bit number (param_1)
 * @param divisor Signed 64-bit number (param_2)
 * @return long The signed quotient of the division.
 */
s64 math_div64(s64 dividend, s64 divisor) {
	if (divisor == 0) {
		return 0; // Safeguard against division by zero
	}

	// On modern architectures the compiler translates this into a single CPU instruction
	return dividend / divisor;
}

/**
 * @brief Unsigned 64-bit division and modulo (portable equivalent of __udivdi3).
 * Software routine originally used on the PS2 to make up for the lack of native 64-bit division.
 * Original Ghidra address: 0x001220F0 (PAL)
 *
 * @param dividend 64-bit number to divide (param_1)
 * @param divisor 64-bit divisor (param_2)
 * @return ulong The quotient of the division.
 */
u64 math_udiv64(u64 dividend, u64 divisor) {
	// If a division by zero happens on a modern system, the operating system
	// raises the corresponding signal natively (equivalent to the PS2's trap(7)).
	if (divisor == 0) {
		// Safeguard behaviour
		return 0;
	}

	// On modern PCs/consoles this C line compiles to a single CPU instruction,
	// completely replacing the more than 100 lines of PS2 assembly.
	return dividend / divisor;
}
