#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h> // ¡Cambiamos <SDL2/SDL.h> por <SDL.h>!
#include <stdio.h>

// Macros de depuración con colores e íconos para identificar problemas rápido
#define LOG_INFO(modulo, texto, ...)  printf("ℹ️  [" modulo "] " texto "\n", ##__VA_ARGS__)
#define LOG_SUCCESS(modulo, texto, ...) printf("✅ [" modulo "] " texto "\n", ##__VA_ARGS__)
#define LOG_WARN(modulo, texto, ...)    fprintf(stderr, "⚠️  [" modulo "] " texto "\n", ##__VA_ARGS__)
#define LOG_ERROR(modulo, texto, ...)   fprintf(stderr, "❌ [" modulo "] ERRROR CRÍTICO: " texto "\n", ##__VA_ARGS__)

/**
 * @brief Envía una señal de liberación al semáforo gráfico principal desde el subsistema.
 */
void Sys_ReleaseGraphicsSemaphore(void);

// ... Mantener lo anterior (g_GraphicsSemaphore, etc.) ...

// Declaraciones globales para el par de semáforos de renderizado/doble buffer
extern SDL_sem* g_RenderSemaphore_A;
extern int g_RenderSemaphoreID_A;

extern SDL_sem* g_RenderSemaphore_B;
extern int g_RenderSemaphoreID_B;

/**
 * @brief Inicializa el par de semáforos de control de renderizado de manera perezosa.
 */
void Sys_InitRenderBuffers(void);

// Definimos -1 como el estado no inicializado, tal como la PS2
#define SYS_SEMAPHORE_INVALID -1

// Declaración global del semáforo para PC usando el tipo nativo de SDL
extern SDL_sem* g_GraphicsSemaphore;
extern int g_GraphicsSemaphoreID; // Mantenemos el ID entero para compatibilidad si el juego lo lee

/**
 * @brief Inicializa el semáforo del sistema si no ha sido creado.
 */
void Sys_InitGraphicsSemaphore(void);

#endif // SYSTEM_H

// ... Mantener lo anterior ...

/**
 * @brief Espera a que el semáforo de gráficos sea liberado para avanzar al siguiente cuadro.
 * @return Siempre retorna 0 para mantener compatibilidad con el tipo de retorno original.
 */
long long Sys_WaitGraphicsFrame(void);

// ... Mantener lo anterior ...

/**
 * @brief Valida la compatibilidad del firmware del sistema.
 * @return 1 para éxito (consola/entorno válido), u0 en caso de incompatibilidad.
 */
unsigned int Sys_CheckConsoleVersion(void);
