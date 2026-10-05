// src/kernel_sys.c
#include "core/ee_memory.h"
#include "types.h"
#include "kernel_sys.h"
#include "math_util.h"  // sys_assert_fail, txt_format_scientific_wrapper
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

// Control of the secondary/alternate static log buffer
s32   g_log_buffer_count_alt = 0;
char  g_log_static_buffer_alt[128]; // Size based on the 0x7f limit
char* g_log_buffer_write_ptr_alt = g_log_static_buffer_alt;

// Remaining static global buffer variables of the PS2
char g_dtoa_output_buffer[256] = { 0 }; // DAT_0013c100

/**
 * @brief The engine's internal vsnprintf. Formats a string with a size limit.
 * Initializes the local control structure and delegates the flow to the conditional channel dispatcher.
 * Original Ghidra address: 0x00115DA8 (PAL)
 *
 * @param p_dest_buffer Physical RAM buffer where the resulting text is written (param_8).
 * @param p_format_str Base format string with tokens (param_9).
 * @param args_list Packed variable argument list (param_10 / ...).
 * @return s32 Total number of characters written successfully.
 */
s32 txt_vsnprintf_internal(char* p_dest_buffer, const char* p_format_str, va_list args_list) {
	if (p_dest_buffer == NULL || p_format_str == NULL) {
		return 0;
	}

	// Local control structure emulating the PS2's contiguous apuStack_e0 dump
	// Indexed as 32-bit integers to interact with the dispatcher
	s32 local_buffer_struct[8];

	local_buffer_struct[0] = (s32)((long)p_dest_buffer);
	local_buffer_struct[1] = 0x7FFFFFFF; // Buffer state control flags (uStack_cc)
	local_buffer_struct[2] = 0x7FFFFFFF; // Logical physical limits (uStack_d8)
	local_buffer_struct[3] = 0x00000208; // Alignment mask and channel type (uStack_d4)

	// Invoke the smart dispatcher at once to process the string on the correct channel
	s32 total_written = txt_sprintf_channel_dispatcher(local_buffer_struct, p_format_str, args_list);

	// Retrieve the final position of the write cursor advanced by the text engines
	char* p_final_cursor = (char*)((long)local_buffer_struct[0]);

	// Insert the safety null terminator to close the string cleanly
	*p_final_cursor = '\0';

	return total_written;
}

/**
 * @brief Intercepts and dispatches format strings, checking for scientific/floating-point tokens (%e, %g, %f).
 * Dynamically distributes the processing load between the main engine and the alternate telemetry engine.
 * Original Ghidra address: 0x00118CC8 (PAL)
 */
s32 txt_sprintf_channel_dispatcher(s32* p_buffer_struct, const char* p_format_str, va_list args_list) {
	if (p_format_str == NULL || *p_format_str == '\0') {
		// Dispatch directly to the main channel if the string is empty
		void* target_destination = (void*)((long)p_buffer_struct[0x15]);
		return custom_vsprintf_engine(target_destination, (int*)p_buffer_struct, p_format_str, args_list);
	}

	const u8* p_scan = (const u8*)p_format_str;
	u8 current_char = *p_scan;

	// Scan the string for the format tokens specific to channel B
	do {
		if (current_char == 0x25) { // '%' character
			const u8* p_token = p_scan + 1;
			p_scan = p_scan + 1;

			if (*p_token != '\0') {
				// Skip the precision, width or numeric flag modifiers
				while ((char)*p_scan < 'A') {
					if (p_scan[1] == '\0') {
						current_char = *p_scan;
						goto evaluate_token;
					}
					p_scan++;
				}
				current_char = *p_scan;

			evaluate_token:
				// Check whether the final token belongs to the scientific floating-point set ('E', 'G', 'f', etc.)
				switch ((s32)(current_char - 0x45)) {
				case 0:  // 'E'
				case 2:  // 'G'
				case 7:  // 'f'
				case 0x20: // 'e'
				case 0x21: // alternate / modified 'f'
				case 0x22: // 'g'
					// Divert the execution flow to the secondary telemetry engine
					void* target_dest_alt = (void*)((long)p_buffer_struct[0x15]);
					return custom_vsprintf_engine_alt(target_dest_alt, (int*)p_buffer_struct, p_format_str, args_list);
				default:
					p_scan++;
					break;
				}
			}
		}
		else {
			p_scan++;
		}

		if (*p_scan == '\0') {
			break;
		}
		current_char = *p_scan;
	} while (1);

	// If the string contained no scientific tokens, process it through the standard master channel
	void* target_destination = (void*)((long)p_buffer_struct[0x15]);
	return custom_vsprintf_engine(target_destination, (int*)p_buffer_struct, p_format_str, args_list);
}

/**
 * @brief Secondary formatting and string-building engine of the alternate telemetry channel.
 * Parses logical tokens and injects bursts of text into Insomniac Games' log channel B.
 * Original Ghidra address: 0x00118D98 (PAL)
 */
