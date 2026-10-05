// src/hud_ammo.c
#include "hud_ammo.h"
#include <math.h> // Requerido para invocar a sinf() de forma nativa

// Dirección física de la tabla global de estado de armas en la RAM de la PS2
#define INVENTORY_WEAPONS_DATA_PTR     ((const u8*)0x0019B2F8)
#define MAX_WEAPONS_LIMIT_CONFIG       28 // 0x1B + 1 posiciones de ranuras base

// Dirección global de la memoria RAM de la PS2 que almacena el ID del arma activa
#define GLOBAL_ACTIVE_WEAPON_ID_PTR    ((const u8*)0x001396C8)

// Definición de recursos estáticos del Canvas Principal
#define RECURSO_HUD_CANVAS          ((const char*)0x001AE6D8) // "HudBase"
#define RECURSO_HEALTH_OUTLINE      ((const char*)0x001AE6E8) // "HealthBarOutline"
#define RECURSO_HEALTH_FILL         ((const char*)0x001AE6F8) // "HealthBarFill"

// Definiciones del set extendido de recursos estáticos del HUD
#define RECURSO_WEAPON_NAME         ((const char*)0x001AE700) // "WeaponName"
#define RECURSO_WEAPON_XP           ((const char*)0x001AE710) // "WeaXP"
#define RECURSO_AMMO_ICON           ((const char*)0x001AE720) // "AmmoIcon"
#define RECURSO_AMMO_ICON_BACK      ((const char*)0x001AE730) // "AmmoIconBack"
#define RECURSO_BOLT_TEXT           ((const char*)0x001AE740) // "BoltText"
#define RECURSO_BOLT_ICON           ((const char*)0x001AE750) // "BoltIcon"
#define RECURSO_QSEL_BACK         ((const char*)0x001AE058) // "QSEL_BACK"
#define RECURSO_QSEL_BORD         ((const char*)0x001AE068) // "QSelBI%d"
#define FORMATO_SLOT_RADIAL         ((const char*)0x001AE088) // "QSelBI%d"
#define FORMATO_ICONO_RADIAL    ((const char*)0x001AE098) // "QSelIco%d"

/**
 * @brief Recupera el identificador único (ID) del arma que el jugador tiene equipada actualmente en tiempo real.
 * Utilizada por el subsistema de munición y renderizado del HUD para sincronizar los contadores visuales.
 * Dirección original en Ghidra: 0x002B18D8 (PAL)
 *
 * @return u8 ID numérico del arma activa (ej. 0 = Llave, 1 = Lancer, etc.).
 */
u8 inv_get_active_weapon_id(void) {
	// Retorna de forma directa el byte de estado global de la RAM
	return *GLOBAL_ACTIVE_WEAPON_ID_PTR;
}

/**
 * @brief Calcula el desplazamiento espacial o ranuras restantes en el menú radial a partir del arma equipada.
 * Utiliza el conteo global y el ID activo para coordinar los límites de rotación de la interfaz Quick Select.
 * Dirección original en Ghidra: 0x002B18E8 (PAL)
 *
 * @return s32 Distancia o ranuras restantes indexadas (Clamped entre 0 y 40).
 */
s32 inv_get_quick_select_remaining_space(void) {
	// 1. Recupera el total de armas desbloqueadas y el ID del armamento en mano
	s32 total_weapons = inv_count_unlocked_weapons();
	u8 active_weapon_id = inv_get_active_weapon_id();

	// 2. Calcula la distancia diferencial en el anillo de selección
	s32 remaining_slots = total_weapons - (s32)active_weapon_id;

	// Regla de salvaguarda contra desbordamientos negativos
	if (remaining_slots < 0) {
		remaining_slots = 0;
	}

	// Aplica el clamp estándar de 40 posiciones (0x28) del HUD de Insomniac
	s32 clamped_offset = 0x28;
	if (remaining_slots < 0x29) {
		clamped_offset = remaining_slots;
	}

	return clamped_offset;
}

/**
 * @brief Cuenta la cantidad total de armas válidas y desbloqueadas actualmente en el inventario del jugador.
 * Peina la matriz de datos globales aplicando límites de control para definir el tamaño de la interfaz del HUD.
 * Dirección original en Ghidra: 0x002B1930 (PAL)
 *
 * @return s32 Cantidad final de ranuras de armas activas listas para renderizarse (Clamped entre 0 y 40).
 */
s32 inv_count_unlocked_weapons(void) {
	s32 total_active_elements = 0;
	s32 memory_offset = 0;

	// Bucle general que peina secuencialmente las 28 ranuras de armas del motor de Insomniac
	for (s32 weapon_idx = 1; weapon_idx <= MAX_WEAPONS_LIMIT_CONFIG; weapon_idx++) {
		const u8* p_weapon_bytes = INVENTORY_WEAPONS_DATA_PTR + memory_offset;

		// Cada ranura de arma almacena un bloque contiguo de 4 bytes con flags de estado
		for (s32 byte_idx = 0; byte_idx < 4; byte_idx++) {
			u8 flag_byte = p_weapon_bytes[byte_idx];

			// Si el flag contiene datos válidos, se contabiliza como un componente activo
			if (flag_byte != 0) {
				total_active_elements++;
			}
		}

		// Calcula el paso de alineación de memoria indexada para la siguiente ranura (weapon_idx * 4)
		memory_offset = weapon_idx * 4;
	}

	// Regla de salvaguarda: El conteo no puede ser menor a cero absoluto
	if (total_active_elements < 0) {
		total_active_elements = 0;
	}

	// Aplica un clamp matemático estricto: El HUD comercial de la PS2 soporta hasta 40 ranuras gráficas (0x28)
	s32 clamped_count = 0x28;
	if (total_active_elements < 0x29) {
		clamped_count = total_active_elements;
	}

	return clamped_count;
}

/**
 * @brief Modifica la posición física de anclaje de los componentes del HUD según el modo de video (4:3 o 16:9/PAL).
 * Inyecta las coordenadas de píxeles empaquetadas correspondientes para corregir la distorsión de la pantalla.
 * Dirección original en Ghidra: 0x0034EC58 (PAL)
 *
 * @param p_hud_main_struct Dirección base de la estructura central de la interfaz (param_1).
 * @param video_mode ID del modo de video activo (0 para NTSC/4:3, 1 para PAL/16:9) (param_2).
 */
void hud_update_layout_aspect_ratio(u32* p_hud_main_struct, s32 video_mode) {
	if (p_hud_main_struct == NULL) {
		return;
	}

	// El offset 0x15A4 equivale al índice 1385 en enteros de 32 bits (1385 * 4 = 5540 bytes)
	u8* p_base = (u8*)p_hud_main_struct;
	*(s32*)(p_base + 0x15A4) = video_mode;

	u32* p_sub_widget = (u32*)(p_base + 0x2A0);

	// Caso A: Modo Estándar NTSC / 4:3 (Valores extraídos de la máscara 0x7567)
	if (video_mode == 0) {
		s16 x_pos = 103; // 0x67 en hexadecimal (píxeles horizontales)
		s16 y_pos = 117; // 0x75 en hexadecimal (píxeles verticales)
		hud_set_widget_position_2d(p_sub_widget, (s32)x_pos, (s32)y_pos);
	}
	// Caso B: Modo Panorámico 16:9 / PAL (Valores extraídos de la máscara 0xEAA2)
	else if (video_mode == 1) {
		s16 x_pos = 162; // 0xA2 en hexadecimal
		s16 y_pos = 234; // 0xEA en hexadecimal
		hud_set_widget_position_2d(p_sub_widget, (s32)x_pos, (s32)y_pos);
	}
}

/**
 * @brief Configura el multiplicador de capacidad o modificador base del subsistema de munición en el offset 0x18.
 * Dirección original en Ghidra: 0x0034BCE0 (PAL)
 *
 * @param multiplier_val Valor de control o dirección de escala a inyectar (param_1).
 * @param p_extended_ammo_struct Dirección de memoria base de la subestructura de munición (param_2).
 */
void inv_set_ammo_capacity_multiplier(u32 multiplier_val, u32* p_extended_ammo_struct) {
	if (p_extended_ammo_struct != NULL) {
		// El offset 0x18 equivale al índice 6 en un arreglo de enteros de 32 bits (6 * 4 = 24 bytes)
		// Nota: Ghidra invirtió el orden de los argumentos en el descompilador original (param_1 es el valor, param_2 es el puntero)
		p_extended_ammo_struct[0x06] = multiplier_val;
	}
}

/**
 * @brief Recupera el puntero al vector principal de transformación (Posición) de un widget del HUD.
 * Lee directamente la dirección física almacenada en el offset +0 de la estructura.
 * Dirección original en Ghidra: 0x00337AF0 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @return f32* Puntero al vector de posición (X, Y, Z, W) del widget, o NULL si no está asignado.
 */
f32* hud_get_widget_position_vector_ptr(u32* p_widget) {
	if (p_widget == NULL) {
		return NULL;
	}

	// Retorna de forma directa el puntero almacenado en el índice 0 (+0 bytes)
	return (f32*)(*p_widget);
}

/**
 * @brief Configura un estado o atributo en ráfaga indexada de 4 bytes en el offset 0x8C del subsistema de munición.
 * Dirección original en Ghidra: 0x0034BC70 (PAL)
 *
 * @param p_extended_ammo_struct Dirección base de la subestructura de munición (param_1).
 * @param group_index Índice de la ranura o grupo a modificar en pasos de 4 bytes (param_2).
 * @param attribute_val Valor de estado, bandera o atributo a inyectar (param_3).
 */
void inv_set_ammo_matrix_group_state(void* p_extended_ammo_struct, s32 group_index, u32 attribute_val) {
	if (p_extended_ammo_struct == NULL) {
		return;
	}

	// Calcula el offset exacto aplicando el paso de 4 bytes desplazado a 0x8C
	u8* p_target_slot = (u8*)p_extended_ammo_struct + (group_index * 4) + 0x8C;

	// Inyecta el valor de control de forma directa
	*(u32*)p_target_slot = attribute_val;
}

/**
 * @brief Configura un puntero de grupo o índice de control en ráfaga indexada de 4 bytes en el offset 0x20 del subsistema de munición.
 * Dirección original en Ghidra: 0x0034BC28 (PAL)
 *
 * @param p_extended_ammo_struct Dirección base de la subestructura de munición (param_1).
 * @param group_index Índice de la ranura o grupo a modificar en pasos de 4 bytes (param_2).
 * @param p_group_data Dirección de memoria del bloque de datos o parámetro a enlazar (param_3).
 */
void inv_set_ammo_matrix_group_ptr(void* p_extended_ammo_struct, s32 group_index, u32 p_group_data) {
	if (p_extended_ammo_struct == NULL) {
		return;
	}

	// Calcula el offset exacto aplicando el paso de 4 bytes desplazado a 0x20
	u8* p_target_slot = (u8*)p_extended_ammo_struct + (group_index * 4) + 0x20;

	// Inyecta la dirección o valor de control de forma directa
	*(u32*)p_target_slot = p_group_data;
}

/**
 * @brief Inicializa por defecto y pone a cero el estado del subsistema de la matriz de munición extendida.
 * Configura los factores flotantes de escala (1.0f) e interpolación fina de cuadros (0.0666f) del HUD.
 * Dirección original en Ghidra: 0x0034BB38 (PAL)
 *
 * @param p_extended_ammo_struct Dirección base de la subestructura de munición (param_1).
 */
