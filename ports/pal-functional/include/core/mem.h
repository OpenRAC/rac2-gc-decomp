#ifndef MEM_H
#define MEM_H

#include <stddef.h>

/* [CONFIRM] The engine's word-by-word memory copy helper.
 *   The ELF implements it as a 4-byte lw/sw loop.
 *   On PC: memcpy. param_3 = size in BYTES (the ELF shifts it >> 2
 *   to obtain the word count).
 *
 *   Called from display_init_channel_b (0x11f000) and possibly
 *   from other handshakes that copy render buffers. */
void mem_copy(void* dst, const void* src, size_t bytes);

#endif /* MEM_H */
