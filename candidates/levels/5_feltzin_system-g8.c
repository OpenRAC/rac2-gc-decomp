

/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_8172b0f0a91a7cba_SDataClearMode112Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_8172b0f0a91a7cba_SDataClearMode112Layout;


extern Rac2Native_8172b0f0a91a7cba_SDataClearMode112Layout LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00321998(void)
{
    LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE01 &= ~0x20;
    if (LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE00.mode_08 != 2)
        return;
    if (LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE00.selector_10 == 0) {
        LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE02 = 9;
        return;
    }
    if (LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE00.selector_10 == -1) {
        LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE00.selector_10 = 0;
        LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE02 = 9;
        return;
    }
    if (LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE00.selector_10 == -2)
        LVL_5_FELTZIN_SYSTEM_F8172b0f0a91a7cba_AT00321998_ROLE02 = 5;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_b09e58bdde03cd06_SDataRootMarker76Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_b09e58bdde03cd06_SDataRootMarker76Layout;


extern Rac2Native_b09e58bdde03cd06_SDataRootMarker76Layout LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE02;
extern int LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE03;
extern int LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE01;

void LVL_5_FELTZIN_SYSTEM_FUN_00321A08(void)
{
    if (LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE02.selector_10 != -2) {
        LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE03 = 3;
        return;
    }
    if (LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE01 != 0 || (LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE00 & 2) != 0)
        LVL_5_FELTZIN_SYSTEM_Fb09e58bdde03cd06_AT00321A08_ROLE03 = 6;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_584c3844a02fd1eb_SDataBranch124RootLayout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_584c3844a02fd1eb_SDataBranch124RootLayout;


extern Rac2Native_584c3844a02fd1eb_SDataBranch124RootLayout LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE02;
extern unsigned int LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE03;
extern int LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE01;

void LVL_5_FELTZIN_SYSTEM_FUN_00321A58(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE02.selector_10 != -2) {
        LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE03 = 3;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE00 & 0x20u) != 0) {
        LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE00 ^= 0x20u;
        if (LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE01 != 0)
            LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE03 = 23;
        else
            LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE03 = 5;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE00 & 0x8u) != 0) {
        LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE00 ^= 0x8u;
        LVL_5_FELTZIN_SYSTEM_F584c3844a02fd1eb_AT00321A58_ROLE03 = 7;
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


extern Rac2Native_a661193cb7db1192_SDataAccessorRootReset44Layout LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE01;

void LVL_5_FELTZIN_SYSTEM_FUN_00321AD8(void)
{
    LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE00.selector_10 = 0;
    if (LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE00.selected_164 < 0) {
        LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE00.value_168 = 0;
        LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE00.selected_164 = 3;
    }
    LVL_5_FELTZIN_SYSTEM_Fa661193cb7db1192_AT00321AD8_ROLE01 = 8;
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


extern Rac2Native_1716667661d8e672_SDataMoreGuard96Layout LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE00;
extern unsigned int LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00321B08(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE00.mode_15c != 2 || LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE00.selected_164 >= 0)
        return;
    if (LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE00.extra_16c != 0) {
        LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE02 = 17;
        LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE01 |= 0x40u;
        return;
    }
    LVL_5_FELTZIN_SYSTEM_F1716667661d8e672_AT00321B08_ROLE02 = 14;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_715e9b46a377ec8c_SDataMoreBits76RootLayout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_715e9b46a377ec8c_SDataMoreBits76RootLayout;


extern Rac2Native_715e9b46a377ec8c_SDataMoreBits76RootLayout LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE01;
extern unsigned int LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00321B68(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE01.selector_10 != 0) {
        LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE02 = 3;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE00 & 6u) != 0) {
        LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE02 = 10;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE00 & 0x200u) != 0)
        LVL_5_FELTZIN_SYSTEM_F715e9b46a377ec8c_AT00321B68_ROLE02 = 25;
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


extern Rac2Native_99b76d48da369e8d_SDataMoreThreshold156Layout LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE01;

