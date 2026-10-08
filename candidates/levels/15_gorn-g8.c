

/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_8172b0f0a91a7cba_SDataClearMode112Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_8172b0f0a91a7cba_SDataClearMode112Layout;


extern Rac2Native_8172b0f0a91a7cba_SDataClearMode112Layout LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE00;
extern int LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE01;
extern int LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE02;

void LVL_15_GORN_FUN_00322170(void)
{
    LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE01 &= ~0x20;
    if (LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE00.mode_08 != 2)
        return;
    if (LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE00.selector_10 == 0) {
        LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE02 = 9;
        return;
    }
    if (LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE00.selector_10 == -1) {
        LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE00.selector_10 = 0;
        LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE02 = 9;
        return;
    }
    if (LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE00.selector_10 == -2)
        LVL_15_GORN_F8172b0f0a91a7cba_AT00322170_ROLE02 = 5;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_b09e58bdde03cd06_SDataRootMarker76Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_b09e58bdde03cd06_SDataRootMarker76Layout;


extern Rac2Native_b09e58bdde03cd06_SDataRootMarker76Layout LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE02;
extern int LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE00;
extern int LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE03;
extern int LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE01;

void LVL_15_GORN_FUN_003221E0(void)
{
    if (LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE02.selector_10 != -2) {
        LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE03 = 3;
        return;
    }
    if (LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE01 != 0 || (LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE00 & 2) != 0)
        LVL_15_GORN_Fb09e58bdde03cd06_AT003221E0_ROLE03 = 6;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_584c3844a02fd1eb_SDataBranch124RootLayout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_584c3844a02fd1eb_SDataBranch124RootLayout;


extern Rac2Native_584c3844a02fd1eb_SDataBranch124RootLayout LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE02;
extern unsigned int LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE00;
extern int LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE03;
extern int LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE01;

void LVL_15_GORN_FUN_00322230(void)
{
    if (LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE02.selector_10 != -2) {
        LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE03 = 3;
        return;
    }
    if ((LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE00 & 0x20u) != 0) {
        LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE00 ^= 0x20u;
        if (LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE01 != 0)
            LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE03 = 23;
        else
            LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE03 = 5;
        return;
    }
    if ((LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE00 & 0x8u) != 0) {
        LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE00 ^= 0x8u;
        LVL_15_GORN_F584c3844a02fd1eb_AT00322230_ROLE03 = 7;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_a661193cb7db1192_SDataAccessorRootReset44Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
    unsigned char unknown_14[0x150];
    int selected_164;
    int value_168;
} Rac2Native_a661193cb7db1192_SDataAccessorRootReset44Layout;


extern Rac2Native_a661193cb7db1192_SDataAccessorRootReset44Layout LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE00;
extern int LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE01;

void LVL_15_GORN_FUN_003222B0(void)
{
    LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE00.selector_10 = 0;
    if (LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE00.selected_164 < 0) {
        LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE00.value_168 = 0;
        LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE00.selected_164 = 3;
    }
    LVL_15_GORN_Fa661193cb7db1192_AT003222B0_ROLE01 = 8;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_1716667661d8e672_SDataMoreGuard96Layout {
    unsigned char unknown_00[0x15c];
    int mode_15c;
    unsigned char unknown_160[0x4];
    int selected_164;
    unsigned char unknown_168[0x4];
    int extra_16c;
} Rac2Native_1716667661d8e672_SDataMoreGuard96Layout;


extern Rac2Native_1716667661d8e672_SDataMoreGuard96Layout LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE00;
extern unsigned int LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE01;
extern int LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE02;

void LVL_15_GORN_FUN_003222E0(void)
{
    if (LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE00.mode_15c != 2 || LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE00.selected_164 >= 0)
        return;
    if (LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE00.extra_16c != 0) {
        LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE02 = 17;
        LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE01 |= 0x40u;
        return;
    }
    LVL_15_GORN_F1716667661d8e672_AT003222E0_ROLE02 = 14;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_715e9b46a377ec8c_SDataMoreBits76RootLayout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_715e9b46a377ec8c_SDataMoreBits76RootLayout;


extern Rac2Native_715e9b46a377ec8c_SDataMoreBits76RootLayout LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE01;
extern unsigned int LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE00;
extern int LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE02;

void LVL_15_GORN_FUN_00322340(void)
{
    if (LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE01.selector_10 != 0) {
        LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE02 = 3;
        return;
    }
    if ((LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE00 & 6u) != 0) {
        LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE02 = 10;
        return;
    }
    if ((LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE00 & 0x200u) != 0)
        LVL_15_GORN_F715e9b46a377ec8c_AT00322340_ROLE02 = 25;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_99b76d48da369e8d_SDataMoreThreshold156Layout {
    unsigned char unknown_00[0xc];
    int value_0c;
    int selector_10;
    unsigned char unknown_14[0x4];
    short phase_18;
    unsigned char unknown_1a[0x6];
    int value_20;
    unsigned char unknown_24[0x138];
    int mode_15c;
    unsigned char unknown_160[0x4];
    int selected_164;
    unsigned char unknown_168[0x4];
    int extra_16c;
} Rac2Native_99b76d48da369e8d_SDataMoreThreshold156Layout;


extern Rac2Native_99b76d48da369e8d_SDataMoreThreshold156Layout LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00;
extern int LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE01;

void LVL_15_GORN_FUN_003223D0(void)
{
    short phase;
    if (LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.mode_15c != 2 || LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.selected_164 >= 0)
        return;
    if (LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.extra_16c != 0) {
        LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE01 = 12;
        return;
    }
    if (LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.selector_10 < -1) {
        LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE01 = 3;
        return;
    }
    phase = LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.phase_18;
    if (phase == -2) {
        if (LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.value_0c + LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE00.value_20 < 475)
            LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE01 = 19;
        else
            LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE01 = 12;
        return;
    }
    if (phase >= -1)
        LVL_15_GORN_F99b76d48da369e8d_AT003223D0_ROLE01 = 16;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_a0e246aa2dc0b8e7_SDataAccessorRoot56Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_a0e246aa2dc0b8e7_SDataAccessorRoot56Layout;


extern Rac2Native_a0e246aa2dc0b8e7_SDataAccessorRoot56Layout LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE01;
extern int LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE00;
extern int LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE02;

void LVL_15_GORN_FUN_00322470(void)
{
    if (LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE01.selector_10 != 0) {
        LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE02 = 3;
        return;
    }
    if ((LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE00 & 2) != 0)
        LVL_15_GORN_Fa0e246aa2dc0b8e7_AT00322470_ROLE02 = 13;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_998ff33d7f808dbf_SDataBranch120RootLayout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_998ff33d7f808dbf_SDataBranch120RootLayout;


extern Rac2Native_998ff33d7f808dbf_SDataBranch120RootLayout LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE02;
extern unsigned int LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE00;
extern int LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE03;
extern int LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE01;

void LVL_15_GORN_FUN_003224A8(void)
{
    if (LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE02.selector_10 != 0) {
        LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE03 = 3;
        return;
    }
    if ((LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE00 & 0x20u) != 0) {
        LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE00 ^= 0x20u;
        if (LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE01 != 0)
            LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE03 = 24;
        else
            LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE03 = 12;
        return;
    }
    if ((LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE00 & 0x10u) != 0) {
        LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE00 ^= 0x10u;
        LVL_15_GORN_F998ff33d7f808dbf_AT003224A8_ROLE03 = 14;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_60904fd170c92806_SDataMoreCleanup184Layout {
    unsigned char unknown_00[0x10];
    int selector_10;
    unsigned char unknown_14[0x168];
    int value_17c;
} Rac2Native_60904fd170c92806_SDataMoreCleanup184Layout;


extern Rac2Native_60904fd170c92806_SDataMoreCleanup184Layout LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE00;
extern int LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01;
extern int LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE02;

void LVL_15_GORN_FUN_003225D8(void)
{
    if ((LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 & 4) != 0)
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 &= ~4;
    if ((LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 & 2) != 0)
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 &= ~2;
    if ((LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 & 0x80) != 0) {
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE02 = 21;
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 = (LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 ^ 0x80) | 0x40;
        return;
    }
    if ((LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 & 0x100) != 0) {
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE02 = 20;
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 = (LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE01 ^ 0x100) | 0x40;
        return;
    }
    if (LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE00.selector_10 != 0) {
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE02 = 3;
        return;
    }
    if (LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE00.value_17c != 0)
        LVL_15_GORN_F60904fd170c92806_AT003225D8_ROLE02 = 1;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_8391b53329533f81_SDataGuardedFlags112Layout {
    unsigned char unknown_00[0x24];
    int word_24;
    unsigned char unknown_28[0x134];
    int mode_15c;
    unsigned char unknown_160[0x4];
    int selected_164;
    unsigned char unknown_168[0x4];
    int extra_16c;
    unsigned char unknown_170[0xc];
    int value_17c;
} Rac2Native_8391b53329533f81_SDataGuardedFlags112Layout;


extern Rac2Native_8391b53329533f81_SDataGuardedFlags112Layout LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00;
extern unsigned int LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE01;
extern int LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE02;

void LVL_15_GORN_FUN_00322750(void)
{
    if (LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00.mode_15c != 2 || LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00.selected_164 >= 0)
        return;
    if (LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00.extra_16c != 0 || LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00.word_24 != 0) {
        LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00.value_17c = 0;
        LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE02 = 21;
        LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE01 |= 0x440u;
        return;
    }
    LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE00.value_17c = 1;
    LVL_15_GORN_F8391b53329533f81_AT00322750_ROLE02 = 1;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_83cf4a65b6159c6c_SDataRootClear72Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_83cf4a65b6159c6c_SDataRootClear72Layout;


extern Rac2Native_83cf4a65b6159c6c_SDataRootClear72Layout LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE01;
extern int LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE00;
extern int LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE02;

void LVL_15_GORN_FUN_003228C8(void)
{
    if (LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE01.selector_10 != -2) {
        LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE02 = 3;
        return;
    }
    if ((LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE00 & 0x20) != 0) {
        LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE00 ^= 0x20;
        LVL_15_GORN_F83cf4a65b6159c6c_AT003228C8_ROLE02 = 5;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_25b7bae55e05d925_SDataRootClear68Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_25b7bae55e05d925_SDataRootClear68Layout;


extern Rac2Native_25b7bae55e05d925_SDataRootClear68Layout LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE01;
extern int LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE00;
extern int LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE02;

void LVL_15_GORN_FUN_00322910(void)
{
    if (LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE01.selector_10 != 0) {
        LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE02 = 3;
        return;
    }
    if ((LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE00 & 0x20) != 0) {
        LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE00 ^= 0x20;
        LVL_15_GORN_F25b7bae55e05d925_AT00322910_ROLE02 = 12;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct {
    unsigned char reserved_00[0x18];
    unsigned char enabled;
    unsigned char count;
    unsigned char kind;
    unsigned char reserved_1b[0x65];
    unsigned char payload[1];
} Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchContext;

typedef unsigned long Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchFlags;


extern int LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE02;
extern void LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE03(void *owner, void *payload, int count);
extern Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchFlags LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE01(void *owner, Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchContext *context);
extern Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchFlags LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE00(void *owner, Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchContext *context);

Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchFlags LVL_15_GORN_FUN_0033F7E8(void *owner, Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchContext *context)
{
    Rac2Native_bf55aa2a4d0dd351_SDataCallDispatchFlags result = 0;
    if (context->enabled == 1 && context->count != 0)
        LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE03(owner, context->payload, context->count);
    switch (context->kind) {
    case 0:
    case 1:
    case 2:
        if (LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE02 == 0)
            result = LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE01(owner, context);
        else
            result = LVL_15_GORN_Fbf55aa2a4d0dd351_AT0033F7E8_ROLE00(owner, context);
        break;
    default:
        break;
    }
    if ((result & 2) != 0) context->enabled = 0;
    return result;
}
