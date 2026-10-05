#include "types.h"
#include <stdarg.h>
#include <stdio.h>
#include <time.h> // Requerido para estructuras timespec en sistemas modernos
#include <assert.h>
#include <stdbool.h>
#include "ps2_kernel.h"

// Definiciones de los offsets estáticos de audio e interrupciones en la RAM de la PS2
#define IO_WAIT_SEMA_ID             (*(s32*)0x0013642C)
#define CURRENT_AUDIO_CMD_ID        (*(s32*)0x00136418)
#define AUDIO_SESSION_STATUS_FLAG   (*(s32*)0x00136448)
#define DEBUG_NET_LOG_LEVEL         (*(s32*)0x00136410)

// Definición de las variables globales de transición de etapa en la RAM de la PS2
#define BOOT_INTRO_DELAY_STATE       (*(s32*)0x001A642C)
#define BOOT_INTRO_TRANSITION_FLAG   (*(u8*)0x001A642E)
#define NEXT_GAME_STAGE_CALLBACK     (*(void**)(long)0x001A6440)
#define NEXT_GAME_STAGE_ARGUMENT     (*(u32*)0x001A6444)

// Definición de las variables de estado de la Memory Card en la RAM de la PS2
#define MC_ACTIVE_COMMAND_ID        (*(s32*)0x00137EE8)
#define MC_CHANNEL_WIDGET_HANDLE    (*(u32*)0x00141B80)
#define MC_RESULT_METADATA_VAL      (*(u32*)0x00143140)

// Definición de las variables globales de la versión de controladores de la Memory Card en la PS2
#define MC_MUTEX_SEMA_ID            (*(s32*)0x00137EEC)
#define MC_IS_BOUND_FLAG            (*(s32*)0x00141BA4)
#define MC_MCSERV_VERSION           (*(u32*)0x00143144)
#define MC_MCMAN_VERSION            (*(u32*)0x00143148)

// Definiciones de los registros físicos y buffers globales mapeados de la Memory Card
#define MC_SLOT_INPUT_BUFFER_PTR    (*(u32*)0x00141C00)

// Definición de las variables de descriptor de apertura mapeadas en la RAM de la PS2
#define MC_OPEN_PATH_PTR            (*(u32*)0x00141C10)
#define MC_OPEN_FLAGS_MASK          (*(u32*)0x00141C14)

// Referencias a tus tablas globales de callbacks externas ya definidas en el repositorio
extern u32 g_sys_sif_general_callback_table;
extern u32 g_sys_sif_system_callback_table;

// Referencias a tus helpers e infraestructura consolidados de ps2_kernel.c y el arranque
s32  scePollSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);

// Asegúrate de que arriba en tus prototipos o cabeceras esté declarada exactamente así:
bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

extern s32 g_sys_mc_is_bound_flag;
extern s32 g_sys_mc_mutex_sema_id;
extern s32 g_sys_mc_active_command_id;
extern u32 g_sys_mc_channel_widget_handle;

/**
 * @brief Envía el comando de apertura de un archivo en la Memory Card (Comando 4) al bus de hardware.
 * Configura la ruta, ranura y banderas binarias de control aplicando exclusión mutua por semáforos.
 * Dirección original en Ghidra: 0x00127720 (PAL)
 *
 * @param slot_index Ranura de la tarjeta a interrogar (0 = Slot 1, 1 = Slot 2) (param_1).
 * @param p_file_path Cadena de texto con la ruta formateada del archivo de la partida (param_2).
 * @param open_flags Máscara de bits con los modos de acceso de lectura/escritura del SDK (param_3).
 * @return s32 Código de estado (0 para comando aceptado en el bus, valores negativos para error).
 */
s32 sceMcOpen(u32 slot_index, const char* p_file_path, u32 open_flags) {
	s32 status_code;

	// 1. Validar que el subsistema de la Memory Card esté formalmente levantado
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protege el bus realizando un sondeo no bloqueante sobre el semáforo del canal
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	if (sema_status < 0) {
		return -200;
	}

	// 3. Vuelca en ráfaga contigua los parámetros de apertura en la sección de datos estáticos
	extern u32 g_sys_mc_slot_input_buffer_val;
	*(u32*)0x00141C00 = slot_index; // Reutiliza el buffer del ID del slot
	MC_OPEN_PATH_PTR = (u32)(long)p_file_path;
	MC_OPEN_FLAGS_MASK = open_flags;

	// Despacha la orden mediante la ráfaga Comando 4 (Síncrona prioritaria = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		4, 1, 0x141C00, 0x30, 0x143140, 4, 0, 0
	);

	// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 4;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}


// Referencias a tus helpers e infraestructura perfectamente consolidados
s32  scePollSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);

// Variables globales externas provenientes de tu módulo de arranque
extern s32 g_sys_mc_is_bound_flag;
extern s32 g_sys_mc_mutex_sema_id;
extern s32 g_sys_mc_active_command_id;
extern u32 g_sys_mc_channel_widget_handle;

// Referencias a tus componentes e infraestructura del Kernel y HUD perfectamente entrelazados
s32  sceCreateSema(void);
s32  sceWaitSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
u32  sys_mc_io_sync_command_guard(long command_type, long p_out_cmd_ptr, long p_out_meta_ptr);
s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr, long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

extern s32 g_sys_mc_result_metadata_val;

/**
 * @brief Inicializa por completo el subsistema físico de la Memory Card y valida las versiones de los módulos IRX del IOP.
 * Abre la sesión de control 0x80000400 y aplica barreras de exclusión mutua para asegurar la lectora.
 * Dirección original en Ghidra: 0x00127348 (PAL)
 *
 * @return s32 Código de estado general del sistema de guardado (0 o metadatos para éxito, valores negativos para error).
 */
s32 sys_mc_init_subsystem(void) {
	s32 status_code;

	// 1. Inicializa y reserva el semáforo de exclusión mutua para las ranuras de la tarjeta
	if (MC_MUTEX_SEMA_ID < 0) {
		MC_MUTEX_SEMA_ID = sceCreateSema();
	}

	// Asegura que el canal físico esté libre e inactivo antes de reconfigurar
	sys_mc_io_sync_command_guard(0, 0, 0);

#if defined(PLATFORM_PS2)
	sceWaitSema(MC_MUTEX_SEMA_ID);
#endif

	// Inicializa el entorno del cliente RPC
	sys_sif_rpc_init_client();

	// 2. LAZO DE ESPERA DE ENLACE: Abre la sesión de control de la tarjeta (Comando 0x80000400)
	extern u32 g_sys_mc_channel_widget_handle;
	while (1) {
		while (1) {
			s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_mc_channel_widget_handle, 0x80000400, 0);

			if (session_status >= 0) {
				break;
			}

			// Si falla el enlace por hardware con el IOP, lanza el log de pánico
			boot_txt_render_extended_string((const u8*)"bind error libmc \n", 0, 0, 0, 0, 0, 0, 0);

			// Lazo infinito de seguridad en la consola original en caso de error grave de hardware
#if defined(PLATFORM_PS2)
			while (1) {}
#else
			return -1; // Escape seguro para el entorno portable en PC
#endif
		}

		if (MC_IS_BOUND_FLAG != 0) {
			break;
		}

		s32 delay_counter = 0x100000;
		while (delay_counter != 0) { delay_counter--; }
	}

	// 3. CONSULTA DE INTEGRIDAD: Solicita las versiones de los controladores de guardado al IOP (Comando 0xFE)
	s32 transaction_status = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		0xFE, 0, 0x141C00, 0x30, 0x143140, 0x0C, 0, 0
	);

#if defined(PLATFORM_PS2)
	sceSignalSema(MC_MUTEX_SEMA_ID);
#endif

	if (transaction_status < 0) {
		MC_IS_BOUND_FLAG = 0;
		status_code = transaction_status - 100;
	}
	// 4. VALIDACIÓN DE VERSIONES DEL COMPILADOR DE SONY (Safety Checks)
	else if (MC_MCSERV_VERSION < 0x20A) {
		boot_txt_render_extended_string((const u8*)"libmc: too old release of mcserv.irx\n", 0, 0, 0, 0, 0, 0, 0);
		MC_IS_BOUND_FLAG = 0;
		status_code = -0x78;
	}
	else {
		status_code = g_sys_mc_result_metadata_val;
		if (MC_MCMAN_VERSION < 0x20E) {
			boot_txt_render_extended_string((const u8*)"libmc: too old release of mcman.irx\n", 0, 0, 0, 0, 0, 0, 0);
			MC_IS_BOUND_FLAG = 0;
			status_code = -0x79;
		}
	}

	// Para el port de PC moderno, sobreescribimos con éxito forzado para habilitar 
	// de largo la escritura local de archivos directos sin depender de los módulos de la PS2:
#if !defined(PLATFORM_PS2)
	MC_IS_BOUND_FLAG = 1;
	status_code = 0;
#endif

	return status_code;
}

// Referencias a tus helpers e infraestructura del Kernel ya consolidados
s32  hud_validate_widget_node_state(const u32* p_widget_handle);
void mc_io_wait_delay_ms(void);
s32  sceSignalSema(s32 sema_id);

// Referencia a tu variable global del semáforo de espera de ps2_kernel.c
extern s32 g_sys_io_wait_sema_id;

/**
 * @brief Custodia la sincronización de las operaciones I/O (Lectura/Escritura) de la Memory Card.
 * Bloquea el hilo de ejecución mediante mc_io_wait_delay_ms mientras el hardware reporte actividad.
 * Dirección original en Ghidra: 0x00127B88 (PAL)
 */
u32 sys_mc_io_sync_command_guard(long command_type, long p_out_cmd_ptr, long p_out_meta_ptr) {
	s32 is_mc_node_active;
	u32 query_status;

	// 1. Control de inicialización o estado inactivo de la lectora
	if (MC_ACTIVE_COMMAND_ID == 0) {
		return 0xFFFFFFFF;
	}

	is_mc_node_active = hud_validate_widget_node_state(&MC_CHANNEL_WIDGET_HANDLE);

	// 2. Bucle de bloqueo: Congela la CPU de forma segura si el bus I/O de la tarjeta está ocupado
	if (command_type == 0 && is_mc_node_active != 0) {
		while (1) {
			s32 is_busy = hud_validate_widget_node_state(&MC_CHANNEL_WIDGET_HANDLE);
			is_mc_node_active = 0;
			if (is_busy == 0) {
				break;
			}
			// Invoca a tu función de retardo por hardware del paso anterior
			mc_io_wait_delay_ms();
		}
	}

	// Calcula el estado lógico resultante de la ocupación del canal
	query_status = (u32)(is_mc_node_active == 0);

	if (p_out_cmd_ptr != 0) {
		*(s32*)p_out_cmd_ptr = MC_ACTIVE_COMMAND_ID;
	}

	// 3. Si la transacción finalizó, libera la exclusión mutua y vuelca los metadatos
	if (query_status != 0) {
		MC_ACTIVE_COMMAND_ID = 0;

		if (p_out_meta_ptr != 0) {
			*(u32*)p_out_meta_ptr = MC_RESULT_METADATA_VAL;
		}

		// Libera atómicamente el semáforo del Kernel pasándole el ID global de espera
		sceSignalSema(g_sys_io_wait_sema_id);
	}

	return query_status;
}

