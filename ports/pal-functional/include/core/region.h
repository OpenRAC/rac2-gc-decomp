// include/core/region.h
#ifndef RAC2_CORE_REGION_H
#define RAC2_CORE_REGION_H

/*
 * Release region selected at build time (CMake option RAC2_REGION, which
 * defines RAC2_REGION_PAL or RAC2_REGION_NTSC).
 *
 * The reconstruction was made from the European PAL executable SCES_516.07.
 * The USA release SCUS_972.68 runs the same engine on an NTSC display. Only
 * values that follow from the video standard or the release identity are
 * switched here: the boot executable name, the GS output mode, the frame clock
 * and the base display height. Every RAM address quoted in the sources is
 * still a PAL address; a USA build needs a PAL-to-USA address map before those
 * accesses can be switched as well.
 */

#if defined(RAC2_REGION_NTSC) && defined(RAC2_REGION_PAL)
#error "Define only one of RAC2_REGION_NTSC and RAC2_REGION_PAL"
#endif
#if !defined(RAC2_REGION_NTSC) && !defined(RAC2_REGION_PAL)
#define RAC2_REGION_PAL 1   /* the release this reconstruction was made from */
#endif

#if defined(RAC2_REGION_NTSC)
#define RAC2_REGION_NAME     "NTSC-U"
#define RAC2_BOOT_SERIAL     "SCUS_972.68"
#define RAC2_GS_OMODE        2                 /* SCE_GS_NTSC */
#define RAC2_FIELD_RATE_HZ   59.94
#define RAC2_DISPLAY_HEIGHT  448
#else
#define RAC2_REGION_NAME     "PAL"
#define RAC2_BOOT_SERIAL     "SCES_516.07"
#define RAC2_GS_OMODE        3                 /* SCE_GS_PAL */
#define RAC2_FIELD_RATE_HZ   50.0
#define RAC2_DISPLAY_HEIGHT  512
#endif

#define RAC2_DISPLAY_WIDTH    640
/* One engine tick per displayed field: 20 ms on PAL, about 16.68 ms on NTSC. */
#define RAC2_FRAME_TARGET_MS  (1000.0 / RAC2_FIELD_RATE_HZ)

#endif // RAC2_CORE_REGION_H
