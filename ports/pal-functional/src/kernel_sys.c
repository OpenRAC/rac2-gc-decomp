// src/kernel_sys.c
#include "types.h"
#include "kernel_sys.h"
#include "math_util.h"  // sys_assert_fail, txt_format_scientific_wrapper
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>

// Control de buffer estático secundario/alternativo de logs
s32   g_log_buffer_count_alt = 0;
char  g_log_static_buffer_alt[128]; // Tamaño basado en el límite 0x7f
char* g_log_buffer_write_ptr_alt = g_log_static_buffer_alt;

// Variables de buffer globales estáticas remanentes de la PS2
char g_dtoa_output_buffer[256] = { 0 }; // DAT_0013c100

/**
 * @brief Función interna vsnprintf del motor. Da formato a un string con límite de tamaño.
 * Inicializa la estructura de control local y delega el flujo al despachador de canales condicional.
 * Dirección original en Ghidra: 0x00115DA8 (PAL)
 *
 * @param p_dest_buffer Búfer físico en la RAM donde se escribirá el texto resultante (param_8).
 * @param p_format_str Cadena de formato base con tokens (param_9).
 * @param args_list Lista de argumentos variables empaquetados (param_10 / ...).
 * @return s32 Cantidad total de caracteres escritos con éxito.
 */
s32 txt_vsnprintf_internal(char* p_dest_buffer, const char* p_format_str, va_list args_list) {
	if (p_dest_buffer == NULL || p_format_str == NULL) {
		return 0;
	}

	// Estructura de control local que emula el volcado contiguo apuStack_e0 de la PS2
	// Indexada como enteros de 32 bits para interactuar con el despachador
	s32 local_buffer_struct[8];

	local_buffer_struct[0] = (s32)((long)p_dest_buffer);
	local_buffer_struct[1] = 0x7FFFFFFF; // Banderas de control de estado del búfer (uStack_cc)
	local_buffer_struct[2] = 0x7FFFFFFF; // Límites físicos lógicos (uStack_d8)
	local_buffer_struct[3] = 0x00000208; // Máscara de alineación y tipo de canal (uStack_d4)

	// Invoca de golpe al despachador inteligente para procesar la cadena por el canal correcto
	s32 total_written = txt_sprintf_channel_dispatcher(local_buffer_struct, p_format_str, args_list);

	// Recupera la posición final del cursor de escritura avanzado por los motores de texto
	char* p_final_cursor = (char*)((long)local_buffer_struct[0]);

	// Inyecta el terminador nulo de seguridad para cerrar el string de forma limpia
	*p_final_cursor = '\0';

	return total_written;
}

/**
 * @brief Intercepta y despacha cadenas de formato evaluando la presencia de tokens científicos/flotantes (%e, %g, %f).
 * Distribuye de forma dinámica la carga de procesamiento entre el motor principal y el motor alterno de telemetría.
 * Dirección original en Ghidra: 0x00118CC8 (PAL)
 */
