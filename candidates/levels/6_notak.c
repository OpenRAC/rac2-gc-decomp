typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_6_NOTAK_D_001A8EB0;
void LVL_6_NOTAK_FUN_00317028(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_6_NOTAK_FUN_003332A8(s32 index) {
    NativeTable20 values=LVL_6_NOTAK_D_001A8EB0;
    return values.items[index];
}

u32 LVL_6_NOTAK_FUN_002ED5D0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_6_NOTAK_D_0018C0B4;
u32 LVL_6_NOTAK_FUN_00316068(void) {
    return LVL_6_NOTAK_D_0018C0B4;
}

s32 LVL_6_NOTAK_FUN_00323150(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_6_NOTAK_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_6_NOTAK_FUN_002ED468(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_6_NOTAK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_6_NOTAK_FUN_002ED4A0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_6_NOTAK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_6_NOTAK_FUN_002EE0C0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_6_NOTAK_FUN_00316D80(f32, f32, f32, f32, s32, s32);

void LVL_6_NOTAK_FUN_00316828(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_6_NOTAK_FUN_00316D80(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_6_NOTAK_FUN_0032FB00(int width, int height, int address, int mode);
extern void LVL_6_NOTAK_FUN_003BC960(unsigned int reg, unsigned long value);
extern void LVL_6_NOTAK_FUN_0032FE68(int width, int height);

void LVL_6_NOTAK_FUN_00324EF0(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_6_NOTAK_FUN_0032FB00(width, height, address, 1);
    LVL_6_NOTAK_FUN_003BC960(0x47, 0x30000UL);
    LVL_6_NOTAK_FUN_003BC960(0x42, 0x8000000044UL);
    LVL_6_NOTAK_FUN_0032FE68(0x100, 0x100);
    LVL_6_NOTAK_FUN_003BC960(0x42, 0x8000000044UL);
}

extern void LVL_6_NOTAK_FUN_00312E60(f32, f32, f32, f32 *, s32);

void LVL_6_NOTAK_FUN_002ED2F8(f32 *output) {
 LVL_6_NOTAK_FUN_00312E60(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_6_NOTAK_FUN_002ED4D8(s32 index) {
 s32 value=LVL_6_NOTAK_FUN_002ED4A0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_6_NOTAK_FUN_002ED510(s32 index) {
 return LVL_6_NOTAK_FUN_002ED4A0(index)==47;
}

s32 LVL_6_NOTAK_FUN_002F29C8(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

typedef struct {
    u32 unknown;
    s32 data_offset;
    s32 length;
    s32 extra;
} PackedHeaderCountView;

s32 LVL_6_NOTAK_FUN_003BEF00(const PackedHeaderCountView *header) {
    s32 fixed = header->extra + 16;
    s32 total = header->data_offset + fixed + header->length;
    return ((total + 15) / 16) * 4;
}

f32 LVL_6_NOTAK_FUN_00460B50(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_6_NOTAK_FUN_003829F0(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_6_NOTAK_D_00287F80[13];
void LVL_6_NOTAK_FUN_003363B0(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_6_NOTAK_D_00287F80[i].key == key) break;
    }
    if (i < 13) {
        LVL_6_NOTAK_D_00287F80[i].fields[9] = value;
        if (LVL_6_NOTAK_D_00287F80[i].busy == 0)
            LVL_6_NOTAK_D_00287F80[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_6_NOTAK_D_001395B8[];
u32 LVL_6_NOTAK_FUN_00344E30(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_6_NOTAK_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_6_NOTAK_D_00231DC0[];
s32 LVL_6_NOTAK_FUN_003C14C8(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_6_NOTAK_D_00231DC0;
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
extern int LVL_6_NOTAK_D_0022E680[];
extern ListOverrideObject *LVL_6_NOTAK_D_00227180[];
extern ListOverridePair LVL_6_NOTAK_D_0022E080[];
void LVL_6_NOTAK_FUN_003AC170(void)
{
    int *selected = LVL_6_NOTAK_D_0022E680;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_6_NOTAK_D_00227180[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_6_NOTAK_D_0022E080[row->key];
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
extern NativeObjectSearchRecord32 LVL_6_NOTAK_D_00254A30[];
s32 LVL_6_NOTAK_FUN_0043BBA8(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_6_NOTAK_D_00254A30[index].field14 == object) {
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

s32 LVL_6_NOTAK_FUN_00315F28(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_6_NOTAK_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_6_NOTAK_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_6_NOTAK_D_00189E20)->mode == 0x31) {
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
extern void *LVL_6_NOTAK_D_0018C0B0;
extern void *LVL_6_NOTAK_D_0018B134;
extern void *LVL_6_NOTAK_D_0018B040;
void *LVL_6_NOTAK_FUN_002E45E8(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_6_NOTAK_D_0018C0B0;
    if (kind == 1)
        return LVL_6_NOTAK_D_0018B134;
    if (kind == 6)
        return LVL_6_NOTAK_D_0018B040;
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
extern unsigned char LVL_6_NOTAK_D_00139568[];
extern MappedClassEntry LVL_6_NOTAK_D_0026C7F0[];
unsigned int LVL_6_NOTAK_FUN_00332F80(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_6_NOTAK_D_0026C7F0[LVL_6_NOTAK_D_00139568[i]];
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
extern s32 LVL_6_NOTAK_FUN_0032D308(s32 value);
NativeCompactHeader *LVL_6_NOTAK_FUN_003C3860(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_6_NOTAK_FUN_0032D308(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_6_NOTAK_FUN_0032D308(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_6_NOTAK_D_001A79F0;
s32 LVL_6_NOTAK_FUN_00315EA8(void) {
    s32 found = 0;
    if (LVL_6_NOTAK_D_001A79F0 == 25 || LVL_6_NOTAK_D_001A79F0 == 5 ||
        LVL_6_NOTAK_D_001A79F0 == 10 || LVL_6_NOTAK_D_001A79F0 == 15) {
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
void LVL_6_NOTAK_FUN_00312950(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_6_NOTAK_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_6_NOTAK_FUN_00312988(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_6_NOTAK_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags &= ~1;
    secondary = root->secondary;
    if (secondary) secondary->flags &= ~1;
}

typedef struct {
    u8 prefix[0x40];
    s32 mode;
    u8 between[0x14];
    s32 state;
} StateTransitionView;
typedef char StateTransitionViewSize[(sizeof(StateTransitionView) == 0x5c) ? 1 : -1];
extern StateTransitionView LVL_6_NOTAK_D_001BF980;
void LVL_6_NOTAK_FUN_003328E8(void) {
    if (LVL_6_NOTAK_D_001BF980.mode == 7 && LVL_6_NOTAK_D_001BF980.state == 1) {
        LVL_6_NOTAK_D_001BF980.state = 2;
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
extern DobboFormatRoot396 LVL_6_NOTAK_D_001C9E20;
extern const char LVL_6_NOTAK_D_001A99E0[];
extern const char LVL_6_NOTAK_D_001A99E8[];
extern const unsigned char *LVL_6_NOTAK_FUN_00333B40(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_6_NOTAK_FUN_003484B0(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_6_NOTAK_FUN_00333B40(LVL_6_NOTAK_D_001C9E20.rows[index].text_id);
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
        int key = LVL_6_NOTAK_D_001C9E20.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_6_NOTAK_D_0026C7F0[LVL_6_NOTAK_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_6_NOTAK_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_6_NOTAK_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_6_NOTAK_FUN_002F3118(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_6_NOTAK_D_00189E20;
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
extern int LVL_6_NOTAK_FUN_003A0AA8(int, unsigned int, void *);
void LVL_6_NOTAK_FUN_0048B6F0(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_6_NOTAK_FUN_003A0AA8(3, 0, 0);
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
        LVL_6_NOTAK_FUN_003A0AA8(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_6_NOTAK_FUN_003A0AA8(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_6_NOTAK_FUN_003A0AA8(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_6_NOTAK_D_001B2780[16];
extern u32 LVL_6_NOTAK_D_001B27C0[16];

int LVL_6_NOTAK_FUN_00351290(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_6_NOTAK_D_001B2780[index] == 0 ||
            LVL_6_NOTAK_D_001B2780[index] == object) {
            LVL_6_NOTAK_D_001B2780[index] = object;
            LVL_6_NOTAK_D_001B27C0[index] = 0;
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
extern void LVL_6_NOTAK_FUN_0031C1B0(OozlaAppendObject164 *, int, const float *);
extern void LVL_6_NOTAK_FUN_0032D5F8(float *, const float *, const float *);
extern void LVL_6_NOTAK_FUN_0032DA88(float *, const float *, const float *);
extern void LVL_6_NOTAK_FUN_0031C4F8(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_6_NOTAK_FUN_0031C108(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_6_NOTAK_FUN_0031C1B0(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_6_NOTAK_FUN_0032D5F8(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_6_NOTAK_FUN_0032DA88(difference, difference, &object->transform[0][0]);
        LVL_6_NOTAK_FUN_0031C4F8(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_6_NOTAK_FUN_00490B68(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_6_NOTAK_FUN_00491000(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_6_NOTAK_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_6_NOTAK_FUN_0036DE70(void) {
    if (((CallState *)LVL_6_NOTAK_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_6_NOTAK_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_6_NOTAK_D_001A63A8)->active); ((CallState *)LVL_6_NOTAK_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_6_NOTAK_FUN_0035A5C0(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_6_NOTAK_FUN_00344DB0(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_6_NOTAK_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_6_NOTAK_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

extern unsigned char D_19B278[];

int LVL_6_NOTAK_FUN_00366D58(void)
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

s32 LVL_6_NOTAK_FUN_002ED580(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_6_NOTAK_D_00189E20;

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

extern void LVL_6_NOTAK_FUN_00350388(NativeUpdate775View *object);

void LVL_6_NOTAK_FUN_003E1150(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_6_NOTAK_FUN_00350388(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_6_NOTAK_FUN_0036A278(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_6_NOTAK_FUN_0035FC28(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_6_NOTAK_FUN_002E66F0(void)
{
}


unsigned int LVL_6_NOTAK_FUN_00315FF8(void)
{
    return 0;
}


void LVL_6_NOTAK_FUN_0032C900(void)
{
}


void LVL_6_NOTAK_FUN_003338D8(void)
{
}


void LVL_6_NOTAK_FUN_00334688(void)
{
}


void LVL_6_NOTAK_FUN_0033BB48(void)
{
}


void LVL_6_NOTAK_FUN_0033BB50(void)
{
}


void LVL_6_NOTAK_FUN_0033FB10(void)
{
}


void LVL_6_NOTAK_FUN_00348868(void)
{
}


unsigned int LVL_6_NOTAK_FUN_0035E908(void)
{
    return 0;
}


unsigned int LVL_6_NOTAK_FUN_003B0838(void)
{
    return 0;
}


void LVL_6_NOTAK_FUN_003B5350(void)
{
}


void LVL_6_NOTAK_FUN_003BC368(void)
{
}


void LVL_6_NOTAK_FUN_003C1F78(void)
{
}


void LVL_6_NOTAK_FUN_003C5690(void)
{
}


void LVL_6_NOTAK_FUN_004383D0(void)
{
}


void LVL_6_NOTAK_FUN_00459B10(void)
{
}


void LVL_6_NOTAK_FUN_004635E0(void)
{
}


void LVL_6_NOTAK_FUN_0047E328(void)
{
}


void LVL_6_NOTAK_FUN_0047FD98(void)
{
}


void LVL_6_NOTAK_FUN_0047FFE8(void)
{
}


void LVL_6_NOTAK_FUN_004804E0(void)
{
}


void LVL_6_NOTAK_FUN_00488D58(void)
{
}


void LVL_6_NOTAK_FUN_004899B0(void)
{
}


void LVL_6_NOTAK_FUN_004957B8(void)
{
}


void LVL_6_NOTAK_FUN_00497B48(void)
{
}


void LVL_6_NOTAK_FUN_004993E0(void)
{
}
void LVL_6_NOTAK_FUN_003FA500(char *p)
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
void LVL_6_NOTAK_FUN_00406D98(char *p)
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
void LVL_6_NOTAK_FUN_00412368(char *p)
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
void LVL_6_NOTAK_FUN_00419210(char *p)
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
void LVL_6_NOTAK_FUN_0041DC30(char *p)
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
void LVL_6_NOTAK_FUN_00422530(char *p)
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
void LVL_6_NOTAK_FUN_00428810(char *p)
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
void LVL_6_NOTAK_FUN_00433798(char *p)
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
void LVL_6_NOTAK_FUN_00434B40(char *p)
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
void LVL_6_NOTAK_FUN_0045F470(char *p)
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
void LVL_6_NOTAK_FUN_00469288(char *p)
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

extern u8 LVL_6_NOTAK_F62e6ff2b_D_00189E20[];
extern u8 LVL_6_NOTAK_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_6_NOTAK_FUN_003A06A0(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_6_NOTAK_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_6_NOTAK_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_6_NOTAK_F62e6ff2b_D_00188660;
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

extern u8 LVL_6_NOTAK_F1c0a2bbf_D_001ADD18[];
extern u8 LVL_6_NOTAK_F1c0a2bbf_D_001ADD38[];
extern void LVL_6_NOTAK_F1c0a2bbf_FUN_00115E38(void *, s32, void *);

void LVL_6_NOTAK_FUN_0047E340(u32 *p, u32 a1, u32 a2, u32 a3)
{
    if (a1 < 4u)
        LVL_6_NOTAK_F1c0a2bbf_FUN_00115E38(LVL_6_NOTAK_F1c0a2bbf_D_001ADD18, 37, LVL_6_NOTAK_F1c0a2bbf_D_001ADD38);
    p[0] = a2;
    p[1] = a3;
    p[2] = a1;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
void LVL_6_NOTAK_FUN_003E8E48(char *p)
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
void LVL_6_NOTAK_FUN_0042B4B0(char *p)
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
extern Blob LVL_6_NOTAK_F6894d7c1_D_001A8E60;
int LVL_6_NOTAK_FUN_00332BB0(int x)
{
    Blob b;
    int i;
    b = LVL_6_NOTAK_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_6_NOTAK_Fafab4c55_FUN_00115DA8(char *, char *, ...);
extern char LVL_6_NOTAK_Fafab4c55_D_001AD5F0[];
extern char LVL_6_NOTAK_Fafab4c55_D_001AD600[];
extern char LVL_6_NOTAK_Fafab4c55_D_001AD608[];

void LVL_6_NOTAK_FUN_003B6D50(char *dst, int value)
{
    if (value > 999999)
        LVL_6_NOTAK_Fafab4c55_FUN_00115DA8(dst, LVL_6_NOTAK_Fafab4c55_D_001AD5F0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_6_NOTAK_Fafab4c55_FUN_00115DA8(dst, LVL_6_NOTAK_Fafab4c55_D_001AD600, value / 1000, value % 1000);
    else
        LVL_6_NOTAK_Fafab4c55_FUN_00115DA8(dst, LVL_6_NOTAK_Fafab4c55_D_001AD608, value);
}
typedef struct {
    char *base;
    unsigned limit;
    unsigned size;
    unsigned cur;
    int count;
    void *free;
} Hdr;

extern int LVL_6_NOTAK_Fe77c6258_FUN_00115E38(char *, int, char *);
extern char LVL_6_NOTAK_Fe77c6258_D_001ADD18[];
extern char LVL_6_NOTAK_Fe77c6258_D_001ADD60[];

void *LVL_6_NOTAK_FUN_0047E3C8(Hdr *p)
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
        LVL_6_NOTAK_Fe77c6258_FUN_00115E38(LVL_6_NOTAK_Fe77c6258_D_001ADD18, 83, LVL_6_NOTAK_Fe77c6258_D_001ADD60);
        return 0;
    }
    r = p->base + cur;
    p->cur = cur + p->size;
    p->count++;
    return r;
}
void LVL_6_NOTAK_FUN_003CA858(char *p)
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
void LVL_6_NOTAK_FUN_003CCEE0(char *p)
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
void LVL_6_NOTAK_FUN_0046C128(char *p)
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

extern Entry *LVL_6_NOTAK_Fc68ad20a_D_0018C2B8;

int LVL_6_NOTAK_FUN_003116F8(int key, int *out)
{
    Entry *e = LVL_6_NOTAK_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_6_NOTAK_F55a1acb8_D_001A63A8;
extern short LVL_6_NOTAK_F55a1acb8_D_001A63AC;
extern int LVL_6_NOTAK_F55a1acb8_FUN_00133688(void);
extern void LVL_6_NOTAK_F55a1acb8_FUN_0011AEA0(int);

void LVL_6_NOTAK_FUN_0036EF40(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_6_NOTAK_F55a1acb8_FUN_00133688()) {
        LVL_6_NOTAK_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_6_NOTAK_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_6_NOTAK_F55a1acb8_D_001A63A8.f6;
    q = LVL_6_NOTAK_F55a1acb8_D_001A63A8.f18;
    LVL_6_NOTAK_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_6_NOTAK_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_6_NOTAK_F55a1acb8_D_001A63A8.f1C;
        LVL_6_NOTAK_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_6_NOTAK_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_6_NOTAK_Fa2dbe766_D_00189E20[];

int LVL_6_NOTAK_FUN_0040C200(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_6_NOTAK_Fa2dbe766_D_00189E20;
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
void LVL_6_NOTAK_FUN_003877B0(void)
{
    *(short *)0x001AA802 = (*(unsigned char *)0x001A7BC9) ? 3 : 0;
    *(short *)0x001AA81A = (*(unsigned char *)0x001A7BCA) ? 3 : 0;
    *(short *)0x001AA832 = (*(unsigned char *)0x001A7BCB) ? 3 : 0;
    *(short *)0x001AA84A = (*(unsigned char *)0x001A7BCC) ? 3 : 0;
    *(short *)0x001AA862 = (*(unsigned char *)0x001A7BCE) ? 3 : 0;
}
void LVL_6_NOTAK_FUN_00450B08(char *object)
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
extern char LVL_6_NOTAK_Fea34650e_D_00189E20[];

void LVL_6_NOTAK_FUN_003127E8(void)
{
    char *b = LVL_6_NOTAK_Fea34650e_D_00189E20;
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
        char *c = LVL_6_NOTAK_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_6_NOTAK_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_6_NOTAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_6_NOTAK_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_6_NOTAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_6_NOTAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_6_NOTAK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_6_NOTAK_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_6_NOTAK_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_6_NOTAK_FUN_0042BCE0(char *p)
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
void LVL_6_NOTAK_FUN_0046C0C0(char *p)
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
extern char LVL_6_NOTAK_F0be97c76_D_00189E20[];

void LVL_6_NOTAK_FUN_002E6670(void)
{
    char *base = LVL_6_NOTAK_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_6_NOTAK_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_6_NOTAK_Fee2b87d1_D_00152CD0;

s32 LVL_6_NOTAK_FUN_003444F0(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_6_NOTAK_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_6_NOTAK_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_6_NOTAK_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_6_NOTAK_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* Ring header advance: the four header words are written, then the resident
   pointer itself is advanced.  The final store is the only access the retail
   body makes through $gp, hence the -G8 profile (same shape as the qualified
   19_grelbin body, with this overlay's own global addresses). */
extern int *LVL_6_NOTAK_F236d541c_D_001B2F88 __attribute__((sda));
extern int LVL_6_NOTAK_F236d541c_D_001A742C __attribute__((sda));

void LVL_6_NOTAK_FUN_0032FA90(void)
{
    *(int *)LVL_6_NOTAK_F236d541c_D_001B2F88 = 0x30000009;
    *(int *)((char *)LVL_6_NOTAK_F236d541c_D_001B2F88 + 4) = (LVL_6_NOTAK_F236d541c_D_001A742C + 192) & 0x0FFFFFFF;
    *(int *)((char *)LVL_6_NOTAK_F236d541c_D_001B2F88 + 8) = 0;
    *(int *)((char *)LVL_6_NOTAK_F236d541c_D_001B2F88 + 12) = 0x50000009;
    LVL_6_NOTAK_F236d541c_D_001B2F88 = (int *)((char *)LVL_6_NOTAK_F236d541c_D_001B2F88 + 16);
}
/* Paired-strip packet emitter, 372 bytes, placed in levels/18_damosel and
   levels/6_notak.  The body writes a 16-byte GIF header into the resident
   packet cursor (a small-data global), advances the cursor, writes the tag
   words and two packed 64-bit strip descriptors, then advances the cursor by
   another 48 bytes.  The retail loads the cursor absolutely and advances it
   through $gp, so the unit is compiled under the small-data profile (-O2 -G8). */
extern int *LVL_6_NOTAK_F25780968_D_001B2F88 __attribute__((sda));
extern int LVL_6_NOTAK_F25780968_D_001A7350 __attribute__((sda));
extern int LVL_6_NOTAK_F25780968_D_001A7354 __attribute__((sda));

void LVL_6_NOTAK_FUN_0033ABA0(int a0, int a1, int a2, int a3, int p4, int p5, int p6)
{
    long long *q;

    *(int *)((char *)LVL_6_NOTAK_F25780968_D_001B2F88 + 0) = 0x10000003;
    *(int *)((char *)LVL_6_NOTAK_F25780968_D_001B2F88 + 4) = 0;
    *(int *)((char *)LVL_6_NOTAK_F25780968_D_001B2F88 + 8) = 0;
    *(int *)((char *)LVL_6_NOTAK_F25780968_D_001B2F88 + 12) = 0x50000003;
    q = (long long *)LVL_6_NOTAK_F25780968_D_001B2F88;
    LVL_6_NOTAK_F25780968_D_001B2F88 = (int *)((char *)q + 16);
    q[2] = 0x4400000000008001LL;
    q[3] = 17424;
    q[4] = 70;
    q[5] = p4;
#define LO_D0 (*(int *)((char *)&LVL_6_NOTAK_F25780968_D_001A7350 + 0))
#define HI_D4 (*(int *)((char *)&LVL_6_NOTAK_F25780968_D_001A7354 + 0))
    if (p6 != 0) {
        q[6] = (a0 + LO_D0 - 8)
             | ((long long)(a1 + HI_D4 - 8) << 16)
             | ((long long)p5 << 32);
        q[7] = (a2 + LO_D0 - 8)
             | ((long long)(a3 + HI_D4 - 8) << 16)
             | ((long long)p5 << 32);
    } else {
        q[6] = ((a0 << 4) + LO_D0 - 16)
             | ((long long)((a1 << 4) + HI_D4 - 16) << 16)
             | ((long long)p5 << 32);
        q[7] = ((a2 << 4) + LO_D0 - 16)
             | ((long long)((a3 << 4) + HI_D4 - 16) << 16)
             | ((long long)p5 << 32);
    }
    LVL_6_NOTAK_F25780968_D_001B2F88 = (int *)((char *)LVL_6_NOTAK_F25780968_D_001B2F88 + 48);
}

/* 0x1B2FA0 / 0x1B2FA4 are 4-byte small-data objects (gp = 0x1AEFF0, offsets
   +0x3FB0 / +0x3FB4).  The retail addresses them absolutely in every normal
   reference and through $gp in the branch delay slot the reorg pass fills; the
   plain declarations give exactly that split. */
extern int LVL_6_NOTAK_Fafa454c6_D_001B2FA0 __attribute__((sda));
extern int LVL_6_NOTAK_Fafa454c6_D_001B2FA4 __attribute__((sda));

extern void LVL_6_NOTAK_Fafa454c6_FUN_0011A950(int, int);
extern void LVL_6_NOTAK_Fafa454c6_FUN_0011B658(int);

void LVL_6_NOTAK_FUN_003BD1A8(void)
{
    if ((*(volatile u32 *)0x1000E010 & 0x20000) != 0) {
        *(volatile u32 *)0x1000E010 = 0x20000;
    }
    LVL_6_NOTAK_Fafa454c6_FUN_0011A950(1, LVL_6_NOTAK_Fafa454c6_D_001B2FA0);
    LVL_6_NOTAK_Fafa454c6_FUN_0011A950(15, LVL_6_NOTAK_Fafa454c6_D_001B2FA4);
    LVL_6_NOTAK_Fafa454c6_FUN_0011B658(1);
    LVL_6_NOTAK_Fafa454c6_D_001B2FA0 = 0;
    LVL_6_NOTAK_Fafa454c6_D_001B2FA4 = 0;
}
typedef struct OBJ {
    char pad0[32];
    unsigned char f20;
    char pad1[0x64 - 33];
    void (*f64)(struct OBJ *);
    char pad2[0xAA - 0x68];
    short fAA;
} OBJ;

extern OBJ *LVL_6_NOTAK_F32969de2_D_001B2820 __attribute__((sda));
extern OBJ *LVL_6_NOTAK_F32969de2_D_001B2824 __attribute__((sda));
extern OBJ *LVL_6_NOTAK_F32969de2_D_001AC000 __attribute__((sda));

void LVL_6_NOTAK_FUN_004000C0(void)
{
    OBJ *p;
    short key = LVL_6_NOTAK_F32969de2_D_001AC000->fAA;

    for (p = LVL_6_NOTAK_F32969de2_D_001B2820; p < LVL_6_NOTAK_F32969de2_D_001B2824; p = (OBJ *)((char *)p + 256)) {
        if (p == 0)
            continue;
        if (p->fAA != key)
            continue;
        if (p->f20 == 254)
            continue;
        if (p->f20 == 253)
            continue;
        if (p == LVL_6_NOTAK_F32969de2_D_001AC000)
            continue;
        p->f64(p);
    }
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_6_NOTAK_F1157be91_D_001A63E8;
extern unsigned char LVL_6_NOTAK_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_6_NOTAK_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_6_NOTAK_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_6_NOTAK_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_6_NOTAK_F1157be91_FUN_00133230(void);
extern int LVL_6_NOTAK_F1157be91_FUN_00132028(void);

int LVL_6_NOTAK_FUN_0036EDC8(int a0, int a1, int a2) {
    CdMode mode = LVL_6_NOTAK_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_6_NOTAK_F1157be91_D_001A7900[0];
    LVL_6_NOTAK_F1157be91_D_001A7430[0] = 0;
    LVL_6_NOTAK_F1157be91_D_001A7434 = 0;
    LVL_6_NOTAK_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_6_NOTAK_F1157be91_FUN_00133230();
    LVL_6_NOTAK_F1157be91_FUN_00132028();
    return 1;
}
extern int *LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 __attribute__((sda));

void LVL_6_NOTAK_FUN_003BC960(unsigned int param_1, unsigned long param_2)
{
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 0) = 0x10000002;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 4) = 0;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 8) = 0;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 12) = 0x50000002;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 16) = 0x8001;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 20) = 0x10000000;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 24) = 14;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 28) = 0;
    *(long long *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 32) = param_2;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 40) = param_1;
    *(int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 44) = 0;
    LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 = (int *)((char *)LVL_6_NOTAK_Fe85cf1e5_D_001B2F88 + 48);
}
struct Sep6 {
    int v[6];
};

extern struct Sep6 LVL_6_NOTAK_Fe9186f4c_D_001AA4D0;
extern char LVL_6_NOTAK_Fe9186f4c_D_001AA4E8[];
extern char LVL_6_NOTAK_Fe9186f4c_D_001AA4F8[];
extern char LVL_6_NOTAK_Fe9186f4c_D_001AA508[];
extern void LVL_6_NOTAK_Fe9186f4c_FUN_00115DA8();

void LVL_6_NOTAK_FUN_0036F270(char *buf, int value, int index) {
    struct Sep6 sep = LVL_6_NOTAK_Fe9186f4c_D_001AA4D0;
    int rest;
    if (value > 999999) {
        rest = value % 1000000;
        LVL_6_NOTAK_Fe9186f4c_FUN_00115DA8(buf, LVL_6_NOTAK_Fe9186f4c_D_001AA4E8, value / 1000000, sep.v[index % 6], rest / 1000,
                sep.v[index % 6], rest % 1000);
    } else if (value >= 1000) {
        LVL_6_NOTAK_Fe9186f4c_FUN_00115DA8(buf, LVL_6_NOTAK_Fe9186f4c_D_001AA4F8, value / 1000, sep.v[index % 6], value % 1000);
    } else {
        LVL_6_NOTAK_Fe9186f4c_FUN_00115DA8(buf, LVL_6_NOTAK_Fe9186f4c_D_001AA508, value);
    }
}
/* Family 34e74db3063e7a24 — 108 bytes, 2 placements (18_damosel, 6_notak).
   Same shape as be569dc253cd5555: walk the resident function-pointer table at
   0x1B2180, call each entry while the resident count at 0x1B21C0 says there is
   one, then clear the count.  Small-data profile: the count is a 4-byte scalar,
   so cc1 prints the bare-symbol macro and gas expands every site on its own. */
extern int LVL_6_NOTAK_F34e74db3_D_001B21C0 __attribute__((sda));
extern void (*LVL_6_NOTAK_F34e74db3_D_001B2180[])(void);

void LVL_6_NOTAK_FUN_00317100(void)
{
    int i;

    for (i = 0; i < LVL_6_NOTAK_F34e74db3_D_001B21C0; i++) {
        LVL_6_NOTAK_F34e74db3_D_001B2180[i]();
    }

    LVL_6_NOTAK_F34e74db3_D_001B21C0 = 0;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_6_NOTAK_F4e5bde81_D_00189E20;
extern s32 LVL_6_NOTAK_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_6_NOTAK_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_6_NOTAK_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_6_NOTAK_FUN_00312B80(void) {
    s32 result = LVL_6_NOTAK_F4e5bde81_D_00189E20.field348;
    if (LVL_6_NOTAK_F4e5bde81_D_001A8FF0 != 0 && LVL_6_NOTAK_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_6_NOTAK_F4e5bde81_D_001A8FF4 != 0 || LVL_6_NOTAK_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_6_NOTAK_F4e5bde81_D_00189E20.field2294 == 110 && LVL_6_NOTAK_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_6_NOTAK_F4e5bde81_D_00189E20.field2294 == 109 || LVL_6_NOTAK_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_6_NOTAK_F4e5bde81_D_00189E20.field1497 != 0 && LVL_6_NOTAK_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_6_NOTAK_F4e5bde81_D_00189E20.field2294 == 0 && LVL_6_NOTAK_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_6_NOTAK_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
/* family 0c7f51df6235c46f - 364 bytes, 2 placements
   levels/18_damosel @0x00397AE0, levels/6_notak @0x003BCD90

   Loop over 16 KiB blocks.  Each iteration writes a four-word header into the
   resident packet cursor (small-data global at 0x1B2F88), advances the cursor
   through the helper call's delay slot, hands the payload area plus the rounded
   block and slice offsets to the eight-argument helper, then writes the far
   tag word and advances the cursor again. */

extern int *LVL_6_NOTAK_F0c7f51df_D_001B2F88 __attribute__((sda));
extern void LVL_6_NOTAK_F0c7f51df_FUN_00126288(void *p, int a1, int a2, int a3, int a4, int a5, int a6, int a7);

void LVL_6_NOTAK_FUN_003BCD90(int a0, int a1, int a2)
{
    int n = (a2 + 16383) & ~0x3FFF;
    int block = 0;
    int slice = 0;
    int t;
    char *q;
    char *p;

    while (n > 0) {
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 0) = 0x10000006;
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 4) = 0;
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 8) = 0;
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 12) = 0x50000006;
        t = a1 + block;
        block += 16384;
        n -= 16384;
        q = (char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 16;
        LVL_6_NOTAK_F0c7f51df_D_001B2F88 = (int *)q;
        LVL_6_NOTAK_F0c7f51df_FUN_00126288(q, (t << 8) >> 16, 1, 1, 0, 0, 64, 64);
        p = (char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88;
        LVL_6_NOTAK_F0c7f51df_D_001B2F88 = (int *)(p + 96);
        *(int *)(p + 96) = 0x30000300;
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 4) = a0 + slice;
        slice += 12288;
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 8) = 0;
        *(int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 12) = 0x50000300;
        LVL_6_NOTAK_F0c7f51df_D_001B2F88 = (int *)((char *)LVL_6_NOTAK_F0c7f51df_D_001B2F88 + 16);
    }
}
/* Family 07d033343bfa9687 — 148 bytes, levels/18_damosel and levels/6_notak.

   Write the four-word ring header into the resident packet cursor and advance
   the cursor itself; when the cursor is null, hand the sibling buffer 48 bytes
   further on to the stop helper instead.  Every reference re-reads the global
   because a store through it may alias the cursor itself, and the closing
   store is the only access retail makes through $gp, so the unit uses the
   qualified small-data spelling (-G0 plus an explicit `sda` attribute) — the
   same recipe as the c1edda5c and 9b94f3cd ring-header bodies. */

extern int *LVL_6_NOTAK_F07d03334_D_001B2F88 __attribute__((sda));
extern int LVL_6_NOTAK_F07d03334_D_001A742C __attribute__((sda));
extern void LVL_6_NOTAK_F07d03334_FUN_00126108(void *);

void LVL_6_NOTAK_FUN_0032F978(void)
{
    if (LVL_6_NOTAK_F07d03334_D_001B2F88 != 0) {
        *(int *)LVL_6_NOTAK_F07d03334_D_001B2F88 = 0x30000009;
        *(int *)((char *)LVL_6_NOTAK_F07d03334_D_001B2F88 + 4) = (LVL_6_NOTAK_F07d03334_D_001A742C + 48) & 0x0FFFFFFF;
        *(int *)((char *)LVL_6_NOTAK_F07d03334_D_001B2F88 + 8) = 0;
        *(int *)((char *)LVL_6_NOTAK_F07d03334_D_001B2F88 + 12) = 0x50000009;
        LVL_6_NOTAK_F07d03334_D_001B2F88 = (int *)((char *)LVL_6_NOTAK_F07d03334_D_001B2F88 + 16);
    } else {
        LVL_6_NOTAK_F07d03334_FUN_00126108((void *)(LVL_6_NOTAK_F07d03334_D_001A742C + 48));
    }
}
/* Family 0f425722ec0b6d0f — 140 bytes, levels/18_damosel and levels/6_notak.

   Same measured body as the e7046bc9 family (levels/10_hrugis_cloud and
   levels/5_feltzin_system), with this overlay pair's own addresses: walk the
   256-byte-stride table between the two resident bounds and count the records
   that qualify, either because their type byte is 254/255 and their field is
   within the resident limit, or because the sticky flag is already set.  The
   flag latches on the 0xff type byte through the compiler's conditional move.
   The outer test and the inner back-edge test both read the same resident
   bound, and the running count is a small-data word, so the unit uses the
   qualified small-data spelling (-G0 plus `sda` on the resident words). */

extern unsigned char *LVL_6_NOTAK_F0f425722_D_001B2820 __attribute__((sda));
extern unsigned char *LVL_6_NOTAK_F0f425722_D_001B2824 __attribute__((sda));
extern unsigned int LVL_6_NOTAK_F0f425722_D_001B2348 __attribute__((sda));
extern unsigned int LVL_6_NOTAK_F0f425722_D_001B2740 __attribute__((sda));

void LVL_6_NOTAK_FUN_0034FF90(void)
{
    unsigned char *p = LVL_6_NOTAK_F0f425722_D_001B2820;
    int seen = 0;
    unsigned int limit;

    LVL_6_NOTAK_F0f425722_D_001B2740 = 0;

    while (p < LVL_6_NOTAK_F0f425722_D_001B2824) {
        limit = LVL_6_NOTAK_F0f425722_D_001B2348;
        do {
            if ((p[32] >= 254 && *(unsigned int *)(p + 160) <= limit) || seen) {
                LVL_6_NOTAK_F0f425722_D_001B2740 = LVL_6_NOTAK_F0f425722_D_001B2740 + 1;
                seen = (p[32] == 0xff) ? 1 : seen;
            }
            p += 256;
        } while (p < LVL_6_NOTAK_F0f425722_D_001B2824);
    }
}
void LVL_6_NOTAK_FUN_00360DE0(int param_1, short *param_2, short param_3)
{
    int i;

    param_1 = (param_1 - *(int *)0x001B281C) << 8 >> 16;
    for (i = 1; i <= param_2[0]; i++) {
        if (param_2[i] == param_1) return;
    }
    if (param_2[0] < param_3) {
        param_2[0] = param_2[0] + 1;
        param_2[param_2[0]] = param_1;
    }
}
/* Family 023cd14a875d20e4 (120 B, 2 placements): call every registered
 * callback with its paired argument, bounded by a resident count word.
 *
 * The three resident globals are reached with `lui`+`lw` / `lui`+`addiu`
 * pairs that the ASSEMBLER macro expands from a single RTL insn; this only
 * happens when the externs carry the `sda` attribute (SYMBOL_REF_FLAG), which
 * suppresses gcc's HIGH/LO_SUM address split.  Without it the compiler keeps
 * the `%hi` in an extra callee-saved register and the body is 3 words long.
 */
typedef void (*fn_t)(int);

extern int LVL_6_NOTAK_F023cd14a_D_001B22F8 __attribute__((sda));   /* resident callback count */
extern fn_t LVL_6_NOTAK_F023cd14a_D_001B22D8[] __attribute__((sda)); /* callback table        */
extern int LVL_6_NOTAK_F023cd14a_D_001B22E8[] __attribute__((sda));  /* argument table        */

void LVL_6_NOTAK_FUN_00326670(void) {
    int i;
    for (i = 0; i < LVL_6_NOTAK_F023cd14a_D_001B22F8; i++) {
        LVL_6_NOTAK_F023cd14a_D_001B22D8[i](LVL_6_NOTAK_F023cd14a_D_001B22E8[i]);
    }
}
extern int LVL_6_NOTAK_F74e6a904_D_001B2F94 __attribute__((sda));
extern int LVL_6_NOTAK_F74e6a904_D_001B2F90 __attribute__((sda));
extern int LVL_6_NOTAK_F74e6a904_D_001A8F10 __attribute__((sda));
extern int LVL_6_NOTAK_F74e6a904_D_001B2F80[];
extern int LVL_6_NOTAK_F74e6a904_D_001B2F88 __attribute__((sda));
extern int LVL_6_NOTAK_F74e6a904_D_001B2370 __attribute__((sda));
extern int LVL_6_NOTAK_F74e6a904_D_001B2374 __attribute__((sda));

void LVL_6_NOTAK_FUN_003BC620(void)
{
    int a0;
    int a1;
    int a2;
    int v1;

    a0 = 1 - LVL_6_NOTAK_F74e6a904_D_001B2F94;
    a1 = LVL_6_NOTAK_F74e6a904_D_001B2F90;
    a2 = LVL_6_NOTAK_F74e6a904_D_001A8F10;
    LVL_6_NOTAK_F74e6a904_D_001B2F94 = a0;
    v1 = LVL_6_NOTAK_F74e6a904_D_001B2F80[a0];
    a1 = v1 + LVL_6_NOTAK_F74e6a904_D_001B2F90;
    LVL_6_NOTAK_F74e6a904_D_001B2F88 = v1;
    a1 = a1 - a2;
    LVL_6_NOTAK_F74e6a904_D_001B2370 = a1;
    LVL_6_NOTAK_F74e6a904_D_001B2374 = a1 - 8192;
}
int LVL_6_NOTAK_FUN_003239C8(int a0, int a1, int a2)
{
    unsigned char *base = *(unsigned char **)0x001B23C0;
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
extern int *LVL_6_NOTAK_F412a47f9_D_001B2F88 __attribute__((sda));
extern int *LVL_6_NOTAK_F412a47f9_D_001B2DA0 __attribute__((sda));
extern int LVL_6_NOTAK_F412a47f9_D_001A72D0 __attribute__((sda));
extern int LVL_6_NOTAK_F412a47f9_D_001A72D4 __attribute__((sda));
extern int LVL_6_NOTAK_F412a47f9_D_001B22BC __attribute__((sda));

typedef struct { int *saved; } SavedSlot412a47f9;
extern SavedSlot412a47f9 LVL_6_NOTAK_F412a47f9_D_001B2DC0 __attribute__((sda));

typedef struct {
    char gap0[12];
    short count;
    char gap14[2];
    long long *table;
} ResidentTable412a47f9;

void LVL_6_NOTAK_FUN_0039E580(void)
{
    ResidentTable412a47f9 *resident = (ResidentTable412a47f9 *)LVL_6_NOTAK_F412a47f9_D_001B2DA0;
    int *current = LVL_6_NOTAK_F412a47f9_D_001B2F88;
    int index;

    LVL_6_NOTAK_F412a47f9_D_001B2DC0.saved = current;
    LVL_6_NOTAK_F412a47f9_D_001B2F88 = (int *)((char *)current + 16);
    LVL_6_NOTAK_F412a47f9_D_001A72D0 = LVL_6_NOTAK_F412a47f9_D_001A72D4;
    LVL_6_NOTAK_F412a47f9_D_001B22BC = 0;
    for (index = 0; index < resident->count; ++index)
        *(long long *)((char *)resident->table + index * 16) = 0;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_6_NOTAK_Fabf21065e887d7a9_AT0034EE00_ROLE00;

int LVL_6_NOTAK_FUN_0034EE00(void)
{
    if ((LVL_6_NOTAK_Fabf21065e887d7a9_AT0034EE00_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_6_NOTAK_Fabf21065e887d7a9_AT0034EE00_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_6_NOTAK_Fabf21065e887d7a9_AT0034EE00_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_6_NOTAK_Fabf21065e887d7a9_AT0034EE00_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_6_NOTAK_F5b4b17178a13f443_AT00363BC8_ROLE00(void *object);

void LVL_6_NOTAK_FUN_00363BC8(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_6_NOTAK_F5b4b17178a13f443_AT00363BC8_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_6_NOTAK_Facdcf1600d770d3b_AT003A0E70_ROLE00[];

void LVL_6_NOTAK_FUN_003A0E70(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_6_NOTAK_Facdcf1600d770d3b_AT003A0E70_ROLE00;
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


extern void LVL_6_NOTAK_Fb1b523716b470b36_AT003C99B8_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_6_NOTAK_Fb1b523716b470b36_AT003C99B8_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_6_NOTAK_FUN_003C99B8(void *object)
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
    LVL_6_NOTAK_Fb1b523716b470b36_AT003C99B8_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_6_NOTAK_Fb1b523716b470b36_AT003C99B8_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_6_NOTAK_Fb1b523716b470b36_AT003CC460_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_6_NOTAK_Fb1b523716b470b36_AT003CC460_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_6_NOTAK_FUN_003CC460(void *object)
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
    LVL_6_NOTAK_Fb1b523716b470b36_AT003CC460_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_6_NOTAK_Fb1b523716b470b36_AT003CC460_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT003DB490_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_003DB490(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT003DB490_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT003DFF70_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_003DFF70(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT003DFF70_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_6_NOTAK_F86f665335d9cb905_AT00405928_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_6_NOTAK_FUN_00405928(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_6_NOTAK_F86f665335d9cb905_AT00405928_ROLE00(owner, owner->context_68);
}


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT00409AC8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_00409AC8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT00409AC8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT0040F140_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_0040F140(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT0040F140_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT0042C108_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_0042C108(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT0042C108_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT00431660_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_00431660(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT00431660_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT004454D8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_6_NOTAK_FUN_004454D8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_6_NOTAK_F9cdc323a4d0c2fbd_AT004454D8_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_6_NOTAK_F6af85cabb56d3b41_AT0047A230_ROLE00;

void LVL_6_NOTAK_FUN_0047A230(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_6_NOTAK_F6af85cabb56d3b41_AT0047A230_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_6_NOTAK_F6af85cabb56d3b41_AT0047A230_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_6_NOTAK_FUN_0047D098(float factor, void *context,
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


void LVL_6_NOTAK_FUN_0047D248(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_6_NOTAK_FUN_004825A8(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_6_NOTAK_FUN_0048A5D0(float first, float second, float **cell)
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


extern unsigned char LVL_6_NOTAK_F79744baad5ad7f65_AT00491A40_ROLE00[];

void LVL_6_NOTAK_FUN_00491A40(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_6_NOTAK_F79744baad5ad7f65_AT00491A40_ROLE00[0] == 0) {
        owner->previous_3fc = previous;
        owner->counter_3f8 = 180;
        owner->counter_3f4 = 300;
    }
    owner->value_04 = value;
}