void inv_reset_extended_ammo_subsystem(u32* p_extended_ammo_struct) {
	if (p_extended_ammo_struct == NULL) {
		return;
	}

	// 1. Limpieza de variables de estado secundarias
	p_extended_ammo_struct[4] = 0;
	u32 zero_token = p_extended_ammo_struct[4];

	p_extended_ammo_struct[7] = 0;
	p_extended_ammo_struct[8] = 0;
	p_extended_ammo_struct[9] = 0;
	p_extended_ammo_struct[10] = 0;

	// 2. Inyección de factores de escala (1.0f) y tasa de animación (0x3d88882f = 0.0666667f)
	p_extended_ammo_struct[3] = 0x3F800000; // 1.0f
	p_extended_ammo_struct[6] = 0x3D88882F; // 0.0666667f (Tasa de interpolación para 60 FPS)

	p_extended_ammo_struct[0] = 0x3F800000; // Escala X = 1.0f
	p_extended_ammo_struct[1] = 0x3F800000; // Escala Y = 1.0f
	p_extended_ammo_struct[2] = 0x3F800000; // Escala Z = 1.0f

	// 3. Vaciado masivo inicial de la cuadrícula tridimensional combinada (Slots 0-1, Grupos 0-1)
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 0, 0);
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 0, 1);
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 1, 0);
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 1, 1);
}

/**
 * @brief Configura las estadísticas de munición dentro de la matriz extendida del inventario (Paso multidimensional de 0x10 y 0x30).
 * Inyecta en ráfaga contigua los parámetros de balas calculando el offset exacto por grupo y ranura.
 * Dirección original en Ghidra: 0x0034BC38 (PAL)
 *
 * @param ammo_type ID o tipo de munición asignada (param_1).
 * @param current_ammo Cantidad de balas actuales (param_2).
 * @param max_ammo Capacidad máxima del cargador (param_3).
 * @param upgrade_state Estado de mejora o nivel del componente (param_4).
 * @param p_matrix_base Dirección de memoria base de la estructura del inventario (param_5).
 * @param slot_index Índice de la ranura secundaria (param_6).
 * @param group_index Índice de la categoría o grupo superior de armas (param_7).
 */
void inv_set_extended_ammo_slot_data(u32 ammo_type, u32 current_ammo, u32 max_ammo, u32 upgrade_state,
	void* p_matrix_base, s32 slot_index, s32 group_index) {
	if (p_matrix_base == NULL) {
		return;
	}

	// Calcula la dirección física de la celda aplicando los pasos indexados de 16 y 48 bytes
	u8* p_data_cell = (u8*)p_matrix_base + (slot_index * 0x10) + (group_index * 0x30);

	// Almacena en ráfaga contigua las estadísticas en los desplazamientos indicados
	*(u32*)(p_data_cell + 0x2C) = ammo_type;     // ID/Tipo de Munición
	*(u32*)(p_data_cell + 0x30) = current_ammo;  // Balas Actuales
	*(u32*)(p_data_cell + 0x34) = max_ammo;      // Capacidad Máxima
	*(u32*)(p_data_cell + 0x38) = upgrade_state; // Estado o Multiplicador
}

/**
 * @brief Configura un par de datos contiguos de 32 bits (X, Y) dentro de la estructura de control de munición (offset +8).
 * Utilizado por el bucle de actualización en vivo para inyectar coordenadas de ráfaga o límites del HUD.
 * Dirección original en Ghidra: 0x0034D1B0 (PAL)
 *
 * @param val_x Primer componente o dato de control (param_1).
 * @param val_y Segundo componente o dato de control contiguo (param_2).
 * @param p_dest_struct Dirección base de la estructura contenedora (param_3).
 */
void hud_set_ammo_widget_context_2d(u32 val_x, u32 val_y, void* p_dest_struct) {
	if (p_dest_struct != NULL) {
		// Recupera el puntero físico real almacenado en el desplazamiento +8
		u32** pp_context_target = (u32**)((u8*)p_dest_struct + 8);
		u32* p_context = *pp_context_target;

		if (p_context != NULL) {
			p_context[0] = val_x;   // Almacena en el offset +0 del bloque apuntado
			p_context[1] = val_y;   // Almacena en el offset +4 del bloque apuntado
		}
	}
}

/**
 * @brief Inicializa por completo el layout visual de la munición y las matrices del inventario de armas.
 * Orquesta el registro de widgets (Fondo, Borde, Texto, Deslizador) y configura las estadísticas base de las ranuras.
 * Dirección original en Ghidra: 0x0034BF20 (PAL)
 */
void hud_init_ammo_layout(void* p_hud_main_struct, long param_2, long p_hud_pool, long p4, long p5, long p6, long p7, long p8) {
	u8* p_base = (u8*)p_hud_main_struct;

	// 1. Reserva inicial y limpieza de nodos de transformación en el pool
	*(u32*)(p_base + 0x13C) = (u32)p_hud_pool;
	if (p_hud_pool != 0) {
		int* node0 = hud_allocate_node((int*)p_hud_pool, param_2, p_hud_pool, p4, p5, p6, p7, p8);
		u32* vec0 = (u32*)core_identity_stub(0x10, node0);
		*(u32**)(p_base + 8) = vec0;
		vec0[0] = 0; vec0[1] = 0; vec0[2] = 0; vec0[3] = 0;

		int* node1 = hud_allocate_node((int*)*(u32*)(p_base + 0x13C), 0, 0, 0, 0, 0, 0, 0);
		u32* vec1 = (u32*)core_identity_stub(0x10, node1);
		*(u32**)(p_base + 0x1C8) = vec1;
		vec1[0] = 0; vec1[1] = 0; vec1[2] = 0; vec1[3] = 0;
	}

	// 2. Registro de componentes visuales (Widgets) de la munición
	hud_register_widget_asset((u32*)(p_base + 0x10), (const char*)0x001AE668, p_hud_pool, p4, p5, p6, p7, p8); // "AmmoBack"
	hud_register_widget_asset((u32*)(p_base + 0x5C), (const char*)0x001AE678, p_hud_pool, p4, p5, p6, p7, p8); // "AmmoOutline"
	hud_register_widget_asset((u32*)(p_base + 0xA8), (const char*)0x001AE680, p_hud_pool, p4, p5, p6, p7, p8); // "AmmoText"

	// 3. Inicialización y configuración estética del deslizador/medidor de balas
	u32* p_slider = (u32*)(p_base + 0xF4);
	hud_init_slider_widget(p_slider, 0x92, 0, (const char*)0x001AE688, p_hud_pool, p4, p5, p6); // "AmmoBar"

	hud_set_widget_context_2d(p_slider, 0x8049c1ff, 0x80001eff);
	hud_set_widget_context_2d_ext(p_slider, 0x50f0c070, 0x50f0c070);
	hud_set_widget_color_alt(p_slider, 0x60442d00);
	hud_set_widget_render_mode_alt(p_slider, 100);

	// Escalado vertical Y del medidor dinámico
	f32 scale_y = 1.5f; // Valor estimado basado en la interpolación original
	hud_set_widget_scale_y(p_slider, (s32)scale_y);

	// 4. Configuración secuencial de las ranuras del inventario de armas (Capa A)
	void* p_inv_a = (void*)(p_base + 0x140);
	inv_reset_weapon_inventory(p_inv_a);
	inv_set_weapon_inventory_mode(p_inv_a, 2);
	inv_set_active_weapon_slot(p_inv_a, (p_base + 0x1CC));
	inv_set_animation_factor(p_inv_a, 0.005f);
	inv_set_quick_select_open_state(p_inv_a, 2);
	inv_set_weapon_slot_data(0, 0x80f0c070, 0x42480000, 0, 0, p_inv_a, 0); // Capacidad 50.0f
	inv_update_weapon_visual_pointers(0, 0, p_inv_a, 0);
	inv_set_weapon_inventory_visibility(p_inv_a, 1);

	// 5. Configuración secuencial de las ranuras del inventario de armas (Capa B)
	void* p_inv_b = (void*)(p_base + 0x1D4);
	inv_reset_weapon_inventory(p_inv_b);
	inv_set_weapon_inventory_mode(p_inv_b, 3);
	inv_set_weapon_slot_data(0, 0x442d00, 0, 0, 0, p_inv_b, 0);
	inv_set_weapon_slot_data(0x3E99999A, 0x60442d00, 0, 0, 0, p_inv_b, 1);
	inv_update_weapon_visual_pointers(0, 0x40000000, p_inv_b, 1);
	inv_set_quick_select_open_state(p_inv_b, 0);
	inv_set_active_weapon_slot(p_inv_b, (p_base + 0x1CC));
	inv_set_weapon_inventory_visibility(p_inv_b, -1);

	// Inicialización de banderas de control secundarias finales
	*(u32*)(p_base + 0x3F4) = 0;
	*(u32*)(p_base + 0x400) = 0xFFFFFFFF;
	*(u32*)(p_base + 0x3F8) = 0;
}

/**
 * @brief Intercambia el valor del candado de animación del inventario (offset 0x28) y retorna su estado previo.
 * Utilizado por el motor para liberar transiciones y verificar estados de sincronización en el HUD.
 * Dirección original en Ghidra: 0x0034B790 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 * @param new_lock_state Nuevo valor de control que se inyectará en la bandera (param_2).
 * @return u32 El estado previo que tenía la bandera antes de ser sobreescrita.
 */
u32 inv_swap_animation_lock(u32* p_inventory_base, u32 new_lock_state) {
	if (p_inventory_base == NULL) {
		return 0;
	}

	// El offset 0x28 equivale al índice 10 en un arreglo de enteros de 32 bits (10 * 4 = 40 bytes)
	u32 old_lock_state = p_inventory_base[0x0A];

	// Sobreescribe la bandera con el nuevo estado solicitado
	p_inventory_base[0x0A] = new_lock_state;

	// Devuelve el valor antiguo al pipeline de renderizado
	return old_lock_state;
}

/**
 * @brief Configura y despacha la transición de visibilidad (Fade In/Out) para la interfaz del inventario de armas.
 * Establece los límites de animación flotante en el offset 0x18 dependiendo del estado solicitado.
 * Dirección original en Ghidra: 0x0034B7F8 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 * @param visibility_state Nuevo estado de visibilidad (1 para visible, 0 para ocultar) (param_2).
 */
void inv_set_weapon_inventory_visibility(u32* p_inventory_base, long visibility_state) {
	if (p_inventory_base == NULL) {
		return;
	}

	// El offset 0x1C equivale al índice 7 en un arreglo de enteros de 32 bits (7 * 4 = 28 bytes)
	p_inventory_base[0x07] = (s32)visibility_state;

	// Offset 0x28 equivale al índice 10 (10 * 4 = 40 bytes), controla el bloqueo de interpolación
	if (p_inventory_base[0x0A] == 0) {
		// Si el estado es 1 (Aparecer), arranca la animación desde 0.0f
		if (visibility_state == 1) {
			p_inventory_base[0x06] = 0; // Offset 0x18 (Índice 6)
		}
		// Si no (Ocultar), arranca la animación de desvanecimiento desde 1.0f (0x3f800000)
		else {
			p_inventory_base[0x06] = 0x3F800000; // Offset 0x18 (Índice 6)
		}

		p_inventory_base[0x0A] = 1; // Activa la bandera de inicio de ciclo de animación
	}
}

/**
 * @brief Configura el estado de visibilidad o apertura del menú radial de selección rápida en el offset 0x20.
 * Dirección original en Ghidra: 0x0034B838 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 * @param open_state Nuevo valor de control (1 para abierto/visible, 0 para cerrado/oculto) (param_2).
 */
void inv_set_quick_select_open_state(u32* p_inventory_base, u32 open_state) {
	if (p_inventory_base != NULL) {
		// El offset 0x20 equivale al índice 8 en un arreglo de enteros de 32 bits (8 * 4 = 32 bytes)
		p_inventory_base[0x08] = open_state;
	}
}

/**
 * @brief Configura la velocidad de interpolación o factor de animación de la interfaz en el offset 0x24 del inventario.
 * Dirección original en Ghidra: 0x0034B840 (PAL)
 *
 * @param animation_val Factor flotante o de control destinado a la velocidad de la interfaz (param_1).
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_2).
 */
void inv_set_animation_factor(u32* p_inventory_base, f32 animation_val) {
	if (p_inventory_base != NULL) {
		// El offset 0x24 equivale al índice 9 en un arreglo de enteros de 32 bits (9 * 4 = 36 bytes)
		p_inventory_base[9] = *(u32*)&animation_val;
	}
}

