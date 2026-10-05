#include "core/gpu.h"
#include <SDL.h>
#include "core/render/gl/gl_loader.h"

/*
 * ------------------------------------------------------------------
 *  PORT of FUN_00124418 (0x00124418)
 * ------------------------------------------------------------------
 *
 *  Original ASM (summary):
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
 *    The VIF/VU/GIF registers do not exist.
 *    The functional equivalent is:
 *      - clear the GL state
 *      - configure the minimal pipeline
 *      - upload the context data
 *      - "GIF_CTRL=1" becomes the first glDrawArrays/Elements
 * ------------------------------------------------------------------
 */
void gpu_init_context(void)
{
	/*
	 * --- VIF1 reset + error clear ---
	 * In GL there is no explicit DMA FIFO.
	 * A glClear guarantees that the colour buffer is clean
	 * before drawing the first frame.
	 */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	 * --- VU1.R12 |= 0x200 + poll VU1.R13 ---
	 * VU1 performed vertex transforms. In GL that is the vertex shader.
	 * "Poll until ready" → glFinish() (or a fence for async).
	 * The intro (a full-screen textured quad) has no complex
	 * vertex transform, but the flush is faithful to the original.
	 */
	glFinish();

	/*
	 * --- VU1.R12 = 0x404 (config mode) ---
	 * In the original: configures the VU1 transform mode
	 * (which matrices it applies, which interpolation pipeline).
	 * On PC: configure the vertex shader and the uniforms.
	 *
	 * TODO: once the intro shader exists, this becomes:
	 *   glUseProgram(presentation_shader);
	 *   glUniform2f(loc_resolution, (float)w, (float)h);
	 *   // ... other uniforms
	 */

	 /*
	  * --- 2x GIF DMA packets (0x1363B0, 0x1363C0) ---
	  * On PS2: uploads the display context (palette, clip planes,
	  * viewport) and the first geometry/texture chunk to the GS.
	  *
	  * On PC this becomes:
	  *   - glTexImage2D for the logo
	  *   - glBufferData for the quad VBO
	  *   - glUniform* for the resolution and background colour
	  *
	  * TODO: implement once the assets are extracted with iso2assets.
	  * For now the "context" is: viewport + clear colour (done).
	  */
	int win_w, win_h;
	SDL_GetWindowSize(NULL, &win_w, &win_h);
	glViewport(0, 0, win_w, win_h);

	/*
	 * --- GIF_CTRL = 1 ---
	 * On PS2: enables the DMA engine. From here on the GS
	 * starts processing the packets that entered the FIFO.
	 *
	 * In GL there is no explicit equivalent. The first draw call
	 * (glDrawArrays / glDrawElements) triggers the pipeline.
	 * For the intro it will be something like:
	 *   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	 *
	 * TODO: once the logo mesh exists.
	 */
}
