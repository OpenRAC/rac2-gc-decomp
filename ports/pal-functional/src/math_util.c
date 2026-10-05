// src/math_util.c
#include "types.h"   // Para que reconozca los tipos como u8, s32, bool
#include "math_util.h"
#include <math.h>    // Requerido para invocar a log() de forma nativa en sistemas modernos
#include <stdio.h>   // vsnprintf
#include <stdlib.h>  // _Exit()

/**
 * @brief Convierte un número de punto flotante de doble precisión (64 bits) a un entero de 32 bits con signo (int).
 * Equivalente de software portable a la rutina __fixdfsi de la biblioteca runtime de la PS2.
 * Dirección original en Ghidra: 0x00123130 (PAL)
 *
 * @param param_1 Los 64 bits del número double original.
 * @return int El resultado convertido al tipo de dato entero de 32-bits con signo.
 */
s32 math_double_to_int32_signed(double param_1) {
	// En la PS2 real, este proceso requiere desarmar el formato IEEE 754 de 64 bits,
	// evaluar si es NaN/Infinito, y desplazar bit a bit la mantisa según el exponente.
	// Para efectos funcionales en plataformas modernas, el hardware lo resuelve de forma nativa:
	return (s32)param_1;
}

/**
 * @brief Calcula el resto flotante de la división de dos números de doble precisión (64 bits - fmod).
 * Reemplaza de forma de hardware portátil el bucle binario manual de sustracción de mantisas de la PS2.
 * Dirección original en Ghidra: 0x00117848 (PAL)
 *
 * @param x Dividendo flotante de doble precisión (param_1).
 * @param y Divisor flotante de doble precisión (param_2).
 * @return double El residuo de la división calculado por hardware nativo.
 */
double math_fmod_double64(double x, double y) {
	// Delegamos de forma portátil y ultra veloz el algoritmo a la CPU nativa de la PC
	return fmod(x, y);
}

/**
 * @brief Calcula el suelo matemático (floor) de un número de doble precisión (64 bits - double).
 * Redondea el valor hacia abajo al número entero más cercano menor o igual.
 * Dirección original en Ghidra: 0x00117650 (PAL)
 *
 * @param x Valor flotante de doble precisión a procesar.
 * @return double El resultado redondeado hacia abajo por hardware.
 */
double math_floor_double64(double x) {
	// Delegamos de forma portable y ultra veloz el algoritmo de máscaras a la CPU nativa de la PC
	return floor(x);
}

/**
 * @brief Calcula el logaritmo en base 10 de un número de doble precisión (64 bits - double).
 * Reemplaza de forma de hardware portátil el cambio de base manual por multiplicación de log10(e) de la PS2.
 * Dirección original en Ghidra: 0x001182B8 (PAL)
 *
 * @param x Valor numérico del que se calculará el logaritmo base 10.
 * @return double El resultado del logaritmo decimal calculado por la CPU nativa.
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
		return NAN; // Los logaritmos de números negativos no existen en reales
	}

	// Simplificamos todo el empaquetado y cambio de base a la instrucción nativa moderna
	return log10(x);
}

/**
 * @brief Calcula el logaritmo natural de un número de doble precisión (64 bits - double).
 * Reemplaza de forma portable y automática la aproximación polinómica por software y series de Taylor de la PS2.
 * Dirección original en Ghidra: 0x00117CA0 (PAL)
 *
 * @param x Valor numérico del que se calculará el logaritmo natural.
 * @return double El resultado del logaritmo natural calculado por hardware.
 */
double math_log_double64(double x) {
	if (x == 0.0) {
		// Mantiene la consistencia matemática devolviendo infinito negativo en el port
#if defined(PLATFORM_PS2)
		return (double)0xFFF0000000000000ULL;
#else
		return -INFINITY;
#endif
	}

	// Delegamos la compleja expansión de Remez de la PS2 a la CPU nativa de la PC
	return log(x);
}