s32 custom_vsprintf_engine_alt(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list) {
	if (p_format_str == NULL) {
		return 0;
	}

	// Initialize the original compiler's local locale mapping
	ee_ctype_interface_wrapper();

	char local_buffer[1024];

	// Portably delegate the massive parsing of flags ('-', '+', '#', '.')
	// and 64-bit precision to the native hardware of the modern CPU:
	s32 chars_written = vsnprintf(local_buffer, sizeof(local_buffer), p_format_str, args_list);

	if (chars_written > 0) {
		// If the state sets bit 0x200, write directly to the destination memory address
		if (p_state_struct != NULL && (*p_state_struct & 0x200) != 0) {
			char** p_dest_ptr = (char**)output_dest;
			ee_memcpy(*p_dest_ptr, local_buffer, chars_written);
			*p_dest_ptr += chars_written;
		}
		// Otherwise, dispatch the telemetry flow to the secondary buffer channel
		else {
			sys_log_write_buffered_alt(1, local_buffer, chars_written, 0);
			sys_log_write_buffered_alt(1, NULL, 0, 1); // Final line flush
		}
	}

	return chars_written;
}

/**
 * @brief Converts a double-precision floating-point number to its ASCII text representation (dtoa engine).
 * Supports fixed decimal (%f) and scientific exponential (%e / %g) notations.
 * Original Ghidra address: 0x001185E8 (PAL)
 *
 * @param value The original 64-bit double to format (param_1).
 * @param precision Number of requested decimal digits of precision (param_2).
 * @param format_char Token type character ('f', 'e', 'g') (param_3).
 * @param flags Control flags for zero padding or truncation (param_4).
 * @return const char* Pointer to the global static buffer holding the formatted text.
 */
const char* math_dtoa_format(double value, s32 precision, char format_char, s32 flags) {
	// On a real PS2 this requires extracting IEEE 754 exponents, running manual
	// fmod_double64 loops for the digits, applying the ASCII rounder and attaching the exponent with ee_itoa.
	// Portably and in a modern way, the current compiler performs this mathematical conversion:

	char format_specifier[16];

	// Dynamically build the native format specifier (e.g. "%.6f" or "%.6e")
	if (flags != 0 && format_char == 'g') {
		snprintf(format_specifier, sizeof(format_specifier), "%%.%dg", precision);
	}
	else {
		snprintf(format_specifier, sizeof(format_specifier), "%%.%d%c", precision, format_char);
	}

	// Perform the formatting directly in hardware on our global static buffer
	snprintf(g_dtoa_output_buffer, sizeof(g_dtoa_output_buffer), format_specifier, value);

	return g_dtoa_output_buffer;
}

/**
 * @brief Converts an integer to an ASCII character string in any numeric base (itoa / ltoa).
 * Calls ee_strrev directly to fix the final orientation of the characters in memory.
 * Original Ghidra address: 0x00118548 (PAL)
 *
 * @param value Integer to convert (param_1).
 * @param p_dest_buffer Destination RAM buffer where the string is drawn (param_2).
 * @param base Numeric base of the conversion (e.g. 10 for decimal, 16 for hexadecimal) (param_3).
 * @return char* Pointer to the start of the formatted and corrected numeric text string.
 */
char* ee_itoa(s32 value, char* p_dest_buffer, s64 base) {
	if (p_dest_buffer == NULL || base < 2 || base > 36) {
		return p_dest_buffer;
	}

	const char* digits_map = "0123456789abcdefghijklmnopqrstuvwxyz";
	u32 target_value = (u32)value;

	// For base 10 with a negative number, process the absolute value
	if (value < 0 && base == 10) {
		target_value = (u32)(-value);
	}

	s32 iterator = 0;
	s32 current_idx = 0;

	// Consecutive extraction of the logical remainders according to the numeric base
	do {
		current_idx = iterator;
		iterator++;

		p_dest_buffer[current_idx] = digits_map[target_value % (u32)base];
		target_value = target_value / (u32)base;

	} while (target_value != 0);

	// If the value was negative, insert the sign prefix in reverse
	if (value < 0 && base == 10) {
		p_dest_buffer[iterator] = '-';
		iterator = current_idx + 2;
	}

	p_dest_buffer[iterator] = '\0'; // End-of-string marker

	// Call the reversal subroutine to correct the order of the digits
	return ee_strrev(p_dest_buffer);
}

/**
 * @brief Reverses the order of the characters of a text string in place in memory (string reverse).
 * Used by the engine to fix the orientation of digits generated in reverse by the numeric converters.
 * Original Ghidra address: 0x001184D0 (PAL)
 *
 * @param p_str Pointer to the character string to reverse (param_1).
 * @return char* Returns the initial pointer of the reversed string.
 */
char* ee_strrev(char* p_str) {
	if (p_str == NULL || *p_str == '\0') {
		return p_str;
	}

	// 1. Compute the string length manually (equivalent to the PS2 for loop)
	s32 length = 0;
	while (p_str[length] != '\0') {
		length++;
	}

	s32 left_idx = 0;
	s32 right_idx = length - 1;

	// 2. Symmetric swap of characters from both ends towards the centre (crossed pointers)
	while (left_idx < right_idx) {
		char temp = p_str[left_idx];
		p_str[left_idx] = p_str[right_idx];
		p_str[right_idx] = temp;

		left_idx++;
		right_idx--;
	}

	return p_str;
}

