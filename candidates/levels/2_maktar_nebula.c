typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_2_MAKTAR_NEBULA_D_001A8EB0;
void LVL_2_MAKTAR_NEBULA_FUN_002DAE10(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_2_MAKTAR_NEBULA_FUN_002F7CD8(s32 index) {
    NativeTable20 values=LVL_2_MAKTAR_NEBULA_D_001A8EB0;
    return values.items[index];
}

u32 LVL_2_MAKTAR_NEBULA_FUN_002B12D0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_2_MAKTAR_NEBULA_D_0018C0B4;
u32 LVL_2_MAKTAR_NEBULA_FUN_002D9DB8(void) {
    return LVL_2_MAKTAR_NEBULA_D_0018C0B4;
}

s32 LVL_2_MAKTAR_NEBULA_FUN_002E6F80(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_2_MAKTAR_NEBULA_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_2_MAKTAR_NEBULA_FUN_002B1168(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_2_MAKTAR_NEBULA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_2_MAKTAR_NEBULA_FUN_002B11A0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_2_MAKTAR_NEBULA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_2_MAKTAR_NEBULA_FUN_002B1DC0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_2_MAKTAR_NEBULA_FUN_002DAB68(f32, f32, f32, f32, s32, s32);

void LVL_2_MAKTAR_NEBULA_FUN_002DA610(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_2_MAKTAR_NEBULA_FUN_002DAB68(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_2_MAKTAR_NEBULA_FUN_002F4530(int width, int height, int address, int mode);
extern void LVL_2_MAKTAR_NEBULA_FUN_00382CB8(unsigned int reg, unsigned long value);
extern void LVL_2_MAKTAR_NEBULA_FUN_002F4898(int width, int height);

void LVL_2_MAKTAR_NEBULA_FUN_002E8D20(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_2_MAKTAR_NEBULA_FUN_002F4530(width, height, address, 1);
    LVL_2_MAKTAR_NEBULA_FUN_00382CB8(0x47, 0x30000UL);
    LVL_2_MAKTAR_NEBULA_FUN_00382CB8(0x42, 0x8000000044UL);
    LVL_2_MAKTAR_NEBULA_FUN_002F4898(0x100, 0x100);
    LVL_2_MAKTAR_NEBULA_FUN_00382CB8(0x42, 0x8000000044UL);
}

extern void LVL_2_MAKTAR_NEBULA_FUN_002D6B60(f32, f32, f32, f32 *, s32);

void LVL_2_MAKTAR_NEBULA_FUN_002B0FF8(f32 *output) {
 LVL_2_MAKTAR_NEBULA_FUN_002D6B60(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_2_MAKTAR_NEBULA_FUN_002B11D8(s32 index) {
 s32 value=LVL_2_MAKTAR_NEBULA_FUN_002B11A0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_2_MAKTAR_NEBULA_FUN_002B1210(s32 index) {
 return LVL_2_MAKTAR_NEBULA_FUN_002B11A0(index)==47;
}

s32 LVL_2_MAKTAR_NEBULA_FUN_002B66C8(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_2_MAKTAR_NEBULA_FUN_003484E8(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_2_MAKTAR_NEBULA_D_00280200[13];
void LVL_2_MAKTAR_NEBULA_FUN_002FB008(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_2_MAKTAR_NEBULA_D_00280200[i].key == key) break;
    }
    if (i < 13) {
        LVL_2_MAKTAR_NEBULA_D_00280200[i].fields[9] = value;
        if (LVL_2_MAKTAR_NEBULA_D_00280200[i].busy == 0)
            LVL_2_MAKTAR_NEBULA_D_00280200[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_2_MAKTAR_NEBULA_D_001395B8[];
u32 LVL_2_MAKTAR_NEBULA_FUN_00309990(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_2_MAKTAR_NEBULA_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_2_MAKTAR_NEBULA_D_00232A80[];
s32 LVL_2_MAKTAR_NEBULA_FUN_00385598(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_2_MAKTAR_NEBULA_D_00232A80;
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
extern int LVL_2_MAKTAR_NEBULA_D_0022F340[];
extern ListOverrideObject *LVL_2_MAKTAR_NEBULA_D_00227E40[];
extern ListOverridePair LVL_2_MAKTAR_NEBULA_D_0022ED40[];
void LVL_2_MAKTAR_NEBULA_FUN_00372368(void)
{
    int *selected = LVL_2_MAKTAR_NEBULA_D_0022F340;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_2_MAKTAR_NEBULA_D_00227E40[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_2_MAKTAR_NEBULA_D_0022ED40[row->key];
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
extern NativeObjectSearchRecord32 LVL_2_MAKTAR_NEBULA_D_00255590[];
s32 LVL_2_MAKTAR_NEBULA_FUN_00420E00(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_2_MAKTAR_NEBULA_D_00255590[index].field14 == object) {
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

s32 LVL_2_MAKTAR_NEBULA_FUN_002D9C78(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_2_MAKTAR_NEBULA_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_2_MAKTAR_NEBULA_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_2_MAKTAR_NEBULA_D_00189E20)->mode == 0x31) {
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
extern void *LVL_2_MAKTAR_NEBULA_D_0018C0B0;
extern void *LVL_2_MAKTAR_NEBULA_D_0018B134;
extern void *LVL_2_MAKTAR_NEBULA_D_0018B040;
void *LVL_2_MAKTAR_NEBULA_FUN_002A82E8(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_2_MAKTAR_NEBULA_D_0018C0B0;
    if (kind == 1)
        return LVL_2_MAKTAR_NEBULA_D_0018B134;
    if (kind == 6)
        return LVL_2_MAKTAR_NEBULA_D_0018B040;
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
extern unsigned char LVL_2_MAKTAR_NEBULA_D_00139568[];
extern MappedClassEntry LVL_2_MAKTAR_NEBULA_D_00264A70[];
unsigned int LVL_2_MAKTAR_NEBULA_FUN_002F79B0(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_2_MAKTAR_NEBULA_D_00264A70[LVL_2_MAKTAR_NEBULA_D_00139568[i]];
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
extern s32 LVL_2_MAKTAR_NEBULA_FUN_002F1C80(s32 value);
NativeCompactHeader *LVL_2_MAKTAR_NEBULA_FUN_003878F8(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_2_MAKTAR_NEBULA_FUN_002F1C80(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_2_MAKTAR_NEBULA_FUN_002F1C80(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_2_MAKTAR_NEBULA_D_001A79F0;
s32 LVL_2_MAKTAR_NEBULA_FUN_002D9BF8(void) {
    s32 found = 0;
    if (LVL_2_MAKTAR_NEBULA_D_001A79F0 == 25 || LVL_2_MAKTAR_NEBULA_D_001A79F0 == 5 ||
        LVL_2_MAKTAR_NEBULA_D_001A79F0 == 10 || LVL_2_MAKTAR_NEBULA_D_001A79F0 == 15) {
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
void LVL_2_MAKTAR_NEBULA_FUN_002D6650(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_2_MAKTAR_NEBULA_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_2_MAKTAR_NEBULA_FUN_002D6688(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_2_MAKTAR_NEBULA_D_00189E20;
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
extern ConditionalResetSlot LVL_2_MAKTAR_NEBULA_D_001BA340[8];
void LVL_2_MAKTAR_NEBULA_FUN_002DA040(void) {
    ConditionalResetSlot *slot = LVL_2_MAKTAR_NEBULA_D_001BA340;
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
extern StateTransitionView LVL_2_MAKTAR_NEBULA_D_001C0640;
void LVL_2_MAKTAR_NEBULA_FUN_002F7318(void) {
    if (LVL_2_MAKTAR_NEBULA_D_001C0640.mode == 7 && LVL_2_MAKTAR_NEBULA_D_001C0640.state == 1) {
        LVL_2_MAKTAR_NEBULA_D_001C0640.state = 2;
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
extern DobboFormatRoot396 LVL_2_MAKTAR_NEBULA_D_001CAAE0;
extern const char LVL_2_MAKTAR_NEBULA_D_001A9AA0[];
extern const char LVL_2_MAKTAR_NEBULA_D_001A9AA8[];
extern const unsigned char *LVL_2_MAKTAR_NEBULA_FUN_002F8570(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_2_MAKTAR_NEBULA_FUN_0030D010(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_2_MAKTAR_NEBULA_FUN_002F8570(LVL_2_MAKTAR_NEBULA_D_001CAAE0.rows[index].text_id);
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
        int key = LVL_2_MAKTAR_NEBULA_D_001CAAE0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_2_MAKTAR_NEBULA_D_00264A70[LVL_2_MAKTAR_NEBULA_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_2_MAKTAR_NEBULA_D_001A9AA0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_2_MAKTAR_NEBULA_D_001A9AA8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_2_MAKTAR_NEBULA_FUN_002B6E18(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_2_MAKTAR_NEBULA_D_00189E20;
    if (value < root->plane) {
        if (root->plane - value <= root->depth)
            return 1;
    }
    return 0;
}

/* Update the observed two-axis selection fields and their combined index. */
typedef struct {
    unsigned char gap0[0x43c];
    int column, row, index, mode;
} DobboGridState312;
extern int LVL_2_MAKTAR_NEBULA_FUN_00366C90(int, unsigned int, void *);
void LVL_2_MAKTAR_NEBULA_FUN_00464EC8(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_2_MAKTAR_NEBULA_FUN_00366C90(3, 0, 0);
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
        LVL_2_MAKTAR_NEBULA_FUN_00366C90(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_2_MAKTAR_NEBULA_FUN_00366C90(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_2_MAKTAR_NEBULA_FUN_00366C90(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_2_MAKTAR_NEBULA_D_001B3500[16];
extern u32 LVL_2_MAKTAR_NEBULA_D_001B3540[16];

int LVL_2_MAKTAR_NEBULA_FUN_003153E0(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_2_MAKTAR_NEBULA_D_001B3500[index] == 0 ||
            LVL_2_MAKTAR_NEBULA_D_001B3500[index] == object) {
            LVL_2_MAKTAR_NEBULA_D_001B3500[index] = object;
            LVL_2_MAKTAR_NEBULA_D_001B3540[index] = 0;
            return index;
        }
    }
    return -1;
}

typedef struct {
    u8 prefix[0x40];
    void *payload;
    u32 unused44;
    short key;
    u8 marker;
    u8 unused4b;
    u8 selector;
    u8 tail[3];
} GornRecordWrite64;
typedef char GornRecordWrite64Stride[(sizeof(GornRecordWrite64) == 80) ? 1 : -1];

void LVL_2_MAKTAR_NEBULA_FUN_003282B0(GornRecordWrite64 *record, u32 selector,
                            int key, void *payload)
{
    if (record->marker != 0) {
        u8 marker;
        do {
            if (record->selector == selector && record->key == key) {
                record->payload = payload;
                return;
            }
            marker = record->marker;
            ++record;
        } while (marker != 1);
    }
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
extern void LVL_2_MAKTAR_NEBULA_FUN_002DFFE0(OozlaAppendObject164 *, int, const float *);
extern void LVL_2_MAKTAR_NEBULA_FUN_002F1FA0(float *, const float *, const float *);
extern void LVL_2_MAKTAR_NEBULA_FUN_002F2450(float *, const float *, const float *);
extern void LVL_2_MAKTAR_NEBULA_FUN_002E0328(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_2_MAKTAR_NEBULA_FUN_002DFF38(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_2_MAKTAR_NEBULA_FUN_002DFFE0(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_2_MAKTAR_NEBULA_FUN_002F1FA0(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_2_MAKTAR_NEBULA_FUN_002F2450(difference, difference, &object->transform[0][0]);
        LVL_2_MAKTAR_NEBULA_FUN_002E0328(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_2_MAKTAR_NEBULA_FUN_0046A340(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_2_MAKTAR_NEBULA_FUN_0046A7D8(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

extern unsigned char D_19B278[];

int LVL_2_MAKTAR_NEBULA_FUN_0032BAD0(void)
{
    int count = 0;
    int i;

    for (i = 0; i < 0x1C; i++) {
        int j;

        for (j = 0; j < 4; j++) {
            if (D_19B278[i * 4 + j] != 0)
                count++;
        }
    }
    if (count < 0)
        count = 0;
    if (count > 0x28)
        count = 0x28;
    return count;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_2_MAKTAR_NEBULA_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_2_MAKTAR_NEBULA_FUN_00334130(void) {
    if (((CallState *)LVL_2_MAKTAR_NEBULA_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_2_MAKTAR_NEBULA_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_2_MAKTAR_NEBULA_D_001A63A8)->active); ((CallState *)LVL_2_MAKTAR_NEBULA_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_2_MAKTAR_NEBULA_FUN_0031E710(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_2_MAKTAR_NEBULA_FUN_00309910(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_2_MAKTAR_NEBULA_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_2_MAKTAR_NEBULA_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct {
    u8 pad0000[0x2294];
    u32 kind;
    u32 unknown2298;
    u32 mode;
} ResidentFlags2294;

s32 LVL_2_MAKTAR_NEBULA_FUN_002B1280(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_2_MAKTAR_NEBULA_D_00189E20;

    if (root->mode == 17 || root->mode == 18
        || root->kind == 0x67 || root->kind == 0x7f
        || root->kind == 0x73 || root->kind == 0x72) {
        return 1;
    }
    return 0;
}

/* Prototype775 is a research label. Field names below describe only the
 * independently observed byte23 and binary32 word2C operations. */
typedef struct {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[8];
    f32 field2C;
} NativeUpdate775View;

extern void LVL_2_MAKTAR_NEBULA_FUN_003144D8(NativeUpdate775View *object);

void LVL_2_MAKTAR_NEBULA_FUN_003A8178(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_2_MAKTAR_NEBULA_FUN_003144D8(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_2_MAKTAR_NEBULA_FUN_0032F0E8(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_2_MAKTAR_NEBULA_FUN_00323628(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_2_MAKTAR_NEBULA_FUN_002AA3F0(void)
{
}


unsigned int LVL_2_MAKTAR_NEBULA_FUN_002D9D48(void)
{
    return 0;
}


void LVL_2_MAKTAR_NEBULA_FUN_002F1278(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_002F8308(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_002F90B8(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_003007C8(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_003007D0(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00304670(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_0030D3C8(void)
{
}


unsigned int LVL_2_MAKTAR_NEBULA_FUN_00376B90(void)
{
    return 0;
}


void LVL_2_MAKTAR_NEBULA_FUN_0037B6A8(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_003826C0(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00386048(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00389728(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_0041D9E0(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_0043F4C0(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00457B00(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00459570(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_004597C0(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00459CB8(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00462530(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00463188(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_0046EF90(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00471320(void)
{
}


void LVL_2_MAKTAR_NEBULA_FUN_00472BB8(void)
{
}
void LVL_2_MAKTAR_NEBULA_FUN_003BD598(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_003C8618(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_003E5898(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_003F6FF0(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_003FFB58(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00402F78(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_004095E0(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00418DA8(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_0041A150(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_0043D370(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00443D10(char *p)
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

extern u8 LVL_2_MAKTAR_NEBULA_F62e6ff2b_D_00189E20[];
extern u8 LVL_2_MAKTAR_NEBULA_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_2_MAKTAR_NEBULA_FUN_00366888(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_2_MAKTAR_NEBULA_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_2_MAKTAR_NEBULA_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_2_MAKTAR_NEBULA_F62e6ff2b_D_00188660;
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
void LVL_2_MAKTAR_NEBULA_FUN_003AD7C0(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_0040F7C8(char *p)
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
extern Blob LVL_2_MAKTAR_NEBULA_F6894d7c1_D_001A8E60;
int LVL_2_MAKTAR_NEBULA_FUN_002F75E0(int x)
{
    Blob b;
    int i;
    b = LVL_2_MAKTAR_NEBULA_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_2_MAKTAR_NEBULA_Fbc82131e_FUN_00115DA8(char *, char *, ...);
extern char LVL_2_MAKTAR_NEBULA_Fbc82131e_D_001AD6B0[];
extern char LVL_2_MAKTAR_NEBULA_Fbc82131e_D_001AD6C0[];
extern char LVL_2_MAKTAR_NEBULA_Fbc82131e_D_001AD6C8[];

void LVL_2_MAKTAR_NEBULA_FUN_0037D0A8(char *dst, int value)
{
    if (value > 999999)
        LVL_2_MAKTAR_NEBULA_Fbc82131e_FUN_00115DA8(dst, LVL_2_MAKTAR_NEBULA_Fbc82131e_D_001AD6B0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_2_MAKTAR_NEBULA_Fbc82131e_FUN_00115DA8(dst, LVL_2_MAKTAR_NEBULA_Fbc82131e_D_001AD6C0, value / 1000, value % 1000);
    else
        LVL_2_MAKTAR_NEBULA_Fbc82131e_FUN_00115DA8(dst, LVL_2_MAKTAR_NEBULA_Fbc82131e_D_001AD6C8, value);
}
void LVL_2_MAKTAR_NEBULA_FUN_00391A70(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00394110(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_003F44B8(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_0040B558(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00422690(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00446690(char *p)
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

extern Entry *LVL_2_MAKTAR_NEBULA_Fc68ad20a_D_0018C2B8;

int LVL_2_MAKTAR_NEBULA_FUN_002D53F8(int key, int *out)
{
    Entry *e = LVL_2_MAKTAR_NEBULA_Fc68ad20a_D_0018C2B8;
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
extern void LVL_2_MAKTAR_NEBULA_F5b6bba31_FUN_00115E38(char *assertion, int line, char *file);
extern char LVL_2_MAKTAR_NEBULA_F5b6bba31_D_001ADDD8[];
extern char LVL_2_MAKTAR_NEBULA_F5b6bba31_D_001ADDF8[];

void LVL_2_MAKTAR_NEBULA_FUN_00457B18(int *p, unsigned int a1, int a2, int a3)
{
    if (a1 < 4)
        LVL_2_MAKTAR_NEBULA_F5b6bba31_FUN_00115E38(LVL_2_MAKTAR_NEBULA_F5b6bba31_D_001ADDD8, 37, LVL_2_MAKTAR_NEBULA_F5b6bba31_D_001ADDF8);
    p[1] = a3;
    p[2] = a1;
    p[4] = 0;
    p[5] = 0;
    p[3] = 0;
    p[0] = a2;
}
typedef struct {
    int f0;
    short f4;
    unsigned char f6;
    char pad[0x18 - 7];
    int f18;
    int f1C;
} Blk;

extern Blk LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8;
extern short LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63AC;
extern int LVL_2_MAKTAR_NEBULA_F55a1acb8_FUN_00133688(void);
extern void LVL_2_MAKTAR_NEBULA_F55a1acb8_FUN_0011AEA0(int);

void LVL_2_MAKTAR_NEBULA_FUN_00335200(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_2_MAKTAR_NEBULA_F55a1acb8_FUN_00133688()) {
        LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_2_MAKTAR_NEBULA_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f6;
    q = LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f18;
    LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f1C;
        LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_2_MAKTAR_NEBULA_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
void LVL_2_MAKTAR_NEBULA_FUN_0034D2A8(void)
{
    *(short *)0x001AA8C2 = *(unsigned char *)0x001A7BC9 ? 3 : 0;
    *(short *)0x001AA8DA = *(unsigned char *)0x001A7BCA ? 3 : 0;
    *(short *)0x001AA8F2 = *(unsigned char *)0x001A7BCB ? 3 : 0;
    *(short *)0x001AA90A = *(unsigned char *)0x001A7BCC ? 3 : 0;
    *(short *)0x001AA922 = *(unsigned char *)0x001A7BCE ? 3 : 0;
}
extern char LVL_2_MAKTAR_NEBULA_Fa2dbe766_D_00189E20[];

int LVL_2_MAKTAR_NEBULA_FUN_003DED30(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_2_MAKTAR_NEBULA_Fa2dbe766_D_00189E20;
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
typedef struct { unsigned int f0, f4, f8, f12, f16, f20; } T;

extern char LVL_2_MAKTAR_NEBULA_Facf7396d_D_001ADDD8[];
extern char LVL_2_MAKTAR_NEBULA_Facf7396d_D_001ADE20[];
extern void LVL_2_MAKTAR_NEBULA_Facf7396d_FUN_00115E38(char *a0, int a1, char *a2);

void *LVL_2_MAKTAR_NEBULA_FUN_00457BA0(T *p)
{
    unsigned int off, size;
    void *r;

    if (p->f20 != 0)
    {
        unsigned int *x = (unsigned int *)p->f20;

        p->f20 = x[0];
        p->f16 = p->f16 + 1;
        return x;
    }
    off = p->f12;
    size = p->f8;
    if (p->f4 < off + size)
    {
        LVL_2_MAKTAR_NEBULA_Facf7396d_FUN_00115E38(LVL_2_MAKTAR_NEBULA_Facf7396d_D_001ADDD8, 83, LVL_2_MAKTAR_NEBULA_Facf7396d_D_001ADE20);
        return 0;
    }
    r = (void *)(p->f0 + off);
    p->f12 = off + size;
    p->f16 = p->f16 + 1;
    return r;
}
void LVL_2_MAKTAR_NEBULA_FUN_00434678(char *object)
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
extern char LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20[];

void LVL_2_MAKTAR_NEBULA_FUN_002D64E8(void)
{
    char *b = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
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
        char *c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_2_MAKTAR_NEBULA_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_2_MAKTAR_NEBULA_FUN_0040FFF0(char *p)
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
void LVL_2_MAKTAR_NEBULA_FUN_00446628(char *p)
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
extern char LVL_2_MAKTAR_NEBULA_F0be97c76_D_00189E20[];

void LVL_2_MAKTAR_NEBULA_FUN_002AA370(void)
{
    char *base = LVL_2_MAKTAR_NEBULA_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_2_MAKTAR_NEBULA_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_2_MAKTAR_NEBULA_Fee2b87d1_D_00152CD0;

s32 LVL_2_MAKTAR_NEBULA_FUN_00309050(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_2_MAKTAR_NEBULA_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_2_MAKTAR_NEBULA_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_2_MAKTAR_NEBULA_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_2_MAKTAR_NEBULA_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* Family 9b94f3cd1f5fac83 (372 B, 2 placements).
   Retail builds a 64-byte packet from the resident cursor global LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 and
   then advances that global itself, so the closing store addresses the global
   through $gp: the qualified small-data profile is required.
   The four header words are written through the global expression, so each one
   carries its own reload of LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 before its store. */

extern int *LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 __attribute__((sda));
extern int LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7350 __attribute__((sda));
extern int LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7354 __attribute__((sda));

void LVL_2_MAKTAR_NEBULA_FUN_002FF7F8(int a0, int a1, int a2, int a3, long long a4, int a5, int a6)
{
    int *p;

    *(int *)LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 = 0x10000003;
    *(int *)((char *)LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 + 4) = 0;
    *(int *)((char *)LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 + 8) = 0;
    *(int *)((char *)LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 + 12) = 0x50000003;
    p = LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08;
    LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 = p + 4;
    *(long long *)(p + 4) = 0x4400000000008001LL;
    *(long long *)(p + 6) = 17424;
    *(long long *)(p + 8) = 70;
    *(long long *)(p + 10) = a4;
    if (a6) {
        *(long long *)(p + 12) = (a0 + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7350 - 8)
                               | ((long long)(a1 + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7354 - 8) << 16)
                               | ((long long)a5 << 32);
        *(long long *)(p + 14) = (a2 + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7350 - 8)
                               | ((long long)(a3 + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7354 - 8) << 16)
                               | ((long long)a5 << 32);
    } else {
        *(long long *)(p + 12) = ((a0 << 4) + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7350 - 16)
                               | ((long long)((a1 << 4) + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7354 - 16) << 16)
                               | ((long long)a5 << 32);
        *(long long *)(p + 14) = ((a2 << 4) + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7350 - 16)
                               | ((long long)((a3 << 4) + LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001A7354 - 16) << 16)
                               | ((long long)a5 << 32);
    }
    LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 = (int *)((char *)LVL_2_MAKTAR_NEBULA_F9b94f3cd_D_001B3D08 + 48);
}
/* Family c1edda5cc3fa91ec — 148 bytes, levels/2_maktar_nebula and levels/11_joba.

   Write the four-word ring header and advance the resident pointer; when the
   pointer is null, hand the sibling buffer 48 bytes further on to the stop
   helper instead. Every reference reads the global again, and the last store
   addresses it through $gp (a delay-slot form the pinned -G0 profile cannot
   express). */
extern int *LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 __attribute__((sda));
extern int LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001A742C __attribute__((sda));
extern void LVL_2_MAKTAR_NEBULA_Fc1edda5c_FUN_00126108(void *);

void LVL_2_MAKTAR_NEBULA_FUN_002F43A8(void)
{
    if (LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 != 0) {
        *(int *)LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 = 0x30000009;
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 + 4) = (LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001A742C + 48) & 0x0FFFFFFF;
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 + 8) = 0;
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 + 12) = 0x50000009;
        LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 = (int *)((char *)LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001B3D08 + 16);
    } else {
        LVL_2_MAKTAR_NEBULA_Fc1edda5c_FUN_00126108((void *)(LVL_2_MAKTAR_NEBULA_Fc1edda5c_D_001A742C + 48));
    }
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_2_MAKTAR_NEBULA_F1157be91_D_001A63E8;
extern unsigned char LVL_2_MAKTAR_NEBULA_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_2_MAKTAR_NEBULA_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_2_MAKTAR_NEBULA_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_2_MAKTAR_NEBULA_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_2_MAKTAR_NEBULA_F1157be91_FUN_00133230(void);
extern int LVL_2_MAKTAR_NEBULA_F1157be91_FUN_00132028(void);

int LVL_2_MAKTAR_NEBULA_FUN_00335088(int a0, int a1, int a2) {
    CdMode mode = LVL_2_MAKTAR_NEBULA_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_2_MAKTAR_NEBULA_F1157be91_D_001A7900[0];
    LVL_2_MAKTAR_NEBULA_F1157be91_D_001A7430[0] = 0;
    LVL_2_MAKTAR_NEBULA_F1157be91_D_001A7434 = 0;
    LVL_2_MAKTAR_NEBULA_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_2_MAKTAR_NEBULA_F1157be91_FUN_00133230();
    LVL_2_MAKTAR_NEBULA_F1157be91_FUN_00132028();
    return 1;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20;
extern s32 LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_2_MAKTAR_NEBULA_FUN_002D6880(void) {
    s32 result = LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field348;
    if (LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A8FF0 != 0 && LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A8FF4 != 0 || LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field2294 == 110 && LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field2294 == 109 || LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field1497 != 0 && LVL_2_MAKTAR_NEBULA_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field2294 == 0 && LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_2_MAKTAR_NEBULA_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
void LVL_2_MAKTAR_NEBULA_FUN_003247E0(int param_1, short *param_2, short param_3)
{
    int i;

    param_1 = (param_1 - *(int *)0x001B359C) << 8 >> 16;
    for (i = 1; i <= param_2[0]; i++) {
        if (param_2[i] == param_1) return;
    }
    if (param_2[0] < param_3) {
        param_2[0] = param_2[0] + 1;
        param_2[param_2[0]] = param_1;
    }
}
/* Family 489bf28ebf58470c (120 B, 2 placements): call every registered
 * callback with its paired argument, bounded by a resident count word.
 *
 * The three resident globals are reached with `lui`+`lw` / `lui`+`addiu`
 * pairs that the ASSEMBLER macro expands from a single RTL insn; this only
 * happens when the externs carry the `sda` attribute (SYMBOL_REF_FLAG), which
 * suppresses gcc's HIGH/LO_SUM address split.  Without it the compiler keeps
 * the `%hi` in an extra callee-saved register and the body is 3 words long.
 */
typedef void (*fn_t)(int);

extern int LVL_2_MAKTAR_NEBULA_F489bf28e_D_001B3078 __attribute__((sda));   /* resident callback count */
extern fn_t LVL_2_MAKTAR_NEBULA_F489bf28e_D_001B3058[] __attribute__((sda)); /* callback table        */
extern int LVL_2_MAKTAR_NEBULA_F489bf28e_D_001B3068[] __attribute__((sda));  /* argument table        */

void LVL_2_MAKTAR_NEBULA_FUN_002EA4A0(void) {
    int i;
    for (i = 0; i < LVL_2_MAKTAR_NEBULA_F489bf28e_D_001B3078; i++) {
        LVL_2_MAKTAR_NEBULA_F489bf28e_D_001B3058[i](LVL_2_MAKTAR_NEBULA_F489bf28e_D_001B3068[i]);
    }
}
/* Family d50cdefed4015bae — 364 bytes, a 16384-byte block loop that builds two
   four-word GS packets per iteration and calls the measured SDK helper at
   0x00126288 in between.

   The resident write pointer at 0x001B3D08 is read ten times and stored three
   times.  Two authored views of that one measured cell are needed and are the
   qualified form here:
     * the read view is volatile, so every use is a fresh load -- the retail
       body re-loads the cell before every field store (aliasing through the
       loaded pointer is not enough to force it: without the qualifier cc1
       forwards a stored value into the next two reads);
     * the write view is an ordinary pointer, so the store written before the
       call is moved into the `jal` delay slot, which is where retail keeps it
       (a volatile store is left in place).
   The bare symbols keep the address materialisation a per-site assembler job
   (`lui`+`lw` mid-stream, the single `$gp` form in a delay slot).

   -G0 promotion (agent f02, 2026-10-07): under -G8 these two declarations need
   no attribute, because -G8 alone puts the cell in small data.  Under -G0 cc1
   would otherwise model each access as an explicit `lui`+`lw` pair, hoist the
   base into a callee-saved register and reuse it, which changes both the
   register allocation and the body size.  The measured `sda` attribute puts
   cc1 back on the one-instruction model, so gas expands every site exactly as
   the retail build did: absolute `lui`+`lw` in ordinary flow, the single `$gp`
   form inside the `.set nomacro` region cc1 emits for the `jal` delay slot.
   Nothing else about the body changed. */

extern int *volatile LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 __attribute__((sda));
extern int *LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08_1 __attribute__((sda));

extern void LVL_2_MAKTAR_NEBULA_Fd50cdefe_FUN_00126288(int *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);

void LVL_2_MAKTAR_NEBULA_FUN_003830E8(int a0, int a1, int a2)
{
    int s0 = (a2 + 16383) & -16384;
    int s6 = 0;
    int s5 = 0;

    while (s0 > 0) {
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 0) = 0x10000006;
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 4) = 0;
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 8) = 0;
        *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 12) = 0x50000006;
        {
            int *next = (int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 16);
            LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08_1 = next;
            LVL_2_MAKTAR_NEBULA_Fd50cdefe_FUN_00126288(next, (short)((a1 + s6) >> 8), 1, 1, 0, 0, 64, 64);
        }
        s6 += 16384;
        s0 -= 16384;
        {
            int *p = LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08;
            int off = a0 + s5;
            s5 += 12288;
            LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08_1 = (int *)((char *)p + 96);
            *(int *)((char *)p + 96) = 0x30000300;
            *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 4) = off;
            *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 8) = 0;
            *(int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 12) = 0x50000300;
            LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08_1 = (int *)((char *)LVL_2_MAKTAR_NEBULA_Fd50cdefe_D_001B3D08 + 16);
        }
    }
}
extern volatile int * volatile LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08 __attribute__((sda));
extern volatile int * volatile LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08_1 __attribute__((sda));
extern char LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001A6D40[];
extern char LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001A6E90[];

void LVL_2_MAKTAR_NEBULA_FUN_002F4440(int a0)
{
    int *p = (int *)LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08;

    if (p == 0)
        return;
    p[0] = 0x30000015;
    if (a0 == 0)
        LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08[1] = (int)LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001A6D40;
    else
        LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08[1] = (int)LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001A6E90;
    LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08[2] = 0;
    LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08[3] = 0x50000015;
    LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08_1 = LVL_2_MAKTAR_NEBULA_Fcfd2e0cd_D_001B3D08 + 4;
}
extern int LVL_2_MAKTAR_NEBULA_Fbef45185_D_001B359C __attribute__((sda));

void LVL_2_MAKTAR_NEBULA_FUN_00324860(int param_1, unsigned short *param_2)
{
    int i;

    for (i = 1; i <= (short)param_2[0]; i++) {
        if (LVL_2_MAKTAR_NEBULA_Fbef45185_D_001B359C + ((short)param_2[i] << 8) == param_1) {
            param_2[i] = param_2[(short)param_2[0]];
            param_2[0] = param_2[0] - 1;
            return;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_2_MAKTAR_NEBULA_Fabf21065e887d7a9_AT00312F18_ROLE00;

int LVL_2_MAKTAR_NEBULA_FUN_00312F18(void)
{
    if ((LVL_2_MAKTAR_NEBULA_Fabf21065e887d7a9_AT00312F18_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_2_MAKTAR_NEBULA_Fabf21065e887d7a9_AT00312F18_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_2_MAKTAR_NEBULA_Fabf21065e887d7a9_AT00312F18_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_2_MAKTAR_NEBULA_Fabf21065e887d7a9_AT00312F18_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_2_MAKTAR_NEBULA_F5b4b17178a13f443_AT00327240_ROLE00(void *object);

void LVL_2_MAKTAR_NEBULA_FUN_00327240(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_2_MAKTAR_NEBULA_F5b4b17178a13f443_AT00327240_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_2_MAKTAR_NEBULA_Facdcf1600d770d3b_AT00367058_ROLE00[];

void LVL_2_MAKTAR_NEBULA_FUN_00367058(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_2_MAKTAR_NEBULA_Facdcf1600d770d3b_AT00367058_ROLE00;
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


extern void LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00390BD0_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00390BD0_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_2_MAKTAR_NEBULA_FUN_00390BD0(void *object)
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
    LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00390BD0_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00390BD0_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00393690_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00393690_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_2_MAKTAR_NEBULA_FUN_00393690(void *object)
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
    LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00393690_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_2_MAKTAR_NEBULA_Fb1b523716b470b36_AT00393690_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003A26D8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_003A26D8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003A26D8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003A72B8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_003A72B8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003A72B8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_2_MAKTAR_NEBULA_F86f665335d9cb905_AT003C71A8_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_2_MAKTAR_NEBULA_FUN_003C71A8(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_2_MAKTAR_NEBULA_F86f665335d9cb905_AT003C71A8_ROLE00(owner, owner->context_68);
}


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003DC5D0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_003DC5D0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003DC5D0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003E2670_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_003E2670(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT003E2670_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT00410418_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_00410418(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT00410418_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT00416C70_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_00416C70(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT00416C70_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT00428868_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_2_MAKTAR_NEBULA_FUN_00428868(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_2_MAKTAR_NEBULA_F9cdc323a4d0c2fbd_AT00428868_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_2_MAKTAR_NEBULA_F6af85cabb56d3b41_AT00454798_ROLE00;

void LVL_2_MAKTAR_NEBULA_FUN_00454798(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_2_MAKTAR_NEBULA_F6af85cabb56d3b41_AT00454798_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_2_MAKTAR_NEBULA_F6af85cabb56d3b41_AT00454798_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_2_MAKTAR_NEBULA_FUN_00456870(float factor, void *context,
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


void LVL_2_MAKTAR_NEBULA_FUN_00456A20(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_2_MAKTAR_NEBULA_FUN_0045BD80(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_2_MAKTAR_NEBULA_FUN_00463DA8(float first, float second, float **cell)
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


extern unsigned char LVL_2_MAKTAR_NEBULA_F79744baad5ad7f65_AT0046B218_ROLE00[];

void LVL_2_MAKTAR_NEBULA_FUN_0046B218(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_2_MAKTAR_NEBULA_F79744baad5ad7f65_AT0046B218_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_2_MAKTAR_NEBULA_F6b0741c38bf00fee_AT002A9488_ROLE00[];
extern void LVL_2_MAKTAR_NEBULA_F6b0741c38bf00fee_AT002A9488_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_2_MAKTAR_NEBULA_F6b0741c38bf00fee_AT002A9488_ROLE02(short, short, short);

void LVL_2_MAKTAR_NEBULA_FUN_002A9488(void) {
 LVL_2_MAKTAR_NEBULA_F6b0741c38bf00fee_AT002A9488_ROLE01(LVL_2_MAKTAR_NEBULA_F6b0741c38bf00fee_AT002A9488_ROLE00[1],0,1,0x32);
 LVL_2_MAKTAR_NEBULA_F6b0741c38bf00fee_AT002A9488_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_2_MAKTAR_NEBULA_F28c929cadd7aca24_AT002D58F0_ROLE00;

void LVL_2_MAKTAR_NEBULA_FUN_002D58F0(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_2_MAKTAR_NEBULA_F28c929cadd7aca24_AT002D58F0_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_2_MAKTAR_NEBULA_F28c929cadd7aca24_AT002D58F0_ROLE00.selected = selected;
            LVL_2_MAKTAR_NEBULA_F28c929cadd7aca24_AT002D58F0_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_2_MAKTAR_NEBULA_F5fc519c90e0e763a_AT002D6BF8_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_2_MAKTAR_NEBULA_FUN_002D6BF8(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_2_MAKTAR_NEBULA_F5fc519c90e0e763a_AT002D6BF8_ROLE00(-value);
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


void LVL_2_MAKTAR_NEBULA_FUN_00387410(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[139];

void LVL_2_MAKTAR_NEBULA_FUN_00394038(void)
{
    int i;
    LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[138] = 5;
    LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[137] = 0;
    LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[i] = 0;
        LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_2_MAKTAR_NEBULA_F03c444112283bc5f_AT00394038_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_2_MAKTAR_NEBULA_FUN_002A81F0(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_2_MAKTAR_NEBULA_FUN_002D7430(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_2_MAKTAR_NEBULA_FUN_002DF8E8(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_2_MAKTAR_NEBULA_FUN_002DF8F0(void) {
    return 1;
}


extern void LVL_2_MAKTAR_NEBULA_QWEN_11a4c157d102_AT002F8088_ROLE000(unsigned char *);

void LVL_2_MAKTAR_NEBULA_FUN_002F8088(void *owner)
{
    LVL_2_MAKTAR_NEBULA_QWEN_11a4c157d102_AT002F8088_ROLE000(owner);
}



void LVL_2_MAKTAR_NEBULA_QWEN_407ee6f17a73_AT002F8128_ROLE001(int);
void LVL_2_MAKTAR_NEBULA_QWEN_407ee6f17a73_AT002F8128_ROLE000(void*);

void LVL_2_MAKTAR_NEBULA_FUN_002F8128(void *param)
{
  LVL_2_MAKTAR_NEBULA_QWEN_407ee6f17a73_AT002F8128_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_2_MAKTAR_NEBULA_QWEN_407ee6f17a73_AT002F8128_ROLE000(param);
}


void LVL_2_MAKTAR_NEBULA_QWEN_5696fcf76f0c_AT00315BF0_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_2_MAKTAR_NEBULA_FUN_00315BF0(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_5696fcf76f0c_AT00315BF0_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


unsigned int LVL_2_MAKTAR_NEBULA_FUN_00332250(unsigned int param_1)
{
    unsigned int inner_ptr;
    unsigned int result;

    inner_ptr = *(unsigned int *)(param_1 + 0x68);
    result = *(unsigned int *)(inner_ptr + 0x18);
    return result;
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
extern void LVL_2_MAKTAR_NEBULA_QWEN_523e38f49b74_AT0034D5F8_ROLE000(long);
extern void LVL_2_MAKTAR_NEBULA_QWEN_523e38f49b74_AT0034D5F8_ROLE002(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_523e38f49b74_AT0034D5F8_ROLE001(void);

int LVL_2_MAKTAR_NEBULA_FUN_0034D5F8(void)
{
  LVL_2_MAKTAR_NEBULA_QWEN_523e38f49b74_AT0034D5F8_ROLE000(0);
  LVL_2_MAKTAR_NEBULA_QWEN_523e38f49b74_AT0034D5F8_ROLE002();
  LVL_2_MAKTAR_NEBULA_QWEN_523e38f49b74_AT0034D5F8_ROLE001();
  return 0;
}


unsigned long long LVL_2_MAKTAR_NEBULA_FUN_0034D9F8(void);

extern void LVL_2_MAKTAR_NEBULA_QWEN_f47929f95774_AT0034D9F8_ROLE000(long);
extern void LVL_2_MAKTAR_NEBULA_QWEN_f47929f95774_AT0034D9F8_ROLE002(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_f47929f95774_AT0034D9F8_ROLE001(void);

unsigned long long LVL_2_MAKTAR_NEBULA_FUN_0034D9F8(void)
{
  LVL_2_MAKTAR_NEBULA_QWEN_f47929f95774_AT0034D9F8_ROLE000(0);
  LVL_2_MAKTAR_NEBULA_QWEN_f47929f95774_AT0034D9F8_ROLE002();
  LVL_2_MAKTAR_NEBULA_QWEN_f47929f95774_AT0034D9F8_ROLE001();
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
extern void LVL_2_MAKTAR_NEBULA_QWEN_cb305b1f1210_AT0034DC98_ROLE000(long);
extern void LVL_2_MAKTAR_NEBULA_QWEN_cb305b1f1210_AT0034DC98_ROLE002(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_cb305b1f1210_AT0034DC98_ROLE001(void);

long long LVL_2_MAKTAR_NEBULA_FUN_0034DC98(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_cb305b1f1210_AT0034DC98_ROLE000(0LL);
    LVL_2_MAKTAR_NEBULA_QWEN_cb305b1f1210_AT0034DC98_ROLE002();
    LVL_2_MAKTAR_NEBULA_QWEN_cb305b1f1210_AT0034DC98_ROLE001();
    return 0LL;
}


extern void LVL_2_MAKTAR_NEBULA_QWEN_c23b3406f982_AT00353090_ROLE000(long);
extern void LVL_2_MAKTAR_NEBULA_QWEN_c23b3406f982_AT00353090_ROLE002(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_c23b3406f982_AT00353090_ROLE001(void);

int LVL_2_MAKTAR_NEBULA_FUN_00353090(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_c23b3406f982_AT00353090_ROLE000(0);
    LVL_2_MAKTAR_NEBULA_QWEN_c23b3406f982_AT00353090_ROLE002();
    LVL_2_MAKTAR_NEBULA_QWEN_c23b3406f982_AT00353090_ROLE001();
    return 0;
}


void LVL_2_MAKTAR_NEBULA_QWEN_68cf9ebb5ee2_AT00353200_ROLE000(long);
void LVL_2_MAKTAR_NEBULA_QWEN_68cf9ebb5ee2_AT00353200_ROLE001(void);
void LVL_2_MAKTAR_NEBULA_QWEN_68cf9ebb5ee2_AT00353200_ROLE002(void);

unsigned long long LVL_2_MAKTAR_NEBULA_FUN_00353200(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_68cf9ebb5ee2_AT00353200_ROLE000(0);
    LVL_2_MAKTAR_NEBULA_QWEN_68cf9ebb5ee2_AT00353200_ROLE002();
    LVL_2_MAKTAR_NEBULA_QWEN_68cf9ebb5ee2_AT00353200_ROLE001();
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

extern void LVL_2_MAKTAR_NEBULA_QWEN_68f040eb20c9_AT003532B8_ROLE000(long arg0);
extern void LVL_2_MAKTAR_NEBULA_QWEN_68f040eb20c9_AT003532B8_ROLE002(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_68f040eb20c9_AT003532B8_ROLE001(void);

unsigned long long LVL_2_MAKTAR_NEBULA_FUN_003532B8(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_68f040eb20c9_AT003532B8_ROLE000(0);
    LVL_2_MAKTAR_NEBULA_QWEN_68f040eb20c9_AT003532B8_ROLE002();
    LVL_2_MAKTAR_NEBULA_QWEN_68f040eb20c9_AT003532B8_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_2_MAKTAR_NEBULA_QWEN_92ea7c2f4a5c_AT00353668_ROLE000(long);
extern void LVL_2_MAKTAR_NEBULA_QWEN_92ea7c2f4a5c_AT00353668_ROLE001(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_92ea7c2f4a5c_AT00353668_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_2_MAKTAR_NEBULA_FUN_00353668(void)
{
  LVL_2_MAKTAR_NEBULA_QWEN_92ea7c2f4a5c_AT00353668_ROLE000(0);
  LVL_2_MAKTAR_NEBULA_QWEN_92ea7c2f4a5c_AT00353668_ROLE002();
  LVL_2_MAKTAR_NEBULA_QWEN_92ea7c2f4a5c_AT00353668_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_2_MAKTAR_NEBULA_QWEN_d01c568afacc_AT00353768_ROLE000(long param_1);
extern void LVL_2_MAKTAR_NEBULA_QWEN_d01c568afacc_AT00353768_ROLE001(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_d01c568afacc_AT00353768_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_2_MAKTAR_NEBULA_FUN_00353768(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_d01c568afacc_AT00353768_ROLE000(0);
    LVL_2_MAKTAR_NEBULA_QWEN_d01c568afacc_AT00353768_ROLE002();
    LVL_2_MAKTAR_NEBULA_QWEN_d01c568afacc_AT00353768_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_2_MAKTAR_NEBULA_QWEN_093381cf82c3_AT00355918_ROLE000(long);
extern void LVL_2_MAKTAR_NEBULA_QWEN_093381cf82c3_AT00355918_ROLE002(void);
extern void LVL_2_MAKTAR_NEBULA_QWEN_093381cf82c3_AT00355918_ROLE001(void);

long long LVL_2_MAKTAR_NEBULA_FUN_00355918(void)
{
    LVL_2_MAKTAR_NEBULA_QWEN_093381cf82c3_AT00355918_ROLE000(0);
    LVL_2_MAKTAR_NEBULA_QWEN_093381cf82c3_AT00355918_ROLE002();
    LVL_2_MAKTAR_NEBULA_QWEN_093381cf82c3_AT00355918_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_2_MAKTAR_NEBULA_FUN_0035F5E0(void) {
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



extern QwenRecovery_4ecc6b5a4034_ExtraResetRecord164 LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE000[100];
extern QwenRecovery_4ecc6b5a4034_u32 LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[139];

void LVL_2_MAKTAR_NEBULA_FUN_003915A8(void)
{
    int i;
    for (i = 99; i >= 0; --i)
        LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE000[i].word = 0;
    LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[138] = 5;
    LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[137] = 0;
    LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[i] = 0;
        LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_2_MAKTAR_NEBULA_QWEN_4ecc6b5a4034_AT003915A8_ROLE001[i + 128] = 0;
}



void LVL_2_MAKTAR_NEBULA_FUN_00457790(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_2_MAKTAR_NEBULA_FUN_00457798(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_2_MAKTAR_NEBULA_FUN_004578F0(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_2_MAKTAR_NEBULA_FUN_00457A40(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_2_MAKTAR_NEBULA_FUN_00457B10(unsigned long param_1)
{
  return param_1;
}


extern void LVL_2_MAKTAR_NEBULA_QWEN_df8fc8ba49cf_AT0045A4D8_ROLE000(unsigned char *owner, int value);

void LVL_2_MAKTAR_NEBULA_FUN_0045A4D8(unsigned char *owner, int value)
{
    LVL_2_MAKTAR_NEBULA_QWEN_df8fc8ba49cf_AT0045A4D8_ROLE000(owner + 8, value);
}



void* LVL_2_MAKTAR_NEBULA_FUN_0045EED8(void* param_1);

extern void LVL_2_MAKTAR_NEBULA_QWEN_f353c206e726_AT0045EED8_ROLE000(int);
extern unsigned long long LVL_2_MAKTAR_NEBULA_QWEN_f353c206e726_AT0045EED8_ROLE001(unsigned long long);

void* LVL_2_MAKTAR_NEBULA_FUN_0045EED8(void* param_1)
{
  LVL_2_MAKTAR_NEBULA_QWEN_f353c206e726_AT0045EED8_ROLE000((int)param_1 + 8);
  LVL_2_MAKTAR_NEBULA_QWEN_f353c206e726_AT0045EED8_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_2_MAKTAR_NEBULA_QWEN_f2f9288a4e34_AT0045F078_ROLE000(int);

int LVL_2_MAKTAR_NEBULA_FUN_0045F078(int owner)
{
    int result;
    result = LVL_2_MAKTAR_NEBULA_QWEN_f2f9288a4e34_AT0045F078_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_2_MAKTAR_NEBULA_QWEN_e41cd63258f5_AT0045F0B0_ROLE000(unsigned char *);

int LVL_2_MAKTAR_NEBULA_FUN_0045F0B0(int owner)
{
    return LVL_2_MAKTAR_NEBULA_QWEN_e41cd63258f5_AT0045F0B0_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_2_MAKTAR_NEBULA_QWEN_f29f80950ce7_AT004627D0_ROLE000(int);

void LVL_2_MAKTAR_NEBULA_FUN_004627D0(int param_1)
{
  LVL_2_MAKTAR_NEBULA_QWEN_f29f80950ce7_AT004627D0_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_2_MAKTAR_NEBULA_QWEN_bf824305b9f7_AT004630E8_ROLE000(unsigned char *owner, float *records);

void LVL_2_MAKTAR_NEBULA_FUN_004630E8(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_2_MAKTAR_NEBULA_QWEN_bf824305b9f7_AT004630E8_ROLE000(owner + 0x188, records);
}



void LVL_2_MAKTAR_NEBULA_QWEN_61faeca45963_AT004633B0_ROLE000(int param_1);

void LVL_2_MAKTAR_NEBULA_FUN_004633B0(int param_1)
{
  LVL_2_MAKTAR_NEBULA_QWEN_61faeca45963_AT004633B0_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_2_MAKTAR_NEBULA_FUN_00463518(unsigned long param_1)
{
  return param_1;
}


extern int LVL_2_MAKTAR_NEBULA_QWEN_6add11b33414_AT004642C8_ROLE000(unsigned char *owner);

int LVL_2_MAKTAR_NEBULA_FUN_004642C8(unsigned char *owner)
{
    return LVL_2_MAKTAR_NEBULA_QWEN_6add11b33414_AT004642C8_ROLE000(owner + 0x298);
}



extern void LVL_2_MAKTAR_NEBULA_QWEN_de961518de48_AT004642E8_ROLE000(unsigned char *owner);

void LVL_2_MAKTAR_NEBULA_FUN_004642E8(unsigned char *owner)
{
    LVL_2_MAKTAR_NEBULA_QWEN_de961518de48_AT004642E8_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_2_MAKTAR_NEBULA_QWEN_7ba3cdeeb16b_AT00468C08_ROLE000(unsigned long long);

unsigned long long LVL_2_MAKTAR_NEBULA_FUN_00468C08(unsigned long long value)
{
    LVL_2_MAKTAR_NEBULA_QWEN_7ba3cdeeb16b_AT00468C08_ROLE000(value);
    return value;
}



void LVL_2_MAKTAR_NEBULA_FUN_00468E50(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_2_MAKTAR_NEBULA_FUN_00469EB0(unsigned long param_1)
{
  return param_1;
}


void LVL_2_MAKTAR_NEBULA_FUN_0046A1F0(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_2_MAKTAR_NEBULA_FUN_0046A390(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_2_MAKTAR_NEBULA_FUN_0046A3D8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_2_MAKTAR_NEBULA_FUN_0046F330(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_2_MAKTAR_NEBULA_FUN_00471420(void) {
    return 1;
}


extern int LVL_2_MAKTAR_NEBULA_QWEN_cc83cb329fcb_AT00472C60_ROLE000(unsigned char *);

int LVL_2_MAKTAR_NEBULA_FUN_00472C60(int *owner)
{
    int result;
    long status;
    status = LVL_2_MAKTAR_NEBULA_QWEN_cc83cb329fcb_AT00472C60_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_003AD208(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_003BD7F8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_003C8998(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_003E62F0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_0040F210(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_0041AC28(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_0043D6F0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(char *local, char *first, char *second);
extern void LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_2_MAKTAR_NEBULA_FUN_00443CA8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1FA0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_2_MAKTAR_NEBULA_Fc936841d_FUN_002F1F70((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_00456BC8(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(char *p, int v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(char *p, float v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(char *p, float v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(char *p, float v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(char *p, float v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(char *p, float v);
extern void LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(char *p, float v);

void LVL_2_MAKTAR_NEBULA_FUN_0046D8F0(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_00456BC8(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(p1, 1);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(p2, 1);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(p3, 1);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(p4, 1);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(p5, 1);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A398(p6, 1);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_2_MAKTAR_NEBULA_F01bd4546_FUN_0046A3E0(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_00382CB8(int a, int b);
extern void LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002E98F8(int a);
extern void *LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(int id);
extern int LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(void *p, int i);
extern void LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC538(void);
extern void LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC9D0(int a, int b, long c, void *d, int e);
extern void LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC528(void);
extern void LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002E9A18(void);

int LVL_2_MAKTAR_NEBULA_FUN_00359B70(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_00382CB8(66, 68);
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_00382CB8(71, 11);
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002E98F8(0);
    min = LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11613), -1);
    v = LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC5B0(LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC538();
    off = count - 6;
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC9D0(slot, off, 0x80FFA888L, LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11613), -1);
    off += count;
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC9D0(slot, off, 0x80FFA888L, LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11625), -1);
    off += count;
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC9D0(slot, off, 0x80FFA888L, LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11626), -1);
    off += count;
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC9D0(slot, off, 0x80FFA888L, LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11627), -1);
    off += count;
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC9D0(slot, off, 0x80FFA888L, LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002F8570(11599), -1);
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002EC528();
    LVL_2_MAKTAR_NEBULA_F70997ba9_FUN_002E9A18();
    return 2;
}
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);

void LVL_2_MAKTAR_NEBULA_FUN_003ACDD0(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[0], v[3]);
        v[1] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[1], v[4]);
        v[2] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[2], v[5]);
        v[9] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[9], v[12]);
        v[10] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[10], v[13]);
        v[11] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[11], v[14]);
        v[20] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(v[0]) * v[6];
        v[21] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[1]) * v[7];
        v[22] = -LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[2]) * v[8];
        v[24] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(v[9]) * v[15];
        v[25] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[10]) * v[16];
        v[26] = -LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(float, float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);
extern float LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(float);

void LVL_2_MAKTAR_NEBULA_FUN_0040EDD8(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[0], v[3]);
        v[1] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[1], v[4]);
        v[2] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[2], v[5]);
        v[9] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[9], v[12]);
        v[10] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[10], v[13]);
        v[11] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F3000(v[11], v[14]);
        v[20] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(v[0]) * v[6];
        v[21] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[1]) * v[7];
        v[22] = -LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[2]) * v[8];
        v[24] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2580(v[9]) * v[15];
        v[25] = LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[10]) * v[16];
        v[26] = -LVL_2_MAKTAR_NEBULA_F307ea0ee_FUN_002F2598(v[11]) * v[17];
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


extern float LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_0031E6E8(float value);
extern void LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00324DA8(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00391268(void *owner, void *out);

void LVL_2_MAKTAR_NEBULA_FUN_00391180(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00324DA8((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_0031E6E8(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00391268(self, (char *)child + 48);

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


extern float LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_0031E6E8(float value);
extern void LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00324DA8(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00393D20(void *owner, void *out);

void LVL_2_MAKTAR_NEBULA_FUN_00393C38(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00324DA8((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_0031E6E8(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_2_MAKTAR_NEBULA_F9e2cd219_FUN_00393D20(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
extern void LVL_2_MAKTAR_NEBULA_F6df9730c_FUN_002F1DB8(char *dst, int *src, int count);

void LVL_2_MAKTAR_NEBULA_FUN_0030CBE8(char *dst, unsigned char *src)
{
    int table[256];
    int tmp[16];
    int *q;
    int i, j, k;

    for (i = 0; i < 256; i++) {
        table[i] = 0;
        if (i & 0x01) table[i] = 0x0000000f;
        if (i & 0x02) table[i] |= 0x000000f0;
        if (i & 0x04) table[i] |= 0x00000f00;
        if (i & 0x08) table[i] |= 0x0000f000;
        if (i & 0x10) table[i] |= 0x000f0000;
        if (i & 0x20) table[i] |= 0x00f00000;
        if (i & 0x40) table[i] |= 0x0f000000;
        if (i & 0x80) table[i] |= 0xf0000000;
    }
    for (j = 0; j < 128; j++) {
        char *next;
        q = tmp;
        next = dst + 64;
        for (k = 15; k >= 0; k--) {
            *q++ = table[*src++];
        }
        LVL_2_MAKTAR_NEBULA_F6df9730c_FUN_002F1DB8(dst, tmp, 64);
        dst = next;
        LVL_2_MAKTAR_NEBULA_F6df9730c_FUN_002F1DB8(dst, tmp, 64);
        dst += 64;
        LVL_2_MAKTAR_NEBULA_F6df9730c_FUN_002F1DB8(dst, tmp, 64);
        dst += 64;
        LVL_2_MAKTAR_NEBULA_F6df9730c_FUN_002F1DB8(dst, tmp, 64);
        dst += 64;
    }
}
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DFAB8(void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DF8F8(void *);
extern int LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DFCD0(void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002F2270(float, void *, void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002F20A0(void *, void *, void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002E0258(void *, void *, void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_003296B8(void *, void *, int);
extern int LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002E0378(void *, void *, void *, float, float);
extern int LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_00366B30(int, int, void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DFF38(void *, int, void *);
extern int LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_00366B30(int, int, void *);
extern void LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_00366F10(int, void *);
extern char LVL_2_MAKTAR_NEBULA_Fdb046c5d_D_001C0980[];

int LVL_2_MAKTAR_NEBULA_FUN_002DF4E0(char *obj)
{
    float buf[4];
    float out[4];
    char *p;
    char *s1;
    unsigned char k;
    int r;
    int s4;
    int s5;

    p = *(char **)(obj + 104);
    k = *(unsigned char *)(p + 92);
    if (k >= 3)
        return 1;
    if (k != 0)
        LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DFAB8(obj);
    else
        LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DF8F8(obj);

    s5 = LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DFCD0(obj);
    if (s5 == -1)
        return 0;

    s1 = LVL_2_MAKTAR_NEBULA_Fdb046c5d_D_001C0980;
    s4 = -1;
    LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002F2270(1.0f, s1, s1);
    LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002F20A0(buf, s1, p);
    LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002E0258(obj, p + 16, buf);
    LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_003296B8(obj + 16, out, 1);
    r = LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002E0378(p, s1, out, *(float *)(p + 56), *(float *)(p + 60));
    if (r == 0) {
        if (*(unsigned char *)(p + 94) != 255 && *(unsigned char *)(p + 92) == 0)
            s4 = LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_00366B30(*(unsigned char *)(p + 94), 0, obj);
        LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_002DFF38(obj, s5, out);
    } else {
        if (*(unsigned char *)(p + 94) != 255)
            s4 = LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_00366B30(*(unsigned char *)(p + 94), 0, obj);
    }
    if (s4 != -1)
        LVL_2_MAKTAR_NEBULA_Fdb046c5d_FUN_00366F10(s4, p + 32);
    return 0;
}
extern void LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F1FF8(char *out, void *source, float value);
extern void LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_0044BC00(char *buffer, int mode);
extern void LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_0044BAA0(int value, char *buffer);
extern void LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F1F70(char *first, char *second, char *third);
extern float LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F2078(void *owner, char *buffer);

extern int LVL_2_MAKTAR_NEBULA_F7242f0a4_D_001BA560[];

void LVL_2_MAKTAR_NEBULA_FUN_0044C2C8(void *owner, int flag, float value)
{
    char buffer[32];
    int *root = LVL_2_MAKTAR_NEBULA_F7242f0a4_D_001BA560;

    LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F1FF8(buffer, root + 8, value);
    if (flag)
        LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_0044BC00(buffer + 16, 1);
    else
        LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_0044BAA0(root[-4], buffer + 16);
    LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F1F70(buffer, buffer, buffer + 16);
    LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F1FF8(owner, root + 12, LVL_2_MAKTAR_NEBULA_F7242f0a4_FUN_002F2078(root + 12, buffer));
}
extern char LVL_2_MAKTAR_NEBULA_F45821cfb_D_001C8280[];
extern void LVL_2_MAKTAR_NEBULA_F45821cfb_FUN_002F1F38(char *);

void LVL_2_MAKTAR_NEBULA_FUN_0046F958(void)
{
    float *p = (float *)LVL_2_MAKTAR_NEBULA_F45821cfb_D_001C8280;

    p[224] = 0.4f;
    p[225] = 0.8f;
    p[226] = 1.2f;
    *(int *)&p[227] = 0;
    p[228] = 0.577f;
    p[229] = 0.577f;
    p[230] = -0.577f;
    *(int *)&p[231] = 0;
    LVL_2_MAKTAR_NEBULA_F45821cfb_FUN_002F1F38((char *)&p[232]);
    LVL_2_MAKTAR_NEBULA_F45821cfb_FUN_002F1F38((char *)&p[236]);
}
extern char LVL_2_MAKTAR_NEBULA_Fd8e166aa_D_001BD940[];

void LVL_2_MAKTAR_NEBULA_FUN_002E6C50(void)
{
    char *g = LVL_2_MAKTAR_NEBULA_Fd8e166aa_D_001BD940;
    int i = 0;
    int *table = *(int **)(g + 8);
    char *p = (char *)table + table[*(unsigned char *)0x1A7BB4];
    int n = *(int *)p;

    *(char **)(g + 12) = p + 8;
    *(int *)(g + 16) = n;
    for (i = 0; i < *(int *)(g + 16); i++) {
        char *q = *(char **)(g + 12);
        char *r = q - 8;

        *(int *)(q + i * 16) = *(int *)(q + i * 16) + (int)r;
    }
}