// Referencias a tus funciones maestras ya portadas
u32  sys_sound_dispatch_iop_query_filter(void);

// Referencias a tus funciones del Kernel ya integradas de forma portable en ps2_kernel.c
s32 sceGetThreadId(void);
s32 sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg);
s32 sceSleepThread(void);

/**
 * @brief Pausa el hilo de ejecución activo de la Memory Card configurando una alarma del Kernel y durmiendo la CPU.
 * Utilizado por el motor para dar márgenes físicos de estabilización de datos durante operaciones I/O.
 * Dirección original en Ghidra: 0x00127B40 (PAL)
 */
void mc_io_wait_delay_ms(void) {
	// 1. Consulta el identificador del hilo actual propietario de la operación
	sceGetThreadId();

	// 2. Programa la alarma de hardware e introduce el hilo lúdico en modo suspensión voluntario.
	// Para el port nativo moderno de PC, ambas llamadas resuelven de largo de forma pasiva y fluida
	// debido a que las lecturas en discos modernos (SSD/NVMe) ocurren en nanosegundos:
	sceSetAlarm(1000, NULL, NULL); // Retardo por hardware de seguridad simulado (1000 ms/1 s máximo)
	sceSleepThread();
}

/**
 * @brief Máquina de estados raíz encargada de coordinar la transición de la intro al menú jugable principal.
 * Monitorea el estatus del IOP de sonido y ejecuta de forma atómica el callback de la siguiente etapa lúdica.
 * Dirección original en Ghidra: 0x002B8940 (PAL)
 *
 * @param execution_stage Parámetro de control que define la fase del lazo (param_1).
 */
void sys_boot_intro_state_machine(s32 execution_stage) {
	// 1. Evalúa si la fase activa requiere el sondeo de preparación del software
	if (execution_stage == 1) {

		// Interroga si el hardware de sonido e IOP terminaron la carga asíncrona
		s32 audio_status = (s32)sys_sound_dispatch_iop_query_filter();

		if (audio_status == 0) {
			// Limpia la caché de instrucciones antes de dar el salto atómico
			sceFlushCache(0);

			// Respalda en variables locales los descriptores de la siguiente etapa lúdica
			u32 stage_arg = NEXT_GAME_STAGE_ARGUMENT;
			void (*p_stage_callback)(u32, bool) = (void (*)(u32, bool))NEXT_GAME_STAGE_CALLBACK;

			// Resetea las banderas globales del arranque para limpiar la RAM
			BOOT_INTRO_DELAY_STATE = 0;
			bool is_clean_boot = (BOOT_INTRO_TRANSITION_FLAG == '\0');
			BOOT_INTRO_TRANSITION_FLAG = '\0';

			// 2. DISPARADOR MAESTRO DEL JUEGO: Si hay una subrutina enlazada, salta a ella de inmediato
			if (p_stage_callback != NULL) {
				NEXT_GAME_STAGE_ARGUMENT = 0;
				NEXT_GAME_STAGE_CALLBACK = NULL;

				// Redirecciona el hilo de ejecución nativo hacia la pantalla de menú principal / gameplay
				p_stage_callback(stage_arg, is_clean_boot);
			}
		}
		else {
			// Si el bus de audio sigue ocupado cargando pistas, configura un estado de espera activa
			BOOT_INTRO_DELAY_STATE = 2;
		}
	}
}

// Referencias a tus componentes del repositorio perfectamente entrelazados
void sys_io_init_kernel_semaphores(void);
s32  sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8);
s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

// Stubs de soporte adicionales del SDK de la PS2 emulados de forma portable
s32  scePollSema(s32 sema_id);
//void sceSignalSema(s32 sema_id);

// Definiciones de los offsets estáticos de audio e interrupciones en la RAM de la PS2
#define IO_WAIT_SEMA_ID             (*(s32*)0x0013642C)
#define CURRENT_AUDIO_CMD_ID        (*(s32*)0x00136418)
#define AUDIO_SESSION_STATUS_FLAG   (*(s32*)0x00136448)
#define DEBUG_NET_LOG_LEVEL         (*(s32*)0x00136410)
#define AUDIO_HARDWARE_READY_FLAG   (*(s32*)0x00137E6C)

// Referencias a tus componentes de bajo nivel e infraestructura del Kernel
void sys_io_init_kernel_semaphores(void);
s32  scePollSema(s32 sema_id);
s32  sceSignalSema(s32 sema_id);
s32  sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8);
s32  sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags);
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);

// Referencias a tus componentes del repositorio perfectamente entrelazados
u32  hud_allocate_linear_node_slot(s32* p_master_alloc_struct);
void sys_kernel_flush_dcache_range(u32 start_addr, long block_size);
void hud_disable_widget_node(u32* p_internal_node);
u64  sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Stubs oficiales de APIs del Kernel de Sony emulados en ps2_kernel.c
s32 sceCreateSema(void);
s32 sceWaitSema(s32 sema_id);
s32 sceDeleteSema(s32 sema_id);

// Dirección física de destino del buffer de audio en la RAM de la PS2
#define SOUND_IOP_STATUS_BUFFER_PTR    ((void*)0x00137600)

// Definición de la variable de reserva de estado de audio en la RAM de la PS2
#define SIF_SOUND_BACKUP_IOP_STATUS     (*(u32*)0x001A7190)

// Referencia a tu flag global de candado de la cola IO externa
extern s32 g_sys_io_queue_lock_flag;

// Referencia a tu función de sondeo de audio ya integrada
s32 sys_sound_query_iop_status(void);

/**
 * @brief Filtra e intercepta las solicitudes de sondeo del chip de sonido evaluando el candado de la cola general.
 * Retorna de inmediato el estado previo de reserva si el bus de hardware se encuentra congestionado.
 * Dirección original en Ghidra: 0x001336E8 (PAL)
 *
 * @return u32 Código de estado lúdico de sincronización de audio (o buffer de respaldo).
 */
u32 sys_sound_dispatch_iop_query_filter(void) {
	u32 current_status = SIF_SOUND_BACKUP_IOP_STATUS;

	// Si el candado global de la cola está libre, despacha la consulta de audio de inmediato
	if (g_sys_io_queue_lock_flag == 0) {
		current_status = (u32)sys_sound_query_iop_status();
	}

	return current_status;
}

// Referencias a tus componentes e infraestructura del Kernel ya consolidados
s32  sys_sound_init_audio_stream_session(long current_cmd_param);
s32  sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr,
	long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg);
s32  sceSignalSema(s32 sema_id);

// Referencia a tu variable global del semáforo de espera IO de ps2_kernel.c
extern s32 g_sys_io_wait_sema_id;

/**
 * @brief Sondea el estado del firmware de audio del coprocesador IOP ejecutando la transacción RPC Comando 4.
 * Inicializa la sesión del stream bajo el token 3 y libera de forma atómica el semáforo de exclusión mutua.
 * Dirección original en Ghidra: 0x00125588 (PAL)
 *
 * @return s32 Código de estado general (0 para éxito absoluto, 0xFFFFFFFF para fallas en el bus).
 */
s32 sys_sound_query_iop_status(void) {
	// 1. Despierta e inicializa el canal de streaming bajo el ID de comando de audio 3
	s32 session_init_status = sys_sound_init_audio_stream_session(3);
	s32 final_status_code;

	if (session_init_status == 0) {
		final_status_code = 0xFFFFFFFF; // Error: No se pudo levantar el lazo de audio
	}
	else {
		// 2. Despacha la transacción Comando 4 interrogando el buffer de hardware 0x137600 (Tamaño 4 bytes)
		extern u32 g_sys_sound_channel_widget_handle;
		s32 transaction_status = sys_sif_rpc_send_transaction_data(
			&g_sys_sound_channel_widget_handle,
			4, 0, 0, 0,
			(long)SOUND_IOP_STATUS_BUFFER_PTR,
			4, 0, 0
		);

		// 3. Libera atómicamente el semáforo del Kernel pasándole el ID global de espera
		if (transaction_status < 0) {
			sceSignalSema(g_sys_io_wait_sema_id);
			final_status_code = 0xFFFFFFFF; // Error: Falla crítica de transmisión en el bus SIF
		}
		else {
			final_status_code = 0; // Sincronización de audio e IOP completada con éxito
			sceSignalSema(g_sys_io_wait_sema_id);
		}
	}

	return final_status_code;
}

/**
 * @brief Envía, sincroniza y gestiona una ráfaga extendida de transferencia de datos a través de transacciones SIF RPC.
 * Aplica barreras dcache de coherencia física y orquesta la suspensión del hilo de control mediante semáforos del Kernel.
 * Dirección original en Ghidra: 0x0011D620 (PAL)
 */
