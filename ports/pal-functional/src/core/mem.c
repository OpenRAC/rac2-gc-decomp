#include "core/mem.h"
#include <string.h>

/* El ELF: for (i = 0; i < bytes/4; i++) dst[i] = src[i];
 * PC:     memcpy. Identical semántica para bloques alineados a 4 bytes
 *         (que es lo que el motor pasa). */
void mem_copy(void* dst, const void* src, size_t bytes)
{
	if (bytes)
		memcpy(dst, src, bytes);
}
