#include "types.h"
#include "ps2_kernel.h"
#include "ps2_sif.h"

// Variables de estado global de las tablas del bus SIF mapeadas en la RAM de la PS2
#define SIF_GENERAL_CALLBACK_TABLE     (*(u32*)0x0013CFEC)
#define SIF_SYSTEM_CALLBACK_TABLE      (*(u32*)0x0013CFE4)

// Definición de las variables globales de interrupción mapeadas en la RAM de la PS2
#define IO_INTERRUPT_CALLBACK       (*(void(**)(u32))(long)0x001418C4)
#define IO_INTERRUPT_ARGUMENT       (*(u32*)0x001418C8)

/**
 * @brief Manejador de interrupción (Callback) de bajo nivel del bus SIF IO.
 * Evalúa las banderas de reconfiguración y ejecuta el callback dinámico registrado pasando sus metadatos.
 * Dirección original en Ghidra: 0x001248B8 (PAL)
 */
void sys_io_iop_interrupt_handler(void) {
	// 1. Aplica el filtro protector: si el canal se está reconfigurando, aborta el despacho
	if (IO_INTERRUPT_CALLBACK != NULL && g_sys_io_reconfig_flag == 0) {

		// Almacena de forma segura los descriptores en variables locales antes del salto
		void (*p_callback)(u32) = IO_INTERRUPT_CALLBACK;
		u32 callback_arg = IO_INTERRUPT_ARGUMENT;

		// 2. DISPARADOR MAESTRO IO: Ejecuta la subrutina de respuesta en segundo plano de forma portable
		p_callback(callback_arg);
	}
}

// Variables de estado del SIF simuladas para el entorno portátil del port
u8   g_sys_sif_is_initialized = 0;
u32  g_sys_sif_handler_id = 0;
u32  g_sys_sif_reg_status = 0;

/**
 * @brief Empaqueta y despacha una transacción de transferencia asíncrona de datos a través del bus de hardware SIF (DMA).
 * Dirección original en Ghidra: 0x0011CBE8 (PAL)
 */
u64 sys_sif_submit_dma_packet(u32 command_type, u64 sync_flags, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len) {
	if (packet_size - 16 > 0x60) {
		return 0;
	}

	s32 calculated_offset = 0;
	if (transfer_len < 1) {
		p_packet_header = 0;
		*p_packet_header = (u32)(u8)(*p_packet_header);
	}
	else {
		p_packet_header = dest_addr;
		calculated_offset = 1;
		*p_packet_header = (u32)(u8)(*p_packet_header) | ((u32)transfer_len << 8);

		if ((sync_flags & 4) != 0) {
			calculated_offset = 0x10;
			goto finalize_packet;
		}
	}
	calculated_offset = calculated_offset << 4;

finalize_packet:
	p_packet_header = command_type;
	*(u8*)p_packet_header = (u8)packet_size;

	u64 transaction_id;
	if ((sync_flags & 1) == 0) {
#if defined(PLATFORM_PS2)
		transaction_id = sceSifSetDma();
#else
		transaction_id = 1;
#endif
	}
	else {
#if defined(PLATFORM_PS2)
		transaction_id = isceSifSetDma();
#else
		transaction_id = 1;
#endif
	}
	return transaction_id;
}

/**
 * @brief Envoltorio de conveniencia simplificado para despachar paquetes asíncronos en el bus SIF DMA fijando banderas en 0.
 * Dirección original en Ghidra: 0x0011CD20 (PAL)
 */
void sys_sif_submit_dma_packet_simple(u32 command_type, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len) {
	sys_sif_submit_dma_packet(command_type, 0, p_packet_header, packet_size, src_addr, dest_addr, transfer_len);
}

/**
 * @brief Envoltorio de conveniencia para despachar paquetes síncronos prioritarios en el bus SIF DMA fijando banderas en 1.
 * Dirección original en Ghidra: 0x0011CD60 (PAL)
 */
void sys_sif_submit_dma_packet_sync(u32 command_type, u32* p_packet_header, long packet_size,
	u32 src_addr, u32 dest_addr, long transfer_len) {
	sys_sif_submit_dma_packet(command_type, 1, p_packet_header, packet_size, src_addr, dest_addr, transfer_len);
}

/**
 * @brief Configura e inicializa por completo el gestor del subsistema SIF y los canales DMA de comunicación asíncrona.
 * Dirección original en Ghidra: 0x0011C8D8 (PAL)
 */