s32 sys_sif_rpc_send_transaction_data(u32* p_session_handle, u32 command_id, u64 sync_flags, long src_addr,
	long src_size, long dest_addr, long dest_size, long p8, u32 extra_arg) {
	if (p_session_handle == NULL) {
		return -1;
	}

	// Definimos una estructura simulada para que el compilador entienda el tipo
	typedef struct {
		int dummy[16]; // Búfer de relleno para compatibilidad de tamaño
	} SifRpcClientData;

	// Tu código original ahora compilará perfectamente:
	extern SifRpcClientData g_sys_sif_rpc_client_struct;
	u32* p_node_slot = (u32*)hud_allocate_linear_node_slot((s32*)&g_sys_sif_rpc_client_struct);

	if (p_node_slot == NULL) {
		return 0xFFFFFFFF; // Error: Buffer lleno
	}

	u32 structural_signature = p_node_slot[0x06]; // Offset 0x18 de validación
	p_session_handle[8] = extra_arg;
	*p_session_handle = (u32)(long)p_node_slot;
	p_session_handle[1] = structural_signature;
	p_session_handle[7] = (s32)p8;

	p_node_slot[8] = command_id;
	p_node_slot[9] = (u32)src_size;
	p_node_slot[10] = (u32)dest_addr;
	p_node_slot[11] = (u32)dest_size;
	p_node_slot[5] = (u32)(long)p_node_slot;

	u32 callback_val = p_session_handle[9];
	p_node_slot[7] = (u32)(long)p_session_handle;
	p_node_slot[13] = callback_val;

	u32 u_src_addr = (u32)src_addr;

	// 2. BARRERAS DE COHERENCIA DE CACHÉ DE LA CPU (FLUSH D-CACHE)
	if ((sync_flags & 2) == 0) {
		if (src_addr == dest_addr) {
			long final_size = dest_size;
			if (dest_size <= src_size) {
				final_size = src_size;
			}
			sys_kernel_flush_dcache_range(u_src_addr, final_size);
		}
		else {
			if (src_size > 0) {
				sys_kernel_flush_dcache_range(u_src_addr, src_size);
			}
			if (dest_size > 0) {
				sys_kernel_flush_dcache_range((u32)dest_addr, dest_size);
			}
		}
	}

	s32 status_code = 0xFFFFFFFF;

	// 3. CASO A: Transmisión síncrona obligatoria con semáforos del Kernel
	if ((sync_flags & 1) == 0) {
		s32 sema_id = sceCreateSema();
		p_session_handle[2] = sema_id;

		if (sema_id < 0) {
			hud_disable_widget_node(p_node_slot);
			return 0xFFFFFFFD;
		}

		p_node_slot[12] = 1; // Bandera de estado activo

		// Envía el comando asíncrono de transferencia de bloques (0x8000000a)
		long dma_status = (long)sys_sif_submit_dma_packet_simple(0x8000000A, p_node_slot, 0x40, u_src_addr, p_session_handle[5], src_size);

		if (dma_status != 0) {
#if defined(PLATFORM_PS2)
			sceWaitSema(sema_id);
			sceDeleteSema(sema_id);
#endif
			return 0; // Éxito en la transacción síncrona
		}

#if defined(PLATFORM_PS2)
		sceDeleteSema(sema_id);
#endif
	}
	// 4. CASO B: Despacho asíncrono libre (Fire and Forget)
	else {
		if (p8 == 0) {
			p_node_slot[12] = 0;
		}
		else {
			p_node_slot[12] = 1;
		}
		p_session_handle[2] = 0xFFFFFFFF;

		long dma_status = (long)sys_sif_submit_dma_packet_simple(0x8000000A, p_node_slot, 0x40, u_src_addr, p_session_handle[5], src_size);
		if (dma_status != 0) {
			return 0;
		}
	}

	// Control de escape si la cola de descriptores experimentó fallas de hardware
	hud_disable_widget_node(p_node_slot);
	return 0xFFFFFFFE;
}

/**
 * @brief Inicializa la sesión de streaming y orquesta el enlace atómico del bus de audio para la intro y menús.
 * Interroga los guardianes de sincronización y abre de forma segura la transacción RPC 0x80000593.
 * Dirección original en Ghidra: 0x00124C98 (PAL)
 *
 * @param current_cmd_param Identificador del comando u orden lúdica de audio a despachar (param_1).
 * @return s32 Código de estado (1 para éxito, 0 para falla o colisión de hilos).
 */
s32 sys_sound_init_audio_stream_session(long current_cmd_param) {
	// 1. Asegura que los semáforos base de E/S estén dados de alta en el Kernel de Sony
	sys_io_init_kernel_semaphores();

	// Sondeo de estado no bloqueante sobre el semáforo de espera IO
	s32 current_kernel_sema = scePollSema(IO_WAIT_SEMA_ID);

	// 2. Si el ID coincide, el hilo toma de forma segura el control de la sesión de audio
	if (IO_WAIT_SEMA_ID == current_kernel_sema) {
		CURRENT_AUDIO_CMD_ID = (s32)current_cmd_param;

		// Si el juego original solo consulta de forma pasiva el estado del hilo de audio:
		sceReferThreadStatus(1, NULL);

		// Consulta al guardián del canal S (Sonido) si el bus físico está disponible
		s32 is_sound_busy = sys_sound_sync_command_guard(1, 0, 0, 0, 0, 0, 0, 0);

		if (is_sound_busy == 0) {
			// Inicializa de forma segura el entorno del cliente RPC
			sys_sif_rpc_init_client();

			if (AUDIO_SESSION_STATUS_FLAG > -1) {
				return 1;
			}

			// 3. LAZO DE ESPERA ACTIVA: Abre la sesión de audio asíncrona del motor (Comando 0x80000593)
			extern u32 g_sys_sound_channel_widget_handle;
			while (1) {
				while (1) {
					s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_sound_channel_widget_handle, 0x80000593, 0);

					if (session_status >= 0) {
						break;
					}

					// Si ocurre un error de hardware en el bus, imprime la alerta con tu interceptor tipográfico
					if (DEBUG_NET_LOG_LEVEL > 0) {
						boot_txt_render_extended_string((const u8*)"Libcdvd bind err S cmd\n", 0, 0, 0, 0, 0, 0, 0);
					}

					// Retardo fino de ciclos de espera física para reintentar la transacción en la consola
					s32 delay_counter = 0x100000;
					while (delay_counter != -1) { delay_counter--; }
				}

				// Condición de quiebre una vez establecido el puente de streaming con el firmware del IOP
				if (AUDIO_HARDWARE_READY_FLAG != 0) {
					break;
				}

				s32 delay_counter = 0x100000;
				while (delay_counter != -1) { delay_counter--; }
			}

			AUDIO_SESSION_STATUS_FLAG = 0;
			return 1;
		}

		// Si el guardián de audio reporta ocupación, libera el semáforo para no colgar el sistema
		sceSignalSema(IO_WAIT_SEMA_ID);
	}
	// Caso de error en el semáforo principal registrado por depuración de red
	else if (DEBUG_NET_LOG_LEVEL > 0) {
		boot_txt_render_extended_string((const u8*)"Scmd fail sema cur_cmd:%d keep_cmd:%d\n", current_cmd_param, (long)CURRENT_AUDIO_CMD_ID, 0, 0, 0, 0, 0);
		return 0;
	}

	return 0;
}

// Referencias a tus funciones del repositorio requeridas para el enlace
u32  hud_alloc_ring_buffer_node(u32* p_ring_struct);
s32* hud_find_node_by_id(s32 target_node_id, void* p_hud_container);
void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Referencias a tus componentes del repositorio requeridos para el enlace
u32  hud_alloc_ring_buffer_node(u32* p_ring_struct);
void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Referencia a tu manejador de interrupciones de teclado del paso anterior
void sys_hardware_keyboard_interrupt_handler(u64 expected_thread_id);

// Referencias a tus funciones de sincronización del Kernel ya unificadas
extern bool kernel_system_sync_guard(void);
extern void kernel_system_sync_release(void);

// Referencias a tus componentes del repositorio necesarios para la interconexión
u32  hud_allocate_linear_node_slot(s32* p_master_alloc_struct);
void hud_disable_widget_node(u32* p_internal_node);
u64  sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Stubs oficiales de APIs del Kernel de Sony emulados
s32 sceCreateSema(void);
s32 sceWaitSema(s32 sema_id);
s32 sceDeleteSema(s32 sema_id);

/**
 * @brief Abre y gestiona una sesión de comando asíncrona a través de transacciones SIF RPC.
 * Reserva una ranura lineal, orquesta la sincronización con semáforos del Kernel y despacha los metadatos.
 * Dirección original en Ghidra: 0x0011D450 (PAL)
 *
 * @param p_session_handle Puntero a la estructura local del manejador de la sesión (param_1).
 * @param command_id Identificador del comando u orden lúdica a enviar al IOP (param_2).
 * @param sync_flags Máscara de bits que determina si la espera será síncrona o de fondo (param_3).
 * @return s32 Código de estado (0 para éxito, valores negativos para errores de hardware).
 */
s32 sys_sif_rpc_open_transaction_session(u32* p_session_handle, u32 command_id, u64 sync_flags) {
	if (p_session_handle == NULL) {
		return -1;
	}

	// Inicializa las variables de estado en los desplazamientos contiguos del manejador
	p_session_handle[4] = 0; // Offset +16
	p_session_handle[9] = 0; // Offset +36

	// Definimos una estructura simulada para que el compilador entienda el tipo
	typedef struct {
		int dummy[16]; // Búfer de relleno para compatibilidad de tamaño
	} SifRpcClientData;

	// Tu código original ahora compilará perfectamente:
	extern SifRpcClientData g_sys_sif_rpc_client_struct;
	u32* p_node_slot = (u32*)hud_allocate_linear_node_slot((s32*)&g_sys_sif_rpc_client_struct);

	s32 status_code = 0xFFFFFFFF; // Error por defecto si el buffer está lleno

	if (p_node_slot != NULL) {
		u32 structural_signature = p_node_slot[6]; // Offset 0x18 de validación

		*p_session_handle = (u32)(long)p_node_slot; // Almacena dirección del nodo
		p_session_handle[1] = structural_signature;  // Guarda firma de control

		p_node_slot[8] = command_id;         // Inyecta el ID del comando en el paquete
		p_node_slot[5] = (u32)(long)p_node_slot; // Autodireccionamiento de la cabecera
		p_node_slot[7] = (u32)(long)p_session_handle;

		// Caso A: Espera síncrona obligatoria por hardware mediante Semáforos
		if ((sync_flags & 1) == 0) {
			s32 sema_id = sceCreateSema();
			p_session_handle[2] = sema_id;

			if (sema_id < 0) {
				hud_disable_widget_node(p_node_slot);
				status_code = 0xFFFFFFFD; // Error: Falló creación de semáforo
			}
			else {
				// Envía el comando asíncrono de apertura (0x80000009)
				u64 dma_status = sys_sif_submit_dma_packet_simple(0x80000009, p_node_slot, 0x40, 0, 0, 0);

				if (dma_status == 0) {
					hud_disable_widget_node(p_node_slot);
#if defined(PLATFORM_PS2)
					sceDeleteSema(sema_id);
#endif
					status_code = 0xFFFFFFFE; // Error: Falló el bus DMA
				}
				else {
					// Duerme pacíficamente la CPU esperando la respuesta del bus
#if defined(PLATFORM_PS2)
					sceWaitSema(sema_id);
					sceDeleteSema(sema_id);
#endif
					status_code = 0; // Transacción completada con éxito absoluto
				}
			}
		}
		// Caso B: Despacho asíncrono libre (Fire and Forget)
		else {
			p_session_handle[2] = 0xFFFFFFFF;
			u64 dma_status = sys_sif_submit_dma_packet_simple(0x80000009, p_node_slot, 0x40, 0, 0, 0);
			status_code = 0;

			if (dma_status == 0) {
				hud_disable_widget_node(p_node_slot);
				status_code = 0xFFFFFFFE;
			}
		}
	}

	return status_code;
}

