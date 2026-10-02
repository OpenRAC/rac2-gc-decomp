#ifndef MOBY_H
#define MOBY_H

#include "common.h"

/*
 * Moby Instance Structure for Ratchet & Clank 2 (Going Commando).
 *
 * Recovered from original Insomniac .mdebug STABS symbols (Deadlocked prototype leak)
 * and verified against retail function signatures (InitMobyInstance: 0x100 / 256 bytes).
 */

struct Moby;

typedef void (*MobyUpdateFunc)(struct Moby *moby);

/*
 * Moby modeBits flags (subset known):
 *   0x40 - No pre-update
 */
#define MOBY_MODE_NO_PRE_UPDATE        0x40

/*
 * Known Moby Class IDs (oClass):
 *   4351 - Debug "clank-switch" trigger moby (G34, @creepnt 2021)
 */
#define MOBY_OCLASS_DEBUG_CLANK_SWITCH 4351

/*
 * Moby Update Dispatch:
 *   Vita port leak (C:\projects\RCVita\RC_Vita\rc2\code\game\boot.cpp) confirms
 *   an update table with 8192 entries indexed across loaded actors.
 */
#define MOBY_UPDATE_TABLE_CAPACITY     8192

typedef struct BSphere {
    f32 x;
    f32 y;
    f32 z;
    f32 rad;
} BSphere;

typedef struct vec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} vec4;

/*
 * Authentic Insomniac MobyInstance structure (256 bytes / 0x100).
 * Demangled signature: InitMobyInstance(MobyInstance *, int)
 */
