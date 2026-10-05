#include <string.h>
#include "types.h"

/**
 * @brief Safely copies a text string into a destination buffer, applying the MIPS
 * hardware vector optimization and padding the remainder with nulls (\0).
 * Original Ghidra address: 0x00115AC0 (PAL)
 *
 * @param dest_addr Address of the destination buffer in RAM (param_1).
 * @param src_addr Address of the source string to copy (param_2).
 * @param max_len Maximum total number of characters to transfer, preventing overflows (param_3).
 * @return u32 Returns the base pointer of the destination buffer.
 */
u32 sys_strncpy_safe(u32 dest_addr, const char* src_addr, u32 max_len) {
	if (dest_addr == 0 || src_addr == NULL || max_len == 0) {
		return dest_addr;
	}

	char* p_dest = (char*)(uintptr_t)dest_addr;

	// On PC the exact burst behaviour of the 128-bit MIPS CPU is emulated
	// natively and portably with the standard C library optimization:
	strncpy(p_dest, src_addr, max_len);

	// The original Insomniac Games binary guarantees that if the string is shorter
	// than max_len, the remaining buffer space is always filled with null bytes (\0).
	// strncpy does this by specification, keeping full parity with Ghidra.

	return dest_addr;
}