bool sys_sif_init_manager(void) {
	bool interrupt_status = kernel_system_sync_guard();

	if (g_sys_sif_is_initialized != 0) {
		if (interrupt_status) {
			kernel_system_sync_release();
		}
		return true;
	}
	g_sys_sif_is_initialized = 1;

	if (interrupt_status) {
		kernel_system_sync_release();
	}

#if defined(PLATFORM_PS2)
	g_sys_sif_handler_id = sceAddDmacHandler(5, (void*)0x0011C8A0, 0);
#endif
	sys_kernel_enable_dmac();

	u32 handshake_reg = 1;
	if (handshake_reg != 0) {
		g_sys_sif_reg_status = handshake_reg;
		u32 dummy_payload = 0;
		sys_sif_submit_dma_packet_simple(0x80000000, &dummy_payload, 0x14, 0, 0, 0);
		return true;
	}
	return true;
}

/**
 * @brief Registra un puntero de función (Callback) y sus argumentos dentro de la tabla indexada de eventos del bus SIF.
 * Dirección original en Ghidra: 0x0011CB90 (PAL)
 */
void sys_sif_register_callback(long command_id, void* callback_ptr, void* callback_arg) {
	u32 table_base_address = SIF_GENERAL_CALLBACK_TABLE;
	if (command_id < 0) {
		table_base_address = SIF_SYSTEM_CALLBACK_TABLE;
	}
	u32* p_callback_slot = (u32*)(long)((s32)command_id * 8 + table_base_address);
	p_callback_slot[0] = (u32)(long)callback_ptr;
	p_callback_slot[1] = (u32)(long)callback_arg;
}

/**
 * @brief Remueve y desregistra un callback de la tabla indexada de eventos del bus SIF inyectando un puntero nulo.
 * Dirección original en Ghidra: 0x0011CBC0 (PAL)
 */
void sys_sif_unregister_callback(long command_id) {
	u32 table_base_address = SIF_GENERAL_CALLBACK_TABLE;
	if (command_id < 0) {
		table_base_address = SIF_SYSTEM_CALLBACK_TABLE;
	}
	u32* p_callback_slot = (u32*)(long)((s32)command_id * 8 + table_base_address);
	*p_callback_slot = 0;
}

// Definición de banderas globales de control IO mapeadas en la RAM de la PS2
#define IO_INTERRUPT_ACTIVE_FLAG    (*(s32*)0x00136414)
#define IO_THREAD_RESET_DESCRIPTOR  (*(s32*)0x00136454)

/**
 * @brief Apaga, desmantela y libera por completo los recursos y semáforos del subsistema de Entrada/Salida (IO).
 * Remueve el callback -0x7fffffee del bus SIF y destruye los tres semáforos de sincronización física.
 * Dirección original en Ghidra: 0x00124818 (PAL)
 *
 * @return bool Retorna verdadero si el desmantelamiento atómico en el Kernel fue exitoso.
 */
bool sys_io_shutdown_subsystem(void) {
	// 1. Despierta preventivamente los hilos bloqueados antes del apagado
	if (IO_INTERRUPT_ACTIVE_FLAG != 0) {
		IO_THREAD_RESET_DESCRIPTOR = 0xFFFFFFFF;
		sceSignalSema(g_sys_io_wait_sema_id);
	}

	// 2. Destrucción física en cadena de los tres semáforos del subsistema del Kernel
	sceDeleteSema(g_sys_io_lock_sema_id);
	sceDeleteSema(g_sys_io_wait_sema_id);
	sceDeleteSema(g_sys_io_dma_sema_id);

	// Resetea los identificadores globales de control locales para marcar el estado inactivo
	g_sys_io_lock_sema_id = -1;
	g_sys_io_wait_sema_id = -1;
	g_sys_io_dma_sema_id = -1;

	// 3. Exclusión mutua atómica para desregistrar el callback SIF de la cola del IOP
	bool sync_status = kernel_system_sync_guard();

	sys_sif_unregister_callback(-0x7FFFFFEE);

	if (!sync_status) {
		return false;
	}

	kernel_system_sync_release();
	return true;
}

// Definición de las variables globales IO mapeadas en la RAM de la PS2
#define IO_RECONFIG_FLAG            (*(s32*)0x00136424)
#define IO_IS_READY_FLAG            (*(s32*)0x0013643C)

/**
 * @brief Inicializa y configura el canal de servicios de Entrada/Salida (IO) asíncronos en el Kernel.
 * Registra el callback -0x7fffffee en la tabla del bus SIF bajo exclusión mutua atómica.
 * Dirección original en Ghidra: 0x001248F8 (PAL)
 *
 * @return s32 Código de estado de éxito (1).
 */