/**
 * @brief Peina secuencialmente la memoria reservada del HUD buscando una ranura de nodo vacía de 64 bytes (0x40).
 * Si la halla, enciende sus flags de activación e inyecta las firmas de validación de forma segura.
 * Dirección original en Ghidra: 0x0011D140 (PAL)
 *
 * @param p_master_alloc_struct Puntero a la estructura global de control de la asignación (param_1).
 * @return u32 Dirección de memoria física de la ranura de widget reservada y configurada, o 0 si está lleno.
 */
u32 hud_allocate_linear_node_slot(s32* p_master_alloc_struct) {
	if (p_master_alloc_struct == NULL) {
		return 0;
	}

	bool interrupt_status = kernel_system_sync_guard();

	s32 current_index = 0;

	// Índice 1 (1 * 4 = 4 bytes) almacena el puntero base del array físico de nodos
	u32 p_node_array_base = (u32)p_master_alloc_struct[1];

	// Índice 2 (2 * 4 = 8 bytes) almacena el límite o capacidad máxima de ranuras del array
	s32 max_slots_limit = p_master_alloc_struct[2];

	if (max_slots_limit > 0) {
		u8* p_node_cursor = (u8*)(long)p_node_array_base;

		do {
			// Offset 0x10 (Índice 4 en u32) almacena las banderas de estado del widget
			u32* p_node_flags = (u32*)(p_node_cursor + 0x10);

			// Si el bit menos significativo está en cero, la ranura está completamente libre
			if ((*p_node_flags & 1) == 0) {

				// Activa el flag inyectando un 5 (bit de habilitación vivo) y resguarda el índice en la parte alta
				*p_node_flags = (u32)(current_index << 0x10) | 5;

				// Extrae, incrementa y actualiza el contador global maestro de firmas únicas
				s32 old_signature = *p_master_alloc_struct;
				s32 new_signature = old_signature + 1;
				*p_master_alloc_struct = new_signature;

				// Regla de salvaguarda del motor: si la firma llega a 1, la desplaza forzosamente a 2
				if (new_signature == 1) {
					new_signature = old_signature + 2;
					*p_master_alloc_struct = new_signature;
				}

				// Offset 0x14 (Índice 5 en u32) apunta a la propia dirección base del widget
				*(u32*)(p_node_cursor + 0x14) = (u32)(long)p_node_cursor;

				// Offset 0x18 (Índice 6 en u32) almacena la firma de validación única calculada
				*(s32*)(p_node_cursor + 0x18) = new_signature;

				if (interrupt_status) {
					kernel_system_sync_release();
				}

				return (u32)(long)p_node_cursor; // Retorna la ranura asignada lista para usar
			}

			current_index++;
			p_node_cursor += 0x40; // Desplazamiento exacto al siguiente nodo hermano (64 bytes)
		} while (current_index < max_slots_limit);
	}

	if (interrupt_status) {
		kernel_system_sync_release();
	}

	return 0; // El búfer dinámico está completamente lleno
}

/**
 * @brief Callback de interrupción SIF RPC que recibe, empaqueta y encola de forma asíncrona una petición del IOP.
 * Realiza el entrelazado de punteros FIFO en el bus y dispara el hardware interrupt handler si el canal está libre.
 * Dirección original en Ghidra: 0x0011D590 (PAL)
 *
 * @param p_packet_req Dirección base del paquete de interrupción con la orden de carga enviado por el IOP (param_1).
 */
void sys_sif_rpc_on_queue_request(void* p_packet_req) {
	if (p_packet_req == NULL) {
		return;
	}

	u8* p_pkt = (u8*)p_packet_req;

	// Offset 0x34 almacena la dirección física del nodo de datos interno a procesar (int)
	s32 p_node_addr = *(s32*)(p_pkt + 0x34);
	u8* p_node = (u8*)(long)p_node_addr;

	// Offset 0x40 dentro del nodo apunta al descriptor maestro del canal SIF (int**)
	s32** pp_channel_master = *(s32***)(p_node + 0x40);
	s32* p_channel = *pp_channel_master;

	// Índice 3 (3 * 4 = 12 bytes) es el puntero al nodo frontal de la cola FIFO
	if (p_channel[3] == 0) {
		p_channel[3] = p_node_addr;
	}
	else {
		// Índice 4 (4 * 4 = 16 bytes) es el puntero al nodo trasero de salida previo
		u8* p_last_rear_node = (u8*)(long)p_channel[4];
		*(s32*)(p_last_rear_node + 0x3C) = p_node_addr; // Enlace puente FIFO
	}

	// Actualiza el registro de cola trasera con la dirección del nuevo elemento ingresado
	p_channel[4] = p_node_addr;

	// Volcado de metadatos en ráfaga contigua de 32 bits hacia el nodo interno
	*(u32*)(p_node + 0x20) = *(u32*)(p_pkt + 0x14); // ID de comando
	*(u32*)(p_node + 0x1C) = *(u32*)(p_pkt + 0x1C); // Puntero de buffer de origen
	*(u32*)(p_node + 0x24) = *(u32*)(p_pkt + 0x20); // Puntero de buffer de destino
	*(u32*)(p_node + 0x0C) = *(u32*)(p_pkt + 0x24); // Tamaño en bytes de la ráfaga
	*(u32*)(p_node + 0x28) = *(u32*)(p_pkt + 0x28); // Atributos de control
	*(u32*)(p_node + 0x2C) = *(u32*)(p_pkt + 0x2C); // Máscara de sincronización
	*(u32*)(p_node + 0x30) = *(u32*)(p_pkt + 0x30); // Parámetros secundarios
	*(u32*)(p_node + 0x34) = *(u32*)(p_pkt + 0x10); // ID de hilo propietario

	// Evaluación del disparador por hardware cruzado de hilos
	s32 active_thread_id = p_channel[0]; // Índice 0 (+0 bytes)
	s32 channel_busy_flag = p_channel[1]; // Índice 1 (+4 bytes)

	if (active_thread_id > -1 && channel_busy_flag == 0) {
		// Llama a tu rutina del laboratorio para inyectar y procesar la entrada de hardware
		sys_hardware_keyboard_interrupt_handler((u64)active_thread_id);
	}
}

// Definición de las variables globales del búfer de teclado en la RAM de la PS2
#define KEYBOARD_BUFFER_INDEX       (*(s32*)0x0013C68C)
#define KEYBOARD_STATIC_BUFFER_PTR  ((u16*)0x0013C690)

// Referencia a tu contador de caracteres de consola ya integrado
extern s32 g_debug_console_char_count;

// Prototipos oficiales de APIs del Kernel de Sony emulados
s32 iSignalSema(s32 sema_id);
s32 iWakeupThread(s32 thread_id);

/**
 * @brief Manejador de interrupción por hardware que captura las pulsaciones del teclado de desarrollo.
 * Almacena los caracteres ASCII de forma cíclica en un búfer de anillo de 512 posiciones.
 * Dirección original en Ghidra: 0x0011B8D8 (PAL)
 *
 * @param expected_thread_id Identificador del hilo de ejecución esperado para la validación (param_1).
 */
void sys_hardware_keyboard_interrupt_handler(u64 expected_thread_id) {
	u64 hardware_input_char = 0;

	// En la PS2 real, esto ejecuta la instrucción syscall(-47) para interrogar al hardware de Sony.
	// Para el port nativo de PC, el entorno del sistema operativo maneja los eventos de teclado de fondo.
	// Simulamos el comportamiento del registro in_v0 extrayendo el carácter:
#if defined(PLATFORM_PS2)
	__asm__ volatile("syscall" : "=r"(hardware_input_char) : "r"(-47));
#else
	hardware_input_char = 0; // Entrada simulada vacía para evitar bucles basura en PC
#endif

	// 1. Si la tecla coincide con el hilo esperado, procesa la inserción en el anillo de 512 bytes
	if (hardware_input_char == expected_thread_id) {
		if (hardware_input_char < 0x100 && g_debug_console_char_count != 0) {

			// Aplica la máscara binaria & 0x1FF para limitar el índice cíclico a 512 palabras (9 bits)
			s32 ring_index = KEYBOARD_BUFFER_INDEX & 0x1FF;
			KEYBOARD_BUFFER_INDEX = (KEYBOARD_BUFFER_INDEX & 0x1FF) + 1;

			// Inyecta el carácter en ráfaga contigua dentro del búfer estático global
			u16* p_buffer_cell = KEYBOARD_STATIC_BUFFER_PTR + ring_index;
			*p_buffer_cell = (u16)hardware_input_char;

			// Despierta de golpe a los hilos de control mediante semáforos lógicos
#if defined(PLATFORM_PS2)
			iSignalSema(0); // ID de semáforo del sistema operativo
#endif
		}
	}
	// 2. Si hay un desfase en el hilo, fuerza un despertar inmediato de la CPU
	else {
#if defined(PLATFORM_PS2)
		iWakeupThread((s32)expected_thread_id);
#endif
	}
}

/**
 * @brief Callback de interrupción SIF RPC que procesa y despacha comandos de transferencia directa de datos DMA (Streaming).
 * Prepara la ranura en el búfer de anillo y reenvía los punteros físicos de ráfaga física de la PS2.
 * Dirección original en Ghidra: 0x0011D2F0 (PAL)
 *
 * @param p_incoming_packet Dirección del paquete con las direcciones de la ráfaga (param_1).
 * @param p_hud_container Dirección base del contenedor global de la interfaz gráfica (param_2).
 */
void sys_sif_rpc_on_data_transfer(void* p_incoming_packet, void* p_hud_container) {
	if (p_incoming_packet == NULL || p_hud_container == NULL) {
		return;
	}

	u32* p_pkt_in = (u32*)p_incoming_packet;

	// 1. Reserva la ranura del comando cíclico en el búfer en anillo de Insomniac
	u32* p_response_packet = (u32*)hud_alloc_ring_buffer_node((u32*)p_hud_container);

	// 2. Extrae y empaqueta los identificadores de sincronización base de la PS2
	u32 val_1c = p_pkt_in[7]; // Offset 0x1C (Índice 7)
	p_response_packet[5] = p_pkt_in[5]; // Offset 0x14 (Índice 5)
	p_response_packet[7] = val_1c;
	p_response_packet[8] = 0x8000000C; // ID de comando de ejecución de transferencia activa

	// 3. Extrae los punteros y longitudes físicas de la transacción asíncronas
	u32 src_dma_addr = p_pkt_in[8];  // Offset 0x20 (Índice 8)
	u32 dest_dma_addr = p_pkt_in[9]; // Offset 0x24 (Índice 9)
	long block_len = (long)p_pkt_in[10]; // Offset 0x28 (Índice 10)

	// 4. Invoca de golpe a tu wrapper síncrono enviando los descriptores físicos al bus SIF (Tamaño 64 bytes = 0x40)
	sys_sif_submit_dma_packet_sync(0x80000008, p_response_packet, 0x40, src_dma_addr, dest_dma_addr, block_len);
}

