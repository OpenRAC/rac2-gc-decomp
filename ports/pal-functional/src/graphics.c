#include "graphics.h"
#include "core/sce_compat.h"

int Graphics_InitSifInterface(void);
#include "system.h"
#include "ps2_kernel.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

extern int g_GraphicsSifInitialized;
extern unsigned int g_GraphicsVideoFormat;
static unsigned char g_Ps2ScratchpadMemory[0x200] = { 0 };

// Add this line to the global variable section at the very top:
unsigned char g_GraphicsIopCommandBuffers[0x440 * 4] = { 0 };

// Global variables identified in the graphics SIF
unsigned int g_SifClientStructure[16] = { 0 }; // Maps DAT_00140100
int g_SifSessionReady = 1;                   // Maps DAT_00140124 (forced to '1' to break the loop on PC)
int g_GraphicsSifInitialized = 0;            // Maps DAT_001347ac
unsigned int g_GraphicsVideoFormat = 0;      // Maps DAT_001347b0

// Initialize the virtual video modes (control numbers can be used for PC)
unsigned int g_VideoMode_Current = 0;
unsigned int g_VideoMode_Target = 0;
unsigned int g_VideoMode_Fallback = 0;

// Instances of the identified global variables
char g_GraphicsResourcePath[1024] = { 0 }; // Maps DAT_0013ea14
unsigned int g_GraphicsCanvasFlags = 0;   // Maps DAT_0013ea0c
unsigned int g_GraphicsCanvasParam3 = 0;  // Maps DAT_0013ea10
int g_GraphicsScratchpadIndex = 0;        // Maps DAT_0013ee14

int g_GraphicsTempSemaID = 0;             // Maps DAT_0013ea00
int g_GraphicsContextState = 0;           // Maps DAT_0013ea08
void* g_GraphicsStackBufferPtr = NULL;     // Maps DAT_0013ea04

unsigned long long sceGsDefDispEnv(unsigned long long* out_env, short mode_flags, short width, short height, short dx, short dy) {
	LOG_SUCCESS("GRAPHICS", "Graphics Synthesizer intercepted: %dx%d (original mode: %d)", width, height, mode_flags);
	if (out_env == NULL) return 0;

	// 1. Run the synchronization guards cleaned up in the previous steps
	Sys_CheckConsoleVersion();
	sceFlushCache(0, NULL, 0);

	// 2. Intercept the dimensions originally requested by the Ratchet & Clank 2 engine
	g_GraphicsCanvasData.width_native = (int)width;
	g_GraphicsCanvasData.height_native = (int)height;

	// --- MODERN PC IMPROVEMENT (HIGH RESOLUTION) ---
	// If the user has not configured a custom resolution, scale dynamically.
	// This avoids the physical limits of the PS2's CRT television.
	if (g_GraphicsCanvasData.width_modern == 512 && g_GraphicsCanvasData.height_modern == 288) {
		g_GraphicsCanvasData.width_modern = 1920;  // Force 1080p by default on PC
		g_GraphicsCanvasData.height_modern = 1080;
	}

	// 3. Replicate the basic structural fill the game expects to read in memory
	out_env[0] = 0x66; // Internal identifier code of the display buffer
	out_env[1] = (mode_flags == 0) ? 1 : 3;
	out_env[2] = ((unsigned long long)(mode_flags & 0xF) << 15) | ((unsigned long long)((width + 0x3F) >> 6) << 9);

	// 4. Simulate the 64-bit packing of the GS DISPLAY register
	// This prevents secondary game functions from trapping (crashing) when they read 0.
	unsigned long long calculated_display_reg = 0;
	int frame_calc = (width + 0x9FF) / (width == 0 ? 1 : width);

	calculated_display_reg = ((unsigned long long)(frame_calc - 1) << 23) |
		((unsigned long long)(height - 1) << 44) |
		((unsigned long long)(dy & 0xFFF) << 12) |
		(dx & 0xFFF);

	out_env[3] = calculated_display_reg;
	out_env[4] = 0; // Upper control register set to zero

	// Debug print in the PC terminal to verify that everything flows in real time
	printf("[Graphics] Native display environment configured: %dx%d | scaled on PC to: %dx%d (%s)\n",
		width, height, g_GraphicsCanvasData.width_modern, g_GraphicsCanvasData.height_modern,
		(g_GraphicsCanvasData.target_fps == 0.0f) ? "FPS unlocked" : "FPS limited");

	return calculated_display_reg;
}

int g_GraphicsCanvasActiveIndex = 0; // Maps DAT_0013ea1c