s32 txt_sprintf_channel_dispatcher(s32* p_buffer_struct, const char* p_format_str, va_list args_list) {
	if (p_format_str == NULL || *p_format_str == '\0') {
		// Despacho directo al canal principal si el string está vacío
		void* target_destination = (void*)((long)p_buffer_struct[0x15]);
		return custom_vsprintf_engine(target_destination, (int*)p_buffer_struct, p_format_str, args_list);
	}

	const u8* p_scan = (const u8*)p_format_str;
	u8 current_char = *p_scan;

	// Peina el string buscando tokens de formato específicos del canal B
	do {
		if (current_char == 0x25) { // Carácter '%'
			const u8* p_token = p_scan + 1;
			p_scan = p_scan + 1;

			if (*p_token != '\0') {
				// Se salta los modificadores de precisión, ancho o flags numéricos
				while ((char)*p_scan < 'A') {
					if (p_scan[1] == '\0') {
						current_char = *p_scan;
						goto evaluate_token;
					}
					p_scan++;
				}
				current_char = *p_scan;

			evaluate_token:
				// Evalúa si el token final pertenece al set de formato flotante científico ('E', 'G', 'f', etc.)
				switch ((s32)(current_char - 0x45)) {
				case 0:  // 'E'
				case 2:  // 'G'
				case 7:  // 'f'
				case 0x20: // 'e'
				case 0x21: // 'f' alternativo / modificado
				case 0x22: // 'g'
					// Desvía de golpe el flujo de ejecución hacia el motor secundario de telemetría
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

	// Si el string no contenía tokens científicos, se procesa por el canal maestro estándar
	void* target_destination = (void*)((long)p_buffer_struct[0x15]);
	return custom_vsprintf_engine(target_destination, (int*)p_buffer_struct, p_format_str, args_list);
}

/**
 * @brief Motor secundario de formateo y construcción de strings del canal alterno de telemetría.
 * Analiza tokens lógicos e inyecta ráfagas de texto en el canal B de logs de Insomniac Games.
 * Dirección original en Ghidra: 0x00118D98 (PAL)
 */
s32 custom_vsprintf_engine_alt(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list) {
	if (p_format_str == NULL) {
		return 0;
	}

	// Inicializa el mapeo local de localización del compilador original
	ee_ctype_interface_wrapper();

	char local_buffer[1024];

	// Delegamos de forma portátil el parseo masivo de flags ('-', '+', '#', '.')
	// y precisión de 64 bits al hardware nativo de la CPU moderna:
	s32 chars_written = vsnprintf(local_buffer, sizeof(local_buffer), p_format_str, args_list);

	if (chars_written > 0) {
		// Si el estado activa el bit 0x200, escribe directo en la dirección de memoria destino
		if (p_state_struct != NULL && (*p_state_struct & 0x200) != 0) {
			char** p_dest_ptr = (char**)output_dest;
			ee_memcpy(*p_dest_ptr, local_buffer, chars_written);
			*p_dest_ptr += chars_written;
		}
		// Si no, despacha el flujo de telemetría a tu canal secundario de buffers
		else {
			sys_log_write_buffered_alt(1, local_buffer, chars_written, 0);
			sys_log_write_buffered_alt(1, NULL, 0, 1); // Flush final de línea
		}
	}

	return chars_written;
}

/**
 * @brief Convierte un número flotante de doble precisión a su representación en texto ASCII (Dtoa Engine).
 * Soporta representaciones decimales fijas (%f) y notaciones exponenciales científicas (%e / %g).
 * Dirección original en Ghidra: 0x001185E8 (PAL)
 *
 * @param value El número double original de 64 bits que se va a formatear (param_1).
 * @param precision Cantidad de dígitos decimales de precisión solicitados (param_2).
 * @param format_char Carácter del tipo de token ('f', 'e', 'g') (param_3).
 * @param flags Banderas de control de relleno o truncamiento de ceros (param_4).
 * @return const char* Puntero al búfer estático global que almacena el texto formateado.
 */
const char* math_dtoa_format(double value, s32 precision, char format_char, s32 flags) {
	// En la PS2 real, este proceso requiere extraer exponentes IEEE 754, ejecutar bucles manuales de
	// fmod_double64 para los dígitos, aplicar el redondeador ascii y amarrar el exponente con ee_itoa.
	// De forma portable y moderna, el compilador actual resuelve esta conversión matemática:

	char format_specifier[16];

	// Construye dinámicamente el especificador de formato nativo (ej. "%.6f" o "%.6e")
	if (flags != 0 && format_char == 'g') {
		snprintf(format_specifier, sizeof(format_specifier), "%%.%dg", precision);
	}
	else {
		snprintf(format_specifier, sizeof(format_specifier), "%%.%d%c", precision, format_char);
	}

	// Ejecuta el formateo directo por hardware sobre nuestro buffer estático global
	snprintf(g_dtoa_output_buffer, sizeof(g_dtoa_output_buffer), format_specifier, value);

	return g_dtoa_output_buffer;
}

/**
 * @brief Convierte un número entero a una cadena de caracteres ASCII en cualquier base numérica (Itoa / Ltoa).
 * Invoca de forma directa a ee_strrev para rectificar la orientación final de los caracteres en la memoria.
 * Dirección original en Ghidra: 0x00118548 (PAL)
 *
 * @param value Número entero a convertir (param_1).
 * @param p_dest_buffer Búfer de destino en memoria RAM donde se dibujará la string (param_2).
 * @param base Base numérica de conversión (ej. 10 para decimal, 16 para hexadecimal) (param_3).
 * @return char* Puntero al inicio de la cadena de texto numérica ya formateada y corregida.
 */
char* ee_itoa(s32 value, char* p_dest_buffer, s64 base) {
	if (p_dest_buffer == NULL || base < 2 || base > 36) {
		return p_dest_buffer;
	}

	const char* digits_map = "0123456789abcdefghijklmnopqrstuvwxyz";
	u32 target_value = (u32)value;

	// Si es base 10 y el número es negativo, procesa el valor absoluto
	if (value < 0 && base == 10) {
		target_value = (u32)(-value);
	}

	s32 iterator = 0;
	s32 current_idx = 0;

	// Extracción consecutiva de residuos lógicos según la base numérica
	do {
		current_idx = iterator;
		iterator++;

		p_dest_buffer[current_idx] = digits_map[target_value % (u32)base];
		target_value = target_value / (u32)base;

	} while (target_value != 0);

	// Si el valor era negativo, inyecta el prefijo de signo de reversa
	if (value < 0 && base == 10) {
		p_dest_buffer[iterator] = '-';
		iterator = current_idx + 2;
	}

	p_dest_buffer[iterator] = '\0'; // Marcador de fin de string

	// Invoca a tu subrutina de inversión para corregir el orden de los dígitos
	return ee_strrev(p_dest_buffer);
}

/**
 * @brief Invierte el orden de los caracteres de una cadena de texto de forma directa en la memoria (String Reverse).
 * Utilizado por el motor para corregir la orientación de dígitos generados de reversa por los convertidores numéricos.
 * Dirección original en Ghidra: 0x001184D0 (PAL)
 *
 * @param p_str Puntero a la cadena de caracteres que se va a invertir (param_1).
 * @return char* Retorna el puntero inicial de la cadena ya invertida.
 */
char* ee_strrev(char* p_str) {
	if (p_str == NULL || *p_str == '\0') {
		return p_str;
	}

	// 1. Calcula manualmente la longitud del string (Equivalente al bucle for de la PS2)
	s32 length = 0;
	while (p_str[length] != '\0') {
		length++;
	}

	s32 left_idx = 0;
	s32 right_idx = length - 1;

	// 2. Intercambio simétrico de caracteres desde los extremos hacia el centro (Punteros cruzados)
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
 * @brief Aplica redondeo aritmético directo sobre una cadena de texto ASCII que representa un número flotante.
 * Maneja el arrastre de desbordamiento (efecto dominó) si se encuentran dígitos '9' consecutivos.
 * Dirección original en Ghidra: 0x00118460 (PAL)
 *
 * @param p_str_buffer Puntero al búfer de caracteres de la string numérica (param_1).
 * @param precision_index Posición o índice del dígito donde se aplica el corte de redondeo (param_2).
 * @return s32 Retorna 0 si el desbordamiento desborda el inicio del string, o 1 si el redondeo fue exitoso.
 */
s32 txt_round_ascii_digits(char* p_str_buffer, s64 precision_index) {
	if (p_str_buffer == NULL || precision_index <= 0) {
		return 1;
	}

	s32 index = (s32)precision_index - 1;
	char* p_target_char = p_str_buffer + index;

	// Regla de redondeo estándar: Si el dígito de evaluación es mayor a '4' (5, 6, 7, 8, 9)
	if (*p_target_char > '4') {
		*p_target_char = '0'; // Pone a cero el dígito evaluado

		// Bucle de arrastre: Propaga el acarreo hacia la izquierda mientras encuentre caracteres '9'
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
			return 0; // Indica desbordamiento fuera de los límites iniciales del string
		}

		// Incrementa de forma segura el valor del carácter ASCII actual (ej. '7' + 1 = '8')
		*p_target_char = *p_target_char + 1;
	}

	return 1;
}

/**
 * @brief Escribe texto de forma segmentada dentro de un búfer secundario de acumulación alterno.
 * Realiza un vaciado automático (autoflush) al llenarse o de forma explícita mediante flags.
 * Dirección original en Ghidra: 0x00118BC0 (PAL)
 *
 * @param log_level Nivel de prioridad/canal (param_1)
 * @param p_srcString Texto a escribir (param_2)
 * @param write_len Cantidad de caracteres/bytes a procesar (param_3)
 * @param flush_flag Si es 1, fuerza el vaciado del búfer de forma inmediata (param_4)
 * @return u32 Cantidad de bytes procesados con éxito.
 */
u32 sys_log_write_buffered_alt(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag) {
	u32 bytes_processed = 0;

	// Control de vaciado explícito alternativo (Flush)
	if (flush_flag == 1) {
		sys_log_dispatch_message(log_level, g_log_static_buffer_alt, g_log_buffer_count_alt);
		g_log_buffer_count_alt = 0;
		g_log_buffer_write_ptr_alt = g_log_static_buffer_alt;
	}
	else {
		const char* p_src = p_srcString;

		if (write_len != 0) {
			do {
				// Copia el carácter actual al búfer estático secundario
				*g_log_buffer_write_ptr_alt = *p_src;
				g_log_buffer_count_alt++;
				g_log_buffer_write_ptr_alt++;

				// Autoflush automático alterno si supera los 128 bytes (0x7f)
				if (g_log_buffer_count_alt > 0x7F) {
					s32 dispatch_status = sys_log_dispatch_message(log_level, g_log_static_buffer_alt, g_log_buffer_count_alt);
					g_log_buffer_write_ptr_alt = g_log_static_buffer_alt;
					g_log_buffer_count_alt = 0;

					if (dispatch_status == 0) {
						g_log_buffer_count_alt = 0;
						return 0; // Detener flujo si el despachador falla
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
 * @brief Envoltorio de interfaz pública para recuperar la tabla de propiedades de localización de caracteres.
 * Dirección original en Ghidra: 0x00115228 (PAL)
 *
 * @return const void** Puntero directo al arreglo de localización del sistema.
 */
const void** ee_ctype_interface_wrapper(void) {
	// Delega y retorna de forma directa la consulta a la subrutina del núcleo
	return ee_get_ctype_table_ptr();
}

/**
 * @brief Punto de entrada público para el despacho de fallos de aserción en el motor.
 * Formatea la alerta tipográfica y cede el control al manejador definitivo de detención del sistema.
 * Dirección original en Ghidra: 0x00115E38 (PAL)
 *
 * @param p_file Ruta del archivo original de código fuente (param_1).
 * @param line Número de línea física del error (param_2).
 * @param p_assertion Expresión lógica de la condición que falló (param_3).
 */
void sys_assert_dispatch(const char* p_file, s32 line, const char* p_assertion,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Recopila e inyecta la información formateada en los logs del kernel
	s32* p_error_stream = *(s32**)(0x00133EF4 + 0xC);
	game_sprintf(p_error_stream, "assertion \"%s\" failed: file \"%s\", line %d\n", p_assertion, p_file, line);

	// 2. Transfiere el flujo de ejecución al manejador definitivo de pánico y congelamiento
	sys_assert_fail(p_assertion, p_file, line);
}

// Dirección física del arreglo de punteros de localización en la PS2
const u32* g_locale_ctype_array = (const u32*)0x0013A388;

/**
 * @brief Recupera el puntero base del arreglo de tablas de localización de caracteres (CTYPE Pointer Array).
 * Apunta internamente a las matrices de conversión de tipos utilizadas por las funciones de strings.
 * Dirección original en Ghidra: 0x00115210 (PAL)
 *
 * @return const void** Puntero a la dirección del arreglo de localización.
 */
const void** ee_get_ctype_table_ptr(void) {
	// Retorna de forma directa el acceso al mapa de punteros del sistema
	return (const void**)g_locale_ctype_array;
}

/**
 * @brief Evalúa la memoria RAM física de la consola y determina el flujo de inicialización del TLB.
 * Sincroniza la caché si se detecta el entorno comercial estándar de 32 MB de la PS2.
 * Dirección original en Ghidra: 0x0011F130 (PAL)
 */
void kernel_hardware_memory_init(void) {
	long memory_size = 0;

	// En emulación o port moderno, adaptamos la lectura del hardware original de la PS2:
#if defined(PLATFORM_PS2)
	memory_size = GetMemorySize();
#else
	memory_size = 0x2000000; // Forzamos por defecto el flujo comercial de 32MB para el port nativo
#endif

	// 0x2000000 bytes equivalen exactamente a los 32 MB de RAM de la PlayStation 2 comercial
	if (memory_size == 0x2000000) {
		// Invoca a tu rutina de sincronización y validación de páginas de caché
		kernel_tlb_cache_sync();
	}
	else {
		// Inicialización alternativa si se detecta un Kit de Desarrollo (PS2 TOOL / 64MB)
#if defined(PLATFORM_PS2)
		_InitTLB();
#endif
	}
}

/**
 * @brief Invoca una parada suave y segura del sistema enviando un código de salida limpio (0).
 * Utilizado por el motor para interrupciones controladas del flujo de ejecución.
 * Dirección original en Ghidra: 0x00131D08 (PAL)
 */
void sys_safe_exit_stub(void) {
	// Despacha un aborto con código de éxito (0), forzando el reinicio del TLB antes de salir
	sys_kernel_panic_abort(0);
}

/**
 * @brief Detiene la ejecución del juego ante un error crítico (Kernel Panic).
 * Fuerza una reinicialización del subsistema de memoria TLB para estabilizar el hardware antes de salir.
 * Dirección original en Ghidra: 0x0011FA20 (PAL)
 *
 * @param exit_code Código de estado de error que se reportará al sistema (param_1).
 */
void sys_kernel_panic_abort(s32 exit_code) {
	// Intenta reiniciar y limpiar las tablas de traducción de memoria RAM de la PS2
	kernel_hardware_memory_init();

	// Cierra el proceso de golpe de forma portable en sistemas modernos
	_Exit(exit_code);
}

// ============================================================================
// SUBSISTEMA DE CONTROL Y GESTIÓN DE MEMORIA (KERNEL)
// ============================================================================

/**
 * @brief Gestiona la sincronización, vaciado e invalidación de páginas de la memoria caché TLB de la PS2.
 * Dirección original en Ghidra: 0x0011F170 (PAL)
 */
long kernel_tlb_cache_sync(void) {
	s32 total_pages = g_tlb_wired_index + g_tlb_bound_index;

	txt_format_scientific_wrapper((const u8*)0x13AC50, (long)(g_tlb_wired_index - 1), (long)g_tlb_wired_index, (long)(total_pages - 1));

#if defined(PLATFORM_PS2)
	SYNC(0x10);
#endif

	s64 iterator = 0;

	// Bloque de control A: Desbordamiento de entradas fijas
	if (g_tlb_wired_index > 0x30) {
		txt_format_scientific_wrapper((const u8*)0x13AC88);
		sys_kernel_panic_abort(1); // ¡CAMBIO AQUÍ! Conexión con tu función revelada
	}

	while (iterator < g_tlb_wired_index) {
#if defined(PLATFORM_PS2)
		RFU086_WaitEvnetFlag();
#endif
		iterator++;
	}

	// Bloque de control B: Verifica la segunda sección de páginas
	if (total_pages > 0x30) {
		txt_format_scientific_wrapper((const u8*)0x13ACA0);
		sys_kernel_panic_abort(1); // ¡CAMBIO AQUÍ! Conexión con tu función revelada
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

	// Bloque de control C: Banderas extras de hardware
	if (g_tlb_extra_flags > 0) {
		s32 extra_limit = (s32)iterator + g_tlb_extra_flags;
		if (extra_limit > 0x30) {
			txt_format_scientific_wrapper((const u8*)0x13ACB8);
			sys_kernel_panic_abort(1); // ¡CAMBIO AQUÍ! Conexión con tu función revelada
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
 * @brief Envoltorio simplificado para convertir texto a entero de 64 bits (Equivalente portable a atoll).
 * Pasa de forma automática el puntero de error global del sistema a la rutina ee_strtoll.
 * Dirección original en Ghidra: 0x001175F0 (PAL)
 *
 * @param p_srcString Cadena de texto a convertir (param_1).
 * @param p_end_ptr Puntero opcional donde se almacena el final de la lectura (param_2).
 * @param base Base numérica (param_3).
 * @return ulong El número de 64 bits resultante tratado como entero sin signo en el retorno.
 */
u64 ee_atoll_wrapper(const char* p_srcString, char** p_end_ptr, s32 base) {
	// Utiliza el puntero global de errores del hilo del kernel (PTR_DAT_00133ef4)
	s32* p_global_errno = (s32*)0x00133EF4;

	// Despacha la operación de manera directa a nuestra función maestra
	return (u64)ee_strtoll(p_global_errno, p_srcString, p_end_ptr, base);
}

/**
 * @brief Convierte una cadena de caracteres a un valor entero de 64 bits con signo (String to Int64).
 * Reconstrucción portable que preserva los límites lógicos y códigos de error (ERANGE / 0x22) del SDK de PS2.
 * Dirección original en Ghidra: 0x00117278 (PAL)
 *
 * @param p_error_out Puntero donde se almacena el código de error del sistema (param_1 / errno).
 * @param p_srcString Cadena de texto que contiene el número a convertir (param_2).
 * @param p_end_ptr Puntero opcional donde se almacena el final de la lectura (param_3).
 * @param base Base numérica del número (0 para detección automática, 8, 10 o 16) (param_4).
 * @return s64 El número entero de 64 bits resultante.
 */
s64 ee_strtoll(s32* p_error_out, const char* p_srcString, char** p_end_ptr, s32 base) {
	if (p_srcString == NULL) {
		return 0;
	}

	// En la PS2 real, esto requiere mapear caracteres vía la tabla de banderas &PTR_DAT_0013a281,
	// calcular límites de overflow usando math_udiv64, y acumular dígitos con math_mul64.
	// De forma portable y moderna, delegamos la conversión exacta al runtime nativo:

	char* local_end_ptr = NULL;

	// Limpia o inicializa la variable local de errores antes de la llamada
#if defined(PLATFORM_PS2)
// Registro interno de errores
#else
	errno = 0;
#endif

	s64 result = strtoll(p_srcString, &local_end_ptr, base);

	// Mapeo fiel del control de desbordamiento (Overflow / ERANGE = 0x22)
	if (errno == ERANGE) {
		if (p_error_out != NULL) {
			*p_error_out = 0x22; // Inyecta el error de rango (34 decimal) esperado por el motor
		}
	}

	// Si el programador del juego solicitó el puntero de parada, lo asigna de vuelta
	if (p_end_ptr != NULL) {
		*p_end_ptr = (local_end_ptr != NULL) ? local_end_ptr : (char*)p_srcString;
	}

	return result;
}

// Variables globales estimadas de la estructura de la lista en memoria RAM (0x0013CAC0)
u32 g_list_root_param = 0;
u32 g_list_element_count = 0;
void* g_list_head_ptr = NULL;
void* g_list_tail_ptr = NULL;
u32 g_list_sentinel_node = 0; // Representa a DAT_0013cad0

// Bandera de control de inicialización
s32 g_deci2_is_initialized = 0;

// Control de buffer estático de logs
s32   g_log_buffer_count = 0;
char  g_log_static_buffer[128]; // Tamaño basado en el límite 0x7f
char* g_log_buffer_write_ptr = g_log_static_buffer;

/**
 * @brief Función principal de construcción de texto con formato (Printf / Sprintf del juego).
 * Empaqueta los argumentos variables del Stack y despacha el flujo hacia el wrapper del motor.
 * Dirección original en Ghidra: 0x00115CF0 (PAL)
 *
 * @param p_buffer_struct Estructura de control del búfer de texto destino (param_1)
 * @param p_format_str Cadena de formato base (ej. "Munición: %d/%d") (param_2)
 * @param ... Argumentos variables adicionales a formatear.
 * @return s32 Cantidad total de caracteres escritos.
 */
s32 game_sprintf(s32* p_buffer_struct, const char* p_format_str, ...) {
	s32 total_written = 0;
	va_list args;

	// Inicializa la lista de argumentos variables apuntando justo después de p_format_str
	// Esto reemplaza de forma portable el volcado masivo en el Stack (uStack_30) de la PS2
	va_start(args, p_format_str);

	// Despacha la estructura de control, el formato y los argumentos al wrapper intermedio
	total_written = txt_sprintf_wrapper(p_buffer_struct, p_format_str, args);

	// Libera la lista de argumentos dinámicos
	va_end(args);

	return total_written;
}

/**
 * @brief Función de interfaz para formatear cadenas de texto en un búfer estructurado.
 * Extrae el puntero de destino desde el offset 0x15 y despacha la petición al motor maestro.
 * Dirección original en Ghidra: 0x00119BC8 (PAL)
 *
 * @param p_buffer_struct Estructura de control del búfer de texto (param_1)
 * @param p_format_str Cadena de formato (ej. "Guitones: %d") (param_2)
 * @param args_list Lista de argumentos variables (param_3)
 * @return s32 Cantidad de caracteres formateados y escritos.
 */
s32 txt_sprintf_wrapper(s32* p_buffer_struct, const char* p_format_str, va_list args_list) {
	if (p_buffer_struct == NULL) {
		return 0;
	}

	// El offset 0x15 (indexado como int, equivalente a bytes 0x54) contiene el puntero destino real
	void* target_destination = (void*)((long)p_buffer_struct[0x15]);

	// Despacha la operación al motor de formateo que reconstruimos previamente
	return custom_vsprintf_engine(target_destination, (int*)p_buffer_struct, p_format_str, args_list);
}

/**
 * @brief Reconstrucción funcional del motor tipográfico e intérprete de tokens % de Insomniac Games.
 * Procesa formatos estándar (%d, %i, %x, %s, %c) y despacha ráfagas al sistema de logs o búferes de memoria.
 * Dirección original en Ghidra: 0x00119BF8 (PAL)
 */
s32 custom_vsprintf_engine(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list) {
	if (p_format_str == NULL) {
		return 0;
	}

	// Búfer local intermedio para emular de forma portable la construcción de caracteres
	char local_buffer[1024];

	// Delegamos la conversión de formatos de bajo nivel (como las divisiones consecutivas
	// entre 10 de math_div64 o los mapas de caracteres "0123456789abcdef") a vsnprintf nativo:
	s32 chars_written = vsnprintf(local_buffer, sizeof(local_buffer), p_format_str, args_list);

	if (chars_written > 0) {
		// En la PS2 original, si el bit 0x200 del estado está activo, escribe directo en memoria (ee_memcpy).
		// Si no, lo manda de forma fragmentada al buffer del HUD de la interfaz o logs.
		if (p_state_struct != NULL && (*p_state_struct & 0x200) != 0) {
			// Copia de ráfaga directa a la dirección de destino apuntada por output_dest
			char** p_dest_ptr = (char**)output_dest;
			ee_memcpy(*p_dest_ptr, local_buffer, chars_written);
			*p_dest_ptr += chars_written; // Avanza el puntero de escritura en la RAM
		}
		else {
			// Despacha los caracteres al búfer segmentado con auto-flush activo
			sys_log_write_buffered(1, local_buffer, chars_written, 0);
			sys_log_write_buffered(1, NULL, 0, 1); // Fuerza el vaciado de línea final (Flush)
		}
	}

	return chars_written;
}

/**
 * @brief Busca la primera aparición de un byte en un bloque de memoria (Memchr).
 * Versión portátil del algoritmo de ráfagas vectoriales de 16 bytes del Emotion Engine.
 * Dirección original en Ghidra: 0x00115250 (PAL)
 *
 * @param ptr Puntero al bloque de memoria inicial (param_1)
 * @param value Valor del byte buscado (param_2)
 * @param num Cantidad de bytes máximos a escanear (param_3)
 * @return void* Puntero a la posición del byte encontrado, o NULL si no existe.
 */
void* ee_memchr(const void* ptr, int value, u32 num) {
	if (ptr == NULL) {
		return NULL;
	}

	const unsigned char* p = (const unsigned char*)ptr;
	unsigned char target = (unsigned char)(value & 0xFF);

	// En la PS2 original, un bucle procesa bloques vectoriales de 16 bytes (0x10) 
	// usando máscaras en paralelo si la memoria está perfectamente alineada.
	// Lógicamente, el comportamiento equivalente es:
	for (u32 i = 0; i < num; i++) {
		if (p[i] == target) {
			return (void*)(p + i);
		}
	}

	return NULL;
}

/**
 * @brief Copia un bloque de memoria desde un origen a un destino (Memcpy).
 * Equivalente portátil a la rutina de copia por bloques de 32 bytes optimizada para la PS2.
 * Dirección original en Ghidra: 0x001153D4 (PAL)
 *
 * @param dest Puntero al bloque de memoria de destino (param_1)
 * @param src Puntero al bloque de memoria de origen (param_2)
 * @param size Cantidad de bytes a copiar (param_3)
 * @return void* Puntero al bloque de destino.
 */
void* ee_memcpy(void* dest, const void* src, u32 size) {
	if (dest == NULL || src == NULL) {
		return dest;
	}

	u8* d = (u8*)dest;
	const u8* s = (const u8*)src;

	// En la PS2 real, si la memoria está alineada, se ejecuta un bucle do-while
	// que vacía y llena los registros en ráfagas masivas de 32 bytes (0x20).
	// Funcionalmente, el comportamiento idéntico y seguro en C es:
	for (u32 i = 0; i < size; i++) {
		d[i] = s[i];
	}

	return dest;
}

/**
 * @brief Escribe texto de forma segmentada dentro de un búfer de acumulación de 128 bytes.
 * Realiza un vaciado automático (autoflush) al llenarse o de forma explícita mediante flags.
 * Dirección original en Ghidra: 0x00119AC0 (PAL)
 *
 * @param log_level Nivel de prioridad/canal (param_1)
 * @param p_srcString Texto a escribir (param_2)
 * @param write_len Cantidad de caracteres/bytes a procesar (param_3)
 * @param flush_flag Si es 1, fuerza el vaciado del búfer de forma inmediata (param_4)
 * @return u32 Cantidad de bytes procesados con éxito.
 */
u32 sys_log_write_buffered(s32 log_level, const char* p_srcString, u32 write_len, s32 flush_flag) {
	u32 bytes_processed = 0;

	// Control de vaciado explícito (Flush)
	if (flush_flag == 1) {
		sys_log_dispatch_message(log_level, g_log_static_buffer, g_log_buffer_count);
		g_log_buffer_count = 0;
		g_log_buffer_write_ptr = g_log_static_buffer;
	}
	else {
		const char* p_src = p_srcString;

		if (write_len != 0) {
			do {
				// Copia el carácter actual del juego al búfer estático
				*g_log_buffer_write_ptr = *p_src;
				g_log_buffer_count++;
				g_log_buffer_write_ptr++;

				// Autoflush: Vaciado automático si supera los 128 bytes (0x7f)
				if (g_log_buffer_count > 0x7F) {
					s32 dispatch_status = sys_log_dispatch_message(log_level, g_log_static_buffer, g_log_buffer_count);
					g_log_buffer_write_ptr = g_log_static_buffer;
					g_log_buffer_count = 0;

					if (dispatch_status == 0) {
						g_log_buffer_count = 0;
						return 0; // Detener flujo si el despachador falla
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
 * @brief Despacha y filtra los mensajes de diagnóstico del juego hacia el subsistema de logs.
 * Realiza una inicialización bajo demanda (Lazy Init) del sistema de red si no está activo.
 * Dirección original en Ghidra: 0x0011B1E8 (PAL)
 *
 * @param log_level Nivel de prioridad o canal del mensaje (param_1)
 * @param p_message Cadena de caracteres del mensaje (param_2)
 * @param message_len Longitud de la cadena de texto (param_3)
 * @return s32 Cantidad de caracteres impresos, o -1 si el canal es inválido o falla la red.
 */
s32 sys_log_dispatch_message(u32 log_level, const char* p_message, s32 message_len) {
	s32 result_status = -1;

	// Comprueba de forma segura si el nivel es 1 o 2 (equivalente a: log_level - 1U < 2)
	if (log_level == 1 || log_level == 2) {

		// Inicialización bajo demanda del sistema si es la primera vez que se usa
		if (g_deci2_is_initialized == 0) {
			bool init_success = sys_deci2_subsystem_init();
			if (!init_success) {
				return -1; // Aborta si el subsistema de hardware falla
			}
			g_deci2_is_initialized = 1;
		}

		// Envía el mensaje formateado al buffer de impresión por red
		result_status = sys_deci2_print_log(p_message, message_len);
	}

	return result_status;
}

/**
 * @brief Inicializa una lista enlazada global o cola de administración de memoria del motor.
 * Configura el nodo raíz, inicializa el contador a 0 y enlaza los punteros Head y Tail.
 * Dirección original en Ghidra: 0x0011BAC8 (PAL)
 *
 * @param param_1 Parámetro de configuración inicial o capacidad (registro a0)
 * @return void* Puntero a la cabecera de la estructura global inicializada.
 */
void* sys_queue_initialize(u32 param_1) {
	g_list_root_param = param_1;
	g_list_element_count = 0; // Inicializa el tamaño/contador actual en cero

	// Tanto el Head como el Tail apuntan inicialmente al nodo base vacío (DAT_0013cad0)
	g_list_head_ptr = &g_list_sentinel_node;
	g_list_tail_ptr = &g_list_sentinel_node;

	return &g_list_root_param;
}

// Variables de control de red del sistema
s32 g_deci2_channel_status = -1;
u32 g_deci2_var1 = 0;
u32 g_deci2_var2 = 0;
u32 g_deci2_var3 = 0;
void* g_deci2_buffer_a = NULL;
void* g_deci2_buffer_b = NULL;
void* g_deci2_queue_ptr = NULL;

// Variables ficticias del buffer de cabecera de red
u16 g_net_packet_size = 0;
u8  g_net_packet_id1 = 0;
u8  g_net_packet_id2 = 0;
u8  g_net_packet_flags = 0;
u16 g_net_packet_type = 0;

// Control de semáforo e impresión
s32 g_deci2_print_mutex = 0;
s32 g_deci2_packet_len = 0;
char g_deci2_print_buffer[256]; // Representa el espacio físico en DAT_2013cc0c

/**
 * @brief Transmite un mensaje de registro formateado al sistema de depuración remota.
 * Traduce caracteres de salto de línea e interactúa con los canales de red DECI2.
 * Dirección original en Ghidra: 0x0011BCD0 (PAL)
 *
 * @param p_message Cadena de caracteres a imprimir (param_1)
 * @param max_len Longitud máxima a procesar (param_2)
 * @return int Cantidad de caracteres transmitidos de forma exitosa, o -1 en caso de bloqueo/error.
 */
s32 sys_deci2_print_log(const char* p_message, s32 max_len) {
	s32 total_chars_written = 0;
	s32 status_code = -1;

	if (g_deci2_print_mutex == 0) {
		kernel_system_sync_guard();
		g_deci2_print_mutex = 1;
		status_code = 0;

		// Bucle de formateo y copia de caracteres (Máximo 256 bytes)
		while (total_chars_written < 256 && max_len > 0) {
			max_len--;
			char current_char = *p_message;

			// Conversión de salto de línea de Unix (\n) a formato de consola (\r)
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

		// Configura el tamaño del paquete sumando la cabecera base (12 bytes / 0xc)
		g_deci2_packet_len = total_chars_written + 0xC;

		// Simulación del envío de datos a través de las llamadas del sistema
		sys_deci2_call_wrapper();

		// En PC o plataformas modernas, redirigimos este log directamente a la consola nativa
#ifndef PLATFORM_PS2
		g_deci2_print_buffer[total_chars_written] = '\0'; // Asegurar cierre de string
		// printf("[PS2 LOG]: %s", g_deci2_print_buffer); // Descomentar para ver logs en el port
		g_deci2_print_mutex = 0;
#endif

		// El juego original espera activamente a que el hardware del canal C limpie el mutex
		// while (g_deci2_print_mutex != 0) { sys_deci2_call_channel_c(); }

		kernel_system_sync_release();
	}

	return status_code;
}

/**
 * @brief Inicializa el subsistema de comunicación de depuración DECI2 del motor.
 * Configura la cola de transmisión y escribe las cabeceras de protocolo en memoria.
 * Dirección original en Ghidra: 0x0011BE20 (PAL)
 *
 * @return bool Devuelve true si el canal de red se inicializó correctamente, false en caso contrario.
 */
bool sys_deci2_subsystem_init(void) {
	// 1. Limpieza de las cachés del procesador central
#ifdef PLATFORM_PS2
	FlushCache(0); // WRITEBACK_DCACHE
#endif

	// 2. Intenta abrir y verificar el estado del canal de depuración
	// Nota: sys_deci2_call_channel_a() en nuestro port portable devuelve un estado pasivo (ej. 0)
	g_deci2_channel_status = sys_deci2_call_channel_a();

	bool is_channel_valid = (-1 < g_deci2_channel_status);

	if (is_channel_valid) {
		// Inicializa variables de estado internas
		g_deci2_var1 = 0;
		g_deci2_var2 = 0;
		g_deci2_var3 = 0;

		// Asignación de los buffers globales del sistema de diagnóstico
		g_deci2_buffer_a = (void*)0x2013CD40;
		g_deci2_buffer_b = (void*)0x2013CC00;

		// Escribe los valores de la cabecera de protocolo de red del motor de Insomniac Games
		g_net_packet_size = 0x210; // Tamaño del paquete de red
		g_net_packet_id1 = 0x45;  // Identificador de protocolo ('E')
		g_net_packet_id2 = 0x48;  // Identificador de protocolo ('H')
		g_net_packet_flags = 0;
		g_net_packet_type = 0;

		// 3. Inicializa la cola dinámica de mensajes con capacidad para 256 elementos (0x100)
		g_deci2_queue_ptr = sys_queue_initialize(0x100);
	}

	return is_channel_valid;
}

/**
 * @brief Calcula la longitud de una cadena de caracteres (Strlen).
 * Equivalente portátil a la rutina optimizada bitwise del Emotion Engine.
 * Dirección original en Ghidra: 0x001157AC (PAL)
 *
 * @param str Puntero a la cadena de texto de entrada.
 * @return int El número de caracteres en la cadena (sin contar el nulo terminal).
 */
int ee_strlen(const char* str) {
	if (str == NULL) {
		return 0;
	}

	const char* s = str;

	// El motor de PS2 utiliza operaciones bitwise paralelas (0xfefefefefefefeff)
	// para escanear bloques de memoria de 8 y 16 bytes simultáneamente.
	// Conceptualmente, su comportamiento lógico exacto es avanzar hasta encontrar '\0':
	while (*s != '\0') {
		s++;
	}

	// Devuelve la diferencia de direcciones, es decir, la longitud exacta
	return (int)(s - str);
}

/**
 * @brief Compara dos cadenas de caracteres (Strcmp).
 * Equivalente portátil a la rutina optimizada bitwise del procesador Emotion Engine.
 * Dirección original en Ghidra: 0x00115544 (PAL)
 *
 * @param str1 Primera cadena a comparar (param_1)
 * @param str2 Segunda cadena a comparar (param_2)
 * @return int 0 si son idénticas, un valor negativo si str1 < str2, o positivo si str1 > str2.
 */
int ee_strcmp(const char* str1, const char* str2) {
	if (str1 == NULL || str2 == NULL) {
		return (str1 == str2) ? 0 : (str1 == NULL ? -1 : 1);
	}

	const unsigned char* s1 = (const unsigned char*)str1;
	const unsigned char* s2 = (const unsigned char*)str2;

	// En la PS2 real, si la memoria está alineada, se ejecutan restas multimedia en paralelo.
	// Conceptualmente, su comportamiento lógico equivale a iterar byte por byte:
	while (*s1 != '\0' && *s1 == *s2) {
		s1++;
		s2++;
	}

	// Devuelve la diferencia matemática exacta entre los caracteres donde difieren
	return (int)*s1 - (int)*s2;
}

/**
 * @brief Decodifica un carácter multi-byte del motor de texto según la codificación activa.
 * Soporta fallbacks de codificación JIS/SJIS heredados del desarrollo original.
 * Dirección original en Ghidra: 0x00115FC0 (PAL)
 *
 * @param p_localeContext Contexto de localización e idioma activo (param_1)
 * @param p_outChar Puntero donde se almacena el carácter decodificado final (param_2)
 * @param p_srcString Puntero a la cadena de texto a leer (param_3)
 * @param max_bytes Límite de bytes seguros a procesar (param_4)
 * @param p_state Estado de control de secuencia de escape (param_5)
 * @return int Cantidad de bytes leídos para el carácter, o -1 en caso de error.
 */
s32 txt_decode_multibyte_char(u8* p_localeContext, u32* p_outChar, const u8* p_srcString, u32 max_bytes, s32* p_state) {
	if (p_srcString == NULL) {
		return 0;
	}

	// Comprobación de seguridad básica del motor
	if (max_bytes == 0) {
		return -1; // Equivale al 0xffffffff retornado en PS2
	}

	// El motor original de PS2 realiza comparaciones con "C-SJIS", "C-EUCJP" y "C-JIS"
	// utilizando ee_strcmp para decidir si procesa caracteres de doble byte.
	// Para efectos funcionales en el port de la región europea (PAL):

	u8 lead_byte = *p_srcString;

	// Si no es un formato especial asiático, procesa como carácter estándar de 1 byte
	if (p_outChar != NULL) {
		*p_outChar = (u32)lead_byte;
	}

	return (lead_byte != 0) ? 1 : 0;
}

/**
 * @brief Envoltorio de paso para la llamada del sistema de depuración DECI2 de Sony.
 * Utilizada originalmente en los kits de desarrollo (PS2 TOOL) para enviar registros al PC del programador.
 * Dirección original en Ghidra: 0x0011B9C8 (PAL)
 */
void sys_deci2_call_wrapper(void) {
	// En las consolas comerciales y emuladores modernos, esta llamada no tiene efecto funcional.
	// Para el port nativo, se documenta y se deja como un retorno directo vacío (Stub).
	return;
}

/**
 * @brief Segundo envoltorio de paso para el canal de depuración DECI2 de Sony.
 * Dirección original en Ghidra: 0x0011B978 (PAL)
 */
s32 sys_deci2_call_channel_a(void) {
	// Retorno pasivo (Stub) para compatibilidad en el port nativo: 0 = canal disponible/sin error.
	return 0;
}

/**
 * @brief Tercer envoltorio de paso para el canal de depuración DECI2 de Sony.
 * Dirección original en Ghidra: 0x0011B9F8 (PAL)
 */
void sys_deci2_call_channel_c(void) {
	// Retorno directo pasivo (Stub) para compatibilidad estructural en el port nativo.
	return;
}
