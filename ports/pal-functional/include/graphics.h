#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

// Structure that mirrors the PS2 memory block, with extra fields for PC
typedef struct {
	// --- Original PS2 fields (native game values) ---
	int width_native;       // Original resolution (e.g. 512)
	int height_native;      // Original resolution (e.g. 288 or 512)

	// --- Extended PC fields (user-modifiable) ---
	int width_modern;       // Modified resolution (e.g. 1920 or 3840)
	int height_modern;      // Modified resolution (e.g. 1080 or 2160)
	float target_fps;       // Frame rate (e.g. 60.0, 144.0, 0 for unlimited)
} GraphicsCanvas;

// ... Keep the previous declarations ...

/**
 * @brief Finds and reserves a free command slot in the virtual Scratchpad memory.
 * @return Assigned virtual address (simulated offset), or 0 if the buffer is full.
 */
unsigned int Graphics_AllocateScratchpadSlot(void);

// Global declaration so other subsystems can read it
extern GraphicsCanvas g_GraphicsCanvasData;

/**
 * @brief Returns the pointer to the configurable graphics data context.
 */
GraphicsCanvas* Graphics_GetCanvasData(void);

/**
 * @brief Configures the game's custom PC resolution.
 */
void Graphics_SetCustomResolution(int width, int height, float fps);

/**
 * @brief Processes the data packets returned by the video subsystem.
 * @param packet_ptr Pointer to the data packet of the SIF transaction.
 */
void Graphics_ProcessIopTransaction(void* packet_ptr);

/**
 * @brief Second SIF callback, dispatching asynchronous graphics subroutines.
 */
void Graphics_SifCallback_Dispatch(void* param_1, unsigned int* param_2);

// ... Keep the previous declarations ...

extern unsigned int g_VideoMode_Current;
extern unsigned int g_VideoMode_Target;
extern unsigned int g_VideoMode_Fallback;

/**
 * @brief Compares the video mode registers to detect inconsistencies or screen changes.
 * @return true if the three modes all differ (reconfiguration required), false otherwise.
 */
bool Graphics_CheckVideoModeChange(void);

// ... Keep the previous declarations ...

/**
 * @brief Configures the modern graphics environment and loads the resource metadata.
 * @param resource_path Path of the resource/map to load.
 * @param flags Graphics configuration masks of the engine.
 * @param param_3 Additional context parameters.
 * @return 0 on success, or a negative error code.
 */
int Graphics_SetupCanvasEnvironment(const char* resource_path, unsigned int flags, unsigned int param_3);

// ... Keep the previous declarations ...

/**
 * @brief Returns the simulated virtual address of a Scratchpad slot from its index.
 * @param slot_index Index of the requested slot (0-31).
 * @return Computed virtual address (0x13ff00 + offset), or 0 if the index is invalid.
 */
unsigned int Graphics_GetScratchpadSlotAddress(unsigned long slot_index);

// ... Keep the previous declarations ...

/**
 * @brief Dispatches the graphics transaction accumulated in the Scratchpad to the modern environment.
 * @param slot_index Index of the slot to process.
 * @param param_2 Data control/address parameter.
 * @param param_3 Size or pointer of the resource metadata.
 * @return 0 on success, or a negative error code on failure.
 */
int Graphics_DispatchCanvasTransaction(unsigned long slot_index, unsigned int param_2, long param_3);

// ... Keep the previous declarations ...

/**
 * @brief Closes an active graphics transaction context and frees its Scratchpad slot.
 * @param slot_index Index of the slot to free (0-31).
 * @return 0 on success, or a negative error code on failure.
 */
int Graphics_CloseCanvasTransaction(unsigned long slot_index);

// ... Keep the previous declarations (GraphicsCanvas, etc.) ...

/**
 * @brief Configures the PC display environment by intercepting the PS2 registers.
 * @param out_env Pointer to the buffer where the game stores the display environment structure.
 * @param mode_flags Initialization flags.
 * @param width Requested native width (e.g. 512).
 * @param height Requested native height (e.g. 288 or 512).
 * @param dx Horizontal offset.
 * @param dy Vertical offset.
 * @return Packed binary control structure, compatible with the original.
 */
unsigned long long sceGsDefDispEnv(unsigned long long* out_env, short mode_flags, short width, short height, short dx, short dy);


#endif // GRAPHICS_H