int Graphics_CloseCanvasTransaction(unsigned long slot_index) {
	// 1. Locate the virtual Scratchpad slot address from the index
	unsigned int scratchpad_addr = Graphics_GetScratchpadSlotAddress(slot_index);
	unsigned int* slot_ptr = (unsigned int*)(uintptr_t)scratchpad_addr;

	Sys_WaitGraphicsFrame();

	// 2. Validation: if the general graphics SIF system is not active
	if (g_GraphicsSifInitialized == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -1;
	}

	// 3. Validation: if the slot is invalid or already empty (puVar1[1] == 0)
	if (scratchpad_addr == 0 || slot_ptr == NULL || slot_ptr[1] == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -9;
	}

	// 4. Map the global parameters, imitating the original flow
	g_GraphicsCanvasFlags = slot_ptr[0];
	g_GraphicsCanvasParam3 = (int)((scratchpad_addr - 0x13ff00) / 0x10);

	// Simulate the call's context variables for structural consistency
	g_GraphicsTempSemaID = g_GraphicsSemaphoreID;
	g_GraphicsContextState = 4;

	// 5. The key step: mark the slot as FREE (puVar1[1] = 0)
	// On PC, modify the correct position in our virtual array directly
	int local_offset = scratchpad_addr - 0x13ff00;

	// Fixed: get the pointer by adding the byte offset to the base of the array
	unsigned int* local_slot = (unsigned int*)(g_Ps2ScratchpadMemory + local_offset);
	local_slot[1] = 0;

	// 6. Simulate SIF transaction 1 (environment close on the IOP)
	// Force a positive response from the simulated hardware
	int simulated_iop_status = 1; // 1 = success returned in DAT_2013f640

	Sys_ReleaseGraphicsSemaphore();

	if (simulated_iop_status == 0) {
		return -11; // Virtual communication error (-0xb)
	}

	// Wait for the semaphore and delete the temporary synchronization context
	sceWaitSema(g_RenderSemaphoreID_A);
	sceDeleteSema(g_GraphicsTempSemaID);

	return 0; // Clean success: slot released and ready on PC
}

int Graphics_DispatchCanvasTransaction(unsigned long slot_index, unsigned int param_2, long param_3) { // 1. Get the virtual Scratchpad address from the index
	unsigned int scratchpad_addr = Graphics_GetScratchpadSlotAddress(slot_index);
	unsigned int* slot_ptr = (unsigned int*)(uintptr_t)scratchpad_addr;

	Sys_WaitGraphicsFrame();

	// 2. Validate the subsystem initialization
	if (g_GraphicsSifInitialized == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -1; // Error: SIF not initialized
	}

	// 3. Validate the command slot
	if (scratchpad_addr == 0 || slot_ptr == NULL || slot_ptr[1] == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -9; // Error: invalid or inactive slot (0xfffffff7)
	}

	unsigned int uVar1 = slot_ptr[1];

	// 4. Map the global parameters, replicating the original arithmetic
	g_GraphicsCanvasFlags = slot_ptr[0];

	// Originally computed: (int)(puVar2 + -0x4ffc0) >> 4;
	g_GraphicsCanvasActiveIndex = (int)((scratchpad_addr - 0x13ff00) / 0x10);

	// Fixed: if param_3 is a pointer or address, apply the safe PC cast
	*(uintptr_t*)&g_GraphicsResourcePath = (uintptr_t)param_3;
	g_GraphicsCanvasParam3 = param_2;

	// 5. Original asynchronous handling (thread control flags on PS2)
	// On PC there is no need to mask transactions in g_GraphicsActiveTransactions
	// because modern execution is synchronous and deterministic at the software-thread level.
	if ((uVar1 & 0x8000) != 0) {
		// Passive simulation of the asynchronous section if the engine requires it in its flags
		sceWaitSema(g_RenderSemaphoreID_A);
		sceSignalSema(g_RenderSemaphoreID_A);
	}

	// 6. Skip the MIPS-specific hardware calls:
	// sys_kernel_flush_dcache_range(param_2, param_3);
	// sys_kernel_flush_dcache_range(0x13ea00, 0x20);

	// 7. Simulate SIF transaction 2 (draw/render)
	// Force an immediate success response from the virtual coprocessor
	int simulated_iop_status = 1; // 1 = success returned in DAT_2013f640

	Sys_ReleaseGraphicsSemaphore();

	if (simulated_iop_status == 0) {
		return -11; // Transaction failure (0xfffffff5)
	}

	// If the mode requires immediate explicit (synchronous) synchronization
	if ((uVar1 & 0x8000) == 0) {
		sceWaitSema(g_RenderSemaphoreID_A);
		sceDeleteSema(g_GraphicsTempSemaID); // Safe virtual cleanup
	}

	return 0; // Complete success: the frame was dispatched correctly
}

