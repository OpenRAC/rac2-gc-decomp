#include "cdvd.h"
#include "ps2_kernel.h"
#include "system.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int g_CdvdNcmdInitialized = -1; // -1 means NOT initialized, as on the PS2
int g_CdvdCurrentCommand = 0;

int sceCdInit(int mode) {
	// 1. If it was already initialized, return success directly
	if (g_CdvdNcmdInitialized > -1) {
		return 1;
	}

	// 2. Safely simulate the I/O kernel checks
	// sys_io_init_kernel_semaphores();
	// On PC, assume our graphics/render semaphores already control the flow

	// Successful simulation of scePollSema() for the I/O lock
	g_CdvdCurrentCommand = mode;

	// 3. Skip the 'while(true)' loops that open the SIF RPC session entirely.
	// On a real PS2 this blocked waiting for the physical hardware.
	// On PC, force immediate success.
	LOG_SUCCESS("CDVD", "File system mapped correctly. Original mode: %d\n", mode);
	//printf("[CDVD] File system mapped correctly. Original mode: %d\n", mode);

	// 4. Mark the subsystem as READY
	g_CdvdNcmdInitialized = 0;

	return 1; // Returns 1 (reader start-up fully successful)
}

int sceCdStop(void) {
	// 1. Force initialization in mode 2, as the game does
	sceCdInit(2);

	// 2. Skip SIF transaction 0x0E (hardware Stop/Standby)
	// On PC there are no mechanical parts to slow down.

	// 3. Emulate releasing the Input/Output bus semaphore
	// to stay consistent with the lock opened by sceCdInit.
	// Use the simulated virtual I/O ID if it is wired up in ps2_kernel.c
	// sceSignalSema(g_sys_io_lock_sema_id);

	LOG_INFO("CDVD", "Stop/Standby command handled natively.\n");

	return 0; // Official return value of the Sony stub
}

// Global pointer to the large data container extracted from the ISO
static FILE* g_GameDataFile = NULL;
static FILE* g_CurrentWadFile = NULL;
static char g_ActiveWadPath[256] = "";

int sceCdRead(unsigned int sector_start, int sector_count, unsigned int dest_buffer, unsigned char* mode_struct) {
	LOG_INFO("CDVD", "Read request: first sector %u | count: %d sectors.", sector_start, sector_count);

	sceCdStop();
	sceCdInit(4);

	if (sector_count <= 0) return 0;

	size_t bytes_to_read = (size_t)sector_count * 2048;
	void* real_pc_destination = (void*)(uintptr_t)dest_buffer;

	if (real_pc_destination == NULL) return 0;

	// --- PROTECTED LOCAL LINKING SYSTEM ---
	// The decompiled engine will look for sectors. A later step will map
	// RC2.HDR to know which exact .wad of 'orig/G/' each sector belongs to.
	// For now, default to the main container for loading the intro.
	if (g_CurrentWadFile == NULL) {
		snprintf(g_ActiveWadPath, sizeof(g_ActiveWadPath), "orig/G/audio0.wad"); // Example local path in orig/
		g_CurrentWadFile = fopen(g_ActiveWadPath, "rb");

		if (g_CurrentWadFile == NULL) {
			// Fall back to the boot executable if the game requests sectors of the main binary
			snprintf(g_ActiveWadPath, sizeof(g_ActiveWadPath), "orig/SCES_516.07");
			g_CurrentWadFile = fopen(g_ActiveWadPath, "rb");
		}
	}

	if (g_CurrentWadFile != NULL) {
		// Multiply the original sector by 2048 bytes to seek within the protected file
		long long byte_offset = (long long)sector_start * 2048;

		fseek(g_CurrentWadFile, byte_offset, SEEK_SET);
		size_t bytes_read = fread(real_pc_destination, 1, bytes_to_read, g_CurrentWadFile);

		if (bytes_read > 0) {
			LOG_SUCCESS("CDVD", "Reading from: %s | %zu bytes loaded from sector %u.\n", g_ActiveWadPath, bytes_read, sector_start);
			return 1; // Successfully copied into PC RAM
		}
	}

	LOG_ERROR("CDVD", "Could not read sector %u from the protected local path.\n", sector_start);
	return 0;
}