/**
 * @brief Applies arithmetic rounding directly to an ASCII string representing a floating-point number.
 * Handles the overflow carry (domino effect) when consecutive '9' digits are found.
 * Original Ghidra address: 0x00118460 (PAL)
 *
 * @param p_str_buffer Pointer to the character buffer of the numeric string (param_1).
 * @param precision_index Position or index of the digit where the rounding cut is applied (param_2).
 * @return s32 Returns 0 if the carry overflows the start of the string, or 1 if rounding succeeded.
 */
s32 txt_round_ascii_digits(char* p_str_buffer, s64 precision_index) {
	if (p_str_buffer == NULL || precision_index <= 0) {
		return 1;
	}

	s32 index = (s32)precision_index - 1;
	char* p_target_char = p_str_buffer + index;

	// Standard rounding rule: if the evaluated digit is greater than '4' (5, 6, 7, 8, 9)
	if (*p_target_char > '4') {
		*p_target_char = '0'; // Zeroes the evaluated digit

		// Carry loop: propagates the carry to the left while '9' characters are found
		while (1) {
			index--;
			p_target_char--;

			if (index < 1 || *p_target_char != '9') {
				break;
			}
			*p_target_char = '0';
		}

		p_target_char = p_str_buffer + index;
		if (*p_target_char == '9') {
			return 0; // Signals an overflow beyond the start of the string
		}

		// Safely increment the value of the current ASCII character (e.g. '7' + 1 = '8')
		*p_target_char = *p_target_char + 1;
	}

	return 1;
}

/**
 * @brief Writes text in segments into a secondary alternate accumulation buffer.
 * Flushes automatically (autoflush) when full, or explicitly through flags.
 * Original Ghidra address: 0x00118BC0 (PAL)
 *
 * @param log_level Priority level/channel (param_1)
 * @param p_srcString Text to write (param_2)
 * @param write_len Number of characters/bytes to process (param_3)
 * @param flush_flag If 1, forces the buffer to flush immediately (param_4)
 * @return u32 Number of bytes processed successfully.
 */
u32 sys_log_write_buffered_alt(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag) {
	u32 bytes_processed = 0;

	// Alternate explicit flush control
	if (flush_flag == 1) {
		sys_log_dispatch_message(log_level, g_log_static_buffer_alt, g_log_buffer_count_alt);
		g_log_buffer_count_alt = 0;
		g_log_buffer_write_ptr_alt = g_log_static_buffer_alt;
	}
	else {
		const char* p_src = p_srcString;

		if (write_len != 0) {
			do {
				// Copy the current character into the secondary static buffer
				*g_log_buffer_write_ptr_alt = *p_src;
				g_log_buffer_count_alt++;
				g_log_buffer_write_ptr_alt++;

				// Alternate automatic autoflush beyond 128 bytes (0x7f)
				if (g_log_buffer_count_alt > 0x7F) {
					s32 dispatch_status = sys_log_dispatch_message(log_level, g_log_static_buffer_alt, g_log_buffer_count_alt);
					g_log_buffer_write_ptr_alt = g_log_static_buffer_alt;
					g_log_buffer_count_alt = 0;

					if (dispatch_status == 0) {
						g_log_buffer_count_alt = 0;
						return 0; // Stop the flow if the dispatcher fails
					}
				}

				bytes_processed++;
				p_src = p_srcString + bytes_processed;

			} while (bytes_processed < write_len);
		}
	}

	return bytes_processed;
}

/**
 * @brief Public interface wrapper that retrieves the character locale property table.
 * Original Ghidra address: 0x00115228 (PAL)
 *
 * @return const void** Direct pointer to the system locale array.
 */
const void** ee_ctype_interface_wrapper(void) {
	// Delegates and returns the query directly to the core subroutine
	return ee_get_ctype_table_ptr();
}

/**
 * @brief Public entry point for dispatching assertion failures in the engine.
 * Formats the typographic alert and hands control to the definitive system-stop handler.
 * Original Ghidra address: 0x00115E38 (PAL)
 *
 * @param p_file Path of the original source file (param_1).
 * @param line Physical line number of the error (param_2).
 * @param p_assertion Logical expression of the condition that failed (param_3).
 */
void sys_assert_dispatch(const char* p_file, s32 line, const char* p_assertion,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Collect the formatted information and inject it into the kernel logs
	/* The stream pointer is a 32-bit EE address stored at 0x00133F00 */
	s32* p_error_stream = (s32*)EE_ADDR(*(u32*)EE_ADDR(0x00133EF4 + 0xC));
	game_sprintf(p_error_stream, "assertion \"%s\" failed: file \"%s\", line %d\n", p_assertion, p_file, line);

	// 2. Transfer the execution flow to the definitive panic and freeze handler
	sys_assert_fail(p_assertion, p_file, line);
}

// Physical address of the PS2 locale pointer array
const u32* g_locale_ctype_array = (const u32*)EE_ADDR(0x0013A388);

/**
 * @brief Returns the base pointer of the character locale table array (CTYPE pointer array).
 * Internally points to the type conversion matrices used by the string functions.
 * Original Ghidra address: 0x00115210 (PAL)
 *
 * @return const void** Pointer to the address of the locale array.
 */
