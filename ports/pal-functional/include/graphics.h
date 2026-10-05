#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

// Estructura que simula el bloque de memoria de PS2, pero añade campos para PC
typedef struct {
	// --- Campos Originales PS2 (Valores nativos del juego) ---
	int width_native;       // Resolución original (ej. 512)
	int height_native;      // Resolución original (ej. 288 o 512)

	// --- Campos Extendidos para PC (Modificables por el usuario) ---
	int width_modern;       // Resolución modificada (ej. 1920 o 3840)
	int height_modern;      // Resolución modificada (ej. 1080 o 2160)
	float target_fps;       // Tasa de cuadros (ej. 60.0, 144.0, 0 para ilimitado)
} GraphicsCanvas;

// ... Mantener lo anterior ...

/**
 * @brief Busca y reserva una ranura de comando libre en la memoria del Scratchpad virtual.
 * @return Dirección virtual asignada (offset simulado), o 0 si el búfer está lleno.
 */
unsigned int Graphics_AllocateScratchpadSlot(void);

// Declaración global para que otros subsistemas la lean
extern GraphicsCanvas g_GraphicsCanvasData;

/**
 * @brief Obtiene el puntero al contexto de datos gráficos configurables.
 */
GraphicsCanvas* Graphics_GetCanvasData(void);

/**
 * @brief Configura la resolución personalizada del juego para PC.
 */
void Graphics_SetCustomResolution(int width, int height, float fps);

/**
 * @brief Ejecuta el procesamiento de paquetes de datos devueltos por el subsistema de video.
 * @param packet_ptr Puntero al paquete de datos de la transacción SIF.
 */
void Graphics_ProcessIopTransaction(void* packet_ptr);

/**
 * @brief Segundo callback del SIF encargado de despachar sub-rutinas gráficas asíncronas.
 */
void Graphics_SifCallback_Dispatch(void* param_1, unsigned int* param_2);

// ... Mantener lo anterior ...

extern unsigned int g_VideoMode_Current;
extern unsigned int g_VideoMode_Target;
extern unsigned int g_VideoMode_Fallback;

/**
 * @brief Compara los registros de modo de video para detectar inconsistencias o cambios de pantalla.
 * @return true si los tres modos difieren entre sí (requiere reconfiguración), false de lo contrario.
 */
bool Graphics_CheckVideoModeChange(void);

// ... Mantener lo anterior ...

/**
 * @brief Configura el entorno gráfico moderno y procesa la carga de metadatos de recursos.
 * @param resource_path Ruta del recurso/mapa a cargar.
 * @param flags Máscaras de configuración gráfica del motor.
 * @param param_3 Parámetros adicionales de contexto.
 * @return 0 para éxito, o código de error negativo.
 */
int Graphics_SetupCanvasEnvironment(const char* resource_path, unsigned int flags, unsigned int param_3);

// ... Mantener lo anterior ...

/**
 * @brief Obtiene la dirección virtual simulada de una ranura del Scratchpad basándose en su índice.
 * @param slot_index Índice de la ranura solicitada (0-31).
 * @return Dirección virtual calculada (0x13ff00 + offset), o 0 si el índice es inválido.
 */
unsigned int Graphics_GetScratchpadSlotAddress(unsigned long slot_index);

// ... Mantener lo anterior ...

/**
 * @brief Despacha la transacción gráfica acumulada en el Scratchpad hacia el entorno moderno.
 * @param slot_index Índice de la ranura a procesar.
 * @param param_2 Parámetro de control/dirección de datos.
 * @param param_3 Tamaño o puntero de metadatos de recursos.
 * @return 0 para éxito, o código de error negativo en caso de fallo.
 */
int Graphics_DispatchCanvasTransaction(unsigned long slot_index, unsigned int param_2, long param_3);

// ... Mantener lo anterior ...

/**
 * @brief Cierra un contexto de transacción gráfica activo y libera su ranura en el Scratchpad.
 * @param slot_index Índice de la ranura a liberar (0-31).
 * @return 0 para éxito, o código de error negativo en caso de fallo.
 */
int Graphics_CloseCanvasTransaction(unsigned long slot_index);

// ... Mantener lo anterior (GraphicsCanvas, etc.) ...

/**
 * @brief Configura el entorno de despliegue en PC interceptando los registros de la PS2.
 * @param out_env Puntero al buffer donde el juego guarda la estructura del entorno de pantalla.
 * @param mode_flags Flags de inicialización.
 * @param width Ancho nativo solicitado (ej. 512).
 * @param height Alto nativo solicitado (ej. 288 o 512).
 * @param dx Desplazamiento horizontal.
 * @param dy Desplazamiento vertical.
 * @return Estructura binaria de control compatible empaquetada.
 */
unsigned long long sceGsDefDispEnv(unsigned long long* out_env, short mode_flags, short width, short height, short dx, short dy);


#endif // GRAPHICS_H