/**
 * @brief Callback de interrupción SIF RPC que responde al coprocesador IOP sobre el estado físico de un nodo del HUD.
 * Reserva una ranura en el buffer de anillo, interroga al árbol mediante hud_find_node_by_id y despacha un paquete síncrono.
 * Dirección original en Ghidra: 0x0011D3A0 (PAL)
 *
 * @param p_incoming_packet Dirección del paquete con la consulta enviado por el IOP (param_1).
 * @param p_hud_container Dirección base del contenedor global de la interfaz gráfica (param_2).
 */
void sys_sif_rpc_on_query_node_status(void* p_incoming_packet, void* p_hud_container) {
	if (p_incoming_packet == NULL || p_hud_container == NULL) {
		return;
	}

	u32* p_pkt_in = (u32*)p_incoming_packet;

	// 1. Reserva de una ranura de ráfaga en el buffer circular usando tu función asignadora
	u32* p_response_packet = (u32*)hud_alloc_ring_buffer_node((u32*)p_hud_container);

	// 2. Copia y empaqueta los metadatos de control base en los desplazamientos contiguos
	u32 val_14 = p_pkt_in[5]; // Offset 0x14
	p_response_packet[7] = p_pkt_in[7]; // Offset 0x1C
	p_response_packet[5] = val_14;
	p_response_packet[8] = 0x80000009; // ID de estado de respuesta

	// 3. Invoca a tu buscador para interrogar la existencia del widget por ID (Offset 0x20)
	s32 target_id = (s32)p_pkt_in[8];
	s32* p_found_node = hud_find_node_by_id(target_id, p_hud_container);

	if (p_found_node == NULL) {
		// Ranuras de seguridad vacías si el nodo no fue localizado
		p_response_packet[9] = 0;
		p_response_packet[10] = 0;
		p_response_packet[11] = 0;
	}
	else {
		// Inyecta las direcciones reales de la RAM, semáforos y estados
		p_response_packet[9] = (u32)(long)p_found_node;
		p_response_packet[10] = (u32)p_found_node[2]; // Atributo o semáforo interno
		p_response_packet[11] = (u32)p_found_node[5]; // Bandera de estado secundaria
	}

	// 4. Despacho prioritario de vuelta al bus SIF usando tu wrapper síncrono (Paquete de 64 bytes = 0x40)
	sys_sif_submit_dma_packet_sync(0x80000008, p_response_packet, 0x40, 0, 0, 0);
}

// Referencia a tu función maestra SIF ya integrada en tu repositorio
u64 sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len);

/**
 * @brief Asigna e indexa de forma cíclica la siguiente ranura disponible dentro de un búfer en anillo del HUD.
 * Aplica aritmética de módulo para reutilizar ranuras fijas de 64 bytes (0x40) y previene divisiones por cero.
 * Dirección original en Ghidra: 0x0011D208 (PAL)
 *
 * @param p_ring_struct Dirección base de la estructura de control del búfer circular (param_1).
 * @return u32 Dirección de memoria física de la ranura calculada lista para su uso.
 */
u32 hud_alloc_ring_buffer_node(u32* p_ring_struct) {
	if (p_ring_struct == NULL) {
		return 0;
	}

	// Offset 0x18 equivale al índice 6 en enteros de 32 bits (6 * 4 = 24 bytes) - Capacidad máxima
	s32 max_capacity = (s32)p_ring_struct[0x06];

	// Control de seguridad equivalente a la instrucción de hardware trap(7) de la PS2
	assert(max_capacity != 0 && "Error critico del HUD: Capacidad del buffer en anillo es cero.");
	if (max_capacity == 0) {
		return 0;
	}

	// Offset 0x24 equivale al índice 9 (9 * 4 = 36 bytes) - Contador acumulativo de inserciones
	s32 total_inserts = (s32)p_ring_struct[0x09];

	// Calcula el índice circular seguro aplicando la operación módulo
	s32 circular_index = total_inserts % max_capacity;

	// Incrementa el cursor acumulativo para la siguiente asignación
	p_ring_struct[0x09] = (u32)(circular_index + 1);

	// Offset 0x14 equivale al índice 5 (5 * 4 = 20 bytes) - Dirección base del array de nodos
	u32 p_array_base = p_ring_struct[0x05];

	// Cada nodo tiene un tamaño estricto de 64 bytes (0x40 en hexadecimal)
	return p_array_base + (u32)(circular_index * 0x40);
}

// Definición de las variables globales del filtro de comandos en la RAM de la PS2
#define IO_QUEUE_LOCK_FLAG         (*(s32*)0x001A750C)
#define IO_BACKUP_COMMAND_ID       (*(u32*)0x001A7510)

// Definición de identificadores de semáforos en la RAM de la PS2
#define IO_LOCK_SEMA_ID             (*(s32*)0x00136428)
#define IO_WAIT_SEMA_ID             (*(s32*)0x0013642C)
#define IO_DMA_SEMA_ID              (*(s32*)0x00136420)

// Referencia a tu variable global de comandos pendientes ya integrada
#define IO_PENDING_COMMANDS_COUNT   (*(s32*)0x00136430)

// Definición del offset del canal de sonido en la RAM de la PS2
#define SOUND_CHANNEL_WIDGET_HANDLE    (*(u32*)0x00137E48)
#define DEBUG_NET_LOG_LEVEL            (*(s32*)0x00136410)

// Variables de control de estado del subsistema RPC simuladas para el port a PC
u8  g_sys_sif_rpc_is_initialized = 0;
u32 g_sys_sif_rpc_client_struct[16] = { 0 };
u32 g_sys_sif_rpc_dummy_packet = 0;

// Referencias a tus helpers e inicializadores ya integrados en tu repositorio
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
bool sys_sif_init_manager(void);
void sys_sif_register_callback(long command_id, u32 callback_ptr, u32 callback_arg);
u32  sys_sif_get_channel_descriptor_ptr(s32 channel_index);
//void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Stubs nativos adicionales del SDK de Sony
u32  sceSifSetReg(void);

// Referencia a tu desactivador de nodos del HUD ya integrado en el repositorio
void hud_disable_widget_node(u32* p_internal_node);

// Stub oficial del SDK del Kernel de Sony para interrupciones de semáforos
s32 iSignalSema(s32 sema_id);

/**
 * @brief Realiza una búsqueda exhaustiva bidimensional en árbol y listas enlazadas para localizar un nodo del HUD por su ID.
 * Recorre las ramas del contenedor (offset 0x24) y los hermanos de lista (offset 0x38) hasta hallar una coincidencia.
 * Dirección original en Ghidra: 0x0011D350 (PAL)
 *
 * @param target_node_id Identificador único del componente que se desea buscar (param_1).
 * @param p_hud_container Dirección base de la estructura contenedora de la interfaz (param_2).
 * @return s32* Puntero al nodo físico de la estructura hallada en la RAM, o NULL si no existe.
 */
s32* hud_find_node_by_id(s32 target_node_id, void* p_hud_container) {
	if (p_hud_container == NULL) {
		return NULL;
	}

	u8* p_container_bytes = (u8*)p_hud_container;

	// Offset 0x28 (Índice 10 en u32) apunta a la primera rama o bloque de control del contenedor
	s32 p_current_branch = *(s32*)(p_container_bytes + 0x28);

	while (p_current_branch != 0) {
		u8* p_branch_bytes = (u8*)p_current_branch;

		// Offset +8 (Índice 2 en u32) dentro de la rama contiene el puntero al inicio de la lista de nodos
		s32* p_node_cursor = *(s32**)(p_branch_bytes + 8);

		while (p_node_cursor != NULL) {
			// El índice 0 (+0 bytes) almacena de forma fija el ID identificador del nodo actual
			s32 current_node_id = *p_node_cursor;

			if (current_node_id == target_node_id) {
				return p_node_cursor; // Hallado con éxito: retorna el puntero físico de la RAM
			}

			// Offset 0x38 (Índice 14 / 0x0E en s32) almacena el puntero al siguiente nodo hermano
			p_node_cursor = (s32*)p_node_cursor[0x0E];
		}

		// Si la lista de la rama actual se agota, salta al offset 0x14 (Índice 5) para pasar a la siguiente rama
		p_current_branch = *(s32*)(p_branch_bytes + 0x14);
	}

	return NULL; // No se encontró ninguna coincidencia en toda la superestructura
}

/**
 * @brief Manejador de interrupción por hardware (Callback) invocado al completarse una transacción en el bus SIF RPC.
 * Ejecuta el callback del usuario asociado, actualiza los metadatos de buffers y despierta los hilos pausados del sistema.
 * Dirección original en Ghidra: 0x0011D238 (PAL)
 *
 * @param p_packet_res Dirección base del paquete de interrupción recibido desde el IOP (param_1).
 */
void sys_sif_rpc_on_transaction_complete(void* p_packet_res) {
	if (p_packet_res == NULL) {
		return;
	}

	u8* p_pkt = (u8*)p_packet_res;

	// Offset 0x20 almacena el identificador o tipo de evento SIF recibido (uint)
	u32 transaction_event_id = *(u32*)(p_pkt + 0x20);

	// Offset 0x1C almacena el puntero a la estructura interna de control del canal (int*)
	s32** pp_channel_struct = *(s32***)(p_pkt + 0x1C);
	s32* p_channel = *pp_channel_struct;

	if (transaction_event_id == 0x8000000A) {
		// Índice 7 (7 * 4 = 28 bytes) almacena el puntero de función de callback del usuario
		void (*user_callback)(s32) = (void (*)(s32))p_channel[7];

		if (user_callback != NULL) {
			// Índice 8 (8 * 4 = 32 bytes) almacena el argumento asociado que se le pasa al callback
			user_callback(p_channel[8]);

			// Recarga la estructura por si fue alterada durante la llamada del usuario
			pp_channel_struct = *(s32***)(p_pkt + 0x1C);
			p_channel = *pp_channel_struct;
		}
	}
	else if (transaction_event_id == 0x80000009) {
		// Sincroniza en ráfaga contigua los metadatos de tamaño y punteros devueltos por el hardware
		p_channel[9] = *(s32*)(p_pkt + 0x24); // Offset 0x24: Nueva dirección o ID
		p_channel[5] = *(s32*)(p_pkt + 0x28); // Offset 0x28: Tamaño del bloque
		p_channel[6] = *(s32*)(p_pkt + 0x2C); // Offset 0x2C: Atributos secundarios
	}

	// Índice 2 (2 * 4 = 8 bytes) almacena el ID del semáforo asignado al hilo del canal
	s32 sema_id = p_channel[2];

	if (sema_id > -1) {
		// En la PS2 real, esto incrementa el semáforo de hardware desde la interrupción.
		// Para efectos funcionales en PC, el entorno multitarea moderno lo resuelve de forma nativa:
#if defined(PLATFORM_PS2)
		iSignalSema(sema_id);
#endif
	}

	// Invoca a tu función del laboratorio para apagar de forma segura el nodo en el HUD
	hud_disable_widget_node((u32*)*pp_channel_struct);

	// Corta el enlace de memoria RAM poniendo a cero la referencia del puntero
	*pp_channel_struct = NULL;
}