/**
 * @brief Configura el índice del slot del arma activa o seleccionada en el offset 0x80 del inventario.
 * Dirección original en Ghidra: 0x0034B788 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 * @param slot_index Índice de la ranura o identificación del arma que se va a equipar (param_2).
 */
void inv_set_active_weapon_slot(u32* p_inventory_base, u32 slot_index) {
	if (p_inventory_base != NULL) {
		// El offset 0x80 equivale al índice 32 en un arreglo de enteros de 32 bits (32 * 4 = 128 bytes)
		p_inventory_base[0x20] = slot_index;
	}
}

/**
 * @brief Configura la bandera de transición o estado de carga del inventario de armas en el offset 0x2C.
 * Dirección original en Ghidra: 0x0034B758 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 * @param transition_flag Nuevo valor de control o bandera de estado a inyectar (param_2).
 */
void inv_set_weapon_inventory_transition_flag(u32* p_inventory_base, u32 transition_flag) {
	if (p_inventory_base != NULL) {
		// El offset 0x2C equivale al índice 11 en un arreglo de enteros de 32 bits (11 * 4 = 44 bytes)
		p_inventory_base[0x0B] = transition_flag;
	}
}

/**
 * @brief Configura el modo de visualización o el estado secundario del inventario de armas en el offset 0x84.
 * Dirección original en Ghidra: 0x0034B7F0 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 * @param inventory_mode Nuevo valor de estado o modo lúdico a inyectar (param_2).
 */
void inv_set_weapon_inventory_mode(u32* p_inventory_base, u32 inventory_mode) {
	if (p_inventory_base != NULL) {
		// El offset 0x84 equivale al índice 33 en un arreglo de enteros de 32 bits (33 * 4 = 132 bytes)
		p_inventory_base[33] = inventory_mode;
	}
}

/**
 * @brief Reinicia el estado global del inventario de armas a sus valores predeterminados de fábrica.
 * Limpia las propiedades dinámicas e inicializa las primeras ranuras de la matriz del arsenal.
 * Dirección original en Ghidra: 0x0034B698 (PAL)
 *
 * @param p_inventory_base Dirección de memoria base de la estructura global del inventario (param_1).
 */
void inv_reset_weapon_inventory(void* p_inventory_base) {
	if (p_inventory_base == NULL) {
		return;
	}

	u8* p_inv = (u8*)p_inventory_base;

	// 1. Limpieza de banderas de estado e inicialización de variables base
	*(u32*)(p_inv + 0x18) = 0;
	*(u32*)(p_inv + 0x28) = 0;
	u32 zero_token = *(u32*)(p_inv + 0x18);

	*(u32*)(p_inv + 0x2C) = 0;
	*(u32*)(p_inv + 0x80) = 0;

	// 2. Inyección del factor de animación fina (0x3ba3d70a = 0.005f)
	*(f32*)(p_inv + 0x24) = 0.005f;

	// 3. Inicialización condicional de las ranuras de la matriz utilizando tus funciones
	inv_set_weapon_slot_data(zero_token, zero_token, zero_token, zero_token, zero_token, p_inventory_base, 0);

	// Inyecta el valor flotante 1.0f (0x3f800000) de forma masiva en el slot 1
	u32 float_one_token = 0x3F800000;
	inv_set_weapon_slot_data(float_one_token, float_one_token, float_one_token, float_one_token, float_one_token, p_inventory_base, 1);

	// 4. Configuración de banderas secundarias e inicialización de punteros visuales
	*(u32*)(p_inv + 0x84) = 2; // Selector o contador secundario por defecto
	inv_update_weapon_visual_pointers(zero_token, zero_token, p_inventory_base, 0);

	*(u32*)(p_inv + 0x20) = 0;
	*(u32*)(p_inv + 0x1C) = 1; // Bandera de activación o visibilidad inicial
}

/**
 * @brief Actualiza los punteros a los recursos gráficos (assets/texturas) vinculados a una ranura específica del arsenal.
 * Escribe las direcciones en ráfagas indexadas de 4 bytes para el pipeline de renderizado del inventario.
 * Dirección original en Ghidra: 0x0034B7D8 (PAL)
 *
 * @param p_primary_asset Puntero al recurso gráfico base o textura principal del arma (param_1).
 * @param p_secondary_asset Puntero al recurso gráfico secundario o máscara de interfaz (param_2).
 * @param p_array_base Dirección de memoria base del arreglo de punteros visuales (param_3).
 * @param slot_index Índice de la ranura o columna a modificar (param_4).
 */
void inv_update_weapon_visual_pointers(u32 p_primary_asset, u32 p_secondary_asset,
	void* p_array_base, s32 slot_index) {
	if (p_array_base == NULL) {
		return;
	}

	// Calcula el puntero exacto a la ranura seleccionada (Paso de 4 bytes)
	u32* p_slot_target = (u32*)((u8*)p_array_base + slot_index * 4);

	p_slot_target[0] = p_primary_asset;   // Inyecta en el offset base +0
	p_slot_target[3] = p_secondary_asset; // Inyecta en el offset +12 bytes (índice 3 en u32)
}

/**
 * @brief Inicializa e inyecta los parámetros estadísticos y visuales de un arma dentro de la matriz indexada de inventario.
 * Calcula los offsets contiguos de 16 bytes (0x10) para almacenar munición, IDs y referencias del HUD.
 * Dirección original en Ghidra: 0x0034B7A0 (PAL)
 *
 * @param p_asset_ptr Puntero al nombre o recurso visual del widget del arma (param_1).
 * @param current_ammo Cantidad de balas actuales (param_2).
 * @param max_ammo Capacidad máxima del cargador (param_3).
 * @param weapon_id Identificador único del tipo de arma/munición (param_4).
 * @param experience_val Progreso de experiencia o nivel del arma (param_5).
 * @param p_matrix_base Dirección de memoria base de la tabla de datos del inventario (param_6).
 * @param slot_index Índice de la ranura o columna a modificar (param_7).
 */
void inv_set_weapon_slot_data(u32 p_asset_ptr, u32 current_ammo, u32 max_ammo, u32 weapon_id,
	u32 experience_val, void* p_matrix_base, s32 slot_index) {

	// Calcula la dirección física de la estructura de datos del arma (Paso de 16 bytes)
	u8* p_data_row = (u8*)p_matrix_base + ((s32)p_matrix_base + slot_index * 0x10);

	// Inyecta en ráfaga contigua las estadísticas dinámicas del cargador en los desplazamientos +0x30
	*(u32*)(p_data_row + 0x30) = current_ammo;    // Munición Actual
	*(u32*)(p_data_row + 0x34) = max_ammo;        // Munición Máxima
	*(u32*)(p_data_row + 0x38) = weapon_id;       // ID del Arma
	*(u32*)(p_data_row + 0x3C) = experience_val;  // Valor de XP / Modificador

	// Enlaza el recurso o asset gráfico del widget en la sección de punteros visuales (+0x70)
	u8* p_visual_row = (u8*)p_matrix_base + slot_index * 4;
	*(u32*)(p_visual_row + 0x70) = p_asset_ptr;
}

/**
 * @brief Configura de forma aislada el componente vertical Y (Escala/Orientación) del vector secundario de un widget.
 * Convierte el valor entero a punto flotante y lo inyecta en el desplazamiento +4 del vector del offset +4.
 * Dirección original en Ghidra: 0x00338A20 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param y_scale Factor de escala o dimensión vertical en enteros que será convertida a flotante (param_2).
 */
void hud_set_widget_scale_y(u32* p_widget, s32 y_scale) {
	if (p_widget != NULL) {
		// El offset +4 de la estructura base almacena el puntero al vector de transformación
		f32** pp_vector_target = (f32**)((u8*)p_widget + 4);
		f32* p_vector = *pp_vector_target;

		if (p_vector != NULL) {
			// El desplazamiento +4 dentro del propio vector corresponde al componente Y (índice 1 en f32)
			p_vector[1] = (f32)y_scale;
		}
	}
}

/**
 * @brief Configura las banderas de renderizado o modo de mezcla (Alias alternativo de hud_set_widget_render_mode).
 * Escribe directamente en la propiedad de control gráfico ubicada en el offset 0x40.
 * Dirección original en Ghidra: 0x003388E8 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param render_flags Máscara de bits con las opciones de dibujado y transparencia (param_2).
 */
void hud_set_widget_render_mode_alt(u32* p_widget, u32 render_flags) {
	if (p_widget != NULL) {
		// El offset 0x40 equivale al índice 0x10 en un arreglo de enteros de 32 bits (16 * 4 = 64 bytes)
		p_widget[0x10] = render_flags;
	}
}

/**
 * @brief Configura el color o la opacidad de un componente visual (Alias alternativo de hud_set_widget_color).
 * Escribe directamente en la propiedad ubicada en el offset 0x44 de la estructura.
 * Dirección original en Ghidra: 0x00338A38 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param color_rgba Valor de 32-bits que codifica el color y opacidad (param_2).
 */
void hud_set_widget_color_alt(u32* p_widget, u32 color_rgba) {
	if (p_widget != NULL) {
		// El offset 0x44 equivale al índice 11 en un arreglo de enteros de 32 bits (17 * 4 = 68 bytes)
		p_widget[0x11] = color_rgba;
	}
}

/**
 * @brief Inicializa una estructura de widget destinada a deslizadores lógicos o selectores de valor en la interfaz.
 * Configura los parámetros iniciales de escala y establece el límite superior por defecto al 100%.
 * Dirección original en Ghidra: 0x003380E8 (PAL)
 *
 * @param p_widget Dirección base de la estructura del widget de la interfaz (param_1).
 * @param value_id Identificador o valor inicial asignado al deslizador (param_2).
 * @param p_data_source Puntero de control o fuente de datos del elemento (param_3).
 */
void hud_init_slider_widget(u32* p_widget, u32 value_id, u32 p_data_source, uintptr_t asset_name_ptr,
	uintptr_t p_hud_pool, long p6, long p7, long p8) {

	// 1. Invoca al constructor base para inicializar las matrices espaciales
	// Ajustamos los casteos de los parámetros para que coincidan con la llamada matemática original
	hud_clear_widget_matrices(p_widget, (void*)asset_name_ptr, (void*)p_hud_pool,
		(long)asset_name_ptr, p_hud_pool, p6, p7, p8);

	// 2. Inyecta los parámetros de estado y enlaces de control
	p_widget[0x0F] = value_id;      // Offset 0x3C
	p_widget[0x0D] = p_data_source; // Offset 0x34

	// 3. Establece los límites y banderas iniciales por defecto del motor
	p_widget[0x11] = 0x80000000;    // Offset 0x44 (Máscara de actualización)
	p_widget[0x10] = 100;           // Offset 0x40 (Límite máximo del 100%)
}

/**
 * @brief Configura el segundo par de datos de 32 bits (offsets +8 y +12) dentro de la estructura del contexto del widget (offset 0x0C).
 * Utilizado usualmente para definir los límites de recorte UV o las dimensiones secundarias de una textura.
 * Dirección original en Ghidra: 0x00338190 (PAL)
 *
 * @param p_widget Dirección base de la estructura del widget de la interfaz (param_1).
 * @param val_z Tercer componente o dato de control (param_2).
 * @param val_w Cuarto componente o dato de control contiguo (param_3).
 */
void hud_set_widget_context_2d_ext(u32* p_widget, u32 val_z, u32 val_w) {
	if (p_widget != NULL) {
		// El offset 0x0C equivale al índice 3 en enteros de 32 bits (3 * 4 = 12 bytes)
		u32** pp_context_target = (u32**)((u8*)p_widget + 0x0C);
		u32* p_context = *pp_context_target;

		if (p_context != NULL) {
			p_context[2] = val_z;   // Almacena en el offset +8 del bloque de contexto
			p_context[3] = val_w;   // Almacena en el offset +12 del bloque de contexto
		}
	}
}

