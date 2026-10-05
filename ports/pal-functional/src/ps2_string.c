#include <string.h>
#include "types.h"

/**
 * @brief Copia de forma segura una cadena de texto hacia un búfer de destino aplicando optimización
 * vectorial MIPS por hardware e inflado de nulos (\0) remanentes.
 * Dirección original en Ghidra: 0x00115AC0 (PAL)
 *
 * @param dest_addr Dirección del buffer de destino en la RAM (param_1).
 * @param src_addr Dirección de la string de origen a copiar (param_2).
 * @param max_len Límite máximo total de caracteres a transferir para evitar desbordamientos (param_3).
 * @return u32 Retorna el puntero base del buffer de destino.
 */
u32 sys_strncpy_safe(u32 dest_addr, const char* src_addr, u32 max_len) {
	if (dest_addr == 0 || src_addr == NULL || max_len == 0) {
		return dest_addr;
	}

	char* p_dest = (char*)(uintptr_t)dest_addr;

	// En PC emulamos de forma nativa y portátil el comportamiento exacto de ráfaga
	// de la CPU MIPS de 128 bits utilizando la optimización estándar de la librería de C:
	strncpy(p_dest, src_addr, max_len);

	// El binario original de Insomniac Games garantiza que si la string es más corta 
	// que max_len, el espacio restante del búfer se rellenará forzosamente con bytes nulos (\0).
	// strncpy hace esto por especificación oficial, manteniendo la paridad del 100% con Ghidra.

	return dest_addr;
}