unsigned int Graphics_GetScratchpadSlotAddress(unsigned long slot_index) {
	int calculated_address = 0;

	// 1. Request exclusive access to render-control semaphore A
	Sys_InitRenderBuffers();
	sceWaitSema(g_RenderSemaphoreID_A);

	// 2. Bounds check: the Insomniac Scratchpad supports at most 32 slots (0x20)
	if (slot_index < 0x20) {
		// Replicate the original computation: index * 16 + Base_Scratchpad
		calculated_address = (int)slot_index * 0x10 + 0x13ff00;
		sceSignalSema(g_RenderSemaphoreID_A);
	}
	else {
		// Safe out-of-range index
		sceSignalSema(g_RenderSemaphoreID_A);
		calculated_address = 0;
	}

	return calculated_address;
}

int Graphics_SetupCanvasEnvironment(const char* resource_path, unsigned int flags, unsigned int param_3) {
	int result_code = 0;

	// 1. Synchronization and lazy initialization of the SIF layer
	Sys_WaitGraphicsFrame();
	if (g_GraphicsSifInitialized == 0) {
		Graphics_InitSifInterface();
	}

	// 2. Validate changes of the video mode
	if (Graphics_CheckVideoModeChange()) {
		Sys_ReleaseGraphicsSemaphore();
		return -0x10004; // Error: inconsistent video mode
	}

	// 3. Reserve a command slot in the virtual Scratchpad
	// Get the simulated PS2-compatible address (0x13ff00 + offset)
	unsigned int scratchpad_addr = Graphics_AllocateScratchpadSlot();
	int* slot_ptr = (int*)(uintptr_t)scratchpad_addr;

	if (scratchpad_addr == 0) {
		Sys_ReleaseGraphicsSemaphore();
		return -0x13; // Error: fast command queue full
	}

	// 4. Safe copy of the resource path (replaces Ghidra's original for loop)
	// Guarantees it does not exceed 1024 bytes and ends with a null character
	strncpy(g_GraphicsResourcePath, resource_path, 1023);
	g_GraphicsResourcePath[1023] = '\0';

	// 5. Translation of the original MIPS pointer arithmetic
	// On a real PS2 it computed the slot index from the distance to the RAM base
	// iVar6 = (int)(piVar4 + -0x4ffc0) >> 4;
	g_GraphicsScratchpadIndex = (int)((scratchpad_addr - 0x13ff00) / 0x10);

	// Store the metadata in the global control variables
	g_GraphicsCanvasFlags = flags & 0x6FFFFFFF;
	g_GraphicsCanvasParam3 = param_3;
	g_GraphicsScratchpadIndex = g_GraphicsScratchpadIndex;

	// 6. Simulate the SIF transaction on PC
	// Skip creating temporary semaphores and Sony RPC calls.
	// Force a successful behaviour, simulating that the IOP answered successfully.
	int simulated_stack_response = 0; // Simulate aiStack_120[0] returning 0 (success)

	Sys_ReleaseGraphicsSemaphore();

	// Replicate the logic of the original success block:
	// Request exclusive access to write to the Scratchpad slot
	sceWaitSema(g_RenderSemaphoreID_A);

	// Apply the bit mask requested by the game to the slot state
	// piVar4[1] = piVar4[1] | param_2;
	// *piVar4 = aiStack_120[0];
	if (slot_ptr != NULL) {
		// Since we operate on our mapped memory, modify the correct offsets
		// in our virtual array g_Ps2ScratchpadMemory
		int local_offset = scratchpad_addr - 0x13ff00;
		int* local_slot = (int*)&g_Ps2ScratchpadMemory[local_offset];

		local_slot[1] |= flags;
		local_slot[0] = simulated_stack_response;
	}

	sceSignalSema(g_RenderSemaphoreID_A);
	result_code = simulated_stack_response;

	return result_code;
}

// Reminder: g_Ps2ScratchpadMemory was already declared earlier in this file as:
// static unsigned char g_Ps2ScratchpadMemory[0x200]; 

