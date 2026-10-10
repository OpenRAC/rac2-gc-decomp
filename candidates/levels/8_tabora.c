typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_8_TABORA_D_001A8EB0;
void LVL_8_TABORA_FUN_002E3728(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_8_TABORA_FUN_002FFAB8(s32 index) {
    NativeTable20 values=LVL_8_TABORA_D_001A8EB0;
    return values.items[index];
}

u32 LVL_8_TABORA_FUN_002BF680(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_8_TABORA_D_0018C0B4;
u32 LVL_8_TABORA_FUN_002E26D0(void) {
    return LVL_8_TABORA_D_0018C0B4;
}

s32 LVL_8_TABORA_FUN_002EF8B8(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_8_TABORA_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_8_TABORA_FUN_002BF518(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_8_TABORA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_8_TABORA_FUN_002BF550(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_8_TABORA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_8_TABORA_FUN_002C0170(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_8_TABORA_FUN_002E3480(f32, f32, f32, f32, s32, s32);

void LVL_8_TABORA_FUN_002E2F28(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_8_TABORA_FUN_002E3480(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_8_TABORA_FUN_002FC310(int width, int height, int address, int mode);
extern void LVL_8_TABORA_FUN_00389AB0(unsigned int reg, unsigned long value);
extern void LVL_8_TABORA_FUN_002FC678(int width, int height);

void LVL_8_TABORA_FUN_002F1658(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_8_TABORA_FUN_002FC310(width, height, address, 1);
    LVL_8_TABORA_FUN_00389AB0(0x47, 0x30000UL);
    LVL_8_TABORA_FUN_00389AB0(0x42, 0x8000000044UL);
    LVL_8_TABORA_FUN_002FC678(0x100, 0x100);
    LVL_8_TABORA_FUN_00389AB0(0x42, 0x8000000044UL);
}

s32 LVL_8_TABORA_FUN_002BF588(s32 index) {
 s32 value=LVL_8_TABORA_FUN_002BF550(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_8_TABORA_FUN_002BF5C0(s32 index) {
 return LVL_8_TABORA_FUN_002BF550(index)==47;
}

s32 LVL_8_TABORA_FUN_002C4898(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

typedef struct {
    u32 unknown;
    s32 data_offset;
    s32 length;
    s32 extra;
} PackedHeaderCountView;

s32 LVL_8_TABORA_FUN_0038C158(const PackedHeaderCountView *header) {
    s32 fixed = header->extra + 16;
    s32 total = header->data_offset + fixed + header->length;
    return ((total + 15) / 16) * 4;
}

/* Update the measured fields of the first matching key among13records. */
typedef struct { unsigned int fields[25]; int key; unsigned int busy; unsigned int tail[9]; } UpdateRecord;
typedef char UpdateRecordSize[(sizeof(UpdateRecord) == 144) ? 1 : -1];
extern UpdateRecord LVL_8_TABORA_D_00288480[13];
void LVL_8_TABORA_FUN_00302E70(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_8_TABORA_D_00288480[i].key == key) break;
    }
    if (i < 13) {
        LVL_8_TABORA_D_00288480[i].fields[9] = value;
        if (LVL_8_TABORA_D_00288480[i].busy == 0)
            LVL_8_TABORA_D_00288480[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_8_TABORA_D_001395B8[];
u32 LVL_8_TABORA_FUN_00311FA0(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_8_TABORA_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_8_TABORA_D_00232080[];
s32 LVL_8_TABORA_FUN_0038E720(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_8_TABORA_D_00232080;
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
extern int LVL_8_TABORA_D_0022E940[];
extern ListOverrideObject *LVL_8_TABORA_D_00227440[];
extern ListOverridePair LVL_8_TABORA_D_0022E340[];
void LVL_8_TABORA_FUN_003791E8(void)
{
    int *selected = LVL_8_TABORA_D_0022E940;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_8_TABORA_D_00227440[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_8_TABORA_D_0022E340[row->key];
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
extern NativeObjectSearchRecord32 LVL_8_TABORA_D_0025D170[];
s32 LVL_8_TABORA_FUN_00407DC8(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_8_TABORA_D_0025D170[index].field14 == object) {
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

s32 LVL_8_TABORA_FUN_002E2590(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_8_TABORA_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_8_TABORA_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_8_TABORA_D_00189E20)->mode == 0x31) {
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
extern void *LVL_8_TABORA_D_0018C0B0;
extern void *LVL_8_TABORA_D_0018B134;
extern void *LVL_8_TABORA_D_0018B040;
void *LVL_8_TABORA_FUN_002B78D0(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_8_TABORA_D_0018C0B0;
    if (kind == 1)
        return LVL_8_TABORA_D_0018B134;
    if (kind == 6)
        return LVL_8_TABORA_D_0018B040;
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
extern unsigned char LVL_8_TABORA_D_00139568[];
extern MappedClassEntry LVL_8_TABORA_D_0026CCF0[];
unsigned int LVL_8_TABORA_FUN_002FF790(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_8_TABORA_D_0026CCF0[LVL_8_TABORA_D_00139568[i]];
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
extern s32 LVL_8_TABORA_FUN_002F9B30(s32 value);
NativeCompactHeader *LVL_8_TABORA_FUN_00390AB8(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_8_TABORA_FUN_002F9B30(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_8_TABORA_FUN_002F9B30(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_8_TABORA_D_001A79F0;
s32 LVL_8_TABORA_FUN_002E2510(void) {
    s32 found = 0;
    if (LVL_8_TABORA_D_001A79F0 == 25 || LVL_8_TABORA_D_001A79F0 == 5 ||
        LVL_8_TABORA_D_001A79F0 == 10 || LVL_8_TABORA_D_001A79F0 == 15) {
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
extern ConditionalResetSlot LVL_8_TABORA_D_001B9940[8];
void LVL_8_TABORA_FUN_002E2958(void) {
    ConditionalResetSlot *slot = LVL_8_TABORA_D_001B9940;
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
extern StateTransitionView LVL_8_TABORA_D_001BFC40;
void LVL_8_TABORA_FUN_002FF0F8(void) {
    if (LVL_8_TABORA_D_001BFC40.mode == 7 && LVL_8_TABORA_D_001BFC40.state == 1) {
        LVL_8_TABORA_D_001BFC40.state = 2;
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
extern DobboFormatRoot396 LVL_8_TABORA_D_001CA0E0;
extern const char LVL_8_TABORA_D_001A9AA0[];
extern const char LVL_8_TABORA_D_001A9AA8[];
extern const unsigned char *LVL_8_TABORA_FUN_003003D8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_8_TABORA_FUN_00315620(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_8_TABORA_FUN_003003D8(LVL_8_TABORA_D_001CA0E0.rows[index].text_id);
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
        int key = LVL_8_TABORA_D_001CA0E0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_8_TABORA_D_0026CCF0[LVL_8_TABORA_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_8_TABORA_D_001A9AA0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_8_TABORA_D_001A9AA8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_8_TABORA_FUN_002C4FA0(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_8_TABORA_D_00189E20;
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
extern int LVL_8_TABORA_FUN_0036DA40(int, unsigned int, void *);
void LVL_8_TABORA_FUN_00455980(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_8_TABORA_FUN_0036DA40(3, 0, 0);
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
        LVL_8_TABORA_FUN_0036DA40(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_8_TABORA_FUN_0036DA40(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_8_TABORA_FUN_0036DA40(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_8_TABORA_D_001B2940[16];
extern u32 LVL_8_TABORA_D_001B2980[16];

int LVL_8_TABORA_FUN_0031E448(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_8_TABORA_D_001B2940[index] == 0 ||
            LVL_8_TABORA_D_001B2940[index] == object) {
            LVL_8_TABORA_D_001B2940[index] = object;
            LVL_8_TABORA_D_001B2980[index] = 0;
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
extern void LVL_8_TABORA_FUN_002E8918(OozlaAppendObject164 *, int, const float *);
extern void LVL_8_TABORA_FUN_002F9E20(float *, const float *, const float *);
extern void LVL_8_TABORA_FUN_002FA298(float *, const float *, const float *);
extern void LVL_8_TABORA_FUN_002E8C60(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_8_TABORA_FUN_002E8870(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_8_TABORA_FUN_002E8918(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_8_TABORA_FUN_002F9E20(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_8_TABORA_FUN_002FA298(difference, difference, &object->transform[0][0]);
        LVL_8_TABORA_FUN_002E8C60(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_8_TABORA_FUN_0045ADF8(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_8_TABORA_FUN_0045B290(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

extern unsigned char D_19B278[];

int LVL_8_TABORA_FUN_003356C8(void)
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
extern u8 LVL_8_TABORA_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_8_TABORA_FUN_0033B7D8(void) {
    if (((CallState *)LVL_8_TABORA_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_8_TABORA_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_8_TABORA_D_001A63A8)->active); ((CallState *)LVL_8_TABORA_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_8_TABORA_FUN_003277F8(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_8_TABORA_FUN_00311F20(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_8_TABORA_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_8_TABORA_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct {
    u8 pad0000[0x2294];
    u32 kind;
    u32 unknown2298;
    u32 mode;
} ResidentFlags2294;

s32 LVL_8_TABORA_FUN_002BF630(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_8_TABORA_D_00189E20;

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

extern void LVL_8_TABORA_FUN_0031D540(NativeUpdate775View *object);

void LVL_8_TABORA_FUN_003ADFA8(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_8_TABORA_FUN_0031D540(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_8_TABORA_FUN_00338070(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_8_TABORA_FUN_0032D3D0(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_8_TABORA_FUN_002B93F0(void)
{
}


unsigned int LVL_8_TABORA_FUN_002E2660(void)
{
    return 0;
}


void LVL_8_TABORA_FUN_002F9128(void)
{
}


void LVL_8_TABORA_FUN_00300170(void)
{
}


void LVL_8_TABORA_FUN_00300F20(void)
{
}


void LVL_8_TABORA_FUN_00308608(void)
{
}


void LVL_8_TABORA_FUN_00308610(void)
{
}


void LVL_8_TABORA_FUN_0030CC80(void)
{
}


void LVL_8_TABORA_FUN_003159D8(void)
{
}


unsigned int LVL_8_TABORA_FUN_0037D918(void)
{
    return 0;
}


void LVL_8_TABORA_FUN_003824A0(void)
{
}


void LVL_8_TABORA_FUN_003894B8(void)
{
}


void LVL_8_TABORA_FUN_0038F1D0(void)
{
}


void LVL_8_TABORA_FUN_003928E8(void)
{
}


void LVL_8_TABORA_FUN_004049A8(void)
{
}


void LVL_8_TABORA_FUN_0042DFA0(void)
{
}


void LVL_8_TABORA_FUN_004485B8(void)
{
}


void LVL_8_TABORA_FUN_0044A028(void)
{
}


void LVL_8_TABORA_FUN_0044A278(void)
{
}


void LVL_8_TABORA_FUN_0044A770(void)
{
}


void LVL_8_TABORA_FUN_00452FE8(void)
{
}


void LVL_8_TABORA_FUN_00453C40(void)
{
}


void LVL_8_TABORA_FUN_0045FA48(void)
{
}


void LVL_8_TABORA_FUN_00461DD8(void)
{
}


void LVL_8_TABORA_FUN_00463670(void)
{
}
void LVL_8_TABORA_FUN_003C4DF0(char *p)
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
void LVL_8_TABORA_FUN_003D5D40(char *p)
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
void LVL_8_TABORA_FUN_003E46E0(char *p)
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
void LVL_8_TABORA_FUN_003EBFF8(char *p)
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
void LVL_8_TABORA_FUN_003EF5A0(char *p)
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
void LVL_8_TABORA_FUN_003F5880(char *p)
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
void LVL_8_TABORA_FUN_004003B0(char *p)
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
void LVL_8_TABORA_FUN_00401758(char *p)
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
void LVL_8_TABORA_FUN_0042C1A0(char *p)
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
void LVL_8_TABORA_FUN_00434110(char *p)
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

extern u8 LVL_8_TABORA_F62e6ff2b_D_00189E20[];
extern u8 LVL_8_TABORA_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_8_TABORA_FUN_0036D638(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_8_TABORA_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_8_TABORA_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_8_TABORA_F62e6ff2b_D_00188660;
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
void LVL_8_TABORA_FUN_003B5018(char *p)
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
void LVL_8_TABORA_FUN_003FA1E0(char *p)
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
extern Blob LVL_8_TABORA_F6894d7c1_D_001A8E60;
int LVL_8_TABORA_FUN_002FF3C0(int x)
{
    Blob b;
    int i;
    b = LVL_8_TABORA_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_8_TABORA_Fbc82131e_FUN_00115DA8(char *, char *, ...);
extern char LVL_8_TABORA_Fbc82131e_D_001AD6B0[];
extern char LVL_8_TABORA_Fbc82131e_D_001AD6C0[];
extern char LVL_8_TABORA_Fbc82131e_D_001AD6C8[];

void LVL_8_TABORA_FUN_00383EA0(char *dst, int value)
{
    if (value > 999999)
        LVL_8_TABORA_Fbc82131e_FUN_00115DA8(dst, LVL_8_TABORA_Fbc82131e_D_001AD6B0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_8_TABORA_Fbc82131e_FUN_00115DA8(dst, LVL_8_TABORA_Fbc82131e_D_001AD6C0, value / 1000, value % 1000);
    else
        LVL_8_TABORA_Fbc82131e_FUN_00115DA8(dst, LVL_8_TABORA_Fbc82131e_D_001AD6C8, value);
}
void LVL_8_TABORA_FUN_00397978(char *p)
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
void LVL_8_TABORA_FUN_0039A000(char *p)
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
void LVL_8_TABORA_FUN_00437928(char *p)
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

extern Entry *LVL_8_TABORA_Fc68ad20a_D_0018C2B8;

int LVL_8_TABORA_FUN_002DDD50(int key, int *out)
{
    Entry *e = LVL_8_TABORA_Fc68ad20a_D_0018C2B8;
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
extern void LVL_8_TABORA_F5b6bba31_FUN_00115E38(char *assertion, int line, char *file);
extern char LVL_8_TABORA_F5b6bba31_D_001ADDD8[];
extern char LVL_8_TABORA_F5b6bba31_D_001ADDF8[];

void LVL_8_TABORA_FUN_004485D0(int *p, unsigned int a1, int a2, int a3)
{
    if (a1 < 4)
        LVL_8_TABORA_F5b6bba31_FUN_00115E38(LVL_8_TABORA_F5b6bba31_D_001ADDD8, 37, LVL_8_TABORA_F5b6bba31_D_001ADDF8);
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

extern Blk LVL_8_TABORA_F55a1acb8_D_001A63A8;
extern short LVL_8_TABORA_F55a1acb8_D_001A63AC;
extern int LVL_8_TABORA_F55a1acb8_FUN_00133688(void);
extern void LVL_8_TABORA_F55a1acb8_FUN_0011AEA0(int);

void LVL_8_TABORA_FUN_0033C8A8(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_8_TABORA_F55a1acb8_FUN_00133688()) {
        LVL_8_TABORA_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_8_TABORA_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_8_TABORA_F55a1acb8_D_001A63A8.f6;
    q = LVL_8_TABORA_F55a1acb8_D_001A63A8.f18;
    LVL_8_TABORA_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_8_TABORA_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_8_TABORA_F55a1acb8_D_001A63A8.f1C;
        LVL_8_TABORA_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_8_TABORA_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
void LVL_8_TABORA_FUN_00354868(void)
{
    *(short *)0x001AA8C2 = *(unsigned char *)0x001A7BC9 ? 3 : 0;
    *(short *)0x001AA8DA = *(unsigned char *)0x001A7BCA ? 3 : 0;
    *(short *)0x001AA8F2 = *(unsigned char *)0x001A7BCB ? 3 : 0;
    *(short *)0x001AA90A = *(unsigned char *)0x001A7BCC ? 3 : 0;
    *(short *)0x001AA922 = *(unsigned char *)0x001A7BCE ? 3 : 0;
}
extern char LVL_8_TABORA_Fa2dbe766_D_00189E20[];

int LVL_8_TABORA_FUN_003DE578(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_8_TABORA_Fa2dbe766_D_00189E20;
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

extern char LVL_8_TABORA_Facf7396d_D_001ADDD8[];
extern char LVL_8_TABORA_Facf7396d_D_001ADE20[];
extern void LVL_8_TABORA_Facf7396d_FUN_00115E38(char *a0, int a1, char *a2);

void *LVL_8_TABORA_FUN_00448658(T *p)
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
        LVL_8_TABORA_Facf7396d_FUN_00115E38(LVL_8_TABORA_Facf7396d_D_001ADDD8, 83, LVL_8_TABORA_Facf7396d_D_001ADE20);
        return 0;
    }
    r = (void *)(p->f0 + off);
    p->f12 = off + size;
    p->f16 = p->f16 + 1;
    return r;
}
void LVL_8_TABORA_FUN_004195E0(char *object)
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
extern char LVL_8_TABORA_Fea34650e_D_00189E20[];

void LVL_8_TABORA_FUN_002DEE40(void)
{
    char *b = LVL_8_TABORA_Fea34650e_D_00189E20;
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
        char *c = LVL_8_TABORA_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_8_TABORA_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_8_TABORA_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_8_TABORA_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_8_TABORA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_8_TABORA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_8_TABORA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_8_TABORA_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_8_TABORA_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_8_TABORA_FUN_003FA8B8(char *p)
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
void LVL_8_TABORA_FUN_004378C0(char *p)
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

extern struct Table1 LVL_8_TABORA_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_8_TABORA_Fee2b87d1_D_00152CD0;

s32 LVL_8_TABORA_FUN_00311660(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_8_TABORA_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_8_TABORA_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_8_TABORA_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_8_TABORA_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_8_TABORA_F1157be91_D_001A63E8;
extern unsigned char LVL_8_TABORA_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_8_TABORA_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_8_TABORA_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_8_TABORA_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_8_TABORA_F1157be91_FUN_00133230(void);
extern int LVL_8_TABORA_F1157be91_FUN_00132028(void);

int LVL_8_TABORA_FUN_0033C730(int a0, int a1, int a2) {
    CdMode mode = LVL_8_TABORA_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_8_TABORA_F1157be91_D_001A7900[0];
    LVL_8_TABORA_F1157be91_D_001A7430[0] = 0;
    LVL_8_TABORA_F1157be91_D_001A7434 = 0;
    LVL_8_TABORA_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_8_TABORA_F1157be91_FUN_00133230();
    LVL_8_TABORA_F1157be91_FUN_00132028();
    return 1;
}
extern char LVL_8_TABORA_Fd1a69f7e_D_00238000[];
extern char LVL_8_TABORA_Fd1a69f7e_D_002392C0[];
extern char LVL_8_TABORA_Fd1a69f7e_D_002392C0_1[];

void LVL_8_TABORA_FUN_003974B0(void) {
    int *p;
    int *q;
    int *t;
    int *r;
    int i;

    p = (int *)LVL_8_TABORA_Fd1a69f7e_D_00238000;
    p = (int *)((char *)p + 4788);
    for (i = 99; i >= 0; i--) {
        *p = 0;
        p -= 12;
    }
    q = (int *)LVL_8_TABORA_Fd1a69f7e_D_002392C0;
    q[138] = 5;
    q[137] = 0;
    q[136] = 0;
    t = q;
    for (i = 63; i >= 0; i--) {
        t[0] = 0;
        t[64] = 0;
        t++;
    }
    r = (int *)LVL_8_TABORA_Fd1a69f7e_D_002392C0_1;
    r = (int *)((char *)r + 540);
    for (i = 7; i >= 0; i--) {
        *r = 0;
        r--;
    }
}
/* Family be569dc253cd5555 — 108 bytes, 2 placements (19_grelbin, 8_tabora).
   Walk the resident function-pointer table at 0x1B2340, call each entry while
   the resident count at 0x1B2380 says there is one, then clear the count.
   Default profile (-O2 -G0 -ffunction-sections): both globals are reached with
   lui/lw absolute addressing, no $gp access anywhere in the body. */
extern int LVL_8_TABORA_Fbe569dc2_D_001B2380 __attribute__((sda));
extern void (*LVL_8_TABORA_Fbe569dc2_D_001B2340[])(void);

void LVL_8_TABORA_FUN_002E3868(void)
{
    int i;

    for (i = 0; i < LVL_8_TABORA_Fbe569dc2_D_001B2380; i++) {
        LVL_8_TABORA_Fbe569dc2_D_001B2340[i]();
    }

    LVL_8_TABORA_Fbe569dc2_D_001B2380 = 0;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_8_TABORA_F4e5bde81_D_00189E20;
extern s32 LVL_8_TABORA_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_8_TABORA_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_8_TABORA_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_8_TABORA_FUN_002DF198(void) {
    s32 result = LVL_8_TABORA_F4e5bde81_D_00189E20.field348;
    if (LVL_8_TABORA_F4e5bde81_D_001A8FF0 != 0 && LVL_8_TABORA_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_8_TABORA_F4e5bde81_D_001A8FF4 != 0 || LVL_8_TABORA_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_8_TABORA_F4e5bde81_D_00189E20.field2294 == 110 && LVL_8_TABORA_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_8_TABORA_F4e5bde81_D_00189E20.field2294 == 109 || LVL_8_TABORA_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_8_TABORA_F4e5bde81_D_00189E20.field1497 != 0 && LVL_8_TABORA_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_8_TABORA_F4e5bde81_D_00189E20.field2294 == 0 && LVL_8_TABORA_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_8_TABORA_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
typedef short s16;

typedef struct {
    u8 f0; u8 f1; u8 f2; u8 f3;
    s16 f4; s16 f6; s16 f8; s16 fA; s16 fC;
    u8 fE; u8 fF;
} Slot;

extern Slot LVL_8_TABORA_F909a5233_D_001B9940[8] __attribute__((nosda));

int LVL_8_TABORA_FUN_002E2780(Slot *src)
{
    int count;
    int i;
    int free;

    count = 0;
    while (count < 8 && src[count].f0 != 255)
        count++;

    free = 0;
    for (i = 0; i < 8 && free < count; i++) {
        if (LVL_8_TABORA_F909a5233_D_001B9940[i].f0 == 0)
            free++;
    }

    if (free != count)
        return -1;

    for (free = 0; free < count; free++) {
        i = 0;
        while (i < 8 && LVL_8_TABORA_F909a5233_D_001B9940[i].f0 != 0)
            i++;
        if (i < 8) {
            LVL_8_TABORA_F909a5233_D_001B9940[i].f0 = src[free].f0;
            LVL_8_TABORA_F909a5233_D_001B9940[i].f1 = src[free].f1;
            LVL_8_TABORA_F909a5233_D_001B9940[i].f2 = src[free].f2;
            LVL_8_TABORA_F909a5233_D_001B9940[i].f3 = src[free].f3;
            LVL_8_TABORA_F909a5233_D_001B9940[i].f4 = src[free].f4;
            LVL_8_TABORA_F909a5233_D_001B9940[i].f6 = src[free].f6;
            LVL_8_TABORA_F909a5233_D_001B9940[i].f8 = src[free].f8;
            LVL_8_TABORA_F909a5233_D_001B9940[i].fA = src[free].fA;
            LVL_8_TABORA_F909a5233_D_001B9940[i].fC = src[free].fC;
            LVL_8_TABORA_F909a5233_D_001B9940[i].fE = src[free].fE - src[free].fF;
            LVL_8_TABORA_F909a5233_D_001B9940[i].fF = src[free].fF;
            if (LVL_8_TABORA_F909a5233_D_001B9940[i].fA + LVL_8_TABORA_F909a5233_D_001B9940[i].fC == 0)
                LVL_8_TABORA_F909a5233_D_001B9940[i].fA++;
        }
    }
    return i;
}
/* family c459a266b43242d6 - 352 bytes, 2 placements
   levels/19_grelbin @0x00412F70, levels/8_tabora @0x0041E630

   Fills five output floats from two pinned tables and a resident flag word,
   then clamps the three still free outputs to [-1, 1]. */

extern float LVL_8_TABORA_Fc459a266_D_00189E20[];
extern int LVL_8_TABORA_Fc459a266_D_00138180[];

void LVL_8_TABORA_FUN_0041E630(float *a0, float *a1, float *a2, float *a3, float *p4)
{
    float f1;
    float f2;
    int v1;

    *a0 = -LVL_8_TABORA_Fc459a266_D_00189E20[2028];
    *a2 = -LVL_8_TABORA_Fc459a266_D_00189E20[2029];
    v1 = LVL_8_TABORA_Fc459a266_D_00138180[104];
    f2 = 0.0f;
    if (v1 & 0x40)
        f2 = 1.0f;
    f1 = (v1 & 0x20) ? (f2 - 1.0f) : f2;
    *a1 = f1;
    *a3 = *(float *)&LVL_8_TABORA_Fc459a266_D_00138180[75];
    v1 = LVL_8_TABORA_Fc459a266_D_00138180[104];
    f1 = 0.0f;
    if (v1 & 0x1)
        f1 = 1.0f;
    if (v1 & 0x2)
        f1 = f1 - 1.0f;
    *p4 = f1;
    if (*a0 > 1.0f)
        *a0 = 1.0f;
    else if (*a0 < -1.0f)
        *a0 = -1.0f;
    if (*a1 > 1.0f)
        *a1 = 1.0f;
    else if (*a1 < -1.0f)
        *a1 = -1.0f;
    if (*p4 > 1.0f)
        *p4 = 1.0f;
    else if (*p4 < -1.0f)
        *p4 = -1.0f;
}
/* 6065799417900311 - nested scan over a table, 260 B, no arguments. */

extern int LVL_8_TABORA_F60657994_D_002211E0 __attribute__((nosda));
extern char *LVL_8_TABORA_F60657994_D_0021F760[];
struct Pair {
    short a;
    short b;
};
extern struct Pair LVL_8_TABORA_F60657994_D_00220CE0[];
struct Item {
    char *ptr;
    int extra;
};

void LVL_8_TABORA_FUN_00368E20(void)
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

    p = &LVL_8_TABORA_F60657994_D_002211E0;
    if (*p < 0)
        return;
    while (*p >= 0) {
        entry = (int *)LVL_8_TABORA_F60657994_D_0021F760[*p];
        for (i = 0; i < *(short *)((char *)entry + 40); i++) {
            items = (struct Item *)((char *)entry + 64);
            node = *(char **)((char *)items + (i << 3));
            block = node + 16;
            slot = block + (*(int *)(block + 4) << 4) + 16;
            for (j = 0; j < *(int *)block; j++) {
                pair = &LVL_8_TABORA_F60657994_D_00220CE0[*(unsigned char *)(slot + 19)];
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
extern struct Node LVL_8_TABORA_F4a421ea7_D_00226C40 __attribute__((nosda));
extern short LVL_8_TABORA_F4a421ea7_D_00226940[][2];

void LVL_8_TABORA_FUN_003776C0(void)
{
    struct Node *p = &LVL_8_TABORA_F4a421ea7_D_00226C40;

    if (p->elems != 0) {
        do {
            struct Elem *e = p->elems;

            if (p->count > 0) {
                int n = p->count;

                do {
                    short *t = LVL_8_TABORA_F4a421ea7_D_00226940[e->f35];
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

extern struct Rec LVL_8_TABORA_Fe21f3c02_D_001BC730[];

struct Rec *LVL_8_TABORA_FUN_002E2E88(int a)
{
    struct Rec *p = LVL_8_TABORA_Fe21f3c02_D_001BC730;
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
/* Family 6230a7600b032836 (120 B, 2 placements): call every registered
 * callback with its paired argument, bounded by a resident count word.
 *
 * The three resident globals are reached with `lui`+`lw` / `lui`+`addiu`
 * pairs that the ASSEMBLER macro expands from a single RTL insn; this only
 * happens when the externs carry the `sda` attribute (SYMBOL_REF_FLAG), which
 * suppresses gcc's HIGH/LO_SUM address split.  Without it the compiler keeps
 * the `%hi` in an extra callee-saved register and the body is 3 words long.
 */
typedef void (*fn_t)(int);

extern int LVL_8_TABORA_F6230a760_D_001B24B8 __attribute__((sda));   /* resident callback count */
extern fn_t LVL_8_TABORA_F6230a760_D_001B2498[] __attribute__((sda)); /* callback table        */
extern int LVL_8_TABORA_F6230a760_D_001B24A8[] __attribute__((sda));  /* argument table        */

void LVL_8_TABORA_FUN_002F2E28(void) {
    int i;
    for (i = 0; i < LVL_8_TABORA_F6230a760_D_001B24B8; i++) {
        LVL_8_TABORA_F6230a760_D_001B2498[i](LVL_8_TABORA_F6230a760_D_001B24A8[i]);
    }
}
int LVL_8_TABORA_FUN_002F0130(int a0, int a1, int a2)
{
    unsigned char *base = *(unsigned char **)0x001B2580;
    unsigned char *end = base + *(int *)base;
    unsigned short *node = (unsigned short *)(base + 4);
    int i2;
    int i1;
    int i0;

    i2 = a2 - node[0];
    if (i2 < 0) return 0;
    if (!(i2 < node[1])) return 0;
    if (!node[i2 + 2]) return 0;
    node = (unsigned short *)(base + node[i2 + 2] * 4);
    i1 = a1 - node[0];
    if (i1 < 0) return 0;
    if (!(i1 < node[1])) return 0;
    if (!node[i1 + 2]) return 0;
    node = (unsigned short *)(base + node[i1 + 2] * 4);
    i0 = a0 - node[0];
    if (i0 < 0 || !(i0 < node[1])) return 0;
    if (node[i0 + 2] == 0xFFFF) return 0;
    return (int)(end + node[i0 + 2] * 128);
}
extern unsigned char LVL_8_TABORA_F6251b968_D_001D2480[];
extern char LVL_8_TABORA_F6251b968_D_001CA680[];

unsigned char *LVL_8_TABORA_FUN_0031DCC8(unsigned char *a0, int a1)
{
    int slot;
    unsigned char *table;
    unsigned char *entry;
    int *q;
    unsigned char *r;

    table = LVL_8_TABORA_F6251b968_D_001D2480;
    slot = 0;
    while (slot < 6 && *(unsigned char *)(table + slot * 64 + 4) != 0)
        slot++;

    entry = LVL_8_TABORA_F6251b968_D_001D2480 + slot * 64;
    entry[4] = 1;
    *(unsigned short *)entry = a1;
    *(unsigned char **)(entry + 16) = (unsigned char *)(LVL_8_TABORA_F6251b968_D_001CA680 + slot * 5376);

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

extern struct Rec7f9c1f18 LVL_8_TABORA_F7f9c1f18_D_001BD2C8[];

void LVL_8_TABORA_FUN_002EEA08(int p0, int p1, int p2, int p3, int p4, int p5,
                                      int p6, int p7, int p8, int p9, unsigned idx)
{
    if (idx >= 32u)
        return;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[0] = p0;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[1] = p1;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[2] = p2;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[3] = p3;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[4] = p4;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[5] = p5;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[6] = p6;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[7] = p7;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[8] = p8;
    LVL_8_TABORA_F7f9c1f18_D_001BD2C8[idx].v[9] = p9;
}
/* Append one 16-byte record to the level's queue at 0x226210 and submit it
   through the DMA helper LVL_8_TABORA_Fa3822957_FUN_0011AFE0.  The four queue globals live in a
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

extern LevelQueuea3822957 LVL_8_TABORA_Fa3822957_D_001A7240 __attribute__((sda));
extern char LVL_8_TABORA_Fa3822957_D_00226210[];
extern int LVL_8_TABORA_Fa3822957_FUN_0011AFE0(int *dma, int flag);

int LVL_8_TABORA_FUN_00377058(int p0, int p1, int p2, int p3)
{
    int args[4];
    int n, k;

    if (LVL_8_TABORA_Fa3822957_D_001A7240.end - (LVL_8_TABORA_Fa3822957_D_001A7240.cursor - LVL_8_TABORA_Fa3822957_D_001A7240.start) < p2 * 16)
        return -1;
    if (LVL_8_TABORA_Fa3822957_D_001A7240.count == 64)
        return -2;

    args[0] = p0;
    args[1] = LVL_8_TABORA_Fa3822957_D_001A7240.cursor;
    args[2] = p1 * 16;
    args[3] = 0;
    LVL_8_TABORA_Fa3822957_FUN_0011AFE0(args, 1);

    n = LVL_8_TABORA_Fa3822957_D_001A7240.count;
    k = n;
    n = n + 1;
    LVL_8_TABORA_Fa3822957_D_001A7240.count = n;
    *(int *)(LVL_8_TABORA_Fa3822957_D_00226210 + k * 16) = LVL_8_TABORA_Fa3822957_D_001A7240.cursor;
    *(int *)(LVL_8_TABORA_Fa3822957_D_00226210 + k * 16 + 4) = p2;
    *(int *)(LVL_8_TABORA_Fa3822957_D_00226210 + k * 16 + 8) = p3;
    LVL_8_TABORA_Fa3822957_D_001A7240.cursor = LVL_8_TABORA_Fa3822957_D_001A7240.cursor + p2 * 16;
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

extern int LVL_8_TABORA_Fac18d0d0_D_001DE280[];
extern ListObjectac18d0d0 *LVL_8_TABORA_Fac18d0d0_D_001DAC40[];
extern Pairac18d0d0 LVL_8_TABORA_Fac18d0d0_D_001DDAC0[];

void LVL_8_TABORA_FUN_0031E970(void)
{
    int *selected;
    ListObjectac18d0d0 *object;
    Row16ac18d0d0 *row;
    unsigned char *keys;
    unsigned int *dst;
    Pairac18d0d0 *pair;
    int *next;

    selected = LVL_8_TABORA_Fac18d0d0_D_001DE280;
    while (*selected >= 0) {
        next = selected + 1;
        object = LVL_8_TABORA_Fac18d0d0_D_001DAC40[*selected];
        row = object->rows;
        for (;;) {
            keys = (unsigned char *)row;
            dst = (unsigned int *)(row->target & 0x7FFFFFFF);
            if (*keys != 255) {
                do {
                    pair = &LVL_8_TABORA_Fac18d0d0_D_001DDAC0[*keys];
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


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_8_TABORA_Fabf21065e887d7a9_AT0031BFB8_ROLE00;

int LVL_8_TABORA_FUN_0031BFB8(void)
{
    if ((LVL_8_TABORA_Fabf21065e887d7a9_AT0031BFB8_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_8_TABORA_Fabf21065e887d7a9_AT0031BFB8_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_8_TABORA_Fabf21065e887d7a9_AT0031BFB8_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_8_TABORA_Fabf21065e887d7a9_AT0031BFB8_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_8_TABORA_F5b4b17178a13f443_AT003319B8_ROLE00(void *object);

void LVL_8_TABORA_FUN_003319B8(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_8_TABORA_F5b4b17178a13f443_AT003319B8_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_8_TABORA_Facdcf1600d770d3b_AT0036DE08_ROLE00[];

void LVL_8_TABORA_FUN_0036DE08(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_8_TABORA_Facdcf1600d770d3b_AT0036DE08_ROLE00;
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


extern void LVL_8_TABORA_Fb1b523716b470b36_AT00396AD8_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_8_TABORA_Fb1b523716b470b36_AT00396AD8_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_8_TABORA_FUN_00396AD8(void *object)
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
    LVL_8_TABORA_Fb1b523716b470b36_AT00396AD8_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_8_TABORA_Fb1b523716b470b36_AT00396AD8_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_8_TABORA_Fb1b523716b470b36_AT00399580_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_8_TABORA_Fb1b523716b470b36_AT00399580_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_8_TABORA_FUN_00399580(void *object)
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
    LVL_8_TABORA_Fb1b523716b470b36_AT00399580_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_8_TABORA_Fb1b523716b470b36_AT00399580_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003A85B0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_003A85B0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003A85B0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003AD0E8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_003AD0E8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003AD0E8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_8_TABORA_F86f665335d9cb905_AT003D2F30_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_8_TABORA_FUN_003D2F30(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_8_TABORA_F86f665335d9cb905_AT003D2F30_ROLE00(owner, owner->context_68);
}


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003DBE40_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_003DBE40(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003DBE40_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003E14B8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_003E14B8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003E14B8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003FACE0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_003FACE0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003FACE0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003FE278_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_003FE278(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT003FE278_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_8_TABORA_F9cdc323a4d0c2fbd_AT0040DB28_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_8_TABORA_FUN_0040DB28(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_8_TABORA_F9cdc323a4d0c2fbd_AT0040DB28_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_8_TABORA_F6af85cabb56d3b41_AT00445E80_ROLE00;

void LVL_8_TABORA_FUN_00445E80(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_8_TABORA_F6af85cabb56d3b41_AT00445E80_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_8_TABORA_F6af85cabb56d3b41_AT00445E80_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_8_TABORA_FUN_00447328(float factor, void *context,
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


void LVL_8_TABORA_FUN_004474D8(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_8_TABORA_FUN_0044C838(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_8_TABORA_FUN_00454860(float first, float second, float **cell)
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


extern unsigned char LVL_8_TABORA_F79744baad5ad7f65_AT0045BCD0_ROLE00[];

void LVL_8_TABORA_FUN_0045BCD0(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_8_TABORA_F79744baad5ad7f65_AT0045BCD0_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_8_TABORA_F6b0741c38bf00fee_AT002B8598_ROLE00[];
extern void LVL_8_TABORA_F6b0741c38bf00fee_AT002B8598_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_8_TABORA_F6b0741c38bf00fee_AT002B8598_ROLE02(short, short, short);

void LVL_8_TABORA_FUN_002B8598(void) {
 LVL_8_TABORA_F6b0741c38bf00fee_AT002B8598_ROLE01(LVL_8_TABORA_F6b0741c38bf00fee_AT002B8598_ROLE00[1],0,1,0x32);
 LVL_8_TABORA_F6b0741c38bf00fee_AT002B8598_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_8_TABORA_F28c929cadd7aca24_AT002DE248_ROLE00;

void LVL_8_TABORA_FUN_002DE248(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_8_TABORA_F28c929cadd7aca24_AT002DE248_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_8_TABORA_F28c929cadd7aca24_AT002DE248_ROLE00.selected = selected;
            LVL_8_TABORA_F28c929cadd7aca24_AT002DE248_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_8_TABORA_F5fc519c90e0e763a_AT002DF510_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_8_TABORA_FUN_002DF510(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_8_TABORA_F5fc519c90e0e763a_AT002DF510_ROLE00(-value);
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


void LVL_8_TABORA_FUN_00390598(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[139];

void LVL_8_TABORA_FUN_00399F28(void)
{
    int i;
    LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[138] = 5;
    LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[137] = 0;
    LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[i] = 0;
        LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_8_TABORA_F03c444112283bc5f_AT00399F28_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_8_TABORA_FUN_002B77D8(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_8_TABORA_FUN_002DFD48(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_8_TABORA_FUN_002E8220(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_8_TABORA_FUN_002E8228(void) {
    return 1;
}


extern void LVL_8_TABORA_QWEN_11a4c157d102_AT002FFE68_ROLE000(unsigned char *);

void LVL_8_TABORA_FUN_002FFE68(void *owner)
{
    LVL_8_TABORA_QWEN_11a4c157d102_AT002FFE68_ROLE000(owner);
}



void LVL_8_TABORA_QWEN_407ee6f17a73_AT002FFF08_ROLE001(int);
void LVL_8_TABORA_QWEN_407ee6f17a73_AT002FFF08_ROLE000(void*);

void LVL_8_TABORA_FUN_002FFF08(void *param)
{
  LVL_8_TABORA_QWEN_407ee6f17a73_AT002FFF08_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_8_TABORA_QWEN_407ee6f17a73_AT002FFF08_ROLE000(param);
}


void LVL_8_TABORA_QWEN_5696fcf76f0c_AT0031EC58_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_8_TABORA_FUN_0031EC58(void)
{
    LVL_8_TABORA_QWEN_5696fcf76f0c_AT0031EC58_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


unsigned int LVL_8_TABORA_FUN_0033A878(unsigned int param_1)
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
extern void LVL_8_TABORA_QWEN_523e38f49b74_AT00354BB8_ROLE000(long);
extern void LVL_8_TABORA_QWEN_523e38f49b74_AT00354BB8_ROLE002(void);
extern void LVL_8_TABORA_QWEN_523e38f49b74_AT00354BB8_ROLE001(void);

int LVL_8_TABORA_FUN_00354BB8(void)
{
  LVL_8_TABORA_QWEN_523e38f49b74_AT00354BB8_ROLE000(0);
  LVL_8_TABORA_QWEN_523e38f49b74_AT00354BB8_ROLE002();
  LVL_8_TABORA_QWEN_523e38f49b74_AT00354BB8_ROLE001();
  return 0;
}


unsigned long long LVL_8_TABORA_FUN_00354FB8(void);

extern void LVL_8_TABORA_QWEN_f47929f95774_AT00354FB8_ROLE000(long);
extern void LVL_8_TABORA_QWEN_f47929f95774_AT00354FB8_ROLE002(void);
extern void LVL_8_TABORA_QWEN_f47929f95774_AT00354FB8_ROLE001(void);

unsigned long long LVL_8_TABORA_FUN_00354FB8(void)
{
  LVL_8_TABORA_QWEN_f47929f95774_AT00354FB8_ROLE000(0);
  LVL_8_TABORA_QWEN_f47929f95774_AT00354FB8_ROLE002();
  LVL_8_TABORA_QWEN_f47929f95774_AT00354FB8_ROLE001();
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
extern void LVL_8_TABORA_QWEN_cb305b1f1210_AT00355258_ROLE000(long);
extern void LVL_8_TABORA_QWEN_cb305b1f1210_AT00355258_ROLE002(void);
extern void LVL_8_TABORA_QWEN_cb305b1f1210_AT00355258_ROLE001(void);

long long LVL_8_TABORA_FUN_00355258(void)
{
    LVL_8_TABORA_QWEN_cb305b1f1210_AT00355258_ROLE000(0LL);
    LVL_8_TABORA_QWEN_cb305b1f1210_AT00355258_ROLE002();
    LVL_8_TABORA_QWEN_cb305b1f1210_AT00355258_ROLE001();
    return 0LL;
}


extern void LVL_8_TABORA_QWEN_c23b3406f982_AT0035A650_ROLE000(long);
extern void LVL_8_TABORA_QWEN_c23b3406f982_AT0035A650_ROLE002(void);
extern void LVL_8_TABORA_QWEN_c23b3406f982_AT0035A650_ROLE001(void);

int LVL_8_TABORA_FUN_0035A650(void)
{
    LVL_8_TABORA_QWEN_c23b3406f982_AT0035A650_ROLE000(0);
    LVL_8_TABORA_QWEN_c23b3406f982_AT0035A650_ROLE002();
    LVL_8_TABORA_QWEN_c23b3406f982_AT0035A650_ROLE001();
    return 0;
}


void LVL_8_TABORA_QWEN_68cf9ebb5ee2_AT0035A7C0_ROLE000(long);
void LVL_8_TABORA_QWEN_68cf9ebb5ee2_AT0035A7C0_ROLE001(void);
void LVL_8_TABORA_QWEN_68cf9ebb5ee2_AT0035A7C0_ROLE002(void);

unsigned long long LVL_8_TABORA_FUN_0035A7C0(void)
{
    LVL_8_TABORA_QWEN_68cf9ebb5ee2_AT0035A7C0_ROLE000(0);
    LVL_8_TABORA_QWEN_68cf9ebb5ee2_AT0035A7C0_ROLE002();
    LVL_8_TABORA_QWEN_68cf9ebb5ee2_AT0035A7C0_ROLE001();
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

extern void LVL_8_TABORA_QWEN_68f040eb20c9_AT0035A878_ROLE000(long arg0);
extern void LVL_8_TABORA_QWEN_68f040eb20c9_AT0035A878_ROLE002(void);
extern void LVL_8_TABORA_QWEN_68f040eb20c9_AT0035A878_ROLE001(void);

unsigned long long LVL_8_TABORA_FUN_0035A878(void)
{
    LVL_8_TABORA_QWEN_68f040eb20c9_AT0035A878_ROLE000(0);
    LVL_8_TABORA_QWEN_68f040eb20c9_AT0035A878_ROLE002();
    LVL_8_TABORA_QWEN_68f040eb20c9_AT0035A878_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_8_TABORA_QWEN_92ea7c2f4a5c_AT0035AC28_ROLE000(long);
extern void LVL_8_TABORA_QWEN_92ea7c2f4a5c_AT0035AC28_ROLE001(void);
extern void LVL_8_TABORA_QWEN_92ea7c2f4a5c_AT0035AC28_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_8_TABORA_FUN_0035AC28(void)
{
  LVL_8_TABORA_QWEN_92ea7c2f4a5c_AT0035AC28_ROLE000(0);
  LVL_8_TABORA_QWEN_92ea7c2f4a5c_AT0035AC28_ROLE002();
  LVL_8_TABORA_QWEN_92ea7c2f4a5c_AT0035AC28_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_8_TABORA_QWEN_d01c568afacc_AT0035AD28_ROLE000(long param_1);
extern void LVL_8_TABORA_QWEN_d01c568afacc_AT0035AD28_ROLE001(void);
extern void LVL_8_TABORA_QWEN_d01c568afacc_AT0035AD28_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_8_TABORA_FUN_0035AD28(void)
{
    LVL_8_TABORA_QWEN_d01c568afacc_AT0035AD28_ROLE000(0);
    LVL_8_TABORA_QWEN_d01c568afacc_AT0035AD28_ROLE002();
    LVL_8_TABORA_QWEN_d01c568afacc_AT0035AD28_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_8_TABORA_QWEN_093381cf82c3_AT0035CED8_ROLE000(long);
extern void LVL_8_TABORA_QWEN_093381cf82c3_AT0035CED8_ROLE002(void);
extern void LVL_8_TABORA_QWEN_093381cf82c3_AT0035CED8_ROLE001(void);

long long LVL_8_TABORA_FUN_0035CED8(void)
{
    LVL_8_TABORA_QWEN_093381cf82c3_AT0035CED8_ROLE000(0);
    LVL_8_TABORA_QWEN_093381cf82c3_AT0035CED8_ROLE002();
    LVL_8_TABORA_QWEN_093381cf82c3_AT0035CED8_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_8_TABORA_FUN_00366BA0(void) {
    return 1;
}


void LVL_8_TABORA_FUN_00448248(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_8_TABORA_FUN_00448250(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_8_TABORA_FUN_004483A8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_8_TABORA_FUN_004484F8(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_8_TABORA_FUN_004485C8(unsigned long param_1)
{
  return param_1;
}


extern void LVL_8_TABORA_QWEN_df8fc8ba49cf_AT0044AF90_ROLE000(unsigned char *owner, int value);

void LVL_8_TABORA_FUN_0044AF90(unsigned char *owner, int value)
{
    LVL_8_TABORA_QWEN_df8fc8ba49cf_AT0044AF90_ROLE000(owner + 8, value);
}



void* LVL_8_TABORA_FUN_0044F990(void* param_1);

extern void LVL_8_TABORA_QWEN_f353c206e726_AT0044F990_ROLE000(int);
extern unsigned long long LVL_8_TABORA_QWEN_f353c206e726_AT0044F990_ROLE001(unsigned long long);

void* LVL_8_TABORA_FUN_0044F990(void* param_1)
{
  LVL_8_TABORA_QWEN_f353c206e726_AT0044F990_ROLE000((int)param_1 + 8);
  LVL_8_TABORA_QWEN_f353c206e726_AT0044F990_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_8_TABORA_QWEN_f2f9288a4e34_AT0044FB30_ROLE000(int);

int LVL_8_TABORA_FUN_0044FB30(int owner)
{
    int result;
    result = LVL_8_TABORA_QWEN_f2f9288a4e34_AT0044FB30_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_8_TABORA_QWEN_e41cd63258f5_AT0044FB68_ROLE000(unsigned char *);

int LVL_8_TABORA_FUN_0044FB68(int owner)
{
    return LVL_8_TABORA_QWEN_e41cd63258f5_AT0044FB68_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_8_TABORA_QWEN_f29f80950ce7_AT00453288_ROLE000(int);

void LVL_8_TABORA_FUN_00453288(int param_1)
{
  LVL_8_TABORA_QWEN_f29f80950ce7_AT00453288_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_8_TABORA_QWEN_bf824305b9f7_AT00453BA0_ROLE000(unsigned char *owner, float *records);

void LVL_8_TABORA_FUN_00453BA0(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_8_TABORA_QWEN_bf824305b9f7_AT00453BA0_ROLE000(owner + 0x188, records);
}



void LVL_8_TABORA_QWEN_61faeca45963_AT00453E68_ROLE000(int param_1);

void LVL_8_TABORA_FUN_00453E68(int param_1)
{
  LVL_8_TABORA_QWEN_61faeca45963_AT00453E68_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_8_TABORA_FUN_00453FD0(unsigned long param_1)
{
  return param_1;
}


extern int LVL_8_TABORA_QWEN_6add11b33414_AT00454D80_ROLE000(unsigned char *owner);

int LVL_8_TABORA_FUN_00454D80(unsigned char *owner)
{
    return LVL_8_TABORA_QWEN_6add11b33414_AT00454D80_ROLE000(owner + 0x298);
}



extern void LVL_8_TABORA_QWEN_de961518de48_AT00454DA0_ROLE000(unsigned char *owner);

void LVL_8_TABORA_FUN_00454DA0(unsigned char *owner)
{
    LVL_8_TABORA_QWEN_de961518de48_AT00454DA0_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_8_TABORA_QWEN_7ba3cdeeb16b_AT004596C0_ROLE000(unsigned long long);

unsigned long long LVL_8_TABORA_FUN_004596C0(unsigned long long value)
{
    LVL_8_TABORA_QWEN_7ba3cdeeb16b_AT004596C0_ROLE000(value);
    return value;
}



void LVL_8_TABORA_FUN_00459908(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_8_TABORA_FUN_0045A968(unsigned long param_1)
{
  return param_1;
}


void LVL_8_TABORA_FUN_0045ACA8(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_8_TABORA_FUN_0045AE48(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_8_TABORA_FUN_0045AE90(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_8_TABORA_FUN_0045FDE8(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_8_TABORA_FUN_00461ED8(void) {
    return 1;
}


extern int LVL_8_TABORA_QWEN_cc83cb329fcb_AT00463718_ROLE000(unsigned char *);

int LVL_8_TABORA_FUN_00463718(int *owner)
{
    int result;
    long status;
    status = LVL_8_TABORA_QWEN_cc83cb329fcb_AT00463718_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_003B4A60(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_003C5050(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_003D60C0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_003E5138(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_003F9C28(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_00402230(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_0042C520(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_8_TABORA_Fc936841d_FUN_002F9E20(char *local, char *first, char *second);
extern void LVL_8_TABORA_Fc936841d_FUN_002F9DF0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_8_TABORA_FUN_004340A8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_8_TABORA_Fc936841d_FUN_002F9E20(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_8_TABORA_Fc936841d_FUN_002F9DF0((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_8_TABORA_F01bd4546_FUN_00447680(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE50(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE50(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE50(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE50(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE50(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE50(char *p, int v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE98(char *p, float v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE98(char *p, float v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE98(char *p, float v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE98(char *p, float v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE98(char *p, float v);
extern void LVL_8_TABORA_F01bd4546_FUN_0045AE98(char *p, float v);

void LVL_8_TABORA_FUN_0045E3A8(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_8_TABORA_F01bd4546_FUN_00447680(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_8_TABORA_F01bd4546_FUN_0045AE50(p1, 1);
        LVL_8_TABORA_F01bd4546_FUN_0045AE50(p2, 1);
        LVL_8_TABORA_F01bd4546_FUN_0045AE50(p3, 1);
        LVL_8_TABORA_F01bd4546_FUN_0045AE50(p4, 1);
        LVL_8_TABORA_F01bd4546_FUN_0045AE50(p5, 1);
        LVL_8_TABORA_F01bd4546_FUN_0045AE50(p6, 1);
        LVL_8_TABORA_F01bd4546_FUN_0045AE98(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_8_TABORA_F01bd4546_FUN_0045AE98(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_8_TABORA_F01bd4546_FUN_0045AE98(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_8_TABORA_F01bd4546_FUN_0045AE98(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_8_TABORA_F01bd4546_FUN_0045AE98(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_8_TABORA_F01bd4546_FUN_0045AE98(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_8_TABORA_F70997ba9_FUN_00389AB0(int a, int b);
extern void LVL_8_TABORA_F70997ba9_FUN_002F2230(int a);
extern void *LVL_8_TABORA_F70997ba9_FUN_003003D8(int id);
extern int LVL_8_TABORA_F70997ba9_FUN_002F5168(void *p, int i);
extern void LVL_8_TABORA_F70997ba9_FUN_002F50F0(void);
extern void LVL_8_TABORA_F70997ba9_FUN_002F5588(int a, int b, long c, void *d, int e);
extern void LVL_8_TABORA_F70997ba9_FUN_002F50E0(void);
extern void LVL_8_TABORA_F70997ba9_FUN_002F2350(void);

int LVL_8_TABORA_FUN_00361130(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_8_TABORA_F70997ba9_FUN_00389AB0(66, 68);
    LVL_8_TABORA_F70997ba9_FUN_00389AB0(71, 11);
    LVL_8_TABORA_F70997ba9_FUN_002F2230(0);
    min = LVL_8_TABORA_F70997ba9_FUN_002F5168(LVL_8_TABORA_F70997ba9_FUN_003003D8(11613), -1);
    v = LVL_8_TABORA_F70997ba9_FUN_002F5168(LVL_8_TABORA_F70997ba9_FUN_003003D8(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_8_TABORA_F70997ba9_FUN_002F5168(LVL_8_TABORA_F70997ba9_FUN_003003D8(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_8_TABORA_F70997ba9_FUN_002F5168(LVL_8_TABORA_F70997ba9_FUN_003003D8(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_8_TABORA_F70997ba9_FUN_002F5168(LVL_8_TABORA_F70997ba9_FUN_003003D8(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_8_TABORA_F70997ba9_FUN_002F5168(LVL_8_TABORA_F70997ba9_FUN_003003D8(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_8_TABORA_F70997ba9_FUN_002F50F0();
    off = count - 6;
    LVL_8_TABORA_F70997ba9_FUN_002F5588(slot, off, 0x80FFA888L, LVL_8_TABORA_F70997ba9_FUN_003003D8(11613), -1);
    off += count;
    LVL_8_TABORA_F70997ba9_FUN_002F5588(slot, off, 0x80FFA888L, LVL_8_TABORA_F70997ba9_FUN_003003D8(11625), -1);
    off += count;
    LVL_8_TABORA_F70997ba9_FUN_002F5588(slot, off, 0x80FFA888L, LVL_8_TABORA_F70997ba9_FUN_003003D8(11626), -1);
    off += count;
    LVL_8_TABORA_F70997ba9_FUN_002F5588(slot, off, 0x80FFA888L, LVL_8_TABORA_F70997ba9_FUN_003003D8(11627), -1);
    off += count;
    LVL_8_TABORA_F70997ba9_FUN_002F5588(slot, off, 0x80FFA888L, LVL_8_TABORA_F70997ba9_FUN_003003D8(11599), -1);
    LVL_8_TABORA_F70997ba9_FUN_002F50E0();
    LVL_8_TABORA_F70997ba9_FUN_002F2350();
    return 2;
}
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);

void LVL_8_TABORA_FUN_003B4628(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[0], v[3]);
        v[1] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[1], v[4]);
        v[2] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[2], v[5]);
        v[9] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[9], v[12]);
        v[10] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[10], v[13]);
        v[11] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[11], v[14]);
        v[20] = LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(v[0]) * v[6];
        v[21] = LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[1]) * v[7];
        v[22] = -LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[2]) * v[8];
        v[24] = LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(v[9]) * v[15];
        v[25] = LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[10]) * v[16];
        v[26] = -LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);
extern float LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(float);

void LVL_8_TABORA_FUN_003F97F0(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[0], v[3]);
        v[1] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[1], v[4]);
        v[2] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[2], v[5]);
        v[9] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[9], v[12]);
        v[10] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[10], v[13]);
        v[11] = LVL_8_TABORA_F307ea0ee_FUN_002FADE0(v[11], v[14]);
        v[20] = LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(v[0]) * v[6];
        v[21] = LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[1]) * v[7];
        v[22] = -LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[2]) * v[8];
        v[24] = LVL_8_TABORA_F307ea0ee_FUN_002FA3C8(v[9]) * v[15];
        v[25] = LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[10]) * v[16];
        v[26] = -LVL_8_TABORA_F307ea0ee_FUN_002FA3E0(v[11]) * v[17];
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


extern float LVL_8_TABORA_F9e2cd219_FUN_003277A0(float value);
extern void LVL_8_TABORA_F9e2cd219_FUN_0032F180(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_8_TABORA_F9e2cd219_FUN_00397170(void *owner, void *out);

void LVL_8_TABORA_FUN_00397088(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_8_TABORA_F9e2cd219_FUN_0032F180((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_8_TABORA_F9e2cd219_FUN_003277A0(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_8_TABORA_F9e2cd219_FUN_00397170(self, (char *)child + 48);

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


extern float LVL_8_TABORA_F9e2cd219_FUN_003277A0(float value);
extern void LVL_8_TABORA_F9e2cd219_FUN_0032F180(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_8_TABORA_F9e2cd219_FUN_00399C10(void *owner, void *out);

void LVL_8_TABORA_FUN_00399B28(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_8_TABORA_F9e2cd219_FUN_0032F180((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_8_TABORA_F9e2cd219_FUN_003277A0(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_8_TABORA_F9e2cd219_FUN_00399C10(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
extern void LVL_8_TABORA_F6df9730c_FUN_002F9C68(char *dst, int *src, int count);

void LVL_8_TABORA_FUN_003151F8(char *dst, unsigned char *src)
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
        LVL_8_TABORA_F6df9730c_FUN_002F9C68(dst, tmp, 64);
        dst = next;
        LVL_8_TABORA_F6df9730c_FUN_002F9C68(dst, tmp, 64);
        dst += 64;
        LVL_8_TABORA_F6df9730c_FUN_002F9C68(dst, tmp, 64);
        dst += 64;
        LVL_8_TABORA_F6df9730c_FUN_002F9C68(dst, tmp, 64);
        dst += 64;
    }
}
extern void LVL_8_TABORA_Fdb046c5d_FUN_002E83F0(void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_002E8230(void *);
extern int LVL_8_TABORA_Fdb046c5d_FUN_002E8608(void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_002FA0B8(float, void *, void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_002F9F08(void *, void *, void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_002E8B90(void *, void *, void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_00333B60(void *, void *, int);
extern int LVL_8_TABORA_Fdb046c5d_FUN_002E8CB0(void *, void *, void *, float, float);
extern int LVL_8_TABORA_Fdb046c5d_FUN_0036D8E0(int, int, void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_002E8870(void *, int, void *);
extern int LVL_8_TABORA_Fdb046c5d_FUN_0036D8E0(int, int, void *);
extern void LVL_8_TABORA_Fdb046c5d_FUN_0036DCC0(int, void *);
extern char LVL_8_TABORA_Fdb046c5d_D_001BFF80[];

int LVL_8_TABORA_FUN_002E7E18(char *obj)
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
        LVL_8_TABORA_Fdb046c5d_FUN_002E83F0(obj);
    else
        LVL_8_TABORA_Fdb046c5d_FUN_002E8230(obj);

    s5 = LVL_8_TABORA_Fdb046c5d_FUN_002E8608(obj);
    if (s5 == -1)
        return 0;

    s1 = LVL_8_TABORA_Fdb046c5d_D_001BFF80;
    s4 = -1;
    LVL_8_TABORA_Fdb046c5d_FUN_002FA0B8(1.0f, s1, s1);
    LVL_8_TABORA_Fdb046c5d_FUN_002F9F08(buf, s1, p);
    LVL_8_TABORA_Fdb046c5d_FUN_002E8B90(obj, p + 16, buf);
    LVL_8_TABORA_Fdb046c5d_FUN_00333B60(obj + 16, out, 1);
    r = LVL_8_TABORA_Fdb046c5d_FUN_002E8CB0(p, s1, out, *(float *)(p + 56), *(float *)(p + 60));
    if (r == 0) {
        if (*(unsigned char *)(p + 94) != 255 && *(unsigned char *)(p + 92) == 0)
            s4 = LVL_8_TABORA_Fdb046c5d_FUN_0036D8E0(*(unsigned char *)(p + 94), 0, obj);
        LVL_8_TABORA_Fdb046c5d_FUN_002E8870(obj, s5, out);
    } else {
        if (*(unsigned char *)(p + 94) != 255)
            s4 = LVL_8_TABORA_Fdb046c5d_FUN_0036D8E0(*(unsigned char *)(p + 94), 0, obj);
    }
    if (s4 != -1)
        LVL_8_TABORA_Fdb046c5d_FUN_0036DCC0(s4, p + 32);
    return 0;
}
extern void LVL_8_TABORA_F7242f0a4_FUN_002F9E60(char *out, void *source, float value);
extern void LVL_8_TABORA_F7242f0a4_FUN_0043CE98(char *buffer, int mode);
extern void LVL_8_TABORA_F7242f0a4_FUN_0043CD38(int value, char *buffer);
extern void LVL_8_TABORA_F7242f0a4_FUN_002F9DF0(char *first, char *second, char *third);
extern float LVL_8_TABORA_F7242f0a4_FUN_002F9EE0(void *owner, char *buffer);

extern int LVL_8_TABORA_F7242f0a4_D_001B9B60[];

void LVL_8_TABORA_FUN_0043D560(void *owner, int flag, float value)
{
    char buffer[32];
    int *root = LVL_8_TABORA_F7242f0a4_D_001B9B60;

    LVL_8_TABORA_F7242f0a4_FUN_002F9E60(buffer, root + 8, value);
    if (flag)
        LVL_8_TABORA_F7242f0a4_FUN_0043CE98(buffer + 16, 1);
    else
        LVL_8_TABORA_F7242f0a4_FUN_0043CD38(root[-4], buffer + 16);
    LVL_8_TABORA_F7242f0a4_FUN_002F9DF0(buffer, buffer, buffer + 16);
    LVL_8_TABORA_F7242f0a4_FUN_002F9E60(owner, root + 12, LVL_8_TABORA_F7242f0a4_FUN_002F9EE0(root + 12, buffer));
}
extern char LVL_8_TABORA_F45821cfb_D_001C7880[];
extern void LVL_8_TABORA_F45821cfb_FUN_002F9DB8(char *);

void LVL_8_TABORA_FUN_00460410(void)
{
    float *p = (float *)LVL_8_TABORA_F45821cfb_D_001C7880;

    p[224] = 0.4f;
    p[225] = 0.8f;
    p[226] = 1.2f;
    *(int *)&p[227] = 0;
    p[228] = 0.577f;
    p[229] = 0.577f;
    p[230] = -0.577f;
    *(int *)&p[231] = 0;
    LVL_8_TABORA_F45821cfb_FUN_002F9DB8((char *)&p[232]);
    LVL_8_TABORA_F45821cfb_FUN_002F9DB8((char *)&p[236]);
}
extern char LVL_8_TABORA_Fd8e166aa_D_001BCF40[];

void LVL_8_TABORA_FUN_002EF588(void)
{
    char *g = LVL_8_TABORA_Fd8e166aa_D_001BCF40;
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
extern char LVL_8_TABORA_F7cb419c1_D_001FF980[];

extern int LVL_8_TABORA_F7cb419c1_FUN_00365FB0(int arg);

int LVL_8_TABORA_FUN_0035C7A8(void)
{
    char *p = LVL_8_TABORA_F7cb419c1_D_001FF980;

    *(int *)(p + 460) = LVL_8_TABORA_F7cb419c1_FUN_00365FB0(*(int *)(p + 460));
    return 0;
}
extern char LVL_8_TABORA_F0a76d85b_D_001B99C0[];

extern void LVL_8_TABORA_F0a76d85b_FUN_002FA5A8(char *p);
extern void LVL_8_TABORA_F0a76d85b_FUN_002EFD50(void);

void LVL_8_TABORA_FUN_004604A0(void)
{
    char *p = LVL_8_TABORA_F0a76d85b_D_001B99C0;

    *(int *)(p + 320) = 0;
    *(int *)(p + 324) = 0;
    *(int *)(p + 328) = 0;
    LVL_8_TABORA_F0a76d85b_FUN_002FA5A8(p + 880);
    LVL_8_TABORA_F0a76d85b_FUN_002EFD50();
}
extern char LVL_8_TABORA_F391de845_D_001B9B00[];
extern float LVL_8_TABORA_F391de845_FUN_002F9F98(char *a, char *b);
extern float LVL_8_TABORA_F391de845_FUN_0036C330(void *self, float d, float x, float y);

float LVL_8_TABORA_FUN_0036C428(char *self, char *p)
{
    float v = LVL_8_TABORA_F391de845_FUN_002F9F98(p, LVL_8_TABORA_F391de845_D_001B9B00);
    float *q = *(float **)(self + 8);

    return LVL_8_TABORA_F391de845_FUN_0036C330(q, v, q[0], q[1]);
}
extern int LVL_8_TABORA_F2f080549_D_001A8FF4 __attribute__((sda));
extern char LVL_8_TABORA_F2f080549_D_001B2650[] __attribute__((sda));
extern char LVL_8_TABORA_F2f080549_D_001A9000[] __attribute__((sda));
extern float LVL_8_TABORA_F2f080549_FUN_002F9F98(char *a, char *b);

float LVL_8_TABORA_FUN_00333F38(char *p)
{
    float v;

    if (LVL_8_TABORA_F2f080549_D_001A8FF4 == 0) {
        v = LVL_8_TABORA_F2f080549_FUN_002F9F98(p, LVL_8_TABORA_F2f080549_D_001B2650);
    } else {
        v = 100.0f - LVL_8_TABORA_F2f080549_FUN_002F9F98(p, LVL_8_TABORA_F2f080549_D_001A9000);
    }
    return v;
}
extern char LVL_8_TABORA_Fa76f1772_D_001BFF60[];
extern void LVL_8_TABORA_Fa76f1772_FUN_002F9E20(char *local, char *data);
extern float LVL_8_TABORA_Fa76f1772_FUN_002F9EE0(char *local, char *p);

int LVL_8_TABORA_FUN_00440A38(int a0, int a1)
{
    char local[16];
    int s0;
    int r;

    if (a1 != 1) {
        r = 0;
    } else {
        s0 = *(int *)(a0 + 112) + 352;
        LVL_8_TABORA_Fa76f1772_FUN_002F9E20(local, LVL_8_TABORA_Fa76f1772_D_001BFF60);
        r = LVL_8_TABORA_Fa76f1772_FUN_002F9EE0(local, (char *)s0) < 0.0f ? 1 : 0;
    }
    return r;
}
extern void LVL_8_TABORA_F46b43b72_FUN_0033C638(int value, char *target);
extern void LVL_8_TABORA_F46b43b72_FUN_0033C800(int value);

extern int LVL_8_TABORA_F46b43b72_D_001BD800[];
extern int LVL_8_TABORA_F46b43b72_D_0014B540[];

int LVL_8_TABORA_FUN_00310D40(int index)
{
    int j = index + 1;
    int *d = LVL_8_TABORA_F46b43b72_D_001BD800;
    int *b = LVL_8_TABORA_F46b43b72_D_0014B540;
    int *t = (int *)((char *)b + 25416);
    int n = d[12] * 332;
    int hold = d[28];
    int j4 = j * 4;
    int cur = *(int *)((char *)t + (n + index * 4));
    int next = *(int *)((char *)t + (j4 + n));

    n = next - cur;
    if (n > 0) {
        LVL_8_TABORA_F46b43b72_FUN_0033C638(hold, (char *)(cur + b[6341]));
        LVL_8_TABORA_F46b43b72_FUN_0033C800(0);
    }
    return 1;
}
extern void LVL_8_TABORA_Fa2d20de7_FUN_00327A58(void *object, float first, float second);
extern void LVL_8_TABORA_Fa2d20de7_FUN_002F9DF0(char *first, char *second, void *third);
extern int LVL_8_TABORA_Fa2d20de7_FUN_002EA300(void *first, char *second, int mode, int value, int extra);
extern void LVL_8_TABORA_Fa2d20de7_FUN_002F9E20(void *first, void *second, void *third);
extern void LVL_8_TABORA_Fa2d20de7_FUN_002F9E60(void *first, void *second, float value);

extern short LVL_8_TABORA_Fa2d20de7_D_001B9B00[];
extern short LVL_8_TABORA_Fa2d20de7_D_001BFF60[];
extern int LVL_8_TABORA_Fa2d20de7_D_001886CC[];

void LVL_8_TABORA_FUN_0036C1E0(void *object)
{
    LVL_8_TABORA_Fa2d20de7_FUN_00327A58(object, 0.5f, 6.0f);
    LVL_8_TABORA_Fa2d20de7_FUN_002F9DF0(object, object, LVL_8_TABORA_Fa2d20de7_D_001B9B00);
    if (LVL_8_TABORA_Fa2d20de7_FUN_002EA300(LVL_8_TABORA_Fa2d20de7_D_001B9B00, object, 130, LVL_8_TABORA_Fa2d20de7_D_001886CC[0], 0)) {
        LVL_8_TABORA_Fa2d20de7_FUN_002F9E20(object, LVL_8_TABORA_Fa2d20de7_D_001BFF60, LVL_8_TABORA_Fa2d20de7_D_001B9B00);
        LVL_8_TABORA_Fa2d20de7_FUN_002F9E60(object, object, 0.75f);
        LVL_8_TABORA_Fa2d20de7_FUN_002F9DF0(object, object, LVL_8_TABORA_Fa2d20de7_D_001B9B00);
    }
}
extern char LVL_8_TABORA_F15d5f4fb_D_001B99C0[];
extern char LVL_8_TABORA_F15d5f4fb_D_00189E20[];
extern float LVL_8_TABORA_F15d5f4fb_FUN_002FA490(float a, float b);
extern float LVL_8_TABORA_F15d5f4fb_FUN_002FAEC8(float a, float b);
extern float LVL_8_TABORA_F15d5f4fb_FUN_002F9F98(char *a, char *b);

void LVL_8_TABORA_FUN_00444F88(char *o)
{
    char *B = LVL_8_TABORA_F15d5f4fb_D_001B99C0;
    char *D = LVL_8_TABORA_F15d5f4fb_D_00189E20;
    float *p = *(float **)(D + 8848);
    float r = LVL_8_TABORA_F15d5f4fb_FUN_002FAEC8(*(float *)(B + 344),
                      LVL_8_TABORA_F15d5f4fb_FUN_002FA490(p[4] - *(float *)(B + 320),
                              p[5] - *(float *)(B + 324)));

    if (r < 0.5585054f) {
        r = LVL_8_TABORA_F15d5f4fb_FUN_002F9F98(D + 128, B + 320);
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





extern void LVL_8_TABORA_F4778f810_FUN_00333CB0(char *a, char *b, char *c, f32 d);
extern void LVL_8_TABORA_F4778f810_FUN_002F9DF0(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002FA298(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002FABA0(char *a, char *b);
extern void LVL_8_TABORA_F4778f810_FUN_002FA980(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002FA298(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002F9E20(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002F9E20(char *a, char *b, char *c);

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


void LVL_8_TABORA_FUN_002E8230(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_8_TABORA_F4778f810_FUN_00333CB0((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_8_TABORA_F4778f810_FUN_002F9DF0((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_8_TABORA_F4778f810_FUN_002FA298((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_8_TABORA_F4778f810_FUN_002FABA0((char *)p + 0x10, (char *)&tmp[3]);
    LVL_8_TABORA_F4778f810_FUN_002FA980((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_8_TABORA_F4778f810_FUN_002FA298((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_8_TABORA_F4778f810_FUN_002F9E20((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_8_TABORA_F4778f810_FUN_002F9E20((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
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





extern void LVL_8_TABORA_F4778f810_FUN_00333CB0(char *a, char *b, char *c, f32 d);
extern void LVL_8_TABORA_F4778f810_FUN_002F9DF0(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002FA298(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002FABA0(char *a, char *b);
extern void LVL_8_TABORA_F4778f810_FUN_002FA980(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002FA298(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002F9E20(char *a, char *b, char *c);
extern void LVL_8_TABORA_F4778f810_FUN_002F9E20(char *a, char *b, char *c);

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


void LVL_8_TABORA_FUN_002E8310(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_8_TABORA_F4778f810_FUN_00333CB0((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_8_TABORA_F4778f810_FUN_002F9DF0((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_8_TABORA_F4778f810_FUN_002FA298((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_8_TABORA_F4778f810_FUN_002FABA0((char *)p + 0x10, (char *)&tmp[3]);
    LVL_8_TABORA_F4778f810_FUN_002FA980((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_8_TABORA_F4778f810_FUN_002FA298((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_8_TABORA_F4778f810_FUN_002F9E20((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_8_TABORA_F4778f810_FUN_002F9E20((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}


extern void LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(int arg0, int arg1, int arg2, int arg3);
extern int LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(float value);
extern void LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(int arg0, int arg1, int arg2, int arg3);
extern int LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(float value);
extern void LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(int arg0, int arg1, int arg2, int arg3);
extern int LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(float value);
extern void LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(int arg0, int arg1, int arg2, int arg3);
extern int LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(float value);
extern void LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(int arg0, int arg1, int arg2, int arg3);
extern int LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(float value);
extern void LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(int arg0, int arg1, int arg2, int arg3);

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


extern ResidentState_Fd160fb9c LVL_8_TABORA_Fd160fb9c_D_00189E20;

void LVL_8_TABORA_FUN_002B8830(void)
{
    int selector;

    if (LVL_8_TABORA_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_8_TABORA_Fd160fb9c_D_00189E20.f1B8 < 15)
            return;
    }

    selector = LVL_8_TABORA_Fd160fb9c_D_00189E20.b149D;

    if (LVL_8_TABORA_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_8_TABORA_Fd160fb9c_D_00189E20.f1B8 == 22)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 0, 1, 30);
    }

    switch (LVL_8_TABORA_Fd160fb9c_D_00189E20.p2290[67]) {
    case 3:
        if (LVL_8_TABORA_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(49.5f) != 0)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 0, 1, 30);
        if (LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(17.0f) != 0)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 1, 1, 30);
        return;
    case 4:
        if (LVL_8_TABORA_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(12.5f) != 0)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 0, 1, 30);
        if (LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(1.0f) != 0)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 1, 1, 30);
        return;
    case 20:
        if (LVL_8_TABORA_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(8.0f) != 0)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 0, 1, 30);
        if (LVL_8_TABORA_Fd160fb9c_FUN_002C4FA0(21.0f) != 0)
            LVL_8_TABORA_Fd160fb9c_FUN_002B85D8(selector, 1, 1, 30);
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


extern Persistent_F882f1178 LVL_8_TABORA_F882f1178_D_00189E20;
extern int LVL_8_TABORA_F882f1178_D_001A8F00 __attribute__((sda));

extern float LVL_8_TABORA_F882f1178_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F882f1178_FUN_002FA3E0(float);
extern int LVL_8_TABORA_F882f1178_FUN_002FAF80(int, int, float);
extern float LVL_8_TABORA_F882f1178_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F882f1178_FUN_002FADE0(float, float);
extern float LVL_8_TABORA_F882f1178_FUN_002FA3E0(float);
extern int LVL_8_TABORA_F882f1178_FUN_002FAF80(int, int, float);
extern void LVL_8_TABORA_F882f1178_FUN_00334798(int, float *, int, int, float);
extern void LVL_8_TABORA_F882f1178_FUN_00334798(int, float *, int, int, float);

void LVL_8_TABORA_FUN_002E0820(void)
{
    Node_F882f1178 *node;

    node = (Node_F882f1178 *)LVL_8_TABORA_F882f1178_D_00189E20.p1360;
    if (node != 0) {
        if (*(int *)0x1A8F00 == 2)
            LVL_8_TABORA_F882f1178_D_00189E20.f250C = LVL_8_TABORA_F882f1178_FUN_002FADE0(LVL_8_TABORA_F882f1178_D_00189E20.f250C, 0.020362177863717079f);
        else
            LVL_8_TABORA_F882f1178_D_00189E20.f250C = LVL_8_TABORA_F882f1178_FUN_002FADE0(LVL_8_TABORA_F882f1178_D_00189E20.f250C, 0.034906592220067978f);
        node->field120 = LVL_8_TABORA_F882f1178_FUN_002FAF80(0xd2d2d2, 0x285050,
                                 LVL_8_TABORA_F882f1178_FUN_002FA3E0(LVL_8_TABORA_F882f1178_D_00189E20.f250C) * 0.5f + 0.5f);
    }
    node = (Node_F882f1178 *)LVL_8_TABORA_F882f1178_D_00189E20.p1364;
    if (node != 0) {
        if (LVL_8_TABORA_F882f1178_D_001A8F00 != 2) {
            float limit;
            int which;

            limit = LVL_8_TABORA_F882f1178_FUN_002FADE0(LVL_8_TABORA_F882f1178_D_00189E20.f2510, 0.049451004713773727f);
            LVL_8_TABORA_F882f1178_D_00189E20.f2510 = limit;
            which = 0x1ee628;
            if (limit > 0.0f && limit < 1.9198623895645142f) {
                which = 0x1e1ed2;
            }
            node->field120 = LVL_8_TABORA_F882f1178_FUN_002FAF80(node->field120, which, 0.070000000298023224f);
        } else {
            LVL_8_TABORA_F882f1178_D_00189E20.f2510 = LVL_8_TABORA_F882f1178_FUN_002FADE0(LVL_8_TABORA_F882f1178_D_00189E20.f2510, 0.026179943233728409f);
            node->field120 = LVL_8_TABORA_F882f1178_FUN_002FAF80(0x1e1ed2, 0x1e1e50,
                                     LVL_8_TABORA_F882f1178_FUN_002FA3E0(LVL_8_TABORA_F882f1178_D_00189E20.f2510) * 0.5f + 0.5f);
        }
    }
    if (LVL_8_TABORA_F882f1178_D_00189E20.p1368 != 0) {
        if (LVL_8_TABORA_F882f1178_D_001A8F00 == 2)
            LVL_8_TABORA_F882f1178_FUN_00334798((int)LVL_8_TABORA_F882f1178_D_00189E20.p1368, &LVL_8_TABORA_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.026179943233728409f);
        else
            LVL_8_TABORA_F882f1178_FUN_00334798((int)LVL_8_TABORA_F882f1178_D_00189E20.p1368, &LVL_8_TABORA_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.040724355727434158f);
    }
}
extern void LVL_8_TABORA_F8411efa9_FUN_003EBFF8(char *pkt);
extern long long LVL_8_TABORA_F8411efa9_FUN_002F2568(char *p);
extern float LVL_8_TABORA_F8411efa9_FUN_002FA490(float x, float y);
extern float LVL_8_TABORA_F8411efa9_FUN_002F9F50(char *p);
extern float LVL_8_TABORA_F8411efa9_FUN_002FA490(float x, float y);
extern void LVL_8_TABORA_F8411efa9_FUN_002FA658(float *matrix, float *quat);
extern void LVL_8_TABORA_F8411efa9_FUN_002FA0B8(float *off, char *src, float scale);
extern void LVL_8_TABORA_F8411efa9_FUN_003EC080(char *pkt, float angle);
extern void LVL_8_TABORA_F8411efa9_FUN_002F6D38(char *pkt, float *matrix, int mode);

void LVL_8_TABORA_FUN_003EC2A0(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_8_TABORA_F8411efa9_FUN_003EBFF8(pkt);
    r = LVL_8_TABORA_F8411efa9_FUN_002F2568(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_8_TABORA_F8411efa9_FUN_002FA490(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_8_TABORA_F8411efa9_FUN_002FA490(LVL_8_TABORA_F8411efa9_FUN_002F9F50(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_8_TABORA_F8411efa9_FUN_002FA658(m, quat);
    LVL_8_TABORA_F8411efa9_FUN_002FA0B8(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_8_TABORA_F8411efa9_FUN_003EC080(pkt, f12);
    LVL_8_TABORA_F8411efa9_FUN_002F6D38(pkt, m, 0);
}
extern void LVL_8_TABORA_F8411efa9_FUN_003EF5A0(char *pkt);
extern long long LVL_8_TABORA_F8411efa9_FUN_002F2568(char *p);
extern float LVL_8_TABORA_F8411efa9_FUN_002FA490(float x, float y);
extern float LVL_8_TABORA_F8411efa9_FUN_002F9F50(char *p);
extern float LVL_8_TABORA_F8411efa9_FUN_002FA490(float x, float y);
extern void LVL_8_TABORA_F8411efa9_FUN_002FA658(float *matrix, float *quat);
extern void LVL_8_TABORA_F8411efa9_FUN_002FA0B8(float *off, char *src, float scale);
extern void LVL_8_TABORA_F8411efa9_FUN_003EF628(char *pkt, float angle);
extern void LVL_8_TABORA_F8411efa9_FUN_002F6D38(char *pkt, float *matrix, int mode);

void LVL_8_TABORA_FUN_003EF690(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_8_TABORA_F8411efa9_FUN_003EF5A0(pkt);
    r = LVL_8_TABORA_F8411efa9_FUN_002F2568(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_8_TABORA_F8411efa9_FUN_002FA490(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_8_TABORA_F8411efa9_FUN_002FA490(LVL_8_TABORA_F8411efa9_FUN_002F9F50(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_8_TABORA_F8411efa9_FUN_002FA658(m, quat);
    LVL_8_TABORA_F8411efa9_FUN_002FA0B8(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_8_TABORA_F8411efa9_FUN_003EF628(pkt, f12);
    LVL_8_TABORA_F8411efa9_FUN_002F6D38(pkt, m, 0);
}
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);

void LVL_8_TABORA_FUN_003E4888(char *p, float scale)
{
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p, p, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 16, p + 16, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 32, p + 32, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 48, p + 48, scale);
}
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);

void LVL_8_TABORA_FUN_003EC080(char *p, float scale)
{
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p, p, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 16, p + 16, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 32, p + 32, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 48, p + 48, scale);
}
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);

void LVL_8_TABORA_FUN_003EF628(char *p, float scale)
{
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p, p, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 16, p + 16, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 32, p + 32, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 48, p + 48, scale);
}
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);

void LVL_8_TABORA_FUN_003F5908(char *p, float scale)
{
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p, p, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 16, p + 16, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 32, p + 32, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 48, p + 48, scale);
}
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);

void LVL_8_TABORA_FUN_00400438(char *p, float scale)
{
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p, p, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 16, p + 16, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 32, p + 32, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 48, p + 48, scale);
}
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);
extern void LVL_8_TABORA_F2c74c194_FUN_002F9E60(char *dst, char *src, float scale);

void LVL_8_TABORA_FUN_004017E0(char *p, float scale)
{
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p, p, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 16, p + 16, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 32, p + 32, scale);
    LVL_8_TABORA_F2c74c194_FUN_002F9E60(p + 48, p + 48, scale);
}




extern u8 LVL_8_TABORA_F458c670e_D_00189E20[];

extern void LVL_8_TABORA_F458c670e_FUN_0032B108(f32 *a, f32 *b, f32 c, f32 d, f32 e, f32 f);
extern f32 LVL_8_TABORA_F458c670e_FUN_002F9D60(f32 v);
extern void LVL_8_TABORA_F458c670e_FUN_0032B048(f32 *p, f32 v, f32 w);

#ifndef RAC2_T_RES_F458C670E
#define RAC2_T_RES_F458C670E
typedef struct {
    u8 pad000[0x88];
    f32 f088;
    u8 pad08C[0x1B8 - 0x08C];
    s32 i1B8;
    u8 pad1BC[0x330 - 0x1BC];
    f32 f330;
    u8 pad334[0x790 - 0x334];
    f32 f790;
    f32 f794;
    u8 pad798[4];
    f32 f79C;
    f32 f7A0;
    u8 pad7A4[0x9B0 - 0x7A4];
    f32 f9B0;
    u8 pad9B4[0xA3C - 0x9B4];
    f32 fA3C;
    u8 padA40[0x2294 - 0xA40];
    s32 i2294;
    u8 pad2298[4];
    s32 i229C;
    u8 pad22A0[4];
    s32 i22A4;
    u8 pad22A8[8];
    s32 i22B0;
} Res_F458c670e;
#endif


void LVL_8_TABORA_FUN_002D6F48(void)
{
    f32 old, v, r, sum;

    if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i1B8 >= 11
        || ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790 < 0.0f
        || ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i22A4 == 17
        || ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i22B0 == 17) {
        if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i2294 == 53) {
            LVL_8_TABORA_F458c670e_FUN_0032B108(&((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790,
                    &((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->fA3C,
                    0.0f, 0.03f, 0.3f, 0.025000002f);
            ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794 = 0.0f;
        } else {
            old = ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790;
            v = ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794;
            ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794 =
                v - (old * 0.005f + v * 0.045f);
            r = LVL_8_TABORA_F458c670e_FUN_002F9D60(old);
            if (r < 0.001f) {
                if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794 < 0.0001f)
                    ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794 = 0.0f;
            }
        }
    }
    sum = ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790
        + ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794;
    ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790 = sum;
    if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i2294 == 53) {
        if (sum > 0.0f) {
            if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794 > 0.0f)
                ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f794 = 0.0f;
            LVL_8_TABORA_F458c670e_FUN_0032B048(&((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790,
                    0.0f, 0.011666667f);
        }
    }
    if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i2294 != 124) {
        f32 x = ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f330;
        if (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->i229C == 22)
            x = ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f9B0;
        LVL_8_TABORA_F458c670e_FUN_0032B108(&((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f79C,
                &((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f7A0,
                x, 0.027f, 0.3f, 0.0f);
    }
    ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f088 =
        (((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f79C + (-0.12f))
        + ((Res_F458c670e *)LVL_8_TABORA_F458c670e_D_00189E20)->f790;
}
extern void LVL_8_TABORA_F40487154_FUN_002FA638(char *a, char *b);
extern float LVL_8_TABORA_F40487154_FUN_002FA3E0(float value);
extern void LVL_8_TABORA_F40487154_FUN_002F9E60(char *a, char *b, float value);
extern float LVL_8_TABORA_F40487154_FUN_002FADE0(float value, float scale);

void LVL_8_TABORA_FUN_003F2148(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_8_TABORA_F40487154_FUN_002FA638(object + 192, object + 240);
    x = LVL_8_TABORA_F40487154_FUN_002FA3E0(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_8_TABORA_F40487154_FUN_002FA3E0(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_8_TABORA_F40487154_FUN_002FA3E0(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_8_TABORA_F40487154_FUN_002F9E60(object + 192, object + 192, x);
    LVL_8_TABORA_F40487154_FUN_002F9E60(object + 208, object + 208, y);
    LVL_8_TABORA_F40487154_FUN_002F9E60(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_8_TABORA_F40487154_FUN_002FADE0(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_8_TABORA_F40487154_FUN_002FADE0(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_8_TABORA_F40487154_FUN_002FADE0(*(float *)(data + 12), 0.5f);
}
extern void LVL_8_TABORA_F40487154_FUN_002FA638(char *a, char *b);
extern float LVL_8_TABORA_F40487154_FUN_002FA3E0(float value);
extern void LVL_8_TABORA_F40487154_FUN_002F9E60(char *a, char *b, float value);
extern float LVL_8_TABORA_F40487154_FUN_002FADE0(float value, float scale);

void LVL_8_TABORA_FUN_003FDCC8(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_8_TABORA_F40487154_FUN_002FA638(object + 192, object + 240);
    x = LVL_8_TABORA_F40487154_FUN_002FA3E0(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_8_TABORA_F40487154_FUN_002FA3E0(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_8_TABORA_F40487154_FUN_002FA3E0(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_8_TABORA_F40487154_FUN_002F9E60(object + 192, object + 192, x);
    LVL_8_TABORA_F40487154_FUN_002F9E60(object + 208, object + 208, y);
    LVL_8_TABORA_F40487154_FUN_002F9E60(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_8_TABORA_F40487154_FUN_002FADE0(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_8_TABORA_F40487154_FUN_002FADE0(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_8_TABORA_F40487154_FUN_002FADE0(*(float *)(data + 12), 0.5f);
}
extern int LVL_8_TABORA_F6eb4f363_FUN_00327858(int mode);
extern float LVL_8_TABORA_F6eb4f363_FUN_00329BF0(char *object, char *local);
extern void LVL_8_TABORA_F6eb4f363_FUN_002F9E60(char *local, int value, float scale);
extern float LVL_8_TABORA_F6eb4f363_FUN_003278F0(float low, float high);
extern void LVL_8_TABORA_F6eb4f363_FUN_00344D98(char *object, char *local, float amount, float base);

void LVL_8_TABORA_FUN_003969C8(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_8_TABORA_F6eb4f363_FUN_00327858(5))
        return;
    base = LVL_8_TABORA_F6eb4f363_FUN_00329BF0(object + 16, local);
    LVL_8_TABORA_F6eb4f363_FUN_002F9E60(local, value, 0.25f);
    LVL_8_TABORA_F6eb4f363_FUN_00344D98(object + 16, local, LVL_8_TABORA_F6eb4f363_FUN_003278F0(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_8_TABORA_F6eb4f363_FUN_00327858(int mode);
extern float LVL_8_TABORA_F6eb4f363_FUN_00329BF0(char *object, char *local);
extern void LVL_8_TABORA_F6eb4f363_FUN_002F9E60(char *local, int value, float scale);
extern float LVL_8_TABORA_F6eb4f363_FUN_003278F0(float low, float high);
extern void LVL_8_TABORA_F6eb4f363_FUN_00344D98(char *object, char *local, float amount, float base);

void LVL_8_TABORA_FUN_00399470(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_8_TABORA_F6eb4f363_FUN_00327858(5))
        return;
    base = LVL_8_TABORA_F6eb4f363_FUN_00329BF0(object + 16, local);
    LVL_8_TABORA_F6eb4f363_FUN_002F9E60(local, value, 0.25f);
    LVL_8_TABORA_F6eb4f363_FUN_00344D98(object + 16, local, LVL_8_TABORA_F6eb4f363_FUN_003278F0(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_8_TABORA_F6eb4f363_FUN_00327858(int mode);
extern float LVL_8_TABORA_F6eb4f363_FUN_00329BF0(char *object, char *local);
extern void LVL_8_TABORA_F6eb4f363_FUN_002F9E60(char *local, int value, float scale);
extern float LVL_8_TABORA_F6eb4f363_FUN_003278F0(float low, float high);
extern void LVL_8_TABORA_F6eb4f363_FUN_00344D98(char *object, char *local, float amount, float base);

void LVL_8_TABORA_FUN_0039F870(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_8_TABORA_F6eb4f363_FUN_00327858(5))
        return;
    base = LVL_8_TABORA_F6eb4f363_FUN_00329BF0(object + 16, local);
    LVL_8_TABORA_F6eb4f363_FUN_002F9E60(local, value, 0.25f);
    LVL_8_TABORA_F6eb4f363_FUN_00344D98(object + 16, local, LVL_8_TABORA_F6eb4f363_FUN_003278F0(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_8_TABORA_Fc10c1216_FUN_002F9A90(char *);
extern void LVL_8_TABORA_Fc10c1216_FUN_0033E208(char *);
extern int LVL_8_TABORA_Fc10c1216_FUN_002FAF38(float);
extern void LVL_8_TABORA_Fc10c1216_FUN_002F9DF0(char *, char *, char *);

void LVL_8_TABORA_FUN_00344028(char *p)
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

    if (q[1] <= 0.0244f || LVL_8_TABORA_Fc10c1216_FUN_002F9A90(p + 10) != 0) {
        LVL_8_TABORA_Fc10c1216_FUN_0033E208(p);
        return;
    }

    if (q[1] >= 0.12f) {
        q[3] = -q[3];
        q[1] = q[1] + q[3] * 0.007f;
    }
    *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    *(int *)(p + 4) = (*(int *)(p + 4) & 0xffffff) | (LVL_8_TABORA_Fc10c1216_FUN_002FAF38(q[1] * 255.0f) << 24);
    q[2] = q[2] + 0.002f;
    if (q[2] > 1.0f)
        q[2] = q[2] - 1.0f;
    *(char *)(p + 8) = (char)(q[2] * 255.0f);
    LVL_8_TABORA_Fc10c1216_FUN_002F9DF0(p + 16, p + 16, (char *)(q + 4));
}