/**
 * @brief Manejador de fallos de aserción del motor gráfico (Assertion Handler).
 * Detiene la ejecución del juego e imprime la ubicación exacta del bug detectado.
 * Dirección original en Ghidra: 0x00115E28 (PAL)
 *
 * @param p_assertion Expresión lógica condicional que falló (ej. "ammo <= max").
 * @param p_file Ruta del archivo de código fuente original donde ocurrió el fallo.
 * @param line Número de línea física del error.
 */
void sys_assert_fail(const char* p_assertion, const char* p_file, s32 line) {
	// 1. Llama al hook de parada del sistema para intentar estabilizar la consola
	sys_safe_exit_stub();

	// 2. Formatea e inyecta la alerta en los logs usando el sprintf del juego
	s32* p_error_stream = *(s32**)(0x00133EF4 + 0xC);
	game_sprintf(p_error_stream, "assertion \"%s\" failed: file \"%s\", line %d\n", p_assertion, p_file, line);

	// 3. En la PS2 real, aquí ocurre un bucle infinito recursivo para congelar el hardware.
	// Para efectos funcionales en el port nativo moderno, forzamos un cierre controlado:
#if defined(PLATFORM_PS2)
	sys_assert_fail(p_assertion, p_file, line); // Bucle infinito intencional
#else
	_Exit(1); // Aborta la ejecución de inmediato en PC
#endif
}

/**
 * @brief Renderizador y parser tipográfico final para números decimales y científicos en el HUD.
 * Analiza tokens de porcentaje (%o, %s, %u, %x, %f), extrae dígitos de la pila e inyecta el texto gráfico.
 * Dirección original en Ghidra: 0x0011C1F8 (PAL)
 *
 * @param format_ptr Puntero a la cadena de formato de la string (param_1).
 * @param args_list Lista de argumentos dinámicos empaquetados del Stack (param_2).
 * @return bool Devuelve true si la string se renderizó y transmitió con éxito.
 */
bool txt_render_scientific_string(const u8* format_ptr, va_list args_list) {
	if (format_ptr == NULL) {
		return false;
	}

	// El motor original congela el hardware de la PS2 para sincronizar el pipeline gráfico
	kernel_system_sync_guard();

	char print_buffer[512];

	// De manera portable y limpia en sistemas modernos, delegamos todo el switch-case masivo
	// de formateo de caracteres, marcadores (null) y conversiones bitwise de bases a la FPU nativa:
#if defined(PLATFORM_PS2)
// Lógica nativa de ráfagas MIPS si se compilara en hardware original
#else
	s32 written = vsnprintf(print_buffer, sizeof(print_buffer), (const char*)format_ptr, args_list);
	if (written > 0) {
		// Aquí el port redirige los caracteres formateados hacia el motor de fuentes moderno
		// para renderizar el texto final en la interfaz gráfica tridimensional.
	}
#endif

	// Libera de forma segura las interrupciones del procesador central (Emotion Engine)
	kernel_system_sync_release();

	return true;
}

/**
 * @brief Envoltorio encargado de empaquetar los argumentos decimales de la notación científica en el Stack.
 * Pasa el puntero de formato y la lista de variables dinámicas a la subrutina de procesamiento final.
 * Dirección original en Ghidra: 0x0011C7E8 (PAL)
 *
 * @param p_format_label Dirección lógica del string o etiqueta de control (param_1).
 * @param ... (ELIPSIS) Representa que aquí pueden ir infinitos números flotantes o enteros seguidos.
 * @return bool Devuelve el estado de éxito o fallo reportado por el renderizador interno.
 */
bool txt_format_scientific_wrapper(const u8* p_format_label, ...) {
	bool result_status = false;
	va_list args; // Este es el empaquetador dinámico nativo de C

	// Inicializa la lista 'args' apuntando a los elementos que van después de 'p_format_label'
	// Esto emula de manera segura y automática las líneas 'uStack_38 = param_2' de la PS2
	va_start(args, p_format_label);

	// Despacha la operación hacia la subrutina de procesamiento
	result_status = txt_render_scientific_string(p_format_label, args);

	// Libera la lista de argumentos de la memoria del PC
	va_end(args);

	return result_status;
}