s32 sys_io_init_subsystem(void) {
	// 1. Marca el estado de reconfiguración física en la RAM
	IO_RECONFIG_FLAG = 1;

	// 2. Protege el bus registrando el manejador de interrupciones del IOP de forma Thread-Safe
	bool sync_status = kernel_system_sync_guard();

	sys_sif_register_callback(-0x7FFFFFEE, (void*)sys_io_iop_interrupt_handler, NULL);

	if (sync_status) {
		kernel_system_sync_release();
	}

	// 3. Libera el flag de configuración y enciende la bandera de disponibilidad del canal
	IO_RECONFIG_FLAG = 0;
	IO_IS_READY_FLAG = 1;

	return 1; // Inicialización exitosa del pipeline
}

// Definición de registros y buffers estáticos de la lectora de DVD mapeados en la RAM de la PS2
#define CDVD_INIT_MODE_BUFFER_PTR   (*(u32*)0x00141B40)
#define CDVD_BACKUP_METADATA_1      (*(u32*)0x00136440)
#define CDVD_BACKUP_METADATA_2      (*(u32*)0x00136438)
#define CDVD_BACKUP_METADATA_3      (*(u32*)0x00136448)
#define CDVD_BACKUP_METADATA_4      (*(u32*)0x00136444)
#define CDVD_BACKUP_STATUS_1        (*(u32*)0x00136434)
#define CDVD_BACKUP_STATUS_2        (*(s32*)0x0013644C)
#define DEBUG_NET_LOG_LEVEL         (*(s32*)0x00136410)

// Variables globales del subsistema de DVD mapeadas desde Ghidra
s32 g_sys_cdvd_thread_owner_id = 0;
s32 g_sys_cdvd_init_attempts_count = 0;
u32 g_sys_cdvd_channel_widget_handle = 0;
s32 g_sys_cdvd_is_bound_flag = 0;

/**
 * @brief Inicializa y monta el sistema de archivos de la lectora de DVD (libcdvd) a través de transacciones SIF RPC.
 * Registra el canal 0x80000592 y orquesta el encendido o apagado en caliente de la fontanería de Entrada/Salida.
 * Dirección original en Ghidra: 0x00124E08 (PAL)
 */
u32 sys_cdvd_init_filesystem(s32 init_mode) {
	// 1. Verifica la disponibilidad del bus de sincronización
	s32 is_sound_busy = sys_sound_sync_command_guard(1, 0, 0, 0, 0, 0, 0, 0);
	u32 status_code = 0;

	if (is_sound_busy == 0) {
		sys_sif_rpc_init_client();

		g_sys_cdvd_thread_owner_id = sceGetThreadId();
		g_sys_io_reconfig_flag = 1;
		g_sys_cdvd_init_attempts_count = g_sys_cdvd_init_attempts_count + 1;

		// Inicialización en ráfaga contigua de máscaras de control de fábrica
		g_sys_io_is_ready_flag = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_1 = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_2 = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_3 = 0xFFFFFFFF;
		CDVD_BACKUP_METADATA_4 = 0xFFFFFFFF;
		CDVD_BACKUP_STATUS_1 = 0;
		CDVD_BACKUP_STATUS_2 = 0xFFFFFFFF;

		// 2. LAZO DE ESPERA DE ENLACE: Acopla el canal exclusivo de la lectora (Comando 0x80000592)
		while (1) {
			while (1) {
				s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_cdvd_channel_widget_handle, 0x80000592, 0);
				if (session_status >= 0) {
					break;
				}

				// Si la lectora física experimenta demoras, lanza el log de pánico
				if (DEBUG_NET_LOG_LEVEL > 0) {
					boot_txt_render_extended_string((const u8*)"Libcdvd bind err %d CD_Init %d\n", session_status, g_sys_cdvd_init_attempts_count, 0, 0, 0, 0, 0);
				}

				s32 delay_counter = 0x100000;
				while (delay_counter != -1) { delay_counter--; }
			}

			if (g_sys_cdvd_is_bound_flag != 0) {
				break;
			}

			s32 delay_counter = 0x100000;
			while (delay_counter != -1) { delay_counter--; }
		}

		CDVD_BACKUP_STATUS_2 = 0;
		CDVD_INIT_MODE_BUFFER_PTR = (u32)init_mode;

		// Asegura la coherencia física del buffer de modo antes de despachar
		sys_kernel_flush_dcache_range(0x00141B40, 4);

		// 3. DESPACHO DEL COMANDO DE MONTAJE: Transfiere el bloque de inicialización de la lectora (Comando 0)
		s32 transaction_status = sys_sif_rpc_send_transaction_data(
			&g_sys_cdvd_channel_widget_handle,
			0, 0, 0x00141B40, 4, 0x00137600, 0x10, 0, 0
		);

		// 4. BIFURCACIÓN DE FASES DEL ENTORNO DE ENTRADA/SALIDA
		if (transaction_status < 0) {
			g_sys_io_reconfig_flag = 0;
			status_code = 0;
		}
		else {
			g_sys_io_reconfig_flag = 0;
			status_code = 2;

			// Si el modo no es un apagado en caliente (Exit Mode = 5), levanta las compuertas IO
			if ((init_mode < 0 || init_mode < 2) || init_mode != 5) {
				sys_io_init_kernel_semaphores();
				sys_io_init_subsystem();
			}
			// Si el motor ordena expulsar o apagar el servicio de la lectora, desmonta el sistema
			else {
				if (DEBUG_NET_LOG_LEVEL > 0) {
					boot_txt_render_extended_string((const u8*)"Libcdvd Exit\n", 0, 0xFFFFFFFF, 0, 0, 0, 0, 0);
				}
				sys_io_shutdown_subsystem();
				g_sys_io_lock_sema_id = 0xFFFFFFFF;
				g_sys_io_wait_sema_id = 0xFFFFFFFF;
				g_sys_io_dma_sema_id = 0xFFFFFFFF;
			}
		}
	}

	return status_code;
}

