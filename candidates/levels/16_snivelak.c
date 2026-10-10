typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_16_SNIVELAK_D_001A8EB0;
void LVL_16_SNIVELAK_FUN_002D0B60(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_16_SNIVELAK_FUN_002ED1C8(s32 index) {
    NativeTable20 values=LVL_16_SNIVELAK_D_001A8EB0;
    return values.items[index];
}

u32 LVL_16_SNIVELAK_FUN_002AE4B0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_16_SNIVELAK_D_0018C0B4;
u32 LVL_16_SNIVELAK_FUN_002CFB08(void) {
    return LVL_16_SNIVELAK_D_0018C0B4;
}

s32 LVL_16_SNIVELAK_FUN_002DCD48(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_16_SNIVELAK_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_16_SNIVELAK_FUN_002AE348(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_16_SNIVELAK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_16_SNIVELAK_FUN_002AE380(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_16_SNIVELAK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_16_SNIVELAK_FUN_002AEFA0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_16_SNIVELAK_FUN_002D08B8(f32, f32, f32, f32, s32, s32);

void LVL_16_SNIVELAK_FUN_002D0360(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_16_SNIVELAK_FUN_002D08B8(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_16_SNIVELAK_FUN_002E9A20(int width, int height, int address, int mode);
extern void LVL_16_SNIVELAK_FUN_003731D0(unsigned int reg, unsigned long value);
extern void LVL_16_SNIVELAK_FUN_002E9D88(int width, int height);

void LVL_16_SNIVELAK_FUN_002DEAE8(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_16_SNIVELAK_FUN_002E9A20(width, height, address, 1);
    LVL_16_SNIVELAK_FUN_003731D0(0x47, 0x30000UL);
    LVL_16_SNIVELAK_FUN_003731D0(0x42, 0x8000000044UL);
    LVL_16_SNIVELAK_FUN_002E9D88(0x100, 0x100);
    LVL_16_SNIVELAK_FUN_003731D0(0x42, 0x8000000044UL);
}

s32 LVL_16_SNIVELAK_FUN_002AE3B8(s32 index) {
 s32 value=LVL_16_SNIVELAK_FUN_002AE380(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_16_SNIVELAK_FUN_002AE3F0(s32 index) {
 return LVL_16_SNIVELAK_FUN_002AE380(index)==47;
}

s32 LVL_16_SNIVELAK_FUN_002B3760(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

f32 LVL_16_SNIVELAK_FUN_0041C678(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_16_SNIVELAK_FUN_00339698(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_16_SNIVELAK_D_0027F530[13];
void LVL_16_SNIVELAK_FUN_002F02D0(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_16_SNIVELAK_D_0027F530[i].key == key) break;
    }
    if (i < 13) {
        LVL_16_SNIVELAK_D_0027F530[i].fields[9] = value;
        if (LVL_16_SNIVELAK_D_0027F530[i].busy == 0)
            LVL_16_SNIVELAK_D_0027F530[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_16_SNIVELAK_D_001395B8[];
u32 LVL_16_SNIVELAK_FUN_002FF0F8(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_16_SNIVELAK_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_16_SNIVELAK_D_00232080[];
s32 LVL_16_SNIVELAK_FUN_00375AB0(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_16_SNIVELAK_D_00232080;
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
extern int LVL_16_SNIVELAK_D_0022E940[];
extern ListOverrideObject *LVL_16_SNIVELAK_D_00227440[];
extern ListOverridePair LVL_16_SNIVELAK_D_0022E340[];
void LVL_16_SNIVELAK_FUN_00362A20(void)
{
    int *selected = LVL_16_SNIVELAK_D_0022E940;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_16_SNIVELAK_D_00227440[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_16_SNIVELAK_D_0022E340[row->key];
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
extern NativeObjectSearchRecord32 LVL_16_SNIVELAK_D_00254CF0[];
s32 LVL_16_SNIVELAK_FUN_003E94C0(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_16_SNIVELAK_D_00254CF0[index].field14 == object) {
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

s32 LVL_16_SNIVELAK_FUN_002CF9C8(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_16_SNIVELAK_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_16_SNIVELAK_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_16_SNIVELAK_D_00189E20)->mode == 0x31) {
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
extern void *LVL_16_SNIVELAK_D_0018C0B0;
extern void *LVL_16_SNIVELAK_D_0018B134;
extern void *LVL_16_SNIVELAK_D_0018B040;
void *LVL_16_SNIVELAK_FUN_002A6840(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_16_SNIVELAK_D_0018C0B0;
    if (kind == 1)
        return LVL_16_SNIVELAK_D_0018B134;
    if (kind == 6)
        return LVL_16_SNIVELAK_D_0018B040;
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
extern unsigned char LVL_16_SNIVELAK_D_00139568[];
extern MappedClassEntry LVL_16_SNIVELAK_D_00263DA0[];
unsigned int LVL_16_SNIVELAK_FUN_002ECEA0(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_16_SNIVELAK_D_00263DA0[LVL_16_SNIVELAK_D_00139568[i]];
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
extern s32 LVL_16_SNIVELAK_FUN_002E71C8(s32 value);
NativeCompactHeader *LVL_16_SNIVELAK_FUN_00377EA8(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_16_SNIVELAK_FUN_002E71C8(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_16_SNIVELAK_FUN_002E71C8(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_16_SNIVELAK_D_001A79F0;
s32 LVL_16_SNIVELAK_FUN_002CF948(void) {
    s32 found = 0;
    if (LVL_16_SNIVELAK_D_001A79F0 == 25 || LVL_16_SNIVELAK_D_001A79F0 == 5 ||
        LVL_16_SNIVELAK_D_001A79F0 == 10 || LVL_16_SNIVELAK_D_001A79F0 == 15) {
        found = 1;
    }
    return found;
}

typedef struct {
    u8 field0;
    u8 active;
    u8 middle[4];
    unsigned short count;
    u8 trailing[8];
} ConditionalResetSlot;
typedef char ConditionalResetSlotSize[(sizeof(ConditionalResetSlot) == 16) ? 1 : -1];
extern ConditionalResetSlot LVL_16_SNIVELAK_D_001B9940[8];
void LVL_16_SNIVELAK_FUN_002CFD90(void) {
    ConditionalResetSlot *slot = LVL_16_SNIVELAK_D_001B9940;
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
extern StateTransitionView LVL_16_SNIVELAK_D_001BFC40;
void LVL_16_SNIVELAK_FUN_002EC808(void) {
    if (LVL_16_SNIVELAK_D_001BFC40.mode == 7 && LVL_16_SNIVELAK_D_001BFC40.state == 1) {
        LVL_16_SNIVELAK_D_001BFC40.state = 2;
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
extern DobboFormatRoot396 LVL_16_SNIVELAK_D_001CA0E0;
extern const char LVL_16_SNIVELAK_D_001A99E0[];
extern const char LVL_16_SNIVELAK_D_001A99E8[];
extern const unsigned char *LVL_16_SNIVELAK_FUN_002EDA60(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_16_SNIVELAK_FUN_00302778(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_16_SNIVELAK_FUN_002EDA60(LVL_16_SNIVELAK_D_001CA0E0.rows[index].text_id);
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
        int key = LVL_16_SNIVELAK_D_001CA0E0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_16_SNIVELAK_D_00263DA0[LVL_16_SNIVELAK_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_16_SNIVELAK_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_16_SNIVELAK_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_16_SNIVELAK_FUN_002B3E10(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_16_SNIVELAK_D_00189E20;
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
extern int LVL_16_SNIVELAK_FUN_003573E8(int, unsigned int, void *);
void LVL_16_SNIVELAK_FUN_004454D8(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_16_SNIVELAK_FUN_003573E8(3, 0, 0);
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
        LVL_16_SNIVELAK_FUN_003573E8(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_16_SNIVELAK_FUN_003573E8(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_16_SNIVELAK_FUN_003573E8(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_16_SNIVELAK_D_001B2A80[16];
extern u32 LVL_16_SNIVELAK_D_001B2AC0[16];

int LVL_16_SNIVELAK_FUN_0030AB10(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_16_SNIVELAK_D_001B2A80[index] == 0 ||
            LVL_16_SNIVELAK_D_001B2A80[index] == object) {
            LVL_16_SNIVELAK_D_001B2A80[index] = object;
            LVL_16_SNIVELAK_D_001B2AC0[index] = 0;
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
extern void LVL_16_SNIVELAK_FUN_002D5DA8(OozlaAppendObject164 *, int, const float *);
extern void LVL_16_SNIVELAK_FUN_002E74B8(float *, const float *, const float *);
extern void LVL_16_SNIVELAK_FUN_002E7968(float *, const float *, const float *);
extern void LVL_16_SNIVELAK_FUN_002D60F0(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_16_SNIVELAK_FUN_002D5D00(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_16_SNIVELAK_FUN_002D5DA8(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_16_SNIVELAK_FUN_002E74B8(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_16_SNIVELAK_FUN_002E7968(difference, difference, &object->transform[0][0]);
        LVL_16_SNIVELAK_FUN_002D60F0(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_16_SNIVELAK_FUN_0044A950(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_16_SNIVELAK_FUN_0044ADE8(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
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
} ResidentFlags2294;

s32 LVL_16_SNIVELAK_FUN_002AE460(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_16_SNIVELAK_D_00189E20;

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

s32 LVL_16_SNIVELAK_FUN_002FF078(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_16_SNIVELAK_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_16_SNIVELAK_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_16_SNIVELAK_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_16_SNIVELAK_FUN_003257D0(void) {
    if (((CallState *)LVL_16_SNIVELAK_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_16_SNIVELAK_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_16_SNIVELAK_D_001A63A8)->active); ((CallState *)LVL_16_SNIVELAK_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_16_SNIVELAK_FUN_00313E40(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

extern unsigned char D_19B278[];

int LVL_16_SNIVELAK_FUN_0031EFB8(void)
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

/* Prototype775 is a research label. Field names below describe only the
 * independently observed byte23 and binary32 word2C operations. */
typedef struct {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[8];
    f32 field2C;
} NativeUpdate775View;

extern void LVL_16_SNIVELAK_FUN_00309C08(NativeUpdate775View *object);

void LVL_16_SNIVELAK_FUN_00399318(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_16_SNIVELAK_FUN_00309C08(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_16_SNIVELAK_FUN_00321980(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_16_SNIVELAK_FUN_00318BC8(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_16_SNIVELAK_FUN_002A8368(void)
{
}


unsigned int LVL_16_SNIVELAK_FUN_002CFA98(void)
{
    return 0;
}


void LVL_16_SNIVELAK_FUN_002E67C0(void)
{
}


void LVL_16_SNIVELAK_FUN_002ED7F8(void)
{
}


void LVL_16_SNIVELAK_FUN_002EE5A8(void)
{
}


void LVL_16_SNIVELAK_FUN_002F5E40(void)
{
}


void LVL_16_SNIVELAK_FUN_002F5E48(void)
{
}


void LVL_16_SNIVELAK_FUN_002F9DD8(void)
{
}


void LVL_16_SNIVELAK_FUN_00302B30(void)
{
}


unsigned int LVL_16_SNIVELAK_FUN_003670E8(void)
{
    return 0;
}


void LVL_16_SNIVELAK_FUN_0036BBC0(void)
{
}


void LVL_16_SNIVELAK_FUN_00372BD8(void)
{
}


void LVL_16_SNIVELAK_FUN_00376560(void)
{
}


void LVL_16_SNIVELAK_FUN_00379CD8(void)
{
}


void LVL_16_SNIVELAK_FUN_003E55B8(void)
{
}


void LVL_16_SNIVELAK_FUN_0041E368(void)
{
}


void LVL_16_SNIVELAK_FUN_00438110(void)
{
}


void LVL_16_SNIVELAK_FUN_00439B80(void)
{
}


void LVL_16_SNIVELAK_FUN_00439DD0(void)
{
}


void LVL_16_SNIVELAK_FUN_0043A2C8(void)
{
}


void LVL_16_SNIVELAK_FUN_00442B40(void)
{
}


void LVL_16_SNIVELAK_FUN_00443798(void)
{
}


void LVL_16_SNIVELAK_FUN_0044F5A0(void)
{
}


void LVL_16_SNIVELAK_FUN_00451930(void)
{
}


void LVL_16_SNIVELAK_FUN_004531C8(void)
{
}
void LVL_16_SNIVELAK_FUN_003B0D50(char *p)
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
void LVL_16_SNIVELAK_FUN_003BA3C8(char *p)
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
void LVL_16_SNIVELAK_FUN_003C5E38(char *p)
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
void LVL_16_SNIVELAK_FUN_003CCCE0(char *p)
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
void LVL_16_SNIVELAK_FUN_003D0288(char *p)
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
void LVL_16_SNIVELAK_FUN_003D6568(char *p)
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
void LVL_16_SNIVELAK_FUN_003E0980(char *p)
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
void LVL_16_SNIVELAK_FUN_003E1D28(char *p)
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
void LVL_16_SNIVELAK_FUN_003E71C0(char *p)
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
void LVL_16_SNIVELAK_FUN_003EE908(char *p)
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
void LVL_16_SNIVELAK_FUN_003FBB88(char *p)
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
void LVL_16_SNIVELAK_FUN_003FCA70(char *p)
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
void LVL_16_SNIVELAK_FUN_00405790(char *p)
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
void LVL_16_SNIVELAK_FUN_00413FB0(char *p)
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
void LVL_16_SNIVELAK_FUN_0041AF98(char *p)
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
void LVL_16_SNIVELAK_FUN_00423728(char *p)
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

extern u8 LVL_16_SNIVELAK_F62e6ff2b_D_00189E20[];
extern u8 LVL_16_SNIVELAK_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_16_SNIVELAK_FUN_00356FE0(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_16_SNIVELAK_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_16_SNIVELAK_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_16_SNIVELAK_F62e6ff2b_D_00188660;
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

extern u8 LVL_16_SNIVELAK_F1c0a2bbf_D_001ADD18[];
extern u8 LVL_16_SNIVELAK_F1c0a2bbf_D_001ADD38[];
extern void LVL_16_SNIVELAK_F1c0a2bbf_FUN_00115E38(void *, s32, void *);

void LVL_16_SNIVELAK_FUN_00438128(u32 *p, u32 a1, u32 a2, u32 a3)
{
    if (a1 < 4u)
        LVL_16_SNIVELAK_F1c0a2bbf_FUN_00115E38(LVL_16_SNIVELAK_F1c0a2bbf_D_001ADD18, 37, LVL_16_SNIVELAK_F1c0a2bbf_D_001ADD38);
    p[0] = a2;
    p[1] = a3;
    p[2] = a1;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
void LVL_16_SNIVELAK_FUN_0039EB20(char *p)
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
void LVL_16_SNIVELAK_FUN_003D9378(char *p)
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
extern Blob LVL_16_SNIVELAK_F6894d7c1_D_001A8E60;
int LVL_16_SNIVELAK_FUN_002ECAD0(int x)
{
    Blob b;
    int i;
    b = LVL_16_SNIVELAK_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_16_SNIVELAK_Fafab4c55_FUN_00115DA8(char *, char *, ...);
extern char LVL_16_SNIVELAK_Fafab4c55_D_001AD5F0[];
extern char LVL_16_SNIVELAK_Fafab4c55_D_001AD600[];
extern char LVL_16_SNIVELAK_Fafab4c55_D_001AD608[];

void LVL_16_SNIVELAK_FUN_0036D5C0(char *dst, int value)
{
    if (value > 999999)
        LVL_16_SNIVELAK_Fafab4c55_FUN_00115DA8(dst, LVL_16_SNIVELAK_Fafab4c55_D_001AD5F0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_16_SNIVELAK_Fafab4c55_FUN_00115DA8(dst, LVL_16_SNIVELAK_Fafab4c55_D_001AD600, value / 1000, value % 1000);
    else
        LVL_16_SNIVELAK_Fafab4c55_FUN_00115DA8(dst, LVL_16_SNIVELAK_Fafab4c55_D_001AD608, value);
}
typedef struct {
    char *base;
    unsigned limit;
    unsigned size;
    unsigned cur;
    int count;
    void *free;
} Hdr;

extern int LVL_16_SNIVELAK_Fe77c6258_FUN_00115E38(char *, int, char *);
extern char LVL_16_SNIVELAK_Fe77c6258_D_001ADD18[];
extern char LVL_16_SNIVELAK_Fe77c6258_D_001ADD60[];

void *LVL_16_SNIVELAK_FUN_004381B0(Hdr *p)
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
        LVL_16_SNIVELAK_Fe77c6258_FUN_00115E38(LVL_16_SNIVELAK_Fe77c6258_D_001ADD18, 83, LVL_16_SNIVELAK_Fe77c6258_D_001ADD60);
        return 0;
    }
    r = p->base + cur;
    p->cur = cur + p->size;
    p->count++;
    return r;
}
void LVL_16_SNIVELAK_FUN_00381E00(char *p)
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
void LVL_16_SNIVELAK_FUN_00384488(char *p)
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
void LVL_16_SNIVELAK_FUN_003A9238(char *p)
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
void LVL_16_SNIVELAK_FUN_00425EB8(char *p)
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

extern Entry *LVL_16_SNIVELAK_Fc68ad20a_D_0018C2B8;

int LVL_16_SNIVELAK_FUN_002CB1D8(int key, int *out)
{
    Entry *e = LVL_16_SNIVELAK_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_16_SNIVELAK_F55a1acb8_D_001A63A8;
extern short LVL_16_SNIVELAK_F55a1acb8_D_001A63AC;
extern int LVL_16_SNIVELAK_F55a1acb8_FUN_00133688(void);
extern void LVL_16_SNIVELAK_F55a1acb8_FUN_0011AEA0(int);

void LVL_16_SNIVELAK_FUN_003268A0(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_16_SNIVELAK_F55a1acb8_FUN_00133688()) {
        LVL_16_SNIVELAK_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_16_SNIVELAK_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f6;
    q = LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f18;
    LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f1C;
        LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_16_SNIVELAK_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_16_SNIVELAK_Fa2dbe766_D_00189E20[];

int LVL_16_SNIVELAK_FUN_003BEDA8(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_16_SNIVELAK_Fa2dbe766_D_00189E20;
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
void LVL_16_SNIVELAK_FUN_0033E210(void)
{
    *(short *)0x001AA802 = (*(unsigned char *)0x001A7BC9) ? 3 : 0;
    *(short *)0x001AA81A = (*(unsigned char *)0x001A7BCA) ? 3 : 0;
    *(short *)0x001AA832 = (*(unsigned char *)0x001A7BCB) ? 3 : 0;
    *(short *)0x001AA84A = (*(unsigned char *)0x001A7BCC) ? 3 : 0;
    *(short *)0x001AA862 = (*(unsigned char *)0x001A7BCE) ? 3 : 0;
}
void LVL_16_SNIVELAK_FUN_00407CA8(char *object)
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
extern char LVL_16_SNIVELAK_Fea34650e_D_00189E20[];

void LVL_16_SNIVELAK_FUN_002CC2C8(void)
{
    char *b = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
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
        char *c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_16_SNIVELAK_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_16_SNIVELAK_FUN_003D9A50(char *p)
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
void LVL_16_SNIVELAK_FUN_00425E50(char *p)
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

struct Slot { s32 w; s32 rest[4]; };
struct Table1 { char pad[19264]; struct Slot slots[48]; };
struct Table2 { char pad[52]; s32 slots[4]; };

extern struct Table1 LVL_16_SNIVELAK_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_16_SNIVELAK_Fee2b87d1_D_00152CD0;

s32 LVL_16_SNIVELAK_FUN_002FE7B8(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_16_SNIVELAK_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_16_SNIVELAK_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_16_SNIVELAK_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_16_SNIVELAK_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_16_SNIVELAK_F1157be91_D_001A63E8;
extern unsigned char LVL_16_SNIVELAK_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_16_SNIVELAK_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_16_SNIVELAK_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_16_SNIVELAK_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_16_SNIVELAK_F1157be91_FUN_00133230(void);
extern int LVL_16_SNIVELAK_F1157be91_FUN_00132028(void);

int LVL_16_SNIVELAK_FUN_00326728(int a0, int a1, int a2) {
    CdMode mode = LVL_16_SNIVELAK_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_16_SNIVELAK_F1157be91_D_001A7900[0];
    LVL_16_SNIVELAK_F1157be91_D_001A7430[0] = 0;
    LVL_16_SNIVELAK_F1157be91_D_001A7434 = 0;
    LVL_16_SNIVELAK_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_16_SNIVELAK_F1157be91_FUN_00133230();
    LVL_16_SNIVELAK_F1157be91_FUN_00132028();
    return 1;
}
extern char LVL_16_SNIVELAK_Fd1a69f7e_D_00238000[];
extern char LVL_16_SNIVELAK_Fd1a69f7e_D_002392C0[];
extern char LVL_16_SNIVELAK_Fd1a69f7e_D_002392C0_1[];

void LVL_16_SNIVELAK_FUN_00381938(void) {
    int *p;
    int *q;
    int *t;
    int *r;
    int i;

    p = (int *)LVL_16_SNIVELAK_Fd1a69f7e_D_00238000;
    p = (int *)((char *)p + 4788);
    for (i = 99; i >= 0; i--) {
        *p = 0;
        p -= 12;
    }
    q = (int *)LVL_16_SNIVELAK_Fd1a69f7e_D_002392C0;
    q[138] = 5;
    q[137] = 0;
    q[136] = 0;
    t = q;
    for (i = 63; i >= 0; i--) {
        t[0] = 0;
        t[64] = 0;
        t++;
    }
    r = (int *)LVL_16_SNIVELAK_Fd1a69f7e_D_002392C0_1;
    r = (int *)((char *)r + 540);
    for (i = 7; i >= 0; i--) {
        *r = 0;
        r--;
    }
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_16_SNIVELAK_F4e5bde81_D_00189E20;
extern s32 LVL_16_SNIVELAK_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_16_SNIVELAK_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_16_SNIVELAK_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_16_SNIVELAK_FUN_002CC620(void) {
    s32 result = LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field348;
    if (LVL_16_SNIVELAK_F4e5bde81_D_001A8FF0 != 0 && LVL_16_SNIVELAK_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_16_SNIVELAK_F4e5bde81_D_001A8FF4 != 0 || LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field2294 == 110 && LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field2294 == 109 || LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field1497 != 0 && LVL_16_SNIVELAK_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field2294 == 0 && LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_16_SNIVELAK_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
typedef short s16;

typedef struct {
    u8 f0; u8 f1; u8 f2; u8 f3;
    s16 f4; s16 f6; s16 f8; s16 fA; s16 fC;
    u8 fE; u8 fF;
} Slot;

extern Slot LVL_16_SNIVELAK_F909a5233_D_001B9940[8] __attribute__((nosda));

int LVL_16_SNIVELAK_FUN_002CFBB8(Slot *src)
{
    int count;
    int i;
    int free;

    count = 0;
    while (count < 8 && src[count].f0 != 255)
        count++;

    free = 0;
    for (i = 0; i < 8 && free < count; i++) {
        if (LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f0 == 0)
            free++;
    }

    if (free != count)
        return -1;

    for (free = 0; free < count; free++) {
        i = 0;
        while (i < 8 && LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f0 != 0)
            i++;
        if (i < 8) {
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f0 = src[free].f0;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f1 = src[free].f1;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f2 = src[free].f2;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f3 = src[free].f3;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f4 = src[free].f4;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f6 = src[free].f6;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].f8 = src[free].f8;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fA = src[free].fA;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fC = src[free].fC;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fE = src[free].fE - src[free].fF;
            LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fF = src[free].fF;
            if (LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fA + LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fC == 0)
                LVL_16_SNIVELAK_F909a5233_D_001B9940[i].fA++;
        }
    }
    return i;
}
/* 6065799417900311 - nested scan over a table, 260 B, no arguments. */

extern int LVL_16_SNIVELAK_F60657994_D_002211E0 __attribute__((nosda));
extern char *LVL_16_SNIVELAK_F60657994_D_0021F760[];
struct Pair {
    short a;
    short b;
};
extern struct Pair LVL_16_SNIVELAK_F60657994_D_00220CE0[];
struct Item {
    char *ptr;
    int extra;
};

void LVL_16_SNIVELAK_FUN_003527C8(void)
{
    int *p;
    int *entry;
    struct Item *items;
    char *node;
    char *block;
    char *slot;
    struct Pair *pair;
    short value;
    int i;
    int j;

    p = &LVL_16_SNIVELAK_F60657994_D_002211E0;
    if (*p < 0)
        return;
    while (*p >= 0) {
        entry = (int *)LVL_16_SNIVELAK_F60657994_D_0021F760[*p];
        for (i = 0; i < *(short *)((char *)entry + 40); i++) {
            items = (struct Item *)((char *)entry + 64);
            node = *(char **)((char *)items + (i << 3));
            block = node + 16;
            slot = block + (*(int *)(block + 4) << 4) + 16;
            for (j = 0; j < *(int *)block; j++) {
                pair = &LVL_16_SNIVELAK_F60657994_D_00220CE0[*(unsigned char *)(slot + 19)];
                value = pair->a;
                if (value != 0)
                    *(int *)(slot + 48) = (*(int *)(slot + 48) & 0xFFFFC000) | value;
                value = pair->b;
                if (value != 0)
                    *(int *)(slot + 32) = (*(int *)(slot + 32) & 0xFFFFC000) | value;
                slot += 64;
            }
        }
        p++;
    }
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
extern struct Node LVL_16_SNIVELAK_F4a421ea7_D_00226C40 __attribute__((nosda));
extern short LVL_16_SNIVELAK_F4a421ea7_D_00226940[][2];

void LVL_16_SNIVELAK_FUN_00360EF8(void)
{
    struct Node *p = &LVL_16_SNIVELAK_F4a421ea7_D_00226C40;

    if (p->elems != 0) {
        do {
            struct Elem *e = p->elems;

            if (p->count > 0) {
                int n = p->count;

                do {
                    short *t = LVL_16_SNIVELAK_F4a421ea7_D_00226940[e->f35];
                    if (t[0] != 0) e->f0 = (e->f0 & 0xFFFFC000u) | (unsigned int)t[0];
                    if (t[1] != 0) e->f48 = (e->f48 & 0xFFFFC000u) | (unsigned int)t[1];
                    e++;
                } while (--n);
            }
            p++;
        } while (p->elems != 0);
    }
}
/* family e21f3c02ab439188 - 152 B, 2 placements.
 * Find the first free 64-byte record in a 32-entry array and initialise it.
 * retail store order R after the guard:
 *   20, 28, 52, 50, 29, 56, 0, 4, 8, 12, 16, 24, 25, 26, 30, 31, 60, 48
 * sched1 hoists R[0] (the source's last statement) to the front, so the source
 * keeps R[1..] and ends with f20.
 */
struct Rec {
    float f0;
    float f4;
    float f8;
    float f12;
    float f16;
    float f20;
    unsigned char f24;
    unsigned char f25;
    short f26;
    unsigned char f28;
    unsigned char f29;
    unsigned char f30;
    unsigned char f31;
    unsigned char pad32[16];
    short f48;
    short f50;
    int f52;
    int f56;
    int f60;
};

extern struct Rec LVL_16_SNIVELAK_Fe21f3c02_D_001BC730[];

struct Rec *LVL_16_SNIVELAK_FUN_002D02C0(int a)
{
    struct Rec *p = LVL_16_SNIVELAK_Fe21f3c02_D_001BC730;
    int i;

    for (i = 0; i < 32; i++) {
        if (p->f29 == 0) {
            p->f0 = -1.0f;
            p->f4 = -1.0f;
            p->f8 = -1.0f;
            p->f12 = -1.0f;
            p->f16 = -1.0f;
            p->f20 = -1.0f;
            p->f28 = a;
            p->f52 = 6;
            p->f50 = 120;
            p->f29 = 1;
            p->f56 = -1;
            p->f24 = 0;
            p->f25 = 0;
            p->f26 = 0;
            p->f30 = 0;
            p->f31 = 0;
            p->f60 = 0;
            p->f48 = 0;
            return p;
        }
        p++;
    }
    return 0;
}
extern unsigned char LVL_16_SNIVELAK_F6251b968_D_001D2480[];
extern char LVL_16_SNIVELAK_F6251b968_D_001CA680[];

unsigned char *LVL_16_SNIVELAK_FUN_0030A390(unsigned char *a0, int a1)
{
    int slot;
    unsigned char *table;
    unsigned char *entry;
    int *q;
    unsigned char *r;

    table = LVL_16_SNIVELAK_F6251b968_D_001D2480;
    slot = 0;
    while (slot < 6 && *(unsigned char *)(table + slot * 64 + 4) != 0)
        slot++;

    entry = LVL_16_SNIVELAK_F6251b968_D_001D2480 + slot * 64;
    entry[4] = 1;
    *(unsigned short *)entry = a1;
    *(unsigned char **)(entry + 16) = (unsigned char *)(LVL_16_SNIVELAK_F6251b968_D_001CA680 + slot * 5376);

    *(int *)(entry + 20) = *(int *)(a0 + 36);
    q = *(int **)(*(int *)(a0 + 36) + 28);
    r = (unsigned char *)q[(short)a1 + 1];
    *(unsigned short *)(entry + 2) = r[2];
    *(int *)(entry + 12) = (int)(r + (r[0] + 4));
    *(int *)(entry + 28) = *(int *)(a0 + 80);
    *(int *)(a0 + 80) = (int)entry;
    return entry;
}
struct Rec7f9c1f18 {
    int v[10];
};

extern struct Rec7f9c1f18 LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[];

void LVL_16_SNIVELAK_FUN_002DBE98(int p0, int p1, int p2, int p3, int p4, int p5,
                                      int p6, int p7, int p8, int p9, unsigned idx)
{
    if (idx >= 32u)
        return;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[0] = p0;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[1] = p1;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[2] = p2;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[3] = p3;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[4] = p4;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[5] = p5;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[6] = p6;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[7] = p7;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[8] = p8;
    LVL_16_SNIVELAK_F7f9c1f18_D_001BD2C8[idx].v[9] = p9;
}
/* Append one 16-byte record to the level's queue at 0x226210 and submit it
   through the DMA helper LVL_16_SNIVELAK_Fa3822957_FUN_0011AFE0.  The four queue globals live in a
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
} LevelQueuea3822957;

extern LevelQueuea3822957 LVL_16_SNIVELAK_Fa3822957_D_001A7240 __attribute__((sda));
extern char LVL_16_SNIVELAK_Fa3822957_D_00226210[];
extern int LVL_16_SNIVELAK_Fa3822957_FUN_0011AFE0(int *dma, int flag);

int LVL_16_SNIVELAK_FUN_00360890(int p0, int p1, int p2, int p3)
{
    int args[4];
    int n, k;

    if (LVL_16_SNIVELAK_Fa3822957_D_001A7240.end - (LVL_16_SNIVELAK_Fa3822957_D_001A7240.cursor - LVL_16_SNIVELAK_Fa3822957_D_001A7240.start) < p2 * 16)
        return -1;
    if (LVL_16_SNIVELAK_Fa3822957_D_001A7240.count == 64)
        return -2;

    args[0] = p0;
    args[1] = LVL_16_SNIVELAK_Fa3822957_D_001A7240.cursor;
    args[2] = p1 * 16;
    args[3] = 0;
    LVL_16_SNIVELAK_Fa3822957_FUN_0011AFE0(args, 1);

    n = LVL_16_SNIVELAK_Fa3822957_D_001A7240.count;
    k = n;
    n = n + 1;
    LVL_16_SNIVELAK_Fa3822957_D_001A7240.count = n;
    *(int *)(LVL_16_SNIVELAK_Fa3822957_D_00226210 + k * 16) = LVL_16_SNIVELAK_Fa3822957_D_001A7240.cursor;
    *(int *)(LVL_16_SNIVELAK_Fa3822957_D_00226210 + k * 16 + 4) = p2;
    *(int *)(LVL_16_SNIVELAK_Fa3822957_D_00226210 + k * 16 + 8) = p3;
    LVL_16_SNIVELAK_Fa3822957_D_001A7240.cursor = LVL_16_SNIVELAK_Fa3822957_D_001A7240.cursor + p2 * 16;
    return k;
}
typedef struct {
    unsigned char key;
    unsigned char reserved01[11];
    int target;
} Row16ac18d0d0;
typedef struct {
    unsigned char reserved00[32];
    Row16ac18d0d0 *rows;
} ListObjectac18d0d0;
typedef struct {
    short first;
    short second;
} Pairac18d0d0;

extern int LVL_16_SNIVELAK_Fac18d0d0_D_001DE280[];
extern ListObjectac18d0d0 *LVL_16_SNIVELAK_Fac18d0d0_D_001DAC40[];
extern Pairac18d0d0 LVL_16_SNIVELAK_Fac18d0d0_D_001DDAC0[];

void LVL_16_SNIVELAK_FUN_0030B038(void)
{
    int *selected;
    ListObjectac18d0d0 *object;
    Row16ac18d0d0 *row;
    unsigned char *keys;
    unsigned int *dst;
    Pairac18d0d0 *pair;
    int *next;

    selected = LVL_16_SNIVELAK_Fac18d0d0_D_001DE280;
    while (*selected >= 0) {
        next = selected + 1;
        object = LVL_16_SNIVELAK_Fac18d0d0_D_001DAC40[*selected];
        row = object->rows;
        for (;;) {
            keys = (unsigned char *)row;
            dst = (unsigned int *)(row->target & 0x7FFFFFFF);
            if (*keys != 255) {
                do {
                    pair = &LVL_16_SNIVELAK_Fac18d0d0_D_001DDAC0[*keys];
                    if (pair->first != 0) {
                        dst[12] = (dst[12] & 0xFFFFC000u) | pair->first;
                    }
                    keys++;
                    if (pair->second != 0) {
                        dst[16] = (dst[16] & 0xFFFFC000u) | pair->second;
                    }
                    dst += 16;
                } while (*keys != 255);
            }
            if (row->target < 0) {
                goto out;
            }
            row++;
        }
out:
        ;
        selected = next;
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


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_16_SNIVELAK_Fabf21065e887d7a9_AT00308680_ROLE00;

int LVL_16_SNIVELAK_FUN_00308680(void)
{
    if ((LVL_16_SNIVELAK_Fabf21065e887d7a9_AT00308680_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_16_SNIVELAK_Fabf21065e887d7a9_AT00308680_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_16_SNIVELAK_Fabf21065e887d7a9_AT00308680_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_16_SNIVELAK_Fabf21065e887d7a9_AT00308680_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_16_SNIVELAK_F5b4b17178a13f443_AT0031C2B8_ROLE00(void *object);

void LVL_16_SNIVELAK_FUN_0031C2B8(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_16_SNIVELAK_F5b4b17178a13f443_AT0031C2B8_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_16_SNIVELAK_Facdcf1600d770d3b_AT00357720_ROLE00[];

void LVL_16_SNIVELAK_FUN_00357720(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_16_SNIVELAK_Facdcf1600d770d3b_AT00357720_ROLE00;
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


extern void LVL_16_SNIVELAK_Fb1b523716b470b36_AT00380F60_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_16_SNIVELAK_Fb1b523716b470b36_AT00380F60_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_16_SNIVELAK_FUN_00380F60(void *object)
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
    LVL_16_SNIVELAK_Fb1b523716b470b36_AT00380F60_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_16_SNIVELAK_Fb1b523716b470b36_AT00380F60_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_16_SNIVELAK_Fb1b523716b470b36_AT00383A08_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_16_SNIVELAK_Fb1b523716b470b36_AT00383A08_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_16_SNIVELAK_FUN_00383A08(void *object)
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
    LVL_16_SNIVELAK_Fb1b523716b470b36_AT00383A08_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_16_SNIVELAK_Fb1b523716b470b36_AT00383A08_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT00392A38_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_00392A38(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT00392A38_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT00398458_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_00398458(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT00398458_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_16_SNIVELAK_F86f665335d9cb905_AT003B8EF8_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_16_SNIVELAK_FUN_003B8EF8(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_16_SNIVELAK_F86f665335d9cb905_AT003B8EF8_ROLE00(owner, owner->context_68);
}


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003BC670_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_003BC670(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003BC670_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003C1CE8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_003C1CE8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003C1CE8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003D9E78_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_003D9E78(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003D9E78_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003DE848_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_003DE848(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003DE848_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003EF6E0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_16_SNIVELAK_FUN_003EF6E0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_16_SNIVELAK_F9cdc323a4d0c2fbd_AT003EF6E0_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_16_SNIVELAK_F6af85cabb56d3b41_AT00435478_ROLE00;

void LVL_16_SNIVELAK_FUN_00435478(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_16_SNIVELAK_F6af85cabb56d3b41_AT00435478_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_16_SNIVELAK_F6af85cabb56d3b41_AT00435478_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_16_SNIVELAK_FUN_00436E80(float factor, void *context,
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


void LVL_16_SNIVELAK_FUN_00437030(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_16_SNIVELAK_FUN_0043C390(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_16_SNIVELAK_FUN_004443B8(float first, float second, float **cell)
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


extern unsigned char LVL_16_SNIVELAK_F79744baad5ad7f65_AT0044B828_ROLE00[];

void LVL_16_SNIVELAK_FUN_0044B828(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_16_SNIVELAK_F79744baad5ad7f65_AT0044B828_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_16_SNIVELAK_F6b0741c38bf00fee_AT002A7508_ROLE00[];
extern void LVL_16_SNIVELAK_F6b0741c38bf00fee_AT002A7508_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_16_SNIVELAK_F6b0741c38bf00fee_AT002A7508_ROLE02(short, short, short);

void LVL_16_SNIVELAK_FUN_002A7508(void) {
 LVL_16_SNIVELAK_F6b0741c38bf00fee_AT002A7508_ROLE01(LVL_16_SNIVELAK_F6b0741c38bf00fee_AT002A7508_ROLE00[1],0,1,0x32);
 LVL_16_SNIVELAK_F6b0741c38bf00fee_AT002A7508_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_16_SNIVELAK_F28c929cadd7aca24_AT002CB6D0_ROLE00;

void LVL_16_SNIVELAK_FUN_002CB6D0(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_16_SNIVELAK_F28c929cadd7aca24_AT002CB6D0_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_16_SNIVELAK_F28c929cadd7aca24_AT002CB6D0_ROLE00.selected = selected;
            LVL_16_SNIVELAK_F28c929cadd7aca24_AT002CB6D0_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_16_SNIVELAK_F5fc519c90e0e763a_AT002CC998_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_16_SNIVELAK_FUN_002CC998(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_16_SNIVELAK_F5fc519c90e0e763a_AT002CC998_ROLE00(-value);
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


void LVL_16_SNIVELAK_FUN_00377928(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[139];

void LVL_16_SNIVELAK_FUN_003843B0(void)
{
    int i;
    LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[138] = 5;
    LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[137] = 0;
    LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[i] = 0;
        LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_16_SNIVELAK_F03c444112283bc5f_AT003843B0_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_16_SNIVELAK_FUN_002A6748(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_16_SNIVELAK_FUN_002CD1D0(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_16_SNIVELAK_FUN_002D56B0(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_16_SNIVELAK_FUN_002D56B8(void) {
    return 1;
}


extern void LVL_16_SNIVELAK_QWEN_11a4c157d102_AT002ED578_ROLE000(unsigned char *);

void LVL_16_SNIVELAK_FUN_002ED578(void *owner)
{
    LVL_16_SNIVELAK_QWEN_11a4c157d102_AT002ED578_ROLE000(owner);
}



void LVL_16_SNIVELAK_QWEN_407ee6f17a73_AT002ED618_ROLE001(int);
void LVL_16_SNIVELAK_QWEN_407ee6f17a73_AT002ED618_ROLE000(void*);

void LVL_16_SNIVELAK_FUN_002ED618(void *param)
{
  LVL_16_SNIVELAK_QWEN_407ee6f17a73_AT002ED618_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_16_SNIVELAK_QWEN_407ee6f17a73_AT002ED618_ROLE000(param);
}


void LVL_16_SNIVELAK_QWEN_5696fcf76f0c_AT0030B320_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_16_SNIVELAK_FUN_0030B320(void)
{
    LVL_16_SNIVELAK_QWEN_5696fcf76f0c_AT0030B320_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


unsigned int LVL_16_SNIVELAK_FUN_00324658(unsigned int param_1)
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
extern void LVL_16_SNIVELAK_QWEN_523e38f49b74_AT0033E560_ROLE000(long);
extern void LVL_16_SNIVELAK_QWEN_523e38f49b74_AT0033E560_ROLE002(void);
extern void LVL_16_SNIVELAK_QWEN_523e38f49b74_AT0033E560_ROLE001(void);

int LVL_16_SNIVELAK_FUN_0033E560(void)
{
  LVL_16_SNIVELAK_QWEN_523e38f49b74_AT0033E560_ROLE000(0);
  LVL_16_SNIVELAK_QWEN_523e38f49b74_AT0033E560_ROLE002();
  LVL_16_SNIVELAK_QWEN_523e38f49b74_AT0033E560_ROLE001();
  return 0;
}


unsigned long long LVL_16_SNIVELAK_FUN_0033E960(void);

extern void LVL_16_SNIVELAK_QWEN_f47929f95774_AT0033E960_ROLE000(long);
extern void LVL_16_SNIVELAK_QWEN_f47929f95774_AT0033E960_ROLE002(void);
extern void LVL_16_SNIVELAK_QWEN_f47929f95774_AT0033E960_ROLE001(void);

unsigned long long LVL_16_SNIVELAK_FUN_0033E960(void)
{
  LVL_16_SNIVELAK_QWEN_f47929f95774_AT0033E960_ROLE000(0);
  LVL_16_SNIVELAK_QWEN_f47929f95774_AT0033E960_ROLE002();
  LVL_16_SNIVELAK_QWEN_f47929f95774_AT0033E960_ROLE001();
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
extern void LVL_16_SNIVELAK_QWEN_cb305b1f1210_AT0033EC00_ROLE000(long);
extern void LVL_16_SNIVELAK_QWEN_cb305b1f1210_AT0033EC00_ROLE002(void);
extern void LVL_16_SNIVELAK_QWEN_cb305b1f1210_AT0033EC00_ROLE001(void);

long long LVL_16_SNIVELAK_FUN_0033EC00(void)
{
    LVL_16_SNIVELAK_QWEN_cb305b1f1210_AT0033EC00_ROLE000(0LL);
    LVL_16_SNIVELAK_QWEN_cb305b1f1210_AT0033EC00_ROLE002();
    LVL_16_SNIVELAK_QWEN_cb305b1f1210_AT0033EC00_ROLE001();
    return 0LL;
}


extern void LVL_16_SNIVELAK_QWEN_c23b3406f982_AT00343FF8_ROLE000(long);
extern void LVL_16_SNIVELAK_QWEN_c23b3406f982_AT00343FF8_ROLE002(void);
extern void LVL_16_SNIVELAK_QWEN_c23b3406f982_AT00343FF8_ROLE001(void);

int LVL_16_SNIVELAK_FUN_00343FF8(void)
{
    LVL_16_SNIVELAK_QWEN_c23b3406f982_AT00343FF8_ROLE000(0);
    LVL_16_SNIVELAK_QWEN_c23b3406f982_AT00343FF8_ROLE002();
    LVL_16_SNIVELAK_QWEN_c23b3406f982_AT00343FF8_ROLE001();
    return 0;
}


void LVL_16_SNIVELAK_QWEN_68cf9ebb5ee2_AT00344168_ROLE000(long);
void LVL_16_SNIVELAK_QWEN_68cf9ebb5ee2_AT00344168_ROLE001(void);
void LVL_16_SNIVELAK_QWEN_68cf9ebb5ee2_AT00344168_ROLE002(void);

unsigned long long LVL_16_SNIVELAK_FUN_00344168(void)
{
    LVL_16_SNIVELAK_QWEN_68cf9ebb5ee2_AT00344168_ROLE000(0);
    LVL_16_SNIVELAK_QWEN_68cf9ebb5ee2_AT00344168_ROLE002();
    LVL_16_SNIVELAK_QWEN_68cf9ebb5ee2_AT00344168_ROLE001();
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

extern void LVL_16_SNIVELAK_QWEN_68f040eb20c9_AT00344220_ROLE000(long arg0);
extern void LVL_16_SNIVELAK_QWEN_68f040eb20c9_AT00344220_ROLE002(void);
extern void LVL_16_SNIVELAK_QWEN_68f040eb20c9_AT00344220_ROLE001(void);

unsigned long long LVL_16_SNIVELAK_FUN_00344220(void)
{
    LVL_16_SNIVELAK_QWEN_68f040eb20c9_AT00344220_ROLE000(0);
    LVL_16_SNIVELAK_QWEN_68f040eb20c9_AT00344220_ROLE002();
    LVL_16_SNIVELAK_QWEN_68f040eb20c9_AT00344220_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_16_SNIVELAK_QWEN_92ea7c2f4a5c_AT003445D0_ROLE000(long);
extern void LVL_16_SNIVELAK_QWEN_92ea7c2f4a5c_AT003445D0_ROLE001(void);
extern void LVL_16_SNIVELAK_QWEN_92ea7c2f4a5c_AT003445D0_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_16_SNIVELAK_FUN_003445D0(void)
{
  LVL_16_SNIVELAK_QWEN_92ea7c2f4a5c_AT003445D0_ROLE000(0);
  LVL_16_SNIVELAK_QWEN_92ea7c2f4a5c_AT003445D0_ROLE002();
  LVL_16_SNIVELAK_QWEN_92ea7c2f4a5c_AT003445D0_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_16_SNIVELAK_QWEN_d01c568afacc_AT003446D0_ROLE000(long param_1);
extern void LVL_16_SNIVELAK_QWEN_d01c568afacc_AT003446D0_ROLE001(void);
extern void LVL_16_SNIVELAK_QWEN_d01c568afacc_AT003446D0_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_16_SNIVELAK_FUN_003446D0(void)
{
    LVL_16_SNIVELAK_QWEN_d01c568afacc_AT003446D0_ROLE000(0);
    LVL_16_SNIVELAK_QWEN_d01c568afacc_AT003446D0_ROLE002();
    LVL_16_SNIVELAK_QWEN_d01c568afacc_AT003446D0_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_16_SNIVELAK_QWEN_093381cf82c3_AT00346880_ROLE000(long);
extern void LVL_16_SNIVELAK_QWEN_093381cf82c3_AT00346880_ROLE002(void);
extern void LVL_16_SNIVELAK_QWEN_093381cf82c3_AT00346880_ROLE001(void);

long long LVL_16_SNIVELAK_FUN_00346880(void)
{
    LVL_16_SNIVELAK_QWEN_093381cf82c3_AT00346880_ROLE000(0);
    LVL_16_SNIVELAK_QWEN_093381cf82c3_AT00346880_ROLE002();
    LVL_16_SNIVELAK_QWEN_093381cf82c3_AT00346880_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_16_SNIVELAK_FUN_00350548(void) {
    return 1;
}


float LVL_16_SNIVELAK_FUN_003960E8(const unsigned char *owner)
{
    const unsigned char *linked = *(const unsigned char * const *)(owner + 0x68);
    return *(const float *)(linked + 0x34);
}



unsigned int LVL_16_SNIVELAK_FUN_003F7FF8(unsigned int param_1)
{
    unsigned int inner_ptr;
    unsigned int result;

    inner_ptr = *(unsigned int *)(param_1 + 0x68);
    result = *(unsigned int *)(inner_ptr + 0x18);
    return result;
}


void LVL_16_SNIVELAK_FUN_00437DA0(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_16_SNIVELAK_FUN_00437DA8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_16_SNIVELAK_FUN_00437F00(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_16_SNIVELAK_FUN_00438050(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_16_SNIVELAK_FUN_00438120(unsigned long param_1)
{
  return param_1;
}


extern void LVL_16_SNIVELAK_QWEN_df8fc8ba49cf_AT0043AAE8_ROLE000(unsigned char *owner, int value);

void LVL_16_SNIVELAK_FUN_0043AAE8(unsigned char *owner, int value)
{
    LVL_16_SNIVELAK_QWEN_df8fc8ba49cf_AT0043AAE8_ROLE000(owner + 8, value);
}



void* LVL_16_SNIVELAK_FUN_0043F4E8(void* param_1);

extern void LVL_16_SNIVELAK_QWEN_f353c206e726_AT0043F4E8_ROLE000(int);
extern unsigned long long LVL_16_SNIVELAK_QWEN_f353c206e726_AT0043F4E8_ROLE001(unsigned long long);

void* LVL_16_SNIVELAK_FUN_0043F4E8(void* param_1)
{
  LVL_16_SNIVELAK_QWEN_f353c206e726_AT0043F4E8_ROLE000((int)param_1 + 8);
  LVL_16_SNIVELAK_QWEN_f353c206e726_AT0043F4E8_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_16_SNIVELAK_QWEN_f2f9288a4e34_AT0043F688_ROLE000(int);

int LVL_16_SNIVELAK_FUN_0043F688(int owner)
{
    int result;
    result = LVL_16_SNIVELAK_QWEN_f2f9288a4e34_AT0043F688_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_16_SNIVELAK_QWEN_e41cd63258f5_AT0043F6C0_ROLE000(unsigned char *);

int LVL_16_SNIVELAK_FUN_0043F6C0(int owner)
{
    return LVL_16_SNIVELAK_QWEN_e41cd63258f5_AT0043F6C0_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_16_SNIVELAK_QWEN_f29f80950ce7_AT00442DE0_ROLE000(int);

void LVL_16_SNIVELAK_FUN_00442DE0(int param_1)
{
  LVL_16_SNIVELAK_QWEN_f29f80950ce7_AT00442DE0_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_16_SNIVELAK_QWEN_bf824305b9f7_AT004436F8_ROLE000(unsigned char *owner, float *records);

void LVL_16_SNIVELAK_FUN_004436F8(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_16_SNIVELAK_QWEN_bf824305b9f7_AT004436F8_ROLE000(owner + 0x188, records);
}



void LVL_16_SNIVELAK_QWEN_61faeca45963_AT004439C0_ROLE000(int param_1);

void LVL_16_SNIVELAK_FUN_004439C0(int param_1)
{
  LVL_16_SNIVELAK_QWEN_61faeca45963_AT004439C0_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_16_SNIVELAK_FUN_00443B28(unsigned long param_1)
{
  return param_1;
}


extern int LVL_16_SNIVELAK_QWEN_6add11b33414_AT004448D8_ROLE000(unsigned char *owner);

int LVL_16_SNIVELAK_FUN_004448D8(unsigned char *owner)
{
    return LVL_16_SNIVELAK_QWEN_6add11b33414_AT004448D8_ROLE000(owner + 0x298);
}



extern void LVL_16_SNIVELAK_QWEN_de961518de48_AT004448F8_ROLE000(unsigned char *owner);

void LVL_16_SNIVELAK_FUN_004448F8(unsigned char *owner)
{
    LVL_16_SNIVELAK_QWEN_de961518de48_AT004448F8_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_16_SNIVELAK_QWEN_7ba3cdeeb16b_AT00449218_ROLE000(unsigned long long);

unsigned long long LVL_16_SNIVELAK_FUN_00449218(unsigned long long value)
{
    LVL_16_SNIVELAK_QWEN_7ba3cdeeb16b_AT00449218_ROLE000(value);
    return value;
}



void LVL_16_SNIVELAK_FUN_00449460(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_16_SNIVELAK_FUN_0044A4C0(unsigned long param_1)
{
  return param_1;
}


void LVL_16_SNIVELAK_FUN_0044A800(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_16_SNIVELAK_FUN_0044A9A0(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_16_SNIVELAK_FUN_0044A9E8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_16_SNIVELAK_FUN_0044F940(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_16_SNIVELAK_FUN_00451A30(void) {
    return 1;
}


extern int LVL_16_SNIVELAK_QWEN_cc83cb329fcb_AT00453270_ROLE000(unsigned char *);

int LVL_16_SNIVELAK_FUN_00453270(int *owner)
{
    int result;
    long status;
    status = LVL_16_SNIVELAK_QWEN_cc83cb329fcb_AT00453270_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_0039E568(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_003B0FB0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_003BA748(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_003C6890(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_003D8DC0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_003E2800(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_0041B318(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(char *local, char *first, char *second);
extern void LVL_16_SNIVELAK_Fc936841d_FUN_002E7488(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_16_SNIVELAK_FUN_004236C0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_16_SNIVELAK_Fc936841d_FUN_002E74B8(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_16_SNIVELAK_Fc936841d_FUN_002E7488((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_16_SNIVELAK_F01bd4546_FUN_004371D8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(char *p, int v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(char *p, float v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(char *p, float v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(char *p, float v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(char *p, float v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(char *p, float v);
extern void LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(char *p, float v);

void LVL_16_SNIVELAK_FUN_0044DF00(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_16_SNIVELAK_F01bd4546_FUN_004371D8(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(p1, 1);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(p2, 1);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(p3, 1);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(p4, 1);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(p5, 1);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9A8(p6, 1);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_16_SNIVELAK_F01bd4546_FUN_0044A9F0(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_16_SNIVELAK_F70997ba9_FUN_003731D0(int a, int b);
extern void LVL_16_SNIVELAK_F70997ba9_FUN_002DF6C0(int a);
extern void *LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(int id);
extern int LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(void *p, int i);
extern void LVL_16_SNIVELAK_F70997ba9_FUN_002E2300(void);
extern void LVL_16_SNIVELAK_F70997ba9_FUN_002E2798(int a, int b, long c, void *d, int e);
extern void LVL_16_SNIVELAK_F70997ba9_FUN_002E22F0(void);
extern void LVL_16_SNIVELAK_F70997ba9_FUN_002DF7E0(void);

int LVL_16_SNIVELAK_FUN_0034AAD8(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_16_SNIVELAK_F70997ba9_FUN_003731D0(66, 68);
    LVL_16_SNIVELAK_F70997ba9_FUN_003731D0(71, 11);
    LVL_16_SNIVELAK_F70997ba9_FUN_002DF6C0(0);
    min = LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11613), -1);
    v = LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_16_SNIVELAK_F70997ba9_FUN_002E2378(LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_16_SNIVELAK_F70997ba9_FUN_002E2300();
    off = count - 6;
    LVL_16_SNIVELAK_F70997ba9_FUN_002E2798(slot, off, 0x80FFA888L, LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11613), -1);
    off += count;
    LVL_16_SNIVELAK_F70997ba9_FUN_002E2798(slot, off, 0x80FFA888L, LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11625), -1);
    off += count;
    LVL_16_SNIVELAK_F70997ba9_FUN_002E2798(slot, off, 0x80FFA888L, LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11626), -1);
    off += count;
    LVL_16_SNIVELAK_F70997ba9_FUN_002E2798(slot, off, 0x80FFA888L, LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11627), -1);
    off += count;
    LVL_16_SNIVELAK_F70997ba9_FUN_002E2798(slot, off, 0x80FFA888L, LVL_16_SNIVELAK_F70997ba9_FUN_002EDA60(11599), -1);
    LVL_16_SNIVELAK_F70997ba9_FUN_002E22F0();
    LVL_16_SNIVELAK_F70997ba9_FUN_002DF7E0();
    return 2;
}
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);

void LVL_16_SNIVELAK_FUN_0039E130(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[0], v[3]);
        v[1] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[1], v[4]);
        v[2] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[2], v[5]);
        v[9] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[9], v[12]);
        v[10] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[10], v[13]);
        v[11] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[11], v[14]);
        v[20] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(v[0]) * v[6];
        v[21] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[1]) * v[7];
        v[22] = -LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[2]) * v[8];
        v[24] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(v[9]) * v[15];
        v[25] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[10]) * v[16];
        v[26] = -LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);
extern float LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(float);

void LVL_16_SNIVELAK_FUN_003D8988(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[0], v[3]);
        v[1] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[1], v[4]);
        v[2] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[2], v[5]);
        v[9] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[9], v[12]);
        v[10] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[10], v[13]);
        v[11] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E84F0(v[11], v[14]);
        v[20] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(v[0]) * v[6];
        v[21] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[1]) * v[7];
        v[22] = -LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[2]) * v[8];
        v[24] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7A98(v[9]) * v[15];
        v[25] = LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[10]) * v[16];
        v[26] = -LVL_16_SNIVELAK_F307ea0ee_FUN_002E7AB0(v[11]) * v[17];
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


extern float LVL_16_SNIVELAK_F9e2cd219_FUN_00313E18(float value);
extern void LVL_16_SNIVELAK_F9e2cd219_FUN_0031A038(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_16_SNIVELAK_F9e2cd219_FUN_003815F8(void *owner, void *out);

void LVL_16_SNIVELAK_FUN_00381510(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_16_SNIVELAK_F9e2cd219_FUN_0031A038((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_16_SNIVELAK_F9e2cd219_FUN_00313E18(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_16_SNIVELAK_F9e2cd219_FUN_003815F8(self, (char *)child + 48);

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


extern float LVL_16_SNIVELAK_F9e2cd219_FUN_00313E18(float value);
extern void LVL_16_SNIVELAK_F9e2cd219_FUN_0031A038(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_16_SNIVELAK_F9e2cd219_FUN_00384098(void *owner, void *out);

void LVL_16_SNIVELAK_FUN_00383FB0(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_16_SNIVELAK_F9e2cd219_FUN_0031A038((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_16_SNIVELAK_F9e2cd219_FUN_00313E18(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_16_SNIVELAK_F9e2cd219_FUN_00384098(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
extern void LVL_16_SNIVELAK_F6df9730c_FUN_002E7300(char *dst, int *src, int count);

void LVL_16_SNIVELAK_FUN_00302350(char *dst, unsigned char *src)
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
        LVL_16_SNIVELAK_F6df9730c_FUN_002E7300(dst, tmp, 64);
        dst = next;
        LVL_16_SNIVELAK_F6df9730c_FUN_002E7300(dst, tmp, 64);
        dst += 64;
        LVL_16_SNIVELAK_F6df9730c_FUN_002E7300(dst, tmp, 64);
        dst += 64;
        LVL_16_SNIVELAK_F6df9730c_FUN_002E7300(dst, tmp, 64);
        dst += 64;
    }
}
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_002D5880(void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_002D56C0(void *);
extern int LVL_16_SNIVELAK_Fdb046c5d_FUN_002D5A98(void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_002E7788(float, void *, void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_002E75B8(void *, void *, void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_002D6020(void *, void *, void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_0031D978(void *, void *, int);
extern int LVL_16_SNIVELAK_Fdb046c5d_FUN_002D6140(void *, void *, void *, float, float);
extern int LVL_16_SNIVELAK_Fdb046c5d_FUN_00357288(int, int, void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_002D5D00(void *, int, void *);
extern int LVL_16_SNIVELAK_Fdb046c5d_FUN_00357288(int, int, void *);
extern void LVL_16_SNIVELAK_Fdb046c5d_FUN_003575D8(int, void *);
extern char LVL_16_SNIVELAK_Fdb046c5d_D_001BFF80[];

int LVL_16_SNIVELAK_FUN_002D52A8(char *obj)
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
        LVL_16_SNIVELAK_Fdb046c5d_FUN_002D5880(obj);
    else
        LVL_16_SNIVELAK_Fdb046c5d_FUN_002D56C0(obj);

    s5 = LVL_16_SNIVELAK_Fdb046c5d_FUN_002D5A98(obj);
    if (s5 == -1)
        return 0;

    s1 = LVL_16_SNIVELAK_Fdb046c5d_D_001BFF80;
    s4 = -1;
    LVL_16_SNIVELAK_Fdb046c5d_FUN_002E7788(1.0f, s1, s1);
    LVL_16_SNIVELAK_Fdb046c5d_FUN_002E75B8(buf, s1, p);
    LVL_16_SNIVELAK_Fdb046c5d_FUN_002D6020(obj, p + 16, buf);
    LVL_16_SNIVELAK_Fdb046c5d_FUN_0031D978(obj + 16, out, 1);
    r = LVL_16_SNIVELAK_Fdb046c5d_FUN_002D6140(p, s1, out, *(float *)(p + 56), *(float *)(p + 60));
    if (r == 0) {
        if (*(unsigned char *)(p + 94) != 255 && *(unsigned char *)(p + 92) == 0)
            s4 = LVL_16_SNIVELAK_Fdb046c5d_FUN_00357288(*(unsigned char *)(p + 94), 0, obj);
        LVL_16_SNIVELAK_Fdb046c5d_FUN_002D5D00(obj, s5, out);
    } else {
        if (*(unsigned char *)(p + 94) != 255)
            s4 = LVL_16_SNIVELAK_Fdb046c5d_FUN_00357288(*(unsigned char *)(p + 94), 0, obj);
    }
    if (s4 != -1)
        LVL_16_SNIVELAK_Fdb046c5d_FUN_003575D8(s4, p + 32);
    return 0;
}
extern void LVL_16_SNIVELAK_F7242f0a4_FUN_002E7510(char *out, void *source, float value);
extern void LVL_16_SNIVELAK_F7242f0a4_FUN_0042B428(char *buffer, int mode);
extern void LVL_16_SNIVELAK_F7242f0a4_FUN_0042B2C8(int value, char *buffer);
extern void LVL_16_SNIVELAK_F7242f0a4_FUN_002E7488(char *first, char *second, char *third);
extern float LVL_16_SNIVELAK_F7242f0a4_FUN_002E7590(void *owner, char *buffer);

extern int LVL_16_SNIVELAK_F7242f0a4_D_001B9B60[];

void LVL_16_SNIVELAK_FUN_0042BAF0(void *owner, int flag, float value)
{
    char buffer[32];
    int *root = LVL_16_SNIVELAK_F7242f0a4_D_001B9B60;

    LVL_16_SNIVELAK_F7242f0a4_FUN_002E7510(buffer, root + 8, value);
    if (flag)
        LVL_16_SNIVELAK_F7242f0a4_FUN_0042B428(buffer + 16, 1);
    else
        LVL_16_SNIVELAK_F7242f0a4_FUN_0042B2C8(root[-4], buffer + 16);
    LVL_16_SNIVELAK_F7242f0a4_FUN_002E7488(buffer, buffer, buffer + 16);
    LVL_16_SNIVELAK_F7242f0a4_FUN_002E7510(owner, root + 12, LVL_16_SNIVELAK_F7242f0a4_FUN_002E7590(root + 12, buffer));
}
extern char LVL_16_SNIVELAK_F45821cfb_D_001C7880[];
extern void LVL_16_SNIVELAK_F45821cfb_FUN_002E7450(char *);

void LVL_16_SNIVELAK_FUN_0044FF68(void)
{
    float *p = (float *)LVL_16_SNIVELAK_F45821cfb_D_001C7880;

    p[224] = 0.4f;
    p[225] = 0.8f;
    p[226] = 1.2f;
    *(int *)&p[227] = 0;
    p[228] = 0.577f;
    p[229] = 0.577f;
    p[230] = -0.577f;
    *(int *)&p[231] = 0;
    LVL_16_SNIVELAK_F45821cfb_FUN_002E7450((char *)&p[232]);
    LVL_16_SNIVELAK_F45821cfb_FUN_002E7450((char *)&p[236]);
}
extern char LVL_16_SNIVELAK_Fd8e166aa_D_001BCF40[];

void LVL_16_SNIVELAK_FUN_002DCA18(void)
{
    char *g = LVL_16_SNIVELAK_Fd8e166aa_D_001BCF40;
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
extern char LVL_16_SNIVELAK_F7cb419c1_D_001FF980[];

extern int LVL_16_SNIVELAK_F7cb419c1_FUN_0034F958(int arg);

int LVL_16_SNIVELAK_FUN_00346150(void)
{
    char *p = LVL_16_SNIVELAK_F7cb419c1_D_001FF980;

    *(int *)(p + 460) = LVL_16_SNIVELAK_F7cb419c1_FUN_0034F958(*(int *)(p + 460));
    return 0;
}
extern char LVL_16_SNIVELAK_F0a76d85b_D_001B99C0[];

extern void LVL_16_SNIVELAK_F0a76d85b_FUN_002E7C78(char *p);
extern void LVL_16_SNIVELAK_F0a76d85b_FUN_002DD1E0(void);

void LVL_16_SNIVELAK_FUN_0044FFF8(void)
{
    char *p = LVL_16_SNIVELAK_F0a76d85b_D_001B99C0;

    *(int *)(p + 320) = 0;
    *(int *)(p + 324) = 0;
    *(int *)(p + 328) = 0;
    LVL_16_SNIVELAK_F0a76d85b_FUN_002E7C78(p + 880);
    LVL_16_SNIVELAK_F0a76d85b_FUN_002DD1E0();
}
extern char LVL_16_SNIVELAK_F391de845_D_001B9B00[];
extern float LVL_16_SNIVELAK_F391de845_FUN_002E7648(char *a, char *b);
extern float LVL_16_SNIVELAK_F391de845_FUN_00355CD8(void *self, float d, float x, float y);

float LVL_16_SNIVELAK_FUN_00355DD0(char *self, char *p)
{
    float v = LVL_16_SNIVELAK_F391de845_FUN_002E7648(p, LVL_16_SNIVELAK_F391de845_D_001B9B00);
    float *q = *(float **)(self + 8);

    return LVL_16_SNIVELAK_F391de845_FUN_00355CD8(q, v, q[0], q[1]);
}
extern int LVL_16_SNIVELAK_F2f080549_D_001A8FF4 __attribute__((sda));
extern char LVL_16_SNIVELAK_F2f080549_D_001B2790[] __attribute__((sda));
extern char LVL_16_SNIVELAK_F2f080549_D_001A9000[] __attribute__((sda));
extern float LVL_16_SNIVELAK_F2f080549_FUN_002E7648(char *a, char *b);

float LVL_16_SNIVELAK_FUN_0031DD50(char *p)
{
    float v;

    if (LVL_16_SNIVELAK_F2f080549_D_001A8FF4 == 0) {
        v = LVL_16_SNIVELAK_F2f080549_FUN_002E7648(p, LVL_16_SNIVELAK_F2f080549_D_001B2790);
    } else {
        v = 100.0f - LVL_16_SNIVELAK_F2f080549_FUN_002E7648(p, LVL_16_SNIVELAK_F2f080549_D_001A9000);
    }
    return v;
}
extern char LVL_16_SNIVELAK_Fa76f1772_D_001BFF60[];
extern void LVL_16_SNIVELAK_Fa76f1772_FUN_002E74B8(char *local, char *data);
extern float LVL_16_SNIVELAK_Fa76f1772_FUN_002E7590(char *local, char *p);

int LVL_16_SNIVELAK_FUN_0042EFC8(int a0, int a1)
{
    char local[16];
    int s0;
    int r;

    if (a1 != 1) {
        r = 0;
    } else {
        s0 = *(int *)(a0 + 112) + 352;
        LVL_16_SNIVELAK_Fa76f1772_FUN_002E74B8(local, LVL_16_SNIVELAK_Fa76f1772_D_001BFF60);
        r = LVL_16_SNIVELAK_Fa76f1772_FUN_002E7590(local, (char *)s0) < 0.0f ? 1 : 0;
    }
    return r;
}
extern void LVL_16_SNIVELAK_F46b43b72_FUN_00326630(int value, char *target);
extern void LVL_16_SNIVELAK_F46b43b72_FUN_003267F8(int value);

extern int LVL_16_SNIVELAK_F46b43b72_D_001BD800[];
extern int LVL_16_SNIVELAK_F46b43b72_D_0014B540[];

int LVL_16_SNIVELAK_FUN_002FDE98(int index)
{
    int j = index + 1;
    int *d = LVL_16_SNIVELAK_F46b43b72_D_001BD800;
    int *b = LVL_16_SNIVELAK_F46b43b72_D_0014B540;
    int *t = (int *)((char *)b + 25416);
    int n = d[12] * 332;
    int hold = d[28];
    int j4 = j * 4;
    int cur = *(int *)((char *)t + (n + index * 4));
    int next = *(int *)((char *)t + (j4 + n));

    n = next - cur;
    if (n > 0) {
        LVL_16_SNIVELAK_F46b43b72_FUN_00326630(hold, (char *)(cur + b[6341]));
        LVL_16_SNIVELAK_F46b43b72_FUN_003267F8(0);
    }
    return 1;
}
extern void LVL_16_SNIVELAK_Fa2d20de7_FUN_00314048(void *object, float first, float second);
extern void LVL_16_SNIVELAK_Fa2d20de7_FUN_002E7488(char *first, char *second, void *third);
extern int LVL_16_SNIVELAK_Fa2d20de7_FUN_002D7790(void *first, char *second, int mode, int value, int extra);
extern void LVL_16_SNIVELAK_Fa2d20de7_FUN_002E74B8(void *first, void *second, void *third);
extern void LVL_16_SNIVELAK_Fa2d20de7_FUN_002E7510(void *first, void *second, float value);

extern short LVL_16_SNIVELAK_Fa2d20de7_D_001B9B00[];
extern short LVL_16_SNIVELAK_Fa2d20de7_D_001BFF60[];
extern int LVL_16_SNIVELAK_Fa2d20de7_D_001886CC[];

void LVL_16_SNIVELAK_FUN_00355B88(void *object)
{
    LVL_16_SNIVELAK_Fa2d20de7_FUN_00314048(object, 0.5f, 6.0f);
    LVL_16_SNIVELAK_Fa2d20de7_FUN_002E7488(object, object, LVL_16_SNIVELAK_Fa2d20de7_D_001B9B00);
    if (LVL_16_SNIVELAK_Fa2d20de7_FUN_002D7790(LVL_16_SNIVELAK_Fa2d20de7_D_001B9B00, object, 130, LVL_16_SNIVELAK_Fa2d20de7_D_001886CC[0], 0)) {
        LVL_16_SNIVELAK_Fa2d20de7_FUN_002E74B8(object, LVL_16_SNIVELAK_Fa2d20de7_D_001BFF60, LVL_16_SNIVELAK_Fa2d20de7_D_001B9B00);
        LVL_16_SNIVELAK_Fa2d20de7_FUN_002E7510(object, object, 0.75f);
        LVL_16_SNIVELAK_Fa2d20de7_FUN_002E7488(object, object, LVL_16_SNIVELAK_Fa2d20de7_D_001B9B00);
    }
}
extern char LVL_16_SNIVELAK_F15d5f4fb_D_001B99C0[];
extern char LVL_16_SNIVELAK_F15d5f4fb_D_00189E20[];
extern float LVL_16_SNIVELAK_F15d5f4fb_FUN_002E7B60(float a, float b);
extern float LVL_16_SNIVELAK_F15d5f4fb_FUN_002E85D8(float a, float b);
extern float LVL_16_SNIVELAK_F15d5f4fb_FUN_002E7648(char *a, char *b);

void LVL_16_SNIVELAK_FUN_00434580(char *o)
{
    char *B = LVL_16_SNIVELAK_F15d5f4fb_D_001B99C0;
    char *D = LVL_16_SNIVELAK_F15d5f4fb_D_00189E20;
    float *p = *(float **)(D + 8848);
    float r = LVL_16_SNIVELAK_F15d5f4fb_FUN_002E85D8(*(float *)(B + 344),
                      LVL_16_SNIVELAK_F15d5f4fb_FUN_002E7B60(p[4] - *(float *)(B + 320),
                              p[5] - *(float *)(B + 324)));

    if (r < 0.5585054f) {
        r = LVL_16_SNIVELAK_F15d5f4fb_FUN_002E7648(D + 128, B + 320);
        if (r < 8.0f) {
            *(char *)(B + 659) = 0;
            *(short *)(o + 126) = 2;
            *(float *)(B + 692) = 0.018f;
            *(float *)(B + 680) = 0.018f;
        } else {
            *(short *)(o + 126) = 4;
        }
    } else {
        *(short *)(o + 126) = 4;
    }
    *(char *)(o + 125) = 0;
}

#ifndef RAC2_T_S8_F4778F810
#define RAC2_T_S8_F4778F810
typedef signed char s8_F4778f810;
#endif

#ifndef RAC2_T_S16_F4778F810
#define RAC2_T_S16_F4778F810
typedef signed short s16_F4778f810;
#endif

#ifndef RAC2_T_U16_F4778F810
#define RAC2_T_U16_F4778F810
typedef unsigned short u16_F4778f810;
#endif





extern void LVL_16_SNIVELAK_F4778f810_FUN_0031DAC8(char *a, char *b, char *c, f32 d);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E7488(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E7968(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E82B0(char *a, char *b);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E8090(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E7968(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E74B8(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E74B8(char *a, char *b, char *c);

#ifndef RAC2_T_V4_F4778F810
#define RAC2_T_V4_F4778F810
typedef struct {
    f32 x, y, z, w;
} V4_F4778f810;
#endif


#ifndef RAC2_T_SUB_F4778F810
#define RAC2_T_SUB_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x10];
    u8 at020[0x28];
    f32 f048;
} Sub_F4778f810;
#endif


#ifndef RAC2_T_OBJ_F4778F810
#define RAC2_T_OBJ_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x58];
    char *p068;
    u8 pad06C[0xC0 - 0x6C];
    u8 at0C0[0x10];
} Obj_F4778f810;
#endif


void LVL_16_SNIVELAK_FUN_002D56C0(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_16_SNIVELAK_F4778f810_FUN_0031DAC8((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_16_SNIVELAK_F4778f810_FUN_002E7488((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_16_SNIVELAK_F4778f810_FUN_002E7968((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_16_SNIVELAK_F4778f810_FUN_002E82B0((char *)p + 0x10, (char *)&tmp[3]);
    LVL_16_SNIVELAK_F4778f810_FUN_002E8090((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_16_SNIVELAK_F4778f810_FUN_002E7968((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_16_SNIVELAK_F4778f810_FUN_002E74B8((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_16_SNIVELAK_F4778f810_FUN_002E74B8((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}

#ifndef RAC2_T_S8_F4778F810
#define RAC2_T_S8_F4778F810
typedef signed char s8_F4778f810;
#endif

#ifndef RAC2_T_S16_F4778F810
#define RAC2_T_S16_F4778F810
typedef signed short s16_F4778f810;
#endif

#ifndef RAC2_T_U16_F4778F810
#define RAC2_T_U16_F4778F810
typedef unsigned short u16_F4778f810;
#endif





extern void LVL_16_SNIVELAK_F4778f810_FUN_0031DAC8(char *a, char *b, char *c, f32 d);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E7488(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E7968(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E82B0(char *a, char *b);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E8090(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E7968(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E74B8(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F4778f810_FUN_002E74B8(char *a, char *b, char *c);

#ifndef RAC2_T_V4_F4778F810
#define RAC2_T_V4_F4778F810
typedef struct {
    f32 x, y, z, w;
} V4_F4778f810;
#endif


#ifndef RAC2_T_SUB_F4778F810
#define RAC2_T_SUB_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x10];
    u8 at020[0x28];
    f32 f048;
} Sub_F4778f810;
#endif


#ifndef RAC2_T_OBJ_F4778F810
#define RAC2_T_OBJ_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x58];
    char *p068;
    u8 pad06C[0xC0 - 0x6C];
    u8 at0C0[0x10];
} Obj_F4778f810;
#endif


void LVL_16_SNIVELAK_FUN_002D57A0(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_16_SNIVELAK_F4778f810_FUN_0031DAC8((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_16_SNIVELAK_F4778f810_FUN_002E7488((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_16_SNIVELAK_F4778f810_FUN_002E7968((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_16_SNIVELAK_F4778f810_FUN_002E82B0((char *)p + 0x10, (char *)&tmp[3]);
    LVL_16_SNIVELAK_F4778f810_FUN_002E8090((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_16_SNIVELAK_F4778f810_FUN_002E7968((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_16_SNIVELAK_F4778f810_FUN_002E74B8((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_16_SNIVELAK_F4778f810_FUN_002E74B8((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}
extern char LVL_16_SNIVELAK_F669d6320_D_001BFF80[];
extern void LVL_16_SNIVELAK_F669d6320_FUN_002E7788(void *out, void *in, float scale);
extern void LVL_16_SNIVELAK_F669d6320_FUN_002E7488(void *out, void *in1, void *in2);
extern int LVL_16_SNIVELAK_F669d6320_FUN_002D7790(void *a, void *b, int mode, void *self, int flag);
extern void LVL_16_SNIVELAK_F669d6320_FUN_002E7510(void *out, void *in, float scale);

void LVL_16_SNIVELAK_FUN_003F84D8(char *self, char *p1, int p2)
{
    float a[4];
    float b[4];

    LVL_16_SNIVELAK_F669d6320_FUN_002E7788(a, self + 224, 0.1f);
    LVL_16_SNIVELAK_F669d6320_FUN_002E7788(b, self + 224, -3.0f);
    LVL_16_SNIVELAK_F669d6320_FUN_002E7488(a, a, self + 16);
    LVL_16_SNIVELAK_F669d6320_FUN_002E7488(b, b, self + 16);

    if (LVL_16_SNIVELAK_F669d6320_FUN_002D7790(a, b, 2, self, 0))
        LVL_16_SNIVELAK_F669d6320_FUN_002E7788(p1, LVL_16_SNIVELAK_F669d6320_D_001BFF80, 1.0f);
    else
        LVL_16_SNIVELAK_F669d6320_FUN_002E7788(p1, self + 224, -1.0f);

    if (p2 == 0)
        LVL_16_SNIVELAK_F669d6320_FUN_002E7510(p1, p1, -1.0f);
}


extern void LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(int arg0, int arg1, int arg2, int arg3);
extern int LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(float value);
extern void LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(int arg0, int arg1, int arg2, int arg3);
extern int LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(float value);
extern void LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(int arg0, int arg1, int arg2, int arg3);
extern int LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(float value);
extern void LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(int arg0, int arg1, int arg2, int arg3);
extern int LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(float value);
extern void LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(int arg0, int arg1, int arg2, int arg3);
extern int LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(float value);
extern void LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(int arg0, int arg1, int arg2, int arg3);

#ifndef RAC2_T_RESIDENTSTATE_FD160FB9C
#define RAC2_T_RESIDENTSTATE_FD160FB9C
typedef struct {
    u8 pad000[0x1b8];
    int f1B8;
    u8 pad1bc[0xc2c - 0x1bc];
    int fC2C;
    u8 padc30[0x149d - 0xc30];
    u8 b149D;
    u8 pad149e[0x2290 - 0x149e];
    u8 *p2290;
    int f2294;
} ResidentState_Fd160fb9c;
#endif


extern ResidentState_Fd160fb9c LVL_16_SNIVELAK_Fd160fb9c_D_00189E20;

void LVL_16_SNIVELAK_FUN_002A77A0(void)
{
    int selector;

    if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.f1B8 < 15)
            return;
    }

    selector = LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.b149D;

    if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.f1B8 == 22)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 0, 1, 30);
    }

    switch (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.p2290[67]) {
    case 3:
        if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(49.5f) != 0)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 0, 1, 30);
        if (LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(17.0f) != 0)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 1, 1, 30);
        return;
    case 4:
        if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(12.5f) != 0)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 0, 1, 30);
        if (LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(1.0f) != 0)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 1, 1, 30);
        return;
    case 20:
        if (LVL_16_SNIVELAK_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(8.0f) != 0)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 0, 1, 30);
        if (LVL_16_SNIVELAK_Fd160fb9c_FUN_002B3E10(21.0f) != 0)
            LVL_16_SNIVELAK_Fd160fb9c_FUN_002A7548(selector, 1, 1, 30);
        return;
    }
}
#ifndef RAC2_T_PERSISTENT_F882F1178
#define RAC2_T_PERSISTENT_F882F1178
typedef struct {
    unsigned char pad0[4960];
    void *p1360;
    void *p1364;
    void *p1368;
    unsigned char pad1[4512];
    float f250C;
    float f2510;
    float f2514;
} Persistent_F882f1178;
#endif


#ifndef RAC2_T_NODE_F882F1178
#define RAC2_T_NODE_F882F1178
typedef struct {
    unsigned char pad[120];
    int field120;
} Node_F882f1178;
#endif


extern Persistent_F882f1178 LVL_16_SNIVELAK_F882f1178_D_00189E20;
extern int LVL_16_SNIVELAK_F882f1178_D_001A8F00 __attribute__((sda));

extern float LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F882f1178_FUN_002E7AB0(float);
extern int LVL_16_SNIVELAK_F882f1178_FUN_002E8690(int, int, float);
extern float LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(float, float);
extern float LVL_16_SNIVELAK_F882f1178_FUN_002E7AB0(float);
extern int LVL_16_SNIVELAK_F882f1178_FUN_002E8690(int, int, float);
extern void LVL_16_SNIVELAK_F882f1178_FUN_0031E860(int, float *, int, int, float);
extern void LVL_16_SNIVELAK_F882f1178_FUN_0031E860(int, float *, int, int, float);

void LVL_16_SNIVELAK_FUN_002CDC58(void)
{
    Node_F882f1178 *node;

    node = (Node_F882f1178 *)LVL_16_SNIVELAK_F882f1178_D_00189E20.p1360;
    if (node != 0) {
        if (*(int *)0x1A8F00 == 2)
            LVL_16_SNIVELAK_F882f1178_D_00189E20.f250C = LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(LVL_16_SNIVELAK_F882f1178_D_00189E20.f250C, 0.020362177863717079f);
        else
            LVL_16_SNIVELAK_F882f1178_D_00189E20.f250C = LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(LVL_16_SNIVELAK_F882f1178_D_00189E20.f250C, 0.034906592220067978f);
        node->field120 = LVL_16_SNIVELAK_F882f1178_FUN_002E8690(0xd2d2d2, 0x285050,
                                 LVL_16_SNIVELAK_F882f1178_FUN_002E7AB0(LVL_16_SNIVELAK_F882f1178_D_00189E20.f250C) * 0.5f + 0.5f);
    }
    node = (Node_F882f1178 *)LVL_16_SNIVELAK_F882f1178_D_00189E20.p1364;
    if (node != 0) {
        if (LVL_16_SNIVELAK_F882f1178_D_001A8F00 != 2) {
            float limit;
            int which;

            limit = LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(LVL_16_SNIVELAK_F882f1178_D_00189E20.f2510, 0.049451004713773727f);
            LVL_16_SNIVELAK_F882f1178_D_00189E20.f2510 = limit;
            which = 0x1ee628;
            if (limit > 0.0f && limit < 1.9198623895645142f) {
                which = 0x1e1ed2;
            }
            node->field120 = LVL_16_SNIVELAK_F882f1178_FUN_002E8690(node->field120, which, 0.070000000298023224f);
        } else {
            LVL_16_SNIVELAK_F882f1178_D_00189E20.f2510 = LVL_16_SNIVELAK_F882f1178_FUN_002E84F0(LVL_16_SNIVELAK_F882f1178_D_00189E20.f2510, 0.026179943233728409f);
            node->field120 = LVL_16_SNIVELAK_F882f1178_FUN_002E8690(0x1e1ed2, 0x1e1e50,
                                     LVL_16_SNIVELAK_F882f1178_FUN_002E7AB0(LVL_16_SNIVELAK_F882f1178_D_00189E20.f2510) * 0.5f + 0.5f);
        }
    }
    if (LVL_16_SNIVELAK_F882f1178_D_00189E20.p1368 != 0) {
        if (LVL_16_SNIVELAK_F882f1178_D_001A8F00 == 2)
            LVL_16_SNIVELAK_F882f1178_FUN_0031E860((int)LVL_16_SNIVELAK_F882f1178_D_00189E20.p1368, &LVL_16_SNIVELAK_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.026179943233728409f);
        else
            LVL_16_SNIVELAK_F882f1178_FUN_0031E860((int)LVL_16_SNIVELAK_F882f1178_D_00189E20.p1368, &LVL_16_SNIVELAK_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.040724355727434158f);
    }
}
extern void LVL_16_SNIVELAK_F8411efa9_FUN_003CCCE0(char *pkt);
extern long long LVL_16_SNIVELAK_F8411efa9_FUN_002DF9F8(char *p);
extern float LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(float x, float y);
extern float LVL_16_SNIVELAK_F8411efa9_FUN_002E7600(char *p);
extern float LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(float x, float y);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_002E7D28(float *matrix, float *quat);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_002E7788(float *off, char *src, float scale);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_003CCD68(char *pkt, float angle);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_002E3DE8(char *pkt, float *matrix, int mode);

void LVL_16_SNIVELAK_FUN_003CCF88(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_16_SNIVELAK_F8411efa9_FUN_003CCCE0(pkt);
    r = LVL_16_SNIVELAK_F8411efa9_FUN_002DF9F8(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(LVL_16_SNIVELAK_F8411efa9_FUN_002E7600(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_16_SNIVELAK_F8411efa9_FUN_002E7D28(m, quat);
    LVL_16_SNIVELAK_F8411efa9_FUN_002E7788(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_16_SNIVELAK_F8411efa9_FUN_003CCD68(pkt, f12);
    LVL_16_SNIVELAK_F8411efa9_FUN_002E3DE8(pkt, m, 0);
}
extern void LVL_16_SNIVELAK_F8411efa9_FUN_003D0288(char *pkt);
extern long long LVL_16_SNIVELAK_F8411efa9_FUN_002DF9F8(char *p);
extern float LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(float x, float y);
extern float LVL_16_SNIVELAK_F8411efa9_FUN_002E7600(char *p);
extern float LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(float x, float y);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_002E7D28(float *matrix, float *quat);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_002E7788(float *off, char *src, float scale);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_003D0310(char *pkt, float angle);
extern void LVL_16_SNIVELAK_F8411efa9_FUN_002E3DE8(char *pkt, float *matrix, int mode);

void LVL_16_SNIVELAK_FUN_003D0378(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_16_SNIVELAK_F8411efa9_FUN_003D0288(pkt);
    r = LVL_16_SNIVELAK_F8411efa9_FUN_002DF9F8(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_16_SNIVELAK_F8411efa9_FUN_002E7B60(LVL_16_SNIVELAK_F8411efa9_FUN_002E7600(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_16_SNIVELAK_F8411efa9_FUN_002E7D28(m, quat);
    LVL_16_SNIVELAK_F8411efa9_FUN_002E7788(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_16_SNIVELAK_F8411efa9_FUN_003D0310(pkt, f12);
    LVL_16_SNIVELAK_F8411efa9_FUN_002E3DE8(pkt, m, 0);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003C5FE0(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003CCD68(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003D0310(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003D65F0(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003E0A08(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003E1DB0(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);
extern void LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(char *dst, char *src, float scale);

void LVL_16_SNIVELAK_FUN_003EE990(char *p, float scale)
{
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p, p, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 16, p + 16, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 32, p + 32, scale);
    LVL_16_SNIVELAK_F2c74c194_FUN_002E7510(p + 48, p + 48, scale);
}
extern void LVL_16_SNIVELAK_F40487154_FUN_002E7D08(char *a, char *b);
extern float LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(float value);
extern void LVL_16_SNIVELAK_F40487154_FUN_002E7510(char *a, char *b, float value);
extern float LVL_16_SNIVELAK_F40487154_FUN_002E84F0(float value, float scale);

void LVL_16_SNIVELAK_FUN_003D2E30(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_16_SNIVELAK_F40487154_FUN_002E7D08(object + 192, object + 240);
    x = LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_16_SNIVELAK_F40487154_FUN_002E7510(object + 192, object + 192, x);
    LVL_16_SNIVELAK_F40487154_FUN_002E7510(object + 208, object + 208, y);
    LVL_16_SNIVELAK_F40487154_FUN_002E7510(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_16_SNIVELAK_F40487154_FUN_002E84F0(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_16_SNIVELAK_F40487154_FUN_002E84F0(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_16_SNIVELAK_F40487154_FUN_002E84F0(*(float *)(data + 12), 0.5f);
}
extern void LVL_16_SNIVELAK_F40487154_FUN_002E7D08(char *a, char *b);
extern float LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(float value);
extern void LVL_16_SNIVELAK_F40487154_FUN_002E7510(char *a, char *b, float value);
extern float LVL_16_SNIVELAK_F40487154_FUN_002E84F0(float value, float scale);

void LVL_16_SNIVELAK_FUN_003DE298(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_16_SNIVELAK_F40487154_FUN_002E7D08(object + 192, object + 240);
    x = LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_16_SNIVELAK_F40487154_FUN_002E7AB0(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_16_SNIVELAK_F40487154_FUN_002E7510(object + 192, object + 192, x);
    LVL_16_SNIVELAK_F40487154_FUN_002E7510(object + 208, object + 208, y);
    LVL_16_SNIVELAK_F40487154_FUN_002E7510(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_16_SNIVELAK_F40487154_FUN_002E84F0(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_16_SNIVELAK_F40487154_FUN_002E84F0(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_16_SNIVELAK_F40487154_FUN_002E84F0(*(float *)(data + 12), 0.5f);
}
extern int LVL_16_SNIVELAK_F6eb4f363_FUN_00313EA0(int mode);
extern float LVL_16_SNIVELAK_F6eb4f363_FUN_00315D70(char *object, char *local);
extern void LVL_16_SNIVELAK_F6eb4f363_FUN_002E7510(char *local, int value, float scale);
extern float LVL_16_SNIVELAK_F6eb4f363_FUN_00313F38(float low, float high);
extern void LVL_16_SNIVELAK_F6eb4f363_FUN_0032E6B8(char *object, char *local, float amount, float base);

void LVL_16_SNIVELAK_FUN_00380E50(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_16_SNIVELAK_F6eb4f363_FUN_00313EA0(5))
        return;
    base = LVL_16_SNIVELAK_F6eb4f363_FUN_00315D70(object + 16, local);
    LVL_16_SNIVELAK_F6eb4f363_FUN_002E7510(local, value, 0.25f);
    LVL_16_SNIVELAK_F6eb4f363_FUN_0032E6B8(object + 16, local, LVL_16_SNIVELAK_F6eb4f363_FUN_00313F38(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_16_SNIVELAK_F6eb4f363_FUN_00313EA0(int mode);
extern float LVL_16_SNIVELAK_F6eb4f363_FUN_00315D70(char *object, char *local);
extern void LVL_16_SNIVELAK_F6eb4f363_FUN_002E7510(char *local, int value, float scale);
extern float LVL_16_SNIVELAK_F6eb4f363_FUN_00313F38(float low, float high);
extern void LVL_16_SNIVELAK_F6eb4f363_FUN_0032E6B8(char *object, char *local, float amount, float base);

void LVL_16_SNIVELAK_FUN_003838F8(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_16_SNIVELAK_F6eb4f363_FUN_00313EA0(5))
        return;
    base = LVL_16_SNIVELAK_F6eb4f363_FUN_00315D70(object + 16, local);
    LVL_16_SNIVELAK_F6eb4f363_FUN_002E7510(local, value, 0.25f);
    LVL_16_SNIVELAK_F6eb4f363_FUN_0032E6B8(object + 16, local, LVL_16_SNIVELAK_F6eb4f363_FUN_00313F38(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_16_SNIVELAK_F6eb4f363_FUN_00313EA0(int mode);
extern float LVL_16_SNIVELAK_F6eb4f363_FUN_00315D70(char *object, char *local);
extern void LVL_16_SNIVELAK_F6eb4f363_FUN_002E7510(char *local, int value, float scale);
extern float LVL_16_SNIVELAK_F6eb4f363_FUN_00313F38(float low, float high);
extern void LVL_16_SNIVELAK_F6eb4f363_FUN_0032E6B8(char *object, char *local, float amount, float base);

void LVL_16_SNIVELAK_FUN_00389CF8(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_16_SNIVELAK_F6eb4f363_FUN_00313EA0(5))
        return;
    base = LVL_16_SNIVELAK_F6eb4f363_FUN_00315D70(object + 16, local);
    LVL_16_SNIVELAK_F6eb4f363_FUN_002E7510(local, value, 0.25f);
    LVL_16_SNIVELAK_F6eb4f363_FUN_0032E6B8(object + 16, local, LVL_16_SNIVELAK_F6eb4f363_FUN_00313F38(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_16_SNIVELAK_Fc10c1216_FUN_002E7128(char *);
extern void LVL_16_SNIVELAK_Fc10c1216_FUN_00327D48(char *);
extern int LVL_16_SNIVELAK_Fc10c1216_FUN_002E8648(float);
extern void LVL_16_SNIVELAK_Fc10c1216_FUN_002E7488(char *, char *, char *);

void LVL_16_SNIVELAK_FUN_0032D948(char *p)
{
    float *q = (float *)(p + 32);

    if (0.0f < q[3])
        q[1] = q[1] + q[3] * 0.007f;
    else if (0.03f < q[1])
        q[1] = q[1] + q[3] * 0.007f;
    else {
        q[1] = q[1] + q[3] * 1.4000000664964318275452e-03f;
        *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    }

    if (q[1] <= 0.0244f || LVL_16_SNIVELAK_Fc10c1216_FUN_002E7128(p + 10) != 0) {
        LVL_16_SNIVELAK_Fc10c1216_FUN_00327D48(p);
        return;
    }

    if (q[1] >= 0.12f) {
        q[3] = -q[3];
        q[1] = q[1] + q[3] * 0.007f;
    }
    *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    *(int *)(p + 4) = (*(int *)(p + 4) & 0xffffff) | (LVL_16_SNIVELAK_Fc10c1216_FUN_002E8648(q[1] * 255.0f) << 24);
    q[2] = q[2] + 0.002f;
    if (q[2] > 1.0f)
        q[2] = q[2] - 1.0f;
    *(char *)(p + 8) = (char)(q[2] * 255.0f);
    LVL_16_SNIVELAK_Fc10c1216_FUN_002E7488(p + 16, p + 16, (char *)(q + 4));
}
extern int LVL_16_SNIVELAK_F250fbfa4_FUN_002E7128(char *p);
extern void LVL_16_SNIVELAK_F250fbfa4_FUN_00327D48(char *p);
extern void LVL_16_SNIVELAK_F250fbfa4_FUN_002E7488(char *p0, char *p1, char *p2);

void LVL_16_SNIVELAK_FUN_003374A8(char *a0)
{
    char local[16];
    char *s1 = a0;
    char *s0;
    int a1;
    int v1;
    char *p;

    if (LVL_16_SNIVELAK_F250fbfa4_FUN_002E7128(a0 + 10) != 0) {
        LVL_16_SNIVELAK_F250fbfa4_FUN_00327D48(a0);
        return;
    }
    s0 = a0 + 32;
    a1 = *(int *)(s1 + 4);
    v1 = (a1 >> 24) + *(unsigned char *)(s0 + 20);
    *(int *)(s1 + 4) = (a1 & 0x00ffffff) | (v1 << 24);
    p = s1 + 16;
    *(float *)(local + 0) = *(float *)(s1 + 32);
    *(float *)(local + 4) = *(float *)(s0 + 4);
    *(float *)(local + 8) = *(float *)(s0 + 8);
    *(int *)(local + 12) = 0;
    LVL_16_SNIVELAK_F250fbfa4_FUN_002E7488(p, p, local);
    *(float *)(s0 + 8) = *(float *)(s0 + 8) - *(float *)(s0 + 12);
    *(unsigned char *)(s1 + 8) = *(unsigned char *)(s1 + 8) + *(unsigned char *)(s0 + 21);
    *(float *)(s1 + 12) = *(float *)(s1 + 12) + *(float *)(s0 + 16);
}
extern int LVL_16_SNIVELAK_F250fbfa4_FUN_002E7128(char *p);
extern void LVL_16_SNIVELAK_F250fbfa4_FUN_00327D48(char *p);
extern void LVL_16_SNIVELAK_F250fbfa4_FUN_002E7488(char *p0, char *p1, char *p2);

void LVL_16_SNIVELAK_FUN_00338370(char *a0)
{
    char local[16];
    char *s1 = a0;
    char *s0;
    int a1;
    int v1;
    char *p;

    if (LVL_16_SNIVELAK_F250fbfa4_FUN_002E7128(a0 + 10) != 0) {
        LVL_16_SNIVELAK_F250fbfa4_FUN_00327D48(a0);
        return;
    }
    s0 = a0 + 32;
    a1 = *(int *)(s1 + 4);
    v1 = (a1 >> 24) + *(unsigned char *)(s0 + 20);
    *(int *)(s1 + 4) = (a1 & 0x00ffffff) | (v1 << 24);
    p = s1 + 16;
    *(float *)(local + 0) = *(float *)(s1 + 32);
    *(float *)(local + 4) = *(float *)(s0 + 4);
    *(float *)(local + 8) = *(float *)(s0 + 8);
    *(int *)(local + 12) = 0;
    LVL_16_SNIVELAK_F250fbfa4_FUN_002E7488(p, p, local);
    *(float *)(s0 + 8) = *(float *)(s0 + 8) - *(float *)(s0 + 12);
    *(unsigned char *)(s1 + 8) = *(unsigned char *)(s1 + 8) + *(unsigned char *)(s0 + 21);
    *(float *)(s1 + 12) = *(float *)(s1 + 12) + *(float *)(s0 + 16);
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043D0B0(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043D4F0(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043D7A8(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043DD30(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043E7B8(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043EA80(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043EE48(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043F0F8(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(char *p);

char *LVL_16_SNIVELAK_FUN_0043F2F0(char *object)
{
    LVL_16_SNIVELAK_Ffb202470_FUN_0043BEC8(object + 8);
    return object;
}
extern void LVL_16_SNIVELAK_Fe524d931_FUN_0043C1A0(char *p);
extern void LVL_16_SNIVELAK_Fe524d931_FUN_0043C390(char *p, float x, float y);
extern void LVL_16_SNIVELAK_Fe524d931_FUN_003573E8(int a, int b, int c);
extern void LVL_16_SNIVELAK_Fe524d931_FUN_002E9850(void);
extern short LVL_16_SNIVELAK_Fe524d931_D_001A6480[];

int LVL_16_SNIVELAK_FUN_0043D210(char *object, int mask)
{
    char *sub = object + 8;
    short oldx;
    short oldy;
    int *v;

    LVL_16_SNIVELAK_Fe524d931_FUN_0043C1A0(sub);
    v = *(int **)(object + 684);
    LVL_16_SNIVELAK_Fe524d931_FUN_0043C390(sub, *(float *)v, *(float *)(v + 1));
    if ((mask & 0x40) != 0 && *(int *)(object + 680) == 0)
        LVL_16_SNIVELAK_Fe524d931_FUN_003573E8(4, 0, 0);
    if ((mask & 0xf000) != 0) {
        short *p = LVL_16_SNIVELAK_Fe524d931_D_001A6480;
        oldx = p[180];
        oldy = p[181];
        if ((mask & 0x1000) != 0) {
            ((unsigned short *)p)[181] = ((unsigned short *)p)[181] - 1;
            if ((short)((unsigned short *)p)[181] < -32)
                p[181] = -32;
        }
        if ((mask & 0x4000) != 0) {
            unsigned short *q = (unsigned short *)LVL_16_SNIVELAK_Fe524d931_D_001A6480;
            q[181] = q[181] + 1;
            if ((short)q[181] > 32)
                ((short *)q)[181] = 32;
        }
        if ((mask & 0x8000) != 0) {
            unsigned short *q = (unsigned short *)LVL_16_SNIVELAK_Fe524d931_D_001A6480;
            q[180] = q[180] - 1;
            if ((short)q[180] < -40)
                ((short *)q)[180] = -40;
        }
        if ((mask & 0x2000) != 0) {
            unsigned short *q = (unsigned short *)LVL_16_SNIVELAK_Fe524d931_D_001A6480;
            q[180] = q[180] + 1;
            if ((short)q[180] > 40)
                ((short *)q)[180] = 40;
        }
        {
            short *q = LVL_16_SNIVELAK_Fe524d931_D_001A6480;
            if (oldx != q[180] || oldy != q[181])
                LVL_16_SNIVELAK_Fe524d931_FUN_003573E8(4, 0, 0);
        }
        LVL_16_SNIVELAK_Fe524d931_FUN_002E9850();
    }
    return (mask >> 6) & 1;
}
extern void LVL_16_SNIVELAK_F0b028b34_FUN_00451C00(char *a, unsigned int b, int c, int d);
extern void LVL_16_SNIVELAK_F0b028b34_FUN_00451C00(char *a, unsigned int b, int c, int d);
extern void LVL_16_SNIVELAK_F0b028b34_FUN_00451B90(int a);

int LVL_16_SNIVELAK_FUN_00451CA0(int *p)
{
    int i;

    p[17] = 1;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[22] = 0;
    p[23] = 0;
    for (i = 0; i < p[21]; i++) {
        *(long long *)((char *)p[20] + i * 24) = -1;
        *(long long *)((char *)p[20] + i * 24 + 8) = -1;
        *(int *)((char *)p[20] + i * 24 + 16) = 0;
        *(int *)((char *)p[20] + i * 24 + 20) = 0;
    }
    for (i = 0; i < p[2]; i++) {
        LVL_16_SNIVELAK_F0b028b34_FUN_00451C00((char *)(p[1] + i * 16), (i * 2048 + p[0]) & 0x0FFFFFFF, 3, 128);
    }
    LVL_16_SNIVELAK_F0b028b34_FUN_00451C00((char *)(p[1] + i * 16), p[1] & 0x0FFFFFFF, 2, 0);
    *(volatile unsigned int *)0x1000B420 = 0;
    *(volatile unsigned int *)0x1000B410 = p[0] & 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000B430 = p[1] & 0x0FFFFFFF;
    LVL_16_SNIVELAK_F0b028b34_FUN_00451B90(5);
    return 1;
}
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_00377668(char *a, char *b);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7488(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7510(char *a, char *b, float f);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_00375548(char *a, char *b);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7510(char *a, char *b, float f);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7488(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7510(char *a, char *b, float f);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7488(char *a, char *b, char *c);
extern int LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E8690(unsigned int a, int b, float f);
extern void LVL_16_SNIVELAK_Fbdd4a4aa_FUN_00327D48(char *a);

void LVL_16_SNIVELAK_FUN_00339198(int *obj)
{
    char *a = (char *)obj + 16;
    char *b = (char *)obj + 32;
    char tmp[16];

    LVL_16_SNIVELAK_Fbdd4a4aa_FUN_00377668(a, a);
    *(int *)(b + 16) = *(int *)(b + 16) - 1;
    if (*(int *)(b + 16) < 0)
        *(int *)(b + 16) = 0;
    *(unsigned char *)((char *)obj + 8) = *(unsigned char *)((char *)obj + 8) + 1;
    LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7488(a, b, a);
    *(float *)((char *)obj + 12) = *(float *)((char *)obj + 12) + 1.5750000000000000000000e+03f;
    LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7510(b, b, 9.9000000953674316406250e-01f);
    LVL_16_SNIVELAK_Fbdd4a4aa_FUN_00375548(a, tmp);
    if ((*(int *)((char *)obj + 4) & 0xFF) < 32) {
        LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7510(tmp, tmp, 1.5000000130385160446167e-03f);
        LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7488(b, b, tmp);
    } else {
        LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7510(tmp, tmp, -7.5000000651925802230835e-04f);
        LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E7488(b, b, tmp);
    }
    *(int *)((char *)obj + 4) = LVL_16_SNIVELAK_Fbdd4a4aa_FUN_002E8690(*(unsigned int *)((char *)obj + 4) & 0x00FFFFFF, 0, 2.5000000372529029846191e-02f)
        | (*(int *)(b + 16) << 24);
    if (*(int *)(b + 16) == 0)
        LVL_16_SNIVELAK_Fbdd4a4aa_FUN_00327D48((char *)obj);
}
#ifndef RAC2_T_OBJ_F7B2F1854
#define RAC2_T_OBJ_F7B2F1854
typedef struct Obj_F7b2f1854 {
    int *f0;
    int *f4;
    int *f8;
    int *f12;
    int *f16;
    int f20;
    int f24;
    int f28;
    int f32;
    int f36;
    int f40;
    int f44;
} Obj_F7b2f1854;
#endif


extern int *LVL_16_SNIVELAK_F7b2f1854_FUN_004381B0(int n);
extern int *LVL_16_SNIVELAK_F7b2f1854_FUN_00438118(int size, int *p);
extern void LVL_16_SNIVELAK_F7b2f1854_FUN_004371D8(Obj_F7b2f1854 *o, int v);

void LVL_16_SNIVELAK_FUN_00437338(Obj_F7b2f1854 *o, int a1, int count)
{
    int *p;
    int n;

    o->f44 = count;
    if (count != 0) {
        p = LVL_16_SNIVELAK_F7b2f1854_FUN_00438118(16, LVL_16_SNIVELAK_F7b2f1854_FUN_004381B0(count));
        n = o->f44;
        o->f0 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_16_SNIVELAK_F7b2f1854_FUN_00438118(16, LVL_16_SNIVELAK_F7b2f1854_FUN_004381B0(n));
        n = o->f44;
        o->f8 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_16_SNIVELAK_F7b2f1854_FUN_00438118(16, LVL_16_SNIVELAK_F7b2f1854_FUN_004381B0(n));
        n = o->f44;
        o->f4 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_16_SNIVELAK_F7b2f1854_FUN_00438118(16, LVL_16_SNIVELAK_F7b2f1854_FUN_004381B0(n));
        n = o->f44;
        o->f12 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_16_SNIVELAK_F7b2f1854_FUN_00438118(16, LVL_16_SNIVELAK_F7b2f1854_FUN_004381B0(n));
        o->f16 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;
    }
    o->f40 = a1;
    o->f32 = 0;
    o->f20 = 0;
    o->f28 = 0;
    o->f24 = 0;
    o->f36 = 0;
    LVL_16_SNIVELAK_F7b2f1854_FUN_004371D8(o, 1);
}
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437A40(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);

char *LVL_16_SNIVELAK_FUN_00444D18(char *p)
{
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 76);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 152);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 228);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 304);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 380);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 456);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 528);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437A40(p + 600);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 664);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 736);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 808);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 880);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 952);
    return p;
}
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437518(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437A40(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);
extern void LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(char *p);

char *LVL_16_SNIVELAK_FUN_00446778(char *p)
{
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 76);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 152);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 228);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 304);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437518(p + 380);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 456);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 528);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437A40(p + 600);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 664);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 736);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 808);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 880);
    LVL_16_SNIVELAK_F449e2f67_FUN_00437CD8(p + 952);
    return p;
}
extern char *LVL_16_SNIVELAK_Ffc961fca_FUN_0031A3B8(char *a1);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7D28(char *dst, char *src);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7488(char *dst, char *src, char *tail);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E74B8(char *dst, char *src, char *tail);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7D28(char *dst, char *src);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7FB0(char *dst, char *src);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7968(char *dst, char *src, char *tail);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7968(char *dst, char *src, char *tail);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7968(char *dst, char *src, char *tail);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E7488(char *dst, char *src, char *tail);
extern void LVL_16_SNIVELAK_Ffc961fca_FUN_002E8090(char *dst, char *src, char *tail);

void LVL_16_SNIVELAK_FUN_00396280(char *o0, char *o1)
{
    char b0[64];
    char b1[64];
    char b2[64];
    char *r = LVL_16_SNIVELAK_Ffc961fca_FUN_0031A3B8(o1);

    if (r == 0)
        return;
    LVL_16_SNIVELAK_Ffc961fca_FUN_002E7D28(b0, r);
    LVL_16_SNIVELAK_Ffc961fca_FUN_002E7488(o0 + 16, o0 + 16, r + 16);
    LVL_16_SNIVELAK_Ffc961fca_FUN_002E74B8(o0 + 16, o0 + 16, o1 + 16);
    if ((*(int *)(r + 60) & 2) != 0) {
        LVL_16_SNIVELAK_Ffc961fca_FUN_002E7D28(b2, r + 32);
        LVL_16_SNIVELAK_Ffc961fca_FUN_002E7FB0(b1, b2);
        LVL_16_SNIVELAK_Ffc961fca_FUN_002E7968(o0 + 16, o0 + 16, b1);
        LVL_16_SNIVELAK_Ffc961fca_FUN_002E7968(o0 + 16, o0 + 16, o1 + 192);
    } else {
        LVL_16_SNIVELAK_Ffc961fca_FUN_002E7968(o0 + 16, o0 + 16, b0);
    }
    LVL_16_SNIVELAK_Ffc961fca_FUN_002E7488(o0 + 16, o0 + 16, o1 + 16);
    LVL_16_SNIVELAK_Ffc961fca_FUN_002E8090(o0 + 192, b0, o0 + 192);
}
extern int LVL_16_SNIVELAK_Fd1c348f5_FUN_002E7128(char *p);
extern int LVL_16_SNIVELAK_Fd1c348f5_FUN_002E8648(float v);
extern int LVL_16_SNIVELAK_Fd1c348f5_FUN_002E7128(char *p);
extern void LVL_16_SNIVELAK_Fd1c348f5_FUN_00327D48(char *p);
extern int LVL_16_SNIVELAK_Fd1c348f5_FUN_002E8648(float v);

void LVL_16_SNIVELAK_FUN_0032B4B0(char *p)
{
    char *q = p + 32;
    int r;
    int v;
    float f;

    *(unsigned char *)(p + 8) = *(unsigned char *)(p + 8) + *(unsigned char *)(q + 8);
    *(float *)(p + 24) = *(float *)(p + 24) + *(float *)(q + 16);
    *(float *)(p + 12) = *(float *)(p + 12) * *(float *)(q + 12);
    if (*(int *)(q + 4) == 0) {
        r = LVL_16_SNIVELAK_Fd1c348f5_FUN_002E7128(p + 10);
        if (r != 0) {
            *(int *)(q + 4) = 1;
            *(short *)(p + 10) = 30;
            *(int *)(p + 4) = *(int *)(q + 20) | 0x7f000000;
        } else {
            f = (float)(10 - *(short *)(p + 10)) * 9.6000003814697265625000e+00f;
            v = LVL_16_SNIVELAK_Fd1c348f5_FUN_002E8648(f);
            *(int *)(p + 4) = ((v + 32) << 24) | *(int *)(q + 20);
        }
    } else {
        r = LVL_16_SNIVELAK_Fd1c348f5_FUN_002E7128(p + 10);
        if (r != 0) {
            LVL_16_SNIVELAK_Fd1c348f5_FUN_00327D48(p);
        } else {
            f = (float)*(short *)(p + 10) * 4.2333333492279052734375e+00f;
            v = LVL_16_SNIVELAK_Fd1c348f5_FUN_002E8648(f);
            *(int *)(p + 4) = (v << 24) | *(int *)(q + 20);
        }
    }
}
extern void LVL_16_SNIVELAK_F2d5993bb_FUN_002E7210(char *p, int a1, int a2);
extern void LVL_16_SNIVELAK_F2d5993bb_FUN_002E7260(char *p, int a1, int a2);
extern int LVL_16_SNIVELAK_F2d5993bb_FUN_003056F0(char *p, int a1);

int LVL_16_SNIVELAK_FUN_003057D8(char *out, int mult, int *recs)
{
    int off = 0;
    char *p = out + 8;
    char *r;
    int v;
    int w;

    if (*(int *)recs != 0) {
        r = (char *)recs;
        do {
            off += 8;
            v = *(int *)(r + 0) + mult * *(int *)(r + 4);
            *(int *)(p + 0) = *(int *)(r + 8);
            *(int *)(p + 4) = *(int *)(r + 4);
            p += 8;
            if (*(int *)(r + 8) == 6000)
                LVL_16_SNIVELAK_F2d5993bb_FUN_002E7210(p, 0, *(int *)(r + 4));
            else
                LVL_16_SNIVELAK_F2d5993bb_FUN_002E7260(p, v, *(int *)(r + 4));
            w = *(int *)(r + 4);
            r += 16;
            p += w;
            off += w;
            p = (char *)(((int)p + 3) & ~3);
            off = (off + 3) & ~3;
        } while (*(int *)r != 0);
    }
    off += 8;
    *(int *)(p + 4) = 0;
    *(int *)(p + 0) = -1;
    v = LVL_16_SNIVELAK_F2d5993bb_FUN_003056F0(out + 8, off);
    *(int *)(out + 4) = v;
    *(int *)(out + 0) = off;
    return off + 8;
}
extern int LVL_16_SNIVELAK_F77a1e64d_FUN_00318B88(char *object);
extern float *LVL_16_SNIVELAK_F77a1e64d_FUN_003180C0(char *object);

int LVL_16_SNIVELAK_FUN_003D2F58(char *object)
{
    float *p;

    if (LVL_16_SNIVELAK_F77a1e64d_FUN_00318B88(object) != 0)
        return 0;
    p = LVL_16_SNIVELAK_F77a1e64d_FUN_003180C0(object);
    if (p == 0)
        return 0;
    if (0.0f < *p)
        return 0;
    return 1;
}
extern int LVL_16_SNIVELAK_F77a1e64d_FUN_00318B88(char *object);
extern float *LVL_16_SNIVELAK_F77a1e64d_FUN_003180C0(char *object);

int LVL_16_SNIVELAK_FUN_003DE3C0(char *object)
{
    float *p;

    if (LVL_16_SNIVELAK_F77a1e64d_FUN_00318B88(object) != 0)
        return 0;
    p = LVL_16_SNIVELAK_F77a1e64d_FUN_003180C0(object);
    if (p == 0)
        return 0;
    if (0.0f < *p)
        return 0;
    return 1;
}
extern int LVL_16_SNIVELAK_F77a1e64d_FUN_00318B88(char *object);
extern float *LVL_16_SNIVELAK_F77a1e64d_FUN_003180C0(char *object);

int LVL_16_SNIVELAK_FUN_0042B268(char *object)
{
    float *p;

    if (LVL_16_SNIVELAK_F77a1e64d_FUN_00318B88(object) != 0)
        return 0;
    p = LVL_16_SNIVELAK_F77a1e64d_FUN_003180C0(object);
    if (p == 0)
        return 0;
    if (0.0f < *p)
        return 0;
    return 1;
}
extern void LVL_16_SNIVELAK_Fb342d477_D_0011AC60(int value);
extern void LVL_16_SNIVELAK_Fb342d477_FUN_00451B90(int value);
extern void LVL_16_SNIVELAK_Fb342d477_FUN_00451B20(int value);
extern void LVL_16_SNIVELAK_Fb342d477_D_0011AC40(int value);

int LVL_16_SNIVELAK_FUN_00452150(int *p)
{
    LVL_16_SNIVELAK_Fb342d477_D_0011AC60(p[16]);
    p[17] = 0;
    LVL_16_SNIVELAK_Fb342d477_FUN_00451B90(5);
    p[7] = *(volatile int *)0x1000B410;
    p[8] = *(volatile int *)0x1000B430;
    p[9] = *(volatile int *)0x1000B420;
    p[10] = *(volatile int *)0x1000B400;
    if (*(volatile int *)0x10002010 & 0xF0)
        while (*(volatile int *)0x10002010 & 0xF0)
            ;
    LVL_16_SNIVELAK_Fb342d477_FUN_00451B20(0);
    p[11] = *(volatile int *)0x1000B010;
    p[12] = *(volatile int *)0x1000B020;
    p[13] = *(volatile int *)0x1000B000;
    p[14] = *(volatile int *)0x10002020;
    p[15] = *(volatile int *)0x10002010;
    LVL_16_SNIVELAK_Fb342d477_D_0011AC40(p[16]);
    return 1;
}
/* Family 0e7bb6a8908d30dc -- 244 bytes, 28 placements (1 boot + 27 levels). */

extern char *LVL_16_SNIVELAK_F0e7bb6a8_FUN_0031A3B8(char *a);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7D28(char *dst, char *src);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7FB0(char *dst, char *src);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7FB0(char *dst, char *src);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E74B8(char *dst, char *a, char *b);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7990(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7D28(char *dst, char *src);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E80E0(char *a, char *b, char *c);
extern void LVL_16_SNIVELAK_F0e7bb6a8_FUN_003186A0(char *a, char *b);

int LVL_16_SNIVELAK_FUN_0031A900(char *unused, char *obj, char *arg2, char *arg3, char *arg4, char *arg5)
{
    char buf0[64];
    char buf1[16];
    char buf2[64];
    char *p;

    p = LVL_16_SNIVELAK_F0e7bb6a8_FUN_0031A3B8(obj);
    if (p == 0)
        return 0;
    if (*(int *)(p + 60) & 0x40) {
        LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7D28(buf0, obj + 240);
        LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7FB0(buf0, buf0);
    } else {
        LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7FB0(buf0, obj + 192);
    }
    LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E74B8(buf1, arg2, obj + 16);
    LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7990(arg4, buf1, buf0);
    LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E7D28(buf2, arg3);
    LVL_16_SNIVELAK_F0e7bb6a8_FUN_002E80E0(buf2, buf0, buf2);
    LVL_16_SNIVELAK_F0e7bb6a8_FUN_003186A0(buf2, arg5);
    return 1;
}
extern void LVL_16_SNIVELAK_Fa2a84657_FUN_002E7528(float *tmp, char *v, float k);
extern void LVL_16_SNIVELAK_Fa2a84657_FUN_002E74A0(float *tmp, char *a, char *v);
extern int LVL_16_SNIVELAK_Fa2a84657_FUN_002E7128(char *field);
extern void LVL_16_SNIVELAK_Fa2a84657_FUN_00327D48(unsigned char *p);

void LVL_16_SNIVELAK_FUN_00331010(unsigned char *p)
{
    float tmp[4];
    char *v;
    int n;

    v = (char *)p + 32;
    LVL_16_SNIVELAK_Fa2a84657_FUN_002E7528(tmp, v, 9.4999998807907104492188e-01f);
    *(float *)(v + 8) = *(float *)(v + 8) + 1.3888889225199818611145e-03f;
    LVL_16_SNIVELAK_Fa2a84657_FUN_002E74A0(tmp, (char *)p + 16, v);

    n = *(int *)(p + 4) + (int)0xFE000000;
    *(int *)(p + 4) = n;
    if (((unsigned)(n & 0xFF000000) - 1) > 0x5EFFFFFFu || LVL_16_SNIVELAK_Fa2a84657_FUN_002E7128((char *)p + 10) != 0) {
        LVL_16_SNIVELAK_Fa2a84657_FUN_00327D48(p);
    } else {
        float f0;
        float f1;

        f1 = *(float *)(p + 12);
        f0 = 2.1000000000000000000000e+05f - f1;
        f0 = f0 * 7.0000000298023223876953e-02f;
        f1 = f1 + f0;
        *(float *)(p + 12) = f1;
        p[8] = p[8] + 1;
    }
}
extern int LVL_16_SNIVELAK_F750245c6_D_001A7340 __attribute__((sda));
extern void LVL_16_SNIVELAK_F750245c6_FUN_002F4930(int a, int b, int c, int d, char *e, int f);
void LVL_16_SNIVELAK_FUN_003403E0(int a0, char *a1)
{
    int h, k, p, m;
    k = a0 / 2 + 5;
    h = LVL_16_SNIVELAK_F750245c6_D_001A7340 / 2;
    m = h - k;
    p = h + k;
    LVL_16_SNIVELAK_F750245c6_FUN_002F4930(m - 2, 312, p + 4, 314, a1, 0);
    LVL_16_SNIVELAK_F750245c6_FUN_002F4930(m - 2, 333, p + 4, 335, a1, 0);
    LVL_16_SNIVELAK_F750245c6_FUN_002F4930(m - 2, 313, m, 334, a1, 0);
    LVL_16_SNIVELAK_F750245c6_FUN_002F4930(p + 2, 313, p + 4, 334, a1, 0);
}
extern float LVL_16_SNIVELAK_Fe617c30b_FUN_002E8638(int a);
extern void LVL_16_SNIVELAK_Fe617c30b_FUN_002E7510(char *p, char *q, float f);
extern void LVL_16_SNIVELAK_Fe617c30b_FUN_002E7488(char *p, char *q, char *r);
extern int LVL_16_SNIVELAK_Fe617c30b_FUN_003161E0(int a, int b, float f);
extern int LVL_16_SNIVELAK_Fe617c30b_FUN_002E7128(char *p);
extern void LVL_16_SNIVELAK_Fe617c30b_FUN_00327D48(char *p);

void LVL_16_SNIVELAK_FUN_00329B30(char *a0)
{
    char *s0 = a0 + 32;
    int x;
    int u;
    int t;
    float f;

    x = *(int *)(s0 + 28);
    t = *(short *)(a0 + 10);
    u = *(short *)(s0 + 24) * (x - t) / x + *(short *)(s0 + 26);

    f = LVL_16_SNIVELAK_Fe617c30b_FUN_002E8638(u) * 1000.0f;
    *(float *)(a0 + 12) = f;
    LVL_16_SNIVELAK_Fe617c30b_FUN_002E7510(s0, s0, 9.8000001907348632812500e-01f);
    LVL_16_SNIVELAK_Fe617c30b_FUN_002E7488(a0 + 16, a0 + 16, s0);
    *(unsigned char *)(a0 + 8) = *(unsigned char *)(a0 + 8) + 1;
    f = LVL_16_SNIVELAK_Fe617c30b_FUN_002E8638(*(int *)(s0 + 28));
    *(int *)(a0 + 4) = LVL_16_SNIVELAK_Fe617c30b_FUN_003161E0(*(int *)(s0 + 20), *(int *)(s0 + 16),
                               (float)*(short *)(a0 + 10) / f);
    if (LVL_16_SNIVELAK_Fe617c30b_FUN_002E7128(a0 + 10) != 0)
        LVL_16_SNIVELAK_Fe617c30b_FUN_00327D48(a0);
}
extern int LVL_16_SNIVELAK_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F840(int);

void LVL_16_SNIVELAK_FUN_003060F8(void)
{
    int value = LVL_16_SNIVELAK_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F840(value + 0x36F28);
}
extern int LVL_16_SNIVELAK_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F860(int);

void LVL_16_SNIVELAK_FUN_00306728(void)
{
    int value = LVL_16_SNIVELAK_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F860(value + 0x36F28);
}
extern int LVL_16_SNIVELAK_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F800(int);

void LVL_16_SNIVELAK_FUN_00306978(void)
{
    int value = LVL_16_SNIVELAK_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F800(value + 0x36F28);
}
extern int LVL_16_SNIVELAK_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F880(int);

void LVL_16_SNIVELAK_FUN_003069A8(void)
{
    int value = LVL_16_SNIVELAK_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_16_SNIVELAK_F4a4e68d9_FUN_0044F880(value + 0x36F28);
}
extern int LVL_16_SNIVELAK_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_16_SNIVELAK_F4a4e68d9_FUN_00439418(int);

void LVL_16_SNIVELAK_FUN_003075B8(void)
{
    int value = LVL_16_SNIVELAK_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_16_SNIVELAK_F4a4e68d9_FUN_00439418(value + 0x36F28);
}
extern void LVL_16_SNIVELAK_F42d2147f_FUN_002E7788(char *a0, char *a1, float f);
extern void LVL_16_SNIVELAK_F42d2147f_FUN_002E7510(char *a0, char *a1, float f);
extern void LVL_16_SNIVELAK_F42d2147f_FUN_002E7488(char *a0, char *a1, char *a2);
extern char LVL_16_SNIVELAK_F42d2147f_D_00189E20[];
extern char LVL_16_SNIVELAK_F42d2147f_D_00189EA0[];

void LVL_16_SNIVELAK_FUN_00432638(char *object)
{
    char local[16];
    char buf[16];
    char *p = LVL_16_SNIVELAK_F42d2147f_D_00189E20;
    unsigned char v;

    LVL_16_SNIVELAK_F42d2147f_FUN_002E7788(local, (char *)(*(int *)(p + 8848) + 224), 1.0f);
    v = *(unsigned char *)(p + 8884);
    if (v == 2) {
        LVL_16_SNIVELAK_F42d2147f_FUN_002E7510(buf, local, 9.5f);
    } else if (v == 1) {
        LVL_16_SNIVELAK_F42d2147f_FUN_002E7510(buf, local, 0.75f);
    } else {
        LVL_16_SNIVELAK_F42d2147f_FUN_002E7510(buf, local, 1.6f);
    }
    LVL_16_SNIVELAK_F42d2147f_FUN_002E7488(object + 48, LVL_16_SNIVELAK_F42d2147f_D_00189EA0, buf);
    LVL_16_SNIVELAK_F42d2147f_FUN_002E7488(object + 48, LVL_16_SNIVELAK_F42d2147f_D_00189EA0 + 208, object + 48);
}
extern char LVL_16_SNIVELAK_F93479d13_D_00189E20[];
extern void LVL_16_SNIVELAK_F93479d13_FUN_00309C08(char *entry);
extern void LVL_16_SNIVELAK_F93479d13_FUN_00309C08(char *entry);

void LVL_16_SNIVELAK_FUN_002B1710(int slot, int value)
{
    char *e;
    void (*fn)(char *);

    {
        char *p = LVL_16_SNIVELAK_F93479d13_D_00189E20 + slot * 80;

        *(int *)(p + 4660) = value;
        if (*(int *)(p + 4676) != 3) {
            *(int *)(p + 4676) = 3;
            e = *(char **)(p + 4640);
            if (e != 0) {
                if (*(unsigned char *)(e + 32) != 254) {
                    if (*(unsigned char *)(e + 32) != 253) {
                        fn = *(void (**)(char *))(e + 100);
                        if (fn != 0)
                            fn(e);
                    }
                }
            }
        }
    }
    {
        char *q = LVL_16_SNIVELAK_F93479d13_D_00189E20 + slot * 80;

        e = *(char **)(q + 4640);
        *(int *)(q + 4676) = 0;
        *(int *)(q + 4680) = 0;
        if (e != 0) {
            LVL_16_SNIVELAK_F93479d13_FUN_00309C08(e);
            *(char **)(q + 4640) = 0;
        }
        e = *(char **)(q + 4644);
        if (e != 0 && slot != 3) {
            LVL_16_SNIVELAK_F93479d13_FUN_00309C08(e);
            *(char **)(q + 4644) = 0;
        }
    }
}
extern char LVL_16_SNIVELAK_F5fa3e1af_D_00189E20[];
extern float LVL_16_SNIVELAK_F5fa3e1af_FUN_002E7450(float *buf);
extern float LVL_16_SNIVELAK_F5fa3e1af_FUN_002E75D0(float *buf);

void LVL_16_SNIVELAK_FUN_003CE358(char *obj)
{
    float buf[2];
    char *d = LVL_16_SNIVELAK_F5fa3e1af_D_00189E20;
    char *e = *(char **)(obj + 104);
    float a, b, x;

    a = -*(float *)(d + 8112);
    *(float *)(e + 20) = a;
    b = -*(float *)(d + 8116);
    *(float *)(e + 16) = b;

    if (a > 1.0f) {
        *(float *)(e + 20) = 1.0f;
    } else if (a < -1.0f) {
        *(float *)(e + 20) = -1.0f;
    }

    x = *(float *)(e + 16);
    if (x > 1.0f) {
        *(float *)(e + 16) = 1.0f;
    } else if (x < -1.0f) {
        *(float *)(e + 16) = -1.0f;
    }
    LVL_16_SNIVELAK_F5fa3e1af_FUN_002E7450(buf);
    buf[0] = *(float *)(e + 16);
    buf[1] = *(float *)(e + 20);
    *(float *)(e + 48) = LVL_16_SNIVELAK_F5fa3e1af_FUN_002E75D0(buf);
}


extern int LVL_16_SNIVELAK_F9a90bcc4_FUN_00313EA0(int count);

int LVL_16_SNIVELAK_FUN_0031FC80(u8 *owner, int b, int *outIndex,
                                       float *o0, float *o1, float *o2, float *o3,
                                       float *o4, float *o5, float *o6)
{
    int n;
    int count;
    int i;
    int k;
    int left;
    u8 *p;

    n = 0;
    count = *(u8 *)(*(int *)(owner + 36) + 12);
    if (count != 0) {
        int *t = (int *)(*(int *)(owner + 36) + 72);
        u8 *q;
        left = count;
        do {
            q = *(u8 **)(*t + 20);
            if (q != 0 && *q == b)
                n++;
            t++;
            left--;
        } while (left != 0);
    }

    if (n != 0)
        goto second;

    return 0;

found:
    *o0 = *(float *)(p + 8) * 0.016666668f;
    *o1 = *(float *)(p + 12) * 0.016666668f;
    *o2 = *(float *)(p + 16) * 0.00027777778f;
    *o3 = *(float *)(p + 20) * 0.00027777778f;
    *o6 = *(float *)(p + 4) * 0.00027777778f;
    *o4 = *(float *)(p + 24);
    *o5 = *(float *)(p + 28);
    *outIndex = i;
    return 1;

second:
    k = LVL_16_SNIVELAK_F9a90bcc4_FUN_00313EA0(n);
    for (i = 0; i < *(u8 *)(*(int *)(owner + 36) + 12); i++) {
        char *tbl = (char *)(*(int *)(owner + 36) + 72);
        char *e = *(char **)(tbl + i * 4);
        p = *(u8 **)(e + 20);
        if (p == 0 || *p != b)
            continue;
        if (k == 0)
            goto found;
        k--;
    }
    return 0;
}


extern void LVL_16_SNIVELAK_Feda2e14e_FUN_002E7488(void *out, void *in, void *src);
extern int LVL_16_SNIVELAK_Feda2e14e_FUN_002E7128(void *p);
extern void LVL_16_SNIVELAK_Feda2e14e_FUN_00327D48(void *self);
extern float LVL_16_SNIVELAK_Feda2e14e_FUN_002E8638(int n);
extern int LVL_16_SNIVELAK_Feda2e14e_FUN_002E8690(int handle, int previous, float ratio);
extern char LVL_16_SNIVELAK_Feda2e14e_D_00189E20[];

void LVL_16_SNIVELAK_FUN_00332210(u8 *self)
{
    char *s2 = (char *)self + 32;
    int n;
    float a;
    float b;

    if (*(int *)(s2 + 28) == 1) {
        char *base = LVL_16_SNIVELAK_Feda2e14e_D_00189E20;
        *(float *)(self + 16) = *(float *)(base + 128) + *(float *)(s2 + 16);
        *(float *)(self + 20) = *(float *)(base + 132) + *(float *)(s2 + 20);
        *(float *)(self + 24) = *(float *)(base + 136) + *(float *)(s2 + 24);
        LVL_16_SNIVELAK_Feda2e14e_FUN_002E7488(self + 16, self + 16, s2);
        *(float *)(s2 + 16) = *(float *)(self + 16) - *(float *)(base + 128);
        *(float *)(s2 + 20) = *(float *)(self + 20) - *(float *)(base + 132);
        *(float *)(s2 + 24) = *(float *)(self + 24) - *(float *)(base + 136);
    } else {
        LVL_16_SNIVELAK_Feda2e14e_FUN_002E7488(self + 16, self + 16, s2);
    }

    if (*(float *)(self + 16) < 2.0f || *(float *)(self + 16) > 1021.0f
        || *(float *)(self + 20) < 2.0f || *(float *)(self + 20) > 1021.0f
        || *(float *)(self + 24) < 2.0f || *(float *)(self + 24) > 1021.0f) {
        LVL_16_SNIVELAK_Feda2e14e_FUN_00327D48(self);
        return;
    }

    if (LVL_16_SNIVELAK_Feda2e14e_FUN_002E7128((char *)self + 10) != 0) {
        LVL_16_SNIVELAK_Feda2e14e_FUN_00327D48(self);
        return;
    }

    n = *(int *)(self + 4) & 0xFFFFFF;
    a = LVL_16_SNIVELAK_Feda2e14e_FUN_002E8638(*(short *)(self + 10) - 1);
    b = LVL_16_SNIVELAK_Feda2e14e_FUN_002E8638(*(short *)(self + 10));
    *(int *)(self + 4) = LVL_16_SNIVELAK_Feda2e14e_FUN_002E8690(n, *(int *)(self + 4), a / b);
}
extern void LVL_16_SNIVELAK_Feb99aa89_FUN_002CC858(char *target, int mode, float value, float zero, float scale);
extern int LVL_16_SNIVELAK_Feb99aa89_FUN_002D7790(char *first, char *second, int mode, int flag, int extra);
extern float LVL_16_SNIVELAK_Feb99aa89_FUN_002E7648(char *source, short *table);

extern short LVL_16_SNIVELAK_Feb99aa89_D_001BFF60[];
extern float LVL_16_SNIVELAK_Feb99aa89_D_0018A084;

int LVL_16_SNIVELAK_FUN_002ABA20(float *out, float scale, float amount)
{
    char buffer[32];

    LVL_16_SNIVELAK_Feb99aa89_FUN_002CC858(buffer, 1, LVL_16_SNIVELAK_Feb99aa89_D_0018A084 - 0.02f, 0.0f, scale);
    LVL_16_SNIVELAK_Feb99aa89_FUN_002CC858(buffer + 16, 1, amount, 0.0f, scale);
    if (LVL_16_SNIVELAK_Feb99aa89_FUN_002D7790(buffer, buffer + 16, 2, 0, 0)) {
        if (out)
            *out = LVL_16_SNIVELAK_Feb99aa89_FUN_002E7648(buffer, LVL_16_SNIVELAK_Feb99aa89_D_001BFF60);
        return 1;
    }
    return 0;
}
