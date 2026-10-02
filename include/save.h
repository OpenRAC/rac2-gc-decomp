#ifndef SAVE_H
#define SAVE_H

#include "common.h"

/*
 * Insomniac Save Game IFF (Interchange File Format) Chunk IDs.
 *
 * Recovered from save file analysis (chaoticgd, creepnt 2024-2026)
 * and cross-referenced with PS2 memory card save streams:
 *   - NTSC-U: BASCUS-97199 (RaC1), BASCUS-97268 (RaC2)
 *   - PAL:    BESCES-50916 (RaC1), BESCES-51607 (RaC2)
 */

typedef enum SaveGameBlockId {
    SAVE_BLOCK_LEVEL                    = 0,     /* Current / last level ID (int) */
    SAVE_BLOCK_BOLT_COUNT               = 1,     /* Total bolt count (int) */
    SAVE_BLOCK_GAME_COMPLETES           = 2,     /* Playthrough / challenge mode count */
    SAVE_BLOCK_ELAPSED_TIME             = 3,     /* In-game elapsed time */
    SAVE_BLOCK_LAST_SAVE_TIME           = 4,     /* Timestamp (sceCdCLOCK) */
    SAVE_BLOCK_GLOBAL_FLAGS             = 5,     /* Global progression bit flags */
    SAVE_BLOCK_CHEATS_ACTIVATED         = 7,     /* Active cheats bitmask */
    SAVE_BLOCK_SKILL_POINTS             = 8,     /* Skill points unlocked */
    SAVE_BLOCK_AMMO                     = 9,     /* Current ammo table */
    SAVE_BLOCK_UNLOCKS                  = 10,    /* Weapon / Gadget unlock bitmask */
    SAVE_BLOCK_PURCHASABLE_VENDOR_ITEMS = 12,    /* Vendor purchase availability */
    SAVE_BLOCK_GALACTIC_MAP             = 14,    /* Visited planet coordinates */
    SAVE_BLOCK_HELP_MESSAGES            = 16,    /* Help prompt flags */
    SAVE_BLOCK_CAMERA_UP_DOWN_MODE      = 25,    /* Invert pitch setting */
    SAVE_BLOCK_CAMERA_LEFT_RIGHT_MODE   = 26,    /* Invert yaw setting */
    SAVE_BLOCK_CAMERA_ROTATION_SPEED    = 27,    /* Camera sensitivity */
    SAVE_BLOCK_CHEATS_EVER_ACTIVATED    = 37,    /* Permanent cheat taint flag */
    SAVE_BLOCK_TOTAL_PLAY_TIME          = 1003,  /* Lifetime playtime */
    SAVE_BLOCK_TOTAL_DEATHS             = 1005   /* Lifetime death counter */
} SaveGameBlockId;

/*
 * RaC1 Save Import Legacy Weapon bitmask in SAVE_BLOCK_UNLOCKS (Block 10):
 * When detected by the Gadgetron vendor on Planet Barlow in RaC2,
 * legacy weapons are granted for free (0 bolts).
 */
#define RAC1_SAVE_UNLOCK_BOMB_GLOVE     (1 << 0)  /* 0x01: Bomb Glove */
#define RAC1_SAVE_UNLOCK_PYROCITOR      (1 << 1)  /* 0x02: Pyrocitor */
#define RAC1_SAVE_UNLOCK_BLASTER        (1 << 2)  /* 0x04: Blaster */
#define RAC1_SAVE_UNLOCK_GLOVE_OF_DOOM  (1 << 3)  /* 0x08: Glove of Doom */
#define RAC1_SAVE_UNLOCK_SUCK_CANNON    (1 << 4)  /* 0x10: Suck Cannon */

#endif /* SAVE_H */
