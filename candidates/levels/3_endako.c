typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_3_ENDAKO_D_001A8EB0;
void LVL_3_ENDAKO_FUN_002D8418(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_3_ENDAKO_FUN_002F4CD8(s32 index) {
    NativeTable20 values=LVL_3_ENDAKO_D_001A8EB0;
    return values.items[index];
}

u32 LVL_3_ENDAKO_FUN_002B3890(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_3_ENDAKO_D_0018C0B4;
u32 LVL_3_ENDAKO_FUN_002D73C0(void) {
    return LVL_3_ENDAKO_D_0018C0B4;
}

s32 LVL_3_ENDAKO_FUN_002E45F0(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_3_ENDAKO_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_3_ENDAKO_FUN_002B3728(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_3_ENDAKO_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_3_ENDAKO_FUN_002B3760(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_3_ENDAKO_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_3_ENDAKO_FUN_002B4380(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_3_ENDAKO_FUN_002D8170(f32, f32, f32, f32, s32, s32);

void LVL_3_ENDAKO_FUN_002D7C18(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_3_ENDAKO_FUN_002D8170(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_3_ENDAKO_FUN_002F1530(int width, int height, int address, int mode);
extern void LVL_3_ENDAKO_FUN_0037CA78(unsigned int reg, unsigned long value);
extern void LVL_3_ENDAKO_FUN_002F1898(int width, int height);

void LVL_3_ENDAKO_FUN_002E6390(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_3_ENDAKO_FUN_002F1530(width, height, address, 1);
    LVL_3_ENDAKO_FUN_0037CA78(0x47, 0x30000UL);
    LVL_3_ENDAKO_FUN_0037CA78(0x42, 0x8000000044UL);
    LVL_3_ENDAKO_FUN_002F1898(0x100, 0x100);
    LVL_3_ENDAKO_FUN_0037CA78(0x42, 0x8000000044UL);
}

s32 LVL_3_ENDAKO_FUN_002B3798(s32 index) {
 s32 value=LVL_3_ENDAKO_FUN_002B3760(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_3_ENDAKO_FUN_002B37D0(s32 index) {
 return LVL_3_ENDAKO_FUN_002B3760(index)==47;
}

s32 LVL_3_ENDAKO_FUN_002B8C60(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_3_ENDAKO_FUN_00342C48(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_3_ENDAKO_D_00282530[13];
void LVL_3_ENDAKO_FUN_002F8008(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_3_ENDAKO_D_00282530[i].key == key) break;
    }
    if (i < 13) {
        LVL_3_ENDAKO_D_00282530[i].fields[9] = value;
        if (LVL_3_ENDAKO_D_00282530[i].busy == 0)
            LVL_3_ENDAKO_D_00282530[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_3_ENDAKO_D_001395B8[];
u32 LVL_3_ENDAKO_FUN_00306BE8(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_3_ENDAKO_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_3_ENDAKO_D_00231FC0[];
s32 LVL_3_ENDAKO_FUN_0037F358(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_3_ENDAKO_D_00231FC0;
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
extern int LVL_3_ENDAKO_D_0022E880[];
extern ListOverrideObject *LVL_3_ENDAKO_D_00227380[];
extern ListOverridePair LVL_3_ENDAKO_D_0022E280[];
void LVL_3_ENDAKO_FUN_0036C2B8(void)
{
    int *selected = LVL_3_ENDAKO_D_0022E880;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_3_ENDAKO_D_00227380[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_3_ENDAKO_D_0022E280[row->key];
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
extern NativeObjectSearchRecord32 LVL_3_ENDAKO_D_002567C0[];
s32 LVL_3_ENDAKO_FUN_00404480(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_3_ENDAKO_D_002567C0[index].field14 == object) {
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

s32 LVL_3_ENDAKO_FUN_002D7280(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_3_ENDAKO_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_3_ENDAKO_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_3_ENDAKO_D_00189E20)->mode == 0x31) {
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
extern void *LVL_3_ENDAKO_D_0018C0B0;
extern void *LVL_3_ENDAKO_D_0018B134;
extern void *LVL_3_ENDAKO_D_0018B040;
void *LVL_3_ENDAKO_FUN_002AB758(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_3_ENDAKO_D_0018C0B0;
    if (kind == 1)
        return LVL_3_ENDAKO_D_0018B134;
    if (kind == 6)
        return LVL_3_ENDAKO_D_0018B040;
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
extern unsigned char LVL_3_ENDAKO_D_00139568[];
extern MappedClassEntry LVL_3_ENDAKO_D_00266DA0[];
unsigned int LVL_3_ENDAKO_FUN_002F49B0(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_3_ENDAKO_D_00266DA0[LVL_3_ENDAKO_D_00139568[i]];
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
extern s32 LVL_3_ENDAKO_FUN_002EECC0(s32 value);
NativeCompactHeader *LVL_3_ENDAKO_FUN_003816B8(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_3_ENDAKO_FUN_002EECC0(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_3_ENDAKO_FUN_002EECC0(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_3_ENDAKO_D_001A79F0;
s32 LVL_3_ENDAKO_FUN_002D7200(void) {
    s32 found = 0;
    if (LVL_3_ENDAKO_D_001A79F0 == 25 || LVL_3_ENDAKO_D_001A79F0 == 5 ||
        LVL_3_ENDAKO_D_001A79F0 == 10 || LVL_3_ENDAKO_D_001A79F0 == 15) {
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
void LVL_3_ENDAKO_FUN_002D3C58(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_3_ENDAKO_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_3_ENDAKO_FUN_002D3C90(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_3_ENDAKO_D_00189E20;
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
extern ConditionalResetSlot LVL_3_ENDAKO_D_001B97C0[8];
void LVL_3_ENDAKO_FUN_002D7648(void) {
    ConditionalResetSlot *slot = LVL_3_ENDAKO_D_001B97C0;
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
extern StateTransitionView LVL_3_ENDAKO_D_001BFAC0;
void LVL_3_ENDAKO_FUN_002F4318(void) {
    if (LVL_3_ENDAKO_D_001BFAC0.mode == 7 && LVL_3_ENDAKO_D_001BFAC0.state == 1) {
        LVL_3_ENDAKO_D_001BFAC0.state = 2;
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
extern DobboFormatRoot396 LVL_3_ENDAKO_D_001CA020;
extern const char LVL_3_ENDAKO_D_001A9A20[];
extern const char LVL_3_ENDAKO_D_001A9A28[];
extern const unsigned char *LVL_3_ENDAKO_FUN_002F5570(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_3_ENDAKO_FUN_0030A268(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_3_ENDAKO_FUN_002F5570(LVL_3_ENDAKO_D_001CA020.rows[index].text_id);
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
        int key = LVL_3_ENDAKO_D_001CA020.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_3_ENDAKO_D_00266DA0[LVL_3_ENDAKO_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_3_ENDAKO_D_001A9A20, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_3_ENDAKO_D_001A9A28);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_3_ENDAKO_FUN_002B9358(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_3_ENDAKO_D_00189E20;
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
extern int LVL_3_ENDAKO_FUN_00360BE0(int, unsigned int, void *);
void LVL_3_ENDAKO_FUN_0044AA00(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_3_ENDAKO_FUN_00360BE0(3, 0, 0);
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
        LVL_3_ENDAKO_FUN_00360BE0(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_3_ENDAKO_FUN_00360BE0(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_3_ENDAKO_FUN_00360BE0(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_3_ENDAKO_D_001B2980[16];
extern u32 LVL_3_ENDAKO_D_001B29C0[16];

int LVL_3_ENDAKO_FUN_00312660(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_3_ENDAKO_D_001B2980[index] == 0 ||
            LVL_3_ENDAKO_D_001B2980[index] == object) {
            LVL_3_ENDAKO_D_001B2980[index] = object;
            LVL_3_ENDAKO_D_001B29C0[index] = 0;
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
extern void LVL_3_ENDAKO_FUN_002DD650(OozlaAppendObject164 *, int, const float *);
extern void LVL_3_ENDAKO_FUN_002EEFE0(float *, const float *, const float *);
extern void LVL_3_ENDAKO_FUN_002EF478(float *, const float *, const float *);
extern void LVL_3_ENDAKO_FUN_002DD998(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_3_ENDAKO_FUN_002DD5A8(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_3_ENDAKO_FUN_002DD650(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_3_ENDAKO_FUN_002EEFE0(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_3_ENDAKO_FUN_002EF478(difference, difference, &object->transform[0][0]);
        LVL_3_ENDAKO_FUN_002DD998(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_3_ENDAKO_FUN_0044FE78(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_3_ENDAKO_FUN_00450310(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_3_ENDAKO_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_3_ENDAKO_FUN_0032ECF8(void) {
    if (((CallState *)LVL_3_ENDAKO_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_3_ENDAKO_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_3_ENDAKO_D_001A63A8)->active); ((CallState *)LVL_3_ENDAKO_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_3_ENDAKO_FUN_0031B990(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_3_ENDAKO_FUN_00306B68(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_3_ENDAKO_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_3_ENDAKO_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

extern unsigned char D_19B278[];

int LVL_3_ENDAKO_FUN_00327618(void)
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

s32 LVL_3_ENDAKO_FUN_002B3840(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_3_ENDAKO_D_00189E20;

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

extern void LVL_3_ENDAKO_FUN_00311758(NativeUpdate775View *object);

void LVL_3_ENDAKO_FUN_003A4418(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_3_ENDAKO_FUN_00311758(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_3_ENDAKO_FUN_0032AAF0(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_3_ENDAKO_FUN_00320658(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_3_ENDAKO_FUN_002AD7E0(void)
{
}


unsigned int LVL_3_ENDAKO_FUN_002D7350(void)
{
    return 0;
}


void LVL_3_ENDAKO_FUN_002EE2B8(void)
{
}


void LVL_3_ENDAKO_FUN_002F5308(void)
{
}


void LVL_3_ENDAKO_FUN_002F60B8(void)
{
}


void LVL_3_ENDAKO_FUN_002FDBB8(void)
{
}


void LVL_3_ENDAKO_FUN_002FDBC0(void)
{
}


void LVL_3_ENDAKO_FUN_003018C8(void)
{
}


void LVL_3_ENDAKO_FUN_0030A620(void)
{
}


unsigned int LVL_3_ENDAKO_FUN_00370980(void)
{
    return 0;
}


void LVL_3_ENDAKO_FUN_00375468(void)
{
}


void LVL_3_ENDAKO_FUN_0037C480(void)
{
}


void LVL_3_ENDAKO_FUN_0037FE08(void)
{
}


void LVL_3_ENDAKO_FUN_003834E8(void)
{
}


int LVL_3_ENDAKO_FUN_003A4978(unsigned char *p) { return p[0x20] == 1; }


int LVL_3_ENDAKO_FUN_003CA138(unsigned char *p) { return p[0x20] == 1; }


int LVL_3_ENDAKO_FUN_003CA280(unsigned char *p) { return p[0x20] == 1; }


void LVL_3_ENDAKO_FUN_00401060(void)
{
}


void LVL_3_ENDAKO_FUN_00424C38(void)
{
}


void LVL_3_ENDAKO_FUN_0043D638(void)
{
}


void LVL_3_ENDAKO_FUN_0043F0A8(void)
{
}


void LVL_3_ENDAKO_FUN_0043F2F8(void)
{
}


void LVL_3_ENDAKO_FUN_0043F7F0(void)
{
}


void LVL_3_ENDAKO_FUN_00448068(void)
{
}


void LVL_3_ENDAKO_FUN_00448CC0(void)
{
}


void LVL_3_ENDAKO_FUN_00454AC8(void)
{
}


void LVL_3_ENDAKO_FUN_00456E58(void)
{
}


void LVL_3_ENDAKO_FUN_004586F0(void)
{
}


/* Three measured byte-state cases and width-specific field views.
   The correspondence name UpdateMoby_2426 does not establish an original
   class, enumeration, allocation or field-name declaration. */
extern void LVL_3_ENDAKO_STATE_HELPER_2426(u8 *, u8, s32);
extern u32 LVL_3_ENDAKO_STATUS_HELPER_2426(u8 *);
void LVL_3_ENDAKO_FUN_003CA9F8(u8 *entity) {
    switch (entity[0x20]) {
    case 0:
        LVL_3_ENDAKO_STATE_HELPER_2426(entity, 1, -1);
        *(u32 *)(entity + 0x98) = 0;
        *(unsigned short *)(entity + 0x34) =
            (*(unsigned short *)(entity + 0x34) | 0x41) & 0xEFFF;
        break;
    case 1:
        if (LVL_3_ENDAKO_STATUS_HELPER_2426(entity) != 0) {
            LVL_3_ENDAKO_STATE_HELPER_2426(entity, 2, -1);
            *(u32 *)(entity + 0x98) =
                *(u32 *)(*(u8 **)(entity + 0x24) + 0x10);
        }
        break;
    case 2:
        if (LVL_3_ENDAKO_STATUS_HELPER_2426(entity) == 0) {
            LVL_3_ENDAKO_STATE_HELPER_2426(entity, 1, -1);
            *(u32 *)(entity + 0x98) = 0;
        }
        break;
    }
}
void LVL_3_ENDAKO_FUN_003B97A8(char *p)
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
void LVL_3_ENDAKO_FUN_003CD960(char *p)
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
void LVL_3_ENDAKO_FUN_003DBC08(char *p)
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
void LVL_3_ENDAKO_FUN_003E69A0(char *p)
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
void LVL_3_ENDAKO_FUN_003E9F48(char *p)
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
void LVL_3_ENDAKO_FUN_003F0228(char *p)
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
void LVL_3_ENDAKO_FUN_003FC428(char *p)
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
void LVL_3_ENDAKO_FUN_003FD7D0(char *p)
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
void LVL_3_ENDAKO_FUN_00422E30(char *p)
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
void LVL_3_ENDAKO_FUN_00429E20(char *p)
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

extern u8 LVL_3_ENDAKO_F62e6ff2b_D_00189E20[];
extern u8 LVL_3_ENDAKO_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_3_ENDAKO_FUN_003607D8(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_3_ENDAKO_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_3_ENDAKO_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_3_ENDAKO_F62e6ff2b_D_00188660;
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
void LVL_3_ENDAKO_FUN_003A99D0(char *p)
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
void LVL_3_ENDAKO_FUN_003F3938(char *p)
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
extern Blob LVL_3_ENDAKO_F6894d7c1_D_001A8E60;
int LVL_3_ENDAKO_FUN_002F45E0(int x)
{
    Blob b;
    int i;
    b = LVL_3_ENDAKO_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
void LVL_3_ENDAKO_FUN_0038D560(char *p)
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
void LVL_3_ENDAKO_FUN_0038FBE8(char *p)
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
void LVL_3_ENDAKO_FUN_0042CCC0(char *p)
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

extern Entry *LVL_3_ENDAKO_Fc68ad20a_D_0018C2B8;

int LVL_3_ENDAKO_FUN_002D2A00(int key, int *out)
{
    Entry *e = LVL_3_ENDAKO_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_3_ENDAKO_F55a1acb8_D_001A63A8;
extern short LVL_3_ENDAKO_F55a1acb8_D_001A63AC;
extern int LVL_3_ENDAKO_F55a1acb8_FUN_00133688(void);
extern void LVL_3_ENDAKO_F55a1acb8_FUN_0011AEA0(int);

void LVL_3_ENDAKO_FUN_0032FDC8(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_3_ENDAKO_F55a1acb8_FUN_00133688()) {
        LVL_3_ENDAKO_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_3_ENDAKO_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f6;
    q = LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f18;
    LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f1C;
        LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_3_ENDAKO_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_3_ENDAKO_Fa2dbe766_D_00189E20[];

int LVL_3_ENDAKO_FUN_003D5718(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_3_ENDAKO_Fa2dbe766_D_00189E20;
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
typedef unsigned short u16;

void LVL_3_ENDAKO_FUN_00347A08(void) {
    *(u16 *)0x1aa842 = *(u8 *)0x1a7bc9 ? 3 : 0;
    *(u16 *)0x1aa85a = *(u8 *)0x1a7bca ? 3 : 0;
    *(u16 *)0x1aa872 = *(u8 *)0x1a7bcb ? 3 : 0;
    *(u16 *)0x1aa88a = *(u8 *)0x1a7bcc ? 3 : 0;
    *(u16 *)0x1aa8a2 = *(u8 *)0x1a7bce ? 3 : 0;
}
void LVL_3_ENDAKO_FUN_00415350(char *object)
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
extern char LVL_3_ENDAKO_Fea34650e_D_00189E20[];

void LVL_3_ENDAKO_FUN_002D3AF0(void)
{
    char *b = LVL_3_ENDAKO_Fea34650e_D_00189E20;
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
        char *c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_3_ENDAKO_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_3_ENDAKO_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_3_ENDAKO_FUN_003F4010(char *p)
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
void LVL_3_ENDAKO_FUN_0042CC58(char *p)
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

extern struct Table1 LVL_3_ENDAKO_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_3_ENDAKO_Fee2b87d1_D_00152CD0;

s32 LVL_3_ENDAKO_FUN_003062A8(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_3_ENDAKO_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_3_ENDAKO_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_3_ENDAKO_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_3_ENDAKO_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
extern void LVL_3_ENDAKO_F0b67d264_FUN_00115E38(char *a, int b, char *c);
extern char LVL_3_ENDAKO_F0b67d264_D_001ADD58[];
extern char LVL_3_ENDAKO_F0b67d264_D_001ADD78[];

void LVL_3_ENDAKO_FUN_0043D650(int *p, unsigned int n, int a2, int a3)
{
    if (n < 4)
        LVL_3_ENDAKO_F0b67d264_FUN_00115E38(LVL_3_ENDAKO_F0b67d264_D_001ADD58, 37, LVL_3_ENDAKO_F0b67d264_D_001ADD78);

    p[1] = a3;
    p[2] = n;
    p[4] = 0;
    p[5] = 0;
    p[3] = 0;
    p[0] = a2;
}
/* Paired-strip packet emitter, 396 bytes, placed in levels/19_grelbin and
   levels/3_endako.  The body writes a 16-byte GIF header into the
   resident packet cursor (a small-data global), advances the cursor, then
   writes the tag words and two packed 64-bit strip descriptors.  The retail
   loads the cursor with LUI/LO and advances it through $gp, so the unit is
   compiled under the qualified small-data profile (-O2 -G8). */
extern int *LVL_3_ENDAKO_F5e27257c_D_001B3188 __attribute__((sda));
extern int LVL_3_ENDAKO_F5e27257c_D_001A7350 __attribute__((sda));
extern int LVL_3_ENDAKO_F5e27257c_D_001A7354 __attribute__((sda));

void LVL_3_ENDAKO_FUN_002FC668(int a0, int a1, int a2, int a3, int p4, int p5)
{
    long long *q;

    *(int *)((char *)LVL_3_ENDAKO_F5e27257c_D_001B3188 + 0) = 0x10000003;
    *(int *)((char *)LVL_3_ENDAKO_F5e27257c_D_001B3188 + 4) = 0;
    *(int *)((char *)LVL_3_ENDAKO_F5e27257c_D_001B3188 + 8) = 0;
    *(int *)((char *)LVL_3_ENDAKO_F5e27257c_D_001B3188 + 12) = 0x50000003;
    q = (long long *)LVL_3_ENDAKO_F5e27257c_D_001B3188;
    LVL_3_ENDAKO_F5e27257c_D_001B3188 = (int *)((char *)q + 16);
    q[2] = 0x4400000000008001LL;
    q[3] = 17424;
    q[4] = 70;
    q[5] = p4;
#define LO_D0 (*(int *)((char *)&LVL_3_ENDAKO_F5e27257c_D_001A7350 + 0))
#define HI_D4 (*(int *)((char *)&LVL_3_ENDAKO_F5e27257c_D_001A7354 + 0))

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
    LVL_3_ENDAKO_F5e27257c_D_001B3188 = (int *)((char *)LVL_3_ENDAKO_F5e27257c_D_001B3188 + 48);
}
/* Family 430eca3b2fc8d2b0 (112 B, 2 placements).
   A search over the resident table reached through LVL_3_ENDAKO_F430eca3b_D_001AA7B0: the first entry
   is tested outside the loop, the loop scans the rest, and the found index is
   re-read.  Every access names the global itself, so cc1 merges the loads into
   one register and keeps the index arithmetic (no strength reduction), which
   is the retail shape.  The one small-data global LVL_3_ENDAKO_F430eca3b_D_001A79F0 is loaded in the
   branch delay slot, so the qualified small-data profile is required. */

extern int *LVL_3_ENDAKO_F430eca3b_D_001AA7B0 __attribute__((sda));
extern int LVL_3_ENDAKO_F430eca3b_D_001A79F0 __attribute__((sda));

int LVL_3_ENDAKO_FUN_00307730(int a0)
{
    int r = -1;
    int i = 0;

    if (LVL_3_ENDAKO_F430eca3b_D_001AA7B0[0] == a0) {
        r = 0;
    } else {
        while (i < 28) {
            if (LVL_3_ENDAKO_F430eca3b_D_001AA7B0[i] == a0) {
                r = i;
                break;
            }
            i++;
        }
    }
    if (LVL_3_ENDAKO_F430eca3b_D_001AA7B0[r] == 0 && LVL_3_ENDAKO_F430eca3b_D_001A79F0) {
        r = -1;
    }
    return r;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_3_ENDAKO_F1157be91_D_001A63E8;
extern unsigned char LVL_3_ENDAKO_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_3_ENDAKO_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_3_ENDAKO_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_3_ENDAKO_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_3_ENDAKO_F1157be91_FUN_00133230(void);
extern int LVL_3_ENDAKO_F1157be91_FUN_00132028(void);

int LVL_3_ENDAKO_FUN_0032FC50(int a0, int a1, int a2) {
    CdMode mode = LVL_3_ENDAKO_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_3_ENDAKO_F1157be91_D_001A7900[0];
    LVL_3_ENDAKO_F1157be91_D_001A7430[0] = 0;
    LVL_3_ENDAKO_F1157be91_D_001A7434 = 0;
    LVL_3_ENDAKO_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_3_ENDAKO_F1157be91_FUN_00133230();
    LVL_3_ENDAKO_F1157be91_FUN_00132028();
    return 1;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_3_ENDAKO_F4e5bde81_D_00189E20;
extern s32 LVL_3_ENDAKO_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_3_ENDAKO_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_3_ENDAKO_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_3_ENDAKO_FUN_002D3E88(void) {
    s32 result = LVL_3_ENDAKO_F4e5bde81_D_00189E20.field348;
    if (LVL_3_ENDAKO_F4e5bde81_D_001A8FF0 != 0 && LVL_3_ENDAKO_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_3_ENDAKO_F4e5bde81_D_001A8FF4 != 0 || LVL_3_ENDAKO_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_3_ENDAKO_F4e5bde81_D_00189E20.field2294 == 110 && LVL_3_ENDAKO_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_3_ENDAKO_F4e5bde81_D_00189E20.field2294 == 109 || LVL_3_ENDAKO_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_3_ENDAKO_F4e5bde81_D_00189E20.field1497 != 0 && LVL_3_ENDAKO_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_3_ENDAKO_F4e5bde81_D_00189E20.field2294 == 0 && LVL_3_ENDAKO_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_3_ENDAKO_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}

extern u8 LVL_3_ENDAKO_Fd1172b6a_D_001D2540[];

void LVL_3_ENDAKO_FUN_003117B0(u8 *param_1)
{
    if (param_1[66] != 255) {
        u8 *tbl = (u8 *)(*(volatile int *)(param_1 + 36) + 72);

        *(int *)(param_1 + 88) = *(int *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + (param_1[64] << 2) + 28);
        param_1[110] = *(u8 *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + 18);
        param_1[108] = *(u8 *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + 17);
    } else {
        param_1[108] = 255;
        param_1[110] = 0;
        *(int *)(param_1 + 88) = (int)(LVL_3_ENDAKO_Fd1172b6a_D_001D2540 + (param_1[64] << 11));
    }

    *(int *)(param_1 + 92) = *(int *)(*(int *)(*(volatile int *)(param_1 + 36) + (param_1[67] << 2) + 72) + (param_1[65] << 2) + 28);
}
extern void LVL_3_ENDAKO_F0477ffed_FUN_00115DA8(int a, char *fmt, ...);
extern char LVL_3_ENDAKO_F0477ffed_D_001AD630[]; extern char LVL_3_ENDAKO_F0477ffed_D_001AD640[]; extern char LVL_3_ENDAKO_F0477ffed_D_001AD648[];
void LVL_3_ENDAKO_FUN_00376E68(int out, int v) {
    if (v > 999999) {
        LVL_3_ENDAKO_F0477ffed_FUN_00115DA8(out, LVL_3_ENDAKO_F0477ffed_D_001AD630, v / 1000000, (v / 1000) % 1000, v % 1000);
    } else if (v >= 1000) {
        LVL_3_ENDAKO_F0477ffed_FUN_00115DA8(out, LVL_3_ENDAKO_F0477ffed_D_001AD640, v / 1000, v % 1000);
    } else {
        LVL_3_ENDAKO_F0477ffed_FUN_00115DA8(out, LVL_3_ENDAKO_F0477ffed_D_001AD648, v);
    }
}
extern void LVL_3_ENDAKO_F7b754363_FUN_00115E38(char *a, int b, char *c);
extern char LVL_3_ENDAKO_F7b754363_D_001ADD58[];
extern char LVL_3_ENDAKO_F7b754363_D_001ADDA0[];

int LVL_3_ENDAKO_FUN_0043D6D8(unsigned int *p)
{
    unsigned int *n = (unsigned int *)p[5];
    unsigned int offset;
    unsigned int result;

    if (n != 0) {
        p[5] = n[0];
        p[4] = p[4] + 1;
        return (int)n;
    }

    offset = p[3];

    if (p[1] < offset + p[2]) {
        LVL_3_ENDAKO_F7b754363_FUN_00115E38(LVL_3_ENDAKO_F7b754363_D_001ADD58, 83, LVL_3_ENDAKO_F7b754363_D_001ADDA0);
        return 0;
    }

    p[3] = offset + p[2];
    result = p[0] + offset;
    p[4] = p[4] + 1;
    return result;
}
extern int *LVL_3_ENDAKO_Fcceeec15_D_001B3188 __attribute__((sda));
extern char LVL_3_ENDAKO_Fcceeec15_D_001A6D40[];
extern char LVL_3_ENDAKO_Fcceeec15_D_001A6E90[];

void LVL_3_ENDAKO_FUN_002F1440(int param_1)
{
    if (LVL_3_ENDAKO_Fcceeec15_D_001B3188 == 0) {
        return;
    }

    *(int *)LVL_3_ENDAKO_Fcceeec15_D_001B3188 = 0x30000015;

    if (param_1 == 0) {
        *(int *)((char *)LVL_3_ENDAKO_Fcceeec15_D_001B3188 + 4) = (int)LVL_3_ENDAKO_Fcceeec15_D_001A6D40;
    } else {
        *(int *)((char *)LVL_3_ENDAKO_Fcceeec15_D_001B3188 + 4) = (int)LVL_3_ENDAKO_Fcceeec15_D_001A6E90;
    }

    *(int *)((char *)LVL_3_ENDAKO_Fcceeec15_D_001B3188 + 8) = 0;
    *(int *)((char *)LVL_3_ENDAKO_Fcceeec15_D_001B3188 + 12) = 0x50000015;
    LVL_3_ENDAKO_Fcceeec15_D_001B3188 = (int *)((char *)LVL_3_ENDAKO_Fcceeec15_D_001B3188 + 16);
}
void LVL_3_ENDAKO_FUN_00321810(int param_1, short *param_2, short param_3)
{
    int i;

    param_1 = (param_1 - *(int *)0x001B2A1C) << 8 >> 16;
    for (i = 1; i <= param_2[0]; i++) {
        if (param_2[i] == param_1) return;
    }
    if (param_2[0] < param_3) {
        param_2[0] = param_2[0] + 1;
        param_2[param_2[0]] = param_1;
    }
}
extern int LVL_3_ENDAKO_F9306cc1f_D_001CA2A8[];

int LVL_3_ENDAKO_FUN_003073A0(int arg)
{
    int *t = LVL_3_ENDAKO_F9306cc1f_D_001CA2A8;
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
extern int LVL_3_ENDAKO_F8a42bc68_D_002325D0[];

int LVL_3_ENDAKO_FUN_00381458(int value)
{
    int i;
    int found = 0;

    for (i = 0; i < LVL_3_ENDAKO_F8a42bc68_D_002325D0[64]; i++)
    {
        if (LVL_3_ENDAKO_F8a42bc68_D_002325D0[i] == value)
        {
            found = 1;
            break;
        }
    }
    return found;
}
/* Family 435a49e62f53b8db - 368 bytes, 2 members.
   Placements: levels/3_endako @0x002FC368, levels/19_grelbin @0x002F8AF8.

   Measured binding.  LVL_3_ENDAKO_F435a49e6_D_001B3188 is a four-byte pointer in the resident image
   (0x001B3188 = 1782152).  The retail body reads it with a per-site
   `lui $r,%hi` + `lw $r,%lo($r)` pair and writes it with a single
   `sw $r,%lo($gp)`; all three of the latter sit in a compiler delay slot
   (two in the `b` before the branch target, one in the `jr $ra` slot).

   That mix is exactly gas's expansion of the bare-symbol macro form
   `lw $r,LVL_3_ENDAKO_F435a49e6_D_001B3188` / `sw $r,LVL_3_ENDAKO_F435a49e6_D_001B3188`: absolute `lui`+`%lo` in ordinary
   flow, `$gp` (GPREL16) inside a `.set nomacro` region.  cc1 emits that
   macro only for an `sda` symbol, so the declaration carries `sda`.
   Without it cc1 computes the symbol's address once, CSEs it into a base
   register and reloads `0($base)` per site, which also costs an extra
   `move` to keep the incoming `$a1` alive across that register - 344
   bytes against the 368 of the reference. */

extern int *LVL_3_ENDAKO_F435a49e6_D_001B3188 __attribute__((sda));
extern void LVL_3_ENDAKO_F435a49e6_FUN_00126288(void *, short, short, int, int, int, short, short);
extern void LVL_3_ENDAKO_F435a49e6_FUN_0011AEA0(int);
extern void LVL_3_ENDAKO_F435a49e6_FUN_001265B0(void *, int);

void LVL_3_ENDAKO_FUN_002FC368(int a0, int a1, int a2, int a3, int a4, int a5)
{
    char buf[96];
    void *work;
    int t4;
    int s2;

    s2 = 1 << (a3 + a4 - 4);
    t4 = (1 << a3) >> 6;
    if (t4 <= 0)
        t4 = 1;

    if (a5 == 0) {
        *(int *)LVL_3_ENDAKO_F435a49e6_D_001B3188 = 0x10000006;
        *(int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 4) = 0;
        *(int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 8) = 0;
        *(int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 12) = 0x50000006;
        work = (char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 16;
        LVL_3_ENDAKO_F435a49e6_D_001B3188 = (int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 112);
    } else {
        work = buf;
    }

    LVL_3_ENDAKO_F435a49e6_FUN_00126288(work, (short)a1, (short)t4, (short)a2, 0, 0,
            (short)(1 << a3), (short)(1 << a4));

    if (a5 == 0) {
        *(int *)LVL_3_ENDAKO_F435a49e6_D_001B3188 = 0x30000000 | s2;
        *(int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 4) = a0;
        *(int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 8) = 0;
        *(int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 12) = 0x50000000 | s2;
        LVL_3_ENDAKO_F435a49e6_D_001B3188 = (int *)((char *)LVL_3_ENDAKO_F435a49e6_D_001B3188 + 16);
    } else {
        LVL_3_ENDAKO_F435a49e6_FUN_0011AEA0(0);
        LVL_3_ENDAKO_F435a49e6_FUN_001265B0(work, a0);
    }
}
/* RAC2 family 0820eb1123250c0b - 192 bytes, 2 placements.
 * Slot0820eb11 allocator: find the first free of six 64-byte slots, fill it in and
 * link it at the head of the context's list.
 */

typedef struct Slot0820eb11 {
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
} Slot0820eb11;                    /* 64 bytes */

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

extern Slot0820eb11 LVL_3_ENDAKO_F0820eb11_D_001D23C0[6];
extern unsigned char LVL_3_ENDAKO_F0820eb11_D_001CA5C0[];

Slot0820eb11 *LVL_3_ENDAKO_FUN_00311EE0(Ctx *ctx, int index)
{
    int i;
    Slot0820eb11 *slot;
    unsigned char *p;

    for (i = 0; i < 6; i++) {
        if (LVL_3_ENDAKO_F0820eb11_D_001D23C0[i].f04 == 0) {
            break;
        }
    }
    slot = &LVL_3_ENDAKO_F0820eb11_D_001D23C0[i];
    slot->f04 = 1;
    slot->f00 = index;
    slot->f10 = (int)(LVL_3_ENDAKO_F0820eb11_D_001CA5C0 + i * 5376);
    slot->f14 = (int)ctx->f24;
    p = (unsigned char *)*(unsigned int *)(ctx->f24->f1C + (short)index * 4 + 4);
    slot->f02 = p[2];
    slot->f0C = p + (p[0] + 4);
    slot->f1C = ctx->f50;
    ctx->f50 = (int)slot;
    return slot;
}