/**
 * @brief Configura un par de datos contiguos de 32 bits (X, Y) dentro de la estructura del contexto del widget (offset 0x0C).
 * Dirección original en Ghidra: 0x00338178 (PAL)
 *
 * @param p_widget Dirección base de la estructura del widget de la interfaz (param_1).
 * @param val_x Primer componente o dato de control (param_2).
 * @param val_y Segundo componente o dato de control contiguo (param_3).
 */
void hud_set_widget_context_2d(u32* p_widget, u32 val_x, u32 val_y) {
	if (p_widget != NULL) {
		// El offset 0x0C equivale al índice 3 en enteros de 32 bits (3 * 4 = 12 bytes)
		u32** pp_context_target = (u32**)((u8*)p_widget + 0x0C);
		u32* p_context = *pp_context_target;

		if (p_context != NULL) {
			p_context = val_x;   // Almacena en el offset +0 del bloque de contexto
			p_context = val_y;   // Almacena en el offset +4 del bloque de contexto
		}
	}
}

/**
 * @brief Actualiza de forma dinámica el vector de transformación asignado en el offset +4 de un widget.
 * Libera de forma automática el nodo previo mediante hud_free_node si detecta un cambio posicional o de escala.
 * Dirección original en Ghidra: 0x00337B90 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param p_new_vector Puntero al nuevo vector matemático de 4 componentes a renderizar (param_2).
 */
void hud_update_widget_vector(u32* p_widget, u32* p_new_vector) {
	if (p_widget == NULL) {
		return;
	}

	// El offset +4 equivale al índice 1 en un arreglo de enteros de 32 bits (1 * 4 = 4 bytes)
	u32** pp_current_vector = (u32**)((u8*)p_widget + 4);

	// Valida si el nuevo vector es diferente al que ya está cargado en la estructura
	if (p_new_vector != *pp_current_vector) {

		// Offset 0x2C equivale al índice 11 (11 * 4 = 44 bytes), que almacena el puntero del pool
		u32* p_hud_pool = (u32*)p_widget[0x0B];

		if (p_hud_pool == 0) {
			*pp_current_vector = p_new_vector;
		}
		// Offset 0x18 equivale al índice 6 (6 * 4 = 24 bytes), bandera de refresco/ciclo secundaria
		else if (p_widget[0x06] == 0) {
			// Invoca a tu rutina de reciclaje para liberar el nodo de memoria anterior
			hud_free_node((int*)p_hud_pool, (int*)*pp_current_vector);

			p_widget[0x06] = 1; // Activa la bandera de refresco/redibujado del layout vectorial
			*pp_current_vector = p_new_vector;
		}
		else {
			*pp_current_vector = p_new_vector;
		}
	}
}

/**
 * @brief Recupera el puntero al vector secundario de transformación (Escala/Orientación) de un widget del HUD.
 * Lee directamente la dirección física almacenada en el offset +4 de la estructura.
 * Dirección original en Ghidra: 0x00337AF8 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @return f32* Puntero al vector de escala (X, Y, Z, W) del widget, o NULL si no está asignado.
 */
f32* hud_get_widget_vector_ptr(u32* p_widget) {
	if (p_widget == NULL) {
		return NULL;
	}

	// El offset +4 equivale al índice 1 en un arreglo de enteros de 32 bits (1 * 4 = 4 bytes)
	return (f32*)((long)p_widget[1]);
}

/**
 * @brief Calcula el coseno matemático utilizando optimización por hardware (Originalmente vía VU0 Coprocessor 2).
 * Reemplaza de forma portátil las llamadas a microcódigo vectorial e instrucciones _vcallms de la PS2.
 * Dirección original en Ghidra: 0x00283A58 (PAL)
 *
 * @param radians Ángulo en radianes normalizado previamente (param_1).
 * @return f32 El resultado del coseno calculado de forma nativa por hardware.
 */
f32 math_vu0_cos(f32 radians) {
	// En el hardware original de la PS2 se llamaba a la microrutina 0xC90 en la VU0.
	// De manera portable para sistemas modernos, la FPU del PC lo resuelve al instante:
	return cosf(radians);
}

/**
 * @brief Calcula el seno matemático utilizando optimización por hardware (Originalmente vía VU0 Coprocessor 2).
 * Reemplaza de forma portátil las llamadas a microcódigo vectorial e instrucciones _vcallms de la PS2.
 * Dirección original en Ghidra: 0x00283A40 (PAL)
 *
 * @param radians Ángulo en radianes normalizado previamente (param_1).
 * @return f32 El resultado del seno calculado de forma nativa por hardware.
 */
f32 math_vu0_sin_cos(f32 radians) {
	// En el hardware original, se inyecta el ángulo al Coprocesador 2 vía _qmtc2,
	// se llama a la microrutina 0xC80 en la VU0 y se extrae el resultado con _qmfc2.
	// De manera portable para sistemas modernos, la FPU del PC lo resuelve al instante:
	return sinf(radians);
}

#define MATH_PI 3.14159265358979323846f

/**
 * @brief Normaliza la suma de dos ángulos en radianes para mantener el resultado en el rango [-PI, PI].
 * Corrige desbordamientos angulares circulares sumando o restando una revolución completa (2*PI).
 * Dirección original en Ghidra: 0x00284458 (PAL)
 *
 * @param angle_alpha Primer componente angular en radianes (param_1).
 * @param angle_beta Segundo componente angular en radianes a adicionar (param_2).
 * @return f32 El ángulo resultante normalizado.
 */
f32 math_normalize_angle_rad(f32 angle_alpha, f32 angle_beta) {
	f32 result_angle = angle_alpha + angle_beta;

	// Si el ángulo es mayor o igual a PI, le resta una revolución completa (2 * PI)
	if (result_angle >= (f32)MATH_PI) {
		result_angle = (result_angle - (f32)MATH_PI) - (f32)MATH_PI;
	}
	// Si el ángulo es menor a -PI, le adiciona una revolución completa (2 * PI)
	if (result_angle < -(f32)MATH_PI) {
		result_angle = result_angle + (f32)MATH_PI + (f32)MATH_PI;
	}

	return result_angle;
}

/**
 * @brief Inicializa y registra una estructura de widget destinada a albergar un recurso visual o textura (Asset).
 * Configura las matrices base estableciendo una escala inicial uniforme de 1.0f (100%) para evitar distorsiones.
 * Dirección original en Ghidra: 0x003381B0 / Línea 274 aproximada (PAL)
 */