const void** ee_get_ctype_table_ptr(void) {
	// Directly returns access to the system pointer map
	return (const void**)g_locale_ctype_array;
}

/**
 * @brief Evaluates the console's physical RAM and determines the TLB initialization flow.
 * Synchronizes the cache if the standard 32 MB retail PS2 environment is detected.
 * Original Ghidra address: 0x0011F130 (PAL)
 */
void kernel_hardware_memory_init(void) {
	long memory_size = 0;

	// For emulation or a modern port, the original PS2 hardware read is adapted:
#if defined(PLATFORM_PS2)
	memory_size = GetMemorySize();
#else
	memory_size = 0x2000000; // Force the retail 32MB flow by default for the native port
#endif

	// 0x2000000 bytes are exactly the 32 MB of RAM of the retail PlayStation 2
	if (memory_size == 0x2000000) {
		// Call the cache page synchronization and validation routine
		kernel_tlb_cache_sync();
	}
	else {
		// Alternative initialization if a development kit is detected (PS2 TOOL / 64MB)
#if defined(PLATFORM_PS2)
		_InitTLB();
#endif
	}
}

/**
 * @brief Requests a soft, safe system stop with a clean exit code (0).
 * Used by the engine for controlled interruptions of the execution flow.
 * Original Ghidra address: 0x00131D08 (PAL)
 */
void sys_safe_exit_stub(void) {
	// Dispatch an abort with success code (0), forcing a TLB reset before exiting
	sys_kernel_panic_abort(0);
}

/**
 * @brief Stops the game on a critical error (kernel panic).
 * Forces a re-initialization of the TLB memory subsystem to stabilize the hardware before exiting.
 * Original Ghidra address: 0x0011FA20 (PAL)
 *
 * @param exit_code Error status code reported to the system (param_1).
 */
void sys_kernel_panic_abort(s32 exit_code) {
	// Try to reset and clear the PS2 RAM translation tables
	kernel_hardware_memory_init();

	// Terminate the process at once, portably on modern systems
	_Exit(exit_code);
}

// ============================================================================
// MEMORY CONTROL AND MANAGEMENT SUBSYSTEM (KERNEL)
// ============================================================================

/**
 * @brief Manages synchronization, flushing and invalidation of PS2 TLB cache pages.
 * Original Ghidra address: 0x0011F170 (PAL)
 */
long kernel_tlb_cache_sync(void) {
	s32 total_pages = g_tlb_wired_index + g_tlb_bound_index;

	txt_format_scientific_wrapper((const u8*)EE_ADDR(0x13AC50), (long)(g_tlb_wired_index - 1), (long)g_tlb_wired_index, (long)(total_pages - 1));

#if defined(PLATFORM_PS2)
	SYNC(0x10);
#endif

	s64 iterator = 0;

	// Control block A: fixed-entry overflow
	if (g_tlb_wired_index > 0x30) {
		txt_format_scientific_wrapper((const u8*)EE_ADDR(0x13AC88));
		sys_kernel_panic_abort(1); // Connected to the identified panic function
	}

	while (iterator < g_tlb_wired_index) {
#if defined(PLATFORM_PS2)
		RFU086_WaitEvnetFlag();
#endif
		iterator++;
	}

	// Control block B: checks the second page section
	if (total_pages > 0x30) {
		txt_format_scientific_wrapper((const u8*)EE_ADDR(0x13ACA0));
		sys_kernel_panic_abort(1); // Connected to the identified panic function
	}

	while (iterator < total_pages) {
#if defined(PLATFORM_PS2)
		RFU086_WaitEvnetFlag();
#endif
		iterator = iterator + 1;
	}

#if defined(PLATFORM_PS2)
	SYNC(0x10);
#endif

	g_tlb_status_sync = (s32)iterator;

	// Control block C: extra hardware flags
	if (g_tlb_extra_flags > 0) {
		s32 extra_limit = (s32)iterator + g_tlb_extra_flags;
		if (extra_limit > 0x30) {
			txt_format_scientific_wrapper((const u8*)EE_ADDR(0x13ACB8));
			sys_kernel_panic_abort(1); // Connected to the identified panic function
		}
		while (iterator < extra_limit) {
#if defined(PLATFORM_PS2)
			RFU086_WaitEvnetFlag();
#endif
			iterator++;
		}
	}

	for (; iterator < 0x30; iterator++) {
#if defined(PLATFORM_PS2)
		RFU086_WaitEvnetFlag();
#endif
	}

	return (long)((s32)iterator << 13);
}

/**
 * @brief Simplified wrapper converting text to a 64-bit integer (portable equivalent of atoll).
 * Automatically passes the system's global error pointer to the ee_strtoll routine.
 * Original Ghidra address: 0x001175F0 (PAL)
 *
 * @param p_srcString Text string to convert (param_1).
 * @param p_end_ptr Optional pointer that receives the end of the read (param_2).
 * @param base Numeric base (param_3).
 * @return ulong The resulting 64-bit number, returned as an unsigned integer.
 */