typedef struct Moby {
    /* 0x00 */ BSphere       bSphere;              /* 0x00: Bounding sphere in world space */
    /* 0x10 */ vec4          pos;                  /* 0x10: World position (x, y, z, w) */
    /* 0x20 */ u8            state;                /* 0x20: Current state (0xFE=free, 0xFF=tail) */
    /* 0x21 */ u8            group;                /* 0x21: Collision / grouping index */
    /* 0x22 */ u8            mclass;               /* 0x22: Class sub-identifier / moby class */
    /* 0x23 */ u8            alpha;                /* 0x23: Opacity / blend alpha (default 0x80) */
    /* 0x24 */ void         *pClass;               /* 0x24: Pointer to class definition record */
    /* 0x28 */ struct Moby  *pChain;               /* 0x28: Next moby in active update chain */
    /* 0x2C */ u8            collDamage;           /* 0x2C: Collision damage value */
    /* 0x2D */ u8            deathCnt;             /* 0x2D: Death counter */
    /* 0x2E */ u16           occlIndex;            /* 0x2E: Occlusion index */
    /* 0x30 */ u8            updateDist;           /* 0x30: Update distance threshold */
    /* 0x31 */ u8            drawn;                /* 0x31: Drawn flag */
    /* 0x32 */ u16           drawDist;             /* 0x32: Draw distance threshold */
    /* 0x34 */ u16           modeBits;             /* 0x34: Behavioral mode flags (0x40 = no pre-update) */
    /* 0x36 */ u16           modeBits2;            /* 0x36: Secondary mode flags */
    /* 0x38 */ u64           lights;               /* 0x38: Lighting bitmask */
    /* 0x40 */ void         *animSeq;              /* 0x40: Animation sequence pointer */
    /* 0x44 */ f32           animSeqT;             /* 0x44: Animation time parameter */
    /* 0x48 */ f32           animSpeed;            /* 0x48: Animation playback speed */
    /* 0x4C */ s16           animIScale;           /* 0x4C: Animation interpolation scale */
    /* 0x4E */ s16           poseCacheEntryIndex;  /* 0x4E: Pose cache entry index */
    /* 0x50 */ void         *animLayers;           /* 0x50: Animation layer list */
    /* 0x54 */ u8            animSeqId;            /* 0x54: Active sequence ID */
    /* 0x55 */ u8            animFlags;            /* 0x55: Animation flags */
    /* 0x56 */ u8            lSeq;                 /* 0x56: Loop / layer sequence */
    /* 0x57 */ u8            jointCnt;             /* 0x57: Joint count */
    /* 0x58 */ void         *jointCache;           /* 0x58: Joint matrix cache */
    /* 0x5C */ void         *pManipulator;         /* 0x5C: Joint / IK manipulator */
    /* 0x60 */ u32           glow_rgba;            /* 0x60: Glow color (RGBA) */
    /* 0x64 */ u8            lod_trans;            /* 0x64: LOD transition */
    /* 0x65 */ u8            lod_trans2;           /* 0x65: Secondary LOD transition */
    /* 0x66 */ u8            metal;                /* 0x66: Metal surface flag */
    /* 0x67 */ u8            subState;             /* 0x67: Sub-state */
    /* 0x68 */ u8            prevState;            /* 0x68: Previous state */
    /* 0x69 */ u8            stateType;            /* 0x69: State type enum */
    /* 0x6A */ u16           stateTimer;           /* 0x6A: Frames in current state */
    /* 0x6C */ u8            soundTrigger;         /* 0x6C: Sound trigger */
    /* 0x6D */ u8            soundDesired;         /* 0x6D: Desired sound ID */
    /* 0x6E */ u16           soundChannel;         /* 0x6E: Audio channel index */
    /* 0x70 */ f32           scale;                /* 0x70: Uniform scale */
    /* 0x74 */ u16           bangles;              /* 0x74: Attachment bitmask / bangles */
    /* 0x76 */ u8            shadow;               /* 0x76: Shadow flag */
    /* 0x77 */ u8            shadow_index;         /* 0x77: Shadow texture index */
    /* 0x78 */ f32           shadow_plane;         /* 0x78: Ground plane Y for shadow */
    /* 0x7C */ f32           shadow_range;         /* 0x7C: Max shadow render distance */
    /* 0x80 */ BSphere       lSphere;              /* 0x80: Local bounding sphere */
    /* 0x90 */ void         *netObject;            /* 0x90: Network object descriptor */
    /* 0x94 */ u16           updateID;             /* 0x94: Update tick ID */
    /* 0x96 */ u16           spad0;                /* 0x96: Scratchpad scratch variable */
    /* 0x98 */ void         *collData;             /* 0x98: Collision mesh / pill data */
    /* 0x9C */ u32           collActive;           /* 0x9C: Active collision bitmask */
    /* 0xA0 */ s32           collCnt;              /* 0xA0: Collision check count */
    /* 0xA4 */ u8            grid_min_x;           /* 0xA4: PVS grid bounds min X */
    /* 0xA5 */ u8            grid_min_y;           /* 0xA5: PVS grid bounds min Y */
    /* 0xA6 */ u8            grid_max_x;           /* 0xA6: PVS grid bounds max X */
    /* 0xA7 */ u8            grid_max_y;           /* 0xA7: PVS grid bounds max Y */
    /* 0xA8 */ MobyUpdateFunc pUpdate;             /* 0xA8: Per-tick update function pointer */
    /* 0xAC */ void         *pVar;                 /* 0xAC: Pointer to moby-specific state (pVars) */
    /* 0xB0 */ u8            mission;              /* 0xB0: Mission ID */
    /* 0xB1 */ u8            pad;
    /* 0xB2 */ s16           UID;                  /* 0xB2: Unique actor ID in loaded level */
    /* 0xB4 */ s16           bolts;                /* 0xB4: Bolt drop reward */
    /* 0xB6 */ u16           xp;                   /* 0xB6: Experience / nanotech reward */
    /* 0xB8 */ struct Moby  *pParent;              /* 0xB8: Pointer to parent moby or joint node */
    /* 0xBC */ s16           oClass;               /* 0xBC: Object class ID */
    /* 0xBE */ u8            triggers;             /* 0xBE: Trigger flags */
    /* 0xBF */ u8            standarddeathcalled;  /* 0xBF: Standard death handler called flag */
    /* 0xC0 */ f32           rMtx[3][4];           /* 0xC0: 3x4 orientation / rotation matrix */
    /* 0xF0 */ vec4          rot;                  /* 0xF0: Orientation Euler angles (Pitch, Yaw, Roll) */
} Moby;

typedef struct Moby MobyInstance;

/* Core engine moby management APIs */
void CreateMobyChain(void);
void PreUpdateMoby(Moby *moby);
void PostUpdateMoby(Moby *moby);

#endif /* MOBY_H */