void hud_register_widget_asset(u32* p_widget, const char* asset_name_ptr, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Invoca al constructor base para limpiar e inicializar las matrices espaciales
	hud_clear_widget_matrices(p_widget, asset_name_ptr, p_hud_pool, p4, p5, p6, p7, p8);

	// El offset 0x0B equivale al índice 11, almacena el puntero del pool de memoria
	if ((int*)p_widget[0x0B] == NULL) {
		p_widget[0x12] = 0; // Inicializa en cero la bandera de estado secundario (offset 0x48)
	}
	else {
		u32* p_vector_node;

		// 2. Reserva y limpia el primer nodo auxiliar en el offset 0x0D
		int* p_node1 = hud_allocate_node((int*)p_widget[0x0B], 0, 0, p4, p5, p6, p7, p8);
		p_vector_node = (u32*)core_identity_stub(0x10, p_node1);
		p_widget[0x0D] = (u32)p_vector_node;

		p_vector_node[0] = 0;
		p_vector_node[1] = 0;
		p_vector_node[2] = 0;
		p_vector_node[3] = 0;

		// 3. Reserva y limpia el segundo nodo auxiliar en el offset 0x0E
		int* p_node2 = hud_allocate_node((int*)p_widget[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector_node = (u32*)core_identity_stub(0x10, p_node2);
		p_widget[0x0E] = (u32)p_vector_node;

		p_vector_node[0] = 0;
		p_vector_node[1] = 0;
		p_vector_node[2] = 0;
		p_vector_node[3] = 0;

		// 4. Configura la escala base predeterminada en 1.0f (0x3f800000) en el segundo vector indexado (param_1[1])
		f32* p_scale_vector = *(f32**)(&p_widget[1]);
		if (p_scale_vector != NULL) {
			p_scale_vector[0] = 1.0f; // Escala X
			p_scale_vector[1] = 1.0f; // Escala Y
			p_scale_vector[2] = 1.0f; // Escala Z
		}

		p_widget[0x12] = 0;
	}

	// Inicializa en cero la propiedad del offset 0x11 (44 bytes)
	p_widget[0x11] = 0;
}

/**
 * @brief Actualiza de forma dinámica el recurso de datos o contexto asignado a un widget del HUD.
 * Libera de forma automática el nodo previo mediante hud_free_node si detecta un cambio de recurso visual.
 * Dirección original en Ghidra: 0x00337C00 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param p_new_resource Puntero al nuevo dato, string o ícono gráfico a renderizar (param_2).
 */
void hud_update_widget_context(u32* p_widget, u32* p_new_resource) {
	if (p_widget == NULL) {
		return;
	}

	// El offset 0x10 equivale al índice 4 en un arreglo de enteros de 32 bits (4 * 4 = 16 bytes)
	u32** pp_current_resource = (u32**)((u8*)p_widget + 0x10);

	// Valida si el nuevo recurso es diferente al que ya está cargado en pantalla
	if (p_new_resource != *pp_current_resource) {

		// Offset 0x2C equivale al índice 11 (11 * 4 = 44 bytes), que almacena el puntero del pool
		u32* p_hud_pool = (u32*)p_widget[0x0B]; // Usando el índice 11 indexado

		if (p_hud_pool == 0) {
			*pp_current_resource = p_new_resource;
		}
		// Offset 0x24 equivale al índice 9 (9 * 4 = 36 bytes), bandera de refresco/ciclo
		else if (p_widget[0x09] == 0) {
			// Invoca a tu rutina del HUD para reciclar y liberar el nodo de memoria anterior
			hud_free_node((int*)p_hud_pool, (int*)*pp_current_resource);

			p_widget[0x09] = 1; // Activa la bandera de refresco/redibujado del layout
			*pp_current_resource = p_new_resource;
		}
		else {
			*pp_current_resource = p_new_resource;
		}
	}
}

/**
 * @brief Libera un nodo de memoria del HUD y lo devuelve a la lista de reusables (Free List).
 * Decrementa el contador de widgets activos y reestructura los punteros de la cabecera del pool.
 * Dirección original en Ghidra: 0x00338C28 (PAL)
 *
 * @param p_hud_pool Estructura de cabecera del pool de memoria de la interfaz (param_1).
 * @param p_node_to_free Puntero al bloque de memoria del nodo que se va a liberar (param_2).
 */
void hud_free_node(int* p_hud_pool, int* p_node_to_free) {
	if (p_hud_pool == NULL || p_node_to_free == NULL) {
		return;
	}

	// El offset 0x14 corresponde al índice 5 en enteros de 32 bits (5 * 4 = 20 bytes)
	int* p_current_free_head = (int*)p_hud_pool[5];

	// Enlaza el nodo que se libera al frente de la lista de reusables anterior
	*p_node_to_free = (int)p_current_free_head;

	// Coloca el nodo liberado como la nueva cabecera de elementos disponibles en el pool
	p_hud_pool[5] = (int)p_node_to_free;

	// El offset 0x10 (índice 4, 16 bytes) reduce el contador de widgets del HUD en uso
	p_hud_pool[4] = p_hud_pool[4] - 1;
}

/**
 * @brief Configura la posición bidimensional (X, Y) en punto flotante para un componente del HUD.
 * Convierte las coordenadas enteras y las almacena secuencialmente en el puntero del offset 0x34.
 * Dirección original en Ghidra: 0x00338600 (PAL)
 *
 * @param p_widget Dirección base de la estructura del widget de la interfaz (param_1).
 * @param x_coord Coordenada horizontal en píxeles (param_2).
 * @param y_coord Coordenada vertical en píxeles (param_3).
 */
void hud_set_widget_position_2d(u32* p_widget, s32 x_coord, s32 y_coord) {
	if (p_widget != NULL) {
		// El offset 0x34 equivale al índice 13 en un arreglo de enteros de 32 bits (13 * 4 = 52 bytes)
		f32** pp_vector_target = (f32**)((u8*)p_widget + 0x34);
		f32* p_vector = *pp_vector_target;

		if (p_vector != NULL) {
			p_vector[0] = (f32)x_coord; // Inyecta la coordenada X como flotante
			p_vector[1] = (f32)y_coord; // Inyecta la coordenada Y como flotante contigua (+4 bytes)
		}
	}
}

/**
 * @brief Recupera el puntero de datos dinámicos o contexto enlazado a un widget del HUD.
 * Lee directamente la dirección física almacenada en el offset 0x0C de la estructura.
 * Dirección original en Ghidra: 0x00337B00 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @return void* Puntero al contexto de datos del widget, o NULL si no está asignado.
 */
void* hud_get_widget_data_ptr(u32* p_widget) {
	if (p_widget == NULL) {
		return NULL;
	}

	// El offset 0x0C equivale al índice 3 en un arreglo de enteros de 32 bits (3 * 4 = 12 bytes)
	return (void*)((long)p_widget[3]);
}

/**
 * @brief Inicializa una estructura de widget destinada a medidores o barras del HUD (ej. Barra de Experiencia).
 * Configura los vectores base con escalas flotantes predeterminadas (100.0f y 64.0f) y reserva su nodo de estado.
 * Dirección original en Ghidra: 0x003383F0 (PAL)
 */
void hud_init_meter_widget(u32* p_widget, const char* text_ptr, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Invoca al constructor base del widget para limpiar e inicializar las matrices espaciales
	hud_clear_widget_matrices(p_widget, text_ptr, p_hud_pool, p4, p5, p6, p7, p8);

	u32* p_state_vector;

	// 2. Si el pool está activo, reserva y limpia el bloque de estado del medidor en el offset 0xD
	if (p_hud_pool == 0) {
		p_state_vector = (u32*)p_widget[0x0D];
	}
	else {
		int* p_node = hud_allocate_node((int*)p_widget[0x0B], 0, 0, p4, p5, p6, p7, p8);
		p_state_vector = (u32*)core_identity_stub(0x10, p_node);
		p_widget[0x0D] = (u32)p_state_vector;

		p_state_vector[0] = 0;
		p_state_vector[1] = 0;
		p_state_vector[2] = 0;
		p_state_vector[3] = 0;

		p_state_vector = (u32*)p_widget[0x0D];
	}

	p_state_vector[0] = 0;
	p_state_vector[1] = 0;

	// 3. Inyecta los valores mágicos flotantes de inicialización (100.0f y 64.0f)
	f32* p_vector_pos = *(f32**)p_widget;       // Primer vector indexado
	f32* p_vector_scale = *(f32**)(&p_widget[1]); // Segundo vector indexado

	if (p_vector_pos != NULL) {
		p_vector_pos[0] = 100.0f; // 0x42c80000 en la PS2
		p_vector_pos[1] = 100.0f;
	}

	if (p_vector_scale != NULL) {
		p_vector_scale[0] = 64.0f; // 0x42800000 en la PS2
		p_vector_scale[1] = 64.0f;
	}

	// Inicializa en cero la bandera de animación o temporizador
	p_widget[0x0E] = 0;
}


/**
 * @brief Configura el color base o el tinte de transparencia (Alpha) de un widget del HUD.
 * Escribe directamente en la propiedad de control cromático ubicada en el offset 0x44.
 * Dirección original en Ghidra: 0x00338728 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param color_rgba Valor de 32-bits que codifica el color y opacidad en formato RGBA/Color-ID (param_2).
 */
void hud_set_widget_color(u32* p_widget, u32 color_rgba) {
	if (p_widget != NULL) {
		// El offset 0x44 equivale al índice 0x11 en un arreglo de enteros de 32 bits (17 * 4 = 68 bytes)
		p_widget[0x11] = color_rgba;
	}
}

/**
 * @brief Configura las banderas de renderizado o modo de mezcla Alpha en la estructura de un widget del HUD.
 * Escribe directamente en la propiedad de control gráfico ubicada en el offset 0x40.
 * Dirección original en Ghidra: 0x00338730 (PAL)
 *
 * @param p_widget Dirección base de la estructura del componente de la interfaz (param_1).
 * @param render_flags Máscara de bits con las opciones de dibujado y transparencia (param_2).
 */
void hud_set_widget_render_mode(u32* p_widget, u32 render_flags) {
	if (p_widget != NULL) {
		// El offset 0x40 equivale al índice 0x10 en un arreglo de enteros de 32 bits (16 * 4 = 64 bytes)
		p_widget[0x10] = render_flags;
	}
}

/**
 * @brief Almacena de forma directa cuatro componentes individuales (X, Y, Z, W) en un puntero vectorial indexado.
 * Utilizado por el motor para actualizar las coordenadas físicas de transformación de los widgets del HUD.
 * Dirección original en Ghidra: 0x00337C68 (PAL)
 *
 * @param x Componente físico X o canal de color Red (param_1).
 * @param y Componente físico Y o canal de color Green (param_2).
 * @param z Componente físico Z o canal de color Blue (param_3).
 * @param w Componente físico W o canal de control Alpha (param_4).
 * @param p_dest_struct Estructura contenedora del puntero destino real (param_5).
 */
void math_set_vector4_ptr(u32 x, u32 y, u32 z, u32 w, void* p_dest_struct) {
	// Recupera el puntero físico real almacenado en el desplazamiento +4
	u32** pp_vector_target = (u32**)((u8*)p_dest_struct + 4);
	u32* p_vector = *pp_vector_target;

	if (p_vector != NULL) {
		p_vector[0] = x;   // Componente X / R
		p_vector[1] = y;   // Componente Y / G
		p_vector[2] = z;   // Componente Z / B
		p_vector[3] = w;   // Componente W / A
	}
}

/**
 * @brief Inicializa las matrices de transformación espacial y vectores tridimensionales de un Widget del HUD.
 * Reserva los bloques de memoria requeridos en el pool y activa la visibilidad del componente.
 * Dirección original en Ghidra: 0x00337CA8 (PAL)
 */
void hud_clear_widget_matrices(u32* p_widget_transform, const char* text_ptr, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// Guarda la dirección del pool de memoria de la interfaz en el offset 0xB
	p_widget_transform[0x0B] = (u32)p_hud_pool;

	if (p_hud_pool != 0) {
		u32* p_vector;

		// 1. Asignar y limpiar Vector 0 (Posición Inicial)
		int* node0 = hud_allocate_node((int*)p_hud_pool, (long)text_ptr, p_hud_pool, p4, p5, p6, p7, p8);
		p_vector = (u32*)core_identity_stub(0x10, node0);
		p_widget_transform[0] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 2. Asignar y limpiar Vector 2 (Rotación)
		int* node1 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node1);
		p_widget_transform[2] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 3. Asignar y limpiar Vector 1 (Escala)
		int* node2 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node2);
		p_widget_transform[1] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 4. Asignar y limpiar Vector 3 (Velocidad / Interpolación)
		int* node3 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node3);
		p_widget_transform[3] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 5. Asignar y limpiar Vector 4 (Desplazamiento Secundario)
		int* node4 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node4);
		p_widget_transform[4] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;
	}

	// Inicialización de flags físicos y offsets lógicos del motor gráfico
	p_widget_transform[10] = (u32)text_ptr;
	p_widget_transform[8] = 0;
	p_widget_transform[5] = 0;
	p_widget_transform[7] = 0;
	p_widget_transform[6] = 0;
	p_widget_transform[9] = 0;

	// Fuerza la activación de visibilidad para renderizar el componente en pantalla
	hud_set_widget_visibility(p_widget_transform, 1);
}


/**
 * @brief Asigna u obtiene un nodo de memoria libre para un componente visual del HUD (Pool Allocator).
 * Realiza comprobaciones estrictas de límites físicos y dispara aserciones ante desbordamientos de memoria del HUD.
 * Dirección original en Ghidra: 0x00338B98 (PAL)
 *
 * @param p_hud_pool Estructura de cabecera del pool de memoria de la interfaz (param_1).
 * @return int* Puntero al bloque de memoria del nodo inicializado listo para el widget.
 */
int* hud_allocate_node(int* p_hud_pool, long p2, long p3, long param_4,
	long param_5, long param_6, long param_7, long param_8) {
	int* p_allocated_node = (int*)p_hud_pool[5];

	// Caso A: No hay nodos libres reusables en la lista, se debe recortar memoria nueva
	if (p_allocated_node == NULL) {
		int current_offset = p_hud_pool[3];
		u32 next_target_size = current_offset + p_hud_pool[2];

		// Comprobación de desbordamiento de memoria del Pool de la interfaz
		if ((u32)p_hud_pool[1] < next_target_size) {
			// Invoca a tu manejador del kernel para congelar el software e informar la línea del bug
			sys_assert_dispatch((const char*)0x001adb18, 0x53, (const char*)0x001adb60,
				param_4, param_5, param_6, param_7, param_8);
			p_allocated_node = NULL;
		}
		else {
			p_hud_pool[3] = next_target_size;     // Avanza el puntero de asignación de memoria
			p_hud_pool[4] = p_hud_pool[4] + 1;     // Incrementa el contador de widgets activos
			p_allocated_node = (int*)(*p_hud_pool + current_offset); // Dirección física calculada
		}
	}
	// Caso B: Camino rápido (Recicla un nodo previamente liberado en la Free List)
	else {
		int next_free_node = *p_allocated_node;
		p_hud_pool[4] = p_hud_pool[4] + 1;         // Incrementa widgets activos
		p_hud_pool[5] = next_free_node;           // Mueve la cabecera al siguiente nodo libre
	}

	return p_allocated_node;
}


/**
 * @brief Enlaza el recurso tipográfico y configura las propiedades visuales del texto del HUD.
 * Establece fuentes, escalas iniciales (1.0f), espaciado (0.7f) y banderas de renderizado.
 * Dirección original en Ghidra: 0x00338688 (PAL)
 */
void hud_link_widget_text(u32* p_widget, const char* text_resource, long param_3,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Llama a la subrutina interna para limpiar las matrices de transformación
	hud_clear_widget_matrices(p_widget, text_resource, param_3, p4, p5, p6, p7, p8);

	// 2. Asigna el puntero del recurso de la fuente tipográfica global (offset 0xd)
	p_widget[0xD] = 0x002638D0;

	// 3. Configura las escalas iniciales de renderizado X e Y a 1.0f (0x3f800000)
	f32* p_scale = (f32*)p_widget[1];
	p_scale[0] = 1.0f; // Escala X
	p_scale[1] = 1.0f; // Escala Y

	// 4. Configura propiedades de formato y espaciado (0x3f333333 = 0.7f)
	p_widget[0xE] = 1;          // Flag de inicialización o visibilidad activa
	p_widget[0x10] = 0;          // Offset de desplazamiento de renderizado
	p_widget[0x14] = 0x3f333333; // Espaciado entre caracteres / Kerning (0.7f)
	p_widget[0x11] = 1;          // Modo de alineación (ej. Centrado)
	p_widget[0x13] = 0x200;      // Flags de renderizado adicionales (ej. Activar Sombra)
	p_widget[0x12] = 0;          // Rotación o inclinación del texto
}


/**
 * @brief Inicializa por completo el núcleo central de los widgets y medidores gráficos del HUD (Parte 1).
 * Da de alta el lienzo maestro y las coordenadas de la barra de Nanotecnología (vida) en ráfagas vectoriales.
 * Dirección original en Ghidra: 0x0034B860 / Línea de entrada aproximada (PAL)
 */