u64 ee_atoll_wrapper(const char* p_srcString, char** p_end_ptr, s32 base) {
	// Uses the global error pointer of the kernel thread (PTR_DAT_00133ef4)
	s32* p_global_errno = (s32*)EE_ADDR(0x00133EF4);

	// Dispatch the operation directly to the master function
	return (u64)ee_strtoll(p_global_errno, p_srcString, p_end_ptr, base);
}

/**
 * @brief Converts a character string to a signed 64-bit integer value (string to int64).
 * Portable reconstruction preserving the logical limits and error codes (ERANGE / 0x22) of the PS2 SDK.
 * Original Ghidra address: 0x00117278 (PAL)
 *
 * @param p_error_out Pointer receiving the system error code (param_1 / errno).
 * @param p_srcString Text string containing the number to convert (param_2).
 * @param p_end_ptr Optional pointer that receives the end of the read (param_3).
 * @param base Numeric base of the number (0 for auto-detection, 8, 10 or 16) (param_4).
 * @return s64 The resulting 64-bit integer.
 */
s64 ee_strtoll(s32* p_error_out, const char* p_srcString, char** p_end_ptr, s32 base) {
	if (p_srcString == NULL) {
		return 0;
	}

	// On a real PS2 this requires mapping characters through the flag table &PTR_DAT_0013a281,
	// computing overflow limits with math_udiv64 and accumulating digits with math_mul64.
	// Portably and in a modern way, the exact conversion is delegated to the native runtime:

	char* local_end_ptr = NULL;

	// Clear or initialize the local error variable before the call
#if defined(PLATFORM_PS2)
// Internal error register
#else
	errno = 0;
#endif

	s64 result = strtoll(p_srcString, &local_end_ptr, base);

	// Faithful mapping of the overflow check (overflow / ERANGE = 0x22)
	if (errno == ERANGE) {
		if (p_error_out != NULL) {
			*p_error_out = 0x22; // Inserts the range error (34 decimal) expected by the engine
		}
	}

	// If the game programmer requested the stop pointer, assign it back
	if (p_end_ptr != NULL) {
		*p_end_ptr = (local_end_ptr != NULL) ? local_end_ptr : (char*)p_srcString;
	}

	return result;
}

// Estimated global variables of the list structure in RAM (0x0013CAC0)
u32 g_list_root_param = 0;
u32 g_list_element_count = 0;
void* g_list_head_ptr = NULL;
void* g_list_tail_ptr = NULL;
u32 g_list_sentinel_node = 0; // Represents DAT_0013cad0

// Initialization control flag
s32 g_deci2_is_initialized = 0;

// Static log buffer control
s32   g_log_buffer_count = 0;
char  g_log_static_buffer[128]; // Size based on the 0x7f limit
char* g_log_buffer_write_ptr = g_log_static_buffer;

/**
 * @brief Main formatted text building function (the game's printf / sprintf).
 * Packs the variable stack arguments and dispatches the flow to the engine wrapper.
 * Original Ghidra address: 0x00115CF0 (PAL)
 *
 * @param p_buffer_struct Control structure of the destination text buffer (param_1)
 * @param p_format_str Base format string (e.g. "Ammo: %d/%d") (param_2)
 * @param ... Additional variable arguments to format.
 * @return s32 Total number of characters written.
 */
s32 game_sprintf(s32* p_buffer_struct, const char* p_format_str, ...) {
	s32 total_written = 0;
	va_list args;

	// Initialize the variable argument list pointing just after p_format_str
	// This portably replaces the PS2's massive stack dump (uStack_30)
	va_start(args, p_format_str);

	// Dispatch the control structure, the format and the arguments to the intermediate wrapper
	total_written = txt_sprintf_wrapper(p_buffer_struct, p_format_str, args);

	// Release the dynamic argument list
	va_end(args);

	return total_written;
}

/**
 * @brief Interface function that formats text strings into a structured buffer.
 * Extracts the destination pointer from offset 0x15 and dispatches the request to the master engine.
 * Original Ghidra address: 0x00119BC8 (PAL)
 *
 * @param p_buffer_struct Control structure of the text buffer (param_1)
 * @param p_format_str Format string (e.g. "Bolts: %d") (param_2)
 * @param args_list Variable argument list (param_3)
 * @return s32 Number of characters formatted and written.
 */
s32 txt_sprintf_wrapper(s32* p_buffer_struct, const char* p_format_str, va_list args_list) {
	if (p_buffer_struct == NULL) {
		return 0;
	}

	// Offset 0x15 (indexed as int, equivalent to bytes 0x54) holds the real destination pointer
	void* target_destination = (void*)((long)p_buffer_struct[0x15]);

	// Dispatch the operation to the formatting engine reconstructed earlier
	return custom_vsprintf_engine(target_destination, (int*)p_buffer_struct, p_format_str, args_list);
}

/**
 * @brief Functional reconstruction of Insomniac Games' typographic engine and % token interpreter.
 * Processes standard formats (%d, %i, %x, %s, %c) and dispatches bursts to the log system or memory buffers.
 * Original Ghidra address: 0x00119BF8 (PAL)
 */
