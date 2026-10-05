#include "core/boot.h"
#include "core/gpu.h"
#include "core/display.h"

/*
 * Globals de display (equivalentes a DAT_001A7C18 / DAT_001A7C40)
 *
 * En PS2 eran flags en la sección .data del ELF.
 * En PC las mantenemos como globals del módulo core.
 *
 * Para mods: un mod puede setear g_display_mode_flag = 1
 *            para forzar interlace (visual retro).
 */

 /** Flag de "modo dev/debug kit". 1 = venimos de devkit. */
static int g_display_dev_flag = 0;

/** Flag de "modo display". 0 = progressive, !=0 = interlaced. */
static int g_display_mode_flag = 0;

/*
 * ------------------------------------------------------------------
 *  PORT de FUN_002848C0 (0x002848C0)
 * ------------------------------------------------------------------
 *
 *  ASM original (resumen):
 *    gpu_init_context();
 *    if (DAT_001A7C18 != 0) {
 *        *0x1A7C40 = 0;           // force progressive
 *    }
 *    if (DAT_001A7C40 == 0) {
 *        timing = (DAT_001A7C18 != 0) ? 2 : 3;
 *        core_display_set(0, 1, timing, 0);   // progressive
 *    } else {
 *        core_display_set(0, 0, 0x50, 1);     // interlaced
 *    }
 *
 *  PC:
 *    - DAT_001A7C18 → g_display_dev_flag (0 en retail)
 *    - DAT_001A7C40 → g_display_mode_flag (0 = progressive)
 *    - En retail: siempre va al branch progressive, timing=3
 * ------------------------------------------------------------------
 */
void boot_display_init(void) {
    /*
     * 1. Reset completo de la GPU
     *    (VIF1 + VU1 + GIF + context upload)
     */
    gpu_init_context();

    /*
     * 2. Si veníamos de un devkit, fuerza progressive
     *    (en retail esto nunca se ejecuta; g_display_dev_flag == 0)
     */
    if (g_display_dev_flag != 0) {
        g_display_mode_flag = 0;
    }

    /*
     * 3. Configurar display según el modo
     */
    if (g_display_mode_flag == 0) {
        /*
         * PROGRESSIVE (retail default para R&C2)
         *
         * core_display_set(cmd, mode, timing, interlace)
         *   cmd      = 0  (DISPLAY_CMD_CONFIGURE)
         *   mode     = 1  (screen on, full RGB)
         *   timing   = 2 si devkit, 3 si retail
         *             → en PC: 2 = 72Hz, 3 = 60Hz (arbitrario,
         *               lo usamos como "hint" de refresh target)
         *   interlace = 0
         */
        uint16_t timing = (g_display_dev_flag != 0) ? 2 : 3;
        core_display_set(DISPLAY_CMD_CONFIGURE, 1, timing, 0);
    }
    else {
        /*
         * INTERLACED (modo dev o PAL forzado)
         *
         * core_display_set(cmd, mode, timing, interlace)
         *   cmd      = 0  (DISPLAY_CMD_CONFIGURE)
         *   mode     = 0  (screen on, mode alternativo)
         *   timing   = 0x50 (80 → timing interlaced 30fps campos)
         *   interlace = 1
         */
        core_display_set(DISPLAY_CMD_CONFIGURE, 0, 0x50, 1);
    }
}