void hud_initialize_main_widgets(u32* p_hud_context, s32 state_offset, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// Declaramos el flag simulado para el refresco de animación en PC
	static u32 DAT_001a7c18 = 1; // Forzamos 1 (Modo fluido de refresco activo)

	// Declaramos las variables de pila simuladas que Ghidra extrajo del Emotion Engine
	u32 piStack_c0[16] = { 0 };
	u32 piStack_c4[16] = { 0 };
	u32 piStack_c8[16] = { 0 };
	u32 piStack_d8[16] = { 0 };
	u32 piStack_dc[16] = { 0 };
	u32 piStack_e0[16] = { 0 }; // Añadido preventivamente para la línea 1411
	u32 piStack_e4[16] = { 0 };
	u32 piStack_ec[16] = { 0 };
	u32 piStack_f0[16] = { 0 }; // Inicializado seguro como un búfer contiguo
	u32 piStack_f4[16] = { 0 };
	u32 piStack_fc[16] = { 0 };
	u32 piStack_100[16] = { 0 };
	u32 piStack_104[16] = { 0 };
	u32 piStack_108[16] = { 0 };
	u32 piStack_110[16] = { 0 };
	u32 piStack_11c[16] = { 0 };
	u32 piStack_114[16] = { 0 };
	u32 piStack_118[16] = { 0 };
	u32 piStack_120[16] = { 0 };
	u32 piStack_124[16] = { 0 };
	u32 piStack_128[16] = { 0 };
	u32 piStack_130[16] = { 0 };

	// Declaramos las variables locales de soporte de la PS2 para las matrices de munición extendida
	u32 piStack_10c = { 0 };
	u32 piStack_f8 = { 0 };
	u32 piStack_cc = { 0 };

	// Declaramos la variable temporal de control que Ghidra extrajo para la matriz de munición
	int iVar25 = 0;

	// 1. Limpieza de los contadores maestros de refresco del canvas en los offsets +0x567 y +0x568
	p_hud_context[0x567] = 0;
	p_hud_context[0x568] = 0;

	// Cálculo del offset dinámico de búsqueda de estado de producción de Insomniac
	s32 state_lookup_id = state_offset + 0x8710;

	// 2. Registro e inyección de coordenadas del Widget 0 (Lienzo Maestro del HUD)
	hud_register_widget_asset(p_hud_context, RECURSO_HUD_CANVAS, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_hud_context, state_lookup_id, 1);
	math_set_vector4(10.0f, 10.0f, 0.0f, 0.0f, p_hud_context); // 0x41200000 = 10.0f

	// 3. Registro e inyección de coordenadas del Widget 1 (Contorno de Barra de Vida)
	u32* p_health_outline_widget = p_hud_context + 0x13; // Offset indexado param_1 + 0x13
	hud_register_widget_asset(p_health_outline_widget, RECURSO_HEALTH_OUTLINE, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_health_outline_widget, state_lookup_id, 2);
	math_set_vector4(10.0f, 10.0f, 0.0f, 0.0f, p_health_outline_widget);

	// 4. Registro e inyección de coordenadas del Widget 2 (Relleno de Barra de Vida)
	u32* p_health_fill_widget = p_hud_context + 0x26; // Offset indexado param_1 + 0x26
	hud_register_widget_asset(p_health_fill_widget, RECURSO_HEALTH_FILL, p_hud_pool, p4, p5, p6, p7, p8);
	// (La inyección de coordenadas y el set_state continúan en el siguiente frame/bloque de código)
	math_set_vector4(10.0f, 18.5f, 0.0f, 0.0f, p_health_fill_widget); // 0x41940000 = 18.5f

	// (Esta sección continúa de forma directa el flujo dentro de hud_initialize_main_widgets)
	hud_set_state_from_lookup((int)p_health_fill_widget, state_lookup_id, 5);

	// 5. Configuración e inicialización del Widget del Nombre del Arma / AmmoText
	u32* p_wpn_name_widget = p_hud_context + 0x40; // piVar14
	ee_memset((p_hud_context + 0x39), 0, 0x18);
	hud_link_widget_text(p_wpn_name_widget, RECURSO_WEAPON_NAME, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x3f666666, 0, 0, 0, p_wpn_name_widget);
	hud_set_widget_render_mode_alt(p_wpn_name_widget, (u32)(p_hud_context + 0x39));
	math_set_vector4(64.0f, 150.0f, 0.0f, 0.0f, p_wpn_name_widget); // 0x42800000 = 64.0f
	hud_set_widget_color(p_wpn_name_widget, 0);

	// 6. Registro del Medidor de Barra de Experiencia del Arma (WeaXP)
	u32* p_wpn_xp_widget = p_hud_context + 0x56; // piStack_144
	hud_init_meter_widget(p_wpn_xp_widget, RECURSO_WEAPON_XP, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x42000000, 0x42000000, 0, 0, p_wpn_xp_widget);
	math_set_vector4(20.0f, 150.0f, 0.0f, 0.0f, p_wpn_xp_widget); // 0x41a00000 = 20.0f
	u32* p_xp_data = (u32*)hud_get_widget_data_ptr(p_wpn_xp_widget);
	*p_xp_data = 0x60f0f0b0; // Inicialización cromática del color de la experiencia

	// 7. Registro de los Widgets de Iconografía de Balas
	u32* p_ammo_icon = p_hud_context + 0x65; // piStack_140
	hud_register_widget_asset(p_ammo_icon, RECURSO_AMMO_ICON, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4(498.0f, 10.0f, 0.0f, 0.0f, p_ammo_icon); // 0x43f90000 = 498.0f
	hud_set_state_from_lookup((int)p_ammo_icon, state_lookup_id, 3);

	u32* p_ammo_icon_back = p_hud_context + 0x78; // piStack_13c
	hud_register_widget_asset(p_ammo_icon_back, RECURSO_AMMO_ICON_BACK, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4(498.0f, 10.0f, 0.0f, 0.0f, p_ammo_icon_back);
	hud_set_state_from_lookup((int)p_ammo_icon_back, state_lookup_id, 4);

	// 8. Registro del Medidor de Billetera de Guitones (BoltText y BoltIcon)
	u32* p_bolt_text_widget = p_hud_context + 0x92; // piVar16
	ee_memset((p_hud_context + 0x8b), 0, 0x18);
	hud_link_widget_text(p_bolt_text_widget, RECURSO_BOLT_TEXT, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x3f666666, 0, 0, 0, p_bolt_text_widget);
	hud_set_widget_render_mode_alt(p_bolt_text_widget, (u32)(p_hud_context + 0x8b));
	math_set_vector4(440.0f, 150.0f, 0.0f, 0.0f, p_bolt_text_widget); // 0x43dc0000 = 440.0f
	hud_set_widget_color(p_bolt_text_widget, 2);

	u32* p_bolt_icon_widget = p_hud_context + 0xA8; // piStack_138
	hud_init_meter_widget(p_bolt_icon_widget, RECURSO_BOLT_ICON, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x42000000, 0x42000000, 0, 0, p_bolt_icon_widget);
	math_set_vector4(457.0f, 150.0f, 0.0f, 0.0f, p_bolt_icon_widget); // 0x43e48000 = 457.0f
	hud_set_widget_position_2d(p_bolt_icon_widget, 103, 117); // 0x7567 regional plano

	// 9. Registro del Menú Radial de Selección Rápida (Quick Select Base & Anillo)
	u32* p_qsel_back = p_hud_context + 0xb8; // piVar2
	hud_register_widget_asset(p_qsel_back, RECURSO_QSEL_BACK, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_qsel_back, state_lookup_id, 7);
	math_set_vector4_ptr(1.0f, 1.0f, 0, 0, p_qsel_back);
	math_set_vector4(500.0f, 208.0f, 0, 0, p_qsel_back); // 0x42fa0000 = 500.0f, 0x43500000 = 208.0f
	u32* p_qsel_back_data = (u32*)hud_get_widget_data_ptr(p_qsel_back);
	*p_qsel_back_data = 0x442d00;
	hud_update_widget_context(p_qsel_back, (u32*)p_hud_context[0x201]);

	u32* p_qsel_bord = p_hud_context + 0xcb; // piVar12
	hud_register_widget_asset(p_qsel_bord, RECURSO_QSEL_BORD, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_qsel_bord, state_lookup_id, 6);
	math_set_vector4_ptr(1.0f, 1.0f, 0, 0, p_qsel_bord);
	math_set_vector4(500.0f, 208.0f, 0, 0, p_qsel_bord);
	u32* p_qsel_bord_data = (u32*)hud_get_widget_data_ptr(p_qsel_bord);
	*p_qsel_bord_data = 0xf0c070;
	hud_update_widget_context(p_qsel_bord, (u32*)p_hud_context[0x201]);

	// 10. BUCLE DE GENERACIÓN Y REGISTRO EN RÁFAGA DE LAS RANURAS RADIALES (QSelBI0 - QSelBI27)
	char name_construction_buffer[16];
	u32 loop_iterator = 0;
	u32* p_dynamic_slot_widget = NULL;

	do {
		// Tu función vsnprintf interna construyendo secuencialmente los identificadores
		// Simulamos la llamada pasando la lista de argumentos para dar formato a "QSelBI%d"
		// txt_vsnprintf_internal(name_construction_buffer, FORMATO_SLOT_RADIAL, loop_iterator);

		p_dynamic_slot_widget = p_hud_context + (loop_iterator * 0x13) + 0xde; // piVar2 dinámico
		hud_register_widget_asset(p_dynamic_slot_widget, name_construction_buffer, p_hud_pool, p4, p5, p6, p7, p8);

		loop_iterator++;

		// (Esta sección continúa de forma directa la lógica interna de hud_initialize_main_widgets)
		hud_set_state_from_lookup((int)p_dynamic_slot_widget, state_lookup_id, loop_iterator + 8);
		math_set_vector4_ptr(1.0f, 1.0f, 0, 0, p_dynamic_slot_widget);
		math_set_vector4(500.0f, 208.0f, 0, 0, p_dynamic_slot_widget);
		hud_update_widget_context(p_dynamic_slot_widget, (u32*)p_hud_context[0x201]);
		u32* p_dynamic_slot_data = (u32*)hud_get_widget_data_ptr(p_dynamic_slot_widget);
		*p_dynamic_slot_data = 0x442d00;

		// Construcción dinámica de la string del icono "QSelIco%d"
		// txt_vsnprintf_internal(name_construction_buffer, FORMATO_ICONO_RADIAL, loop_iterator);

		u32* p_dynamic_icon_widget = p_hud_context + (loop_iterator * 0x0F) + 0x189; // piVar2 de icono
		hud_init_meter_widget(p_dynamic_icon_widget, name_construction_buffer, p_hud_pool, p4, p5, p6, p7, p8);
		hud_set_widget_visibility(p_dynamic_icon_widget, 0);

		// CÁLCULO GEOMÉTRICO DE PROYECIÓN RADIAL EN LA VU0 POR HARDWARE
		f32 base_angle = ((f32)loop_iterator + (f32)loop_iterator) * 0.3926991f - 3.1415927f;
		f32 normal_angle = math_normalize_angle_rad(base_angle, 1.5707964f);

		f32 sin_val = math_vu0_sin_cos(normal_angle);
		f32 cos_val = math_vu0_cos(normal_angle);

		// Proyecta las coordenadas elípticas horizontales (X) y verticales (Y) en la pantalla
		f32 projected_x = (f32)((s32)(sin_val * 82.14f) + 109); // 0x6d = 109
		f32 projected_y = (f32)((s32)(cos_val * 76.442f) + 189); // 0xbd = 189
		math_set_vector4(projected_x, projected_y, 0, 0, p_dynamic_icon_widget);

		math_set_vector4_ptr(0x42000000, 0x42000000, 0, 0, p_dynamic_icon_widget);
		u32* p_dynamic_icon_data = (u32*)hud_get_widget_data_ptr(p_dynamic_icon_widget);
		*p_dynamic_icon_data = 0xf0f0b0;

		loop_iterator++;
	} while (loop_iterator < 8);

	// 11. Bucle en ráfaga de sincronización y acoplamiento de vectores del HUD (Free List)
	u32* p_sync_vector_src = p_hud_context + 0xde; // piStack_12c
	u32* p_sync_target_a = p_hud_context + 0xf1;   // piVar2
	u32* p_sync_target_b = p_hud_context + 0x198;  // piVar12
	s32 sync_iterator = 6;

	do {
		sync_iterator--;
		u32* p_shared_vec_a = (u32*)hud_get_widget_vector_ptr(p_sync_vector_src);
		hud_update_widget_vector(p_sync_target_a, p_shared_vec_a);
		p_sync_target_a += 0x13;

		u32* p_shared_vec_b = (u32*)hud_get_widget_vector_ptr(p_hud_context + 0x134); // piStack_134
		hud_update_widget_vector(p_sync_target_b, p_shared_vec_b);
		p_sync_target_b += 0x0F;
	} while (sync_iterator > 0);

	// Sincroniza el candado vectorial final en el anclaje perimetral
	u32* p_final_shared_vec = (u32*)hud_get_widget_vector_ptr(p_sync_vector_src);
	hud_update_widget_vector((u32*)(p_hud_context + 0xb8), p_final_shared_vec); // piStack_d4

	// 12. Inicialización del layout maestro de la munición y ráfaga de sub-inventarios secundios
	u32* p_ammo_layout_container = p_hud_context + 0x202; // piStack_e8
	hud_init_ammo_layout(p_ammo_layout_container, (long)state_offset, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_ammo_widget_context_2d(0x43130000, 0x41200000, p_ammo_layout_container);
	p_ammo_layout_container[3] = 0;

	// Inicialización del Inventario de Soporte Secundario (Capa de Dispositivos / Gadgets)
	u32* p_sub_inv_gadgets = p_hud_context + 0x40; // piVar14
	inv_reset_weapon_inventory(p_sub_inv_gadgets);
	inv_set_weapon_inventory_mode(p_sub_inv_gadgets, 3);
	inv_set_weapon_slot_data(0, 0x442d00, 0, 0, 0, p_sub_inv_gadgets, 0);
	inv_set_weapon_slot_data(0x3E99999A, 0x60442d00, 0, 0, 0, p_sub_inv_gadgets, 1);
	inv_set_weapon_slot_data(0x3F800000, 0x60442d00, 0, 0, 0, p_sub_inv_gadgets, 2);
	inv_update_weapon_visual_pointers(0, 0x40000000, p_sub_inv_gadgets, 1);

	u32 context_data_res = (u32)hud_get_widget_data_ptr(p_sync_vector_src);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_gadgets, context_data_res);
	inv_set_quick_select_open_state(p_sub_inv_gadgets, 0);
	inv_set_active_weapon_slot(p_sub_inv_gadgets, (p_hud_context + 0x11c));
	inv_set_weapon_inventory_visibility(p_sub_inv_gadgets, -1);

	// Ajusta la aceleración dinámica de interpolación según las banderas globales del sistema
	f32 anim_speed = (DAT_001a7c18 != 0) ? 0.035f : 0.029f; // Valores estimados de interpolación fina
	inv_set_animation_factor(p_sub_inv_gadgets, anim_speed);
	inv_swap_animation_lock(p_sub_inv_gadgets, 0);

	// (Esta sección continúa de forma directa la lógica interna de hud_initialize_main_widgets)
	f32 alt_anim_speed = (DAT_001a7c18 != 0) ? 0.035f : 0.029f;
	inv_set_animation_factor((u32*)piStack_f0, alt_anim_speed);
	inv_swap_animation_lock((u32*)piStack_f0, 0);

	// Inicialización del Inventario de Soporte Secundario (Capa C)
	u32* p_sub_inv_c = (u32*)piStack_e0;
	inv_reset_weapon_inventory(p_sub_inv_c);
	inv_set_weapon_inventory_mode(p_sub_inv_c, 3);
	inv_set_weapon_slot_data(0, 0x3f99999a, 0x3f99999a, 0, 0, p_sub_inv_c, 0); // 0x3f99999a = 1.2f
	inv_set_weapon_slot_data(0x3F666666, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_c, 1);
	inv_set_weapon_slot_data(0x3F800000, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_c, 2);
	u32* p_vec_c0 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_c0);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_c, (u32)p_vec_c0);
	inv_set_quick_select_open_state(p_sub_inv_c, 0);
	inv_set_active_weapon_slot(p_sub_inv_c, (u32)piStack_120);
	inv_set_weapon_inventory_visibility(p_sub_inv_c, -1);
	inv_set_animation_factor(p_sub_inv_c, alt_anim_speed);
	inv_swap_animation_lock(p_sub_inv_c, 0);

	// Inicialización del Inventario de Soporte Secundario (Capa D - Dispositivos Especiales)
	u32* p_sub_inv_d = (u32*)piStack_128;
	inv_reset_weapon_inventory(p_sub_inv_d);
	inv_set_weapon_inventory_mode(p_sub_inv_d, 3);
	inv_set_weapon_slot_data(0, 0, 0, 0, 0, p_sub_inv_d, 0);
	inv_set_weapon_slot_data(0x3C23D70A, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_d, 1); // 0x3c23d70a = 0.01f
	inv_set_weapon_slot_data(0x3F800000, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_d, 2);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_d, 0);
	inv_set_quick_select_open_state(p_sub_inv_d, 0);
	inv_set_active_weapon_slot(p_sub_inv_d, (u32)piStack_120);
	inv_set_weapon_inventory_visibility(p_sub_inv_d, -1);
	inv_set_animation_factor(p_sub_inv_d, alt_anim_speed);
	inv_swap_animation_lock(p_sub_inv_d, 0);

	// 13. CONFIGURACIÓN Y DESPACHO DE LA MATRIZ DE MUNICIÓN EXTENDIDA (Bloque de Control 1)
	u32* p_ammo_matrix_1 = (u32*)piStack_10c;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_1);
	u32* p_widget_data_1 = (u32*)hud_get_widget_data_ptr((u32*)p_hud_context);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_1, 0, (u32)p_widget_data_1);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_1, 0, 0x442d00);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_1, 1, 0x60442d00);
	p_ammo_matrix_1[2] = 0;
	p_ammo_matrix_1[3] = 0x40000000; // 2.0f
	u32* p_widget_vec_1 = (u32*)hud_get_widget_vector_ptr((u32*)p_hud_context);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_1, 1, (u32)p_widget_vec_1);
	inv_set_extended_ammo_slot_data(0, 0, 0, 0, p_ammo_matrix_1, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_1, 1, 1);
	u32* p_widget_pos_1 = (u32*)hud_get_widget_position_vector_ptr((u32*)p_hud_context);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_1, 2, (u32)p_widget_pos_1);
	inv_set_extended_ammo_slot_data(0x42480000, 0x41c80000, 0, 0, p_ammo_matrix_1, 2, 0); // 50.0f y 25.0f
	inv_set_extended_ammo_slot_data(0x41200000, 0x41200000, 0, 0, p_ammo_matrix_1, 2, 1); // 10.0f y 10.0f

	// 14. CONFIGURACIÓN Y DESPACHO DE LA MATRIZ DE MUNICIÓN EXTENDIDA (Bloque de Control 2)
	u32* p_ammo_matrix_2 = (u32*)piStack_f8;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_2);
	u32* p_widget_data_2 = (u32*)hud_get_widget_data_ptr((u32*)piStack_cc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_2, 0, (u32)p_widget_data_2);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_2, 0, 0xf0c070);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_2, 1, 0x55f0c070);
	p_ammo_matrix_1[3] = (u32)iVar25;
	p_ammo_matrix_1[2] = 0;
	u32* p_widget_vec_2 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_cc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_2, 1, (u32)p_widget_vec_2);
	inv_set_extended_ammo_slot_data(0x3F99999A, 0x3F99999A, 0, 0, p_ammo_matrix_2, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_2, 1, 1);
	u32* p_widget_pos_2 = (u32*)hud_get_widget_position_vector_ptr((u32*)piStack_cc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_2, 2, (u32)p_widget_pos_2);
	inv_set_extended_ammo_slot_data(0, 0, 0, 0, p_ammo_matrix_2, 2, 0);
	inv_set_extended_ammo_slot_data(0x41200000, 0x41200000, 0, 0, p_ammo_matrix_2, 2, 1);

	// 15. CONFIGURACIÓN Y DESPACHO DE LA MATRIZ DE MUNICIÓN EXTENDIDA (Bloque de Control 3)
	u32* p_ammo_matrix_3 = (u32*)piStack_e4;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_3);
	u32* p_widget_data_3 = (u32*)hud_get_widget_data_ptr((u32*)piStack_114);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_3, 0, (u32)p_widget_data_3);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_3, 0, 0xf0f0f0);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_3, 1, 0x80f0f0f0);
	p_ammo_matrix_3[3] = (u32)iVar25;
	p_ammo_matrix_3[2] = 0;

	// Inicialización del Inventario Complementario de Balas (Capa E)
	u32* p_sub_inv_e = (u32*)piStack_c8;
	inv_reset_weapon_inventory(p_sub_inv_e);
	inv_set_weapon_inventory_mode(p_sub_inv_e, 3);
	inv_set_weapon_slot_data(0, 0x1eff, 0, 0, 0, p_sub_inv_e, 0);
	inv_set_weapon_slot_data(0x3F000000, 0x1eff, 0, 0, 0, p_sub_inv_e, 1);
	inv_set_weapon_slot_data(0x3F800000, 0x80001eff, 0, 0, 0, p_sub_inv_e, 2);
	u32* p_vec_e0 = (u32*)hud_get_widget_data_ptr((u32*)piStack_130);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_e, (u32)p_vec_e0);
	inv_set_quick_select_open_state(p_sub_inv_e, 0);
	inv_set_active_weapon_slot(p_sub_inv_e, (u32)piStack_11c);
	inv_set_weapon_inventory_visibility(p_sub_inv_e, 1);
	inv_set_animation_factor(p_sub_inv_e, 0.0666f); // 0x3d88850a \approx 0.0666f

	// 16. CONFIGURACIÓN Y DESPACHO DE LA MATRIZ DE MUNICIÓN EXTENDIDA (Bloque de Control 4 y 5)
	u32* p_ammo_matrix_4 = (u32*)piStack_124;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_4);
	u32* p_widget_data_4 = (u32*)hud_get_widget_data_ptr((u32*)piStack_110);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_4, 0, (u32)p_widget_data_4);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_4, 0, 0xf0f0b0);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_4, 1, 0x60f0f0b0);
	p_ammo_matrix_4[2] = 0;
	p_ammo_matrix_4[3] = (u32)iVar25;
	// Corregido: Pasamos el valor numérico crudo de forma directa sin punteros imposibles
	inv_set_ammo_capacity_multiplier(0x0000666f, p_ammo_matrix_4); // Mochila de...

	u32* p_ammo_matrix_5 = (u32*)piStack_108;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_5);
	u32* p_widget_data_5 = (u32*)hud_get_widget_data_ptr((u32*)piStack_100);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_5, 0, (u32)p_widget_data_5);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_5, 0, 0x442d00);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_5, 1, 0x60442d00);

	// (Esta sección cierra de forma definitiva la lógica interna de hud_initialize_main_widgets)
	u32* p_ammo_matrix_5_base = (u32*)piStack_108;
	p_ammo_matrix_5_base[3] = 0x40000000;
	p_ammo_matrix_5_base[2] = 0; // iVar17 = 0
	u32* p_widget_vec_5 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_100);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_5_base, 1, (u32)p_widget_vec_5);
	inv_set_extended_ammo_slot_data(0, 0, 0, 0, p_ammo_matrix_5_base, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_5_base, 1, 1);
	u32* p_widget_pos_5 = (u32*)hud_get_widget_position_vector_ptr((u32*)piStack_100);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_5_base, 2, (u32)p_widget_pos_2);
	inv_set_extended_ammo_slot_data(0x43E50000, 0x41C80000, 0, 0, p_ammo_matrix_5_base, 2, 0); // 458.0f y 25.0f
	inv_set_extended_ammo_slot_data(0x3F666666, 0x41200000, 0, 0, p_ammo_matrix_5_base, 2, 1);

	// 17. CONFIGURACIÓN Y DESPACHO DE LA MATRIZ DE MUNICIÓN EXTENDIDA (Bloque de Control 6 y 7)
	u32* p_ammo_matrix_6 = (u32*)piStack_f4;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_6);
	u32* p_widget_data_6 = (u32*)hud_get_widget_data_ptr((u32*)piStack_fc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_6, 0, (u32)p_widget_data_6);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_6, 0, 0xf0c070);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_6, 1, 0x55f0c070);
	p_ammo_matrix_6[2] = 0;
	p_ammo_matrix_6[3] = (u32)iVar25;
	u32* p_widget_vec_6 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_fc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_6, 1, (u32)p_widget_vec_6);
	inv_set_extended_ammo_slot_data(0x3F99999A, 0x3F99999A, 0, 0, p_ammo_matrix_6, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_6, 1, 1);
	p_hud_context[0x4AA] = 0;
	p_ammo_matrix_6[1] = (u32)iVar25;
	u32* p_widget_pos_6 = (u32*)hud_get_widget_position_vector_ptr((u32*)piStack_fc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_6, 2, (u32)p_widget_pos_6);
	inv_set_extended_ammo_slot_data(0x44008000, 0x40800000, 0, 0, p_ammo_matrix_6, 2, 0); // 514.0f y 4.0f
	inv_set_extended_ammo_slot_data(0x3F666666, 0x41200000, 0, 0, p_ammo_matrix_6, 2, 1);

	inv_reset_extended_ammo_subsystem((u32*)piStack_dc);
	u32* p_widget_data_7 = (u32*)hud_get_widget_data_ptr((u32*)piStack_ec);
	inv_set_ammo_matrix_group_ptr((u32*)piStack_dc, 0, (u32)p_widget_data_7);
	inv_set_ammo_matrix_group_state((u32*)piStack_dc, 0, 0xf0f0f0);
	inv_set_ammo_matrix_group_state((u32*)piStack_dc, 1, 0x80f0f0f0);
	((u32*)piStack_dc)[3] = (u32)iVar25;
	((u32*)piStack_dc)[2] = 0;

	inv_reset_extended_ammo_subsystem((u32*)piStack_c4);
	u32* p_widget_data_8 = (u32*)hud_get_widget_data_ptr((u32*)piStack_d8);
	inv_set_ammo_matrix_group_ptr((u32*)piStack_c4, 0, (u32)p_widget_data_8);
	inv_set_ammo_matrix_group_state((u32*)piStack_c4, 0, 0xf0f0b0);
	inv_set_ammo_matrix_group_state((u32*)piStack_c4, 1, 0x60f0f0b0);
	((u32*)piStack_c4)[3] = (u32)iVar25;
	((u32*)piStack_c4)[2] = 0;

	// 18. ASIGNACIÓN DE LAS CAPAS DE INVENTARIO ACTIVAS PRINCIPALES
	u32* p_main_inv_1 = (u32*)piStack_118;
	inv_reset_weapon_inventory(p_main_inv_1);
	inv_set_weapon_inventory_mode(p_main_inv_1, 2);
	u32* p_main_data_1 = (u32*)hud_get_widget_data_ptr((u32*)p_hud_context);
	inv_set_weapon_inventory_transition_flag(p_main_inv_1, (u32)p_main_data_1);
	inv_set_active_weapon_slot(p_main_inv_1, (u32)piStack_11c);
	inv_set_animation_factor(p_main_inv_1, 0.005f);
	inv_set_weapon_slot_data(0, 0x70202080, 0, 0, 0, p_main_inv_1, 0);
	inv_set_weapon_slot_data(0x3F800000, 0x70242335, 0, 0, 0, p_main_inv_1, 1);
	inv_set_quick_select_open_state(p_main_inv_1, 2);
	inv_set_weapon_inventory_visibility(p_main_inv_1, 1);
	inv_swap_animation_lock(p_main_inv_1, 0);

	u32* p_main_inv_2 = (u32*)piStack_104;
	inv_reset_weapon_inventory(p_main_inv_2);
	inv_set_weapon_inventory_mode(p_main_inv_2, 2);
	u32* p_main_data_2 = (u32*)hud_get_widget_data_ptr((u32*)piStack_cc);
	inv_set_weapon_inventory_transition_flag(p_main_inv_2, (u32)p_main_data_2);
	inv_set_active_weapon_slot(p_main_inv_2, (u32)piStack_11c);
	inv_set_animation_factor(p_main_inv_2, 0.005f);
	inv_set_weapon_slot_data(0, 0x606060c0, 0, 0, 0, p_main_inv_2, 0);
	inv_set_weapon_slot_data(0x3F800000, 0x60424162, 0, 0, 0, p_main_inv_2, 1);
	inv_set_quick_select_open_state(p_main_inv_2, 2);
	inv_set_weapon_inventory_visibility(p_main_inv_2, 1);
	inv_swap_animation_lock(p_main_inv_2, 0);

	// 19. RELLENO DE FLAGS GLOBALES DE TRANSICIÓN DEL EMOTION ENGINE
	p_hud_context[0xB7] = 0x001A7A80; // Dirección de tabla estática estricta
	p_hud_context[0x561] = 0;
	p_hud_context[0x563] = 0;
	p_hud_context[0x565] = 0;
	p_hud_context[0x566] = -1; // Fuerza la carga limpia sin valores basura
	p_hud_context[0x562] = -1;
	p_hud_context[0x564] = -1;

	// 20. FUERZA LA ACTUALIZACIÓN REGIONAL EN PÍXELES DEL CANVASES
	hud_update_layout_aspect_ratio(p_hud_context, 0);

	// 21. GUARDA EL PARÁMETRO DE GIRO DINÁMICO DEL ANILLO QUICK SELECT
	s32 remaining_slots_count = inv_get_quick_select_remaining_space();
	p_hud_context[0x56A] = remaining_slots_count;

}