s32 custom_vsprintf_engine(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list) {
	if (p_format_str == NULL) {
		return 0;
	}

	// Intermediate local buffer to emulate the character construction portably
	char local_buffer[1024];

	// Delegate the low-level format conversion (such as consecutive divisions
	// by 10 in math_div64 or the "0123456789abcdef" character maps) to the native vsnprintf:
	s32 chars_written = vsnprintf(local_buffer, sizeof(local_buffer), p_format_str, args_list);

	if (chars_written > 0) {
		// On the original PS2, if state bit 0x200 is set, it writes directly to memory (ee_memcpy).
		// Otherwise it sends it in fragments to the interface HUD buffer or the logs.
		if (p_state_struct != NULL && (*p_state_struct & 0x200) != 0) {
			// Direct burst copy to the destination address pointed to by output_dest
			char** p_dest_ptr = (char**)output_dest;
			ee_memcpy(*p_dest_ptr, local_buffer, chars_written);
			*p_dest_ptr += chars_written; // Advances the write pointer in RAM
		}
		else {
			// Dispatch the characters to the segmented buffer with autoflush active
			sys_log_write_buffered(1, local_buffer, chars_written, 0);
			sys_log_write_buffered(1, NULL, 0, 1); // Forces the final line flush
		}
	}

	return chars_written;
}

/**
 * @brief Finds the first occurrence of a byte in a memory block (memchr).
 * Portable version of the Emotion Engine's 16-byte vector burst algorithm.
 * Original Ghidra address: 0x00115250 (PAL)
 *
 * @param ptr Pointer to the initial memory block (param_1)
 * @param value Value of the byte searched for (param_2)
 * @param num Maximum number of bytes to scan (param_3)
 * @return void* Pointer to the position of the byte found, or NULL if absent.
 */
void* ee_memchr(const void* ptr, int value, u32 num) {
	if (ptr == NULL) {
		return NULL;
	}

	const unsigned char* p = (const unsigned char*)ptr;
	unsigned char target = (unsigned char)(value & 0xFF);

	// On the original PS2, a loop processes 16-byte (0x10) vector blocks
	// using parallel masks when the memory is perfectly aligned.
	// Logically, the equivalent behaviour is:
	for (u32 i = 0; i < num; i++) {
		if (p[i] == target) {
			return (void*)(p + i);
		}
	}

	return NULL;
}

/**
 * @brief Copies a memory block from a source to a destination (memcpy).
 * Portable equivalent of the PS2-optimized 32-byte block copy routine.
 * Original Ghidra address: 0x001153D4 (PAL)
 *
 * @param dest Pointer to the destination memory block (param_1)
 * @param src Pointer to the source memory block (param_2)
 * @param size Number of bytes to copy (param_3)
 * @return void* Pointer to the destination block.
 */
void* ee_memcpy(void* dest, const void* src, u32 size) {
	if (dest == NULL || src == NULL) {
		return dest;
	}

	u8* d = (u8*)dest;
	const u8* s = (const u8*)src;

	// On a real PS2, if the memory is aligned, a do-while loop runs
	// that empties and fills the registers in massive 32-byte (0x20) bursts.
	// Functionally, the identical and safe behaviour in C is:
	for (u32 i = 0; i < size; i++) {
		d[i] = s[i];
	}

	return dest;
}

/**
 * @brief Writes text in segments into a 128-byte accumulation buffer.
 * Flushes automatically (autoflush) when full, or explicitly through flags.
 * Original Ghidra address: 0x00119AC0 (PAL)
 *
 * @param log_level Priority level/channel (param_1)
 * @param p_srcString Text to write (param_2)
 * @param write_len Number of characters/bytes to process (param_3)
 * @param flush_flag If 1, forces the buffer to flush immediately (param_4)
 * @return u32 Number of bytes processed successfully.
 */
u32 sys_log_write_buffered(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag) {
	u32 bytes_processed = 0;

	// Explicit flush control
	if (flush_flag == 1) {
		sys_log_dispatch_message(log_level, g_log_static_buffer, g_log_buffer_count);
		g_log_buffer_count = 0;
		g_log_buffer_write_ptr = g_log_static_buffer;
	}
	else {
		const char* p_src = p_srcString;

		if (write_len != 0) {
			do {
				// Copy the game's current character into the static buffer
				*g_log_buffer_write_ptr = *p_src;
				g_log_buffer_count++;
				g_log_buffer_write_ptr++;

				// Autoflush: automatic flush beyond 128 bytes (0x7f)
				if (g_log_buffer_count > 0x7F) {
					s32 dispatch_status = sys_log_dispatch_message(log_level, g_log_static_buffer, g_log_buffer_count);
					g_log_buffer_write_ptr = g_log_static_buffer;
					g_log_buffer_count = 0;

					if (dispatch_status == 0) {
						g_log_buffer_count = 0;
						return 0; // Stop the flow if the dispatcher fails
					}
				}

				bytes_processed++;
				p_src = p_srcString + bytes_processed;

			} while (bytes_processed < write_len);
		}
	}

	return bytes_processed;
}

/**
 * @brief Dispatches and filters the game's diagnostic messages to the log subsystem.
 * Performs on-demand (lazy) initialization of the network system if it is not active.
 * Original Ghidra address: 0x0011B1E8 (PAL)
 *
 * @param log_level Priority level or channel of the message (param_1)
 * @param p_message Character string of the message (param_2)
 * @param message_len Length of the text string (param_3)
 * @return s32 Number of characters printed, or -1 if the channel is invalid or the network fails.
 */