// Definición de registros y buffers estáticos del chequeo de disco mapeados en la RAM de la PS2
#define CDVD_READY_MODE_BUFFER_VAL  (*(u32*)0x00141B50)
#define CDVD_READY_STATUS_BACKUP    (*(s32*)0x00136444)

// Variables globales del canal secundario de libcdvd mapeadas desde Ghidra
u32 g_sys_cdvd_ready_channel_handle = 0;
s32 g_sys_cdvd_ready_is_bound_flag = 0;

/**
 * @brief Interroga el estado de preparación y presencia física del disco en la lectora (sceCdDiskReady wrapper).
 * Abre el canal asíncrono 0x8000059A y despacha de forma síncrona el comando de estado al IOP.
 * Dirección original en Ghidra: 0x001250E8 (PAL)
 *
 * @param check_mode Modo de verificación de hardware enviado al lector de Sony (param_1).
 * @return u32 Estado de lectura (0 para éxito rotundo / medio listo, 6 para espera activa, 0xFFFFFFFF para error).
 */
u32 sys_cdvd_check_disk_ready(long check_mode) {
	// 1. Log de diagnóstico de inicio del sensor
	if (DEBUG_NET_LOG_LEVEL > 0) {
		boot_txt_render_extended_string((const u8*)"DiskReady 0\n", 0, 0, 0, 0, 0, 0, 0);
	}

	sys_io_init_kernel_semaphores();
	s32 sema_status = scePollSema(g_sys_io_wait_sema_id);
	u32 return_value = 6; // Estado por defecto: Espera activa / Reintentar

	if (g_sys_io_wait_sema_id == sema_status) {
		s32 is_sound_busy = sys_sound_sync_command_guard(1, 0, 0, 0, 0, 0, 0, 0);

		if (is_sound_busy == 0) {
			sys_sif_rpc_init_client();

			// 2. LAZO DE ESPERA DE ENLACE: Acopla el canal secundario de la lectora (Comando 0x8000059A)
			if (CDVD_READY_STATUS_BACKUP < 0) {
				while (1) {
					while (1) {
						s32 session_status = sys_sif_rpc_open_transaction_session(&g_sys_cdvd_ready_channel_handle, 0x8000059A, 0);
						if (session_status >= 0) {
							break;
						}

						if (DEBUG_NET_LOG_LEVEL > 0) {
							boot_txt_render_extended_string((const u8*)"Libcdvd bind err CdDiskReady\n", 0, 0, 0, 0, 0, 0, 0);
						}

						s32 delay_counter = 0x100000;
						while (delay_counter != -1) { delay_counter--; }
					}

					if (g_sys_cdvd_ready_is_bound_flag != 0) {
						break;
					}

					s32 delay_counter = 0x100000;
					while (delay_counter != -1) { delay_counter--; }
				}
				CDVD_READY_STATUS_BACKUP = 0;
			}

			// 3. DESPACHO DEL COMANDO SIF: Transfiere el código de modo (Offset 0x141B50)
			CDVD_READY_MODE_BUFFER_VAL = (u32)check_mode;
			sys_kernel_flush_dcache_range(0x00141B50, 4);

			s32 transaction_status = sys_sif_rpc_send_transaction_data(
				&g_sys_cdvd_ready_channel_handle,
				0, 0, 0x00141B50, 4, 0x00137600, 4, 0, 0
			);

			// 4. Si la lectora confirma que el medio físico está girando e íntegro, retorna éxito
			if (transaction_status > -1) {
				if (DEBUG_NET_LOG_LEVEL > 0) {
					boot_txt_render_extended_string((const u8*)"DiskReady ended\n", 0, 0, 0, 0, 0, 0, 0);
				}
				sceSignalSema(g_sys_io_wait_sema_id);
				return 0; // El disco está listo para transferir datos
			}
		}

		// Caso de escape o canal congestionado, libera el semáforo para evitar bloqueos mutuos
		sceSignalSema(g_sys_io_wait_sema_id);
		return_value = 6;
		if (check_mode == 8) {
			return_value = 0xFFFFFFFF; // Código de error crítico del lector de Sony
		}
	}

	// Para efectos del port nativo moderno a PC, donde los archivos locales están en el disco duro,
	// interceptamos y forzamos éxito absoluto de forma automática para dar paso directo a las lecturas:
#if !defined(PLATFORM_PS2)
	return_value = 0;
#endif

	return return_value;
}

