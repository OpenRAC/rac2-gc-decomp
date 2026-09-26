#include "core/display.h"
#include "core/vsync.h"       /* graphics_canvas_data_t, graphics_canvas_data() */
#include "core/critical.h"    /* core_critical_enter / exit */
#include "ps2_kernel.h"       /* RemoveIntcHandler, kernel_system_sync_* */
#include <SDL.h>

/*
 * Funciones externas del GS (PS2). En PC son wrappers o no-ops.
 * Declaradas en ps2_graphics.h (ver abajo).
 */
#include "ps2_graphics.h"

 /*
  * ------------------------------------------------------------------
  *  PORT de FUN_001257d0 (0x001257D0)
  * ------------------------------------------------------------------
  *
  *  Pseudocódigo original (resumen):
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
  *    - SetGsCrt → SDL_SetWindowMode (resolución) + glViewport
  *    - RemoveIntcHandler(2) → no-op (SDL maneja vsync)
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
         * PC: mostrar la ventana + activar el framebuffer
         */
        SDL_ShowWindow(NULL);  /* o SDL_SetWindowOpacity(win, 1.0f) */

        /*
         * canvas->display_mode = param_2
         * canvas->timing_param = param_3
         * canvas->csr_high = (GS_CSR >> 16) & 0xFF
         *
         * En PC estos valores determinan la resolución y refresh.
         * El struct se llena para que vsync_wait() y el renderer
         * puedan leerlos.
         */
         /*
          * NOTA: graphics_canvas_data() retorna const en la API pública.
          * Internamente hay un setter. Por ahora lo casteamos (igual
          * que en PS2 donde era una global mutable).
          */
        graphics_canvas_data_t* c = (graphics_canvas_data_t*)canvas;
        c->display_mode = mode;
        c->timing_param = timing;
        c->flag_interlace = (interlace != 0);

        /*
         * csr_high_byte = (GS_CSR >> 16) & 0xFF
         * En PS2: byte de estado del GS (qué pipeline está activo).
         * En PC: lo derivamos de SDL_GetWindowFlags o lo dejamos 0.
         */
        c->csr_high_byte = 0x00;  /* TODO: derivar de estado GL */

        /*
         * GsPutIMR() → Image Mask Register
         * Controla qué canales RGBA se transfieren.
         * PC equivalente: glColorMask + glBlendFunc
         */
        gputimr();  /* ← wrapper en ps2_graphics.c (ver abajo) */

        /*
         * canvas->flag = (param_4 != 0)
         * (ya escrito arriba como flag_interlace)
         */

         /*
          * if (canvas->irq_active != 0):
          *   core_critical_enter(2);
          *   RemoveIntcHandler(2, handler_data);
          *   handler_data = 0;
          *   irq_active   = 0;
          *
          * "Si había un handler de VS IRQ activo, desactívalo
          *  de forma atómica antes de reconfigurar el CRT."
          */
        if (c->irq_active != 0) {
            uint32_t prev = core_critical_enter(2);
            remove_intc_handler(2, c->intc_handler_data);
            c->intc_handler_data = 0;
            c->irq_active = 0;
            core_critical_exit(prev);
        }

        /*
         * SetGsCrt() → aplica los timings al hardware del GS.
         * En PS2: escribe HSYNC, VSYNC, total lines, resolution.
         * En PC: SDL_SetWindowMode + glViewport.
         */
        set_gs_crt();  /* ← wrapper en ps2_graphics.c */

        return;
    }

    /* ──────────────────────────────────────────────── */
    case DISPLAY_CMD_DISABLE:
    {
        /*
         * GS_CSR = 0x100 → "screen OFF"
         * En PS2: el GS deja de escanear, el CRT muestra negro.
         * En PC: ocultar ventana o simplemente no renderizar.
         */
        SDL_HideWindow(NULL);
        /* Alternativa más suave:
         * SDL_Window *win = SDL_GL_GetWindow();
         * SDL_SetWindowOpacity(win, 0.0f);
         */
        return;
    }

    /* ──────────────────────────────────────────────── */
    case DISPLAY_CMD_RECONFIGURE:
    {
        /*
         * Igual que CONFIGURE pero SIN la sección de IRQ cleanup.
         * Se usa para cambiar de resolución a mitad de gameplay
         * sin perder el vsync handler.
         */
        graphics_canvas_data_t* c = (graphics_canvas_data_t*)canvas;
        c->flag_interlace = (interlace != 0);
        c->display_mode = mode;
        c->timing_param = timing;
        c->csr_high_byte = 0x00;

        /* Sin GsPutIMR aquí (el mask no cambia en un reconfigure) */
        /* Sin IRQ cleanup (el handler sigue activo) */

        set_gs_crt();
        return;
    }

    default:
        return;  /* no-op, fiel al original */
    }
}