s32 sys_log_dispatch_message(u32 log_level, const char* p_message, s32 message_len) {
	s32 result_status = -1;

	// Safely check whether the level is 1 or 2 (equivalent to: log_level - 1U < 2)
	if (log_level == 1 || log_level == 2) {

		// On-demand initialization of the system on first use
		if (g_deci2_is_initialized == 0) {
			bool init_success = sys_deci2_subsystem_init();
			if (!init_success) {
				return -1; // Aborts if the hardware subsystem fails
			}
			g_deci2_is_initialized = 1;
		}

		// Send the formatted message to the network print buffer
		result_status = sys_deci2_print_log(p_message, message_len);
	}

	return result_status;
}

/**
 * @brief Initializes a global linked list or memory management queue of the engine.
 * Configures the root node, initializes the counter to 0 and links the head and tail pointers.
 * Original Ghidra address: 0x0011BAC8 (PAL)
 *
 * @param param_1 Initial configuration parameter or capacity (register a0)
 * @return void* Pointer to the header of the initialized global structure.
 */
void* sys_queue_initialize(u32 param_1) {
	g_list_root_param = param_1;
	g_list_element_count = 0; // Initializes the current size/counter to zero

	// Both head and tail initially point to the empty base node (DAT_0013cad0)
	g_list_head_ptr = &g_list_sentinel_node;
	g_list_tail_ptr = &g_list_sentinel_node;

	return &g_list_root_param;
}

// System network control variables
s32 g_deci2_channel_status = -1;
u32 g_deci2_var1 = 0;
u32 g_deci2_var2 = 0;
u32 g_deci2_var3 = 0;
void* g_deci2_buffer_a = NULL;
void* g_deci2_buffer_b = NULL;
void* g_deci2_queue_ptr = NULL;

// Dummy variables of the network header buffer
u16 g_net_packet_size = 0;
u8  g_net_packet_id1 = 0;
u8  g_net_packet_id2 = 0;
u8  g_net_packet_flags = 0;
u16 g_net_packet_type = 0;

// Semaphore and print control
s32 g_deci2_print_mutex = 0;
s32 g_deci2_packet_len = 0;
char g_deci2_print_buffer[256]; // Represents the physical space at DAT_2013cc0c

/**
 * @brief Transmits a formatted log message to the remote debugging system.
 * Translates newline characters and interacts with the DECI2 network channels.
 * Original Ghidra address: 0x0011BCD0 (PAL)
 *
 * @param p_message Character string to print (param_1)
 * @param max_len Maximum length to process (param_2)
 * @return int Number of characters transmitted successfully, or -1 on a block/error.
 */
s32 sys_deci2_print_log(const char* p_message, s32 max_len) {
	s32 total_chars_written = 0;
	s32 status_code = -1;

	if (g_deci2_print_mutex == 0) {
		kernel_system_sync_guard();
		g_deci2_print_mutex = 1;
		status_code = 0;

		// Character formatting and copy loop (at most 256 bytes)
		while (total_chars_written < 256 && max_len > 0) {
			max_len--;
			char current_char = *p_message;

			// Convert the Unix newline (\n) to the console format (\r)
			if (current_char == '\n') {
				g_deci2_print_buffer[total_chars_written] = '\r';
				total_chars_written++;
				if (total_chars_written >= 256) break;
			}

			g_deci2_print_buffer[total_chars_written] = current_char;
			total_chars_written++;
			p_message++;
			status_code++;
		}

		// Configure the packet size by adding the base header (12 bytes / 0xc)
		g_deci2_packet_len = total_chars_written + 0xC;

		// Simulate sending the data through the system calls
		sys_deci2_call_wrapper();

		// On PC or modern platforms, redirect this log directly to the native console
#ifndef PLATFORM_PS2
		g_deci2_print_buffer[total_chars_written] = '\0'; // Make sure the string is terminated
		// printf("[PS2 LOG]: %s", g_deci2_print_buffer); // Uncomment to see the logs in the port
		g_deci2_print_mutex = 0;
#endif

		// The original game actively waits for the channel C hardware to clear the mutex
		// while (g_deci2_print_mutex != 0) { sys_deci2_call_channel_c(); }

		kernel_system_sync_release();
	}

	return status_code;
}

/**
 * @brief Initializes the engine's DECI2 debug communication subsystem.
 * Configures the transmission queue and writes the protocol headers to memory.
 * Original Ghidra address: 0x0011BE20 (PAL)
 *
 * @return bool Returns true if the network channel initialized correctly, false otherwise.
 */
