#include "cdvd.h"
#include "ps2_kernel.h"
#include "system.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int g_CdvdNcmdInitialized = -1; // -1 significa NO inicializado, tal como la PS2
int g_CdvdCurrentCommand = 0;

int sceCdInit(int mode) {
	// 1. Si ya estaba inicializado previamente, retornamos éxito directo
	if (g_CdvdNcmdInitialized > -1) {
		return 1;
	}

	// 2. Simulamos de forma segura las comprobaciones del Kernel de E/S
	// sys_io_init_kernel_semaphores();
	// En PC, asumimos que nuestros semáforos de gráficos/render ya controlan el flujo

	// Simulación exitosa de scePollSema() para el candado de I/O
	g_CdvdCurrentCommand = mode;

	// 3. Omitimos por completo los bucles 'while(true)' de apertura de sesión RPC SIF.
	// En la PS2 real, esto se colgaba esperando al hardware físico.
	// En PC, forzamos el estado de éxito instantáneo.
	LOG_SUCCESS("CDVD", "Sistema de archivos mapeado correctamente. Modo original: %d\n", mode);
	//printf("[CDVD] Sistema de archivos mapeado correctamente. Modo original: %d\n", mode);

	// 4. Marcamos el subsistema como LISTO
	g_CdvdNcmdInitialized = 0;

	return 1; // Retorna 1 (Éxito total en el arranque del lector)
}

int sceCdStop(void) {
	// 1. Forzamos la inicialización en modo 2 tal como hace el juego
	sceCdInit(2);

	// 2. Omitimos la transacción SIF 0x0E (Stop/Standby de hardware)
	// En PC no hay partes mecánicas que desacelerar.

	// 3. Emulamos la liberación del semáforo del bus de Entrada/Salida 
	// para mantener la coherencia con el candado que abrió sceCdInit.
	// Usamos el ID de I/O virtual simulado si lo tienes acoplado en ps2_kernel.c
	// sceSignalSema(g_sys_io_lock_sema_id);

	LOG_INFO("CDVD", "Comando de reposo (Stop/Standby) procesado de forma nativa.\n");

	return 0; // Retorno oficial del stub de Sony
}

// Puntero global al gran contenedor de datos extraído de tu ISO
static FILE* g_GameDataFile = NULL;
static FILE* g_CurrentWadFile = NULL;
static char g_ActiveWadPath[256] = "";

int sceCdRead(unsigned int sector_start, int sector_count, unsigned int dest_buffer, unsigned char* mode_struct) {
	LOG_INFO("CDVD", "Solicitud de lectura: Sector inicial %u | Cantidad: %d sectores.", sector_start, sector_count);

	sceCdStop();
	sceCdInit(4);

	if (sector_count <= 0) return 0;

	size_t bytes_to_read = (size_t)sector_count * 2048;
	void* real_pc_destination = (void*)(uintptr_t)dest_buffer;

	if (real_pc_destination == NULL) return 0;

	// --- SISTEMA DE ENLAZADO LOCAL PROTEGIDO ---
	// El motor descompilado buscará sectores. En un paso posterior mapearemos 
	// RC2.HDR para saber a qué .wad exacto de 'orig/G/' pertenece cada sector.
	// Por ahora, apuntamos por defecto al contenedor principal para la carga de la intro.
	if (g_CurrentWadFile == NULL) {
		snprintf(g_ActiveWadPath, sizeof(g_ActiveWadPath), "orig/G/audio0.wad"); // Ejemplo de ruta local en orig/
		g_CurrentWadFile = fopen(g_ActiveWadPath, "rb");

		if (g_CurrentWadFile == NULL) {
			// Fallback al ejecutable base si el juego pide sectores del binario principal
			snprintf(g_ActiveWadPath, sizeof(g_ActiveWadPath), "orig/SCES_516.07");
			g_CurrentWadFile = fopen(g_ActiveWadPath, "rb");
		}
	}

	if (g_CurrentWadFile != NULL) {
		// Multiplicamos el sector original por 2048 bytes para posicionarnos en tu archivo protegido
		long long byte_offset = (long long)sector_start * 2048;

		fseek(g_CurrentWadFile, byte_offset, SEEK_SET);
		size_t bytes_read = fread(real_pc_destination, 1, bytes_to_read, g_CurrentWadFile);

		if (bytes_read > 0) {
			LOG_SUCCESS("CDVD", "Leyendo desde: %s | %zu bytes cargados desde el sector %u.\n", g_ActiveWadPath, bytes_read, sector_start);
			return 1; // Éxito de volcado en la RAM de PC
		}
	}

	LOG_ERROR("CDVD", "No se pudo leer el sector %u en la ruta local protegida.\n", sector_start);
	return 0;
}