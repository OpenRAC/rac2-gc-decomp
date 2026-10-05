#include "core/display.h"
#include "core/vsync.h"       /* graphics_canvas_data_t, graphics_canvas_data() */
#include "core/critical.h"    /* core_critical_enter / exit */
#include "ps2_kernel.h"       /* RemoveIntcHandler, kernel_system_sync_* */
#include <SDL.h>

/*
 * External GS functions (PS2). On PC they are wrappers or no-ops.
 * Declared in ps2_graphics.h (see below).
 */
#include "ps2_graphics.h"

 /*
  * ------------------------------------------------------------------
  *  PORT of FUN_001257d0 (0x001257D0)
  * ------------------------------------------------------------------
  *
  *  Original pseudocode (summary):
  *    switch (param_1):
  *      case 0:  // CONFIGURE + ENABLE
  *        canvas = g_GraphicsCanvasData();
  *        GS_CSR = 0x200;                  // screen ON
  *        canvas->display_mode = param_2;
  *        canvas->timing_param = param_3;
  *        canvas->csr_high = (GS_CSR >> 16) & 0xFF;
  *        GsPutIMR();                      // color/alpha mask
  *        canvas->flag = (param_4 != 0);
  *        if (canvas->irq_active != 0):
  *          core_critical_enter(2);
  *          RemoveIntcHandler(2, canvas->handler_data);
  *          canvas->handler_data = 0;
  *          canvas->irq_active   = 0;
  *        SetGsCrt();                      // apply timing to hardware
  *        return;
  *
  *      case 1:  // DISABLE
  *        GS_CSR = 0x100;                  // screen OFF
  *        return;
  *
  *      case 5:  // RECONFIGURE (no IRQ cleanup)
  *        canvas->flag = (param_4 != 0);
  *        canvas->display_mode = param_2;
  *        canvas->timing_param = param_3;
  *        canvas->csr_high = (GS_CSR >> 16) & 0xFF;
  *        SetGsCrt();
  *        return;
  *
  *      default: return;
  *
  *  PC:
  *    - GS_CSR 0x100/0x200 → SDL_HideWindow / SDL_ShowWindow
  *    - GsPutIMR → glClearColor / glColorMask / glBlendFunc
  *    - SetGsCrt → SDL_SetWindowMode (resolution) + glViewport
  *    - RemoveIntcHandler(2) → no-op (SDL handles vsync)
  * ------------------------------------------------------------------
  */

void core_display_set(display_command_t command,
    uint16_t mode,
    uint16_t timing,
    int      interlace)
{
    const graphics_canvas_data_t* canvas = graphics_canvas_data();

    switch (command) {

        /* ──────────────────────────────────────────────── */
    case DISPLAY_CMD_CONFIGURE:
    {
        /*
         * GS_CSR = 0x200 → "screen ON"
         * PC: show the window + enable the framebuffer
         */
        SDL_ShowWindow(NULL);  /* or SDL_SetWindowOpacity(win, 1.0f) */

        /*
         * canvas->display_mode = param_2
         * canvas->timing_param = param_3
         * canvas->csr_high = (GS_CSR >> 16) & 0xFF
         *
         * On PC these values determine the resolution and refresh.
         * The struct is filled so that vsync_wait() and the renderer
         * can read them.
         */
         /*
          * NOTE: graphics_canvas_data() returns const in the public API.
          * Internally there is a setter. For now we cast it (as on
          * the PS2, where it was a mutable global).
          */
        graphics_canvas_data_t* c = (graphics_canvas_data_t*)canvas;
        c->display_mode = mode;
        c->timing_param = timing;
        c->flag_interlace = (interlace != 0);

        /*
         * csr_high_byte = (GS_CSR >> 16) & 0xFF
         * On PS2: GS status byte (which pipeline is active).
         * On PC: derived from SDL_GetWindowFlags or left at 0.
         */
        c->csr_high_byte = 0x00;  /* TODO: derive from the GL state */

        /*
         * GsPutIMR() → Image Mask Register
         * Controls which RGBA channels are transferred.
         * PC equivalent: glColorMask + glBlendFunc
         */
        gputimr();  /* ← wrapper in ps2_graphics.c (see below) */

        /*
         * canvas->flag = (param_4 != 0)
         * (already written above as flag_interlace)
         */

         /*
          * if (canvas->irq_active != 0):
          *   core_critical_enter(2);
          *   RemoveIntcHandler(2, handler_data);
          *   handler_data = 0;
          *   irq_active   = 0;
          *
          * "If a VS IRQ handler was active, disable it
          *  atomically before reconfiguring the CRT."
          */
        if (c->irq_active != 0) {
            uint32_t prev = core_critical_enter(2);
            remove_intc_handler(2, c->intc_handler_data);
            c->intc_handler_data = 0;
            c->irq_active = 0;
            core_critical_exit(prev);
        }

        /*
         * SetGsCrt() → applies the timings to the GS hardware.
         * On PS2: writes HSYNC, VSYNC, total lines, resolution.
         * On PC: SDL_SetWindowMode + glViewport.
         */
        set_gs_crt();  /* ← wrapper in ps2_graphics.c */

        return;
    }

    /* ──────────────────────────────────────────────── */
    case DISPLAY_CMD_DISABLE:
    {
        /*
         * GS_CSR = 0x100 → "screen OFF"
         * On PS2: the GS stops scanning out, the CRT shows black.
         * On PC: hide the window or simply do not render.
         */
        SDL_HideWindow(NULL);
        /* Gentler alternative:
         * SDL_Window *win = SDL_GL_GetWindow();
         * SDL_SetWindowOpacity(win, 0.0f);
         */
        return;
    }

    /* ──────────────────────────────────────────────── */
    case DISPLAY_CMD_RECONFIGURE:
    {
        /*
         * Same as CONFIGURE but WITHOUT the IRQ cleanup section.
         * Used to change resolution in the middle of gameplay
         * without losing the vsync handler.
         */
        graphics_canvas_data_t* c = (graphics_canvas_data_t*)canvas;
        c->flag_interlace = (interlace != 0);
        c->display_mode = mode;
        c->timing_param = timing;
        c->csr_high_byte = 0x00;

        /* No GsPutIMR here (the mask does not change on a reconfigure) */
        /* No IRQ cleanup (the handler stays active) */

        set_gs_crt();
        return;
    }

    default:
        return;  /* no-op, faithful to the original */
    }
}
