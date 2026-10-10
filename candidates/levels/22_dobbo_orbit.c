typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_22_DOBBO_ORBIT_D_001A8EB0;
void LVL_22_DOBBO_ORBIT_FUN_002DDB98(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_22_DOBBO_ORBIT_FUN_002F9760(s32 index) {
    NativeTable20 values=LVL_22_DOBBO_ORBIT_D_001A8EB0;
    return values.items[index];
}

u32 LVL_22_DOBBO_ORBIT_FUN_002B2688(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_22_DOBBO_ORBIT_D_0018C0B4;
u32 LVL_22_DOBBO_ORBIT_FUN_002DCBD8(void) {
    return LVL_22_DOBBO_ORBIT_D_0018C0B4;
}

s32 LVL_22_DOBBO_ORBIT_FUN_002E9CC0(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_22_DOBBO_ORBIT_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_22_DOBBO_ORBIT_FUN_002B2520(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_22_DOBBO_ORBIT_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_22_DOBBO_ORBIT_FUN_002B2558(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_22_DOBBO_ORBIT_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_22_DOBBO_ORBIT_FUN_002B3178(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_22_DOBBO_ORBIT_FUN_002DD8F0(f32, f32, f32, f32, s32, s32);

void LVL_22_DOBBO_ORBIT_FUN_002DD398(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_22_DOBBO_ORBIT_FUN_002DD8F0(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_22_DOBBO_ORBIT_FUN_002F5FB8(int width, int height, int address, int mode);
extern void LVL_22_DOBBO_ORBIT_FUN_0037EC88(unsigned int reg, unsigned long value);
extern void LVL_22_DOBBO_ORBIT_FUN_002F6320(int width, int height);

void LVL_22_DOBBO_ORBIT_FUN_002EBA60(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_22_DOBBO_ORBIT_FUN_002F5FB8(width, height, address, 1);
    LVL_22_DOBBO_ORBIT_FUN_0037EC88(0x47, 0x30000UL);
    LVL_22_DOBBO_ORBIT_FUN_0037EC88(0x42, 0x8000000044UL);
    LVL_22_DOBBO_ORBIT_FUN_002F6320(0x100, 0x100);
    LVL_22_DOBBO_ORBIT_FUN_0037EC88(0x42, 0x8000000044UL);
}

extern void LVL_22_DOBBO_ORBIT_FUN_002D9A18(f32, f32, f32, f32 *, s32);

void LVL_22_DOBBO_ORBIT_FUN_002B23B0(f32 *output) {
 LVL_22_DOBBO_ORBIT_FUN_002D9A18(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_22_DOBBO_ORBIT_FUN_002B2590(s32 index) {
 s32 value=LVL_22_DOBBO_ORBIT_FUN_002B2558(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_22_DOBBO_ORBIT_FUN_002B25C8(s32 index) {
 return LVL_22_DOBBO_ORBIT_FUN_002B2558(index)==47;
}

s32 LVL_22_DOBBO_ORBIT_FUN_002B7BC0(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_22_DOBBO_ORBIT_FUN_00345148(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_22_DOBBO_ORBIT_D_0027FD80[13];
void LVL_22_DOBBO_ORBIT_FUN_002FC868(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_22_DOBBO_ORBIT_D_0027FD80[i].key == key) break;
    }
    if (i < 13) {
        LVL_22_DOBBO_ORBIT_D_0027FD80[i].fields[9] = value;
        if (LVL_22_DOBBO_ORBIT_D_0027FD80[i].busy == 0)
            LVL_22_DOBBO_ORBIT_D_0027FD80[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_22_DOBBO_ORBIT_D_001395B8[];
u32 LVL_22_DOBBO_ORBIT_FUN_0030AEE0(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_22_DOBBO_ORBIT_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_22_DOBBO_ORBIT_D_00232200[];
s32 LVL_22_DOBBO_ORBIT_FUN_00381568(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_22_DOBBO_ORBIT_D_00232200;
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
extern int LVL_22_DOBBO_ORBIT_D_0022EAC0[];
extern ListOverrideObject *LVL_22_DOBBO_ORBIT_D_002275C0[];
extern ListOverridePair LVL_22_DOBBO_ORBIT_D_0022E4C0[];
void LVL_22_DOBBO_ORBIT_FUN_0036E4D0(void)
{
    int *selected = LVL_22_DOBBO_ORBIT_D_0022EAC0;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_22_DOBBO_ORBIT_D_002275C0[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_22_DOBBO_ORBIT_D_0022E4C0[row->key];
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
extern NativeObjectSearchRecord32 LVL_22_DOBBO_ORBIT_D_00254530[];
s32 LVL_22_DOBBO_ORBIT_FUN_003F3CE8(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_22_DOBBO_ORBIT_D_00254530[index].field14 == object) {
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

s32 LVL_22_DOBBO_ORBIT_FUN_002DCA98(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_22_DOBBO_ORBIT_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_22_DOBBO_ORBIT_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_22_DOBBO_ORBIT_D_00189E20)->mode == 0x31) {
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
extern void *LVL_22_DOBBO_ORBIT_D_0018C0B0;
extern void *LVL_22_DOBBO_ORBIT_D_0018B134;
extern void *LVL_22_DOBBO_ORBIT_D_0018B040;
void *LVL_22_DOBBO_ORBIT_FUN_002A8E90(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_22_DOBBO_ORBIT_D_0018C0B0;
    if (kind == 1)
        return LVL_22_DOBBO_ORBIT_D_0018B134;
    if (kind == 6)
        return LVL_22_DOBBO_ORBIT_D_0018B040;
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
extern unsigned char LVL_22_DOBBO_ORBIT_D_00139568[];
extern MappedClassEntry LVL_22_DOBBO_ORBIT_D_002645F0[];
unsigned int LVL_22_DOBBO_ORBIT_FUN_002F9438(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_22_DOBBO_ORBIT_D_002645F0[LVL_22_DOBBO_ORBIT_D_00139568[i]];
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
extern s32 LVL_22_DOBBO_ORBIT_FUN_002F37C0(s32 value);
NativeCompactHeader *LVL_22_DOBBO_ORBIT_FUN_003838C8(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_22_DOBBO_ORBIT_FUN_002F37C0(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_22_DOBBO_ORBIT_FUN_002F37C0(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_22_DOBBO_ORBIT_D_001A79F0;
s32 LVL_22_DOBBO_ORBIT_FUN_002DCA18(void) {
    s32 found = 0;
    if (LVL_22_DOBBO_ORBIT_D_001A79F0 == 25 || LVL_22_DOBBO_ORBIT_D_001A79F0 == 5 ||
        LVL_22_DOBBO_ORBIT_D_001A79F0 == 10 || LVL_22_DOBBO_ORBIT_D_001A79F0 == 15) {
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
void LVL_22_DOBBO_ORBIT_FUN_002D9508(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_22_DOBBO_ORBIT_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_22_DOBBO_ORBIT_FUN_002D9540(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_22_DOBBO_ORBIT_D_00189E20;
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
extern ConditionalResetSlot LVL_22_DOBBO_ORBIT_D_001B9AC0[8];
void LVL_22_DOBBO_ORBIT_FUN_002DCE60(void) {
    ConditionalResetSlot *slot = LVL_22_DOBBO_ORBIT_D_001B9AC0;
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
extern StateTransitionView LVL_22_DOBBO_ORBIT_D_001BFDC0;
void LVL_22_DOBBO_ORBIT_FUN_002F8DA0(void) {
    if (LVL_22_DOBBO_ORBIT_D_001BFDC0.mode == 7 && LVL_22_DOBBO_ORBIT_D_001BFDC0.state == 1) {
        LVL_22_DOBBO_ORBIT_D_001BFDC0.state = 2;
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
extern DobboFormatRoot396 LVL_22_DOBBO_ORBIT_D_001CA260;
extern const char LVL_22_DOBBO_ORBIT_D_001A99E0[];
extern const char LVL_22_DOBBO_ORBIT_D_001A99E8[];
extern const unsigned char *LVL_22_DOBBO_ORBIT_FUN_002F9FF8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_22_DOBBO_ORBIT_FUN_0030E560(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_22_DOBBO_ORBIT_FUN_002F9FF8(LVL_22_DOBBO_ORBIT_D_001CA260.rows[index].text_id);
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
        int key = LVL_22_DOBBO_ORBIT_D_001CA260.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_22_DOBBO_ORBIT_D_002645F0[LVL_22_DOBBO_ORBIT_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_22_DOBBO_ORBIT_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_22_DOBBO_ORBIT_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_22_DOBBO_ORBIT_FUN_002B8350(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_22_DOBBO_ORBIT_D_00189E20;
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
extern int LVL_22_DOBBO_ORBIT_FUN_00362E98(int, unsigned int, void *);
void LVL_22_DOBBO_ORBIT_FUN_0043A490(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_22_DOBBO_ORBIT_FUN_00362E98(3, 0, 0);
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
        LVL_22_DOBBO_ORBIT_FUN_00362E98(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_22_DOBBO_ORBIT_FUN_00362E98(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_22_DOBBO_ORBIT_FUN_00362E98(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_22_DOBBO_ORBIT_D_001B2080[16];
extern u32 LVL_22_DOBBO_ORBIT_D_001B20C0[16];

int LVL_22_DOBBO_ORBIT_FUN_003168F8(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_22_DOBBO_ORBIT_D_001B2080[index] == 0 ||
            LVL_22_DOBBO_ORBIT_D_001B2080[index] == object) {
            LVL_22_DOBBO_ORBIT_D_001B2080[index] = object;
            LVL_22_DOBBO_ORBIT_D_001B20C0[index] = 0;
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
extern void LVL_22_DOBBO_ORBIT_FUN_002E2D20(OozlaAppendObject164 *, int, const float *);
extern void LVL_22_DOBBO_ORBIT_FUN_002F3AB0(float *, const float *, const float *);
extern void LVL_22_DOBBO_ORBIT_FUN_002F3F40(float *, const float *, const float *);
extern void LVL_22_DOBBO_ORBIT_FUN_002E3068(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_22_DOBBO_ORBIT_FUN_002E2C78(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_22_DOBBO_ORBIT_FUN_002E2D20(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_22_DOBBO_ORBIT_FUN_002F3AB0(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_22_DOBBO_ORBIT_FUN_002F3F40(difference, difference, &object->transform[0][0]);
        LVL_22_DOBBO_ORBIT_FUN_002E3068(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_22_DOBBO_ORBIT_FUN_0043F908(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_22_DOBBO_ORBIT_FUN_0043FDA0(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
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

s32 LVL_22_DOBBO_ORBIT_FUN_002B2638(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_22_DOBBO_ORBIT_D_00189E20;

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

s32 LVL_22_DOBBO_ORBIT_FUN_0030AE60(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_22_DOBBO_ORBIT_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_22_DOBBO_ORBIT_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_22_DOBBO_ORBIT_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_22_DOBBO_ORBIT_FUN_00330BA0(void) {
    if (((CallState *)LVL_22_DOBBO_ORBIT_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_22_DOBBO_ORBIT_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_22_DOBBO_ORBIT_D_001A63A8)->active); ((CallState *)LVL_22_DOBBO_ORBIT_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_22_DOBBO_ORBIT_FUN_0031FC28(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

extern unsigned char D_19B278[];

int LVL_22_DOBBO_ORBIT_FUN_0032B3A0(void)
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

extern void LVL_22_DOBBO_ORBIT_FUN_003159F0(NativeUpdate775View *object);

void LVL_22_DOBBO_ORBIT_FUN_003A9A48(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_22_DOBBO_ORBIT_FUN_003159F0(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_22_DOBBO_ORBIT_FUN_0032DD68(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_22_DOBBO_ORBIT_FUN_00324788(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_22_DOBBO_ORBIT_FUN_002AB7A8(void)
{
}


unsigned int LVL_22_DOBBO_ORBIT_FUN_002DCB68(void)
{
    return 0;
}


void LVL_22_DOBBO_ORBIT_FUN_002F2DC8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_002F9D90(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_002FAB40(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00302000(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00302008(void)
{
}


unsigned int LVL_22_DOBBO_ORBIT_FUN_00302380(void)
{
    return 0;
}


void LVL_22_DOBBO_ORBIT_FUN_00305BC0(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_0030E918(void)
{
}


unsigned int LVL_22_DOBBO_ORBIT_FUN_00372C00(void)
{
    return 0;
}


void LVL_22_DOBBO_ORBIT_FUN_003776C0(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_0037E6D8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00382018(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_003856F8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_003F08C8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00415A40(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_0042D0C8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_0042EB38(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_0042ED88(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_0042F280(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00437AF8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00438750(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00444558(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_004468E8(void)
{
}


void LVL_22_DOBBO_ORBIT_FUN_00448180(void)
{
}
void LVL_22_DOBBO_ORBIT_FUN_003BFFC8(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003C8120(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003D2E48(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003D9E30(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003DD3D8(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003E36B8(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003EC2D0(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003ED678(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_004127B0(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_00419C70(char *p)
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

extern u8 LVL_22_DOBBO_ORBIT_F62e6ff2b_D_00189E20[];
extern u8 LVL_22_DOBBO_ORBIT_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_22_DOBBO_ORBIT_FUN_00362A90(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_22_DOBBO_ORBIT_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_22_DOBBO_ORBIT_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_22_DOBBO_ORBIT_F62e6ff2b_D_00188660;
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

extern u8 LVL_22_DOBBO_ORBIT_F1c0a2bbf_D_001ADD18[];
extern u8 LVL_22_DOBBO_ORBIT_F1c0a2bbf_D_001ADD38[];
extern void LVL_22_DOBBO_ORBIT_F1c0a2bbf_FUN_00115E38(void *, s32, void *);

void LVL_22_DOBBO_ORBIT_FUN_0042D0E0(u32 *p, u32 a1, u32 a2, u32 a3)
{
    if (a1 < 4u)
        LVL_22_DOBBO_ORBIT_F1c0a2bbf_FUN_00115E38(LVL_22_DOBBO_ORBIT_F1c0a2bbf_D_001ADD18, 37, LVL_22_DOBBO_ORBIT_F1c0a2bbf_D_001ADD38);
    p[0] = a2;
    p[1] = a3;
    p[2] = a1;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
void LVL_22_DOBBO_ORBIT_FUN_003AEDA0(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_003E6218(char *p)
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
extern Blob LVL_22_DOBBO_ORBIT_F6894d7c1_D_001A8E60;
int LVL_22_DOBBO_ORBIT_FUN_002F9068(int x)
{
    Blob b;
    int i;
    b = LVL_22_DOBBO_ORBIT_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_22_DOBBO_ORBIT_Fafab4c55_FUN_00115DA8(char *, char *, ...);
extern char LVL_22_DOBBO_ORBIT_Fafab4c55_D_001AD5F0[];
extern char LVL_22_DOBBO_ORBIT_Fafab4c55_D_001AD600[];
extern char LVL_22_DOBBO_ORBIT_Fafab4c55_D_001AD608[];

void LVL_22_DOBBO_ORBIT_FUN_003790C0(char *dst, int value)
{
    if (value > 999999)
        LVL_22_DOBBO_ORBIT_Fafab4c55_FUN_00115DA8(dst, LVL_22_DOBBO_ORBIT_Fafab4c55_D_001AD5F0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_22_DOBBO_ORBIT_Fafab4c55_FUN_00115DA8(dst, LVL_22_DOBBO_ORBIT_Fafab4c55_D_001AD600, value / 1000, value % 1000);
    else
        LVL_22_DOBBO_ORBIT_Fafab4c55_FUN_00115DA8(dst, LVL_22_DOBBO_ORBIT_Fafab4c55_D_001AD608, value);
}
typedef struct {
    char *base;
    unsigned limit;
    unsigned size;
    unsigned cur;
    int count;
    void *free;
} Hdr;

extern int LVL_22_DOBBO_ORBIT_Fe77c6258_FUN_00115E38(char *, int, char *);
extern char LVL_22_DOBBO_ORBIT_Fe77c6258_D_001ADD18[];
extern char LVL_22_DOBBO_ORBIT_Fe77c6258_D_001ADD60[];

void *LVL_22_DOBBO_ORBIT_FUN_0042D168(Hdr *p)
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
        LVL_22_DOBBO_ORBIT_Fe77c6258_FUN_00115E38(LVL_22_DOBBO_ORBIT_Fe77c6258_D_001ADD18, 83, LVL_22_DOBBO_ORBIT_Fe77c6258_D_001ADD60);
        return 0;
    }
    r = p->base + cur;
    p->cur = cur + p->size;
    p->count++;
    return r;
}
void LVL_22_DOBBO_ORBIT_FUN_0038A848(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_0038CED0(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_0041C360(char *p)
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

extern Entry *LVL_22_DOBBO_ORBIT_Fc68ad20a_D_0018C2B8;

int LVL_22_DOBBO_ORBIT_FUN_002D82B0(int key, int *out)
{
    Entry *e = LVL_22_DOBBO_ORBIT_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8;
extern short LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63AC;
extern int LVL_22_DOBBO_ORBIT_F55a1acb8_FUN_00133688(void);
extern void LVL_22_DOBBO_ORBIT_F55a1acb8_FUN_0011AEA0(int);

void LVL_22_DOBBO_ORBIT_FUN_00331C00(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_22_DOBBO_ORBIT_F55a1acb8_FUN_00133688()) {
        LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_22_DOBBO_ORBIT_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f6;
    q = LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f18;
    LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f1C;
        LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_22_DOBBO_ORBIT_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_22_DOBBO_ORBIT_Fa2dbe766_D_00189E20[];

int LVL_22_DOBBO_ORBIT_FUN_003CC9E0(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_22_DOBBO_ORBIT_Fa2dbe766_D_00189E20;
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
void LVL_22_DOBBO_ORBIT_FUN_00349CC0(void)
{
    *(short *)0x001AA802 = (*(unsigned char *)0x001A7BC9) ? 3 : 0;
    *(short *)0x001AA81A = (*(unsigned char *)0x001A7BCA) ? 3 : 0;
    *(short *)0x001AA832 = (*(unsigned char *)0x001A7BCB) ? 3 : 0;
    *(short *)0x001AA84A = (*(unsigned char *)0x001A7BCC) ? 3 : 0;
    *(short *)0x001AA862 = (*(unsigned char *)0x001A7BCE) ? 3 : 0;
}
void LVL_22_DOBBO_ORBIT_FUN_00408660(char *object)
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
extern char LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20[];

void LVL_22_DOBBO_ORBIT_FUN_002D93A0(void)
{
    char *b = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
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
        char *c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_22_DOBBO_ORBIT_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_22_DOBBO_ORBIT_FUN_003E68F0(char *p)
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
void LVL_22_DOBBO_ORBIT_FUN_0041C2F8(char *p)
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
extern char LVL_22_DOBBO_ORBIT_F0be97c76_D_00189E20[];

void LVL_22_DOBBO_ORBIT_FUN_002AB728(void)
{
    char *base = LVL_22_DOBBO_ORBIT_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_22_DOBBO_ORBIT_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_22_DOBBO_ORBIT_Fee2b87d1_D_00152CD0;

s32 LVL_22_DOBBO_ORBIT_FUN_0030A5A0(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_22_DOBBO_ORBIT_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_22_DOBBO_ORBIT_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_22_DOBBO_ORBIT_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_22_DOBBO_ORBIT_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_22_DOBBO_ORBIT_F1157be91_D_001A63E8;
extern unsigned char LVL_22_DOBBO_ORBIT_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_22_DOBBO_ORBIT_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_22_DOBBO_ORBIT_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_22_DOBBO_ORBIT_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_22_DOBBO_ORBIT_F1157be91_FUN_00133230(void);
extern int LVL_22_DOBBO_ORBIT_F1157be91_FUN_00132028(void);

int LVL_22_DOBBO_ORBIT_FUN_00331A88(int a0, int a1, int a2) {
    CdMode mode = LVL_22_DOBBO_ORBIT_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_22_DOBBO_ORBIT_F1157be91_D_001A7900[0];
    LVL_22_DOBBO_ORBIT_F1157be91_D_001A7430[0] = 0;
    LVL_22_DOBBO_ORBIT_F1157be91_D_001A7434 = 0;
    LVL_22_DOBBO_ORBIT_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_22_DOBBO_ORBIT_F1157be91_FUN_00133230();
    LVL_22_DOBBO_ORBIT_F1157be91_FUN_00132028();
    return 1;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20;
extern s32 LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_22_DOBBO_ORBIT_FUN_002D9738(void) {
    s32 result = LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field348;
    if (LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A8FF0 != 0 && LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A8FF4 != 0 || LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field2294 == 110 && LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field2294 == 109 || LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field1497 != 0 && LVL_22_DOBBO_ORBIT_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field2294 == 0 && LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_22_DOBBO_ORBIT_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
/* Family cbc1388ebe801e68 (116 B, 2 placements): arm one animation slot on a
 * resident object.  Two global guards, a "current slot" guard, then five
 * resident stores in the retail order.
 *
 * The indexed read `p + a1 * 4 + 1152` must be written as a named byte pointer
 * (`w = p + a1 * 4`) followed by the load at `w + 1152`: when the 1152 is folded
 * into the same PLUS chain, gcc canonicalises the commutative `addu` with the
 * scaled index as the first source operand, one byte away from retail.
 */

extern u8 LVL_22_DOBBO_ORBIT_Fcbc1388e_D_001A63A8[];

void LVL_22_DOBBO_ORBIT_FUN_003A6470(u8 *a0, int a1) {
    u8 *g = LVL_22_DOBBO_ORBIT_Fcbc1388e_D_001A63A8;
    u8 *p = *(u8 **)(a0 + 104);
    int bit;
    if (*(int *)(g + 104) != 0) return;
    if (*(int *)(g + 36) != -1) return;
    if (a1 == *(int *)(p + 1256)) return;
    bit = 1 << a1;
    { u8 *w = p + a1 * 4; *(int *)(g + 36) = (*(int *)(w + 1152) & 0x3fff) + 2000; }
    *(int *)(g + 40) = 0;
    *(int *)(p + 1256) = a1;
    *(int *)(p + 1252) = 1800;
    *(int *)(p + 1248) |= bit;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_22_DOBBO_ORBIT_Fabf21065e887d7a9_AT00314468_ROLE00;

int LVL_22_DOBBO_ORBIT_FUN_00314468(void)
{
    if ((LVL_22_DOBBO_ORBIT_Fabf21065e887d7a9_AT00314468_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_22_DOBBO_ORBIT_Fabf21065e887d7a9_AT00314468_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_22_DOBBO_ORBIT_Fabf21065e887d7a9_AT00314468_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_22_DOBBO_ORBIT_Fabf21065e887d7a9_AT00314468_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_22_DOBBO_ORBIT_F5b4b17178a13f443_AT00327E78_ROLE00(void *object);

void LVL_22_DOBBO_ORBIT_FUN_00327E78(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_22_DOBBO_ORBIT_F5b4b17178a13f443_AT00327E78_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_22_DOBBO_ORBIT_Facdcf1600d770d3b_AT003631D0_ROLE00[];

void LVL_22_DOBBO_ORBIT_FUN_003631D0(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_22_DOBBO_ORBIT_Facdcf1600d770d3b_AT003631D0_ROLE00;
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


extern void LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT003899A8_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT003899A8_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_22_DOBBO_ORBIT_FUN_003899A8(void *object)
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
    LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT003899A8_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT003899A8_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT0038C450_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT0038C450_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_22_DOBBO_ORBIT_FUN_0038C450(void *object)
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
    LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT0038C450_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_22_DOBBO_ORBIT_Fb1b523716b470b36_AT0038C450_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT0039A5A8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_0039A5A8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT0039A5A8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003A8B20_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003A8B20(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003A8B20_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_22_DOBBO_ORBIT_F86f665335d9cb905_AT003C6CB0_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_22_DOBBO_ORBIT_FUN_003C6CB0(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_22_DOBBO_ORBIT_F86f665335d9cb905_AT003C6CB0_ROLE00(owner, owner->context_68);
}


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003CA2A8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003CA2A8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003CA2A8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003CF920_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003CF920(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003CF920_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003E6D18_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003E6D18(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003E6D18_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003EA198_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003EA198(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003EA198_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003FA928_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003FA928(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_22_DOBBO_ORBIT_F9cdc323a4d0c2fbd_AT003FA928_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_22_DOBBO_ORBIT_F6af85cabb56d3b41_AT0042A430_ROLE00;

void LVL_22_DOBBO_ORBIT_FUN_0042A430(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_22_DOBBO_ORBIT_F6af85cabb56d3b41_AT0042A430_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_22_DOBBO_ORBIT_F6af85cabb56d3b41_AT0042A430_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_22_DOBBO_ORBIT_FUN_0042BE38(float factor, void *context,
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


void LVL_22_DOBBO_ORBIT_FUN_0042BFE8(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_22_DOBBO_ORBIT_FUN_00431348(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_22_DOBBO_ORBIT_FUN_00439370(float first, float second, float **cell)
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


extern unsigned char LVL_22_DOBBO_ORBIT_F79744baad5ad7f65_AT004407E0_ROLE00[];

void LVL_22_DOBBO_ORBIT_FUN_004407E0(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_22_DOBBO_ORBIT_F79744baad5ad7f65_AT004407E0_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_22_DOBBO_ORBIT_F6b0741c38bf00fee_AT002AA030_ROLE00[];
extern void LVL_22_DOBBO_ORBIT_F6b0741c38bf00fee_AT002AA030_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_22_DOBBO_ORBIT_F6b0741c38bf00fee_AT002AA030_ROLE02(short, short, short);

void LVL_22_DOBBO_ORBIT_FUN_002AA030(void) {
 LVL_22_DOBBO_ORBIT_F6b0741c38bf00fee_AT002AA030_ROLE01(LVL_22_DOBBO_ORBIT_F6b0741c38bf00fee_AT002AA030_ROLE00[1],0,1,0x32);
 LVL_22_DOBBO_ORBIT_F6b0741c38bf00fee_AT002AA030_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_22_DOBBO_ORBIT_F28c929cadd7aca24_AT002D87A8_ROLE00;

void LVL_22_DOBBO_ORBIT_FUN_002D87A8(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_22_DOBBO_ORBIT_F28c929cadd7aca24_AT002D87A8_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_22_DOBBO_ORBIT_F28c929cadd7aca24_AT002D87A8_ROLE00.selected = selected;
            LVL_22_DOBBO_ORBIT_F28c929cadd7aca24_AT002D87A8_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_22_DOBBO_ORBIT_F5fc519c90e0e763a_AT002D9AB0_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_22_DOBBO_ORBIT_FUN_002D9AB0(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_22_DOBBO_ORBIT_F5fc519c90e0e763a_AT002D9AB0_ROLE00(-value);
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


void LVL_22_DOBBO_ORBIT_FUN_003833E0(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[139];

void LVL_22_DOBBO_ORBIT_FUN_0038CDF8(void)
{
    int i;
    LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[138] = 5;
    LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[137] = 0;
    LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[i] = 0;
        LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_22_DOBBO_ORBIT_F03c444112283bc5f_AT0038CDF8_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_22_DOBBO_ORBIT_FUN_002A8D98(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_22_DOBBO_ORBIT_FUN_002DA2E8(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_22_DOBBO_ORBIT_FUN_002E2628(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_22_DOBBO_ORBIT_FUN_002E2630(void) {
    return 1;
}


extern void LVL_22_DOBBO_ORBIT_QWEN_11a4c157d102_AT002F9B10_ROLE000(unsigned char *);

void LVL_22_DOBBO_ORBIT_FUN_002F9B10(void *owner)
{
    LVL_22_DOBBO_ORBIT_QWEN_11a4c157d102_AT002F9B10_ROLE000(owner);
}



void LVL_22_DOBBO_ORBIT_QWEN_407ee6f17a73_AT002F9BB0_ROLE001(int);
void LVL_22_DOBBO_ORBIT_QWEN_407ee6f17a73_AT002F9BB0_ROLE000(void*);

void LVL_22_DOBBO_ORBIT_FUN_002F9BB0(void *param)
{
  LVL_22_DOBBO_ORBIT_QWEN_407ee6f17a73_AT002F9BB0_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_22_DOBBO_ORBIT_QWEN_407ee6f17a73_AT002F9BB0_ROLE000(param);
}


void LVL_22_DOBBO_ORBIT_QWEN_5696fcf76f0c_AT00317108_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_22_DOBBO_ORBIT_FUN_00317108(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_5696fcf76f0c_AT00317108_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


unsigned int LVL_22_DOBBO_ORBIT_FUN_0032F2B0(unsigned int param_1)
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
extern void LVL_22_DOBBO_ORBIT_QWEN_523e38f49b74_AT0034A010_ROLE000(long);
extern void LVL_22_DOBBO_ORBIT_QWEN_523e38f49b74_AT0034A010_ROLE002(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_523e38f49b74_AT0034A010_ROLE001(void);

int LVL_22_DOBBO_ORBIT_FUN_0034A010(void)
{
  LVL_22_DOBBO_ORBIT_QWEN_523e38f49b74_AT0034A010_ROLE000(0);
  LVL_22_DOBBO_ORBIT_QWEN_523e38f49b74_AT0034A010_ROLE002();
  LVL_22_DOBBO_ORBIT_QWEN_523e38f49b74_AT0034A010_ROLE001();
  return 0;
}


unsigned long long LVL_22_DOBBO_ORBIT_FUN_0034A410(void);

extern void LVL_22_DOBBO_ORBIT_QWEN_f47929f95774_AT0034A410_ROLE000(long);
extern void LVL_22_DOBBO_ORBIT_QWEN_f47929f95774_AT0034A410_ROLE002(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_f47929f95774_AT0034A410_ROLE001(void);

unsigned long long LVL_22_DOBBO_ORBIT_FUN_0034A410(void)
{
  LVL_22_DOBBO_ORBIT_QWEN_f47929f95774_AT0034A410_ROLE000(0);
  LVL_22_DOBBO_ORBIT_QWEN_f47929f95774_AT0034A410_ROLE002();
  LVL_22_DOBBO_ORBIT_QWEN_f47929f95774_AT0034A410_ROLE001();
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
extern void LVL_22_DOBBO_ORBIT_QWEN_cb305b1f1210_AT0034A6B0_ROLE000(long);
extern void LVL_22_DOBBO_ORBIT_QWEN_cb305b1f1210_AT0034A6B0_ROLE002(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_cb305b1f1210_AT0034A6B0_ROLE001(void);

long long LVL_22_DOBBO_ORBIT_FUN_0034A6B0(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_cb305b1f1210_AT0034A6B0_ROLE000(0LL);
    LVL_22_DOBBO_ORBIT_QWEN_cb305b1f1210_AT0034A6B0_ROLE002();
    LVL_22_DOBBO_ORBIT_QWEN_cb305b1f1210_AT0034A6B0_ROLE001();
    return 0LL;
}


extern void LVL_22_DOBBO_ORBIT_QWEN_c23b3406f982_AT0034FAA8_ROLE000(long);
extern void LVL_22_DOBBO_ORBIT_QWEN_c23b3406f982_AT0034FAA8_ROLE002(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_c23b3406f982_AT0034FAA8_ROLE001(void);

int LVL_22_DOBBO_ORBIT_FUN_0034FAA8(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_c23b3406f982_AT0034FAA8_ROLE000(0);
    LVL_22_DOBBO_ORBIT_QWEN_c23b3406f982_AT0034FAA8_ROLE002();
    LVL_22_DOBBO_ORBIT_QWEN_c23b3406f982_AT0034FAA8_ROLE001();
    return 0;
}


void LVL_22_DOBBO_ORBIT_QWEN_68cf9ebb5ee2_AT0034FC18_ROLE000(long);
void LVL_22_DOBBO_ORBIT_QWEN_68cf9ebb5ee2_AT0034FC18_ROLE001(void);
void LVL_22_DOBBO_ORBIT_QWEN_68cf9ebb5ee2_AT0034FC18_ROLE002(void);

unsigned long long LVL_22_DOBBO_ORBIT_FUN_0034FC18(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_68cf9ebb5ee2_AT0034FC18_ROLE000(0);
    LVL_22_DOBBO_ORBIT_QWEN_68cf9ebb5ee2_AT0034FC18_ROLE002();
    LVL_22_DOBBO_ORBIT_QWEN_68cf9ebb5ee2_AT0034FC18_ROLE001();
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

extern void LVL_22_DOBBO_ORBIT_QWEN_68f040eb20c9_AT0034FCD0_ROLE000(long arg0);
extern void LVL_22_DOBBO_ORBIT_QWEN_68f040eb20c9_AT0034FCD0_ROLE002(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_68f040eb20c9_AT0034FCD0_ROLE001(void);

unsigned long long LVL_22_DOBBO_ORBIT_FUN_0034FCD0(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_68f040eb20c9_AT0034FCD0_ROLE000(0);
    LVL_22_DOBBO_ORBIT_QWEN_68f040eb20c9_AT0034FCD0_ROLE002();
    LVL_22_DOBBO_ORBIT_QWEN_68f040eb20c9_AT0034FCD0_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_22_DOBBO_ORBIT_QWEN_92ea7c2f4a5c_AT00350080_ROLE000(long);
extern void LVL_22_DOBBO_ORBIT_QWEN_92ea7c2f4a5c_AT00350080_ROLE001(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_92ea7c2f4a5c_AT00350080_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_22_DOBBO_ORBIT_FUN_00350080(void)
{
  LVL_22_DOBBO_ORBIT_QWEN_92ea7c2f4a5c_AT00350080_ROLE000(0);
  LVL_22_DOBBO_ORBIT_QWEN_92ea7c2f4a5c_AT00350080_ROLE002();
  LVL_22_DOBBO_ORBIT_QWEN_92ea7c2f4a5c_AT00350080_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_22_DOBBO_ORBIT_QWEN_d01c568afacc_AT00350180_ROLE000(long param_1);
extern void LVL_22_DOBBO_ORBIT_QWEN_d01c568afacc_AT00350180_ROLE001(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_d01c568afacc_AT00350180_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_22_DOBBO_ORBIT_FUN_00350180(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_d01c568afacc_AT00350180_ROLE000(0);
    LVL_22_DOBBO_ORBIT_QWEN_d01c568afacc_AT00350180_ROLE002();
    LVL_22_DOBBO_ORBIT_QWEN_d01c568afacc_AT00350180_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_22_DOBBO_ORBIT_QWEN_093381cf82c3_AT00352330_ROLE000(long);
extern void LVL_22_DOBBO_ORBIT_QWEN_093381cf82c3_AT00352330_ROLE002(void);
extern void LVL_22_DOBBO_ORBIT_QWEN_093381cf82c3_AT00352330_ROLE001(void);

long long LVL_22_DOBBO_ORBIT_FUN_00352330(void)
{
    LVL_22_DOBBO_ORBIT_QWEN_093381cf82c3_AT00352330_ROLE000(0);
    LVL_22_DOBBO_ORBIT_QWEN_093381cf82c3_AT00352330_ROLE002();
    LVL_22_DOBBO_ORBIT_QWEN_093381cf82c3_AT00352330_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_22_DOBBO_ORBIT_FUN_0035BFF8(void) {
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



extern QwenRecovery_4ecc6b5a4034_ExtraResetRecord164 LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE000[100];
extern QwenRecovery_4ecc6b5a4034_u32 LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[139];

void LVL_22_DOBBO_ORBIT_FUN_0038A380(void)
{
    int i;
    for (i = 99; i >= 0; --i)
        LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE000[i].word = 0;
    LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[138] = 5;
    LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[137] = 0;
    LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[i] = 0;
        LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_22_DOBBO_ORBIT_QWEN_4ecc6b5a4034_AT0038A380_ROLE001[i + 128] = 0;
}



void LVL_22_DOBBO_ORBIT_FUN_0042CD58(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_22_DOBBO_ORBIT_FUN_0042CD60(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_22_DOBBO_ORBIT_FUN_0042CEB8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_22_DOBBO_ORBIT_FUN_0042D008(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_22_DOBBO_ORBIT_FUN_0042D0D8(unsigned long param_1)
{
  return param_1;
}


extern void LVL_22_DOBBO_ORBIT_QWEN_df8fc8ba49cf_AT0042FAA0_ROLE000(unsigned char *owner, int value);

void LVL_22_DOBBO_ORBIT_FUN_0042FAA0(unsigned char *owner, int value)
{
    LVL_22_DOBBO_ORBIT_QWEN_df8fc8ba49cf_AT0042FAA0_ROLE000(owner + 8, value);
}



void* LVL_22_DOBBO_ORBIT_FUN_004344A0(void* param_1);

extern void LVL_22_DOBBO_ORBIT_QWEN_f353c206e726_AT004344A0_ROLE000(int);
extern unsigned long long LVL_22_DOBBO_ORBIT_QWEN_f353c206e726_AT004344A0_ROLE001(unsigned long long);

void* LVL_22_DOBBO_ORBIT_FUN_004344A0(void* param_1)
{
  LVL_22_DOBBO_ORBIT_QWEN_f353c206e726_AT004344A0_ROLE000((int)param_1 + 8);
  LVL_22_DOBBO_ORBIT_QWEN_f353c206e726_AT004344A0_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_22_DOBBO_ORBIT_QWEN_f2f9288a4e34_AT00434640_ROLE000(int);

int LVL_22_DOBBO_ORBIT_FUN_00434640(int owner)
{
    int result;
    result = LVL_22_DOBBO_ORBIT_QWEN_f2f9288a4e34_AT00434640_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_22_DOBBO_ORBIT_QWEN_e41cd63258f5_AT00434678_ROLE000(unsigned char *);

int LVL_22_DOBBO_ORBIT_FUN_00434678(int owner)
{
    return LVL_22_DOBBO_ORBIT_QWEN_e41cd63258f5_AT00434678_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_22_DOBBO_ORBIT_QWEN_f29f80950ce7_AT00437D98_ROLE000(int);

void LVL_22_DOBBO_ORBIT_FUN_00437D98(int param_1)
{
  LVL_22_DOBBO_ORBIT_QWEN_f29f80950ce7_AT00437D98_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_22_DOBBO_ORBIT_QWEN_bf824305b9f7_AT004386B0_ROLE000(unsigned char *owner, float *records);

void LVL_22_DOBBO_ORBIT_FUN_004386B0(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_22_DOBBO_ORBIT_QWEN_bf824305b9f7_AT004386B0_ROLE000(owner + 0x188, records);
}



void LVL_22_DOBBO_ORBIT_QWEN_61faeca45963_AT00438978_ROLE000(int param_1);

void LVL_22_DOBBO_ORBIT_FUN_00438978(int param_1)
{
  LVL_22_DOBBO_ORBIT_QWEN_61faeca45963_AT00438978_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_22_DOBBO_ORBIT_FUN_00438AE0(unsigned long param_1)
{
  return param_1;
}


extern int LVL_22_DOBBO_ORBIT_QWEN_6add11b33414_AT00439890_ROLE000(unsigned char *owner);

int LVL_22_DOBBO_ORBIT_FUN_00439890(unsigned char *owner)
{
    return LVL_22_DOBBO_ORBIT_QWEN_6add11b33414_AT00439890_ROLE000(owner + 0x298);
}



extern void LVL_22_DOBBO_ORBIT_QWEN_de961518de48_AT004398B0_ROLE000(unsigned char *owner);

void LVL_22_DOBBO_ORBIT_FUN_004398B0(unsigned char *owner)
{
    LVL_22_DOBBO_ORBIT_QWEN_de961518de48_AT004398B0_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_22_DOBBO_ORBIT_QWEN_7ba3cdeeb16b_AT0043E1D0_ROLE000(unsigned long long);

unsigned long long LVL_22_DOBBO_ORBIT_FUN_0043E1D0(unsigned long long value)
{
    LVL_22_DOBBO_ORBIT_QWEN_7ba3cdeeb16b_AT0043E1D0_ROLE000(value);
    return value;
}



void LVL_22_DOBBO_ORBIT_FUN_0043E418(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_22_DOBBO_ORBIT_FUN_0043F478(unsigned long param_1)
{
  return param_1;
}


void LVL_22_DOBBO_ORBIT_FUN_0043F7B8(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_22_DOBBO_ORBIT_FUN_0043F958(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_22_DOBBO_ORBIT_FUN_0043F9A0(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_22_DOBBO_ORBIT_FUN_004448F8(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_22_DOBBO_ORBIT_FUN_004469E8(void) {
    return 1;
}


extern int LVL_22_DOBBO_ORBIT_QWEN_cc83cb329fcb_AT00448228_ROLE000(unsigned char *);

int LVL_22_DOBBO_ORBIT_FUN_00448228(int *owner)
{
    int result;
    long status;
    status = LVL_22_DOBBO_ORBIT_QWEN_cc83cb329fcb_AT00448228_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_003AE7E8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_003C0228(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_003C84A0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_003D38A0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_003E5C60(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_003EE150(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_00412B30(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(char *local, char *first, char *second);
extern void LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_22_DOBBO_ORBIT_FUN_00419C08(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3AB0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_22_DOBBO_ORBIT_Fc936841d_FUN_002F3A80((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0042C190(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(char *p, int v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(char *p, float v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(char *p, float v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(char *p, float v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(char *p, float v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(char *p, float v);
extern void LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(char *p, float v);

void LVL_22_DOBBO_ORBIT_FUN_00442EB8(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0042C190(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(p1, 1);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(p2, 1);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(p3, 1);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(p4, 1);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(p5, 1);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F960(p6, 1);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_22_DOBBO_ORBIT_F01bd4546_FUN_0043F9A8(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_22_DOBBO_ORBIT_F70997ba9_FUN_0037EC88(int a, int b);
extern void LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EC638(int a);
extern void *LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(int id);
extern int LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(void *p, int i);
extern void LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF278(void);
extern void LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF710(int a, int b, long c, void *d, int e);
extern void LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF268(void);
extern void LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EC758(void);

int LVL_22_DOBBO_ORBIT_FUN_00356588(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_0037EC88(66, 68);
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_0037EC88(71, 11);
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EC638(0);
    min = LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11613), -1);
    v = LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF2F0(LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF278();
    off = count - 6;
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF710(slot, off, 0x80FFA888L, LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11613), -1);
    off += count;
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF710(slot, off, 0x80FFA888L, LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11625), -1);
    off += count;
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF710(slot, off, 0x80FFA888L, LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11626), -1);
    off += count;
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF710(slot, off, 0x80FFA888L, LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11627), -1);
    off += count;
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF710(slot, off, 0x80FFA888L, LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002F9FF8(11599), -1);
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EF268();
    LVL_22_DOBBO_ORBIT_F70997ba9_FUN_002EC758();
    return 2;
}
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);

void LVL_22_DOBBO_ORBIT_FUN_003AE3B0(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[0], v[3]);
        v[1] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[1], v[4]);
        v[2] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[2], v[5]);
        v[9] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[9], v[12]);
        v[10] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[10], v[13]);
        v[11] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[11], v[14]);
        v[20] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(v[0]) * v[6];
        v[21] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[1]) * v[7];
        v[22] = -LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[2]) * v[8];
        v[24] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(v[9]) * v[15];
        v[25] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[10]) * v[16];
        v[26] = -LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);
extern float LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(float);

void LVL_22_DOBBO_ORBIT_FUN_003E5828(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[0], v[3]);
        v[1] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[1], v[4]);
        v[2] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[2], v[5]);
        v[9] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[9], v[12]);
        v[10] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[10], v[13]);
        v[11] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4A88(v[11], v[14]);
        v[20] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(v[0]) * v[6];
        v[21] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[1]) * v[7];
        v[22] = -LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[2]) * v[8];
        v[24] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4070(v[9]) * v[15];
        v[25] = LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[10]) * v[16];
        v[26] = -LVL_22_DOBBO_ORBIT_F307ea0ee_FUN_002F4088(v[11]) * v[17];
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


extern float LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0031FC00(float value);
extern void LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_00325BF8(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0038A040(void *owner, void *out);

void LVL_22_DOBBO_ORBIT_FUN_00389F58(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_00325BF8((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0031FC00(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0038A040(self, (char *)child + 48);

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


extern float LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0031FC00(float value);
extern void LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_00325BF8(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0038CAE0(void *owner, void *out);

void LVL_22_DOBBO_ORBIT_FUN_0038C9F8(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_00325BF8((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0031FC00(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_22_DOBBO_ORBIT_F9e2cd219_FUN_0038CAE0(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
extern void LVL_22_DOBBO_ORBIT_F6df9730c_FUN_002F38F8(char *dst, int *src, int count);

void LVL_22_DOBBO_ORBIT_FUN_0030E138(char *dst, unsigned char *src)
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
        LVL_22_DOBBO_ORBIT_F6df9730c_FUN_002F38F8(dst, tmp, 64);
        dst = next;
        LVL_22_DOBBO_ORBIT_F6df9730c_FUN_002F38F8(dst, tmp, 64);
        dst += 64;
        LVL_22_DOBBO_ORBIT_F6df9730c_FUN_002F38F8(dst, tmp, 64);
        dst += 64;
        LVL_22_DOBBO_ORBIT_F6df9730c_FUN_002F38F8(dst, tmp, 64);
        dst += 64;
    }
}
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E27F8(void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2638(void *);
extern int LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2A10(void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002F3D60(float, void *, void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002F3BB0(void *, void *, void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2F98(void *, void *, void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_0032A1C8(void *, void *, int);
extern int LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E30B8(void *, void *, void *, float, float);
extern int LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_00362D38(int, int, void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2C78(void *, int, void *);
extern int LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_00362D38(int, int, void *);
extern void LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_00363088(int, void *);
extern char LVL_22_DOBBO_ORBIT_Fdb046c5d_D_001C0100[];

int LVL_22_DOBBO_ORBIT_FUN_002E2220(char *obj)
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
        LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E27F8(obj);
    else
        LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2638(obj);

    s5 = LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2A10(obj);
    if (s5 == -1)
        return 0;

    s1 = LVL_22_DOBBO_ORBIT_Fdb046c5d_D_001C0100;
    s4 = -1;
    LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002F3D60(1.0f, s1, s1);
    LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002F3BB0(buf, s1, p);
    LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2F98(obj, p + 16, buf);
    LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_0032A1C8(obj + 16, out, 1);
    r = LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E30B8(p, s1, out, *(float *)(p + 56), *(float *)(p + 60));
    if (r == 0) {
        if (*(unsigned char *)(p + 94) != 255 && *(unsigned char *)(p + 92) == 0)
            s4 = LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_00362D38(*(unsigned char *)(p + 94), 0, obj);
        LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_002E2C78(obj, s5, out);
    } else {
        if (*(unsigned char *)(p + 94) != 255)
            s4 = LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_00362D38(*(unsigned char *)(p + 94), 0, obj);
    }
    if (s4 != -1)
        LVL_22_DOBBO_ORBIT_Fdb046c5d_FUN_00363088(s4, p + 32);
    return 0;
}
extern void LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3AF0(char *out, void *source, float value);
extern void LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_004218D0(char *buffer, int mode);
extern void LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_00421770(int value, char *buffer);
extern void LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3A80(char *first, char *second, char *third);
extern float LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3B88(void *owner, char *buffer);

extern int LVL_22_DOBBO_ORBIT_F7242f0a4_D_001B9CE0[];

void LVL_22_DOBBO_ORBIT_FUN_00421F98(void *owner, int flag, float value)
{
    char buffer[32];
    int *root = LVL_22_DOBBO_ORBIT_F7242f0a4_D_001B9CE0;

    LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3AF0(buffer, root + 8, value);
    if (flag)
        LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_004218D0(buffer + 16, 1);
    else
        LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_00421770(root[-4], buffer + 16);
    LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3A80(buffer, buffer, buffer + 16);
    LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3AF0(owner, root + 12, LVL_22_DOBBO_ORBIT_F7242f0a4_FUN_002F3B88(root + 12, buffer));
}
extern char LVL_22_DOBBO_ORBIT_F45821cfb_D_001C7A00[];
extern void LVL_22_DOBBO_ORBIT_F45821cfb_FUN_002F3A48(char *);

void LVL_22_DOBBO_ORBIT_FUN_00444F20(void)
{
    float *p = (float *)LVL_22_DOBBO_ORBIT_F45821cfb_D_001C7A00;

    p[224] = 0.4f;
    p[225] = 0.8f;
    p[226] = 1.2f;
    *(int *)&p[227] = 0;
    p[228] = 0.577f;
    p[229] = 0.577f;
    p[230] = -0.577f;
    *(int *)&p[231] = 0;
    LVL_22_DOBBO_ORBIT_F45821cfb_FUN_002F3A48((char *)&p[232]);
    LVL_22_DOBBO_ORBIT_F45821cfb_FUN_002F3A48((char *)&p[236]);
}
extern char LVL_22_DOBBO_ORBIT_Fd8e166aa_D_001BD0C0[];

void LVL_22_DOBBO_ORBIT_FUN_002E9990(void)
{
    char *g = LVL_22_DOBBO_ORBIT_Fd8e166aa_D_001BD0C0;
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
extern char LVL_22_DOBBO_ORBIT_F7cb419c1_D_001FFB00[];

extern int LVL_22_DOBBO_ORBIT_F7cb419c1_FUN_0035B408(int arg);

int LVL_22_DOBBO_ORBIT_FUN_00351C00(void)
{
    char *p = LVL_22_DOBBO_ORBIT_F7cb419c1_D_001FFB00;

    *(int *)(p + 460) = LVL_22_DOBBO_ORBIT_F7cb419c1_FUN_0035B408(*(int *)(p + 460));
    return 0;
}
extern char LVL_22_DOBBO_ORBIT_F0a76d85b_D_001B9B40[];

extern void LVL_22_DOBBO_ORBIT_F0a76d85b_FUN_002F4250(char *p);
extern void LVL_22_DOBBO_ORBIT_F0a76d85b_FUN_002EA158(void);

void LVL_22_DOBBO_ORBIT_FUN_00444FB0(void)
{
    char *p = LVL_22_DOBBO_ORBIT_F0a76d85b_D_001B9B40;

    *(int *)(p + 320) = 0;
    *(int *)(p + 324) = 0;
    *(int *)(p + 328) = 0;
    LVL_22_DOBBO_ORBIT_F0a76d85b_FUN_002F4250(p + 880);
    LVL_22_DOBBO_ORBIT_F0a76d85b_FUN_002EA158();
}
extern char LVL_22_DOBBO_ORBIT_F391de845_D_001B9C80[];
extern float LVL_22_DOBBO_ORBIT_F391de845_FUN_002F3C40(char *a, char *b);
extern float LVL_22_DOBBO_ORBIT_F391de845_FUN_00361788(void *self, float d, float x, float y);

float LVL_22_DOBBO_ORBIT_FUN_00361880(char *self, char *p)
{
    float v = LVL_22_DOBBO_ORBIT_F391de845_FUN_002F3C40(p, LVL_22_DOBBO_ORBIT_F391de845_D_001B9C80);
    float *q = *(float **)(self + 8);

    return LVL_22_DOBBO_ORBIT_F391de845_FUN_00361788(q, v, q[0], q[1]);
}
extern int LVL_22_DOBBO_ORBIT_F2f080549_D_001A8FF4 __attribute__((sda));
extern char LVL_22_DOBBO_ORBIT_F2f080549_D_001B1D90[] __attribute__((sda));
extern char LVL_22_DOBBO_ORBIT_F2f080549_D_001A9000[] __attribute__((sda));
extern float LVL_22_DOBBO_ORBIT_F2f080549_FUN_002F3C40(char *a, char *b);

float LVL_22_DOBBO_ORBIT_FUN_0032A5A0(char *p)
{
    float v;

    if (LVL_22_DOBBO_ORBIT_F2f080549_D_001A8FF4 == 0) {
        v = LVL_22_DOBBO_ORBIT_F2f080549_FUN_002F3C40(p, LVL_22_DOBBO_ORBIT_F2f080549_D_001B1D90);
    } else {
        v = 100.0f - LVL_22_DOBBO_ORBIT_F2f080549_FUN_002F3C40(p, LVL_22_DOBBO_ORBIT_F2f080549_D_001A9000);
    }
    return v;
}
extern char LVL_22_DOBBO_ORBIT_Fa76f1772_D_001C00E0[];
extern void LVL_22_DOBBO_ORBIT_Fa76f1772_FUN_002F3AB0(char *local, char *data);
extern float LVL_22_DOBBO_ORBIT_Fa76f1772_FUN_002F3B88(char *local, char *p);

int LVL_22_DOBBO_ORBIT_FUN_00425470(int a0, int a1)
{
    char local[16];
    int s0;
    int r;

    if (a1 != 1) {
        r = 0;
    } else {
        s0 = *(int *)(a0 + 112) + 352;
        LVL_22_DOBBO_ORBIT_Fa76f1772_FUN_002F3AB0(local, LVL_22_DOBBO_ORBIT_Fa76f1772_D_001C00E0);
        r = LVL_22_DOBBO_ORBIT_Fa76f1772_FUN_002F3B88(local, (char *)s0) < 0.0f ? 1 : 0;
    }
    return r;
}
extern void LVL_22_DOBBO_ORBIT_F46b43b72_FUN_00331990(int value, char *target);
extern void LVL_22_DOBBO_ORBIT_F46b43b72_FUN_00331B58(int value);

extern int LVL_22_DOBBO_ORBIT_F46b43b72_D_001BD980[];
extern int LVL_22_DOBBO_ORBIT_F46b43b72_D_0014B540[];

int LVL_22_DOBBO_ORBIT_FUN_00309C80(int index)
{
    int j = index + 1;
    int *d = LVL_22_DOBBO_ORBIT_F46b43b72_D_001BD980;
    int *b = LVL_22_DOBBO_ORBIT_F46b43b72_D_0014B540;
    int *t = (int *)((char *)b + 25416);
    int n = d[12] * 332;
    int hold = d[28];
    int j4 = j * 4;
    int cur = *(int *)((char *)t + (n + index * 4));
    int next = *(int *)((char *)t + (j4 + n));

    n = next - cur;
    if (n > 0) {
        LVL_22_DOBBO_ORBIT_F46b43b72_FUN_00331990(hold, (char *)(cur + b[6341]));
        LVL_22_DOBBO_ORBIT_F46b43b72_FUN_00331B58(0);
    }
    return 1;
}
extern void LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_0031FE30(void *object, float first, float second);
extern void LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3A80(char *first, char *second, void *third);
extern int LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002E4708(void *first, char *second, int mode, int value, int extra);
extern void LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3AB0(void *first, void *second, void *third);
extern void LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3AF0(void *first, void *second, float value);

extern short LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001B9C80[];
extern short LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001C00E0[];
extern int LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001886CC[];

void LVL_22_DOBBO_ORBIT_FUN_00361638(void *object)
{
    LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_0031FE30(object, 0.5f, 6.0f);
    LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3A80(object, object, LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001B9C80);
    if (LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002E4708(LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001B9C80, object, 130, LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001886CC[0], 0)) {
        LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3AB0(object, LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001C00E0, LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001B9C80);
        LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3AF0(object, object, 0.75f);
        LVL_22_DOBBO_ORBIT_Fa2d20de7_FUN_002F3A80(object, object, LVL_22_DOBBO_ORBIT_Fa2d20de7_D_001B9C80);
    }
}
extern char LVL_22_DOBBO_ORBIT_F15d5f4fb_D_001B9B40[];
extern char LVL_22_DOBBO_ORBIT_F15d5f4fb_D_00189E20[];
extern float LVL_22_DOBBO_ORBIT_F15d5f4fb_FUN_002F4138(float a, float b);
extern float LVL_22_DOBBO_ORBIT_F15d5f4fb_FUN_002F4B70(float a, float b);
extern float LVL_22_DOBBO_ORBIT_F15d5f4fb_FUN_002F3C40(char *a, char *b);

void LVL_22_DOBBO_ORBIT_FUN_00429810(char *o)
{
    char *B = LVL_22_DOBBO_ORBIT_F15d5f4fb_D_001B9B40;
    char *D = LVL_22_DOBBO_ORBIT_F15d5f4fb_D_00189E20;
    float *p = *(float **)(D + 8848);
    float r = LVL_22_DOBBO_ORBIT_F15d5f4fb_FUN_002F4B70(*(float *)(B + 344),
                      LVL_22_DOBBO_ORBIT_F15d5f4fb_FUN_002F4138(p[4] - *(float *)(B + 320),
                              p[5] - *(float *)(B + 324)));

    if (r < 0.5585054f) {
        r = LVL_22_DOBBO_ORBIT_F15d5f4fb_FUN_002F3C40(D + 128, B + 320);
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





extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_0032A318(char *a, char *b, char *c, f32 d);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3A80(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4848(char *a, char *b);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4628(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0(char *a, char *b, char *c);

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


void LVL_22_DOBBO_ORBIT_FUN_002E2638(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_22_DOBBO_ORBIT_F4778f810_FUN_0032A318((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3A80((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4848((char *)p + 0x10, (char *)&tmp[3]);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4628((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
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





extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_0032A318(char *a, char *b, char *c, f32 d);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3A80(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4848(char *a, char *b);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4628(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0(char *a, char *b, char *c);
extern void LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0(char *a, char *b, char *c);

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


void LVL_22_DOBBO_ORBIT_FUN_002E2718(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_22_DOBBO_ORBIT_F4778f810_FUN_0032A318((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3A80((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4848((char *)p + 0x10, (char *)&tmp[3]);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F4628((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3F40((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_22_DOBBO_ORBIT_F4778f810_FUN_002F3AB0((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}


extern void LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(int arg0, int arg1, int arg2, int arg3);
extern int LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(float value);
extern void LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(int arg0, int arg1, int arg2, int arg3);
extern int LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(float value);
extern void LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(int arg0, int arg1, int arg2, int arg3);
extern int LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(float value);
extern void LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(int arg0, int arg1, int arg2, int arg3);
extern int LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(float value);
extern void LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(int arg0, int arg1, int arg2, int arg3);
extern int LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(float value);
extern void LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(int arg0, int arg1, int arg2, int arg3);

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


extern ResidentState_Fd160fb9c LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20;

void LVL_22_DOBBO_ORBIT_FUN_002AA2C8(void)
{
    int selector;

    if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.f1B8 < 15)
            return;
    }

    selector = LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.b149D;

    if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.f1B8 == 22)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 0, 1, 30);
    }

    switch (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.p2290[67]) {
    case 3:
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(49.5f) != 0)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 0, 1, 30);
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(17.0f) != 0)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 1, 1, 30);
        return;
    case 4:
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(12.5f) != 0)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 0, 1, 30);
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(1.0f) != 0)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 1, 1, 30);
        return;
    case 20:
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(8.0f) != 0)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 0, 1, 30);
        if (LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002B8350(21.0f) != 0)
            LVL_22_DOBBO_ORBIT_Fd160fb9c_FUN_002AA070(selector, 1, 1, 30);
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


extern Persistent_F882f1178 LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20;
extern int LVL_22_DOBBO_ORBIT_F882f1178_D_001A8F00 __attribute__((sda));

extern float LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4088(float);
extern int LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4C28(int, int, float);
extern float LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(float, float);
extern float LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4088(float);
extern int LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4C28(int, int, float);
extern void LVL_22_DOBBO_ORBIT_F882f1178_FUN_0032AE28(int, float *, int, int, float);
extern void LVL_22_DOBBO_ORBIT_F882f1178_FUN_0032AE28(int, float *, int, int, float);

void LVL_22_DOBBO_ORBIT_FUN_002DAD28(void)
{
    Node_F882f1178 *node;

    node = (Node_F882f1178 *)LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.p1360;
    if (node != 0) {
        if (*(int *)0x1A8F00 == 2)
            LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f250C = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f250C, 0.020362177863717079f);
        else
            LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f250C = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f250C, 0.034906592220067978f);
        node->field120 = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4C28(0xd2d2d2, 0x285050,
                                 LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4088(LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f250C) * 0.5f + 0.5f);
    }
    node = (Node_F882f1178 *)LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.p1364;
    if (node != 0) {
        if (LVL_22_DOBBO_ORBIT_F882f1178_D_001A8F00 != 2) {
            float limit;
            int which;

            limit = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2510, 0.049451004713773727f);
            LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2510 = limit;
            which = 0x1ee628;
            if (limit > 0.0f && limit < 1.9198623895645142f) {
                which = 0x1e1ed2;
            }
            node->field120 = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4C28(node->field120, which, 0.070000000298023224f);
        } else {
            LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2510 = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4A88(LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2510, 0.026179943233728409f);
            node->field120 = LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4C28(0x1e1ed2, 0x1e1e50,
                                     LVL_22_DOBBO_ORBIT_F882f1178_FUN_002F4088(LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2510) * 0.5f + 0.5f);
        }
    }
    if (LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.p1368 != 0) {
        if (LVL_22_DOBBO_ORBIT_F882f1178_D_001A8F00 == 2)
            LVL_22_DOBBO_ORBIT_F882f1178_FUN_0032AE28((int)LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.p1368, &LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.026179943233728409f);
        else
            LVL_22_DOBBO_ORBIT_F882f1178_FUN_0032AE28((int)LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.p1368, &LVL_22_DOBBO_ORBIT_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.040724355727434158f);
    }
}
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003D9E30(char *pkt);
extern long long LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002EC970(char *p);
extern float LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(float x, float y);
extern float LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3BF8(char *p);
extern float LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(float x, float y);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4300(float *matrix, float *quat);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3D60(float *off, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003D9EB8(char *pkt, float angle);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F0D60(char *pkt, float *matrix, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003DA0D8(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003D9E30(pkt);
    r = LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002EC970(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3BF8(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4300(m, quat);
    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3D60(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003D9EB8(pkt, f12);
    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F0D60(pkt, m, 0);
}
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003DD3D8(char *pkt);
extern long long LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002EC970(char *p);
extern float LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(float x, float y);
extern float LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3BF8(char *p);
extern float LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(float x, float y);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4300(float *matrix, float *quat);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3D60(float *off, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003DD460(char *pkt, float angle);
extern void LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F0D60(char *pkt, float *matrix, int mode);

void LVL_22_DOBBO_ORBIT_FUN_003DD4C8(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003DD3D8(pkt);
    r = LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002EC970(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4138(LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3BF8(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F4300(m, quat);
    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F3D60(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_003DD460(pkt, f12);
    LVL_22_DOBBO_ORBIT_F8411efa9_FUN_002F0D60(pkt, m, 0);
}
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003D2FF0(char *p, float scale)
{
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p, p, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 16, p + 16, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 32, p + 32, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 48, p + 48, scale);
}
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003D9EB8(char *p, float scale)
{
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p, p, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 16, p + 16, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 32, p + 32, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 48, p + 48, scale);
}
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003DD460(char *p, float scale)
{
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p, p, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 16, p + 16, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 32, p + 32, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 48, p + 48, scale);
}
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003E3740(char *p, float scale)
{
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p, p, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 16, p + 16, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 32, p + 32, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 48, p + 48, scale);
}
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003EC358(char *p, float scale)
{
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p, p, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 16, p + 16, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 32, p + 32, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 48, p + 48, scale);
}
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);
extern void LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(char *dst, char *src, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003ED700(char *p, float scale)
{
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p, p, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 16, p + 16, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 32, p + 32, scale);
    LVL_22_DOBBO_ORBIT_F2c74c194_FUN_002F3AF0(p + 48, p + 48, scale);
}




extern u8 LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20[];

extern void LVL_22_DOBBO_ORBIT_F458c670e_FUN_00322E38(f32 *a, f32 *b, f32 c, f32 d, f32 e, f32 f);
extern f32 LVL_22_DOBBO_ORBIT_F458c670e_FUN_002F39F0(f32 v);
extern void LVL_22_DOBBO_ORBIT_F458c670e_FUN_00322D78(f32 *p, f32 v, f32 w);

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


void LVL_22_DOBBO_ORBIT_FUN_002CEB18(void)
{
    f32 old, v, r, sum;

    if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i1B8 >= 11
        || ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790 < 0.0f
        || ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i22A4 == 17
        || ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i22B0 == 17) {
        if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i2294 == 53) {
            LVL_22_DOBBO_ORBIT_F458c670e_FUN_00322E38(&((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790,
                    &((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->fA3C,
                    0.0f, 0.03f, 0.3f, 0.025000002f);
            ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794 = 0.0f;
        } else {
            old = ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790;
            v = ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794;
            ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794 =
                v - (old * 0.005f + v * 0.045f);
            r = LVL_22_DOBBO_ORBIT_F458c670e_FUN_002F39F0(old);
            if (r < 0.001f) {
                if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794 < 0.0001f)
                    ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794 = 0.0f;
            }
        }
    }
    sum = ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790
        + ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794;
    ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790 = sum;
    if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i2294 == 53) {
        if (sum > 0.0f) {
            if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794 > 0.0f)
                ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f794 = 0.0f;
            LVL_22_DOBBO_ORBIT_F458c670e_FUN_00322D78(&((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790,
                    0.0f, 0.011666667f);
        }
    }
    if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i2294 != 124) {
        f32 x = ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f330;
        if (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->i229C == 22)
            x = ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f9B0;
        LVL_22_DOBBO_ORBIT_F458c670e_FUN_00322E38(&((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f79C,
                &((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f7A0,
                x, 0.027f, 0.3f, 0.0f);
    }
    ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f088 =
        (((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f79C + (-0.12f))
        + ((Res_F458c670e *)LVL_22_DOBBO_ORBIT_F458c670e_D_00189E20)->f790;
}
extern void LVL_22_DOBBO_ORBIT_F40487154_FUN_002F42E0(char *a, char *b);
extern float LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(float value);
extern void LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(char *a, char *b, float value);
extern float LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(float value, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003DFF80(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F42E0(object + 192, object + 240);
    x = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(object + 192, object + 192, x);
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(object + 208, object + 208, y);
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(*(float *)(data + 12), 0.5f);
}
extern void LVL_22_DOBBO_ORBIT_F40487154_FUN_002F42E0(char *a, char *b);
extern float LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(float value);
extern void LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(char *a, char *b, float value);
extern float LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(float value, float scale);

void LVL_22_DOBBO_ORBIT_FUN_003E9BE8(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F42E0(object + 192, object + 240);
    x = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4088(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(object + 192, object + 192, x);
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(object + 208, object + 208, y);
    LVL_22_DOBBO_ORBIT_F40487154_FUN_002F3AF0(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_22_DOBBO_ORBIT_F40487154_FUN_002F4A88(*(float *)(data + 12), 0.5f);
}
extern int LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FC88(int mode);
extern float LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00321A00(char *object, char *local);
extern void LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_002F3AF0(char *local, int value, float scale);
extern float LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FD20(float low, float high);
extern void LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00339D50(char *object, char *local, float amount, float base);

void LVL_22_DOBBO_ORBIT_FUN_00389898(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FC88(5))
        return;
    base = LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00321A00(object + 16, local);
    LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_002F3AF0(local, value, 0.25f);
    LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00339D50(object + 16, local, LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FD20(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FC88(int mode);
extern float LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00321A00(char *object, char *local);
extern void LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_002F3AF0(char *local, int value, float scale);
extern float LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FD20(float low, float high);
extern void LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00339D50(char *object, char *local, float amount, float base);

void LVL_22_DOBBO_ORBIT_FUN_0038C340(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FC88(5))
        return;
    base = LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00321A00(object + 16, local);
    LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_002F3AF0(local, value, 0.25f);
    LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00339D50(object + 16, local, LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FD20(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FC88(int mode);
extern float LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00321A00(char *object, char *local);
extern void LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_002F3AF0(char *local, int value, float scale);
extern float LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FD20(float low, float high);
extern void LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00339D50(char *object, char *local, float amount, float base);

void LVL_22_DOBBO_ORBIT_FUN_00391868(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FC88(5))
        return;
    base = LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00321A00(object + 16, local);
    LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_002F3AF0(local, value, 0.25f);
    LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_00339D50(object + 16, local, LVL_22_DOBBO_ORBIT_F6eb4f363_FUN_0031FD20(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_002F3720(char *);
extern void LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_003330A8(char *);
extern int LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_002F4BE0(float);
extern void LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_002F3A80(char *, char *, char *);

void LVL_22_DOBBO_ORBIT_FUN_00338EE0(char *p)
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

    if (q[1] <= 0.0244f || LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_002F3720(p + 10) != 0) {
        LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_003330A8(p);
        return;
    }

    if (q[1] >= 0.12f) {
        q[3] = -q[3];
        q[1] = q[1] + q[3] * 0.007f;
    }
    *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    *(int *)(p + 4) = (*(int *)(p + 4) & 0xffffff) | (LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_002F4BE0(q[1] * 255.0f) << 24);
    q[2] = q[2] + 0.002f;
    if (q[2] > 1.0f)
        q[2] = q[2] - 1.0f;
    *(char *)(p + 8) = (char)(q[2] * 255.0f);
    LVL_22_DOBBO_ORBIT_Fc10c1216_FUN_002F3A80(p + 16, p + 16, (char *)(q + 4));
}