bool sys_deci2_subsystem_init(void) {
	// 1. Clear the central processor caches
#ifdef PLATFORM_PS2
	FlushCache(0); // WRITEBACK_DCACHE
#endif

	// 2. Try to open and verify the state of the debug channel
	// Note: sys_deci2_call_channel_a() in our portable port returns a passive state (e.g. 0)
	g_deci2_channel_status = sys_deci2_call_channel_a();

	bool is_channel_valid = (-1 < g_deci2_channel_status);

	if (is_channel_valid) {
		// Initialize the internal state variables
		g_deci2_var1 = 0;
		g_deci2_var2 = 0;
		g_deci2_var3 = 0;

		// Assign the global buffers of the diagnostic system
		g_deci2_buffer_a = (void*)EE_ADDR(0x2013CD40);
		g_deci2_buffer_b = (void*)EE_ADDR(0x2013CC00);

		// Write the network protocol header values of the Insomniac Games engine
		g_net_packet_size = 0x210; // Network packet size
		g_net_packet_id1 = 0x45;  // Protocol identifier ('E')
		g_net_packet_id2 = 0x48;  // Protocol identifier ('H')
		g_net_packet_flags = 0;
		g_net_packet_type = 0;

		// 3. Initialize the dynamic message queue with a capacity of 256 elements (0x100)
		g_deci2_queue_ptr = sys_queue_initialize(0x100);
	}

	return is_channel_valid;
}

/**
 * @brief Computes the length of a character string (strlen).
 * Portable equivalent of the Emotion Engine's optimized bitwise routine.
 * Original Ghidra address: 0x001157AC (PAL)
 *
 * @param str Pointer to the input text string.
 * @return int The number of characters in the string (excluding the terminating null).
 */
int ee_strlen(const char* str) {
	if (str == NULL) {
		return 0;
	}

	const char* s = str;

	// The PS2 engine uses parallel bitwise operations (0xfefefefefefefeff)
	// to scan memory blocks of 8 and 16 bytes simultaneously.
	// Conceptually, its exact logical behaviour is to advance until '\0' is found:
	while (*s != '\0') {
		s++;
	}

	// Return the address difference, i.e. the exact length
	return (int)(s - str);
}

/**
 * @brief Compares two character strings (strcmp).
 * Portable equivalent of the Emotion Engine processor's optimized bitwise routine.
 * Original Ghidra address: 0x00115544 (PAL)
 *
 * @param str1 First string to compare (param_1)
 * @param str2 Second string to compare (param_2)
 * @return int 0 if identical, a negative value if str1 < str2, or a positive one if str1 > str2.
 */
int ee_strcmp(const char* str1, const char* str2) {
	if (str1 == NULL || str2 == NULL) {
		return (str1 == str2) ? 0 : (str1 == NULL ? -1 : 1);
	}

	const unsigned char* s1 = (const unsigned char*)str1;
	const unsigned char* s2 = (const unsigned char*)str2;

	// On a real PS2, if the memory is aligned, parallel multimedia subtractions run.
	// Conceptually, its logical behaviour equals iterating byte by byte:
	while (*s1 != '\0' && *s1 == *s2) {
		s1++;
		s2++;
	}

	// Return the exact mathematical difference between the characters where they differ
	return (int)*s1 - (int)*s2;
}

/**
 * @brief Decodes a multi-byte character of the text engine according to the active encoding.
 * Supports JIS/SJIS encoding fallbacks inherited from the original development.
 * Original Ghidra address: 0x00115FC0 (PAL)
 *
 * @param p_localeContext Active locale and language context (param_1)
 * @param p_outChar Pointer receiving the final decoded character (param_2)
 * @param p_srcString Pointer to the text string to read (param_3)
 * @param max_bytes Limit of safe bytes to process (param_4)
 * @param p_state Escape sequence control state (param_5)
 * @return int Number of bytes read for the character, or -1 on error.
 */
s32 txt_decode_multibyte_char(u8* p_localeContext, u32* p_outChar, const u8* p_srcString, u32 max_bytes, s32* p_state) {
	if (p_srcString == NULL) {
		return 0;
	}

	// Basic engine safety check
	if (max_bytes == 0) {
		return -1; // Equivalent to the 0xffffffff returned on PS2
	}

	// The original PS2 engine compares against "C-SJIS", "C-EUCJP" and "C-JIS"
	// with ee_strcmp to decide whether to process double-byte characters.
	// For the functional port of the Western releases (PAL and NTSC-U):

	u8 lead_byte = *p_srcString;

	// If it is not a special Asian format, process it as a standard 1-byte character
	if (p_outChar != NULL) {
		*p_outChar = (u32)lead_byte;
	}

	return (lead_byte != 0) ? 1 : 0;
}

/**
 * @brief Pass-through wrapper of Sony's DECI2 debug system call.
 * Originally used on development kits (PS2 TOOL) to send logs to the programmer's PC.
 * Original Ghidra address: 0x0011B9C8 (PAL)
 */
void sys_deci2_call_wrapper(void) {
	// On retail consoles and modern emulators this call has no functional effect.
	// For the native port it is documented and left as an empty direct return (stub).
	return;
}

/**
 * @brief Second pass-through wrapper of Sony's DECI2 debug channel.
 * Original Ghidra address: 0x0011B978 (PAL)
 */
s32 sys_deci2_call_channel_a(void) {
	// Passive return (stub) for compatibility in the native port: 0 = channel available/no error.
	return 0;
}

/**
 * @brief Third pass-through wrapper of Sony's DECI2 debug channel.
 * Original Ghidra address: 0x0011B9F8 (PAL)
 */
void sys_deci2_call_channel_c(void) {
	// Passive direct return (stub) for structural compatibility in the native port.
	return;
}