// Definición de las variables de descriptor de lectura mapeadas en la RAM de la PS2
#define MC_READ_FD_VAL              (*(u32*)0x00141C04)
#define MC_READ_SIZE_VAL            (*(u32*)0x00141C08)
#define MC_READ_BUFFER_PTR          (*(u32*)0x00141BA8)
#define MC_READ_LEN_VAL             (*(u32*)0x00141BAC)
#define MC_READ_OFFSET_VAL          (*(u32*)0x00141BB0)

/**
 * @brief Envía el comando de lectura en bloque de un archivo de la Memory Card (Comando 1) al bus de hardware.
 * Configura los buffers de destino, offsets y tamaños aplicando flushes dcache e interrupciones síncronas.
 * Dirección original en Ghidra: 0x00127CC0 (PAL)
 *
 * @param file_descriptor Identificador de archivo (FD) obtenido previamente con sceMcOpen (param_1).
 * @param read_size Cantidad de bytes máximos solicitados para la lectura de la partida (param_2).
 * @param p_dest_buffer Puntero de la memoria RAM donde se depositarán los datos leídos (param_3).
 * @param block_len Longitud en bytes del bloque de ráfaga física (param_4).
 * @param offset_pos Desplazamiento o alineación de bytes del cursor dentro del archivo (param_5).
 * @return s32 Código de estado (0 para comando inyectado con éxito en el bus, valores negativos para error).
 */
s32 sceMcRead(u32 file_descriptor, u32 read_size, long p_dest_buffer, long block_len, long offset_pos) {
	s32 status_code;

	// 1. Validar que el subsistema de la Memory Card esté formalmente levantado
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protege el bus realizando un sondeo no bloqueante sobre el semáforo de exclusión mutua
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	if (sema_status < 0) {
		return -200;
	}

	// 3. Vuelca en ráfaga contigua los parámetros de lectura en la sección de datos estáticos
	*(u32*)0x00141C1C = 0x00142080; // Dirección base de la tabla de control
	*(u32*)0x00141C14 = (u32)(p_dest_buffer != 0);
	*(u32*)0x00141C10 = (u32)(block_len != 0);
	*(u32*)0x00141C0C = (u32)(offset_pos != 0);

	MC_READ_BUFFER_PTR = (u32)p_dest_buffer;
	MC_READ_LEN_VAL = (u32)block_len;
	MC_READ_OFFSET_VAL = (u32)offset_pos;
	MC_READ_FD_VAL = file_descriptor;
	MC_READ_SIZE_VAL = read_size;

	// Sincroniza el búfer de control físico hacia la memoria principal (0xC0 = 192 bytes)
	sys_kernel_flush_dcache_range(0x00142080, 0xC0);

	// Despacha la orden mediante la ráfaga Comando 1 (Síncrona prioritaria = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		1, 1, 0x141C00, 0x30, 0x143140, 4, 0x127C68, (u32)0x00142080
	);

	// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 1;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}

// Definición de las variables de descriptor de escritura mapeadas en la RAM de la PS2
#define MC_WRITE_FD_VAL             (*(u32*)0x00141C00)
#define MC_WRITE_SRC_PTR            (*(u32*)0x00141C18)
#define MC_WRITE_SIZE_VAL           (*(u32*)0x00141C0C)

/**
 * @brief Envía el comando de escritura en bloque de un archivo hacia la Memory Card (Comando 5) al bus de hardware.
 * Configura las direcciones de origen de la RAM, longitudes de ráfaga y aplica flushes dobles de dcache de seguridad.
 * Dirección original en Ghidra: 0x00127888 (PAL)
 *
 * @param file_descriptor Identificador de archivo (FD) obtenido previamente con sceMcOpen (param_1).
 * @param src_ram_addr Dirección de la memoria RAM del juego desde donde se leerán los datos a guardar (param_2).
 * @param write_size Cantidad exacta de bytes binarios que se van a inyectar y grabar en la tarjeta (param_3).
 * @return s32 Código de estado (0 para comando aceptado en el bus, valores negativos para error).
 */
