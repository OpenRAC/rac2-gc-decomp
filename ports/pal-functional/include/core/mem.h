#ifndef MEM_H
#define MEM_H

#include <stddef.h>

/* [CONFIRM] Helper de copia de memoria palabra a palabra del motor.
 *   El ELF lo implementa como un bucle lw/sw de 4 bytes.
 *   En PC: memcpy. param_3 = tamaño en BYTES (el ELF lo divide >> 2
 *   para obtener el count de palabras).
 *
 *   Llamado desde display_init_channel_b (0x11f000) y posiblemente
 *   desde otros handshakes que copian buffers de render. */
void mem_copy(void* dst, const void* src, size_t bytes);

#endif /* MEM_H */