void LVL_5_FELTZIN_SYSTEM_FUN_00321BF8(void)
{
    short phase;
    if (LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.mode_15c != 2 || LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.selected_164 >= 0)
        return;
    if (LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.extra_16c != 0) {
        LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE01 = 12;
        return;
    }
    if (LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.selector_10 < -1) {
        LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE01 = 3;
        return;
    }
    phase = LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.phase_18;
    if (phase == -2) {
        if (LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.value_0c + LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE00.value_20 < 475)
            LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE01 = 19;
        else
            LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE01 = 12;
        return;
    }
    if (phase >= -1)
        LVL_5_FELTZIN_SYSTEM_F99b76d48da369e8d_AT00321BF8_ROLE01 = 16;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_a0e246aa2dc0b8e7_SDataAccessorRoot56Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_a0e246aa2dc0b8e7_SDataAccessorRoot56Layout;


extern Rac2Native_a0e246aa2dc0b8e7_SDataAccessorRoot56Layout LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00321C98(void)
{
    if (LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE01.selector_10 != 0) {
        LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE02 = 3;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE00 & 2) != 0)
        LVL_5_FELTZIN_SYSTEM_Fa0e246aa2dc0b8e7_AT00321C98_ROLE02 = 13;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_998ff33d7f808dbf_SDataBranch120RootLayout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_998ff33d7f808dbf_SDataBranch120RootLayout;


extern Rac2Native_998ff33d7f808dbf_SDataBranch120RootLayout LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE02;
extern unsigned int LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE03;
extern int LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE01;

void LVL_5_FELTZIN_SYSTEM_FUN_00321CD0(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE02.selector_10 != 0) {
        LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE03 = 3;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE00 & 0x20u) != 0) {
        LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE00 ^= 0x20u;
        if (LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE01 != 0)
            LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE03 = 24;
        else
            LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE03 = 12;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE00 & 0x10u) != 0) {
        LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE00 ^= 0x10u;
        LVL_5_FELTZIN_SYSTEM_F998ff33d7f808dbf_AT00321CD0_ROLE03 = 14;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_60904fd170c92806_SDataMoreCleanup184Layout {
    unsigned char unknown_00[0x10];
    int selector_10;
    unsigned char unknown_14[0x168];
    int value_17c;
} Rac2Native_60904fd170c92806_SDataMoreCleanup184Layout;


extern Rac2Native_60904fd170c92806_SDataMoreCleanup184Layout LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00321E00(void)
{
    if ((LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 & 4) != 0)
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 &= ~4;
    if ((LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 & 2) != 0)
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 &= ~2;
    if ((LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 & 0x80) != 0) {
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE02 = 21;
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 = (LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 ^ 0x80) | 0x40;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 & 0x100) != 0) {
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE02 = 20;
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 = (LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE01 ^ 0x100) | 0x40;
        return;
    }
    if (LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE00.selector_10 != 0) {
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE02 = 3;
        return;
    }
    if (LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE00.value_17c != 0)
        LVL_5_FELTZIN_SYSTEM_F60904fd170c92806_AT00321E00_ROLE02 = 1;
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


extern Rac2Native_8391b53329533f81_SDataGuardedFlags112Layout LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00;
extern unsigned int LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00321F78(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00.mode_15c != 2 || LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00.selected_164 >= 0)
        return;
    if (LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00.extra_16c != 0 || LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00.word_24 != 0) {
        LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00.value_17c = 0;
        LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE02 = 21;
        LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE01 |= 0x440u;
        return;
    }
    LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE00.value_17c = 1;
    LVL_5_FELTZIN_SYSTEM_F8391b53329533f81_AT00321F78_ROLE02 = 1;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_83cf4a65b6159c6c_SDataRootClear72Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_83cf4a65b6159c6c_SDataRootClear72Layout;


extern Rac2Native_83cf4a65b6159c6c_SDataRootClear72Layout LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_003220F0(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE01.selector_10 != -2) {
        LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE02 = 3;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE00 & 0x20) != 0) {
        LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE00 ^= 0x20;
        LVL_5_FELTZIN_SYSTEM_F83cf4a65b6159c6c_AT003220F0_ROLE02 = 5;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_25b7bae55e05d925_SDataRootClear68Layout {
    unsigned char unknown_00[0x8];
    int mode_08;
    unsigned char unknown_0c[0x4];
    int selector_10;
} Rac2Native_25b7bae55e05d925_SDataRootClear68Layout;


extern Rac2Native_25b7bae55e05d925_SDataRootClear68Layout LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE01;
extern int LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE00;
extern int LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE02;

void LVL_5_FELTZIN_SYSTEM_FUN_00322138(void)
{
    if (LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE01.selector_10 != 0) {
        LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE02 = 3;
        return;
    }
    if ((LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE00 & 0x20) != 0) {
        LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE00 ^= 0x20;
        LVL_5_FELTZIN_SYSTEM_F25b7bae55e05d925_AT00322138_ROLE02 = 12;
    }
}