s32 sceMcWrite(u32 file_descriptor, u32 src_ram_addr, long write_size) {
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

	// 3. Vuelca en ráfaga contigua los parámetros de escritura en la sección de datos estáticos
	*(u32*)0x00141C1C = 0x00142080; // Dirección de la superestructura de control
	MC_WRITE_SIZE_VAL = (u32)write_size;
	MC_WRITE_FD_VAL = file_descriptor;
	MC_WRITE_SRC_PTR = src_ram_addr;

	// DOBLE BARRERA DE COHERENCIA DE MEMORIA (D-CACHE FLUSH)
	// Sincroniza el búfer de origen de datos lúdicos del juego
	sys_kernel_flush_dcache_range(src_ram_addr, write_size);
	// Sincroniza el búfer estructural de control del propio kernel (0xC0 = 192 bytes)
	sys_kernel_flush_dcache_range(0x00142080, 0xC0);

	// Despacha la orden mediante la ráfaga Comando 5 (Síncrona prioritaria = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		5, 1, 0x141C00, 0x30, 0x143140, 4, 0x1277F8, (u32)0x00142080
	);

	// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 5;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}

// Definición de las variables de descriptor de escritura extendidas en la RAM de la PS2
#define MC_EXT_ALIGNMENT_OFFSET    (*(u32*)0x00141C14)
#define MC_EXT_ALIGNED_BUFFER_PTR  ((u8*)0x00141C20)

/**
 * @brief Crea un nuevo directorio o carpeta de trabajo activo dentro de la Memory Card (Comando 0x11).
 * Configura el slot y la ruta de la carpeta enviando la orden de forma síncrona prioritaria al IOP.
 * Dirección original en Ghidra: 0x00128180 (PAL)
 *
 * @param slot_index Ranura de la tarjeta a interrogar (0 = Slot 1, 1 = Slot 2) (param_1).
 * @param p_dir_path Cadena de texto con el nombre de la carpeta a crear (param_2).
 * @return s32 Código de estado (0 para comando aceptado en el bus, valores negativos para error).
 */
s32 sceMcMkdir(u32 slot_index, const char* p_dir_path) {
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

	// 3. Vuelca en ráfaga contigua los parámetros de creación en la sección de datos estáticos
	g_sys_mc_read_fd = slot_index;
	g_sys_mc_read_size = (u32)(uintptr_t)p_dir_path; // Reutiliza el buffer DAT_00141c08 para el puntero de texto

	// Despacha la orden mediante la ráfaga Comando 0x11 (Síncrona prioritaria = 1)
	status_code = sys_sif_rpc_send_transaction_data(
		&g_sys_mc_channel_widget_handle,
		0x11, 1, 0x141C00, 0x30, 0x143140, 4, 0, 0
	);

	// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
	if (status_code == 0) {
		g_sys_mc_active_command_id = 0x11;
	}
	else {
		sceSignalSema(g_sys_mc_mutex_sema_id);
	}

	return status_code;
}

// Definición de las variables de descriptor de borrado mapeadas en la RAM de la PS2
#define MC_DELETE_SLOT_VAL          (*(u32*)0x00141C30)
#define MC_DELETE_CONTEXT_VAL       (*(u32*)0x00141C34)
#define MC_DELETE_FILENAME_BUFFER   ((u8*)0x00141C44)

/**
 * @brief Envía el comando de borrado de un archivo en la Memory Card (Comando 0x0F) al bus de hardware.
 * Valida la integridad del nombre del archivo, ejecuta una copia segura y despacha la transacción al IOP.
 * Dirección original en Ghidra: 0x00128068 (PAL)
 *
 * @param slot_index Ranura de la tarjeta a interrogar (0 = Slot 1, 1 = Slot 2) (param_1).
 * @param context_val Parámetro numérico de contexto secundario del SDK (param_2).
 * @param p_filename_path Cadena de texto con el nombre del archivo binario a eliminar de la tarjeta (param_3).
 * @return s32 Código de estado (0 para comando aceptado en el bus, valores negativos para error).
 */
