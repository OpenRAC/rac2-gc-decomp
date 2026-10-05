#include "core/mem.h"
#include <string.h>

/* The ELF: for (i = 0; i < bytes/4; i++) dst[i] = src[i];
 * PC:     memcpy. Identical semantics for 4-byte-aligned blocks
 *         (which is what the engine passes). */
void mem_copy(void* dst, const void* src, size_t bytes)
{
	if (bytes)
		memcpy(dst, src, bytes);
}
