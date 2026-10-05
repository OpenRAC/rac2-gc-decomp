#include "core/gpu.h"
#include <SDL.h>
#include <SDL_opengl.h>   /* si usas GL clásico; si usas GLES, <GLES3/gl3.h> */

/*
 * ------------------------------------------------------------------
 *  PORT de FUN_00124418 (0x00124418)
 * ------------------------------------------------------------------
 *
 *  ASM original (resumen):
 *    *(0x10003C10) = 1;            // VIF1_FBRST
 *    *(0x10003C20) = 2;            // VIF1_ERR clear
 *    SYNC(0);
 *    vu1_r12 |= 0x200;            // enable flag
 *    SYNC(0x10);
 *    while (vu1_r13 & 0x100);     // poll VU1 ready
 *    vu1_r12 = 0x404;             // config mode
 *    gif_dma_write(0x1363B0);     // context packet
 *    gif_dma_write(0x1363C0);     // data packet
 *    *(0x10003000) = 1;           // GIF_CTRL enable
 *
 *  PC:
 *    Los registros VIF/VU/GIF no existen.
 *    El equivalente funcional es:
 *      - limpiar estado GL
 *      - configurar el pipeline mínimo
 *      - subir los datos del contexto
 *      - el "GIF_CTRL=1" se vuelve el primer glDrawArrays/Elements
 * ------------------------------------------------------------------
 */
void gpu_init_context(void)
{
	/*
	 * --- VIF1 reset + error clear ---
	 * En GL: no hay FIFO de DMA explícito.
	 * Un glClear garantiza que el color buffer está limpio
	 * antes de dibujar el primer frame.
	 */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * --- VU1.R12 |= 0x200 + poll VU1.R13 ---
	 * VU1 hacía vertex transforms. En GL eso es el vertex shader.
	 * "Poll until ready" → glFinish() (o un fence si quieres async).
	 * Para la presentación (quad full-screen con textura) no hay
	 * vertex transform complejo, pero el flush es fiel al original.
	 */
	glFinish();

	/*
	 * --- VU1.R12 = 0x404 (config mode) ---
	 * En el original: configura el modo de transformación de VU1
	 * (qué matrices aplica, qué pipeline de interpolación).
	 * En PC: configurar el vertex shader y los uniforms.
	 *
	 * TODO: cuando tengas el shader de la presentación, aquí va:
	 *   glUseProgram(presentation_shader);
	 *   glUniform2f(loc_resolution, (float)w, (float)h);
	 *   // ... otros uniforms
	 */

	 /*
	  * --- 2x GIF DMA packets (0x1363B0, 0x1363C0) ---
	  * En PS2: suben el contexto de display (palette, clip planes,
	  * viewport) y el primer chunk de geometría/textura al GS.
	  *
	  * En PC: esto se vuelve:
	  *   - glTexImage2D para el logo
	  *   - glBufferData para el VBO del quad
	  *   - glUniform* para resolución, color de fondo
	  *
	  * TODO: implementar cuando extraigas los assets con iso2assets.
	  * Por ahora el "contexto" es: viewport + clear color (ya hecho).
	  */
	int win_w, win_h;
	SDL_GetWindowSize(NULL, &win_w, &win_h);
	glViewport(0, 0, win_w, win_h);

	/*
	 * --- GIF_CTRL = 1 ---
	 * En PS2: habilita el motor de DMA. A partir de aquí el GS
	 * empieza a procesar los packets que entraron al FIFO.
	 *
	 * En GL: no hay equivalente explícito. El primer draw call
	 * (glDrawArrays / glDrawElements) dispara el pipeline.
	 * Para la presentación será algo como:
	 *   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	 *
	 * TODO: cuando tengas el mesh del logo.
	 */
}