unsigned int Graphics_AllocateScratchpadSlot(void) {
	// 1. Make sure of initialization and request exclusive access to the render semaphore
	Sys_InitRenderBuffers();
	sceWaitSema(g_RenderSemaphoreID_A);

	// Map the original physical ranges to local offsets of our PC array
	// Base original PS2: 0x13ff00 -> Offset PC: 0
	// Original PS2 limit: 0x1400ff -> PC offset: 0x1FF
	int local_offset = 0;

	while (true) {
		// Read the state of the current slot at the corresponding offset
		// Originally it read offset +4 of the control pointer (iVar1 = DAT_0013ff04 in the first iteration)
		unsigned int slot_status = *(unsigned int*)&g_Ps2ScratchpadMemory[local_offset + 4];

		if (slot_status == 0) {
			// Free slot found: write the busy control flag
			*(unsigned int*)&g_Ps2ScratchpadMemory[local_offset + 4] = 0x10000000;

			// Release the semaphore and return the simulated PS2-compatible address
			sceSignalSema(g_RenderSemaphoreID_A);
			return 0x13ff00 + local_offset;
		}

		// Stop condition if the next 16-byte increment exceeds the buffer limit
		if (0x1400ff < (0x13ff00 + local_offset + 0x10)) {
			break;
		}

		// Advance to the next slot (16 bytes ahead)
		local_offset += 0x10;
	}

	// Leaving the loop means the fast command buffer is full
	sceSignalSema(g_RenderSemaphoreID_A);
	return 0;
}

bool Graphics_CheckVideoModeChange(void) {
	// On PC, instead of calling memcmp for just 4 bytes,
	// compare the integer values directly.
	// This gives exactly the same logical result, natively and very fast.

	if (g_VideoMode_Current != g_VideoMode_Target) {
		if (g_VideoMode_Current != g_VideoMode_Fallback) {
			if (g_VideoMode_Target != g_VideoMode_Fallback) {
				return true; // The three buffers differ, the video mode changed
			}
		}
	}

	return false; // The graphics environment is stable
}

// Temporary pointer structures of the engine
void* g_GfxBufferPtrA = NULL; // Maps DAT_0013e9c0
void* g_GfxBufferPtrB = NULL; // Maps DAT_0013e9c4

// Simulation of the PS2's physical memory block 0x13ff00
//static unsigned char g_Ps2ScratchpadMemory[0x200] = { 0 };

int Graphics_InitSifInterface(void) {
	// 1. Safely skip the PS2 RPC/SIF hardware initializations
	// sys_sif_rpc_init_client();
	// kernel_system_sync_guard();
	// sys_sif_register_callback(...);

	// 2. Simulation of the session-open loop
	// On PC, forcing g_SifSessionReady = 1 avoids freezing execution
	while (true) {
		int session_status = 0; // Simulate success of sys_sif_rpc_open_transaction_session
		if (session_status < 0) {
			return -1;
		}
		if (g_SifSessionReady != 0) break;
	}

	// 3. Double-buffer initialization and initial synchronization
	Sys_InitRenderBuffers();

	// The engine blocks temporarily on the first render semaphore
	sceWaitSema(g_RenderSemaphoreID_A);

	// 4. Clear the graphics command block (originally between 0x13ff00 and DAT_00140100)
	// The game steps 16 bytes (0x10) at a time and zeroes offset +4
	for (int offset = 0; offset < 0x200; offset += 0x10) {
		*(unsigned int*)&g_Ps2ScratchpadMemory[offset + 4] = 0;
	}

	// Unlock the semaphore to continue the flow
	sceSignalSema(g_RenderSemaphoreID_A);

	// 5. Configure the command exchange buffers
	g_GfxBufferPtrA = &g_GraphicsIopCommandBuffers;
	g_GfxBufferPtrB = NULL; // Maps DAT_0013fac0 safely on PC

	// 6. Configure the final success flags
	// On PC the successful mode is forced; the video format flag is set to 1 (valid/active)
	g_GraphicsSifInitialized = 1;
	g_GraphicsVideoFormat = 1; // 1 = the game assumes a valid/active video format

	return 0; // Clean success
}

/**
 * @brief Initializes the virtual SIF/RPC channel for graphics on PC.
 * @return 0 on success, or a negative error code.
 */
int Graphics_InitSifInterface(void);

void Graphics_SifCallback_Dispatch(void* param_1, unsigned int* param_2) {
	(void)param_1; // Avoids an unused-parameter warning

	if (param_2 != NULL) {
		// In the original decompilation: param_2[0] is the address of the function to invoke
		// and param_2[1] is the argument injected into that function.
		typedef void (*GraphicsSubRoutine)(unsigned int);
		GraphicsSubRoutine funcion_a_ejecutar = (GraphicsSubRoutine)((uintptr_t)param_2[0]);

		if (funcion_a_ejecutar != NULL) {
			funcion_a_ejecutar(param_2[1]);
		}
	}

	// The SYNC(0) and EI() instructions are skipped on PC because they are specific
	// to the MIPS CPU execution pipeline and interrupt control.
}