/**
 * @brief Busca un identificador específico dentro de una estructura de tabla indexada.
 * Operación matemática en un arreglo con saltos de 8 bytes (Estructura de pares Clave/Valor).
 * Dirección original en Ghidra: 0x00338AA8 (PAL)
 *
 * @param table_ptr Puntero a la estructura de la tabla base.
 * @param target_id ID o clave que estamos buscando.
 * @return s32 El valor asociado al ID encontrado, o 0 si no existe/se sale de los límites.
 */
s32 game_lookup_id_in_table(u8* table_ptr, s32 target_id) {
	s32 index = 0;

	// El offset +0x18 almacena la cantidad máxima de elementos válidos en la tabla
	s32 total_elements = *(s32*)(table_ptr + 0x18);

	if (0 < total_elements) {
		index = 1;

		// Optimización del motor: Comprobar directamente el primer elemento (+0x1c)
		if (*(s32*)(table_ptr + 0x1c) == target_id) {
			index = *(s32*)(table_ptr + 0x20); // Devuelve el valor asociado en +0x20
		}
		else {
			// Bucle de búsqueda lineal (do-while)
			do {
				if (*(s32*)(table_ptr + 0x18) <= index) {
					return 0; // Fuera de los límites de la tabla, no encontrado
				}

				// Estructura de par Clave-Valor de 8 bytes: 4 bytes para ID, 4 bytes para Datos
				s32* pair_ptr = (s32*)(index * 8 + (table_ptr + 0x1c));
				index++;

				if (*pair_ptr == target_id) {
					return pair_ptr[1]; // Devuelve el valor contiguo en memoria
				}
			} while (1);
		}
	}

	return index;
}

