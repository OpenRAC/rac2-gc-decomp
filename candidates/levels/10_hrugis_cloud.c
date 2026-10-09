typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_10_HRUGIS_CLOUD_D_001A8EB0;
void LVL_10_HRUGIS_CLOUD_FUN_002F25E0(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_10_HRUGIS_CLOUD_FUN_0030E810(s32 index) {
    NativeTable20 values=LVL_10_HRUGIS_CLOUD_D_001A8EB0;
    return values.items[index];
}

u32 LVL_10_HRUGIS_CLOUD_FUN_002C8D50(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_10_HRUGIS_CLOUD_D_0018C0B4;
u32 LVL_10_HRUGIS_CLOUD_FUN_002F1620(void) {
    return LVL_10_HRUGIS_CLOUD_D_0018C0B4;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002FE708(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_10_HRUGIS_CLOUD_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_10_HRUGIS_CLOUD_FUN_002C8BE8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_10_HRUGIS_CLOUD_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002C8C20(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_10_HRUGIS_CLOUD_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002C9840(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_10_HRUGIS_CLOUD_FUN_002F2338(f32, f32, f32, f32, s32, s32);

void LVL_10_HRUGIS_CLOUD_FUN_002F1DE0(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_10_HRUGIS_CLOUD_FUN_002F2338(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_10_HRUGIS_CLOUD_FUN_0030B068(int width, int height, int address, int mode);
extern void LVL_10_HRUGIS_CLOUD_FUN_003923D8(unsigned int reg, unsigned long value);
extern void LVL_10_HRUGIS_CLOUD_FUN_0030B3D0(int width, int height);

void LVL_10_HRUGIS_CLOUD_FUN_00300530(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_10_HRUGIS_CLOUD_FUN_0030B068(width, height, address, 1);
    LVL_10_HRUGIS_CLOUD_FUN_003923D8(0x47, 0x30000UL);
    LVL_10_HRUGIS_CLOUD_FUN_003923D8(0x42, 0x8000000044UL);
    LVL_10_HRUGIS_CLOUD_FUN_0030B3D0(0x100, 0x100);
    LVL_10_HRUGIS_CLOUD_FUN_003923D8(0x42, 0x8000000044UL);
}

extern void LVL_10_HRUGIS_CLOUD_FUN_002EE460(f32, f32, f32, f32 *, s32);

void LVL_10_HRUGIS_CLOUD_FUN_002C8A78(f32 *output) {
 LVL_10_HRUGIS_CLOUD_FUN_002EE460(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002C8C58(s32 index) {
 s32 value=LVL_10_HRUGIS_CLOUD_FUN_002C8C20(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002C8C90(s32 index) {
 return LVL_10_HRUGIS_CLOUD_FUN_002C8C20(index)==47;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002CDFC8(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

f32 LVL_10_HRUGIS_CLOUD_FUN_0042BC78(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_10_HRUGIS_CLOUD_FUN_00355DC8(const s32 *count, s32 index, s32 delta, s32 wrap) {
    if (wrap != 0) {
        index = (index + delta + *count) % *count;
    } else {
        index += delta;
        if (delta < 0) {
            if (index < 0) index = 0;
        } else if (index > *count - 1) {
            index = *count - 1;
        }
    }
    return index;
}

/* Update the measured fields of the first matching key among13records. */
typedef struct { unsigned int fields[25]; int key; unsigned int busy; unsigned int tail[9]; } UpdateRecord;
typedef char UpdateRecordSize[(sizeof(UpdateRecord) == 144) ? 1 : -1];
extern UpdateRecord LVL_10_HRUGIS_CLOUD_D_00296600[13];
void LVL_10_HRUGIS_CLOUD_FUN_00311930(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_10_HRUGIS_CLOUD_D_00296600[i].key == key) break;
    }
    if (i < 13) {
        LVL_10_HRUGIS_CLOUD_D_00296600[i].fields[9] = value;
        if (LVL_10_HRUGIS_CLOUD_D_00296600[i].busy == 0)
            LVL_10_HRUGIS_CLOUD_D_00296600[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_10_HRUGIS_CLOUD_D_001395B8[];
u32 LVL_10_HRUGIS_CLOUD_FUN_00320408(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_10_HRUGIS_CLOUD_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_10_HRUGIS_CLOUD_D_00231E00[];
s32 LVL_10_HRUGIS_CLOUD_FUN_00394CB8(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_10_HRUGIS_CLOUD_D_00231E00;
    s32 checked=0;
    do {
        ++checked;
        if(entry->field04==key && entry->field00==owner) return entry->field08;
        ++entry;
    } while(checked<32);
    return -1;
}

/* Apply nonzero signed halfword overrides to indexed rows of selected objects. */
typedef struct {
    unsigned int first;
    unsigned char reserved04[0x1c];
    unsigned int second;
    unsigned char reserved24[0x0f];
    unsigned char key;
    unsigned char reserved34[0x1c];
} ListOverrideRow;
typedef struct {
    unsigned char reserved00[0x0f];
    unsigned char count;
    unsigned char reserved10[0x0c];
    ListOverrideRow *rows;
} ListOverrideObject;
typedef struct { short first; short second; } ListOverridePair;
extern int LVL_10_HRUGIS_CLOUD_D_0022E6C0[];
extern ListOverrideObject *LVL_10_HRUGIS_CLOUD_D_002271C0[];
extern ListOverridePair LVL_10_HRUGIS_CLOUD_D_0022E0C0[];
void LVL_10_HRUGIS_CLOUD_FUN_00381D80(void)
{
    int *selected = LVL_10_HRUGIS_CLOUD_D_0022E6C0;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_10_HRUGIS_CLOUD_D_002271C0[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_10_HRUGIS_CLOUD_D_0022E0C0[row->key];
            if (pair->first != 0)
                row->first = (row->first & 0xffffc000u) | pair->first;
            if (pair->second != 0)
                row->second = (row->second & 0xffffc000u) | pair->second;
            i++;
            row++;
        }
        selected++;
    }
}

/* Finds the first matching object in the thirty resident records. */
typedef struct {
    u8 field00[0x14];
    void *field14;
    u8 field18[8];
} NativeObjectSearchRecord32;
extern NativeObjectSearchRecord32 LVL_10_HRUGIS_CLOUD_D_00262930[];
s32 LVL_10_HRUGIS_CLOUD_FUN_004006F8(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_10_HRUGIS_CLOUD_D_00262930[index].field14 == object) {
            result = index;
            break;
        }
    }
    return result;
}

typedef struct { u8 prefix[0xaa]; short class_id; } ClassFilterObject;
typedef struct {
    u8 prefix[0x33c];
    ClassFilterObject *fallback;
    u8 middle[0x14f0];
    ClassFilterObject *primary;
    u8 trailing[0xa60];
    s32 mode;
} ClassFilterRoot;

s32 LVL_10_HRUGIS_CLOUD_FUN_002F14E0(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_10_HRUGIS_CLOUD_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_10_HRUGIS_CLOUD_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_10_HRUGIS_CLOUD_D_00189E20)->mode == 0x31) {
        if (primary) {
            switch (selection) {
                case 0: found = primary->class_id == 0xfbf; break;
                case 1: found = primary->class_id == 0x905; break;
                case 2: found = primary->class_id == 0xeef; break;
            }
        } else if (fallback && selection == 3) {
            found = fallback->class_id == 0xc20;
        }
    }
    return found;
}

/* Select one of three measured pointer slots for the requested kind. */
extern void *LVL_10_HRUGIS_CLOUD_D_0018C0B0;
extern void *LVL_10_HRUGIS_CLOUD_D_0018B134;
extern void *LVL_10_HRUGIS_CLOUD_D_0018B040;
void *LVL_10_HRUGIS_CLOUD_FUN_002BFD68(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_10_HRUGIS_CLOUD_D_0018C0B0;
    if (kind == 1)
        return LVL_10_HRUGIS_CLOUD_D_0018B134;
    if (kind == 6)
        return LVL_10_HRUGIS_CLOUD_D_0018B040;
    return 0;
}

/* Find a mapped record by its unsigned halfword class and return its opaque word. */
typedef struct {
    unsigned int marker;
    unsigned char reserved04[0x38];
    unsigned short class_code;
    unsigned char reserved3E[0xa2];
} MappedClassEntry;
typedef char MappedClassStride[(sizeof(MappedClassEntry) == 0xe0) ? 1 : -1];
extern unsigned char LVL_10_HRUGIS_CLOUD_D_00139568[];
extern MappedClassEntry LVL_10_HRUGIS_CLOUD_D_0027AE70[];
unsigned int LVL_10_HRUGIS_CLOUD_FUN_0030E4E8(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_10_HRUGIS_CLOUD_D_0027AE70[LVL_10_HRUGIS_CLOUD_D_00139568[i]];
        if (record->class_code == key)
            return record->marker;
    }
    return 0;
}

/* Relocates serialized header offsets and compacts each row in its original storage. */
typedef unsigned long long NativeCompactU64;
typedef union {
    s32 field00[4];
    struct {
        NativeCompactU64 field00;
        short field08;
        short field0A;
        unsigned short field0C;
        unsigned short field0E;
    } compact;
} NativeCompactRow16;
typedef struct {
    u8 field00[6];
    short field06;
    u8 field08[4];
    f32 field0C;
    u32 field10;
    u32 field14;
    u32 field18;
    u32 field1C;
    u32 field20;
} NativeCompactHeader;
extern s32 LVL_10_HRUGIS_CLOUD_FUN_00308870(s32 value);
NativeCompactHeader *LVL_10_HRUGIS_CLOUD_FUN_00397018(NativeCompactHeader *header) {
    u32 base = (u32)header;
    NativeCompactRow16 *source;
    s32 index;
    header->field10 += base;
    header->field14 += base;
    header->field18 += base;
    header->field1C += base;
    if (header->field20 != 0) header->field20 += base;
    index = 0;
    source = (NativeCompactRow16 *)header->field18;
    header->field0C *= 0.0032116016f;
    if (header->field06 > 0) {
        do {
            s32 first = source->field00[0];
            s32 second = source->field00[1];
            s32 third = source->field00[2];
            s32 fourth = source->field00[3];
            ((NativeCompactRow16 *)header->field18)[index].compact.field0A = first >> 4;
            source++;
            ((NativeCompactRow16 *)header->field18)[index].compact.field08 = second >> 4;
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_10_HRUGIS_CLOUD_FUN_00308870(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_10_HRUGIS_CLOUD_FUN_00308870(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_10_HRUGIS_CLOUD_D_001A79F0;
s32 LVL_10_HRUGIS_CLOUD_FUN_002F1460(void) {
    s32 found = 0;
    if (LVL_10_HRUGIS_CLOUD_D_001A79F0 == 25 || LVL_10_HRUGIS_CLOUD_D_001A79F0 == 5 ||
        LVL_10_HRUGIS_CLOUD_D_001A79F0 == 10 || LVL_10_HRUGIS_CLOUD_D_001A79F0 == 15) {
        found = 1;
    }
    return found;
}

typedef struct { u8 prefix[0x34]; unsigned short flags; } FlagPairObjectView;
typedef struct {
    u8 prefix[0x1860];
    FlagPairObjectView *secondary;
    u8 between[0xa2c];
    FlagPairObjectView *primary;
} FlagPairResidentView;
typedef char FlagPairResidentSize[(sizeof(FlagPairResidentView) == 0x2294) ? 1 : -1];
void LVL_10_HRUGIS_CLOUD_FUN_002EDF50(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_10_HRUGIS_CLOUD_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_10_HRUGIS_CLOUD_FUN_002EDF88(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_10_HRUGIS_CLOUD_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags &= ~1;
    secondary = root->secondary;
    if (secondary) secondary->flags &= ~1;
}

typedef struct {
    u8 field0;
    u8 active;
    u8 middle[4];
    unsigned short count;
    u8 trailing[8];
} ConditionalResetSlot;
typedef char ConditionalResetSlotSize[(sizeof(ConditionalResetSlot) == 16) ? 1 : -1];
extern ConditionalResetSlot LVL_10_HRUGIS_CLOUD_D_001B94C0[8];
void LVL_10_HRUGIS_CLOUD_FUN_002F18A8(void) {
    ConditionalResetSlot *slot = LVL_10_HRUGIS_CLOUD_D_001B94C0;
    s32 remaining = 7;
    do {
        if (slot->active) {
            slot->active = 0;
            slot->count = 0;
        }
        --remaining;
        ++slot;
    } while (remaining >= 0);
}

typedef struct {
    u8 prefix[0x40];
    s32 mode;
    u8 between[0x14];
    s32 state;
} StateTransitionView;
typedef char StateTransitionViewSize[(sizeof(StateTransitionView) == 0x5c) ? 1 : -1];
extern StateTransitionView LVL_10_HRUGIS_CLOUD_D_001BF7C0;
void LVL_10_HRUGIS_CLOUD_FUN_0030DE50(void) {
    if (LVL_10_HRUGIS_CLOUD_D_001BF7C0.mode == 7 && LVL_10_HRUGIS_CLOUD_D_001BF7C0.state == 1) {
        LVL_10_HRUGIS_CLOUD_D_001BF7C0.state = 2;
    }
}

/* Substitute the first percent selector in a record's localized text. */
typedef struct {
    unsigned char gap0[10];
    short text_id;
    short mapped_key;
    unsigned char gap0e[26];
} DobboFormatRow396;
typedef struct {
    unsigned char gap0[32];
    DobboFormatRow396 *rows;
} DobboFormatRoot396;
typedef struct {
    unsigned char gap0[0x80];
    int amount;
    unsigned char gap84[0x5c];
} DobboFormatMapped396;
typedef char DobboFormatRowStride396[(sizeof(DobboFormatRow396) == 40) ? 1 : -1];
typedef char DobboFormatMappedStride396[(sizeof(DobboFormatMapped396) == 0xe0) ? 1 : -1];
extern DobboFormatRoot396 LVL_10_HRUGIS_CLOUD_D_001C9C60;
extern const char LVL_10_HRUGIS_CLOUD_D_001A9AE0[];
extern const char LVL_10_HRUGIS_CLOUD_D_001A9AE8[];
extern const unsigned char *LVL_10_HRUGIS_CLOUD_FUN_0030F0C0(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_10_HRUGIS_CLOUD_FUN_00323A88(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_10_HRUGIS_CLOUD_FUN_0030F0C0(LVL_10_HRUGIS_CLOUD_D_001C9C60.rows[index].text_id);
    unsigned char *p = temporary;
    if (!source)
        return;
    while (*source && *source != '%')
        *output++ = *source++;
    if (!*source) {
        *output = *source;
        return;
    }
    ++source;
    if (*source == 'b') {
        int key = LVL_10_HRUGIS_CLOUD_D_001C9C60.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_10_HRUGIS_CLOUD_D_0027AE70[LVL_10_HRUGIS_CLOUD_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_10_HRUGIS_CLOUD_D_001A9AE0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_10_HRUGIS_CLOUD_D_001A9AE8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_10_HRUGIS_CLOUD_FUN_002CE718(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_10_HRUGIS_CLOUD_D_00189E20;
    if (value < root->plane) {
        if (root->plane - value <= root->depth)
            return 1;
    }
    return 0;
}

typedef struct { int status; u8 reserved[36]; } GornStatusRecord156;
typedef char GornStatusRecord156Size[(sizeof(GornStatusRecord156) == 40) ? 1 : -1];
extern GornStatusRecord156 LVL_10_HRUGIS_CLOUD_D_002A3C70[];

int LVL_10_HRUGIS_CLOUD_FUN_0037F510(void)
{
    if (LVL_10_HRUGIS_CLOUD_D_002A3C70[2].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[3].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[4].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[5].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[8].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[9].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[10].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[11].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[12].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[13].status != 0 ||
        LVL_10_HRUGIS_CLOUD_D_002A3C70[14].status != 0)
        return 1;
    return 0;
}

/* Update the observed two-axis selection fields and their combined index. */
typedef struct {
    unsigned char gap0[0x43c];
    int column, row, index, mode;
} DobboGridState312;
extern int LVL_10_HRUGIS_CLOUD_FUN_00373870(int, unsigned int, void *);
void LVL_10_HRUGIS_CLOUD_FUN_00456D48(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_10_HRUGIS_CLOUD_FUN_00373870(3, 0, 0);
        --state->row;
        if (state->row < 0) {
            if (state->column == 0) {
                state->mode = 2;
                state->row = 1;
                state->column = 3;
            } else if (state->column == 1) {
                state->row = state->column;
            }
        }
    } else if (buttons & 0x4000) {
        int row;
        LVL_10_HRUGIS_CLOUD_FUN_00373870(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_10_HRUGIS_CLOUD_FUN_00373870(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_10_HRUGIS_CLOUD_FUN_00373870(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_10_HRUGIS_CLOUD_D_001B2480[16];
extern u32 LVL_10_HRUGIS_CLOUD_D_001B24C0[16];

int LVL_10_HRUGIS_CLOUD_FUN_0032BE30(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_10_HRUGIS_CLOUD_D_001B2480[index] == 0 ||
            LVL_10_HRUGIS_CLOUD_D_001B2480[index] == object) {
            LVL_10_HRUGIS_CLOUD_D_001B2480[index] = object;
            LVL_10_HRUGIS_CLOUD_D_001B24C0[index] = 0;
            return index;
        }
    }
    return -1;
}

/* Append an observed point index and update the original geometric descriptor. */
typedef struct {
    unsigned char gap0[0x10];
    float plane[4];
    unsigned char gap20[0x10];
    float (*points)[4];
    unsigned char gap34[0x25];
    unsigned char indices[3];
    unsigned char count;
} OozlaAppendDescriptor164;
typedef struct {
    unsigned char gap0[0x68];
    OozlaAppendDescriptor164 *descriptor;
    unsigned char gap6c[0x54];
    float transform[3][4];
} OozlaAppendObject164;
typedef char OozlaAppendDescriptorCount164[((int)&((OozlaAppendDescriptor164 *)0)->count == 0x5c) ? 1 : -1];
extern void LVL_10_HRUGIS_CLOUD_FUN_002F7768(OozlaAppendObject164 *, int, const float *);
extern void LVL_10_HRUGIS_CLOUD_FUN_00308B60(float *, const float *, const float *);
extern void LVL_10_HRUGIS_CLOUD_FUN_00308FF0(float *, const float *, const float *);
extern void LVL_10_HRUGIS_CLOUD_FUN_002F7AB0(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_10_HRUGIS_CLOUD_FUN_002F76C0(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_10_HRUGIS_CLOUD_FUN_002F7768(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_10_HRUGIS_CLOUD_FUN_00308B60(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_10_HRUGIS_CLOUD_FUN_00308FF0(difference, difference, &object->transform[0][0]);
        LVL_10_HRUGIS_CLOUD_FUN_002F7AB0(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_10_HRUGIS_CLOUD_FUN_0045C1C0(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_10_HRUGIS_CLOUD_FUN_0045C658(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

typedef struct {
    u8 pad0000[0x2294];
    u32 kind;
    u32 unknown2298;
    u32 mode;
} HrugisResidentFlags2294;

s32 LVL_10_HRUGIS_CLOUD_FUN_002C8D00(void)
{
    HrugisResidentFlags2294 *root = (HrugisResidentFlags2294 *)LVL_10_HRUGIS_CLOUD_D_00189E20;

    if (root->mode == 17 || root->mode == 18
        || root->kind == 0x67 || root->kind == 0x7f
        || root->kind == 0x73 || root->kind == 0x72) {
        return 1;
    }
    return 0;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_10_HRUGIS_CLOUD_FUN_00320388(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_10_HRUGIS_CLOUD_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_10_HRUGIS_CLOUD_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_10_HRUGIS_CLOUD_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_10_HRUGIS_CLOUD_FUN_00341C20(void) {
    if (((CallState *)LVL_10_HRUGIS_CLOUD_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_10_HRUGIS_CLOUD_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_10_HRUGIS_CLOUD_D_001A63A8)->active); ((CallState *)LVL_10_HRUGIS_CLOUD_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_10_HRUGIS_CLOUD_FUN_00335160(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

/* Prototype775 is a research label. Field names below describe only the
 * independently observed byte23 and binary32 word2C operations. */
typedef struct {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[8];
    f32 field2C;
} NativeUpdate775View;

extern void LVL_10_HRUGIS_CLOUD_FUN_0032AF78(NativeUpdate775View *object);

void LVL_10_HRUGIS_CLOUD_FUN_003B6768(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_10_HRUGIS_CLOUD_FUN_0032AF78(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_10_HRUGIS_CLOUD_FUN_0033FD48(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_10_HRUGIS_CLOUD_FUN_00339818(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_10_HRUGIS_CLOUD_FUN_002C1E70(void)
{
}


unsigned int LVL_10_HRUGIS_CLOUD_FUN_002F15B0(void)
{
    return 0;
}


void LVL_10_HRUGIS_CLOUD_FUN_00307E78(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0030EE40(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0030FC08(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_003170C8(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0031B0E8(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00323E40(void)
{
}


unsigned int LVL_10_HRUGIS_CLOUD_FUN_00386350(void)
{
    return 0;
}


void LVL_10_HRUGIS_CLOUD_FUN_0038AE10(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00391E28(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00395768(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00398E48(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_003FD2D8(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0042B4F8(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00449980(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0044B3F0(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0044B640(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_0044BB38(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_004543B0(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00455008(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00460E10(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_004631A0(void)
{
}


void LVL_10_HRUGIS_CLOUD_FUN_00464A38(void)
{
}
void LVL_10_HRUGIS_CLOUD_FUN_0039A690(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003CA3A8(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003D50A0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003DF4B8(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003E6360(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003E9908(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003EFBE8(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003F8CE0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003FA088(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_00420298(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_00432A88(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}

extern u8 LVL_10_HRUGIS_CLOUD_F62e6ff2b_D_00189E20[];
extern u8 LVL_10_HRUGIS_CLOUD_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_10_HRUGIS_CLOUD_FUN_00373468(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_10_HRUGIS_CLOUD_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_10_HRUGIS_CLOUD_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_10_HRUGIS_CLOUD_F62e6ff2b_D_00188660;
        if (record[116] != 0) {
            cursor = record + 116;
            do {
                id++;
                if (id >= limit)
                    break;
                cursor += 112;
            } while (*cursor != 0);
        }
    }
    return (id != limit) ? id : 52;
}
void LVL_10_HRUGIS_CLOUD_FUN_003BA5D0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 88) = 0;
    *(float *)(p + 96) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(int *)(p + 84) = 0;
    *(float *)(p + 92) = 1.0f;
    *(int *)(p + 100) = 0;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003F2748(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 88) = 0;
    *(float *)(p + 96) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(int *)(p + 84) = 0;
    *(float *)(p + 92) = 1.0f;
    *(int *)(p + 100) = 0;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
typedef struct { int v[12]; } Blob;
extern Blob LVL_10_HRUGIS_CLOUD_F6894d7c1_D_001A8E60;
int LVL_10_HRUGIS_CLOUD_FUN_0030E118(int x)
{
    Blob b;
    int i;
    b = LVL_10_HRUGIS_CLOUD_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
void LVL_10_HRUGIS_CLOUD_FUN_003A1068(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003A36F0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_00435298(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 8) = 0;
    *(float *)(p + 24) = 2.0f;
    *(int *)(p + 40) = 0;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
typedef struct { short key; short val; } Entry;

extern Entry *LVL_10_HRUGIS_CLOUD_Fc68ad20a_D_0018C2B8;

int LVL_10_HRUGIS_CLOUD_FUN_002ECCF8(int key, int *out)
{
    Entry *e = LVL_10_HRUGIS_CLOUD_Fc68ad20a_D_0018C2B8;
    Entry *p;

    if (e == 0)
        return 0;
    if (e->key == -1)
        goto notfound;
    p = e;
    for (;;) {
        if (key == p->key) {
            *out = p->val;
            return 1;
        }
        p++;
        if (p->key == -1)
            goto notfound;
    }
notfound:
    *out = 0;
    return 0;
}
typedef struct {
    int f0;
    short f4;
    unsigned char f6;
    char pad[0x18 - 7];
    int f18;
    int f1C;
} Blk;

extern Blk LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8;
extern short LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63AC;
extern int LVL_10_HRUGIS_CLOUD_F55a1acb8_FUN_00133688(void);
extern void LVL_10_HRUGIS_CLOUD_F55a1acb8_FUN_0011AEA0(int);

void LVL_10_HRUGIS_CLOUD_FUN_00342C80(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_10_HRUGIS_CLOUD_F55a1acb8_FUN_00133688()) {
        LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_10_HRUGIS_CLOUD_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f6;
    q = LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f18;
    LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f1C;
        LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_10_HRUGIS_CLOUD_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
void LVL_10_HRUGIS_CLOUD_FUN_003E5E90(void)
{
    int *p;
    int i;

    p = *(int **)0x1B32A0 + 60;
    for (i = 3; i >= 0; i--) { *p = 0; p -= 16; }

    p = *(int **)0x1B32A4 + 4799;
    for (i = 149; i >= 0; i--) { *p = 0; p -= 32; }

    p = *(int **)0x1B32A8;
    for (i = 9; i >= 0; i--) { p[1] = 0; p[0] = 0; p += 4; }
}
extern char LVL_10_HRUGIS_CLOUD_Fa2dbe766_D_00189E20[];

int LVL_10_HRUGIS_CLOUD_FUN_003D9350(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_10_HRUGIS_CLOUD_Fa2dbe766_D_00189E20;
    if (*(short *)(*(int *)(b + 8848) + 170) != 0)
        goto fail;
    if (*(unsigned char *)(b + 8884) != 0)
        goto fail;
    if (*(int *)(b + 8852) == 49)
        goto fail;
    if (*(int *)(b + 8860) == 20)
        goto fail;
    if (*(int *)(b + 9420) > 0)
        goto ok;
fail:
    return 0;
ok:
    return 1;
}
void LVL_10_HRUGIS_CLOUD_FUN_00410998(char *object)
{
    char *o = object;
    int *q = *(int **)(o + 104);

    if (*(unsigned char *)(o + 32))
        return;

    q[2] = 64;
    q[4] = 50;
    q[0] = 0;
    q[3] = -1;
    q[5] = 0;
    *(unsigned char *)(o + 32) = 1;
    q[6] = -1;

    *(float *)(o + 44) = *(float *)(*(int *)(o + 36) + 36) * 0.8f;
}
extern char LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20[];

void LVL_10_HRUGIS_CLOUD_FUN_002EDDE8(void)
{
    char *b = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
    char *p;
    int i;

    p = *(char **)(b + 3096);
    *(unsigned short *)(p + 52) &= 0xFFFE;

    for (i = 0; i < 7; i++) {
        p = *(char **)(b + 4640 + i * 80);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
        p = *(char **)(b + 4644 + i * 80);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
        p = *(char **)(b + 4648 + i * 80);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_10_HRUGIS_CLOUD_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_10_HRUGIS_CLOUD_FUN_0037F5B0(char *p)
{
    *(long long *)(p + 112) = 0;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_003F2E20(char *p)
{
    *(long long *)(p + 112) = 0;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void LVL_10_HRUGIS_CLOUD_FUN_00435230(char *p)
{
    *(long long *)(p + 112) = 0;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
extern char LVL_10_HRUGIS_CLOUD_F0be97c76_D_00189E20[];

void LVL_10_HRUGIS_CLOUD_FUN_002C1DF0(void)
{
    char *base = LVL_10_HRUGIS_CLOUD_F0be97c76_D_00189E20;
    int i, j;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 3; j++) {
            char *p = *(char **)(base + 4640 + i * 80);

            if (j == 1)
                p = *(char **)(base + 4644 + i * 80);
            else if (j == 2)
                p = *(char **)(base + 4648 + i * 80);
            if (p != 0)
                *(long long *)(p + 56) = *(long long *)(*(char **)(base + 8848) + 56);
        }
    }
}

struct Slot { s32 w; s32 rest[4]; };
struct Table1 { char pad[19264]; struct Slot slots[48]; };
struct Table2 { char pad[52]; s32 slots[4]; };

extern struct Table1 LVL_10_HRUGIS_CLOUD_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_10_HRUGIS_CLOUD_Fee2b87d1_D_00152CD0;

s32 LVL_10_HRUGIS_CLOUD_FUN_0031FAC8(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_10_HRUGIS_CLOUD_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_10_HRUGIS_CLOUD_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_10_HRUGIS_CLOUD_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_10_HRUGIS_CLOUD_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* Paired-strip packet emitter, 396 bytes, placed in levels/10_hrugis_cloud and
   levels/5_feltzin_system.  The body writes a 16-byte GIF header into the
   resident packet cursor (a small-data global), advances the cursor, then
   writes the tag words and two packed 64-bit strip descriptors.  The retail
   loads the cursor with LUI/LO and advances it through $gp, so the unit is
   compiled under the qualified small-data profile (-O2 -G8). */
extern int *LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001A7350 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001A7354 __attribute__((sda));

void LVL_10_HRUGIS_CLOUD_FUN_00315E00(int a0, int a1, int a2, int a3, int p4, int p5)
{
    long long *q;

    *(int *)((char *)LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 + 0) = 0x10000003;
    *(int *)((char *)LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 + 4) = 0;
    *(int *)((char *)LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 + 8) = 0;
    *(int *)((char *)LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 + 12) = 0x50000003;
    q = (long long *)LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88;
    LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 = (int *)((char *)q + 16);
    q[2] = 0x4400000000008001LL;
    q[3] = 17424;
    q[4] = 65;
    q[5] = p4;
#define LO_D0 (*(int *)((char *)&LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001A7350 + 0))
#define HI_D4 (*(int *)((char *)&LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001A7354 + 0))

    if (p5 != 0) {
        q[6] = (a0 + LO_D0 - 8)
             | ((long long)(a1 + HI_D4 - 8) << 16)
             | 0xFFFFF000000000LL;
        q[7] = (a2 + LO_D0 - 8)
             | ((long long)(a3 + HI_D4 - 8) << 16)
             | 0xFFFFF000000000LL;
    } else {
        q[6] = ((a0 << 4) + LO_D0 - 16)
             | ((long long)((a1 << 4) + HI_D4 - 16) << 16)
             | 0xFFFFF000000000LL;
        q[7] = ((a2 << 4) + LO_D0 - 16)
             | ((long long)((a3 << 4) + HI_D4 - 16) << 16)
             | 0xFFFFF000000000LL;
    }
    LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 = (int *)((char *)LVL_10_HRUGIS_CLOUD_Fa6a1a427_D_001B2C88 + 48);
}
/* family 9bb235d69b795f0f - 372 bytes, 2 placements
   levels/10_hrugis_cloud @0x00316120, levels/5_feltzin_system @0x00313C08

   Builds one packet into the resident buffer: four-word header, advance the
   resident pointer by 16, three pinned 64-bit words plus the caller payload,
   then two packed 64-bit words selected by argument 7, then advance by 48.
   The | chain must be written unshifted-term first: that fixes both the
   accumulation register and the instruction interleaving (see SOLUTION.md).
*/

extern int *LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7350 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7354 __attribute__((sda));

void LVL_10_HRUGIS_CLOUD_FUN_00316120(int a1, int a2, int a3, int a4,
                                       long long a5, int a6, int a7)
{
    *(int *)LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 = 0x10000003;
    *(int *)((char *)LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 + 4) = 0;
    *(int *)((char *)LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 + 8) = 0;
    *(int *)((char *)LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 + 12) = 0x50000003;
    {
        char *p = (char *)LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88;

        LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 = (int *)(p + 16);
        *(long long *)(p + 16) = 0x4400000000008001LL;
        *(long long *)(p + 24) = 0x4410;
        *(long long *)(p + 32) = 0x46;
        *(long long *)(p + 40) = a5;
        if (a7 != 0) {
            *(long long *)(p + 48) = ((a1 + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7350) - 8)
                                   | ((long long)((a2 + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7354) - 8) << 16)
                                   | ((long long)a6 << 32);
            *(long long *)(p + 56) = ((a3 + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7350) - 8)
                                   | ((long long)((a4 + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7354) - 8) << 16)
                                   | ((long long)a6 << 32);
        } else {
            *(long long *)(p + 48) = (((a1 << 4) + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7350) - 16)
                                   | ((long long)(((a2 << 4) + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7354) - 16) << 16)
                                   | ((long long)a6 << 32);
            *(long long *)(p + 56) = (((a3 << 4) + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7350) - 16)
                                   | ((long long)(((a4 << 4) + LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001A7354) - 16) << 16)
                                   | ((long long)a6 << 32);
        }
        LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 = (int *)((char *)LVL_10_HRUGIS_CLOUD_F9bb235d6_D_001B2C88 + 48);
    }
}
extern int *LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2C88 __attribute__((sda));
extern int *LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2AA0 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F060b9f20_D_001A72D0 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F060b9f20_D_001A72D4 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B1FBC __attribute__((sda));

typedef struct { int *saved; } SavedSlot;
extern SavedSlot LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2AC0 __attribute__((sda));

typedef struct {
    char gap0[12];
    short count;
    char gap14[2];
    long long *table;
} ResidentTable;

void LVL_10_HRUGIS_CLOUD_FUN_00371348(void)
{
    ResidentTable *resident = (ResidentTable *)LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2AA0;
    int *current = LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2C88;
    int index;

    LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2AC0.saved = current;
    LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B2C88 = (int *)((char *)current + 16);
    LVL_10_HRUGIS_CLOUD_F060b9f20_D_001A72D0 = LVL_10_HRUGIS_CLOUD_F060b9f20_D_001A72D4;
    LVL_10_HRUGIS_CLOUD_F060b9f20_D_001B1FBC = 0;
    for (index = 0; index < resident->count; ++index)
        *(long long *)((char *)resident->table + index * 16) = 0;
}
extern unsigned char *LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2520 __attribute__((sda));
extern unsigned char *LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2524 __attribute__((sda));
extern unsigned int LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2048 __attribute__((sda));
extern unsigned int LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2440 __attribute__((sda));
void LVL_10_HRUGIS_CLOUD_FUN_0032AB80(void)
{
    unsigned char *p = LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2520;
    int seen = 0;
    unsigned int limit;
    LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2440 = 0;
    while (p < LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2524) {
        limit = LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2048;
        do {
            if ((p[32] >= 254 && *(unsigned int *)(p + 160) <= limit) || seen) {
                LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2440 = LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2440 + 1;
                seen = (p[32] == 0xff) ? 1 : seen;
            }
            p += 256;
        } while (p < LVL_10_HRUGIS_CLOUD_Fe7046bc9_D_001B2524);
    }
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_10_HRUGIS_CLOUD_F1157be91_D_001A63E8;
extern unsigned char LVL_10_HRUGIS_CLOUD_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_10_HRUGIS_CLOUD_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_10_HRUGIS_CLOUD_F1157be91_FUN_00133230(void);
extern int LVL_10_HRUGIS_CLOUD_F1157be91_FUN_00132028(void);

int LVL_10_HRUGIS_CLOUD_FUN_00342B08(int a0, int a1, int a2) {
    CdMode mode = LVL_10_HRUGIS_CLOUD_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_10_HRUGIS_CLOUD_F1157be91_D_001A7900[0];
    LVL_10_HRUGIS_CLOUD_F1157be91_D_001A7430[0] = 0;
    LVL_10_HRUGIS_CLOUD_F1157be91_D_001A7434 = 0;
    LVL_10_HRUGIS_CLOUD_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_10_HRUGIS_CLOUD_F1157be91_FUN_00133230();
    LVL_10_HRUGIS_CLOUD_F1157be91_FUN_00132028();
    return 1;
}
struct src_record {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned short s4;
    unsigned short s6;
    unsigned short s8;
    unsigned short s10;
    unsigned short s12;
    unsigned char e;
    unsigned char f;
};

struct dst_record {
    unsigned char b0;
    unsigned char b1;
    unsigned char b2;
    unsigned char b3;
    unsigned short s4;
    unsigned short s6;
    unsigned short s8;
    short s10;
    short s12;
    unsigned char e;
    unsigned char f;
};

extern struct dst_record LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[8] __attribute__((nosda));

int LVL_10_HRUGIS_CLOUD_FUN_002F16D0(struct src_record *src)
{
    int n = 0;
    int i;
    int j;

    while (n < 8 && src[n].b0 != 255)
        n++;

    j = 0;
    for (i = 0; i < 8 && j < n; i++) {
        if (LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].b0 == 0)
            j++;
    }

    if (j != n)
        return -1;

    for (j = 0; j < n; j++) {
        i = 0;
        while (i < 8 && LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].b0 != 0)
            i++;
        if (i < 8) {
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].b0 = src[j].b0;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].b1 = src[j].b1;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].b2 = src[j].b2;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].b3 = src[j].b3;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s4 = src[j].s4;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s6 = src[j].s6;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s8 = src[j].s8;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s10 = src[j].s10;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s12 = src[j].s12;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].e = src[j].e - src[j].f;
            LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].f = src[j].f;
            if (LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s10 + LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s12 == 0)
                LVL_10_HRUGIS_CLOUD_Fe5c553af_D_001B94C0[i].s10++;
        }
    }
    return i;
}
typedef struct {
    short a;
    short b;
} T2;

extern int LVL_10_HRUGIS_CLOUD_F28b88339_D_00220D60 __attribute__((nosda));
extern int *LVL_10_HRUGIS_CLOUD_F28b88339_D_0021F2E0[];
extern T2 LVL_10_HRUGIS_CLOUD_F28b88339_D_00220860[];

void LVL_10_HRUGIS_CLOUD_FUN_0036EC50(void)
{
    int *wp;
    int *ent;
    int i;
    int j;
    int k;
    int *arr;
    int *pe;
    int *base;
    int *e;

    for (wp = &LVL_10_HRUGIS_CLOUD_F28b88339_D_00220D60; *wp >= 0; wp++) {
        ent = LVL_10_HRUGIS_CLOUD_F28b88339_D_0021F2E0[*wp];
        for (i = 0; i < *(short *)((char *)ent + 40); i++) {
            arr = ent + 16;
            pe = *(int **)((char *)arr + (i << 3));
            base = pe + 4;
            e = (int *)((char *)base + (base[1] << 4)) + 4;
            for (j = 0; j < base[0]; j++) {
                T2 *s = &LVL_10_HRUGIS_CLOUD_F28b88339_D_00220860[((unsigned char *)e)[19]];
                if (s->a != 0)
                    e[12] = (e[12] & 0xFFFFC000) | s->a;
                if (s->b != 0)
                    e[8] = (e[8] & 0xFFFFC000) | s->b;
                e += 16;
            }
        }
    }
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20;
extern s32 LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_10_HRUGIS_CLOUD_FUN_002EE180(void) {
    s32 result = LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field348;
    if (LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A8FF0 != 0 && LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A8FF4 != 0 || LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field2294 == 110 && LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field2294 == 109 || LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field1497 != 0 && LVL_10_HRUGIS_CLOUD_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field2294 == 0 && LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_10_HRUGIS_CLOUD_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
/* family 695584ed7d3e53db - 368 bytes, 2 placements
   levels/10_hrugis_cloud @0x00315C90, levels/5_feltzin_system @0x00313778

   Two-pass packet emitter.  On the "first" path (p5 == 0) it writes a four-word
   header into the resident packet cursor (a small-data global), points s0 at the
   payload area and advances the cursor by 112 bytes; on the other path s0 is a
   local scratch buffer.  An eight-argument helper then fills the payload, and a
   second pass emits the tag plus payload word or delegates to two helpers.

   The cursor is addressed through $gp at the two delay-slot sites only, so the
   declaration carries the `sda` attribute and the unit is compiled -G0. */

extern int *LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 __attribute__((sda));

extern void LVL_10_HRUGIS_CLOUD_F695584ed_FUN_00126288(void *p, int a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern void LVL_10_HRUGIS_CLOUD_F695584ed_FUN_0011AEA0(int a0);
extern void LVL_10_HRUGIS_CLOUD_F695584ed_FUN_001265B0(void *a0, void *a1);

void LVL_10_HRUGIS_CLOUD_FUN_00315C90(int a0, int a1, int a2, int a3, int p4, int p5)
{
    int t5 = a3;
    int n = (1 << t5) >> 6;
    int stride = 1 << (t5 + p4 - 4);
    int *s0;
    char buf[96];

    if (n <= 0) {
        n = 1;
    }
    if (p5 == 0) {
        char *q;

        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 0) = 0x10000006;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 4) = 0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 8) = 0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 12) = 0x50000006;
        q = (char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88;
        s0 = (int *)(q + 16);
        LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 = (int *)(q + 112);
    } else {
        s0 = (int *)buf;
    }
    LVL_10_HRUGIS_CLOUD_F695584ed_FUN_00126288(s0, (short)a1, (short)n, (short)a2, 0, 0, (short)(1 << t5), (short)(1 << p4));
    if (p5 == 0) {
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 0) = 0x30000000 | stride;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 4) = a0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 8) = 0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 12) = 0x50000000 | stride;
        LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 = (int *)((char *)LVL_10_HRUGIS_CLOUD_F695584ed_D_001B2C88 + 16);
    } else {
        LVL_10_HRUGIS_CLOUD_F695584ed_FUN_0011AEA0(0);
        LVL_10_HRUGIS_CLOUD_F695584ed_FUN_001265B0(s0, (void *)a0);
    }
}

extern u8 LVL_10_HRUGIS_CLOUD_F63ee41be_D_001D2180[];

void LVL_10_HRUGIS_CLOUD_FUN_0032AFD0(u8 *param_1)
{
    if (param_1[66] != 255) {
        u8 *tbl = (u8 *)(*(volatile int *)(param_1 + 36) + 72);

        *(int *)(param_1 + 88) = *(int *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + (param_1[64] << 2) + 28);
        param_1[110] = *(u8 *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + 18);
        param_1[108] = *(u8 *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + 17);
    } else {
        param_1[108] = 255;
        param_1[110] = 0;
        *(int *)(param_1 + 88) = (int)(LVL_10_HRUGIS_CLOUD_F63ee41be_D_001D2180 + (param_1[64] << 11));
    }

    *(int *)(param_1 + 92) = *(int *)(*(int *)(*(volatile int *)(param_1 + 36) + (param_1[67] << 2) + 72) + (param_1[65] << 2) + 28);
}
extern char *LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 __attribute__((sda));
extern char LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001A6D40[];
extern char LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001A6E90[];

void LVL_10_HRUGIS_CLOUD_FUN_0030AF78(int a0)
{
    if (LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 != 0) {
        *(int *)LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 = 0x30000015;
        if (a0 == 0) {
            *(int *)(LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 + 4) = (int)LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001A6D40;
        } else {
            *(int *)(LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 + 4) = (int)LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001A6E90;
        }
        *(int *)(LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 + 8) = 0;
        *(int *)(LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 + 12) = 0x50000015;
        LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 = LVL_10_HRUGIS_CLOUD_F8aa1a3c4_D_001B2C88 + 16;
    }
}
void LVL_10_HRUGIS_CLOUD_FUN_0033A6C0(int param_1, short *param_2, short param_3)
{
    int i;

    param_1 = (param_1 - *(int *)0x001B251C) << 8 >> 16;
    for (i = 1; i <= param_2[0]; i++) {
        if (param_2[i] == param_1) return;
    }
    if (param_2[0] < param_3) {
        param_2[0] = param_2[0] + 1;
        param_2[param_2[0]] = param_1;
    }
}
/* Family e8702d836bd3adba (120 B, 2 placements): call every registered
 * callback with its paired argument, bounded by a resident count word.
 *
 * The three resident globals are reached with `lui`+`lw` / `lui`+`addiu`
 * pairs that the ASSEMBLER macro expands from a single RTL insn; this only
 * happens when the externs carry the `sda` attribute (SYMBOL_REF_FLAG), which
 * suppresses gcc's HIGH/LO_SUM address split.  Without it the compiler keeps
 * the `%hi` in an extra callee-saved register and the body is 3 words long.
 */
typedef void (*fn_t)(int);

extern int LVL_10_HRUGIS_CLOUD_Fe8702d83_D_001B1FF8 __attribute__((sda));   /* resident callback count */
extern fn_t LVL_10_HRUGIS_CLOUD_Fe8702d83_D_001B1FD8[] __attribute__((sda)); /* callback table        */
extern int LVL_10_HRUGIS_CLOUD_Fe8702d83_D_001B1FE8[] __attribute__((sda));  /* argument table        */

void LVL_10_HRUGIS_CLOUD_FUN_00301CB0(void) {
    int i;
    for (i = 0; i < LVL_10_HRUGIS_CLOUD_Fe8702d83_D_001B1FF8; i++) {
        LVL_10_HRUGIS_CLOUD_Fe8702d83_D_001B1FD8[i](LVL_10_HRUGIS_CLOUD_Fe8702d83_D_001B1FE8[i]);
    }
}
extern int LVL_10_HRUGIS_CLOUD_F82977136_D_001C9EE8[];

int LVL_10_HRUGIS_CLOUD_FUN_00320BC0(int arg)
{
    int *t = LVL_10_HRUGIS_CLOUD_F82977136_D_001C9EE8;
    int *u = t + 5;
    int i;

    for (i = 0; i < 5; i++) {
        int k = arg ? 4 - i : i;
        if (t[k] == 0)
            continue;
        if (u[k] != -1)
            continue;
        return k;
    }
    return -1;
}
/* Family 7a3a32da2b27da0d — 364 bytes, a block loop that builds two four-word
   GS packets per iteration and calls the measured SDK helper at 0x00126288
   between them.

   -G0 promotion (agent f02, 2026-10-07): the retail body reaches the resident
   write pointer at 0x001B2C88 ten times through the one-instruction small-data
   form (absolute `lui`+`lw` in ordinary flow, the single `$gp` form in the
   `jal` delay slot).  Under -G8 that model is free; under -G0 cc1 needs the
   measured `sda` attribute, otherwise it emits the explicit two-instruction
   pair and shifts register allocation.  Nothing else about the body changed. */

extern int *LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 __attribute__((sda));

extern void LVL_10_HRUGIS_CLOUD_F7a3a32da_FUN_00126288(int *p, int a, int b, int c, int d, int e, int f, int g);

void LVL_10_HRUGIS_CLOUD_FUN_00392808(char *dst, int src, int size)
{
    int count = (size + 16383) & -16384;
    int si = 0;
    int di = 0;
    int v;

    while (count > 0) {
        *(int *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 = 0x10000006;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 4) = 0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 8) = 0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 12) = 0x50000006;
        LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 = (int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 16);
        v = (src + si) << 8;
        v = v >> 16;
        LVL_10_HRUGIS_CLOUD_F7a3a32da_FUN_00126288(LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88, v, 1, 1, 0, 0, 64, 64);
        LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 = (int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 96);
        *(int *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 = 0x30000300;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 4) = (int)(dst + di);
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 8) = 0;
        *(int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 12) = 0x50000300;
        si += 16384;
        di += 12288;
        count -= 16384;
        LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 = (int *)((char *)LVL_10_HRUGIS_CLOUD_F7a3a32da_D_001B2C88 + 16);
    }
}
/* RAC2 family 3cde0d021bd56651 - 192 bytes, 2 placements.
 * Slot3cde0d02 allocator: find the first free of six 64-byte slots, fill it in and
 * link it at the head of the context's list.
 */

typedef struct Slot3cde0d02 {
    short          f00;    /* +0x00 */
    short          f02;    /* +0x02 */
    unsigned char  f04;    /* +0x04 */
    char           pad05[7];
    unsigned char *f0C;    /* +0x0C */
    int            f10;    /* +0x10 */
    int            f14;    /* +0x14 */
    int            f18;    /* +0x18 */
    int            f1C;    /* +0x1C */
    char           pad20[32];
} Slot3cde0d02;                    /* 64 bytes */

typedef struct Mid {
    char           pad00[0x1C];
    char          *f1C;    /* +0x1C */
} Mid;

typedef struct Ctx {
    char           pad00[0x24];
    Mid           *f24;    /* +0x24 */
    char           pad28[0x28];
    int            f50;    /* +0x50 */
} Ctx;

extern Slot3cde0d02 LVL_10_HRUGIS_CLOUD_F3cde0d02_D_001D2000[6];
extern unsigned char LVL_10_HRUGIS_CLOUD_F3cde0d02_D_001CA200[];

Slot3cde0d02 *LVL_10_HRUGIS_CLOUD_FUN_0032B6B0(Ctx *ctx, int index)
{
    int i;
    Slot3cde0d02 *slot;
    unsigned char *p;

    for (i = 0; i < 6; i++) {
        if (LVL_10_HRUGIS_CLOUD_F3cde0d02_D_001D2000[i].f04 == 0) {
            break;
        }
    }
    slot = &LVL_10_HRUGIS_CLOUD_F3cde0d02_D_001D2000[i];
    slot->f04 = 1;
    slot->f00 = index;
    slot->f10 = (int)(LVL_10_HRUGIS_CLOUD_F3cde0d02_D_001CA200 + i * 5376);
    slot->f14 = (int)ctx->f24;
    p = (unsigned char *)*(unsigned int *)(ctx->f24->f1C + (short)index * 4 + 4);
    slot->f02 = p[2];
    slot->f0C = p + (p[0] + 4);
    slot->f1C = ctx->f50;
    ctx->f50 = (int)slot;
    return slot;
}
struct Rec31cd0302 {
    int v[10];
};

extern struct Rec31cd0302 LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[];

void LVL_10_HRUGIS_CLOUD_FUN_002FD858(int p0, int p1, int p2, int p3, int p4, int p5,
                                      int p6, int p7, int p8, int p9, unsigned idx)
{
    if (idx >= 32u)
        return;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[0] = p0;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[1] = p1;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[2] = p2;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[3] = p3;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[4] = p4;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[5] = p5;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[6] = p6;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[7] = p7;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[8] = p8;
    LVL_10_HRUGIS_CLOUD_F31cd0302_D_001BCE48[idx].v[9] = p9;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_10_HRUGIS_CLOUD_Fabf21065e887d7a9_AT003299F0_ROLE00;

int LVL_10_HRUGIS_CLOUD_FUN_003299F0(void)
{
    if ((LVL_10_HRUGIS_CLOUD_Fabf21065e887d7a9_AT003299F0_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_10_HRUGIS_CLOUD_Fabf21065e887d7a9_AT003299F0_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_10_HRUGIS_CLOUD_Fabf21065e887d7a9_AT003299F0_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_10_HRUGIS_CLOUD_Fabf21065e887d7a9_AT003299F0_ROLE00.flags_98 & 0x04000000u) != 0)
        return 1;
    return 0;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner {
    unsigned char prefix_00[0x120];
    void *slots_120[10];
    int count_148;
    int active_14c;
} Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner;


extern void LVL_10_HRUGIS_CLOUD_F5b4b17178a13f443_AT0033CC10_ROLE00(void *object);

void LVL_10_HRUGIS_CLOUD_FUN_0033CC10(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_10_HRUGIS_CLOUD_F5b4b17178a13f443_AT0033CC10_ROLE00(owner->slots_120[index]);
                owner->slots_120[index] = 0;
            }
        }
        owner->active_14c = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_acdcf1600d770d3b_ScalarConfigure116State {
    unsigned char prefix_00[0x60];
    unsigned int value_60;
    int value_64;
    unsigned char selector_68;
    unsigned char axis_69;
    unsigned char mode_6a;
    unsigned char dirty_6b;
} Rac2Native_acdcf1600d770d3b_ScalarConfigure116State;


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_10_HRUGIS_CLOUD_Facdcf1600d770d3b_AT00373BA8_ROLE00[];

void LVL_10_HRUGIS_CLOUD_FUN_00373BA8(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_10_HRUGIS_CLOUD_Facdcf1600d770d3b_AT00373BA8_ROLE00;
    state->value_60 = value;
    if (state->selector_68 != selector) {
        state->dirty_6b |= 1;
        state->selector_68 = selector;
    }
    if (state->value_64 != other) {
        state->dirty_6b |= 4;
        state->value_64 = other;
    }
    if (state->axis_69 != axis || state->mode_6a != mode) {
        state->dirty_6b |= 2;
        state->axis_69 = axis;
        state->mode_6a = mode;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_b1b523716b470b36_FamilyNative192Vector {
    float x;
    float y;
    float z;
    float w;
} Rac2Native_b1b523716b470b36_FamilyNative192Vector;


extern void LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A01C8_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A01C8_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_10_HRUGIS_CLOUD_FUN_003A01C8(void *object)
{
    Rac2Native_b1b523716b470b36_FamilyNative192Vector input;
    Rac2Native_b1b523716b470b36_FamilyNative192Vector output;
    char *base = (char *)object;
    char *state;
    char *source;
    char *kind;
    char *extra;

    input.w = 0.0f;
    state = *(char **)(base + 0x68);
    input.x = *(float *)(state + 0x74);
    input.y = *(float *)(state + 0x78);
    input.z = *(float *)(state + 0x7c);
    source = *(char **)(state + 0x70);
    LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A01C8_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A01C8_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10));
    source = *(char **)(state + 0x70);
    *(float *)(state + 0x74) = *(float *)(source + 0x10);
    *(float *)(state + 0x78) = *(float *)(source + 0x14);
    *(float *)(state + 0x7c) = *(float *)(source + 0x18);
    kind = *(char **)(source + 0x24);
    if (*(short *)(kind + 0x46) == 0x12) {
        extra = *(char **)(source + 0x68);
        if (*(unsigned char *)(extra + 0x14) == 0) {
            *(unsigned char *)(base + 0x20) = 7;
        }
    }
}


extern void LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A2C70_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A2C70_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_10_HRUGIS_CLOUD_FUN_003A2C70(void *object)
{
    Rac2Native_b1b523716b470b36_FamilyNative192Vector input;
    Rac2Native_b1b523716b470b36_FamilyNative192Vector output;
    char *base = (char *)object;
    char *state;
    char *source;
    char *kind;
    char *extra;

    input.w = 0.0f;
    state = *(char **)(base + 0x68);
    input.x = *(float *)(state + 0x74);
    input.y = *(float *)(state + 0x78);
    input.z = *(float *)(state + 0x7c);
    source = *(char **)(state + 0x70);
    LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A2C70_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_10_HRUGIS_CLOUD_Fb1b523716b470b36_AT003A2C70_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10));
    source = *(char **)(state + 0x70);
    *(float *)(state + 0x74) = *(float *)(source + 0x10);
    *(float *)(state + 0x78) = *(float *)(source + 0x14);
    *(float *)(state + 0x7c) = *(float *)(source + 0x18);
    kind = *(char **)(source + 0x24);
    if (*(short *)(kind + 0x46) == 0x12) {
        extra = *(char **)(source + 0x68);
        if (*(unsigned char *)(extra + 0x14) == 0) {
            *(unsigned char *)(base + 0x20) = 7;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner;

typedef struct Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext {
    unsigned char prefix_00[0x10];
    float vector_10[4];
    unsigned int field_20;
    unsigned int active_24;
} Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext;


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003B0DC8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_003B0DC8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003B0DC8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003B58A8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_003B58A8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003B58A8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_10_HRUGIS_CLOUD_F86f665335d9cb905_AT003D16A0_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_10_HRUGIS_CLOUD_FUN_003D16A0(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_10_HRUGIS_CLOUD_F86f665335d9cb905_AT003D16A0_ROLE00(owner, owner->context_68);
}


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003D6C18_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_003D6C18(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003D6C18_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003DC290_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_003DC290(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003DC290_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003F3248_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_003F3248(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003F3248_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003F6BA8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_003F6BA8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT003F6BA8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT00405A08_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_10_HRUGIS_CLOUD_FUN_00405A08(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_10_HRUGIS_CLOUD_F9cdc323a4d0c2fbd_AT00405A08_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_6af85cabb56d3b41_SmallConditional52Owner {
    unsigned char unknown_00[0x7d];
    unsigned char value_7d;
    short value_7e;
    unsigned char unknown_80[6];
    short identity_86;
} Rac2Native_6af85cabb56d3b41_SmallConditional52Owner;

typedef struct Rac2Native_6af85cabb56d3b41_SmallConditional52Global {
    unsigned char unknown_00[0x2294];
    int mode_2294;
    unsigned char unknown_2298[0x208];
    int identity_24a0;
} Rac2Native_6af85cabb56d3b41_SmallConditional52Global;


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_10_HRUGIS_CLOUD_F6af85cabb56d3b41_AT00443320_ROLE00;

void LVL_10_HRUGIS_CLOUD_FUN_00443320(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_10_HRUGIS_CLOUD_F6af85cabb56d3b41_AT00443320_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_10_HRUGIS_CLOUD_F6af85cabb56d3b41_AT00443320_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_10_HRUGIS_CLOUD_FUN_004486F0(float factor, void *context,
                                   float *destination,
                                   const float *first, const float *second)
{
    float complement = 1.0f - factor;
    destination[0] = complement * first[0] + factor * second[0];
    destination[1] = complement * first[1] + factor * second[1];
    destination[2] = complement * first[2] + factor * second[2];
    destination[3] = complement * first[3] + factor * second[3];
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout {
    unsigned char unknown_00[8];
    float first_0;
    float second_0;
    float first_1;
    float second_1;
    float first_2;
    float second_2;
    float first_3;
    float second_3;
} Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout;


void LVL_10_HRUGIS_CLOUD_FUN_004488A0(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_10_HRUGIS_CLOUD_FUN_0044DC00(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_10_HRUGIS_CLOUD_FUN_00455C28(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_79744baad5ad7f65_ScalarChange48Owner {
    unsigned int field_00;
    int value_04;
    unsigned char gap_08[0x3ec];
    int counter_3f4;
    int counter_3f8;
    int previous_3fc;
} Rac2Native_79744baad5ad7f65_ScalarChange48Owner;


extern unsigned char LVL_10_HRUGIS_CLOUD_F79744baad5ad7f65_AT0045D098_ROLE00[];

void LVL_10_HRUGIS_CLOUD_FUN_0045D098(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_10_HRUGIS_CLOUD_F79744baad5ad7f65_AT0045D098_ROLE00[0] == 0) {
        owner->previous_3fc = previous;
        owner->counter_3f8 = 180;
        owner->counter_3f4 = 300;
    }
    owner->value_04 = value;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef int Rac2Native_6b0741c38bf00fee_s32;

typedef unsigned char Rac2Native_6b0741c38bf00fee_u8;

typedef long Rac2Native_6b0741c38bf00fee_s64;

typedef unsigned long Rac2Native_6b0741c38bf00fee_u64;


extern Rac2Native_6b0741c38bf00fee_u8 LVL_10_HRUGIS_CLOUD_F6b0741c38bf00fee_AT002C0F08_ROLE00[];
extern void LVL_10_HRUGIS_CLOUD_F6b0741c38bf00fee_AT002C0F08_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_10_HRUGIS_CLOUD_F6b0741c38bf00fee_AT002C0F08_ROLE02(short, short, short);

void LVL_10_HRUGIS_CLOUD_FUN_002C0F08(void) {
 LVL_10_HRUGIS_CLOUD_F6b0741c38bf00fee_AT002C0F08_ROLE01(LVL_10_HRUGIS_CLOUD_F6b0741c38bf00fee_AT002C0F08_ROLE00[1],0,1,0x32);
 LVL_10_HRUGIS_CLOUD_F6b0741c38bf00fee_AT002C0F08_ROLE02(0x16,7,0);
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef int Rac2Native_28c929cadd7aca24_s32;

typedef unsigned char Rac2Native_28c929cadd7aca24_u8;

typedef struct {
    Rac2Native_28c929cadd7aca24_u8 prefix[0xc40];
    Rac2Native_28c929cadd7aca24_s32 selected;
    Rac2Native_28c929cadd7aca24_s32 index;
    Rac2Native_28c929cadd7aca24_u8 gap[0x1648];
    Rac2Native_28c929cadd7aca24_u8 *object;
} Rac2Native_28c929cadd7aca24_NativeResidentView;


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_10_HRUGIS_CLOUD_F28c929cadd7aca24_AT002ED1F0_ROLE00;

void LVL_10_HRUGIS_CLOUD_FUN_002ED1F0(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_10_HRUGIS_CLOUD_F28c929cadd7aca24_AT002ED1F0_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_10_HRUGIS_CLOUD_F28c929cadd7aca24_AT002ED1F0_ROLE00.selected = selected;
            LVL_10_HRUGIS_CLOUD_F28c929cadd7aca24_AT002ED1F0_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_10_HRUGIS_CLOUD_F5fc519c90e0e763a_AT002EE4F8_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_10_HRUGIS_CLOUD_FUN_002EE4F8(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_10_HRUGIS_CLOUD_F5fc519c90e0e763a_AT002EE4F8_ROLE00(-value);
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_74a5d71aacb394c6_u32;

typedef struct {
    Rac2Native_74a5d71aacb394c6_u32 first[64];
    Rac2Native_74a5d71aacb394c6_u32 second[64];
    unsigned char gap[0x20];
    int count;
    Rac2Native_74a5d71aacb394c6_u32 current;
} Rac2Native_74a5d71aacb394c6_ExtraIndexState48;


void LVL_10_HRUGIS_CLOUD_FUN_00396B30(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[139];

void LVL_10_HRUGIS_CLOUD_FUN_003A3618(void)
{
    int i;
    LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[138] = 5;
    LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[137] = 0;
    LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[i] = 0;
        LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_10_HRUGIS_CLOUD_F03c444112283bc5f_AT003A3618_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_10_HRUGIS_CLOUD_FUN_002BFC70(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_10_HRUGIS_CLOUD_FUN_002EED30(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_10_HRUGIS_CLOUD_FUN_002F7070(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_10_HRUGIS_CLOUD_FUN_002F7078(void) {
    return 1;
}


extern void LVL_10_HRUGIS_CLOUD_QWEN_11a4c157d102_AT0030EBC0_ROLE000(unsigned char *);

void LVL_10_HRUGIS_CLOUD_FUN_0030EBC0(void *owner)
{
    LVL_10_HRUGIS_CLOUD_QWEN_11a4c157d102_AT0030EBC0_ROLE000(owner);
}



void LVL_10_HRUGIS_CLOUD_QWEN_407ee6f17a73_AT0030EC60_ROLE001(int);
void LVL_10_HRUGIS_CLOUD_QWEN_407ee6f17a73_AT0030EC60_ROLE000(void*);

void LVL_10_HRUGIS_CLOUD_FUN_0030EC60(void *param)
{
  LVL_10_HRUGIS_CLOUD_QWEN_407ee6f17a73_AT0030EC60_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_10_HRUGIS_CLOUD_QWEN_407ee6f17a73_AT0030EC60_ROLE000(param);
}


void LVL_10_HRUGIS_CLOUD_QWEN_5696fcf76f0c_AT0032C640_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_10_HRUGIS_CLOUD_FUN_0032C640(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_5696fcf76f0c_AT0032C640_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


/*
 * Disassembled at 0x002cd220
 * Signature: int FUN_002cd220(void)
 * Behavior:
 *   - Saves RA on stack
 *   - Calls FUN_0027C540(0)
 *   - Calls FUN_0029C138()
 *   - Calls FUN_0027C660()
 *   - Restores RA from stack
 *   - Returns 0
 */
extern void LVL_10_HRUGIS_CLOUD_QWEN_523e38f49b74_AT0035A9E8_ROLE000(long);
extern void LVL_10_HRUGIS_CLOUD_QWEN_523e38f49b74_AT0035A9E8_ROLE002(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_523e38f49b74_AT0035A9E8_ROLE001(void);

int LVL_10_HRUGIS_CLOUD_FUN_0035A9E8(void)
{
  LVL_10_HRUGIS_CLOUD_QWEN_523e38f49b74_AT0035A9E8_ROLE000(0);
  LVL_10_HRUGIS_CLOUD_QWEN_523e38f49b74_AT0035A9E8_ROLE002();
  LVL_10_HRUGIS_CLOUD_QWEN_523e38f49b74_AT0035A9E8_ROLE001();
  return 0;
}


unsigned long long LVL_10_HRUGIS_CLOUD_FUN_0035ADE8(void);

extern void LVL_10_HRUGIS_CLOUD_QWEN_f47929f95774_AT0035ADE8_ROLE000(long);
extern void LVL_10_HRUGIS_CLOUD_QWEN_f47929f95774_AT0035ADE8_ROLE002(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_f47929f95774_AT0035ADE8_ROLE001(void);

unsigned long long LVL_10_HRUGIS_CLOUD_FUN_0035ADE8(void)
{
  LVL_10_HRUGIS_CLOUD_QWEN_f47929f95774_AT0035ADE8_ROLE000(0);
  LVL_10_HRUGIS_CLOUD_QWEN_f47929f95774_AT0035ADE8_ROLE002();
  LVL_10_HRUGIS_CLOUD_QWEN_f47929f95774_AT0035ADE8_ROLE001();
  return 0;
}


/* Function at 0x002cd8c0: FUN_002CD8C0
 * 
 * Disassembly summary:
 * - 12 instructions, 48 bytes total
 * - Prologue: save ra, allocate 16-byte stack frame
 * - Three calls to external functions:
 *   1. FUN_0027C540(0)
 *   2. FUN_0029C450()
 *   3. FUN_0027C660()
 * - Epilogue: restore ra, return 0, deallocate stack
 * - No local variables used
 * - Return type is void-like; returns zero in v0
 * - Delay slots are_NOP/unused in this case
 */

/* External function declarations (exact addresses from task metadata) */
extern void LVL_10_HRUGIS_CLOUD_QWEN_cb305b1f1210_AT0035B088_ROLE000(long);
extern void LVL_10_HRUGIS_CLOUD_QWEN_cb305b1f1210_AT0035B088_ROLE002(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_cb305b1f1210_AT0035B088_ROLE001(void);

long long LVL_10_HRUGIS_CLOUD_FUN_0035B088(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_cb305b1f1210_AT0035B088_ROLE000(0LL);
    LVL_10_HRUGIS_CLOUD_QWEN_cb305b1f1210_AT0035B088_ROLE002();
    LVL_10_HRUGIS_CLOUD_QWEN_cb305b1f1210_AT0035B088_ROLE001();
    return 0LL;
}


extern void LVL_10_HRUGIS_CLOUD_QWEN_c23b3406f982_AT00360480_ROLE000(long);
extern void LVL_10_HRUGIS_CLOUD_QWEN_c23b3406f982_AT00360480_ROLE002(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_c23b3406f982_AT00360480_ROLE001(void);

int LVL_10_HRUGIS_CLOUD_FUN_00360480(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_c23b3406f982_AT00360480_ROLE000(0);
    LVL_10_HRUGIS_CLOUD_QWEN_c23b3406f982_AT00360480_ROLE002();
    LVL_10_HRUGIS_CLOUD_QWEN_c23b3406f982_AT00360480_ROLE001();
    return 0;
}


void LVL_10_HRUGIS_CLOUD_QWEN_68cf9ebb5ee2_AT003605F0_ROLE000(long);
void LVL_10_HRUGIS_CLOUD_QWEN_68cf9ebb5ee2_AT003605F0_ROLE001(void);
void LVL_10_HRUGIS_CLOUD_QWEN_68cf9ebb5ee2_AT003605F0_ROLE002(void);

unsigned long long LVL_10_HRUGIS_CLOUD_FUN_003605F0(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_68cf9ebb5ee2_AT003605F0_ROLE000(0);
    LVL_10_HRUGIS_CLOUD_QWEN_68cf9ebb5ee2_AT003605F0_ROLE002();
    LVL_10_HRUGIS_CLOUD_QWEN_68cf9ebb5ee2_AT003605F0_ROLE001();
    return 0;
}



/* 
 * Function: FUN_002D2EE0
 * 
 * Disassembly summary:
 * - Saves return address on stack
 * - Calls FUN_0027C540(0) (with zero argument)
 * - Calls FUN_0029C7A0()
 * - Calls FUN_0027C660()
 * - Restores return address
 * - Returns 0
 *
 * Notes:
 * - The function follows standard MIPS ABI prologue/epilogue.
 * - Delay slots are filled with NOP or argument setup as seen in disassembly.
 * - No local variables used.
 * - All external function addresses come from the assigned externals list.
 */

extern void LVL_10_HRUGIS_CLOUD_QWEN_68f040eb20c9_AT003606A8_ROLE000(long arg0);
extern void LVL_10_HRUGIS_CLOUD_QWEN_68f040eb20c9_AT003606A8_ROLE002(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_68f040eb20c9_AT003606A8_ROLE001(void);

unsigned long long LVL_10_HRUGIS_CLOUD_FUN_003606A8(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_68f040eb20c9_AT003606A8_ROLE000(0);
    LVL_10_HRUGIS_CLOUD_QWEN_68f040eb20c9_AT003606A8_ROLE002();
    LVL_10_HRUGIS_CLOUD_QWEN_68f040eb20c9_AT003606A8_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_10_HRUGIS_CLOUD_QWEN_92ea7c2f4a5c_AT00360A58_ROLE000(long);
extern void LVL_10_HRUGIS_CLOUD_QWEN_92ea7c2f4a5c_AT00360A58_ROLE001(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_92ea7c2f4a5c_AT00360A58_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_10_HRUGIS_CLOUD_FUN_00360A58(void)
{
  LVL_10_HRUGIS_CLOUD_QWEN_92ea7c2f4a5c_AT00360A58_ROLE000(0);
  LVL_10_HRUGIS_CLOUD_QWEN_92ea7c2f4a5c_AT00360A58_ROLE002();
  LVL_10_HRUGIS_CLOUD_QWEN_92ea7c2f4a5c_AT00360A58_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_10_HRUGIS_CLOUD_QWEN_d01c568afacc_AT00360B58_ROLE000(long param_1);
extern void LVL_10_HRUGIS_CLOUD_QWEN_d01c568afacc_AT00360B58_ROLE001(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_d01c568afacc_AT00360B58_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_10_HRUGIS_CLOUD_FUN_00360B58(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_d01c568afacc_AT00360B58_ROLE000(0);
    LVL_10_HRUGIS_CLOUD_QWEN_d01c568afacc_AT00360B58_ROLE002();
    LVL_10_HRUGIS_CLOUD_QWEN_d01c568afacc_AT00360B58_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_10_HRUGIS_CLOUD_QWEN_093381cf82c3_AT00362D08_ROLE000(long);
extern void LVL_10_HRUGIS_CLOUD_QWEN_093381cf82c3_AT00362D08_ROLE002(void);
extern void LVL_10_HRUGIS_CLOUD_QWEN_093381cf82c3_AT00362D08_ROLE001(void);

long long LVL_10_HRUGIS_CLOUD_FUN_00362D08(void)
{
    LVL_10_HRUGIS_CLOUD_QWEN_093381cf82c3_AT00362D08_ROLE000(0);
    LVL_10_HRUGIS_CLOUD_QWEN_093381cf82c3_AT00362D08_ROLE002();
    LVL_10_HRUGIS_CLOUD_QWEN_093381cf82c3_AT00362D08_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_10_HRUGIS_CLOUD_FUN_0036C9D0(void) {
    return 1;
}


/* Measured partial C views; original types and object ownership are unknown. */
typedef unsigned char QwenRecovery_4ecc6b5a4034_u8;
typedef unsigned int QwenRecovery_4ecc6b5a4034_u32;

/* Minimum external views; neither declaration allocates original game storage. */
typedef struct {
    QwenRecovery_4ecc6b5a4034_u8 prefix[0x24];
    QwenRecovery_4ecc6b5a4034_u32 word;
    QwenRecovery_4ecc6b5a4034_u8 tail[8];
} QwenRecovery_4ecc6b5a4034_ExtraResetRecord164;



extern QwenRecovery_4ecc6b5a4034_ExtraResetRecord164 LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE000[100];
extern QwenRecovery_4ecc6b5a4034_u32 LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[139];

void LVL_10_HRUGIS_CLOUD_FUN_003A0BA0(void)
{
    int i;
    for (i = 99; i >= 0; --i)
        LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE000[i].word = 0;
    LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[138] = 5;
    LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[137] = 0;
    LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[i] = 0;
        LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_10_HRUGIS_CLOUD_QWEN_4ecc6b5a4034_AT003A0BA0_ROLE001[i + 128] = 0;
}



void LVL_10_HRUGIS_CLOUD_FUN_00449610(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_10_HRUGIS_CLOUD_FUN_00449618(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_10_HRUGIS_CLOUD_FUN_00449770(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_10_HRUGIS_CLOUD_FUN_004498C0(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_10_HRUGIS_CLOUD_FUN_00449990(unsigned long param_1)
{
  return param_1;
}


extern void LVL_10_HRUGIS_CLOUD_QWEN_df8fc8ba49cf_AT0044C358_ROLE000(unsigned char *owner, int value);

void LVL_10_HRUGIS_CLOUD_FUN_0044C358(unsigned char *owner, int value)
{
    LVL_10_HRUGIS_CLOUD_QWEN_df8fc8ba49cf_AT0044C358_ROLE000(owner + 8, value);
}



void* LVL_10_HRUGIS_CLOUD_FUN_00450D58(void* param_1);

extern void LVL_10_HRUGIS_CLOUD_QWEN_f353c206e726_AT00450D58_ROLE000(int);
extern unsigned long long LVL_10_HRUGIS_CLOUD_QWEN_f353c206e726_AT00450D58_ROLE001(unsigned long long);

void* LVL_10_HRUGIS_CLOUD_FUN_00450D58(void* param_1)
{
  LVL_10_HRUGIS_CLOUD_QWEN_f353c206e726_AT00450D58_ROLE000((int)param_1 + 8);
  LVL_10_HRUGIS_CLOUD_QWEN_f353c206e726_AT00450D58_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_10_HRUGIS_CLOUD_QWEN_f2f9288a4e34_AT00450EF8_ROLE000(int);

int LVL_10_HRUGIS_CLOUD_FUN_00450EF8(int owner)
{
    int result;
    result = LVL_10_HRUGIS_CLOUD_QWEN_f2f9288a4e34_AT00450EF8_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_10_HRUGIS_CLOUD_QWEN_e41cd63258f5_AT00450F30_ROLE000(unsigned char *);

int LVL_10_HRUGIS_CLOUD_FUN_00450F30(int owner)
{
    return LVL_10_HRUGIS_CLOUD_QWEN_e41cd63258f5_AT00450F30_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_10_HRUGIS_CLOUD_QWEN_f29f80950ce7_AT00454650_ROLE000(int);

void LVL_10_HRUGIS_CLOUD_FUN_00454650(int param_1)
{
  LVL_10_HRUGIS_CLOUD_QWEN_f29f80950ce7_AT00454650_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_10_HRUGIS_CLOUD_QWEN_bf824305b9f7_AT00454F68_ROLE000(unsigned char *owner, float *records);

void LVL_10_HRUGIS_CLOUD_FUN_00454F68(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_10_HRUGIS_CLOUD_QWEN_bf824305b9f7_AT00454F68_ROLE000(owner + 0x188, records);
}



void LVL_10_HRUGIS_CLOUD_QWEN_61faeca45963_AT00455230_ROLE000(int param_1);

void LVL_10_HRUGIS_CLOUD_FUN_00455230(int param_1)
{
  LVL_10_HRUGIS_CLOUD_QWEN_61faeca45963_AT00455230_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_10_HRUGIS_CLOUD_FUN_00455398(unsigned long param_1)
{
  return param_1;
}


extern int LVL_10_HRUGIS_CLOUD_QWEN_6add11b33414_AT00456148_ROLE000(unsigned char *owner);

int LVL_10_HRUGIS_CLOUD_FUN_00456148(unsigned char *owner)
{
    return LVL_10_HRUGIS_CLOUD_QWEN_6add11b33414_AT00456148_ROLE000(owner + 0x298);
}



extern void LVL_10_HRUGIS_CLOUD_QWEN_de961518de48_AT00456168_ROLE000(unsigned char *owner);

void LVL_10_HRUGIS_CLOUD_FUN_00456168(unsigned char *owner)
{
    LVL_10_HRUGIS_CLOUD_QWEN_de961518de48_AT00456168_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_10_HRUGIS_CLOUD_QWEN_7ba3cdeeb16b_AT0045AA88_ROLE000(unsigned long long);

unsigned long long LVL_10_HRUGIS_CLOUD_FUN_0045AA88(unsigned long long value)
{
    LVL_10_HRUGIS_CLOUD_QWEN_7ba3cdeeb16b_AT0045AA88_ROLE000(value);
    return value;
}



void LVL_10_HRUGIS_CLOUD_FUN_0045ACD0(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_10_HRUGIS_CLOUD_FUN_0045BD30(unsigned long param_1)
{
  return param_1;
}


void LVL_10_HRUGIS_CLOUD_FUN_0045C070(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_10_HRUGIS_CLOUD_FUN_0045C210(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_10_HRUGIS_CLOUD_FUN_0045C258(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_10_HRUGIS_CLOUD_FUN_004611B0(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_10_HRUGIS_CLOUD_FUN_004632A0(void) {
    return 1;
}


extern int LVL_10_HRUGIS_CLOUD_QWEN_cc83cb329fcb_AT00464AE0_ROLE000(unsigned char *);

int LVL_10_HRUGIS_CLOUD_FUN_00464AE0(int *owner)
{
    int result;
    long status;
    status = LVL_10_HRUGIS_CLOUD_QWEN_cc83cb329fcb_AT00464AE0_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_0039A628(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_003BA018(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_003CA608(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_003D5420(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_003DFF10(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_003F2190(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_003FAB60(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_00420618(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(char *local, char *first, char *second);
extern void LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_10_HRUGIS_CLOUD_FUN_00432A20(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B60(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_10_HRUGIS_CLOUD_Fc936841d_FUN_00308B30((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_00448A48(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(char *p, int v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(char *p, float v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(char *p, float v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(char *p, float v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(char *p, float v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(char *p, float v);
extern void LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(char *p, float v);

void LVL_10_HRUGIS_CLOUD_FUN_0045F770(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_00448A48(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(p1, 1);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(p2, 1);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(p3, 1);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(p4, 1);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(p5, 1);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C218(p6, 1);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_10_HRUGIS_CLOUD_F01bd4546_FUN_0045C260(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_003923D8(int a, int b);
extern void LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00301108(int a);
extern void *LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(int id);
extern int LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(void *p, int i);
extern void LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303F78(void);
extern void LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00304410(int a, int b, long c, void *d, int e);
extern void LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303F68(void);
extern void LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00301228(void);

int LVL_10_HRUGIS_CLOUD_FUN_00366F60(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_003923D8(66, 68);
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_003923D8(71, 11);
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00301108(0);
    min = LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11613), -1);
    v = LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303FF0(LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303F78();
    off = count - 6;
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00304410(slot, off, 0x80FFA888L, LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11613), -1);
    off += count;
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00304410(slot, off, 0x80FFA888L, LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11625), -1);
    off += count;
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00304410(slot, off, 0x80FFA888L, LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11626), -1);
    off += count;
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00304410(slot, off, 0x80FFA888L, LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11627), -1);
    off += count;
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00304410(slot, off, 0x80FFA888L, LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_0030F0C0(11599), -1);
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00303F68();
    LVL_10_HRUGIS_CLOUD_F70997ba9_FUN_00301228();
    return 2;
}
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);

void LVL_10_HRUGIS_CLOUD_FUN_003B9BE0(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[0], v[3]);
        v[1] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[1], v[4]);
        v[2] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[2], v[5]);
        v[9] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[9], v[12]);
        v[10] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[10], v[13]);
        v[11] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[11], v[14]);
        v[20] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(v[0]) * v[6];
        v[21] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[1]) * v[7];
        v[22] = -LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[2]) * v[8];
        v[24] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(v[9]) * v[15];
        v[25] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[10]) * v[16];
        v[26] = -LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(float, float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);
extern float LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(float);

void LVL_10_HRUGIS_CLOUD_FUN_003F1D58(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[0], v[3]);
        v[1] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[1], v[4]);
        v[2] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[2], v[5]);
        v[9] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[9], v[12]);
        v[10] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[10], v[13]);
        v[11] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309B38(v[11], v[14]);
        v[20] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(v[0]) * v[6];
        v[21] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[1]) * v[7];
        v[22] = -LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[2]) * v[8];
        v[24] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309120(v[9]) * v[15];
        v[25] = LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[10]) * v[16];
        v[26] = -LVL_10_HRUGIS_CLOUD_F307ea0ee_FUN_00309138(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
#ifndef RAC2_T_WORKER_F9E2CD219
#define RAC2_T_WORKER_F9E2CD219
typedef struct {
    unsigned char pad00[76];
    short field4C;
    unsigned char pad4E[2];
    int field50;
    float field54;
    float field58;
} Worker_F9e2cd219;
#endif


#ifndef RAC2_T_OWNER_F9E2CD219
#define RAC2_T_OWNER_F9E2CD219
typedef struct {
    unsigned char pad00[104];
    Worker_F9e2cd219 *child;
} Owner_F9e2cd219;
#endif


extern float LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_00335138(float value);
extern void LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_0033AC88(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_003A0860(void *owner, void *out);

void LVL_10_HRUGIS_CLOUD_FUN_003A0778(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_0033AC88((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_00335138(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_003A0860(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
#ifndef RAC2_T_WORKER_F9E2CD219
#define RAC2_T_WORKER_F9E2CD219
typedef struct {
    unsigned char pad00[76];
    short field4C;
    unsigned char pad4E[2];
    int field50;
    float field54;
    float field58;
} Worker_F9e2cd219;
#endif


#ifndef RAC2_T_OWNER_F9E2CD219
#define RAC2_T_OWNER_F9E2CD219
typedef struct {
    unsigned char pad00[104];
    Worker_F9e2cd219 *child;
} Owner_F9e2cd219;
#endif


extern float LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_00335138(float value);
extern void LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_0033AC88(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_003A3300(void *owner, void *out);

void LVL_10_HRUGIS_CLOUD_FUN_003A3218(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_0033AC88((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_00335138(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_10_HRUGIS_CLOUD_F9e2cd219_FUN_003A3300(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