/**
 * @brief Desactiva un nodo de widget del HUD en la memoria, apagando su bit de habilitación y limpiando su firma.
 * Dirección original en Ghidra: 0x0011D1E8 (PAL)
 *
 * @param p_internal_node Dirección base del bloque interno del nodo del widget (param_1).
 */
void hud_disable_widget_node(u32* p_internal_node) {
	if (p_internal_node == NULL) {
		return;
	}

	// Offset 0x18 equivale al índice 6 en enteros de 32 bits (6 * 4 = 24 bytes)
	// Limpia la firma estructural de validación
	p_internal_node[0x06] = 0;

	// Offset 0x10 equivale al índice 4 (4 * 4 = 16 bytes), que almacena las banderas de estado
	// Aplica la máscara binaria & 0xFFFFFFFE para apagar de golpe el bit de "Nodo Activo"
	p_internal_node[0x04] = p_internal_node[0x04] & 0xFFFFFFFE;
}

// Definición física real de la estructura SifRpcClientData extraída de los offsets del SDK de Sony
typedef struct {
	u32  is_active;       // Offset +0x00 (DAT_0013e980)
	u32  p_iop_buffer;     // Offset +0x04 (DAT_0013e984)
	s32  buffer_size;     // Offset +0x08 (DAT_0013e988)
	u32  p_gp_register;   // Offset +0x0C (DAT_0013e98c)
	u32  packet_id;       // Offset +0x10 (DAT_0013e990)
	u32  p_channel_desc;  // Offset +0x14 (DAT_0013e994)
	u32  server_id;       // Offset +0x18 (DAT_0013e998)
	u32  p_command_buff;  // Offset +0x1C (DAT_0013e99c)
	s32  command_size;    // Offset +0x20 (DAT_0013e9a0)
	u32  callback_arg;    // Offset +0x24 (DAT_0013e9a4)
} SifRpcClientData;

// Tu código original ahora compilará perfectamente:
//extern SifRpcClientData g_sys_sif_rpc_client_struct;
extern u32 g_sys_sif_rpc_dummy_packet;

// Referencias a tus funciones e inicializadores ya consolidados en tu repositorio
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
bool sys_sif_init_manager(void);
void sys_sif_register_callback(long command_id, void* callback_ptr, void* callback_arg);
u32  sys_sif_get_channel_descriptor_ptr(s32 channel_index);
//void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Prototipos de soporte del SDK nativo de la PS2
u32  sceSifGetReg(void);
u32  sceSifSetReg(void);

// Callbacks del circuito asíncrono
void sys_sif_rpc_on_transaction_complete(void* p_packet_res);
void sys_sif_rpc_on_query_node_status(void* p_incoming_packet, void* p_hud_container);
void sys_sif_rpc_on_queue_request(void* p_packet_req);
void sys_sif_rpc_on_data_transfer(void* p_incoming_packet, void* p_hud_container);

/**
 * @brief Inicializa por completo el subsistema de cliente y servidor de llamadas remotas RPC del bus SIF.
 * Estructura de forma exacta la información del cliente y registra los cuatro callbacks de interrupción.
 * Dirección original en Ghidra: 0x0011CF78 (PAL)
 */
 // 1. Cambiamos el tipo de retorno de 'bool' a 'int'
int sys_sif_rpc_init_client(void) {
	bool interrupt_status;

	interrupt_status = kernel_system_sync_guard();

	// 1. Verificación contra inicialización duplicada
	if (g_sys_sif_rpc_is_initialized != 0) {
		if (interrupt_status) {
			kernel_system_sync_release();
		}
		return 1; // Cambiado true por 1
	}

	g_sys_sif_rpc_is_initialized = 1;
	kernel_system_sync_release();

	// 2. Despierta el gestor maestro de los canales DMA físicos
	sys_sif_init_manager();

	kernel_system_sync_guard();

	// 3. Configuración en ráfaga contigua de la estructura física
	SifRpcClientData* p_sif_client = (SifRpcClientData*)&g_sys_sif_rpc_client_struct;
	p_sif_client->is_active = 1;
	p_sif_client->p_iop_buffer = 0x2013D180;
	p_sif_client->buffer_size = 0x20;
	p_sif_client->p_gp_register = 0;
	p_sif_client->packet_id = 0;
	p_sif_client->p_channel_desc = 0x2013D980;
	p_sif_client->server_id = 0x20;
	p_sif_client->p_command_buff = 0x2013E180;
	p_sif_client->command_size = 0x20;
	p_sif_client->callback_arg = 0;

	// 4. Registro formal del circuito asíncrono completo con nombres auto-traducidos
	sys_sif_register_callback(-0x7FFFFFF8, (void*)sys_sif_rpc_on_transaction_complete, &g_sys_sif_rpc_client_struct);
	sys_sif_register_callback(-0x7FFFFFF7, (void*)sys_sif_rpc_on_query_node_status, &g_sys_sif_rpc_client_struct);
	sys_sif_register_callback(-0x7FFFFFF6, (void*)sys_sif_rpc_on_queue_request, &g_sys_sif_rpc_client_struct);
	sys_sif_register_callback(-0x7FFFFFF4, (void*)sys_sif_rpc_on_data_transfer, &g_sys_sif_rpc_client_struct);

	kernel_system_sync_release();

	// 5. Lazo de Handshake final con el coprocesador IOP
	long register_check = (long)sceSifGetReg();

	if (register_check == 0) {
		// --- EVASIÓN DE CRASH EN PC ---
		// Escribir directamente en la dirección física fija de PS2 0x0013D1CC 
		// provocaría una Violación de Acceso (Segmentation Fault) instantánea en Windows.
		// Reemplazamos el puntero hardcodeado por una variable global simulada.
		static u32 pc_simulated_dat_0013d1cc = 0;
		pc_simulated_dat_0013d1cc = 1;

		// Envía el comando de inicialización remota RPC (0x80000002)
		sys_sif_submit_dma_packet_simple(0x80000002, &g_sys_sif_rpc_dummy_packet, 0x10, 0, 0, 0);

		// --- EVASIÓN DE BUCLE INFINITO EN PC ---
		// El bucle original 'while(1)' esperaba que un canal DMA físico de la PS2 pusiera un flag.
		// En PC, al no existir ese hardware, se quedaría congelado al 100% de CPU consumida.
		// Forzamos la salida simulando éxito inmediato.
		while (1) {
			u32 channel_desc = 1; // Simulamos que el canal está listo (distinto de 0)
			if (channel_desc != 0) {
				break;
			}
		}

#if defined(PLATFORM_PS2)
		return (int)sceSifSetReg();
#else
		return 1; // Retorna éxito en PC
#endif
	}

	// Equivale macro MIPS SUB81 para extraer el byte de menor peso de forma portátil
	return (int)(register_check & 0xFF);
}

// Variables de estado del SIF simuladas para el port
extern int  g_sys_sif_is_initialized;
extern int  g_sys_sif_handler_id;
extern int  g_sys_sif_reg_status;

// Referencias a tus funciones de bajo nivel mapeadas en el laboratorio
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
u64  sys_kernel_enable_dmac(void);
//void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size, u32 src_addr, u32 dest_addr, long transfer_len);

// Stubs de emulación para APIs nativas del SDK de Sony
s32 sceAddDmacHandler(s32 channel, void* handler, s32 arg);
u32 sceSifGetReg(u32 reg_id);

// Definición de las tablas globales de descriptores de callbacks en la RAM de la PS2
#define SIF_GENERAL_CALLBACK_TABLE     (*(u32*)0x0013CFEC)
#define SIF_SYSTEM_CALLBACK_TABLE      (*(u32*)0x0013CFE4)

// Definición de la dirección física de la tabla de descriptores de canales en la RAM de la PS2
#define SIF_CHANNEL_DESCRIPTOR_TABLE_PTR   ((const u32*)0x0013D100)

/**
 * @brief Recupera de forma indexada el puntero al descriptor de control de un canal del subsistema SIF.
 * Dirección original en Ghidra: 0x0011C8B0 (PAL)
 *
 * @param channel_index Índice numérico del canal SIF a consultar (param_1).
 * @return u32 Dirección física o palabra de control del canal enlazado, o 0 si la tabla es inválida.
 */
u32 sys_sif_get_channel_descriptor_ptr(s32 channel_index) {
	if (SIF_CHANNEL_DESCRIPTOR_TABLE_PTR == NULL) {
		return 0;
	}

	// Calcula el offset exacto aplicando el paso indexado de 4 bytes (1 palabra en MIPS)
	const u32* p_descriptor_slot = SIF_CHANNEL_DESCRIPTOR_TABLE_PTR + channel_index;

	// Retorna de forma directa el valor almacenado en la ranura
	return *p_descriptor_slot;
}

// Referencias a tus helpers ya integrados en el repositorio
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);
s32  hud_validate_widget_node_state(const u32* p_widget_handle);
void sys_kernel_wait_timer(u32 microseconds);

// Prototipos de sincronización del motor y del SDK que ya mapeaste
bool kernel_system_sync_guard(void);
void kernel_system_sync_release(void);
s32  sceEnableDmac(s32 channel);

// Prototipo de tu vaciador de caché ya integrado en el repositorio
void sys_kernel_flush_dcache_range(u32 start_addr, long block_size);

// Prototipos oficiales del SDK de Sony emulados de forma pasiva
u64 sceSifSetDma(void);
u64 isceSifSetDma(void);

// Referencia a tu función maestra SIF ya integrada en el repositorio
u64 sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len);