/**
 * @brief Obtiene un valor de configuración mediante búsqueda e inicializa el campo del HUD.
 * Dirección original en Ghidra: 0x00338070 (PAL)
 */
void hud_set_state_from_lookup(u8* p_hudState, u8* table_ptr, s32 target_id) {
	// Realiza la búsqueda en la tabla lógica
	s32 result_value = game_lookup_id_in_table(table_ptr, target_id);

	// Almacena el resultado en el offset de configuración +0x40 del componente del HUD
	*(s32*)(p_hudState + 0x40) = result_value;
}

/**
 * @brief Asigna cuatro valores de 32 bits de forma consecutiva en una estructura de datos.
 * Comportamiento estándar para configurar vectores espaciales (X, Y, Z, W) o colores (R, G, B, A).
 * Dirección original en Ghidra: 0x00337B18 (PAL)
 *
 * @param val1 Primer componente (ej. coordenada X o canal Rojo)
 * @param val2 Segundo componente (ej. coordenada Y o canal Verde)
 * @param val3 Tercer componente (ej. coordenada Z o canal Azul)
 * @param val4 Cuarto componente (ej. coordenada W o canal Alfa/Transparencia)
 * @param p_targetDestination Puntero que contiene la dirección de la estructura destino.
 */
void math_set_vector4(u32 val1, u32 val2, u32 val3, u32 val4, u32* p_targetDestination) {
	// Obtiene la dirección base real del objeto destino
	u32 base_address = *p_targetDestination;

	// Almacena los 4 componentes de manera contigua en la memoria de la PS2 (saltos de 4 bytes)
	*(u32*)(base_address + 0x0) = val1;
	*(u32*)(base_address + 0x4) = val2;
	*(u32*)(base_address + 0x8) = val3;
	*(u32*)(base_address + 0xC) = val4;
}

/**
 * @brief Rellena un bloque de memoria con un valor específico (Memset).
 * Versión funcional simplificada de la rutina optimizada para los registros multimedia de la PS2.
 * Dirección original en Ghidra: 0x00115484 (PAL)
 *
 * @param dest Puntero al bloque de memoria a rellenar.
 * @param value Valor de byte con el que se va a rellenar.
 * @param size Cantidad de bytes a escribir.
 * @return void* Puntero a la memoria de destino.
 */
void* ee_memset(void* dest, u8 value, u32 size) {
	u8* ptr = (u8*)dest;

	// En la PS2 real, aquí se ejecuta un bucle optimizado de 32 y 8 bytes
	// usando instrucciones vectoriales si el puntero está alineado a 16 bytes.
	// Para efectos funcionales, el comportamiento exacto es:
	for (u32 i = 0; i < size; i++) {
		ptr[i] = value;
	}

	return dest;
}

/**
 * @brief Controla la visibilidad o factor de escala de un componente del HUD.
 * Escribe 1.0f (0x3f800000) o 0.0f en la propiedad de transformación del elemento.
 * Dirección original en Ghidra: 0x00337B48 (PAL)
 *
 * @param p_widget Destino del componente visual (param_1 / registro a0)
 * @param enable Estado booleano para activar o desactivar (param_2 / registro a1)
 */
void hud_set_widget_visibility(u32* p_widget, long enable) {
	// El offset +0x10 (16 bytes) contiene un puntero a la propiedad flotante (ej. Opacidad/Alpha o Escala)
	f32** p_target_property = (f32**)((u8*)p_widget + 0x10);

	if (enable != 0) {
		// En la PS2 escribe 0x3f800000, lo que equivale a 1.0f (Visibilidad/Escala Máxima)
		**p_target_property = 1.0f;
		return;
	}

	// Si es falso, escribe 0.0f (Completamente oculto/Desactivado)
	**p_target_property = 0.0f;
	return;
}

/**
 * @brief Función stub de paso directo de puntero (Identity Function).
 * Utilizada originalmente en el motor para macros de validación o abstracción de nodos.
 * Dirección original en Ghidra: 0x00338B00 (PAL)
 *
 * @param param_1 Primer parámetro (omitido en el retorno)
 * @param p_node Puntero de nodo secundario que es devuelto de forma directa (param_2)
 * @return void* El mismo puntero recibido en param_2.
 */
void* core_identity_stub(long param_1, void* p_node) {
	// El descompilador de Ghidra demuestra que la PS2 simplemente mueve el registro de entrada
	// al registro de salida de inmediato (move $v0, $a1).
	return p_node;
}
