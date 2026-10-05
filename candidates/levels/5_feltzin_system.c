typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_5_FELTZIN_SYSTEM_D_001A8EB0;
void LVL_5_FELTZIN_SYSTEM_FUN_002F00E0(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_5_FELTZIN_SYSTEM_FUN_0030C2F8(s32 index) {
    NativeTable20 values=LVL_5_FELTZIN_SYSTEM_D_001A8EB0;
    return values.items[index];
}

u32 LVL_5_FELTZIN_SYSTEM_FUN_002C6850(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_5_FELTZIN_SYSTEM_D_0018C0B4;
u32 LVL_5_FELTZIN_SYSTEM_FUN_002EF120(void) {
    return LVL_5_FELTZIN_SYSTEM_D_0018C0B4;
}

s32 LVL_5_FELTZIN_SYSTEM_FUN_002FC208(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_5_FELTZIN_SYSTEM_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_5_FELTZIN_SYSTEM_FUN_002C66E8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_5_FELTZIN_SYSTEM_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_5_FELTZIN_SYSTEM_FUN_002C6720(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_5_FELTZIN_SYSTEM_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_5_FELTZIN_SYSTEM_FUN_002C7340(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_5_FELTZIN_SYSTEM_FUN_002EFE38(f32, f32, f32, f32, s32, s32);

void LVL_5_FELTZIN_SYSTEM_FUN_002EF8E0(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_5_FELTZIN_SYSTEM_FUN_002EFE38(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_5_FELTZIN_SYSTEM_FUN_00308B50(int width, int height, int address, int mode);
extern void LVL_5_FELTZIN_SYSTEM_FUN_003906F8(unsigned int reg, unsigned long value);
extern void LVL_5_FELTZIN_SYSTEM_FUN_00308EB8(int width, int height);

void LVL_5_FELTZIN_SYSTEM_FUN_002FE030(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_5_FELTZIN_SYSTEM_FUN_00308B50(width, height, address, 1);
    LVL_5_FELTZIN_SYSTEM_FUN_003906F8(0x47, 0x30000UL);
    LVL_5_FELTZIN_SYSTEM_FUN_003906F8(0x42, 0x8000000044UL);
    LVL_5_FELTZIN_SYSTEM_FUN_00308EB8(0x100, 0x100);
    LVL_5_FELTZIN_SYSTEM_FUN_003906F8(0x42, 0x8000000044UL);
}

extern void LVL_5_FELTZIN_SYSTEM_FUN_002EBF60(f32, f32, f32, f32 *, s32);

void LVL_5_FELTZIN_SYSTEM_FUN_002C6578(f32 *output) {
 LVL_5_FELTZIN_SYSTEM_FUN_002EBF60(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_5_FELTZIN_SYSTEM_FUN_002C6758(s32 index) {
 s32 value=LVL_5_FELTZIN_SYSTEM_FUN_002C6720(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_5_FELTZIN_SYSTEM_FUN_002C6790(s32 index) {
 return LVL_5_FELTZIN_SYSTEM_FUN_002C6720(index)==47;
}

s32 LVL_5_FELTZIN_SYSTEM_FUN_002CBAC8(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_5_FELTZIN_SYSTEM_FUN_00353FB8(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_5_FELTZIN_SYSTEM_D_00294200[13];
void LVL_5_FELTZIN_SYSTEM_FUN_0030F418(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_5_FELTZIN_SYSTEM_D_00294200[i].key == key) break;
    }
    if (i < 13) {
        LVL_5_FELTZIN_SYSTEM_D_00294200[i].fields[9] = value;
        if (LVL_5_FELTZIN_SYSTEM_D_00294200[i].busy == 0)
            LVL_5_FELTZIN_SYSTEM_D_00294200[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_5_FELTZIN_SYSTEM_D_001395B8[];
u32 LVL_5_FELTZIN_SYSTEM_FUN_0031DD00(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_5_FELTZIN_SYSTEM_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_5_FELTZIN_SYSTEM_D_00231E40[];
s32 LVL_5_FELTZIN_SYSTEM_FUN_00392FD8(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_5_FELTZIN_SYSTEM_D_00231E40;
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
extern int LVL_5_FELTZIN_SYSTEM_D_0022E700[];
extern ListOverrideObject *LVL_5_FELTZIN_SYSTEM_D_00227200[];
extern ListOverridePair LVL_5_FELTZIN_SYSTEM_D_0022E100[];
void LVL_5_FELTZIN_SYSTEM_FUN_0037FF40(void)
{
    int *selected = LVL_5_FELTZIN_SYSTEM_D_0022E700;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_5_FELTZIN_SYSTEM_D_00227200[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_5_FELTZIN_SYSTEM_D_0022E100[row->key];
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
extern NativeObjectSearchRecord32 LVL_5_FELTZIN_SYSTEM_D_00262770[];
s32 LVL_5_FELTZIN_SYSTEM_FUN_003FFC40(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_5_FELTZIN_SYSTEM_D_00262770[index].field14 == object) {
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

s32 LVL_5_FELTZIN_SYSTEM_FUN_002EEFE0(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_5_FELTZIN_SYSTEM_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_5_FELTZIN_SYSTEM_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_5_FELTZIN_SYSTEM_D_00189E20)->mode == 0x31) {
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
extern void *LVL_5_FELTZIN_SYSTEM_D_0018C0B0;
extern void *LVL_5_FELTZIN_SYSTEM_D_0018B134;
extern void *LVL_5_FELTZIN_SYSTEM_D_0018B040;
void *LVL_5_FELTZIN_SYSTEM_FUN_002BD868(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_5_FELTZIN_SYSTEM_D_0018C0B0;
    if (kind == 1)
        return LVL_5_FELTZIN_SYSTEM_D_0018B134;
    if (kind == 6)
        return LVL_5_FELTZIN_SYSTEM_D_0018B040;
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
extern unsigned char LVL_5_FELTZIN_SYSTEM_D_00139568[];
extern MappedClassEntry LVL_5_FELTZIN_SYSTEM_D_00278A70[];
unsigned int LVL_5_FELTZIN_SYSTEM_FUN_0030BFD0(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_5_FELTZIN_SYSTEM_D_00278A70[LVL_5_FELTZIN_SYSTEM_D_00139568[i]];
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
extern s32 LVL_5_FELTZIN_SYSTEM_FUN_00306370(s32 value);
NativeCompactHeader *LVL_5_FELTZIN_SYSTEM_FUN_00395338(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_5_FELTZIN_SYSTEM_FUN_00306370(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_5_FELTZIN_SYSTEM_FUN_00306370(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_5_FELTZIN_SYSTEM_D_001A79F0;
s32 LVL_5_FELTZIN_SYSTEM_FUN_002EEF60(void) {
    s32 found = 0;
    if (LVL_5_FELTZIN_SYSTEM_D_001A79F0 == 25 || LVL_5_FELTZIN_SYSTEM_D_001A79F0 == 5 ||
        LVL_5_FELTZIN_SYSTEM_D_001A79F0 == 10 || LVL_5_FELTZIN_SYSTEM_D_001A79F0 == 15) {
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
void LVL_5_FELTZIN_SYSTEM_FUN_002EBA50(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_5_FELTZIN_SYSTEM_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_5_FELTZIN_SYSTEM_FUN_002EBA88(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_5_FELTZIN_SYSTEM_D_00189E20;
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
extern ConditionalResetSlot LVL_5_FELTZIN_SYSTEM_D_001B9500[8];
void LVL_5_FELTZIN_SYSTEM_FUN_002EF3A8(void) {
    ConditionalResetSlot *slot = LVL_5_FELTZIN_SYSTEM_D_001B9500;
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
extern StateTransitionView LVL_5_FELTZIN_SYSTEM_D_001BF800;
void LVL_5_FELTZIN_SYSTEM_FUN_0030B938(void) {
    if (LVL_5_FELTZIN_SYSTEM_D_001BF800.mode == 7 && LVL_5_FELTZIN_SYSTEM_D_001BF800.state == 1) {
        LVL_5_FELTZIN_SYSTEM_D_001BF800.state = 2;
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
extern DobboFormatRoot396 LVL_5_FELTZIN_SYSTEM_D_001C9CA0;
extern const char LVL_5_FELTZIN_SYSTEM_D_001A99E0[];
extern const char LVL_5_FELTZIN_SYSTEM_D_001A99E8[];
extern const unsigned char *LVL_5_FELTZIN_SYSTEM_FUN_0030CBA8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_5_FELTZIN_SYSTEM_FUN_00321380(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_5_FELTZIN_SYSTEM_FUN_0030CBA8(LVL_5_FELTZIN_SYSTEM_D_001C9CA0.rows[index].text_id);
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
        int key = LVL_5_FELTZIN_SYSTEM_D_001C9CA0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_5_FELTZIN_SYSTEM_D_00278A70[LVL_5_FELTZIN_SYSTEM_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_5_FELTZIN_SYSTEM_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_5_FELTZIN_SYSTEM_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_5_FELTZIN_SYSTEM_FUN_002CC218(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_5_FELTZIN_SYSTEM_D_00189E20;
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
extern int LVL_5_FELTZIN_SYSTEM_FUN_00371A60(int, unsigned int, void *);
void LVL_5_FELTZIN_SYSTEM_FUN_00455188(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_5_FELTZIN_SYSTEM_FUN_00371A60(3, 0, 0);
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
        LVL_5_FELTZIN_SYSTEM_FUN_00371A60(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_5_FELTZIN_SYSTEM_FUN_00371A60(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_5_FELTZIN_SYSTEM_FUN_00371A60(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_5_FELTZIN_SYSTEM_D_001B2480[16];
extern u32 LVL_5_FELTZIN_SYSTEM_D_001B24C0[16];

int LVL_5_FELTZIN_SYSTEM_FUN_00329728(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_5_FELTZIN_SYSTEM_D_001B2480[index] == 0 ||
            LVL_5_FELTZIN_SYSTEM_D_001B2480[index] == object) {
            LVL_5_FELTZIN_SYSTEM_D_001B2480[index] = object;
            LVL_5_FELTZIN_SYSTEM_D_001B24C0[index] = 0;
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
extern void LVL_5_FELTZIN_SYSTEM_FUN_002F5268(OozlaAppendObject164 *, int, const float *);
extern void LVL_5_FELTZIN_SYSTEM_FUN_00306660(float *, const float *, const float *);
extern void LVL_5_FELTZIN_SYSTEM_FUN_00306AD8(float *, const float *, const float *);
extern void LVL_5_FELTZIN_SYSTEM_FUN_002F55B0(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_5_FELTZIN_SYSTEM_FUN_002F51C0(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_5_FELTZIN_SYSTEM_FUN_002F5268(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_5_FELTZIN_SYSTEM_FUN_00306660(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_5_FELTZIN_SYSTEM_FUN_00306AD8(difference, difference, &object->transform[0][0]);
        LVL_5_FELTZIN_SYSTEM_FUN_002F55B0(object, descriptor->plane, difference);
    }
    return descriptor->count;
}
