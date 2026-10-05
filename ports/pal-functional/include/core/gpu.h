#ifndef CORE_GPU_H
#define CORE_GPU_H

#include <stdint.h>

/**
 * Inicializa el contexto de render (reset + subida de estado inicial).
 *
 * Port de FUN_00124418 (0x00124418, PS2 ELF).
 *
 * En PS2:
 *   - Reset VIF1 (front buffer + error)
 *   - Config VU1 (R12 = 0x404, poll R13 hasta ready)
 *   - KICK 2x GIF DMA packets (contexto + data)
 *   - GIF_CTRL = 1
 *
 * En PC (SDL2 + OpenGL):
 *   - glViewport + glClear
 *   - Setup de pipeline (shader, VAO, uniforms)
 *   - Upload de contexto (texturas, buffers)
 *
 * Llamar UNA VEZ al arranque, antes del primer frame.
 * No es thread-safe (no llamar desde audio thread).
 */
void gpu_init_context(void);

#endif /* CORE_GPU_H */