// Initialize with default values (e.g. 1080p at 60 FPS by default)
GraphicsCanvas g_GraphicsCanvasData = {
	.width_native = 512,
	.height_native = 288,
	.width_modern = 1920,
	.height_modern = 1080,
	.target_fps = 60.0f
};

GraphicsCanvas* Graphics_GetCanvasData(void) {
	return &g_GraphicsCanvasData;
}

void Graphics_SetCustomResolution(int width, int height, float fps) {
	g_GraphicsCanvasData.width_modern = width;
	g_GraphicsCanvasData.height_modern = height;
	g_GraphicsCanvasData.target_fps = fps;
}

// Instances of the index and the simulated global buffers (adjust the sizes if Ghidra reveals more)
int g_GraphicsTransactionIndex = 0;
//unsigned char g_GraphicsIopCommandBuffers[0x440 * 4] = { 0 };
int g_GraphicsActiveTransactions[32] = { 0 };

void Graphics_ProcessIopTransaction(void* packet_ptr) {
	if (packet_ptr == NULL) return;

	// Structure access to the received packet pointer
	unsigned int* rpc_packet = (unsigned int*)packet_ptr;

	g_GraphicsTransactionIndex = 0;
	if (g_GraphicsVideoFormat != 0) {
		// On the original PS2 it read offset 0xC of the SIF packet
		g_GraphicsTransactionIndex = (int)rpc_packet[3];
	}

	// Compute the pointer to the corresponding IOP command block
	// Note: on PC the PS2 virtual memory mask '| 0x20000000' is removed
	int* cmd_buffer = (int*)(&g_GraphicsIopCommandBuffers[g_GraphicsTransactionIndex * 0x440]);

	int cmd_id = cmd_buffer[0];
	int cmd_type = cmd_buffer[1];

	// Initial copy if the ID is valid
	if (cmd_id > -1) {
		void* dest = (void*)(uintptr_t)cmd_buffer[2];
		void* src = (void*)&cmd_buffer[4];
		size_t size = (size_t)cmd_buffer[3];
		memcpy(dest, src, size);
	}

	// Engine command type processor
	switch (cmd_type) {
	case 2: {
		int len_a = cmd_buffer[5];
		int len_b = cmd_buffer[6];

		if (len_a > 0) {
			char* dest_a = (char*)(uintptr_t)cmd_buffer[7];
			char* src_a = (char*)&cmd_buffer[9];
			for (int i = 0; i < len_a; i++) dest_a[i] = src_a[i];
		}
		if (len_b > 0) {
			char* dest_b = (char*)(uintptr_t)cmd_buffer[8];
			char* src_b = (char*)&cmd_buffer[0x19];
			for (int i = 0; i < len_b; i++) dest_b[i] = src_b[i];
		}
		break;
	}

	case 0xB:
	case 0xC: {
		// Clean simplification of the aligned 64-byte copy (8 quadwords)
		void* dest_bulk = (void*)(uintptr_t)cmd_buffer[5];
		void* src_bulk = (void*)&cmd_buffer[6];
		memcpy(dest_bulk, src_bulk, 64);
		break;
	}

	case 0x17:
	case 0x19:
	case 0x1A: {
		size_t size_cap = (size_t)cmd_buffer[6];
		if (size_cap > 0x400) size_cap = 0x400; // Original safety limit

		void* dest_cap = (void*)(uintptr_t)cmd_buffer[5];
		void* src_cap = (void*)&cmd_buffer[7];
		memcpy(dest_cap, src_cap, size_cap);
		break;
	}
	}

	// Exit logic: control and release of the engine threads
	if (cmd_id < 0) {
		// Clear the active transaction table
		if (g_GraphicsActiveTransactions[0] == -cmd_id) {
			g_GraphicsActiveTransactions[0] = -1;
		}
		else {
			for (int i = 1; i < 32; i++) {
				if (g_GraphicsActiveTransactions[i] == -cmd_id) {
					g_GraphicsActiveTransactions[i] = -1;
					break;
				}
			}
		}
	}
	else {
		// Signal active: safely wakes the game loop on PC
		iSignalSema(g_GraphicsSemaphoreID);
	}
}