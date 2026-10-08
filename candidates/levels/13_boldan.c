typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_13_BOLDAN_D_001A8EB0;
void LVL_13_BOLDAN_FUN_002E23C8(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_13_BOLDAN_FUN_002FE420(s32 index) {
    NativeTable20 values=LVL_13_BOLDAN_D_001A8EB0;
    return values.items[index];
}

u32 LVL_13_BOLDAN_FUN_002B8DD8(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_13_BOLDAN_D_0018C0B4;
u32 LVL_13_BOLDAN_FUN_002E1370(void) {
    return LVL_13_BOLDAN_D_0018C0B4;
}

s32 LVL_13_BOLDAN_FUN_002EE558(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_13_BOLDAN_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_13_BOLDAN_FUN_002B8C70(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_13_BOLDAN_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_13_BOLDAN_FUN_002B8CA8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_13_BOLDAN_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_13_BOLDAN_FUN_002B98C8(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_13_BOLDAN_FUN_002E2120(f32, f32, f32, f32, s32, s32);

void LVL_13_BOLDAN_FUN_002E1BC8(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_13_BOLDAN_FUN_002E2120(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_13_BOLDAN_FUN_002FAC78(int width, int height, int address, int mode);
extern void LVL_13_BOLDAN_FUN_00384E68(unsigned int reg, unsigned long value);
extern void LVL_13_BOLDAN_FUN_002FAFE0(int width, int height);

void LVL_13_BOLDAN_FUN_002F02F8(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_13_BOLDAN_FUN_002FAC78(width, height, address, 1);
    LVL_13_BOLDAN_FUN_00384E68(0x47, 0x30000UL);
    LVL_13_BOLDAN_FUN_00384E68(0x42, 0x8000000044UL);
    LVL_13_BOLDAN_FUN_002FAFE0(0x100, 0x100);
    LVL_13_BOLDAN_FUN_00384E68(0x42, 0x8000000044UL);
}

s32 LVL_13_BOLDAN_FUN_002B8CE0(s32 index) {
 s32 value=LVL_13_BOLDAN_FUN_002B8CA8(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_13_BOLDAN_FUN_002B8D18(s32 index) {
 return LVL_13_BOLDAN_FUN_002B8CA8(index)==47;
}

s32 LVL_13_BOLDAN_FUN_002BE1D0(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

f32 LVL_13_BOLDAN_FUN_00422C90(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_13_BOLDAN_FUN_0034B260(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_13_BOLDAN_D_00288A00[13];
void LVL_13_BOLDAN_FUN_00301528(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_13_BOLDAN_D_00288A00[i].key == key) break;
    }
    if (i < 13) {
        LVL_13_BOLDAN_D_00288A00[i].fields[9] = value;
        if (LVL_13_BOLDAN_D_00288A00[i].busy == 0)
            LVL_13_BOLDAN_D_00288A00[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_13_BOLDAN_D_001395B8[];
u32 LVL_13_BOLDAN_FUN_0030FC08(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_13_BOLDAN_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_13_BOLDAN_D_00231E40[];
s32 LVL_13_BOLDAN_FUN_00387748(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_13_BOLDAN_D_00231E40;
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
extern int LVL_13_BOLDAN_D_0022E700[];
extern ListOverrideObject *LVL_13_BOLDAN_D_00227200[];
extern ListOverridePair LVL_13_BOLDAN_D_0022E100[];
void LVL_13_BOLDAN_FUN_00374678(void)
{
    int *selected = LVL_13_BOLDAN_D_0022E700;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_13_BOLDAN_D_00227200[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_13_BOLDAN_D_0022E100[row->key];
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
extern NativeObjectSearchRecord32 LVL_13_BOLDAN_D_00254AB0[];
s32 LVL_13_BOLDAN_FUN_003FA838(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_13_BOLDAN_D_00254AB0[index].field14 == object) {
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

s32 LVL_13_BOLDAN_FUN_002E1230(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_13_BOLDAN_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_13_BOLDAN_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_13_BOLDAN_D_00189E20)->mode == 0x31) {
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
extern void *LVL_13_BOLDAN_D_0018C0B0;
extern void *LVL_13_BOLDAN_D_0018B134;
extern void *LVL_13_BOLDAN_D_0018B040;
void *LVL_13_BOLDAN_FUN_002AFF68(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_13_BOLDAN_D_0018C0B0;
    if (kind == 1)
        return LVL_13_BOLDAN_D_0018B134;
    if (kind == 6)
        return LVL_13_BOLDAN_D_0018B040;
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
extern unsigned char LVL_13_BOLDAN_D_00139568[];
extern MappedClassEntry LVL_13_BOLDAN_D_0026D270[];
unsigned int LVL_13_BOLDAN_FUN_002FE0F8(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_13_BOLDAN_D_0026D270[LVL_13_BOLDAN_D_00139568[i]];
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
extern s32 LVL_13_BOLDAN_FUN_002F8480(s32 value);
NativeCompactHeader *LVL_13_BOLDAN_FUN_00389B40(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_13_BOLDAN_FUN_002F8480(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_13_BOLDAN_FUN_002F8480(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_13_BOLDAN_D_001A79F0;
s32 LVL_13_BOLDAN_FUN_002E11B0(void) {
    s32 found = 0;
    if (LVL_13_BOLDAN_D_001A79F0 == 25 || LVL_13_BOLDAN_D_001A79F0 == 5 ||
        LVL_13_BOLDAN_D_001A79F0 == 10 || LVL_13_BOLDAN_D_001A79F0 == 15) {
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
void LVL_13_BOLDAN_FUN_002DDC58(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_13_BOLDAN_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_13_BOLDAN_FUN_002DDC90(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_13_BOLDAN_D_00189E20;
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
extern ConditionalResetSlot LVL_13_BOLDAN_D_001B9700[8];
void LVL_13_BOLDAN_FUN_002E15F8(void) {
    ConditionalResetSlot *slot = LVL_13_BOLDAN_D_001B9700;
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
extern StateTransitionView LVL_13_BOLDAN_D_001BFA00;
void LVL_13_BOLDAN_FUN_002FDA60(void) {
    if (LVL_13_BOLDAN_D_001BFA00.mode == 7 && LVL_13_BOLDAN_D_001BFA00.state == 1) {
        LVL_13_BOLDAN_D_001BFA00.state = 2;
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
extern DobboFormatRoot396 LVL_13_BOLDAN_D_001C9EA0;
extern const char LVL_13_BOLDAN_D_001A99E0[];
extern const char LVL_13_BOLDAN_D_001A99E8[];
extern const unsigned char *LVL_13_BOLDAN_FUN_002FECB8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_13_BOLDAN_FUN_00313288(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_13_BOLDAN_FUN_002FECB8(LVL_13_BOLDAN_D_001C9EA0.rows[index].text_id);
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
        int key = LVL_13_BOLDAN_D_001C9EA0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_13_BOLDAN_D_0026D270[LVL_13_BOLDAN_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_13_BOLDAN_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_13_BOLDAN_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_13_BOLDAN_FUN_002BE920(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_13_BOLDAN_D_00189E20;
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
extern int LVL_13_BOLDAN_FUN_00368FB0(int, unsigned int, void *);
void LVL_13_BOLDAN_FUN_0044DB88(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_13_BOLDAN_FUN_00368FB0(3, 0, 0);
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
        LVL_13_BOLDAN_FUN_00368FB0(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_13_BOLDAN_FUN_00368FB0(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_13_BOLDAN_FUN_00368FB0(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_13_BOLDAN_D_001B2880[16];
extern u32 LVL_13_BOLDAN_D_001B28C0[16];

int LVL_13_BOLDAN_FUN_0031B620(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_13_BOLDAN_D_001B2880[index] == 0 ||
            LVL_13_BOLDAN_D_001B2880[index] == object) {
            LVL_13_BOLDAN_D_001B2880[index] = object;
            LVL_13_BOLDAN_D_001B28C0[index] = 0;
            return index;
        }
    }
    return -1;
}

typedef struct {
    u8 field00[0x1f0];
    f32 field1F0[4];
    u8 field200[0xe0];
    f32 field2E0;
    u8 field2E4[4];
    f32 field2E8;
} NativeAngleState __attribute__((aligned(16)));
typedef struct {
    u8 field00[0x10];
    f32 field10[4];
    u8 field20[0x48];
    NativeAngleState *field68;
} NativeAngleOwner __attribute__((aligned(16)));
extern void LVL_13_BOLDAN_FUN_002F8770(f32 *output, const f32 *left, const f32 *right);
extern void LVL_13_BOLDAN_FUN_0032E008(NativeAngleOwner *owner, f32 *output, const f32 *input, s32 mode);
extern f32 LVL_13_BOLDAN_FUN_002F8DF8(f32 x, f32 y);
extern f32 LVL_13_BOLDAN_FUN_002F88B8(const f32 *vector);
void LVL_13_BOLDAN_FUN_00426250(NativeAngleOwner *owner) {
    f32 direction[4] __attribute__((aligned(16)));
    NativeAngleState *state = owner->field68;
    f32 angle, xy_length;
    LVL_13_BOLDAN_FUN_002F8770(direction, state->field1F0, owner->field10);
    LVL_13_BOLDAN_FUN_0032E008(owner, direction, direction, 0);
    angle = LVL_13_BOLDAN_FUN_002F8DF8(direction[0], direction[1]);
    state->field2E0 = angle;
    if (0.78539824f < angle) state->field2E0 = 0.78539824f;
    else if (angle < -0.78539824f) state->field2E0 = -0.78539824f;
    xy_length = LVL_13_BOLDAN_FUN_002F88B8(direction);
    angle = -LVL_13_BOLDAN_FUN_002F8DF8(xy_length, direction[2]);
    state->field2E8 = angle;
    if (0.52359885f < angle) state->field2E8 = 0.52359885f;
    else if (angle < -0.52359885f) state->field2E8 = -0.52359885f;
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
extern void LVL_13_BOLDAN_FUN_002E75B8(OozlaAppendObject164 *, int, const float *);
extern void LVL_13_BOLDAN_FUN_002F8770(float *, const float *, const float *);
extern void LVL_13_BOLDAN_FUN_002F8C00(float *, const float *, const float *);
extern void LVL_13_BOLDAN_FUN_002E7900(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_13_BOLDAN_FUN_002E7510(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_13_BOLDAN_FUN_002E75B8(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_13_BOLDAN_FUN_002F8770(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_13_BOLDAN_FUN_002F8C00(difference, difference, &object->transform[0][0]);
        LVL_13_BOLDAN_FUN_002E7900(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_13_BOLDAN_FUN_00453000(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_13_BOLDAN_FUN_00453498(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_13_BOLDAN_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_13_BOLDAN_FUN_00336FB8(void) {
    if (((CallState *)LVL_13_BOLDAN_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_13_BOLDAN_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_13_BOLDAN_D_001A63A8)->active); ((CallState *)LVL_13_BOLDAN_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_13_BOLDAN_FUN_00324950(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_13_BOLDAN_FUN_0030FB88(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_13_BOLDAN_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_13_BOLDAN_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

extern unsigned char D_19B278[];

int LVL_13_BOLDAN_FUN_0032F860(void)
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

typedef struct {
    u8 pad0000[0x2294];
    u32 kind;
    u32 unknown2298;
    u32 mode;
} ResidentFlags2294;

s32 LVL_13_BOLDAN_FUN_002B8D88(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_13_BOLDAN_D_00189E20;

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

extern void LVL_13_BOLDAN_FUN_0031A718(NativeUpdate775View *object);

void LVL_13_BOLDAN_FUN_003A80E0(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_13_BOLDAN_FUN_0031A718(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_13_BOLDAN_FUN_00332488(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_13_BOLDAN_FUN_003294B0(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_13_BOLDAN_FUN_002B2068(void)
{
}


unsigned int LVL_13_BOLDAN_FUN_002E1300(void)
{
    return 0;
}


void LVL_13_BOLDAN_FUN_002F7A78(void)
{
}


void LVL_13_BOLDAN_FUN_002FEA50(void)
{
}


void LVL_13_BOLDAN_FUN_002FF800(void)
{
}


void LVL_13_BOLDAN_FUN_00306CC0(void)
{
}


void LVL_13_BOLDAN_FUN_00306CC8(void)
{
}


void LVL_13_BOLDAN_FUN_0030A8E8(void)
{
}


void LVL_13_BOLDAN_FUN_00313640(void)
{
}


unsigned int LVL_13_BOLDAN_FUN_00378D40(void)
{
    return 0;
}


void LVL_13_BOLDAN_FUN_0037D858(void)
{
}


void LVL_13_BOLDAN_FUN_00384870(void)
{
}


void LVL_13_BOLDAN_FUN_003881F8(void)
{
}


void LVL_13_BOLDAN_FUN_0038B970(void)
{
}


void LVL_13_BOLDAN_FUN_003F7060(void)
{
}


void LVL_13_BOLDAN_FUN_00424720(void)
{
}


void LVL_13_BOLDAN_FUN_004407C0(void)
{
}


void LVL_13_BOLDAN_FUN_00442230(void)
{
}


void LVL_13_BOLDAN_FUN_00442480(void)
{
}


void LVL_13_BOLDAN_FUN_00442978(void)
{
}


void LVL_13_BOLDAN_FUN_0044B1F0(void)
{
}


void LVL_13_BOLDAN_FUN_0044BE48(void)
{
}


void LVL_13_BOLDAN_FUN_00457C50(void)
{
}


void LVL_13_BOLDAN_FUN_00459FE0(void)
{
}


void LVL_13_BOLDAN_FUN_0045B878(void)
{
}
void LVL_13_BOLDAN_FUN_003BFBB0(char *p)
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
void LVL_13_BOLDAN_FUN_003C9D28(char *p)
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
void LVL_13_BOLDAN_FUN_003D7870(char *p)
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
void LVL_13_BOLDAN_FUN_003DE718(char *p)
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
void LVL_13_BOLDAN_FUN_003E1DD8(char *p)
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
void LVL_13_BOLDAN_FUN_003E80B8(char *p)
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
void LVL_13_BOLDAN_FUN_003F2428(char *p)
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
void LVL_13_BOLDAN_FUN_003F37D0(char *p)
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
void LVL_13_BOLDAN_FUN_0040D050(char *p)
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
void LVL_13_BOLDAN_FUN_0040DF38(char *p)
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
void LVL_13_BOLDAN_FUN_004215B0(char *p)
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
void LVL_13_BOLDAN_FUN_0042AA40(char *p)
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

extern u8 LVL_13_BOLDAN_F62e6ff2b_D_00189E20[];
extern u8 LVL_13_BOLDAN_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_13_BOLDAN_FUN_00368BA8(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_13_BOLDAN_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_13_BOLDAN_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_13_BOLDAN_F62e6ff2b_D_00188660;
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

extern u8 LVL_13_BOLDAN_F1c0a2bbf_D_001ADD18[];
extern u8 LVL_13_BOLDAN_F1c0a2bbf_D_001ADD38[];
extern void LVL_13_BOLDAN_F1c0a2bbf_FUN_00115E38(void *, s32, void *);

void LVL_13_BOLDAN_FUN_004407D8(u32 *p, u32 a1, u32 a2, u32 a3)
{
    if (a1 < 4u)
        LVL_13_BOLDAN_F1c0a2bbf_FUN_00115E38(LVL_13_BOLDAN_F1c0a2bbf_D_001ADD18, 37, LVL_13_BOLDAN_F1c0a2bbf_D_001ADD38);
    p[0] = a2;
    p[1] = a3;
    p[2] = a1;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
void LVL_13_BOLDAN_FUN_003AFDD8(char *p)
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
void LVL_13_BOLDAN_FUN_003EAF10(char *p)
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
extern Blob LVL_13_BOLDAN_F6894d7c1_D_001A8E60;
int LVL_13_BOLDAN_FUN_002FDD28(int x)
{
    Blob b;
    int i;
    b = LVL_13_BOLDAN_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_13_BOLDAN_Fafab4c55_FUN_00115DA8(char *, char *, ...);
extern char LVL_13_BOLDAN_Fafab4c55_D_001AD5F0[];
extern char LVL_13_BOLDAN_Fafab4c55_D_001AD600[];
extern char LVL_13_BOLDAN_Fafab4c55_D_001AD608[];

void LVL_13_BOLDAN_FUN_0037F258(char *dst, int value)
{
    if (value > 999999)
        LVL_13_BOLDAN_Fafab4c55_FUN_00115DA8(dst, LVL_13_BOLDAN_Fafab4c55_D_001AD5F0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_13_BOLDAN_Fafab4c55_FUN_00115DA8(dst, LVL_13_BOLDAN_Fafab4c55_D_001AD600, value / 1000, value % 1000);
    else
        LVL_13_BOLDAN_Fafab4c55_FUN_00115DA8(dst, LVL_13_BOLDAN_Fafab4c55_D_001AD608, value);
}
typedef struct {
    char *base;
    unsigned limit;
    unsigned size;
    unsigned cur;
    int count;
    void *free;
} Hdr;

extern int LVL_13_BOLDAN_Fe77c6258_FUN_00115E38(char *, int, char *);
extern char LVL_13_BOLDAN_Fe77c6258_D_001ADD18[];
extern char LVL_13_BOLDAN_Fe77c6258_D_001ADD60[];

void *LVL_13_BOLDAN_FUN_00440860(Hdr *p)
{
    void *q;
    unsigned cur;
    unsigned n;
    char *r;

    if (p->free != 0) {
        q = p->free;
        p->free = *(void **)q;
        p->count++;
        return q;
    }
    cur = p->cur;
    if (p->limit < cur + p->size) {
        LVL_13_BOLDAN_Fe77c6258_FUN_00115E38(LVL_13_BOLDAN_Fe77c6258_D_001ADD18, 83, LVL_13_BOLDAN_Fe77c6258_D_001ADD60);
        return 0;
    }
    r = p->base + cur;
    p->cur = cur + p->size;
    p->count++;
    return r;
}
void LVL_13_BOLDAN_FUN_00391390(char *p)
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
void LVL_13_BOLDAN_FUN_00393A18(char *p)
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
void LVL_13_BOLDAN_FUN_0042D388(char *p)
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

extern Entry *LVL_13_BOLDAN_Fc68ad20a_D_0018C2B8;

int LVL_13_BOLDAN_FUN_002DCA00(int key, int *out)
{
    Entry *e = LVL_13_BOLDAN_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_13_BOLDAN_F55a1acb8_D_001A63A8;
extern short LVL_13_BOLDAN_F55a1acb8_D_001A63AC;
extern int LVL_13_BOLDAN_F55a1acb8_FUN_00133688(void);
extern void LVL_13_BOLDAN_F55a1acb8_FUN_0011AEA0(int);

void LVL_13_BOLDAN_FUN_00338088(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_13_BOLDAN_F55a1acb8_FUN_00133688()) {
        LVL_13_BOLDAN_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_13_BOLDAN_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f6;
    q = LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f18;
    LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f1C;
        LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_13_BOLDAN_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_13_BOLDAN_Fa2dbe766_D_00189E20[];

int LVL_13_BOLDAN_FUN_003D1708(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_13_BOLDAN_Fa2dbe766_D_00189E20;
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
void LVL_13_BOLDAN_FUN_0034FDD8(void)
{
    *(short *)0x001AA802 = (*(unsigned char *)0x001A7BC9) ? 3 : 0;
    *(short *)0x001AA81A = (*(unsigned char *)0x001A7BCA) ? 3 : 0;
    *(short *)0x001AA832 = (*(unsigned char *)0x001A7BCB) ? 3 : 0;
    *(short *)0x001AA84A = (*(unsigned char *)0x001A7BCC) ? 3 : 0;
    *(short *)0x001AA862 = (*(unsigned char *)0x001A7BCE) ? 3 : 0;
}
void LVL_13_BOLDAN_FUN_00415390(char *object)
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
extern char LVL_13_BOLDAN_Fea34650e_D_00189E20[];

void LVL_13_BOLDAN_FUN_002DDAF0(void)
{
    char *b = LVL_13_BOLDAN_Fea34650e_D_00189E20;
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
        char *c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_13_BOLDAN_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_13_BOLDAN_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_13_BOLDAN_FUN_003EB5E8(char *p)
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
void LVL_13_BOLDAN_FUN_0042D320(char *p)
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
extern char LVL_13_BOLDAN_F0be97c76_D_00189E20[];

void LVL_13_BOLDAN_FUN_002B1FE8(void)
{
    char *base = LVL_13_BOLDAN_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_13_BOLDAN_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_13_BOLDAN_Fee2b87d1_D_00152CD0;

s32 LVL_13_BOLDAN_FUN_0030F2C8(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_13_BOLDAN_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_13_BOLDAN_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_13_BOLDAN_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_13_BOLDAN_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
extern int LVL_13_BOLDAN_F954231c4_D_00231E40[][4];

int LVL_13_BOLDAN_FUN_003876E0(int a0, int a1)
{
    int i;
    int r = 1;

    for (i = 0; i < 32; i++)
        if (LVL_13_BOLDAN_F954231c4_D_00231E40[i][1] == a0 && LVL_13_BOLDAN_F954231c4_D_00231E40[i][0] == a1) {
            r = 0;
            break;
        }
    return r;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_13_BOLDAN_F1157be91_D_001A63E8;
extern unsigned char LVL_13_BOLDAN_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_13_BOLDAN_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_13_BOLDAN_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_13_BOLDAN_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_13_BOLDAN_F1157be91_FUN_00133230(void);
extern int LVL_13_BOLDAN_F1157be91_FUN_00132028(void);

int LVL_13_BOLDAN_FUN_00337F10(int a0, int a1, int a2) {
    CdMode mode = LVL_13_BOLDAN_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_13_BOLDAN_F1157be91_D_001A7900[0];
    LVL_13_BOLDAN_F1157be91_D_001A7430[0] = 0;
    LVL_13_BOLDAN_F1157be91_D_001A7434 = 0;
    LVL_13_BOLDAN_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_13_BOLDAN_F1157be91_FUN_00133230();
    LVL_13_BOLDAN_F1157be91_FUN_00132028();
    return 1;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_13_BOLDAN_F4e5bde81_D_00189E20;
extern s32 LVL_13_BOLDAN_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_13_BOLDAN_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_13_BOLDAN_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_13_BOLDAN_FUN_002DDE88(void) {
    s32 result = LVL_13_BOLDAN_F4e5bde81_D_00189E20.field348;
    if (LVL_13_BOLDAN_F4e5bde81_D_001A8FF0 != 0 && LVL_13_BOLDAN_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_13_BOLDAN_F4e5bde81_D_001A8FF4 != 0 || LVL_13_BOLDAN_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_13_BOLDAN_F4e5bde81_D_00189E20.field2294 == 110 && LVL_13_BOLDAN_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_13_BOLDAN_F4e5bde81_D_00189E20.field2294 == 109 || LVL_13_BOLDAN_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_13_BOLDAN_F4e5bde81_D_00189E20.field1497 != 0 && LVL_13_BOLDAN_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_13_BOLDAN_F4e5bde81_D_00189E20.field2294 == 0 && LVL_13_BOLDAN_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_13_BOLDAN_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
/* v2: guard reads p->count, loop counter is a second read (CSE -> copy). */
struct Elem {
    unsigned int f0;
    unsigned char pad0[31];
    unsigned char f35;
    unsigned char pad1[12];
    unsigned int f48;
    unsigned char pad2[28];
};
struct Node {
    struct Elem *elems;
    int count;
};
extern struct Node LVL_13_BOLDAN_F9328256b_D_00226A00 __attribute__((nosda));
extern short LVL_13_BOLDAN_F9328256b_D_00226700[][2];

void LVL_13_BOLDAN_FUN_00372B50(void)
{
    struct Node *p = &LVL_13_BOLDAN_F9328256b_D_00226A00;

    if (p->elems != 0) {
        do {
            struct Elem *e = p->elems;

            if (p->count > 0) {
                int n = p->count;

                do {
                    short *t = LVL_13_BOLDAN_F9328256b_D_00226700[e->f35];
                    if (t[0] != 0) e->f0 = (e->f0 & 0xFFFFC000u) | (unsigned int)t[0];
                    if (t[1] != 0) e->f48 = (e->f48 & 0xFFFFC000u) | (unsigned int)t[1];
                    e++;
                } while (--n);
            }
            p++;
        } while (p->elems != 0);
    }
}
/* Append one 16-byte record to the level's queue at 0x225FD0 and submit it
   through the DMA helper LVL_13_BOLDAN_F0451f37d_FUN_0011AFE0.  The four queue globals live in a
   0x38-byte state block based at 0x1A7240; because they are reached with a
   constant offset from that base (a CONST address), cc1 gives the store a
   two-instruction length and refuses to drop it into the branch delay slot,
   which is what the retail body does as well.  */

typedef struct {
    int start;      /* +0x00 -> 0x1A7240 */
    int end;        /* +0x04 -> 0x1A7244 */
    int pad[10];    /* +0x08 .. +0x2F */
    int cursor;     /* +0x30 -> 0x1A7270 */
    int count;      /* +0x34 -> 0x1A7274 */
} LevelQueue0451f37d;

extern LevelQueue0451f37d LVL_13_BOLDAN_F0451f37d_D_001A7240 __attribute__((sda));
extern char LVL_13_BOLDAN_F0451f37d_D_00225FD0[];
extern int LVL_13_BOLDAN_F0451f37d_FUN_0011AFE0(int *dma, int flag);

int LVL_13_BOLDAN_FUN_003724E8(int p0, int p1, int p2, int p3)
{
    int args[4];
    int n, k;

    if (LVL_13_BOLDAN_F0451f37d_D_001A7240.end - (LVL_13_BOLDAN_F0451f37d_D_001A7240.cursor - LVL_13_BOLDAN_F0451f37d_D_001A7240.start) < p2 * 16)
        return -1;
    if (LVL_13_BOLDAN_F0451f37d_D_001A7240.count == 64)
        return -2;

    args[0] = p0;
    args[1] = LVL_13_BOLDAN_F0451f37d_D_001A7240.cursor;
    args[2] = p1 * 16;
    args[3] = 0;
    LVL_13_BOLDAN_F0451f37d_FUN_0011AFE0(args, 1);

    n = LVL_13_BOLDAN_F0451f37d_D_001A7240.count;
    k = n;
    n = n + 1;
    LVL_13_BOLDAN_F0451f37d_D_001A7240.count = n;
    *(int *)(LVL_13_BOLDAN_F0451f37d_D_00225FD0 + k * 16) = LVL_13_BOLDAN_F0451f37d_D_001A7240.cursor;
    *(int *)(LVL_13_BOLDAN_F0451f37d_D_00225FD0 + k * 16 + 4) = p2;
    *(int *)(LVL_13_BOLDAN_F0451f37d_D_00225FD0 + k * 16 + 8) = p3;
    LVL_13_BOLDAN_F0451f37d_D_001A7240.cursor = LVL_13_BOLDAN_F0451f37d_D_001A7240.cursor + p2 * 16;
    return k;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_13_BOLDAN_Fabf21065e887d7a9_AT00319190_ROLE00;

int LVL_13_BOLDAN_FUN_00319190(void)
{
    if ((LVL_13_BOLDAN_Fabf21065e887d7a9_AT00319190_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_13_BOLDAN_Fabf21065e887d7a9_AT00319190_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_13_BOLDAN_Fabf21065e887d7a9_AT00319190_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_13_BOLDAN_Fabf21065e887d7a9_AT00319190_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_13_BOLDAN_F5b4b17178a13f443_AT0032CBA0_ROLE00(void *object);

void LVL_13_BOLDAN_FUN_0032CBA0(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_13_BOLDAN_F5b4b17178a13f443_AT0032CBA0_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_13_BOLDAN_Facdcf1600d770d3b_AT00369378_ROLE00[];

void LVL_13_BOLDAN_FUN_00369378(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_13_BOLDAN_Facdcf1600d770d3b_AT00369378_ROLE00;
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


extern void LVL_13_BOLDAN_Fb1b523716b470b36_AT003904F0_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_13_BOLDAN_Fb1b523716b470b36_AT003904F0_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_13_BOLDAN_FUN_003904F0(void *object)
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
    LVL_13_BOLDAN_Fb1b523716b470b36_AT003904F0_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_13_BOLDAN_Fb1b523716b470b36_AT003904F0_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_13_BOLDAN_Fb1b523716b470b36_AT00392F98_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_13_BOLDAN_Fb1b523716b470b36_AT00392F98_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_13_BOLDAN_FUN_00392F98(void *object)
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
    LVL_13_BOLDAN_Fb1b523716b470b36_AT00392F98_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_13_BOLDAN_Fb1b523716b470b36_AT00392F98_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003A1FC8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_003A1FC8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003A1FC8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003A6F50_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_003A6F50(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003A6F50_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_13_BOLDAN_F86f665335d9cb905_AT003C88B8_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_13_BOLDAN_FUN_003C88B8(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_13_BOLDAN_F86f665335d9cb905_AT003C88B8_ROLE00(owner, owner->context_68);
}


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003CEFD0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_003CEFD0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003CEFD0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003D4648_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_003D4648(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003D4648_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003EBA10_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_003EBA10(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003EBA10_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003F02F0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_003F02F0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT003F02F0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT00400F30_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_13_BOLDAN_FUN_00400F30(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_13_BOLDAN_F9cdc323a4d0c2fbd_AT00400F30_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_13_BOLDAN_F6af85cabb56d3b41_AT0043C920_ROLE00;

void LVL_13_BOLDAN_FUN_0043C920(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_13_BOLDAN_F6af85cabb56d3b41_AT0043C920_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_13_BOLDAN_F6af85cabb56d3b41_AT0043C920_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_13_BOLDAN_FUN_0043F530(float factor, void *context,
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


void LVL_13_BOLDAN_FUN_0043F6E0(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_13_BOLDAN_FUN_00444A40(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_13_BOLDAN_FUN_0044CA68(float first, float second, float **cell)
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


extern unsigned char LVL_13_BOLDAN_F79744baad5ad7f65_AT00453ED8_ROLE00[];

void LVL_13_BOLDAN_FUN_00453ED8(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_13_BOLDAN_F79744baad5ad7f65_AT00453ED8_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_13_BOLDAN_F6b0741c38bf00fee_AT002B1108_ROLE00[];
extern void LVL_13_BOLDAN_F6b0741c38bf00fee_AT002B1108_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_13_BOLDAN_F6b0741c38bf00fee_AT002B1108_ROLE02(short, short, short);

void LVL_13_BOLDAN_FUN_002B1108(void) {
 LVL_13_BOLDAN_F6b0741c38bf00fee_AT002B1108_ROLE01(LVL_13_BOLDAN_F6b0741c38bf00fee_AT002B1108_ROLE00[1],0,1,0x32);
 LVL_13_BOLDAN_F6b0741c38bf00fee_AT002B1108_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_13_BOLDAN_F28c929cadd7aca24_AT002DCEF8_ROLE00;

void LVL_13_BOLDAN_FUN_002DCEF8(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_13_BOLDAN_F28c929cadd7aca24_AT002DCEF8_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_13_BOLDAN_F28c929cadd7aca24_AT002DCEF8_ROLE00.selected = selected;
            LVL_13_BOLDAN_F28c929cadd7aca24_AT002DCEF8_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_13_BOLDAN_F5fc519c90e0e763a_AT002DE200_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_13_BOLDAN_FUN_002DE200(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_13_BOLDAN_F5fc519c90e0e763a_AT002DE200_ROLE00(-value);
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


void LVL_13_BOLDAN_FUN_003895C0(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}