/**
 * @brief Vacía y sincroniza un rango de la caché de datos (D-Cache) de la CPU hacia la RAM principal (Writeback Invalidate).
 * Reemplaza de forma de hardware portátil las instrucciones de bajo nivel cacheOp(0x18) e instrucciones SYNC de la PS2.
 * Dirección original en Ghidra: 0x0011CEC8 (PAL)
 *
 * @param start_addr Dirección de memoria RAM de inicio del bloque a sincronizar (param_1).
 * @param block_size Longitud o tamaño en bytes del segmento a vaciar (param_2).
 */
void sys_kernel_flush_dcache_range(u32 start_addr, long block_size) {
	// En la PlayStation 2 real, el procesador requiere alinear el rango a líneas de 64 bytes (0x40),
	// calcular el paso mediante desplazamientos de bits (>> 6 y >> 3) para ejecutar ráfagas desenrolladas,
	// y disparar cacheOp(0x18, ...) intercalado con barreras de memoria SYNC(0).
	// Para el port nativo moderno a PC, el hardware x86_64/ARM maneja de forma automática 
	// la coherencia de la caché y accesos DMA sin necesidad de forzar flushes manuales:
	return;
}

/**
 * @brief Habilita de forma segura y atómica el controlador DMA (DMAC) suspendiendo temporalmente las interrupciones si es requerido.
 * Dirección original en Ghidra: 0x0011B6C0 (PAL)
 *
 * @return u64 Estado o configuración previa del controlador DMAC.
 */
u64 sys_kernel_enable_dmac(void) {
	// Simulamos la lectura de la bandera de estado de interrupción de la CPU MIPS
	u32 status_interrupt_flag = 1; // Forzamos un estado seguro simulado para PC
	bool bVar1 = false;

	// 1. Si las interrupciones de hardware están encendidas, activa el candado atómico del motor
	if (status_interrupt_flag != 0) {
		bVar1 = kernel_system_sync_guard();
	}

	// 2. Invoca la instrucción de activación nativa. En PC, los hilos de renderizado modernos
	// operan con pipelines de memoria virtuales paralelos asíncronos nativos por defecto:
	u64 previous_dma_state = 0;
#if defined(PLATFORM_PS2)
	previous_dma_state = (u64)sceEnableDmac(-1); // Activa todos los canales del bus físico
	__asm__ volatile("sync"); // Instrucción MIPS SYNC(0) para vaciar buses de caché
#endif

	// 3. Libera el candado de la CPU una vez finalizada la transacción del bus
	if (status_interrupt_flag != 0 && bVar1) {
		kernel_system_sync_release();
	}

	return previous_dma_state;
}

/**
 * @brief Custodia la sincronización de comandos del flujo de sonido y streaming. Congela el hilo si el hardware de audio está ocupado.
 * Dirección original en Ghidra: 0x00124C28 (PAL)
 */
s32 sys_sound_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	s32 is_sound_node_active;

	// Caso A: Esperar a que el canal de sonido quede libre antes de avanzar
	if (command_type == 0) {
		if (DEBUG_NET_LOG_LEVEL > 0) {
			boot_txt_render_extended_string((const u8*)"S cmd wait\n", p2, p3, p4, p5, p6, p7, p8);
		}

		// Bucle de bloqueo: Duerme el hilo mientras el procesador de sonido de la PS2 reporte actividad
		while (1) {
			is_sound_node_active = hud_validate_widget_node_state(&SOUND_CHANNEL_WIDGET_HANDLE);
			if (is_sound_node_active == 0) {
				break;
			}

			// Pausa el procesador por 1 milisegundo (1000 microsegundos) para dar margen al bus de audio
			sys_kernel_wait_timer(1000);
		}
		return 0;
	}

	// Caso B: Consulta rápida del estado de ocupación del subsistema de sonido
	return hud_validate_widget_node_state(&SOUND_CHANNEL_WIDGET_HANDLE);
}

// Prototipo de la API oficial del SDK de Sony
s32 sceCreateSema(void);

/**
 * @brief Inicializa y reserva los objetos semáforos de sincronización de bajo nivel del Kernel de la PS2.
 * Prepara el canal de E/S poniendo a cero el contador de comandos pendientes del motor.
 * Dirección original en Ghidra: 0x00124780 (PAL)
 */
void sys_io_init_kernel_semaphores(void) {
	// Evalúa si las ranuras de semáforos se encuentran sin inicializar (-1)
	if (IO_LOCK_SEMA_ID == -1 || IO_WAIT_SEMA_ID == -1) {

		// En la PS2 real, esto invoca de forma directa a la API del Kernel de Sony.
		// Para efectos funcionales en el port portable de PC, asignamos IDs de estado lógicos válidos:
#if defined(PLATFORM_PS2)
		IO_LOCK_SEMA_ID = sceCreateSema();
		IO_WAIT_SEMA_ID = sceCreateSema();
		IO_DMA_SEMA_ID = sceCreateSema();
#else
		IO_LOCK_SEMA_ID = 1;
		IO_WAIT_SEMA_ID = 2;
		IO_DMA_SEMA_ID = 3;
#endif

		// Resetea a cero de forma segura el contador de ráfagas IO pendientes
		IO_PENDING_COMMANDS_COUNT = 0;
	}
}

// Prototipo de la función maestra del menú e intro que acabas de descubrir
void game_main_menu_and_intro_loop(void); // FUN_002b8940

// Referencia a tu filtro de cola ya integrado
u32 sys_io_queue_command_filter(u32 target_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8);

/**
 * @brief Envía el identificador de la etapa de la intro y menú principal del juego a la cola lógica de comandos del sistema.
 * Dirección original en Ghidra: 0x002B74F0 (PAL)
 */
u32 sys_boot_dispatch_stage(long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	// Registramos la dirección de la subrutina del bucle del menú como el comando objetivo a despachar
	u32 target_stage_command = (u32)((long)game_main_menu_and_intro_loop);

	// Invoca de golpe a tu filtro condicional para asegurar la inyección limpia en la FPU/Kernel
	return sys_io_queue_command_filter(target_stage_command, p2, p3, p4, p5, p6, p7, p8);
}

// Referencias a tus helpers ya integrados en el repositorio
u32 sys_io_submit_command(u32 new_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8);

/**
 * @brief Filtra y almacena de forma segura los comandos IO pendientes evaluando el estado del candado global.
 * Evita la corrupción o pérdida de transiciones de datos respaldando las solicitudes en buffers secundarios de reserva.
 * Dirección original en Ghidra: 0x00133730 (PAL)
 */
u32 sys_io_queue_command_filter(u32 target_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	u32 fallback_command = IO_BACKUP_COMMAND_ID;
	u32 current_command = target_command_id;

	// 1. Si el candado global de la cola está libre, despacha el comando de inmediato
	if (IO_QUEUE_LOCK_FLAG == 0) {
		fallback_command = sys_io_submit_command(target_command_id, p2, p3, p4, p5, p6, p7, p8);
		current_command = IO_BACKUP_COMMAND_ID;
	}

	// 2. Respalda el identificador en la variable de reserva para el próximo ciclo
	IO_BACKUP_COMMAND_ID = current_command;

	return fallback_command;
}

// Definición del offset del comando activo en la RAM de la PS2
#define IO_ACTIVE_COMMAND_ID        (*(u32*)0x001418C0)

// Referencia a tu guardián de comandos IO
s32 sys_io_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8);

/**
 * @brief Envía e inyecta de forma segura un nuevo comando en la cola del canal de lectura IO (DVD/Red).
 * Aplica exclusión mutua suspendiendo temporalmente las interrupciones mediante wrappers genéricos del motor.
 * Dirección original en Ghidra: 0x001245D0 (PAL)
 *
 * @param new_command_id Identificador o dirección del nuevo comando a despachar (param_1).
 * @return u32 El ID del comando previo que se encontraba activo en el canal.
 */
u32 sys_io_submit_command(u32 new_command_id, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	// 1. Consulta al guardián con tipo '1' si el canal está libre para recibir la transacción
	s32 is_channel_busy = sys_io_sync_command_guard(1, p2, p3, p4, p5, p6, p7, p8);
	u32 previous_command_id = 0;
	bool bVar1;

	// 2. Si está libre, ejecuta un intercambio atómico protegiendo el hilo de la CPU
	if (is_channel_busy == 0) {
		bVar1 = kernel_system_sync_guard(); // Invoca a tu wrapper genérico de bloqueo

		previous_command_id = IO_ACTIVE_COMMAND_ID;
		IO_ACTIVE_COMMAND_ID = new_command_id; // Inyecta el nuevo comando de carga

		if (bVar1) {
			kernel_system_sync_release(); // Invoca a tu wrapper genérico de liberación
		}
	}

	return previous_command_id;
}

// Definición de las variables de estado IO en la RAM de la PS2
#define DEBUG_NET_LOG_LEVEL         (*(s32*)0x00136410)
#define IO_PENDING_COMMANDS_COUNT   (*(s32*)0x00136430)
#define IO_CHANNEL_WIDGET_HANDLE    (*(u32*)0x001375D0)

// Referencias a tus helpers ya integrados en el repositorio
//bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4, long param_5, long param_6, long param_7, long param_8);
s32  hud_validate_widget_node_state(const u32* p_widget_handle);
void sys_kernel_wait_timer(u32 microseconds);

/**
 * @brief Custodia la sincronización de carga de comandos IO (DVD/Red). Congela el hilo de forma segura si hay transferencias en curso.
 * Dirección original en Ghidra: 0x00124B88 (PAL)
 */
s32 sys_io_sync_command_guard(long command_type, long p2, long p3, long p4, long p5, long p6, long p7, long p8) {
	s32 is_node_active;

	// Caso A: Esperar a que el canal IO quede completamente libre
	if (command_type == 0) {
		if (DEBUG_NET_LOG_LEVEL > 0) {
			boot_txt_render_extended_string((const u8*)"N cmd wait\n", p2, p3, p4, p5, p6, p7, p8);
		}

		// Bucle de bloqueo: Duerme el hilo mientras existan comandos pendientes o el canal reporte actividad
		while (1) {
			is_node_active = hud_validate_widget_node_state(&IO_CHANNEL_WIDGET_HANDLE);
			if (IO_PENDING_COMMANDS_COUNT == 0 && is_node_active == 0) {
				break;
			}

			// Pausa el procesador por 1000 microsegundos (1 milisegundo) para no saturar el bus
			sys_kernel_wait_timer(1000);
		}
		return 0;
	}

	// Caso B: Consulta rápida e instantánea del estado de ocupación del canal
	s32 query_status = 1;
	if (IO_PENDING_COMMANDS_COUNT == 0) {
		is_node_active = hud_validate_widget_node_state(&IO_CHANNEL_WIDGET_HANDLE);
		query_status = 1;
		if (is_node_active == 0) {
			query_status = 0; // El canal está totalmente libre y disponible
		}
	}

	return query_status;
}

