#include "core/boot.h"
#include "core/gpu.h"
#include "core/display.h"

/*
 * Display globals (equivalent to DAT_001A7C18 / DAT_001A7C40)
 *
 * On PS2 they were flags in the .data section of the ELF.
 * On PC we keep them as globals of the core module.
 *
 * For mods: a mod can set g_display_mode_flag = 1
 *            to force interlace (retro look).
 */

 /** "Dev/debug kit mode" flag. 1 = coming from a devkit. */
static int g_display_dev_flag = 0;

/** "Display mode" flag. 0 = progressive, !=0 = interlaced. */
static int g_display_mode_flag = 0;

/*
 * ------------------------------------------------------------------
 *  PORT of FUN_002848C0 (0x002848C0)
 * ------------------------------------------------------------------
 *
 *  Original ASM (summary):
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
 *    - DAT_001A7C18 → g_display_dev_flag (0 in retail)
 *    - DAT_001A7C40 → g_display_mode_flag (0 = progressive)
 *    - In retail: always takes the progressive branch, timing=3
 * ------------------------------------------------------------------
 */
void boot_display_init(void) {
    /*
     * 1. Full GPU reset
     *    (VIF1 + VU1 + GIF + context upload)
     */
    gpu_init_context();

    /*
     * 2. Coming from a devkit forces progressive
     *    (in retail this never runs; g_display_dev_flag == 0)
     */
    if (g_display_dev_flag != 0) {
        g_display_mode_flag = 0;
    }

    /*
     * 3. Configure the display according to the mode
     */
    if (g_display_mode_flag == 0) {
        /*
         * PROGRESSIVE (retail default for R&C2)
         *
         * core_display_set(cmd, mode, timing, interlace)
         *   cmd      = 0  (DISPLAY_CMD_CONFIGURE)
         *   mode     = 1  (screen on, full RGB)
         *   timing   = 2 on devkit, 3 on retail
         *             → on PC: 2 = 72Hz, 3 = 60Hz (arbitrary,
         *               used as a refresh-target "hint")
         *   interlace = 0
         */
        uint16_t timing = (g_display_dev_flag != 0) ? 2 : 3;
        core_display_set(DISPLAY_CMD_CONFIGURE, 1, timing, 0);
    }
    else {
        /*
         * INTERLACED (dev mode or forced PAL)
         *
         * core_display_set(cmd, mode, timing, interlace)
         *   cmd      = 0  (DISPLAY_CMD_CONFIGURE)
         *   mode     = 0  (screen on, alternate mode)
         *   timing   = 0x50 (80 → timing interlaced 30fps campos)
         *   interlace = 1
         */
        core_display_set(DISPLAY_CMD_CONFIGURE, 0, 0x50, 1);
    }
}