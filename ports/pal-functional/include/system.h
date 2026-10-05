#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h> // Changed <SDL2/SDL.h> to <SDL.h>
#include <stdio.h>

// Debug macros with colours and icons to identify problems quickly
#define LOG_INFO(module, text, ...)  printf("ℹ️  [" module "] " text "\n", ##__VA_ARGS__)
#define LOG_SUCCESS(module, text, ...) printf("✅ [" module "] " text "\n", ##__VA_ARGS__)
#define LOG_WARN(module, text, ...)    fprintf(stderr, "⚠️  [" module "] " text "\n", ##__VA_ARGS__)
#define LOG_ERROR(module, text, ...)   fprintf(stderr, "❌ [" module "] CRITICAL ERROR: " text "\n", ##__VA_ARGS__)

/**
 * @brief Sends a release signal to the main graphics semaphore from the subsystem.
 */
void Sys_ReleaseGraphicsSemaphore(void);

// ... Keep the previous declarations (g_GraphicsSemaphore, etc.) ...

// Global declarations for the render/double-buffer semaphore pair
extern SDL_sem* g_RenderSemaphore_A;
extern int g_RenderSemaphoreID_A;

extern SDL_sem* g_RenderSemaphore_B;
extern int g_RenderSemaphoreID_B;

/**
 * @brief Lazily initializes the pair of render-control semaphores.
 */
void Sys_InitRenderBuffers(void);

// -1 means not initialized, as on the PS2
#define SYS_SEMAPHORE_INVALID -1

// Global declaration of the PC semaphore using the native SDL type
extern SDL_sem* g_GraphicsSemaphore;
extern int g_GraphicsSemaphoreID; // Keep the integer ID for compatibility in case the game reads it

/**
 * @brief Initializes the system semaphore if it has not been created.
 */
void Sys_InitGraphicsSemaphore(void);

#endif // SYSTEM_H

// ... Keep the previous declarations ...

/**
 * @brief Waits for the graphics semaphore to be released before advancing to the next frame.
 * @return Always returns 0 to keep compatibility with the original return type.
 */
long long Sys_WaitGraphicsFrame(void);

// ... Keep the previous declarations ...

/**
 * @brief Validates the compatibility of the system firmware.
 * @return 1 on success (valid console/environment), 0 on incompatibility.
 */
unsigned int Sys_CheckConsoleVersion(void);
