typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_7_SIBERIUS_D_001A8EB0;
void LVL_7_SIBERIUS_FUN_002CD8F0(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_7_SIBERIUS_FUN_002E9FF0(s32 index) {
    NativeTable20 values=LVL_7_SIBERIUS_D_001A8EB0;
    return values.items[index];
}

u32 LVL_7_SIBERIUS_FUN_002ACDF0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_7_SIBERIUS_D_0018C0B4;
u32 LVL_7_SIBERIUS_FUN_002CC898(void) {
    return LVL_7_SIBERIUS_D_0018C0B4;
}

s32 LVL_7_SIBERIUS_FUN_002D9AC8(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_7_SIBERIUS_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_7_SIBERIUS_FUN_002ACC88(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_7_SIBERIUS_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_7_SIBERIUS_FUN_002ACCC0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_7_SIBERIUS_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_7_SIBERIUS_FUN_002AD8E0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_7_SIBERIUS_FUN_002CD648(f32, f32, f32, f32, s32, s32);

void LVL_7_SIBERIUS_FUN_002CD0F0(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_7_SIBERIUS_FUN_002CD648(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_7_SIBERIUS_FUN_002E6848(int width, int height, int address, int mode);
extern void LVL_7_SIBERIUS_FUN_003715E0(unsigned int reg, unsigned long value);
extern void LVL_7_SIBERIUS_FUN_002E6BB0(int width, int height);

void LVL_7_SIBERIUS_FUN_002DB868(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_7_SIBERIUS_FUN_002E6848(width, height, address, 1);
    LVL_7_SIBERIUS_FUN_003715E0(0x47, 0x30000UL);
    LVL_7_SIBERIUS_FUN_003715E0(0x42, 0x8000000044UL);
    LVL_7_SIBERIUS_FUN_002E6BB0(0x100, 0x100);
    LVL_7_SIBERIUS_FUN_003715E0(0x42, 0x8000000044UL);
}

s32 LVL_7_SIBERIUS_FUN_002ACCF8(s32 index) {
 s32 value=LVL_7_SIBERIUS_FUN_002ACCC0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_7_SIBERIUS_FUN_002ACD30(s32 index) {
 return LVL_7_SIBERIUS_FUN_002ACCC0(index)==47;
}

s32 LVL_7_SIBERIUS_FUN_002B1FE0(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Update the measured fields of the first matching key among13records. */
typedef struct { unsigned int fields[25]; int key; unsigned int busy; unsigned int tail[9]; } UpdateRecord;
typedef char UpdateRecordSize[(sizeof(UpdateRecord) == 144) ? 1 : -1];
extern UpdateRecord LVL_7_SIBERIUS_D_0027E030[13];
void LVL_7_SIBERIUS_FUN_002ED0F8(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_7_SIBERIUS_D_0027E030[i].key == key) break;
    }
    if (i < 13) {
        LVL_7_SIBERIUS_D_0027E030[i].fields[9] = value;
        if (LVL_7_SIBERIUS_D_0027E030[i].busy == 0)
            LVL_7_SIBERIUS_D_0027E030[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_7_SIBERIUS_D_001395B8[];
u32 LVL_7_SIBERIUS_FUN_002FBCF0(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_7_SIBERIUS_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_7_SIBERIUS_D_00231780[];
s32 LVL_7_SIBERIUS_FUN_00373EC0(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_7_SIBERIUS_D_00231780;
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
extern int LVL_7_SIBERIUS_D_0022E040[];
extern ListOverrideObject *LVL_7_SIBERIUS_D_00226B40[];
extern ListOverridePair LVL_7_SIBERIUS_D_0022DA40[];
void LVL_7_SIBERIUS_FUN_00360CD0(void)
{
    int *selected = LVL_7_SIBERIUS_D_0022E040;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_7_SIBERIUS_D_00226B40[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_7_SIBERIUS_D_0022DA40[row->key];
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
extern NativeObjectSearchRecord32 LVL_7_SIBERIUS_D_002537F0[];
s32 LVL_7_SIBERIUS_FUN_003E42E0(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_7_SIBERIUS_D_002537F0[index].field14 == object) {
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

s32 LVL_7_SIBERIUS_FUN_002CC758(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_7_SIBERIUS_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_7_SIBERIUS_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_7_SIBERIUS_D_00189E20)->mode == 0x31) {
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
extern void *LVL_7_SIBERIUS_D_0018C0B0;
extern void *LVL_7_SIBERIUS_D_0018B134;
extern void *LVL_7_SIBERIUS_D_0018B040;
void *LVL_7_SIBERIUS_FUN_002A55C0(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_7_SIBERIUS_D_0018C0B0;
    if (kind == 1)
        return LVL_7_SIBERIUS_D_0018B134;
    if (kind == 6)
        return LVL_7_SIBERIUS_D_0018B040;
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
extern unsigned char LVL_7_SIBERIUS_D_00139568[];
extern MappedClassEntry LVL_7_SIBERIUS_D_002628A0[];
unsigned int LVL_7_SIBERIUS_FUN_002E9CC8(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_7_SIBERIUS_D_002628A0[LVL_7_SIBERIUS_D_00139568[i]];
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
extern s32 LVL_7_SIBERIUS_FUN_002E4050(s32 value);
NativeCompactHeader *LVL_7_SIBERIUS_FUN_00376220(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_7_SIBERIUS_FUN_002E4050(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_7_SIBERIUS_FUN_002E4050(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_7_SIBERIUS_D_001A79F0;
s32 LVL_7_SIBERIUS_FUN_002CC6D8(void) {
    s32 found = 0;
    if (LVL_7_SIBERIUS_D_001A79F0 == 25 || LVL_7_SIBERIUS_D_001A79F0 == 5 ||
        LVL_7_SIBERIUS_D_001A79F0 == 10 || LVL_7_SIBERIUS_D_001A79F0 == 15) {
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
extern ConditionalResetSlot LVL_7_SIBERIUS_D_001B9040[8];
void LVL_7_SIBERIUS_FUN_002CCB20(void) {
    ConditionalResetSlot *slot = LVL_7_SIBERIUS_D_001B9040;
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
extern StateTransitionView LVL_7_SIBERIUS_D_001BF340;
void LVL_7_SIBERIUS_FUN_002E9630(void) {
    if (LVL_7_SIBERIUS_D_001BF340.mode == 7 && LVL_7_SIBERIUS_D_001BF340.state == 1) {
        LVL_7_SIBERIUS_D_001BF340.state = 2;
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
extern DobboFormatRoot396 LVL_7_SIBERIUS_D_001C97E0;
extern const char LVL_7_SIBERIUS_D_001A9A20[];
extern const char LVL_7_SIBERIUS_D_001A9A28[];
extern const unsigned char *LVL_7_SIBERIUS_FUN_002EA888(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_7_SIBERIUS_FUN_002FF370(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_7_SIBERIUS_FUN_002EA888(LVL_7_SIBERIUS_D_001C97E0.rows[index].text_id);
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
        int key = LVL_7_SIBERIUS_D_001C97E0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_7_SIBERIUS_D_002628A0[LVL_7_SIBERIUS_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_7_SIBERIUS_D_001A9A20, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_7_SIBERIUS_D_001A9A28);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_7_SIBERIUS_FUN_002B2690(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_7_SIBERIUS_D_00189E20;
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
extern int LVL_7_SIBERIUS_FUN_003555E8(int, unsigned int, void *);
void LVL_7_SIBERIUS_FUN_00435628(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_7_SIBERIUS_FUN_003555E8(3, 0, 0);
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
        LVL_7_SIBERIUS_FUN_003555E8(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_7_SIBERIUS_FUN_003555E8(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_7_SIBERIUS_FUN_003555E8(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_7_SIBERIUS_D_001B2240[16];
extern u32 LVL_7_SIBERIUS_D_001B2280[16];

int LVL_7_SIBERIUS_FUN_00307708(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_7_SIBERIUS_D_001B2240[index] == 0 ||
            LVL_7_SIBERIUS_D_001B2240[index] == object) {
            LVL_7_SIBERIUS_D_001B2240[index] = object;
            LVL_7_SIBERIUS_D_001B2280[index] = 0;
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
extern void LVL_7_SIBERIUS_FUN_002D2B28(OozlaAppendObject164 *, int, const float *);
extern void LVL_7_SIBERIUS_FUN_002E4340(float *, const float *, const float *);
extern void LVL_7_SIBERIUS_FUN_002E47D0(float *, const float *, const float *);
extern void LVL_7_SIBERIUS_FUN_002D2E70(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_7_SIBERIUS_FUN_002D2A80(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_7_SIBERIUS_FUN_002D2B28(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_7_SIBERIUS_FUN_002E4340(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_7_SIBERIUS_FUN_002E47D0(difference, difference, &object->transform[0][0]);
        LVL_7_SIBERIUS_FUN_002D2E70(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_7_SIBERIUS_FUN_0043AAA0(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_7_SIBERIUS_FUN_0043AF38(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

extern unsigned char D_19B278[];

int LVL_7_SIBERIUS_FUN_0031D0E8(void)
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
extern u8 LVL_7_SIBERIUS_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_7_SIBERIUS_FUN_00323E48(void) {
    if (((CallState *)LVL_7_SIBERIUS_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_7_SIBERIUS_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_7_SIBERIUS_D_001A63A8)->active); ((CallState *)LVL_7_SIBERIUS_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_7_SIBERIUS_FUN_00310A38(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_7_SIBERIUS_FUN_002FBC70(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_7_SIBERIUS_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_7_SIBERIUS_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

/* Prototype775 is a research label. Field names below describe only the
 * independently observed byte23 and binary32 word2C operations. */
typedef struct {
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[8];
    f32 field2C;
} NativeUpdate775View;

extern void LVL_7_SIBERIUS_FUN_00306800(NativeUpdate775View *object);

void LVL_7_SIBERIUS_FUN_00394AF8(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_7_SIBERIUS_FUN_00306800(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_7_SIBERIUS_FUN_0031FAB0(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_7_SIBERIUS_FUN_003157E8(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_7_SIBERIUS_FUN_002A70E0(void)
{
}


unsigned int LVL_7_SIBERIUS_FUN_002CC828(void)
{
    return 0;
}


void LVL_7_SIBERIUS_FUN_002E3648(void)
{
}


void LVL_7_SIBERIUS_FUN_002EA620(void)
{
}


void LVL_7_SIBERIUS_FUN_002EB3D0(void)
{
}


void LVL_7_SIBERIUS_FUN_002F2C68(void)
{
}


void LVL_7_SIBERIUS_FUN_002F69D0(void)
{
}


void LVL_7_SIBERIUS_FUN_002FF728(void)
{
}


unsigned int LVL_7_SIBERIUS_FUN_003654F8(void)
{
    return 0;
}


void LVL_7_SIBERIUS_FUN_00369FD0(void)
{
}


void LVL_7_SIBERIUS_FUN_00370FE8(void)
{
}


void LVL_7_SIBERIUS_FUN_00374970(void)
{
}


void LVL_7_SIBERIUS_FUN_00378050(void)
{
}


void LVL_7_SIBERIUS_FUN_003E0B08(void)
{
}


void LVL_7_SIBERIUS_FUN_0040E190(void)
{
}


void LVL_7_SIBERIUS_FUN_0040F6B0(void)
{
}


void LVL_7_SIBERIUS_FUN_00428260(void)
{
}


void LVL_7_SIBERIUS_FUN_00429CD0(void)
{
}


void LVL_7_SIBERIUS_FUN_00429F20(void)
{
}


void LVL_7_SIBERIUS_FUN_0042A418(void)
{
}


void LVL_7_SIBERIUS_FUN_00432C90(void)
{
}


void LVL_7_SIBERIUS_FUN_004338E8(void)
{
}


void LVL_7_SIBERIUS_FUN_0043F6F0(void)
{
}


void LVL_7_SIBERIUS_FUN_00441A80(void)
{
}


void LVL_7_SIBERIUS_FUN_00443318(void)
{
}
void LVL_7_SIBERIUS_FUN_003A9E10(char *p)
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
void LVL_7_SIBERIUS_FUN_003B1F68(char *p)
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
void LVL_7_SIBERIUS_FUN_003C0950(char *p)
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
void LVL_7_SIBERIUS_FUN_003C77F8(char *p)
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
void LVL_7_SIBERIUS_FUN_003CAB80(char *p)
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
void LVL_7_SIBERIUS_FUN_003CB8D0(char *p)
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
void LVL_7_SIBERIUS_FUN_003D2068(char *p)
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
void LVL_7_SIBERIUS_FUN_003DBED0(char *p)
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
void LVL_7_SIBERIUS_FUN_003DD278(char *p)
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
void LVL_7_SIBERIUS_FUN_0040C390(char *p)
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
void LVL_7_SIBERIUS_FUN_004150F0(char *p)
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

extern u8 LVL_7_SIBERIUS_F62e6ff2b_D_00189E20[];
extern u8 LVL_7_SIBERIUS_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_7_SIBERIUS_FUN_003551E0(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_7_SIBERIUS_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_7_SIBERIUS_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_7_SIBERIUS_F62e6ff2b_D_00188660;
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
void LVL_7_SIBERIUS_FUN_0039A038(char *p)
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
void LVL_7_SIBERIUS_FUN_003D4BC8(char *p)
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
extern Blob LVL_7_SIBERIUS_F6894d7c1_D_001A8E60;
int LVL_7_SIBERIUS_FUN_002E98F8(int x)
{
    Blob b;
    int i;
    b = LVL_7_SIBERIUS_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
void LVL_7_SIBERIUS_FUN_0037E250(char *p)
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
void LVL_7_SIBERIUS_FUN_003808D8(char *p)
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
void LVL_7_SIBERIUS_FUN_003E9B08(char *p)
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
void LVL_7_SIBERIUS_FUN_003ED798(char *p)
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
void LVL_7_SIBERIUS_FUN_004177E0(char *p)
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

extern Entry *LVL_7_SIBERIUS_Fc68ad20a_D_0018C2B8;

int LVL_7_SIBERIUS_FUN_002C7F68(int key, int *out)
{
    Entry *e = LVL_7_SIBERIUS_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_7_SIBERIUS_F55a1acb8_D_001A63A8;
extern short LVL_7_SIBERIUS_F55a1acb8_D_001A63AC;
extern int LVL_7_SIBERIUS_F55a1acb8_FUN_00133688(void);
extern void LVL_7_SIBERIUS_F55a1acb8_FUN_0011AEA0(int);

void LVL_7_SIBERIUS_FUN_00324F18(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_7_SIBERIUS_F55a1acb8_FUN_00133688()) {
        LVL_7_SIBERIUS_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_7_SIBERIUS_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f6;
    q = LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f18;
    LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f1C;
        LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_7_SIBERIUS_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_7_SIBERIUS_Fa2dbe766_D_00189E20[];

int LVL_7_SIBERIUS_FUN_003BA460(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_7_SIBERIUS_Fa2dbe766_D_00189E20;
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
struct Rec { s32 f0, f1, f2, f3, f4, f5, f6, f7, f8, f9; };
extern struct Rec LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[32];
void LVL_7_SIBERIUS_FUN_002D8C18(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                       s32 g, s32 h, s32 i, s32 j, s32 idx) {
    if ((unsigned)idx < 32) {
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f0 = a;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f1 = b;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f2 = c;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f3 = d;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f4 = e;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f5 = f;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f6 = g;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f7 = h;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f8 = i;
        (LVL_7_SIBERIUS_F480de5ac_D_001BC9C8[idx]).f9 = j;

    }
}
typedef unsigned short u16;

void LVL_7_SIBERIUS_FUN_0033C410(void) {
    *(u16 *)0x1aa842 = *(u8 *)0x1a7bc9 ? 3 : 0;
    *(u16 *)0x1aa85a = *(u8 *)0x1a7bca ? 3 : 0;
    *(u16 *)0x1aa872 = *(u8 *)0x1a7bcb ? 3 : 0;
    *(u16 *)0x1aa88a = *(u8 *)0x1a7bcc ? 3 : 0;
    *(u16 *)0x1aa8a2 = *(u8 *)0x1a7bce ? 3 : 0;
}
void LVL_7_SIBERIUS_FUN_00403EB8(char *object)
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
extern char LVL_7_SIBERIUS_Fea34650e_D_00189E20[];

void LVL_7_SIBERIUS_FUN_002C9058(void)
{
    char *b = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
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
        char *c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_7_SIBERIUS_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_7_SIBERIUS_FUN_003D52A0(char *p)
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
void LVL_7_SIBERIUS_FUN_00417778(char *p)
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

extern struct Table1 LVL_7_SIBERIUS_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_7_SIBERIUS_Fee2b87d1_D_00152CD0;

s32 LVL_7_SIBERIUS_FUN_002FB3B0(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_7_SIBERIUS_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_7_SIBERIUS_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_7_SIBERIUS_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_7_SIBERIUS_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
extern void LVL_7_SIBERIUS_F0b67d264_FUN_00115E38(char *a, int b, char *c);
extern char LVL_7_SIBERIUS_F0b67d264_D_001ADD58[];
extern char LVL_7_SIBERIUS_F0b67d264_D_001ADD78[];

void LVL_7_SIBERIUS_FUN_00428278(int *p, unsigned int n, int a2, int a3)
{
    if (n < 4)
        LVL_7_SIBERIUS_F0b67d264_FUN_00115E38(LVL_7_SIBERIUS_F0b67d264_D_001ADD58, 37, LVL_7_SIBERIUS_F0b67d264_D_001ADD78);

    p[1] = a3;
    p[2] = n;
    p[4] = 0;
    p[5] = 0;
    p[3] = 0;
    p[0] = a2;
}
/* Family 430eca3b2fc8d2b0 (112 B, 2 placements).
   A search over the resident table reached through LVL_7_SIBERIUS_F430eca3b_D_001AA7B0: the first entry
   is tested outside the loop, the loop scans the rest, and the found index is
   re-read.  Every access names the global itself, so cc1 merges the loads into
   one register and keeps the index arithmetic (no strength reduction), which
   is the retail shape.  The one small-data global LVL_7_SIBERIUS_F430eca3b_D_001A79F0 is loaded in the
   branch delay slot, so the qualified small-data profile is required. */

extern int *LVL_7_SIBERIUS_F430eca3b_D_001AA7B0 __attribute__((sda));
extern int LVL_7_SIBERIUS_F430eca3b_D_001A79F0 __attribute__((sda));

int LVL_7_SIBERIUS_FUN_002FC838(int a0)
{
    int r = -1;
    int i = 0;

    if (LVL_7_SIBERIUS_F430eca3b_D_001AA7B0[0] == a0) {
        r = 0;
    } else {
        while (i < 28) {
            if (LVL_7_SIBERIUS_F430eca3b_D_001AA7B0[i] == a0) {
                r = i;
                break;
            }
            i++;
        }
    }
    if (LVL_7_SIBERIUS_F430eca3b_D_001AA7B0[r] == 0 && LVL_7_SIBERIUS_F430eca3b_D_001A79F0) {
        r = -1;
    }
    return r;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_7_SIBERIUS_F1157be91_D_001A63E8;
extern unsigned char LVL_7_SIBERIUS_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_7_SIBERIUS_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_7_SIBERIUS_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_7_SIBERIUS_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_7_SIBERIUS_F1157be91_FUN_00133230(void);
extern int LVL_7_SIBERIUS_F1157be91_FUN_00132028(void);

int LVL_7_SIBERIUS_FUN_00324DA0(int a0, int a1, int a2) {
    CdMode mode = LVL_7_SIBERIUS_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_7_SIBERIUS_F1157be91_D_001A7900[0];
    LVL_7_SIBERIUS_F1157be91_D_001A7430[0] = 0;
    LVL_7_SIBERIUS_F1157be91_D_001A7434 = 0;
    LVL_7_SIBERIUS_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_7_SIBERIUS_F1157be91_FUN_00133230();
    LVL_7_SIBERIUS_F1157be91_FUN_00132028();
    return 1;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_7_SIBERIUS_F4e5bde81_D_00189E20;
extern s32 LVL_7_SIBERIUS_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_7_SIBERIUS_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_7_SIBERIUS_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_7_SIBERIUS_FUN_002C93B0(void) {
    s32 result = LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field348;
    if (LVL_7_SIBERIUS_F4e5bde81_D_001A8FF0 != 0 && LVL_7_SIBERIUS_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_7_SIBERIUS_F4e5bde81_D_001A8FF4 != 0 || LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field2294 == 110 && LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field2294 == 109 || LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field1497 != 0 && LVL_7_SIBERIUS_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field2294 == 0 && LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_7_SIBERIUS_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
typedef short s16;

typedef struct {
    u8 f0; u8 f1; u8 f2; u8 f3;
    s16 f4; s16 f6; s16 f8; s16 fA; s16 fC;
    u8 fE; u8 fF;
} Slot;

extern Slot LVL_7_SIBERIUS_F777b9bda_D_001B9040[8] __attribute__((nosda));

int LVL_7_SIBERIUS_FUN_002CC948(Slot *src)
{
    int count;
    int i;
    int free;

    count = 0;
    while (count < 8 && src[count].f0 != 255)
        count++;

    free = 0;
    for (i = 0; i < 8 && free < count; i++) {
        if (LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f0 == 0)
            free++;
    }

    if (free != count)
        return -1;

    for (free = 0; free < count; free++) {
        i = 0;
        while (i < 8 && LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f0 != 0)
            i++;
        if (i < 8) {
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f0 = src[free].f0;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f1 = src[free].f1;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f2 = src[free].f2;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f3 = src[free].f3;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f4 = src[free].f4;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f6 = src[free].f6;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].f8 = src[free].f8;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fA = src[free].fA;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fC = src[free].fC;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fE = src[free].fE - src[free].fF;
            LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fF = src[free].fF;
            if (LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fA + LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fC == 0)
                LVL_7_SIBERIUS_F777b9bda_D_001B9040[i].fA++;
        }
    }
    return i;
}
/* 3777c7f5d4234584 - nested scan over a table, 260 B, no arguments. */

extern int LVL_7_SIBERIUS_F3777c7f5_D_002208E0 __attribute__((nosda));
extern char *LVL_7_SIBERIUS_F3777c7f5_D_0021EE60[];
struct Pair {
    short a;
    short b;
};
extern struct Pair LVL_7_SIBERIUS_F3777c7f5_D_002203E0[];
struct Item {
    char *ptr;
    int extra;
};

void LVL_7_SIBERIUS_FUN_003509C8(void)
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

    p = &LVL_7_SIBERIUS_F3777c7f5_D_002208E0;
    if (*p < 0)
        return;
    while (*p >= 0) {
        entry = (int *)LVL_7_SIBERIUS_F3777c7f5_D_0021EE60[*p];
        for (i = 0; i < *(short *)((char *)entry + 40); i++) {
            items = (struct Item *)((char *)entry + 64);
            node = *(char **)((char *)items + (i << 3));
            block = node + 16;
            slot = block + (*(int *)(block + 4) << 4) + 16;
            for (j = 0; j < *(int *)block; j++) {
                pair = &LVL_7_SIBERIUS_F3777c7f5_D_002203E0[*(unsigned char *)(slot + 19)];
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

extern u8 LVL_7_SIBERIUS_Fc895cb79_D_001D1D00[];

void LVL_7_SIBERIUS_FUN_00306858(u8 *param_1)
{
    if (param_1[66] != 255) {
        u8 *tbl = (u8 *)(*(volatile int *)(param_1 + 36) + 72);

        *(int *)(param_1 + 88) = *(int *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + (param_1[64] << 2) + 28);
        param_1[110] = *(u8 *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + 18);
        param_1[108] = *(u8 *)(*(int *)(tbl + (*(volatile u8 *)(param_1 + 66) << 2)) + 17);
    } else {
        param_1[108] = 255;
        param_1[110] = 0;
        *(int *)(param_1 + 88) = (int)(LVL_7_SIBERIUS_Fc895cb79_D_001D1D00 + (param_1[64] << 11));
    }

    *(int *)(param_1 + 92) = *(int *)(*(int *)(*(volatile int *)(param_1 + 36) + (param_1[67] << 2) + 72) + (param_1[65] << 2) + 28);
}
extern void LVL_7_SIBERIUS_F0477ffed_FUN_00115DA8(int a, char *fmt, ...);
extern char LVL_7_SIBERIUS_F0477ffed_D_001AD630[]; extern char LVL_7_SIBERIUS_F0477ffed_D_001AD640[]; extern char LVL_7_SIBERIUS_F0477ffed_D_001AD648[];
void LVL_7_SIBERIUS_FUN_0036B9D0(int out, int v) {
    if (v > 999999) {
        LVL_7_SIBERIUS_F0477ffed_FUN_00115DA8(out, LVL_7_SIBERIUS_F0477ffed_D_001AD630, v / 1000000, (v / 1000) % 1000, v % 1000);
    } else if (v >= 1000) {
        LVL_7_SIBERIUS_F0477ffed_FUN_00115DA8(out, LVL_7_SIBERIUS_F0477ffed_D_001AD640, v / 1000, v % 1000);
    } else {
        LVL_7_SIBERIUS_F0477ffed_FUN_00115DA8(out, LVL_7_SIBERIUS_F0477ffed_D_001AD648, v);
    }
}
extern void LVL_7_SIBERIUS_F7b754363_FUN_00115E38(char *a, int b, char *c);
extern char LVL_7_SIBERIUS_F7b754363_D_001ADD58[];
extern char LVL_7_SIBERIUS_F7b754363_D_001ADDA0[];

int LVL_7_SIBERIUS_FUN_00428300(unsigned int *p)
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
        LVL_7_SIBERIUS_F7b754363_FUN_00115E38(LVL_7_SIBERIUS_F7b754363_D_001ADD58, 83, LVL_7_SIBERIUS_F7b754363_D_001ADDA0);
        return 0;
    }

    p[3] = offset + p[2];
    result = p[0] + offset;
    p[4] = p[4] + 1;
    return result;
}
/* RAC2 family 4615e05e7f21cb33 - 192 bytes, 2 placements.
 * Slot4615e05e allocator: find the first free of six 64-byte slots, fill it in and
 * link it at the head of the context's list.
 */

typedef struct Slot4615e05e {
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
} Slot4615e05e;                    /* 64 bytes */

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

extern Slot4615e05e LVL_7_SIBERIUS_F4615e05e_D_001D1B80[6];
extern unsigned char LVL_7_SIBERIUS_F4615e05e_D_001C9D80[];

Slot4615e05e *LVL_7_SIBERIUS_FUN_00306F88(Ctx *ctx, int index)
{
    int i;
    Slot4615e05e *slot;
    unsigned char *p;

    for (i = 0; i < 6; i++) {
        if (LVL_7_SIBERIUS_F4615e05e_D_001D1B80[i].f04 == 0) {
            break;
        }
    }
    slot = &LVL_7_SIBERIUS_F4615e05e_D_001D1B80[i];
    slot->f04 = 1;
    slot->f00 = index;
    slot->f10 = (int)(LVL_7_SIBERIUS_F4615e05e_D_001C9D80 + i * 5376);
    slot->f14 = (int)ctx->f24;
    p = (unsigned char *)*(unsigned int *)(ctx->f24->f1C + (short)index * 4 + 4);
    slot->f02 = p[2];
    slot->f0C = p + (p[0] + 4);
    slot->f1C = ctx->f50;
    ctx->f50 = (int)slot;
    return slot;
}
typedef struct {
    short key;
    short index;
    int value;
} Query;

typedef struct {
    int key;
    int value;
} Entry32fd6f60;

extern Entry32fd6f60 LVL_7_SIBERIUS_F32fd6f60_D_002A4100[];

void LVL_7_SIBERIUS_FUN_003558E8(Query *query)
{
    int i;

    i = 0;
    while (LVL_7_SIBERIUS_F32fd6f60_D_002A4100[i].key != -1 && LVL_7_SIBERIUS_F32fd6f60_D_002A4100[i].key != query->key)
        i++;
    query->index = i;
    query->value = LVL_7_SIBERIUS_F32fd6f60_D_002A4100[i].value;
}
typedef struct {
    unsigned char key;
    unsigned char reserved01[11];
    int target;
} Row167abba11e;
typedef struct {
    unsigned char reserved00[32];
    Row167abba11e *rows;
} ListObject7abba11e;
typedef struct {
    short first;
    short second;
} Pair7abba11e;

extern int LVL_7_SIBERIUS_F7abba11e_D_001DD980[];
extern ListObject7abba11e *LVL_7_SIBERIUS_F7abba11e_D_001DA340[];
extern Pair7abba11e LVL_7_SIBERIUS_F7abba11e_D_001DD1C0[];

void LVL_7_SIBERIUS_FUN_00307C30(void)
{
    int *selected;
    ListObject7abba11e *object;
    Row167abba11e *row;
    unsigned char *keys;
    unsigned int *dst;
    Pair7abba11e *pair;
    int *next;

    selected = LVL_7_SIBERIUS_F7abba11e_D_001DD980;
    while (*selected >= 0) {
        next = selected + 1;
        object = LVL_7_SIBERIUS_F7abba11e_D_001DA340[*selected];
        row = object->rows;
        for (;;) {
            keys = (unsigned char *)row;
            dst = (unsigned int *)(row->target & 0x7FFFFFFF);
            if (*keys != 255) {
                do {
                    pair = &LVL_7_SIBERIUS_F7abba11e_D_001DD1C0[*keys];
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


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_7_SIBERIUS_Fabf21065e887d7a9_AT00305278_ROLE00;

int LVL_7_SIBERIUS_FUN_00305278(void)
{
    if ((LVL_7_SIBERIUS_Fabf21065e887d7a9_AT00305278_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_7_SIBERIUS_Fabf21065e887d7a9_AT00305278_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_7_SIBERIUS_Fabf21065e887d7a9_AT00305278_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_7_SIBERIUS_Fabf21065e887d7a9_AT00305278_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_7_SIBERIUS_F5b4b17178a13f443_AT003190F0_ROLE00(void *object);

void LVL_7_SIBERIUS_FUN_003190F0(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_7_SIBERIUS_F5b4b17178a13f443_AT003190F0_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_7_SIBERIUS_Facdcf1600d770d3b_AT003559D0_ROLE00[];

void LVL_7_SIBERIUS_FUN_003559D0(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_7_SIBERIUS_Facdcf1600d770d3b_AT003559D0_ROLE00;
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


extern void LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037D3B0_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037D3B0_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_7_SIBERIUS_FUN_0037D3B0(void *object)
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
    LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037D3B0_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037D3B0_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037FE58_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037FE58_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_7_SIBERIUS_FUN_0037FE58(void *object)
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
    LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037FE58_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_7_SIBERIUS_Fb1b523716b470b36_AT0037FE58_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT0038EE88_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_0038EE88(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT0038EE88_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT00393968_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_00393968(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT00393968_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_7_SIBERIUS_F86f665335d9cb905_AT003B0AF8_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_7_SIBERIUS_FUN_003B0AF8(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_7_SIBERIUS_F86f665335d9cb905_AT003B0AF8_ROLE00(owner, owner->context_68);
}


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003B7D28_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_003B7D28(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003B7D28_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003BD3A0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_003BD3A0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003BD3A0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003D56C8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_003D56C8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003D56C8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003D9D98_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_003D9D98(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003D9D98_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003F3E48_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_7_SIBERIUS_FUN_003F3E48(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_7_SIBERIUS_F9cdc323a4d0c2fbd_AT003F3E48_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_7_SIBERIUS_F6af85cabb56d3b41_AT00425B40_ROLE00;

void LVL_7_SIBERIUS_FUN_00425B40(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_7_SIBERIUS_F6af85cabb56d3b41_AT00425B40_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_7_SIBERIUS_F6af85cabb56d3b41_AT00425B40_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_7_SIBERIUS_FUN_00426FD0(float factor, void *context,
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


void LVL_7_SIBERIUS_FUN_00427180(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_7_SIBERIUS_FUN_0042C4E0(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_7_SIBERIUS_FUN_00434508(float first, float second, float **cell)
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


extern unsigned char LVL_7_SIBERIUS_F79744baad5ad7f65_AT0043B978_ROLE00[];

void LVL_7_SIBERIUS_FUN_0043B978(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_7_SIBERIUS_F79744baad5ad7f65_AT0043B978_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_7_SIBERIUS_F6b0741c38bf00fee_AT002A6288_ROLE00[];
extern void LVL_7_SIBERIUS_F6b0741c38bf00fee_AT002A6288_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_7_SIBERIUS_F6b0741c38bf00fee_AT002A6288_ROLE02(short, short, short);

void LVL_7_SIBERIUS_FUN_002A6288(void) {
 LVL_7_SIBERIUS_F6b0741c38bf00fee_AT002A6288_ROLE01(LVL_7_SIBERIUS_F6b0741c38bf00fee_AT002A6288_ROLE00[1],0,1,0x32);
 LVL_7_SIBERIUS_F6b0741c38bf00fee_AT002A6288_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_7_SIBERIUS_F28c929cadd7aca24_AT002C8460_ROLE00;

void LVL_7_SIBERIUS_FUN_002C8460(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_7_SIBERIUS_F28c929cadd7aca24_AT002C8460_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_7_SIBERIUS_F28c929cadd7aca24_AT002C8460_ROLE00.selected = selected;
            LVL_7_SIBERIUS_F28c929cadd7aca24_AT002C8460_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_7_SIBERIUS_F5fc519c90e0e763a_AT002C9728_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_7_SIBERIUS_FUN_002C9728(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_7_SIBERIUS_F5fc519c90e0e763a_AT002C9728_ROLE00(-value);
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


void LVL_7_SIBERIUS_FUN_00375D38(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[139];

void LVL_7_SIBERIUS_FUN_00380800(void)
{
    int i;
    LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[138] = 5;
    LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[137] = 0;
    LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[i] = 0;
        LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_7_SIBERIUS_F03c444112283bc5f_AT00380800_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_7_SIBERIUS_FUN_002A54C8(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_7_SIBERIUS_FUN_002C9F60(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_7_SIBERIUS_FUN_002D2430(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_7_SIBERIUS_FUN_002D2438(void) {
    return 1;
}


extern void LVL_7_SIBERIUS_QWEN_11a4c157d102_AT002EA3A0_ROLE000(unsigned char *);

void LVL_7_SIBERIUS_FUN_002EA3A0(void *owner)
{
    LVL_7_SIBERIUS_QWEN_11a4c157d102_AT002EA3A0_ROLE000(owner);
}



void LVL_7_SIBERIUS_QWEN_407ee6f17a73_AT002EA440_ROLE001(int);
void LVL_7_SIBERIUS_QWEN_407ee6f17a73_AT002EA440_ROLE000(void*);

void LVL_7_SIBERIUS_FUN_002EA440(void *param)
{
  LVL_7_SIBERIUS_QWEN_407ee6f17a73_AT002EA440_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_7_SIBERIUS_QWEN_407ee6f17a73_AT002EA440_ROLE000(param);
}


void LVL_7_SIBERIUS_QWEN_5696fcf76f0c_AT00307F18_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_7_SIBERIUS_FUN_00307F18(void)
{
    LVL_7_SIBERIUS_QWEN_5696fcf76f0c_AT00307F18_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


unsigned int LVL_7_SIBERIUS_FUN_00322878(unsigned int param_1)
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
extern void LVL_7_SIBERIUS_QWEN_523e38f49b74_AT0033C760_ROLE000(long);
extern void LVL_7_SIBERIUS_QWEN_523e38f49b74_AT0033C760_ROLE002(void);
extern void LVL_7_SIBERIUS_QWEN_523e38f49b74_AT0033C760_ROLE001(void);

int LVL_7_SIBERIUS_FUN_0033C760(void)
{
  LVL_7_SIBERIUS_QWEN_523e38f49b74_AT0033C760_ROLE000(0);
  LVL_7_SIBERIUS_QWEN_523e38f49b74_AT0033C760_ROLE002();
  LVL_7_SIBERIUS_QWEN_523e38f49b74_AT0033C760_ROLE001();
  return 0;
}


unsigned long long LVL_7_SIBERIUS_FUN_0033CB60(void);

extern void LVL_7_SIBERIUS_QWEN_f47929f95774_AT0033CB60_ROLE000(long);
extern void LVL_7_SIBERIUS_QWEN_f47929f95774_AT0033CB60_ROLE002(void);
extern void LVL_7_SIBERIUS_QWEN_f47929f95774_AT0033CB60_ROLE001(void);

unsigned long long LVL_7_SIBERIUS_FUN_0033CB60(void)
{
  LVL_7_SIBERIUS_QWEN_f47929f95774_AT0033CB60_ROLE000(0);
  LVL_7_SIBERIUS_QWEN_f47929f95774_AT0033CB60_ROLE002();
  LVL_7_SIBERIUS_QWEN_f47929f95774_AT0033CB60_ROLE001();
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
extern void LVL_7_SIBERIUS_QWEN_cb305b1f1210_AT0033CE00_ROLE000(long);
extern void LVL_7_SIBERIUS_QWEN_cb305b1f1210_AT0033CE00_ROLE002(void);
extern void LVL_7_SIBERIUS_QWEN_cb305b1f1210_AT0033CE00_ROLE001(void);

long long LVL_7_SIBERIUS_FUN_0033CE00(void)
{
    LVL_7_SIBERIUS_QWEN_cb305b1f1210_AT0033CE00_ROLE000(0LL);
    LVL_7_SIBERIUS_QWEN_cb305b1f1210_AT0033CE00_ROLE002();
    LVL_7_SIBERIUS_QWEN_cb305b1f1210_AT0033CE00_ROLE001();
    return 0LL;
}


extern void LVL_7_SIBERIUS_QWEN_c23b3406f982_AT003421F8_ROLE000(long);
extern void LVL_7_SIBERIUS_QWEN_c23b3406f982_AT003421F8_ROLE002(void);
extern void LVL_7_SIBERIUS_QWEN_c23b3406f982_AT003421F8_ROLE001(void);

int LVL_7_SIBERIUS_FUN_003421F8(void)
{
    LVL_7_SIBERIUS_QWEN_c23b3406f982_AT003421F8_ROLE000(0);
    LVL_7_SIBERIUS_QWEN_c23b3406f982_AT003421F8_ROLE002();
    LVL_7_SIBERIUS_QWEN_c23b3406f982_AT003421F8_ROLE001();
    return 0;
}


void LVL_7_SIBERIUS_QWEN_68cf9ebb5ee2_AT00342368_ROLE000(long);
void LVL_7_SIBERIUS_QWEN_68cf9ebb5ee2_AT00342368_ROLE001(void);
void LVL_7_SIBERIUS_QWEN_68cf9ebb5ee2_AT00342368_ROLE002(void);

unsigned long long LVL_7_SIBERIUS_FUN_00342368(void)
{
    LVL_7_SIBERIUS_QWEN_68cf9ebb5ee2_AT00342368_ROLE000(0);
    LVL_7_SIBERIUS_QWEN_68cf9ebb5ee2_AT00342368_ROLE002();
    LVL_7_SIBERIUS_QWEN_68cf9ebb5ee2_AT00342368_ROLE001();
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

extern void LVL_7_SIBERIUS_QWEN_68f040eb20c9_AT00342420_ROLE000(long arg0);
extern void LVL_7_SIBERIUS_QWEN_68f040eb20c9_AT00342420_ROLE002(void);
extern void LVL_7_SIBERIUS_QWEN_68f040eb20c9_AT00342420_ROLE001(void);

unsigned long long LVL_7_SIBERIUS_FUN_00342420(void)
{
    LVL_7_SIBERIUS_QWEN_68f040eb20c9_AT00342420_ROLE000(0);
    LVL_7_SIBERIUS_QWEN_68f040eb20c9_AT00342420_ROLE002();
    LVL_7_SIBERIUS_QWEN_68f040eb20c9_AT00342420_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_7_SIBERIUS_QWEN_92ea7c2f4a5c_AT003427D0_ROLE000(long);
extern void LVL_7_SIBERIUS_QWEN_92ea7c2f4a5c_AT003427D0_ROLE001(void);
extern void LVL_7_SIBERIUS_QWEN_92ea7c2f4a5c_AT003427D0_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_7_SIBERIUS_FUN_003427D0(void)
{
  LVL_7_SIBERIUS_QWEN_92ea7c2f4a5c_AT003427D0_ROLE000(0);
  LVL_7_SIBERIUS_QWEN_92ea7c2f4a5c_AT003427D0_ROLE002();
  LVL_7_SIBERIUS_QWEN_92ea7c2f4a5c_AT003427D0_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_7_SIBERIUS_QWEN_d01c568afacc_AT003428D0_ROLE000(long param_1);
extern void LVL_7_SIBERIUS_QWEN_d01c568afacc_AT003428D0_ROLE001(void);
extern void LVL_7_SIBERIUS_QWEN_d01c568afacc_AT003428D0_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_7_SIBERIUS_FUN_003428D0(void)
{
    LVL_7_SIBERIUS_QWEN_d01c568afacc_AT003428D0_ROLE000(0);
    LVL_7_SIBERIUS_QWEN_d01c568afacc_AT003428D0_ROLE002();
    LVL_7_SIBERIUS_QWEN_d01c568afacc_AT003428D0_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_7_SIBERIUS_QWEN_093381cf82c3_AT00344A80_ROLE000(long);
extern void LVL_7_SIBERIUS_QWEN_093381cf82c3_AT00344A80_ROLE002(void);
extern void LVL_7_SIBERIUS_QWEN_093381cf82c3_AT00344A80_ROLE001(void);

long long LVL_7_SIBERIUS_FUN_00344A80(void)
{
    LVL_7_SIBERIUS_QWEN_093381cf82c3_AT00344A80_ROLE000(0);
    LVL_7_SIBERIUS_QWEN_093381cf82c3_AT00344A80_ROLE002();
    LVL_7_SIBERIUS_QWEN_093381cf82c3_AT00344A80_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_7_SIBERIUS_FUN_0034E748(void) {
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



extern QwenRecovery_4ecc6b5a4034_ExtraResetRecord164 LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE000[100];
extern QwenRecovery_4ecc6b5a4034_u32 LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[139];

void LVL_7_SIBERIUS_FUN_0037DD88(void)
{
    int i;
    for (i = 99; i >= 0; --i)
        LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE000[i].word = 0;
    LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[138] = 5;
    LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[137] = 0;
    LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[i] = 0;
        LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_7_SIBERIUS_QWEN_4ecc6b5a4034_AT0037DD88_ROLE001[i + 128] = 0;
}



void LVL_7_SIBERIUS_FUN_00427EF0(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_7_SIBERIUS_FUN_00427EF8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_7_SIBERIUS_FUN_00428050(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_7_SIBERIUS_FUN_004281A0(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_7_SIBERIUS_FUN_00428270(unsigned long param_1)
{
  return param_1;
}


extern void LVL_7_SIBERIUS_QWEN_df8fc8ba49cf_AT0042AC38_ROLE000(unsigned char *owner, int value);

void LVL_7_SIBERIUS_FUN_0042AC38(unsigned char *owner, int value)
{
    LVL_7_SIBERIUS_QWEN_df8fc8ba49cf_AT0042AC38_ROLE000(owner + 8, value);
}



void* LVL_7_SIBERIUS_FUN_0042F638(void* param_1);

extern void LVL_7_SIBERIUS_QWEN_f353c206e726_AT0042F638_ROLE000(int);
extern unsigned long long LVL_7_SIBERIUS_QWEN_f353c206e726_AT0042F638_ROLE001(unsigned long long);

void* LVL_7_SIBERIUS_FUN_0042F638(void* param_1)
{
  LVL_7_SIBERIUS_QWEN_f353c206e726_AT0042F638_ROLE000((int)param_1 + 8);
  LVL_7_SIBERIUS_QWEN_f353c206e726_AT0042F638_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_7_SIBERIUS_QWEN_f2f9288a4e34_AT0042F7D8_ROLE000(int);

int LVL_7_SIBERIUS_FUN_0042F7D8(int owner)
{
    int result;
    result = LVL_7_SIBERIUS_QWEN_f2f9288a4e34_AT0042F7D8_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_7_SIBERIUS_QWEN_e41cd63258f5_AT0042F810_ROLE000(unsigned char *);

int LVL_7_SIBERIUS_FUN_0042F810(int owner)
{
    return LVL_7_SIBERIUS_QWEN_e41cd63258f5_AT0042F810_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_7_SIBERIUS_QWEN_f29f80950ce7_AT00432F30_ROLE000(int);

void LVL_7_SIBERIUS_FUN_00432F30(int param_1)
{
  LVL_7_SIBERIUS_QWEN_f29f80950ce7_AT00432F30_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_7_SIBERIUS_QWEN_bf824305b9f7_AT00433848_ROLE000(unsigned char *owner, float *records);

void LVL_7_SIBERIUS_FUN_00433848(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_7_SIBERIUS_QWEN_bf824305b9f7_AT00433848_ROLE000(owner + 0x188, records);
}



void LVL_7_SIBERIUS_QWEN_61faeca45963_AT00433B10_ROLE000(int param_1);

void LVL_7_SIBERIUS_FUN_00433B10(int param_1)
{
  LVL_7_SIBERIUS_QWEN_61faeca45963_AT00433B10_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_7_SIBERIUS_FUN_00433C78(unsigned long param_1)
{
  return param_1;
}


extern int LVL_7_SIBERIUS_QWEN_6add11b33414_AT00434A28_ROLE000(unsigned char *owner);

int LVL_7_SIBERIUS_FUN_00434A28(unsigned char *owner)
{
    return LVL_7_SIBERIUS_QWEN_6add11b33414_AT00434A28_ROLE000(owner + 0x298);
}



extern void LVL_7_SIBERIUS_QWEN_de961518de48_AT00434A48_ROLE000(unsigned char *owner);

void LVL_7_SIBERIUS_FUN_00434A48(unsigned char *owner)
{
    LVL_7_SIBERIUS_QWEN_de961518de48_AT00434A48_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_7_SIBERIUS_QWEN_7ba3cdeeb16b_AT00439368_ROLE000(unsigned long long);

unsigned long long LVL_7_SIBERIUS_FUN_00439368(unsigned long long value)
{
    LVL_7_SIBERIUS_QWEN_7ba3cdeeb16b_AT00439368_ROLE000(value);
    return value;
}



void LVL_7_SIBERIUS_FUN_004395B0(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_7_SIBERIUS_FUN_0043A610(unsigned long param_1)
{
  return param_1;
}


void LVL_7_SIBERIUS_FUN_0043A950(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_7_SIBERIUS_FUN_0043AAF0(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_7_SIBERIUS_FUN_0043AB38(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_7_SIBERIUS_FUN_0043FA90(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_7_SIBERIUS_FUN_00441B80(void) {
    return 1;
}


extern int LVL_7_SIBERIUS_QWEN_cc83cb329fcb_AT004433C0_ROLE000(unsigned char *);

int LVL_7_SIBERIUS_FUN_004433C0(int *owner)
{
    int result;
    long status;
    status = LVL_7_SIBERIUS_QWEN_cc83cb329fcb_AT004433C0_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_00399A80(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_003AA070(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_003B22E8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_003C13A8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_003D4610(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_003DDD50(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_0040C710(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(char *local, char *first, char *second);
extern void LVL_7_SIBERIUS_Fc936841d_FUN_002E4310(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_7_SIBERIUS_FUN_00415088(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_7_SIBERIUS_Fc936841d_FUN_002E4340(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_7_SIBERIUS_Fc936841d_FUN_002E4310((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_7_SIBERIUS_F01bd4546_FUN_00427328(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(char *p, int v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(char *p, float v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(char *p, float v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(char *p, float v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(char *p, float v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(char *p, float v);
extern void LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(char *p, float v);

void LVL_7_SIBERIUS_FUN_0043E050(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_7_SIBERIUS_F01bd4546_FUN_00427328(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(p1, 1);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(p2, 1);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(p3, 1);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(p4, 1);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(p5, 1);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AAF8(p6, 1);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_7_SIBERIUS_F01bd4546_FUN_0043AB40(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_7_SIBERIUS_F70997ba9_FUN_003715E0(int a, int b);
extern void LVL_7_SIBERIUS_F70997ba9_FUN_002DC440(int a);
extern void *LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(int id);
extern int LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(void *p, int i);
extern void LVL_7_SIBERIUS_F70997ba9_FUN_002DF080(void);
extern void LVL_7_SIBERIUS_F70997ba9_FUN_002DF518(int a, int b, long c, void *d, int e);
extern void LVL_7_SIBERIUS_F70997ba9_FUN_002DF070(void);
extern void LVL_7_SIBERIUS_F70997ba9_FUN_002DC560(void);

int LVL_7_SIBERIUS_FUN_00348CD8(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_7_SIBERIUS_F70997ba9_FUN_003715E0(66, 68);
    LVL_7_SIBERIUS_F70997ba9_FUN_003715E0(71, 11);
    LVL_7_SIBERIUS_F70997ba9_FUN_002DC440(0);
    min = LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11613), -1);
    v = LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_7_SIBERIUS_F70997ba9_FUN_002DF0F8(LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF080();
    off = count - 6;
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF518(slot, off, 0x80FFA888L, LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11613), -1);
    off += count;
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF518(slot, off, 0x80FFA888L, LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11625), -1);
    off += count;
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF518(slot, off, 0x80FFA888L, LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11626), -1);
    off += count;
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF518(slot, off, 0x80FFA888L, LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11627), -1);
    off += count;
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF518(slot, off, 0x80FFA888L, LVL_7_SIBERIUS_F70997ba9_FUN_002EA888(11599), -1);
    LVL_7_SIBERIUS_F70997ba9_FUN_002DF070();
    LVL_7_SIBERIUS_F70997ba9_FUN_002DC560();
    return 2;
}
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);

void LVL_7_SIBERIUS_FUN_00399648(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[0], v[3]);
        v[1] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[1], v[4]);
        v[2] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[2], v[5]);
        v[9] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[9], v[12]);
        v[10] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[10], v[13]);
        v[11] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[11], v[14]);
        v[20] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(v[0]) * v[6];
        v[21] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[1]) * v[7];
        v[22] = -LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[2]) * v[8];
        v[24] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(v[9]) * v[15];
        v[25] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[10]) * v[16];
        v[26] = -LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(float, float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);
extern float LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(float);

void LVL_7_SIBERIUS_FUN_003D41D8(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[0], v[3]);
        v[1] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[1], v[4]);
        v[2] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[2], v[5]);
        v[9] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[9], v[12]);
        v[10] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[10], v[13]);
        v[11] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E5318(v[11], v[14]);
        v[20] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(v[0]) * v[6];
        v[21] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[1]) * v[7];
        v[22] = -LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[2]) * v[8];
        v[24] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4900(v[9]) * v[15];
        v[25] = LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[10]) * v[16];
        v[26] = -LVL_7_SIBERIUS_F307ea0ee_FUN_002E4918(v[11]) * v[17];
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


extern float LVL_7_SIBERIUS_F9e2cd219_FUN_00310A10(float value);
extern void LVL_7_SIBERIUS_F9e2cd219_FUN_00316C58(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_7_SIBERIUS_F9e2cd219_FUN_0037DA48(void *owner, void *out);

void LVL_7_SIBERIUS_FUN_0037D960(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_7_SIBERIUS_F9e2cd219_FUN_00316C58((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_7_SIBERIUS_F9e2cd219_FUN_00310A10(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_7_SIBERIUS_F9e2cd219_FUN_0037DA48(self, (char *)child + 48);

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


extern float LVL_7_SIBERIUS_F9e2cd219_FUN_00310A10(float value);
extern void LVL_7_SIBERIUS_F9e2cd219_FUN_00316C58(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_7_SIBERIUS_F9e2cd219_FUN_003804E8(void *owner, void *out);

void LVL_7_SIBERIUS_FUN_00380400(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_7_SIBERIUS_F9e2cd219_FUN_00316C58((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_7_SIBERIUS_F9e2cd219_FUN_00310A10(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_7_SIBERIUS_F9e2cd219_FUN_003804E8(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
