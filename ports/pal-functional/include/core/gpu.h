#ifndef CORE_GPU_H
#define CORE_GPU_H

#include <stdint.h>

/**
 * Initializes the render context (reset + upload of the initial state).
 *
 * Port of FUN_00124418 (0x00124418, PS2 ELF).
 *
 * On PS2:
 *   - Reset VIF1 (front buffer + error)
 *   - VU1 config (R12 = 0x404, poll R13 until ready)
 *   - KICK 2x GIF DMA packets (context + data)
 *   - GIF_CTRL = 1
 *
 * On PC (SDL2 + OpenGL):
 *   - glViewport + glClear
 *   - Pipeline setup (shader, VAO, uniforms)
 *   - Context upload (textures, buffers)
 *
 * Call ONCE at startup, before the first frame.
 * Not thread-safe (do not call from the audio thread).
 */
void gpu_init_context(void);

#endif /* CORE_GPU_H */