/**
 * @brief Pausa el hilo de ejecución por una cantidad precisa de microsegundos mediante llamadas al Kernel de la PS2.
 * Reemplaza de forma de hardware portátil la creación, espera y destrucción de semáforos/alarmas de la consola.
 * Dirección original en Ghidra: 0x00124568 (PAL)
 *
 * @param microseconds Cantidad exacta de tiempo a pausar el hilo en la CPU.
 */
void sys_kernel_wait_timer(u32 microseconds) {
	// En la PlayStation 2 real, se requiere generar un semáforo condicional con sceCreateSema,
	// amarrarle una alarma por interrupción con sceSetAlarm, suspender el hilo con sceWaitSema
	// y limpiar los recursos del kernel con sceDeleteSema al despertar.
	// Para efectos funcionales y portables en PC modernos, delegamos la pausa al sistema operativo:

	if (microseconds == 0) {
		return;
	}

	struct timespec requested_time;
	// Convierte microsegundos a segundos y nanosegundos estándar de C
	requested_time.tv_sec = microseconds / 1000000;
	requested_time.tv_nsec = (microseconds % 1000000) * 1000;

	// Pausa el hilo actual de forma nativa sin consumir ciclos de CPU de la PC
	nanosleep(&requested_time, NULL);
}

// Variables globales estáticas remanentes de la depuración de la PS2
s32  g_debug_console_char_count = 0;
char g_debug_console_static_buffer[128];
u8   g_debug_console_overflow_flag = 0;

// Prototipo de tu helper de depuración del paso anterior
void sys_init_deci2_debug_link(void);

/**
 * @brief Escribe un carácter individual dentro del búfer estático de la consola de depuración.
 * Ejecuta un vaciado automático por canal de hardware al detectar un salto de línea (\n) o desbordamiento.
 * Dirección original en Ghidra: 0x0011BF18 (PAL)
 *
 * @param character Byte o carácter ASCII a procesar (param_1).
 */
void sys_debug_console_write_char(s32 character) {
	s32 current_idx = g_debug_console_char_count;

	// 1. Control de seguridad contra desbordamiento de búfer (Límite 125 caracteres)
	if (g_debug_console_char_count > 0x7D) {
		g_debug_console_char_count = 0;
		g_debug_console_overflow_flag = 0;
		sys_init_deci2_debug_link();
		current_idx = g_debug_console_char_count;
	}

	// 2. Si es un carácter convencional, lo acumula secuencialmente en la matriz
	if (character != 10) { // 10 = '\n'
		g_debug_console_char_count = current_idx + 1;
		g_debug_console_static_buffer[current_idx] = (char)character;

		// Emulación nativa en consola de PC en tiempo real para desarrollo:
		fputc(character, stderr);
		return;
	}

	// 3. Al detectar salto de línea (\n), cierra la string y despacha el mensaje completo
	g_debug_console_char_count = 0;
	g_debug_console_static_buffer[current_idx] = 10;
	g_debug_console_static_buffer[current_idx + 1] = '\0'; // Terminador nulo de seguridad

	// Redireccionamiento portátil al canal de salida estándar moderno
	fputc('\n', stderr);
	fflush(stderr);

	sys_init_deci2_debug_link();
}


// Definiciones de direcciones globales mapeadas desde tu captura de Ghidra
#define GLOBAL_THREAD_STATE_ID     (*(u32*)0x001A6464)
#define GLOBAL_PAL_FRAME_RATE      (*(u32*)0x001A6468)
#define GLOBAL_INTRO_MANAGER_PTR   (*(u32*)0x001A646C)
#define GLOBAL_PLANET_LOAD_FLAG    (*(u32*)0x001A6470)
#define GLOBAL_INVENTORY_BASE_PTR  (*(u32*)0x001A64B4)

// Prototipos internos de hardware del compilador de la PS2
void ee_fpu_setup_init(void); // FUN_0026f438
s32  custom_vsprintf_engine_alt(void* output_dest, int* p_state_struct, const char* p_format_str, va_list args_list); // FUN_002b74f0 aproximada
// Definición del puntero dinámico global tipográfico en la RAM de la PS2
void* g_hud_typography_callback_ptr = (void*)0x00134718;

// Prototipos requeridos del ecosistema de strings que mapeamos
bool txt_render_scientific_string(const u8* p_src_str, float* p_args_stack);
void custom_hud_glyph_decoder(void); // FUN_0011bf18 aproximada

/**
 * @brief Valida el estado de integridad y activación de un nodo de widget del HUD en la memoria.
 * Realiza una prueba de consistencia de punteros e interroga la máscara de bits de habilitación activa.
 * Dirección original en Ghidra: 0x0011D810 (PAL)
 *
 * @param p_widget_handle Puntero al manejador de la estructura del componente de la interfaz (param_1).
 * @return s32 Retorna 1 si el nodo es válido y está activo en el pipeline gráfico, o 0 si está corrupto o inhabilitado.
 */
s32 hud_validate_widget_node_state(const u32* p_widget_handle) {
	if (p_widget_handle == NULL) {
		return 0;
	}

	// Recupera la dirección base del bloque interno del nodo (índice 0, +0 bytes)
	u32 p_internal_node = *p_widget_handle;

	if (p_internal_node != 0) {
		// Offset 0x18 equivale al índice 6 en enteros de 32 bits (6 * 4 = 24 bytes)
		u32* p_node_struct = (u32*)p_internal_node;
		u32 structural_signature = p_node_struct[6];

		// Verifica si el identificador secundario (+4 bytes) coincide con la firma del nodo
		if (p_widget_handle[1] == structural_signature) {
			// Offset 0x10 equivale al índice 4 (4 * 4 = 16 bytes), que almacena las banderas de estado
			u32 node_flags = p_node_struct[4];

			// Evalúa mediante máscara binaria si el bit de "Nodo Activo/Habilitado" está encendido
			if ((node_flags & 1) != 0) {
				return 1; // El nodo es totalmente íntegro y seguro de procesar
			}
		}
	}

	return 0;
}

/**
 * @brief Inicializa el canal de comunicación y depuración por hardware DECI2 del Kernel de la PS2.
 * En la consola real, abre el enlace con la PC de desarrollo para enviar reportes de fallas.
 * Dirección original en Ghidra: 0x0011BAA0 (PAL)
 */
void sys_init_deci2_debug_link(void) {
	// En la PlayStation 2 real, esto invoca a sceDeci2Open() del SDK de Sony.
	// Para el port nativo de PC, las interfaces físicas del kit TOOL no aplican,
	// por lo que el enlace se inicializa de forma pasiva y segura sin colgar el hilo:
	return;
}

/**
 * @brief Envoltorio tipográfico que altera temporalmente el callback global para renderizar strings con caracteres/iconos especiales.
 * Dirección original en Ghidra: 0x0011C820 (PAL)
 *
 * @param p_src_str Cadena de caracteres a procesar y formatear (param_1).
 * @param param_2 Bloque de argumentos variables empaquetados en la pila.
 * @return bool Retorna verdadero si el formateo científico/extendido fue exitoso.
 */
bool boot_txt_render_extended_string(const u8* p_src_str, long param_2, long param_3, long param_4,
	long param_5, long param_6, long param_7, long param_8) {
	if (p_src_str == NULL) {
		return false;
	}

	// Estructura de pila local contigua que emula el volcado de registros en uStack_38
	long local_args_stack[7];
	local_args_stack[0] = param_2;
	local_args_stack[1] = param_3;
	local_args_stack[2] = param_4;
	local_args_stack[3] = param_5;
	local_args_stack[4] = param_6;
	local_args_stack[5] = param_7;
	local_args_stack[6] = param_8;

	// 1. Respalda el callback tipográfico convencional en la pila
	void* p_previous_callback = *(void**)g_hud_typography_callback_ptr;

	// 2. Intercepta e inyecta el decodificador de glifos extendidos de Insomniac (FUN_0011bf18)
	*(void**)g_hud_typography_callback_ptr = (void*)custom_hud_glyph_decoder;

	// 3. Ejecuta el renderizado de la string procesando los tokens especiales con el nuevo juego de glifos
	bool render_status = txt_render_scientific_string(p_src_str, (float*)local_args_stack);

	// 4. Restaura de forma segura el callback previo para limpiar el pipeline gráfico
	*(void**)g_hud_typography_callback_ptr = p_previous_callback;

	return render_status;
}
/**
 * @brief Función raíz de inicialización estática del motor gráfico (Pre-Main Core Setup).
 * Prepara las banderas de hilos, tasas de refresco de video y limpia los punteros globales de control.
 * Dirección original en Ghidra: 0x002B7480 (PAL)
 *
 * @return s32 Código de estado o resultado del lazo de ejecución del motor.
 */
s32 engine_boot_static_init(void) {
	// 1. Inicializa las configuraciones de hardware de bajo nivel de la FPU
	// ee_fpu_setup_init(); // Equivalente a FUN_0026f438()

	// 2. Establece las condiciones de fábrica iniciales de las variables globales en la RAM
	GLOBAL_THREAD_STATE_ID = 0xFFFFFFFF;
	GLOBAL_PAL_FRAME_RATE = 0x20; // 32 decimal

	// Puesta a cero en ráfaga contigua de los gestores lúdicos de la interfaz y niveles
	GLOBAL_INTRO_MANAGER_PTR = 0;
	GLOBAL_PLANET_LOAD_FLAG = 0;
	*(u32*)0x001A6474 = 0; // DAT_001a6474
	*(u32*)0x001A6478 = 0; // DAT_001a6478
	*(u32*)0x001A6488 = 0; // DAT_001a6488
	*(u32*)0x001A6490 = 0; // DAT_001a6490
	*(u32*)0x001A64A4 = 0; // DAT_001a64a4
	*(u32*)0x001A64B0 = 0; // DAT_001a64b0
	GLOBAL_INVENTORY_BASE_PTR = 0;
	*(u32*)0x002A72B8 = 0; // DAT_002a72b8

	// 3. Invoca condicionalmente al motor alterno pasándole los argumentos de la pila
	// Para propósitos funcionales en PC/Plataformas modernas, emulamos el retorno lógico directo:
	s32 boot_status = 0;

	// (La llamada de la línea 28 uVar1 = FUN_002b74f0 se resolverá de forma unificada en el lazo principal)
	return boot_status;
}