s32 sceMcDelete(u32 slot_index, u32 context_val, const char* p_filename_path) {
	s32 status_code;

	// 1. Validar que el subsistema de la Memory Card esté formalmente levantado
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protege el bus realizando un sondeo no bloqueante sobre el semáforo del canal
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	s32 is_busy_err = -200;

	if (sema_status > -1) {
		// 3. Verificación estricta de la string del archivo (Filtro de seguridad SCE_MC_ERR_NAME)
		if (p_filename_path == NULL || p_filename_path[0] == '\0') {
			sceSignalSema(g_sys_mc_mutex_sema_id);
			return -0xD2; // Error: Nombre de archivo inválido o nulo
		}

		// Ejecuta la copia segura utilizando el clon local de strncpy (límite 0x3FF bytes)
		sys_strncpy_safe((u32)(uintptr_t)MC_DELETE_FILENAME_BUFFER, p_filename_path, 0x3FF);

		// Limpia metadatos contigüos de limpieza de la estructura física del kernel de Sony
		*(u8*)0x00142043 = 0; // DAT_00142043
		*(u32*)0x00141C38 = 0; // DAT_00141c38

		// Vuelca los descriptores de borrado en la sección de datos estáticos
		MC_DELETE_SLOT_VAL = slot_index;
		MC_DELETE_CONTEXT_VAL = context_val;

		// Despacha la orden mediante la ráfaga Comando 0x0F (Síncrona prioritaria = 1, tamaño de bloque 0x414)
		is_busy_err = sys_sif_rpc_send_transaction_data(
			&g_sys_mc_channel_widget_handle,
			0x0F, 1, 0x141C30, 0x414, 0x143140, 4, 0, 0
		);

		// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
		if (is_busy_err == 0) {
			g_sys_mc_active_command_id = 0x0F;
		}
		else {
			sceSignalSema(g_sys_mc_mutex_sema_id);
		}
	}

	return is_busy_err;
}

// Definiciones de los offsets de buffers compartidos de la Memory Card (ya mapeados en la suite)
#define MC_GETDIR_SLOT_VAL          (*(u32*)0x00141C30)
#define MC_GETDIR_CONTEXT_VAL       (*(u32*)0x00141C34)
#define MC_GETDIR_MAX_ENTRIES       (*(u32*)0x00141C38)
#define MC_GETDIR_PATTERN_BUFFER    ((u8*)0x00141C44)

/**
 * @brief Envía el comando de escaneo y listado de directorios de la Memory Card (Comando 2) al bus de hardware.
 * Configura los patrones de búsqueda, límites de entradas y despacha la transacción de forma síncrona al IOP.
 * Dirección original en Ghidra: 0x00127508 (PAL)
 *
 * @param slot_index Ranura de la tarjeta a interrogar (0 = Slot 1, 1 = Slot 2) (param_1).
 * @param context_val Parámetro numérico de contexto secundario del SDK (param_2).
 * @param p_search_pattern Cadena de texto con el patrón o filtro de archivos a escanear (param_3).
 * @param max_entries Cantidad máxima de registros o entradas a listar en la transacción (param_4).
 * @return s32 Código de estado (0 para comando inyectado con éxito en el bus, valores negativos para error).
 */
s32 sceMcGetDir(u32 slot_index, u32 context_val, const char* p_search_pattern, u32 max_entries) {
	s32 status_code;

	// 1. Validar que el subsistema de la Memory Card esté formalmente levantado
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protege el bus realizando un sondeo no bloqueante sobre el semáforo del canal
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	s32 is_busy_err = -200;

	if (sema_status > -1) {
		// 3. Verificación de seguridad de la string (Filtro SCE_MC_ERR_NAME)
		if (p_search_pattern == NULL || p_search_pattern == '\0') {
			sceSignalSema(g_sys_mc_mutex_sema_id);
			return -0xD2; // Error: Patrón de búsqueda inválido o vacío
		}

		// Ejecuta la copia segura en el búfer compartido utilizando tu utilería vectorial
		sys_strncpy_safe(0x00141C44, p_search_pattern, 0x3FF);

		// Limpia metadatos contiguos de la estructura física de control de Sony
		*(u8*)0x00142043 = 0;

		// Vuelca los descriptores de escaneo en la sección de datos estáticos
		MC_GETDIR_SLOT_VAL = slot_index;
		MC_GETDIR_CONTEXT_VAL = context_val;
		MC_GETDIR_MAX_ENTRIES = max_entries;

		// Despacha la orden mediante la ráfaga Comando 2 (Síncrona prioritaria = 1, tamaño 0x414)
		is_busy_err = sys_sif_rpc_send_transaction_data(
			&g_sys_mc_channel_widget_handle,
			2, 1, 0x141C30, 0x414, 0x143140, 4, 0, 0
		);

		// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
		if (is_busy_err == 0) {
			g_sys_mc_active_command_id = 2;
		}
		else {
			sceSignalSema(g_sys_mc_mutex_sema_id);
		}
	}

	return is_busy_err;
}

