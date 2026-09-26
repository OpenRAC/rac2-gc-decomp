#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "types.h"

// --- 1. SUBSISTEMA DE CONTROL ADAPTADO (Universal Pad HAL) ---
// Mapea la estructura física que el juego de PS2 espera recibir en la RAM
typedef struct {
    u8 status;       // 0 = Conectado
    u8 len;          // Tamaño de los datos del reporte (usualmente 4 o 6 bytes)
    u16 buttons;     // Máscara de bits de la cruceta y botones principales (X, O, Triángulo, Cuadrado)
    u8 right_joy_x;  // Joystick derecho X (0-255)
    u8 right_joy_y;  // Joystick derecho Y (0-255)
    u8 left_joy_x;   // Joystick izquierdo X (0-255)
    u8 left_joy_y;   // Joystick izquierdo Y (0-255)
} PS2_PadData;

static PS2_PadData g_virtual_pad = { 0, 6, 0xFFFF, 127, 127, 127, 127 };

/**
 * @brief Intercepta la API scePadRead de la PS2 y vuelca los botones presionados en la PC usando SDL2.
 * Reemplaza la ingeniería inversa de cualquier subrutina de control en Ghidra.
 */
s32 scePadRead(s32 port, s32 slot, PS2_PadData* p_data_out) {
    if (p_data_out == NULL) return 0;

    // Lee el estado del teclado/control físico de la PC mediante SDL2 de forma instantánea
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    u16 btn_mask = 0xFFFF; // En la PS2 los botones activos están en BAJO (0), inactivos en (1)

    // Mapeo universal de controles lúdicos para PC (Teclado a Mando de PS2)
    if (keyboard_state[SDL_SCANCODE_RETURN]) btn_mask &= ~(1 << 3);  // Start (Bit 3)
    if (keyboard_state[SDL_SCANCODE_SPACE])  btn_mask &= ~(1 << 14); // Botón X / Saltar (Bit 14)
    if (keyboard_state[SDL_SCANCODE_UP])     btn_mask &= ~(1 << 4);  // Cruceta Arriba (Bit 4)
    if (keyboard_state[SDL_SCANCODE_DOWN])   btn_mask &= ~(1 << 6);  // Cruceta Abajo (Bit 6)
    if (keyboard_state[SDL_SCANCODE_LEFT])   btn_mask &= ~(1 << 7);  // Cruceta Izquierda (Bit 7)
    if (keyboard_state[SDL_SCANCODE_RIGHT])  btn_mask &= ~(1 << 5);  // Cruceta Derecha (Bit 5)

    g_virtual_pad.buttons = btn_mask;

    // Vuelca de golpe el reporte en la RAM simulada del juego
    memcpy(p_data_out, &g_virtual_pad, sizeof(PS2_PadData));
    return 1; // Retorna éxito al bucle del juego
}

// --- 2. SUBSISTEMA DE DISCO ADAPTADO (Universal CDVD/VFS HAL) ---
/**
 * @brief Abre un archivo de la ISO o carpeta local emulando el sistema de archivos de Sony.
 * Evita descompilar las complejas rutinas de lectura por sectores lógicos del DVD.
 */
FILE* sceCdoOpen(const char* p_filename, s32 mode) {
    char clean_path[256];

    // Si el juego pide "cdrom0:\\TEXT\\SPANISH.DAT;1", limpia los caracteres de PS2
    // y lo traduce a una ruta nativa legible por Windows/Linux ("assets/TEXT/SPANISH.DAT")
    const char* p_cursor = strchr(p_filename, ':');
    if (p_cursor != NULL) {
        p_cursor += 2; // Salta el ':' y las barras diagonales
    }
    else {
        p_cursor = p_filename;
    }

    snprintf(clean_path, sizeof(clean_path), "assets/%s", p_cursor);

    // Remueve marcas de versión de archivos de la PS2 (;1) si existen
    char* p_version_mark = strchr(clean_path, ';');
    if (p_version_mark != NULL) *p_version_mark = '\0';

    return fopen(clean_path, "rb"); // Abre el archivo nativo en la PC
}