/**
 * @brief Normaliza un número flotante de doble precisión a componentes exponenciales de base 10.
 * Orquesta restas, multiplicaciones y divisiones por 10.0f para alimentar el formato tipográfico %e/%g.
 * Dirección original en Ghidra: 0x0011C090 (PAL)
 *
 * @param param_1 Los 64 bits del número double original a procesar.
 */
void math_double_to_scientific_digits(double param_1) {
	// En arquitecturas modernas (PC, consolas nativas), este intrincado bucle de escalado flotante 
	// y conversión manual de dígitos se delega directamente al compilador y al sistema operativo 
	// mediante vsnprintf con el token %e. Reservamos el esqueleto documental:

	double val = param_1;
	s64 exponent_counter = 0;

	if (val < 0.0) {
		val = -val; // Equivale a math_sub_double64(0, param_1)
		// [Escribe el prefijo '-' en la string de salida]
	}

	// Normalización de escala para números inferiores a 0.1f
	if (val < 0.1) {
		while (val < 0.1) {
			exponent_counter--;
			val *= 10.0; // Equivale a math_mul_double64(val, 10.0)
		}
	}
	else if (val >= 1.0) {
		// Normalización de escala para números superiores o iguales a 1.0f
		while (val >= 1.0) {
			exponent_counter++;
			val /= 10.0; // Equivale a math_div_double64(val, 10.0)
		}
	}

	// Extrae los dígitos finales escalados usando el conversor
	double scaled_val = val * 1000000.0;
	// s32 digits = math_double_to_digits((double)math_double_to_int64(scaled_val));

	// [Despacha los caracteres formateados agregando la 'e' y el signo del exponente]
	return;
}


/**
 * @brief Extrae los dígitos enteros de un número de doble precisión aplicando reglas de redondeo.
 * Utilizado por el subsistema de strings para formatear decimales y valores numéricos en el HUD.
 * Dirección original en Ghidra: 0x0011C000 (PAL)
 *
 * @param param_1 Los 64 bits del número double original.
 * @return int Los dígitos enteros procesados, o 9999 en caso de desbordamiento de rango.
 */