/**
 * @brief Envía el comando de verificación estructural y validación de formato de la Memory Card (Comando 11).
 * Envuelve a sceMcGetDir fijando el límite de entradas en 64 y enmascara el ID de comando activo a 0x0B.
 * Dirección original en Ghidra: 0x00127630 (PAL)
 *
 * @param slot_index Ranura de la tarjeta a interrogar (0 = Slot 1, 1 = Slot 2) (param_1).
 * @param context_val Parámetro numérico de contexto secundario del SDK (param_2).
 * @param p_dir_path Cadena de texto con la ruta del directorio base a verificar (param_3).
 * @return s32 Código de estado (0 para comando aceptado en el bus, valores negativos para error).
 */
s32 sceMcCheckMc(u32 slot_index, u32 context_val, const char* p_dir_path) {
	// Redirige los parámetros forzando de forma fija el límite de 64 entradas (0x40)
	s32 status_code = sceMcGetDir(slot_index, context_val, p_dir_path, 0x40);

	// Si la inyección en el bus fue exitosa, enmascara el ID al comando de verificación 11 (0x0B)
	if (status_code == 0) {
		g_sys_mc_active_command_id = 0x0B;
	}

	return status_code;
}

// Definiciones de los offsets de buffers extendidos de formateo (ya mapeados en la suite)
#define MC_FORMAT_SLOT_VAL          (*(u32*)0x00141C30)
#define MC_FORMAT_CONTEXT_VAL       (*(u32*)0x00141C34)
#define MC_FORMAT_MAX_ENTRIES       (*(u32*)0x00141C38)
#define MC_FORMAT_CLUSTERS_VAL      (*(s32*)0x00141C3C)
#define MC_FORMAT_FAT_BUFFER_PTR    (*(u32*)0x00141C40)
#define MC_FORMAT_PATTERN_BUFFER    ((u8*)0x00141C44)

/**
 * @brief Envía el comando de formateo e inicialización estructural de la Memory Card (Comando 0x0D) al bus de hardware.
 * Aplica alineación bitwise de 64 bytes para el búfer FAT e inyecta la orden de forma síncrona prioritaria al IOP.
 * Dirección original en Ghidra: 0x00127E48 (PAL)
 */
s32 sceMcFormat(u32 slot_index, u32 context_val, const char* p_dir_path, u32 max_entries, long clusters_count, u32 fat_buffer_addr) {
	s32 status_code;

	// 1. Validar que el subsistema de la Memory Card esté formalmente levantado
	if (g_sys_mc_is_bound_flag == 0) {
		return -100;
	}

	// 2. Protege el bus realizando un sondeo no bloqueante sobre el semáforo del canal
	long sema_status = (long)scePollSema(g_sys_mc_mutex_sema_id);
	s32 is_busy_err = -200;

	if (sema_status > -1) {
		// 3. Verificación de seguridad de la string (Filtro de seguridad SCE_MC_ERR_NAME)
		if (p_dir_path == NULL || p_dir_path == '\0') {
			sceSignalSema(g_sys_mc_mutex_sema_id);
			return -0xD2;
		}

		// Vuelca los descriptores de formateo extendidos en la sección de datos estáticos
		MC_FORMAT_SLOT_VAL = slot_index;
		MC_FORMAT_CONTEXT_VAL = context_val;
		MC_FORMAT_MAX_ENTRIES = max_entries;
		MC_FORMAT_CLUSTERS_VAL = (s32)clusters_count;
		MC_FORMAT_FAT_BUFFER_PTR = fat_buffer_addr;

		// Ejecuta la copia segura en el búfer compartido utilizando tu utilería vectorial
		sys_strncpy_safe(0x00141C44, p_dir_path, 0x3FF);

		// Limpia metadatos contiguos de la estructura física de control de Sony
		*(u8*)0x00142043 = 0;

		// BARRERA DE COHERENCIA MULTIPLICADA (Capacidad << 6 equivale a multiplicar por 64 bytes)
		if (clusters_count > -1) {
			long calculated_bytes_len = (long)((s32)clusters_count << 6);
			sys_kernel_flush_dcache_range(fat_buffer_addr, calculated_bytes_len);
		}

		// Despacha la orden mediante la ráfaga Comando 0x0D (Síncrona prioritaria = 1, tamaño 0x414)
		is_busy_err = sys_sif_rpc_send_transaction_data(
			&g_sys_mc_channel_widget_handle,
			0x0D, 1, 0x141C30, 0x414, 0x143140, 4, 0, 0
		);

		// 4. Si la inyección en el bus SIF fue exitosa, firma el comando activo en la RAM
		if (is_busy_err == 0) {
			g_sys_mc_active_command_id = 0x0D;
		}
		else {
			sceSignalSema(g_sys_mc_mutex_sema_id);
		}
	}

	return is_busy_err;
}
