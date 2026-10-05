#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "types.h"

// --- 1. ADAPTED CONTROL SUBSYSTEM (Universal Pad HAL) ---
// Maps the physical structure that the PS2 game expects to receive in RAM
typedef struct {
    u8 status;       // 0 = connected
    u8 len;          // Size of the report data (usually 4 or 6 bytes)
    u16 buttons;     // Bit mask of the D-pad and main buttons (X, O, Triangle, Square)
    u8 right_joy_x;  // Joystick derecho X (0-255)
    u8 right_joy_y;  // Right joystick Y (0-255)
    u8 left_joy_x;   // Left joystick X (0-255)
    u8 left_joy_y;   // Left joystick Y (0-255)
} PS2_PadData;

static PS2_PadData g_virtual_pad = { 0, 6, 0xFFFF, 127, 127, 127, 127 };

/**
 * @brief Intercepts the PS2 scePadRead API and writes the buttons pressed on the PC using SDL2.
 * Replaces the reverse engineering of any controller subroutine in Ghidra.
 */
s32 scePadRead(s32 port, s32 slot, PS2_PadData* p_data_out) {
    if (p_data_out == NULL) return 0;

    // Reads the state of the PC keyboard/physical controller through SDL2 immediately
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    u16 btn_mask = 0xFFFF; // On the PS2 active buttons are LOW (0), inactive ones (1)

    // Universal game-control mapping for PC (keyboard to PS2 pad)
    if (keyboard_state[SDL_SCANCODE_RETURN]) btn_mask &= ~(1 << 3);  // Start (Bit 3)
    if (keyboard_state[SDL_SCANCODE_SPACE])  btn_mask &= ~(1 << 14); // X button / jump (bit 14)
    if (keyboard_state[SDL_SCANCODE_UP])     btn_mask &= ~(1 << 4);  // D-pad up (bit 4)
    if (keyboard_state[SDL_SCANCODE_DOWN])   btn_mask &= ~(1 << 6);  // D-pad down (bit 6)
    if (keyboard_state[SDL_SCANCODE_LEFT])   btn_mask &= ~(1 << 7);  // D-pad left (bit 7)
    if (keyboard_state[SDL_SCANCODE_RIGHT])  btn_mask &= ~(1 << 5);  // D-pad right (bit 5)

    g_virtual_pad.buttons = btn_mask;

    // Writes the report into the game's simulated RAM in one go
    memcpy(p_data_out, &g_virtual_pad, sizeof(PS2_PadData));
    return 1; // Returns success to the game loop
}

// --- 2. ADAPTED DISC SUBSYSTEM (Universal CDVD/VFS HAL) ---
/**
 * @brief Opens a file of the ISO or a local folder, emulating Sony's file system.
 * Avoids decompiling the complex logical-sector DVD read routines.
 */
FILE* sceCdoOpen(const char* p_filename, s32 mode) {
    char clean_path[256];

    // If the game requests "cdrom0:\\TEXT\\SPANISH.DAT;1", strip the PS2 characters
    // and translate it to a native path readable by Windows/Linux ("assets/TEXT/SPANISH.DAT")
    const char* p_cursor = strchr(p_filename, ':');
    if (p_cursor != NULL) {
        p_cursor += 2; // Skips the ':' and the slashes
    }
    else {
        p_cursor = p_filename;
    }

    snprintf(clean_path, sizeof(clean_path), "assets/%s", p_cursor);

    // Removes PS2 file version suffixes (;1) if present
    char* p_version_mark = strchr(clean_path, ';');
    if (p_version_mark != NULL) *p_version_mark = '\0';

    return fopen(clean_path, "rb"); // Opens the native file on the PC
}