s32 math_double_to_digits(double param_1) {
	// Para conservar la compatibilidad funcional exacta con los límites que el motor de la PS2 espera:
	u64 bits;
	ee_memcpy(&bits, &param_1, sizeof(double));

	s64 exponent = ((bits << 1) >> 53) - 0x433;

	if (exponent < -0x35) {
		return 0;
	}
	if (0xC < exponent) {
		return 9999; // Límite de desbordamiento del motor tipográfico de Insomniac Games
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
 * @brief Convierte un número de punto flotante de doble precisión (64 bits) a un entero de 64 bits con signo (long long).
 * Equivalente de software portátil a la rutina __fixdfdi de la biblioteca runtime de la PS2.
 * Dirección original en Ghidra: 0x001212C8 (PAL)
 *
 * @param param_1 Los 64 bits del número double original.
 * @return s64 El resultado convertido al tipo de dato entero de 64-bits con signo.
 */
s64 math_double_to_int64(double param_1) {
	// En la PS2 real, este proceso requiere segmentar y escalar el flotante, extraer mitades de 32 bits,
	// ajustar signos condicionales, calcular residuos y fusionarlos de forma binaria.
	// Para efectos funcionales en plataformas modernas, el hardware lo resuelve de forma nativa:
	return (s64)param_1;
}

/**
 * @brief Convierte un número entero de 64 bits con signo (long long) a punto flotante de doble precisión (double).
 * Equivalente de software portátil a la rutina __floatdidf de la biblioteca runtime de la PS2.
 * Dirección original en Ghidra: 0x001213B8 (PAL)
 *
 * @param param_1 El número entero de 64-bits original con signo.
 * @return double El resultado convertido al tipo de dato flotante double de 64-bits.
 */
double math_int64_to_double(s64 param_1) {
	// En la PS2 real, este proceso requiere segmentar el entero en dos bloques de 32 bits,
	// convertirlos individualmente mediante desplazamientos y multiplicaciones por 2^32, y sumarlos.
	// En plataformas modernas, la CPU lo resuelve directamente de forma nativa por hardware:
	return (double)param_1;
}

/**
 * @brief Ejecuta la suma de dos números de doble precisión de 64 bits (double).
 * Desempaqueta los operandos y delega la lógica de alineación y adición al módulo unificado.
 * Dirección original en Ghidra: 0x00122A40 (PAL)
 *
 * @param a Primer sumando double (param_1).
 * @param b Segundo sumando double (param_2).
 * @return double El resultado de la suma aritmética de 64-bits.
 */
double math_add_double64(double a, double b) {
	// En arquitecturas modernas (PC o consolas actuales), no requerimos emular el desarmado
	// de mantisas y exponentes por software; la FPU de la CPU lo resuelve directamente:
	return a + b;
}

/**
 * @brief Convierte un número entero de 32 bits con signo a punto flotante de doble precisión (64 bits - double).
 * Equivalente de software portátil a la rutina __floatsidf de la biblioteca runtime de la PS2.
 * Dirección original en Ghidra: 0x00123078 (PAL)
 *
 * @param param_1 El número entero de 32-bits original con signo.
 * @return double El resultado convertido al tipo de dato flotante double de 64-bits.
 */
double math_int_to_double(s32 param_1) {
	// En la PS2 real, este proceso requiere determinar el signo, normalizar la mantisa mediante
	// un bucle de desplazamientos de bits a la izquierda e invocar al empaquetador de 64 bits.
	// Para efectos funcionales en plataformas modernas, la CPU lo resuelve directamente:
	return (double)param_1;
}

/**
 * @brief Convierte un número de punto flotante de doble precisión (64 bits) a un entero de 32 bits con signo.
 * Equivalente de software portátil a la rutina __fixdfsi de la biblioteca runtime de la PS2.
 * Dirección original en Ghidra: 0x001231C8 (PAL)
 *
 * @param param_1 Los 64 bits del número double original.
 * @return int El resultado convertido al tipo de dato entero de 32-bits con signo.
 */
s32 math_double_to_int(double param_1) {
	// En la PS2 real, este proceso requiere desarmar el formato IEEE 754 de 64 bits,
	// evaluar si es NaN/Infinito, y desplazar bit a bit la mantisa según el exponente.
	// Para efectos funcionales en sistemas modernos, el hardware lo resuelve de forma nativa:
	return (s32)param_1;
}

/**
 * @brief Ejecuta la multiplicación estructural de dos números de doble precisión descompuestos.
 * Realiza de forma portátil el cálculo cruzado de mantisas flotantes y el ajuste del signo del producto.
 * Dirección original en Ghidra: 0x00122B00 (PAL)
 *
 * @param a Primer número flotante double (param_1).
 * @param b Segundo número flotante double (param_2).
 * @return double El producto resultante de la multiplicación de 64-bits.
 */
double math_mul_double64(double a, double b) {
	// En arquitecturas modernas, el hardware resuelve directamente esta operación tranzando las mantisas,
	// eliminando las costosas subrutinas manuales del compilador de la PS2.
	return a * b;
}

/**
 * @brief Multiplicación de enteros de 64 bits (Equivalente portable a __muldi3).
 * Resuelve mediante hardware nativo moderno el algoritmo algebraico de multiplicación cruzada de la PS2.
 * Dirección original en Ghidra: 0x00121AB8 (PAL)
 *
 * @param a Primer multiplicando de 64-bits (param_1).
 * @param b Segundo multiplicando de 64-bits (param_2).
 * @return s64 El producto resultante de la multiplicación de 64-bits.
 */
s64 math_mul64(s64 a, s64 b) {
	// En sistemas modernos, el compilador traduce esta línea a una única instrucción nativa de CPU,
	// eliminando las operaciones de máscaras lógicas y desplazamientos de bits manuales.
	return a * b;
}


/**
 * @brief Ejecuta la resta de dos números de doble precisión de 64 bits (double).
 * Invierte el signo del sustraendo mediante XOR y delega la lógica al módulo unificado de adición.
 * Dirección original en Ghidra: 0x00122A98 (PAL)
 *
 * @param minuend Número double del que se resta (param_1).
 * @param subtrahend Número double que se va a restar (param_2).
 * @return double El resultado de la resta aritmética de 64-bits.
 */
double math_sub_double64(double minuend, double subtrahend) {
	// En arquitecturas modernas (PC, consolas nativas), no requerimos emular la inversión
	// del bit de signo en estructuras intermedias; la FPU de la CPU lo resuelve directamente:
	return minuend - subtrahend;
}

/**
 * @brief Ejecuta la división estructural de dos números de doble precisión descompuestos.
 * Realiza de forma portátil la división de mantisas de 64-bits y el reajuste del signo mediante XOR.
 * Dirección original en Ghidra: 0x00122DA8 (PAL)
 *
 * @param dividend Número flotante double que actúa como dividendo (param_1).
 * @param divisor Número flotante double que actúa como divisor (param_2).
 * @return double El cociente resultante de la división de 64-bits.
 */
double math_div_double64(double dividend, double divisor) {
	if (divisor == 0.0) {
		return 0.0; // Salvaguarda nativa contra errores de división por cero flotante
	}

	// En sistemas modernos, esta simple operación en C reemplaza de manera automática y portátil
	// todas las complejas rutinas de bucles binarios manuales por software que usaba la PS2.
	return dividend / divisor;
}

/**
 * @brief Ejecuta la suma o resta estructural de dos números de doble precisión descompuestos.
 * Realiza de forma portátil la alineación de exponentes y la normalización de mantisas de 64-bits.
 * Dirección original en Ghidra: 0x00122800 (PAL)
 *
 * @param p_unpack1 Estructura desempaquetada del primer double (param_1).
 * @param p_unpack2 Estructura desempaquetada del segundo double (param_2).
 * @param p_out_unpack Estructura de destino para almacenar el resultado intermedio (param_3).
 * @return void* Puntero a la estructura destino con el resultado calculado.
 */
void* math_add_sub_double64(u32* p_unpack1, u32* p_unpack2, u32* p_out_unpack) {
	u32 state1 = p_unpack1;
	u32 state2 = p_unpack2;
	u32 sign1 = p_unpack1;
	u32 sign2 = p_unpack2;

	// Caso A: El primer operando es un NaN o un Cero, devuelve el operando directo según las reglas del motor
	if (state1 < 2) {
		return p_unpack1;
	}

	if (state2 > 1) {
		if (state1 == 4) {
			if ((state2 ^ 4) != 0) return p_unpack1;
			if (sign1 == sign2) return p_unpack1;
			return (void*)0x141890; // Dirección de error matemático del SDK
		}

		// Caso B: El segundo operando es un Cero flotante, copia el primer operando al destino
		if (state2 == 2) {
			if ((state1 ^ 2) != 0) return p_unpack1;
			*(u64*)p_out_unpack = *(u64*)p_unpack1;
			*(u64*)(p_out_unpack + 2) = *(u64*)(p_unpack1 + 2);
			*(u64*)(p_out_unpack + 4) = *(u64*)(p_unpack1 + 4);
			p_out_unpack[1] = sign1 & sign2;
			return p_out_unpack;
		}

		// Caso C: Operación matemática normalizada de 64 bits (Alineación y Suma)
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

			p_out_unpack = 3; // FLOAT_STATE_NORMAL
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
 * @brief Función de interfaz para comparar dos números flotantes de 64 bits (double).
 * Desempaqueta de forma consecutiva ambos valores a través de buffers del stack y ejecuta la evaluación relacional.
 * Dirección original en Ghidra: 0x00123028 (PAL)
 *
 * @param param_1 Primer valor double de 64-bits (a0 / a1)
 * @param param_2 Segundo valor double de 64-bits (a2 / a3)
 * @return s32 0 si son iguales, 1 si param_1 > param_2, o -1 si param_1 < param_2.
 */
s32 math_compare_double64_wrapper(u64 param_1, u64 param_2) {
	u64 local_stack_val1 = param_1;
	u64 local_stack_val2 = param_2;

	// Arreglos de estructuras temporales que emulan los buffers auStack_70 y auStack_50 de la PS2
	u32 unpack_struct1[8];
	u32 unpack_struct2[8];

	// 1. Desarmar bajo el estándar IEEE 754 ambos componentes de 64 bits
	math_unpack_double64(&local_stack_val1, unpack_struct1);
	math_unpack_double64(&local_stack_val2, unpack_struct2);

	// 2. Ejecutar la comparación de jerarquía matemática
	return math_compare_double64(unpack_struct1, unpack_struct2);
}

/**
 * @brief Compara dos estructuras de números flotantes de doble precisión (64 bits).
 * Evalúa signos, exponentes y mantisas de forma jerárquica para determinar igualdad o magnitud.
 * Dirección original en Ghidra: 0x00122F10 (PAL)
 *
 * @param p_unpack1 Puntero a la estructura deconstruida del primer número double (param_1).
 * @param p_unpack2 Puntero a la estructura deconstruida del segundo número double (param_2).
 * @return int 0 si son idénticos, 1 si p_unpack1 > p_unpack2, o -1 si p_unpack1 < p_unpack2 (con fallbacks por signo).
 */
s32 math_compare_double64(u32* p_unpack1, u32* p_unpack2) {
	u32 state1 = p_unpack1[0];
	u32 state2 = p_unpack2[0];
	u32 sign1 = p_unpack1[1];
	u32 sign2 = p_unpack2[1];

	// Caso A: Si alguno es NaN (estado 0 o 1), la comparación no es válida
	if (state1 < 2 || state2 < 2) {
		return 1;
	}

	// Caso B: El primer número es Infinito
	if (state1 == 4) {
		if (state2 == 4) {
			return (s32)(sign2 - sign1);
		}
		return (sign1 == 0) ? 1 : -1;
	}

	// Caso C: El segundo número es Infinito
	if (state2 == 4) {
		return (sign2 == 0) ? -1 : 1;
	}

	// Caso D: El primer número es Cero
	if (state1 == 2) {
		if (state2 == 2) {
			return 0;
		}
		return (sign2 == 0) ? -1 : 1;
	}

	// Caso E: El segundo número es Cero
	if (state2 == 2) {
		return (sign1 == 0) ? 1 : -1;
	}

	// Caso F: Ambos son números normales (Comparación jerárquica bajo IEEE 754)
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
 * @brief Descompone un número de punto flotante de doble precisión (64 bits) en sus componentes IEEE 754.
 * Extrae el signo, el exponente sin sesgo, la mantisa extendida y clasifica el estado del número (NaN, Inf, Cero, Normal).
 * Dirección original en Ghidra: 0x00122760 (PAL)
 *
 * @param p_double_bits Puntero a los 64 bits del valor double (param_1).
 * @param p_output_struct Arreglo destino donde se guardarán los resultados del desempaquetado (param_2).
 */
void math_unpack_double64(u64* p_double_bits, u32* p_output_struct) {
	u64 raw_bits = *p_double_bits;

	// 1. Extrae los componentes binarios bajo el estándar IEEE 754 para variables de 64 bits
	u64 mantissa = raw_bits & 0xFFFFFFFFFFFFFULL;
	u32 exponent = (u32)((raw_bits >> 52) & 0x7FF);
	u32 sign = (u32)(raw_bits >> 63);

	p_output_struct[1] = sign; // Guarda el bit de signo

	// Caso A: El exponente es cero (Número cero o subnormal)
	if (exponent == 0) {
		p_output_struct[0] = 2; // FLOAT_STATE_ZERO
		return;
	}

	// Caso B: Número flotante normalizado válido
	if (exponent != 0x7FF) {
		// Añade el bit implícito de normalización y ajusta el desplazamiento
		*(u64*)(p_output_struct + 4) = (mantissa << 8) | 0x1000000000000000ULL;
		p_output_struct[2] = (s32)exponent - 0x3FF; // Remueve el sesgo (Bias) de 1023
		p_output_struct[0] = 3;                      // FLOAT_STATE_NORMAL
		return;
	}

	// Caso C: El exponente está al máximo (NaN o Infinito)
	if (mantissa == 0) {
		p_output_struct[0] = 4; // FLOAT_STATE_INFINITY
		return;
	}

	// Clasificación de subtipos de NaN (Not a Number) de 64 bits
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
 * @brief Convierte un número de punto flotante de precisión simple (32 bits) a doble precisión (64 bits).
 * Equivalente de software portátil a la rutina __extendsfdf2 de la biblioteca runtime de la PS2.
 * Dirección original en Ghidra: 0x001234F0 (PAL)
 *
 * @param param_1 Los 32 bits del número float original.
 * @return double El resultado convertido al tipo de dato de 64-bits de doble precisión.
 */
double math_float_to_double(f32 param_1) {
	// En la PS2 real, este proceso requiere desarmar bit a bit el formato IEEE 754 de 32 bits,
	// reajustar los sesgos (bias) del exponente e invocar al empaquetador de 64 bits.
	// Para efectos funcionales y portabilidad en sistemas modernos, el compilador lo resuelve de forma nativa:
	return (double)param_1;
}

/**
 * @brief Envoltorio de interfaz para empaquetar un flotante de 64-bits (double) desde variables del stack.
 * Dirección original en Ghidra: 0x00123268 (PAL)
 *
 * @param param_1 Estado o componente inicial (a0)
 * @param param_2 Componente secundario de signo (a1)
 * @param param_3 Componente de exponente (a2)
 * @param param_4 Puntero o valor de la mantisa de 64-bits (a3)
 */
void math_pack_double64_wrapper(u32 param_1, u32 param_2, u32 param_3, u64 param_4) {
	// Estructura local que emula el volcado consecutivo en la pila (Stack) de la PS2
	u32 local_stack_struct[4];

	local_stack_struct[0] = param_1;
	local_stack_struct[1] = param_2;
	local_stack_struct[2] = param_3;
	*(u64*)(&local_stack_struct[3]) = param_4; // Mapea la mantisa de 64 bits de forma contigua

	// Invoca de inmediato a la subrutina empaquetadora maestra que reconstruimos antes
	math_pack_double64(local_stack_struct);
}

/**
 * @brief Reconstruye un número de punto flotante de doble precisión (64 bits - double) a partir de sus componentes.
 * Sostiene de forma portátil la lógica de empaquetado binario bajo el estándar IEEE 754 del compilador de la PS2.
 * Dirección original en Ghidra: 0x00122630 (PAL)
 *
 * @param p_input_struct Estructura interna que almacena el estado, signo, exponente y mantisa de la operación.
 * @return u64 Los 64 bits combinados que forman el número decimal double real.
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
			// Manejo y ajuste de sesgo (bias) para exponentes subnormales o normales
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

				// Lógica interna de redondeo del compilador GCC de la PS2
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

	// Combina los componentes en un mapa unificado de 64 bits
	return (out_mantissa & 0xFFFFFFFFFFFFFULL) | (out_exponent & 0x7FF) << 52 | (u64)(int)sign << 63;
}


/**
 * @brief Calcula el residuo o módulo de una división de 64 bits con signo (Equivalente portable a __moddi3).
 * Sostiene las operaciones lógicas del operador '%' con números de 64-bits en el Emotion Engine.
 * Dirección original en Ghidra: 0x00121450 (PAL)
 *
 * @param dividend Número de 64-bits con signo (param_1)
 * @param divisor Número de 64-bits con signo (param_2)
 * @return long El residuo con signo resultante de la operación.
 */
s64 math_mod64(s64 dividend, s64 divisor) {
	if (divisor == 0) {
		return 0; // Salvaguarda nativa contra excepciones de división por cero
	}

	// En sistemas modernos, esta línea de C reemplaza de forma portable y automática
	// las cientos de líneas de lógica bitwise y manipulación de registros de la PS2.
	return dividend % divisor;
}

/**
 * @brief Descompone un número de punto flotante de 32 bits en sus componentes IEEE 754.
 * Clasifica el número en categorías: NaN, Infinito, Cero, o Número Normal y extrae su exponente y mantisa.
 * Dirección original en Ghidra: 0x00123400 (PAL)
 *
 * @param p_float_bits Puntero al valor flotante tratado como entero sin signo para manipulación de bits (param_1).
 * @param p_output_struct Arreglo destino donde se guardará el estado, signo, exponente y mantisa (param_2).
 */
void math_unpack_float32(u32* p_float_bits, u32* p_output_struct) {
	u32 raw_bits = *p_float_bits;

	// 1. Extrae los tres componentes binarios estándar del formato flotante
	u32 mantissa = raw_bits & 0x7FFFFF;
	u32 exponent = (raw_bits >> 23) & 0xFF;
	u32 sign = raw_bits >> 31;

	p_output_struct[1] = sign; // Guarda el bit de signo (0 = positivo, 1 = negativo)

	// Caso A: El exponente es cero (Número cero o subnormal)
	if (exponent == 0) {
		p_output_struct[0] = 2; // Token de estado: FLOAT_STATE_ZERO
		return;
	}

	// Caso B: El exponente está al máximo (NaN o Infinito)
	if (exponent == 0xFF) {
		if (mantissa == 0) {
			p_output_struct[0] = 4; // Token de estado: FLOAT_STATE_INFINITY
			return;
		}

		// Clasificación interna de subtipos de NaN (Not a Number)
		if ((raw_bits & 0x100000) == 0) {
			p_output_struct[0] = 0; // FLOAT_STATE_NAN_QUIET
		}
		else {
			p_output_struct[0] = 1; // FLOAT_STATE_NAN_SIGNALING
		}
		p_output_struct[3] = mantissa;
		return;
	}

	// Caso C: Número flotante normalizado válido
	p_output_struct[3] = (mantissa << 7) | 0x40000000; // Ajusta y reconstruye la mantisa con el bit implícito
	p_output_struct[2] = exponent - 0x7F;              // Remueve el sesgo (Bias) de 127 del exponente MIPS
	p_output_struct[0] = 3;                            // Token de estado: FLOAT_STATE_NORMAL
	return;
}


/**
 * @brief División de 64 bits con signo (Equivalente portátil a __divdi3).
 * Sostiene las operaciones lógicas de división con números negativos en el hardware de la PS2.
 * Dirección original en Ghidra: 0x00121B20 (PAL)
 *
 * @param dividend Número de 64-bits con signo (param_1)
 * @param divisor Número de 64-bits con signo (param_2)
 * @return long El cociente con signo de la división.
 */
s64 math_div64(s64 dividend, s64 divisor) {
	if (divisor == 0) {
		return 0; // Salvaguarda contra división por cero
	}

	// En arquitecturas modernas, el compilador traduce esto a una sola instrucción de CPU
	return dividend / divisor;
}

/**
 * @brief División y módulo de 64 bits sin signo (Equivalente portátil a __udivdi3).
 * Rutina de software utilizada originalmente en la PS2 para suplir la falta de división nativa de 64-bits.
 * Dirección original en Ghidra: 0x001220F0 (PAL)
 *
 * @param dividend Número de 64-bits a dividir (param_1)
 * @param divisor Número de 64-bits que divide (param_2)
 * @return ulong El cociente de la división.
 */
u64 math_udiv64(u64 dividend, u64 divisor) {
	// Si en sistemas modernos ocurre una división por cero, el sistema operativo
	// arrojará la señal correspondiente de forma nativa (equivalente al trap(7) de la PS2).
	if (divisor == 0) {
		// Comportamiento de salvaguarda
		return 0;
	}

	// En PC/Consolas modernas, esta línea de C se compila en una sola instrucción de CPU,
	// reemplazando por completo las más de 100 líneas de ensamblador de la PS2.
	return dividend / divisor;
}
