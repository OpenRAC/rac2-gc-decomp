typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_30_INSOMNIAC_MUSEUM_D_001A8EB0;
void LVL_30_INSOMNIAC_MUSEUM_FUN_002DE790(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_30_INSOMNIAC_MUSEUM_FUN_002FAB30(s32 index) {
    NativeTable20 values=LVL_30_INSOMNIAC_MUSEUM_D_001A8EB0;
    return values.items[index];
}

u32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B4DD0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_30_INSOMNIAC_MUSEUM_D_0018C0B4;
u32 LVL_30_INSOMNIAC_MUSEUM_FUN_002DD738(void) {
    return LVL_30_INSOMNIAC_MUSEUM_D_0018C0B4;
}

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002EA920(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_30_INSOMNIAC_MUSEUM_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B4C68(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_30_INSOMNIAC_MUSEUM_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B4CA0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_30_INSOMNIAC_MUSEUM_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B58C0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002DE4E8(f32, f32, f32, f32, s32, s32);

void LVL_30_INSOMNIAC_MUSEUM_FUN_002DDF90(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_30_INSOMNIAC_MUSEUM_FUN_002DE4E8(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002F7388(int width, int height, int address, int mode);
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_0037BB30(unsigned int reg, unsigned long value);
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002F76F0(int width, int height);

void LVL_30_INSOMNIAC_MUSEUM_FUN_002EC6C0(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_30_INSOMNIAC_MUSEUM_FUN_002F7388(width, height, address, 1);
    LVL_30_INSOMNIAC_MUSEUM_FUN_0037BB30(0x47, 0x30000UL);
    LVL_30_INSOMNIAC_MUSEUM_FUN_0037BB30(0x42, 0x8000000044UL);
    LVL_30_INSOMNIAC_MUSEUM_FUN_002F76F0(0x100, 0x100);
    LVL_30_INSOMNIAC_MUSEUM_FUN_0037BB30(0x42, 0x8000000044UL);
}

extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002DA4E0(f32, f32, f32, f32 *, s32);

void LVL_30_INSOMNIAC_MUSEUM_FUN_002B4AF8(f32 *output) {
 LVL_30_INSOMNIAC_MUSEUM_FUN_002DA4E0(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B4CD8(s32 index) {
 s32 value=LVL_30_INSOMNIAC_MUSEUM_FUN_002B4CA0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B4D10(s32 index) {
 return LVL_30_INSOMNIAC_MUSEUM_FUN_002B4CA0(index)==47;
}

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002BA048(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_30_INSOMNIAC_MUSEUM_FUN_00342468(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_30_INSOMNIAC_MUSEUM_D_00284B80[13];
void LVL_30_INSOMNIAC_MUSEUM_FUN_002FDC38(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_30_INSOMNIAC_MUSEUM_D_00284B80[i].key == key) break;
    }
    if (i < 13) {
        LVL_30_INSOMNIAC_MUSEUM_D_00284B80[i].fields[9] = value;
        if (LVL_30_INSOMNIAC_MUSEUM_D_00284B80[i].busy == 0)
            LVL_30_INSOMNIAC_MUSEUM_D_00284B80[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_30_INSOMNIAC_MUSEUM_D_001395B8[];
u32 LVL_30_INSOMNIAC_MUSEUM_FUN_0030C2A8(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_30_INSOMNIAC_MUSEUM_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_30_INSOMNIAC_MUSEUM_D_00232340[];
s32 LVL_30_INSOMNIAC_MUSEUM_FUN_0037E518(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_30_INSOMNIAC_MUSEUM_D_00232340;
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
extern int LVL_30_INSOMNIAC_MUSEUM_D_0022EC00[];
extern ListOverrideObject *LVL_30_INSOMNIAC_MUSEUM_D_00227700[];
extern ListOverridePair LVL_30_INSOMNIAC_MUSEUM_D_0022E600[];
void LVL_30_INSOMNIAC_MUSEUM_FUN_0036B548(void)
{
    int *selected = LVL_30_INSOMNIAC_MUSEUM_D_0022EC00;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_30_INSOMNIAC_MUSEUM_D_00227700[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_30_INSOMNIAC_MUSEUM_D_0022E600[row->key];
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
extern NativeObjectSearchRecord32 LVL_30_INSOMNIAC_MUSEUM_D_00259C40[];
s32 LVL_30_INSOMNIAC_MUSEUM_FUN_003F1488(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_30_INSOMNIAC_MUSEUM_D_00259C40[index].field14 == object) {
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

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002DD5F8(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20)->mode == 0x31) {
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
extern void *LVL_30_INSOMNIAC_MUSEUM_D_0018C0B0;
extern void *LVL_30_INSOMNIAC_MUSEUM_D_0018B134;
extern void *LVL_30_INSOMNIAC_MUSEUM_D_0018B040;
void *LVL_30_INSOMNIAC_MUSEUM_FUN_002ABDE8(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_30_INSOMNIAC_MUSEUM_D_0018C0B0;
    if (kind == 1)
        return LVL_30_INSOMNIAC_MUSEUM_D_0018B134;
    if (kind == 6)
        return LVL_30_INSOMNIAC_MUSEUM_D_0018B040;
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
extern unsigned char LVL_30_INSOMNIAC_MUSEUM_D_00139568[];
extern MappedClassEntry LVL_30_INSOMNIAC_MUSEUM_D_002693F0[];
unsigned int LVL_30_INSOMNIAC_MUSEUM_FUN_002FA808(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_30_INSOMNIAC_MUSEUM_D_002693F0[LVL_30_INSOMNIAC_MUSEUM_D_00139568[i]];
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
extern s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002F4BA8(s32 value);
NativeCompactHeader *LVL_30_INSOMNIAC_MUSEUM_FUN_00380878(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_30_INSOMNIAC_MUSEUM_FUN_002F4BA8(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_30_INSOMNIAC_MUSEUM_FUN_002F4BA8(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_30_INSOMNIAC_MUSEUM_D_001A79F0;
s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002DD578(void) {
    s32 found = 0;
    if (LVL_30_INSOMNIAC_MUSEUM_D_001A79F0 == 25 || LVL_30_INSOMNIAC_MUSEUM_D_001A79F0 == 5 ||
        LVL_30_INSOMNIAC_MUSEUM_D_001A79F0 == 10 || LVL_30_INSOMNIAC_MUSEUM_D_001A79F0 == 15) {
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_002D9FD0(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_30_INSOMNIAC_MUSEUM_FUN_002DA008(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20;
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
extern ConditionalResetSlot LVL_30_INSOMNIAC_MUSEUM_D_001B9C00[8];
void LVL_30_INSOMNIAC_MUSEUM_FUN_002DD9C0(void) {
    ConditionalResetSlot *slot = LVL_30_INSOMNIAC_MUSEUM_D_001B9C00;
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
extern StateTransitionView LVL_30_INSOMNIAC_MUSEUM_D_001BFF00;
void LVL_30_INSOMNIAC_MUSEUM_FUN_002FA170(void) {
    if (LVL_30_INSOMNIAC_MUSEUM_D_001BFF00.mode == 7 && LVL_30_INSOMNIAC_MUSEUM_D_001BFF00.state == 1) {
        LVL_30_INSOMNIAC_MUSEUM_D_001BFF00.state = 2;
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
extern DobboFormatRoot396 LVL_30_INSOMNIAC_MUSEUM_D_001CA3A0;
extern const char LVL_30_INSOMNIAC_MUSEUM_D_001A99E0[];
extern const char LVL_30_INSOMNIAC_MUSEUM_D_001A99E8[];
extern const unsigned char *LVL_30_INSOMNIAC_MUSEUM_FUN_002FB3C8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_30_INSOMNIAC_MUSEUM_FUN_0030F928(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_30_INSOMNIAC_MUSEUM_FUN_002FB3C8(LVL_30_INSOMNIAC_MUSEUM_D_001CA3A0.rows[index].text_id);
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
        int key = LVL_30_INSOMNIAC_MUSEUM_D_001CA3A0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_30_INSOMNIAC_MUSEUM_D_002693F0[LVL_30_INSOMNIAC_MUSEUM_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_30_INSOMNIAC_MUSEUM_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_30_INSOMNIAC_MUSEUM_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_30_INSOMNIAC_MUSEUM_FUN_002BA798(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20;
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
extern int LVL_30_INSOMNIAC_MUSEUM_FUN_0035FF10(int, unsigned int, void *);
void LVL_30_INSOMNIAC_MUSEUM_FUN_0043D668(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_30_INSOMNIAC_MUSEUM_FUN_0035FF10(3, 0, 0);
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
        LVL_30_INSOMNIAC_MUSEUM_FUN_0035FF10(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_30_INSOMNIAC_MUSEUM_FUN_0035FF10(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_30_INSOMNIAC_MUSEUM_FUN_0035FF10(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_30_INSOMNIAC_MUSEUM_D_001B2C80[16];
extern u32 LVL_30_INSOMNIAC_MUSEUM_D_001B2CC0[16];

int LVL_30_INSOMNIAC_MUSEUM_FUN_00317CC0(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_30_INSOMNIAC_MUSEUM_D_001B2C80[index] == 0 ||
            LVL_30_INSOMNIAC_MUSEUM_D_001B2C80[index] == object) {
            LVL_30_INSOMNIAC_MUSEUM_D_001B2C80[index] = object;
            LVL_30_INSOMNIAC_MUSEUM_D_001B2CC0[index] = 0;
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
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002E3980(OozlaAppendObject164 *, int, const float *);
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002F4E98(float *, const float *, const float *);
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002F5310(float *, const float *, const float *);
extern void LVL_30_INSOMNIAC_MUSEUM_FUN_002E3CC8(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_30_INSOMNIAC_MUSEUM_FUN_002E38D8(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_30_INSOMNIAC_MUSEUM_FUN_002E3980(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_30_INSOMNIAC_MUSEUM_FUN_002F4E98(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_30_INSOMNIAC_MUSEUM_FUN_002F5310(difference, difference, &object->transform[0][0]);
        LVL_30_INSOMNIAC_MUSEUM_FUN_002E3CC8(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_30_INSOMNIAC_MUSEUM_FUN_00442AE0(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_30_INSOMNIAC_MUSEUM_FUN_00442F78(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
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

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_002B4D80(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_30_INSOMNIAC_MUSEUM_D_00189E20;

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

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_0030C228(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_30_INSOMNIAC_MUSEUM_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_30_INSOMNIAC_MUSEUM_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_30_INSOMNIAC_MUSEUM_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_30_INSOMNIAC_MUSEUM_FUN_0032E248(void) {
    if (((CallState *)LVL_30_INSOMNIAC_MUSEUM_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_30_INSOMNIAC_MUSEUM_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_30_INSOMNIAC_MUSEUM_D_001A63A8)->active); ((CallState *)LVL_30_INSOMNIAC_MUSEUM_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_30_INSOMNIAC_MUSEUM_FUN_00321070(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

extern unsigned char D_19B278[];

int LVL_30_INSOMNIAC_MUSEUM_FUN_0032B338(void)
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

extern void LVL_30_INSOMNIAC_MUSEUM_FUN_00316DB8(NativeUpdate775View *object);

void LVL_30_INSOMNIAC_MUSEUM_FUN_0039EE18(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_30_INSOMNIAC_MUSEUM_FUN_00316DB8(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_30_INSOMNIAC_MUSEUM_FUN_0032C0A0(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_30_INSOMNIAC_MUSEUM_FUN_00325848(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_002ADEF0(void)
{
}


unsigned int LVL_30_INSOMNIAC_MUSEUM_FUN_002DD6C8(void)
{
    return 0;
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_002F41B0(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_002FB160(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_002FBF10(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_003033D0(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_003033D8(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00306F88(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_0030FCE0(void)
{
}


unsigned int LVL_30_INSOMNIAC_MUSEUM_FUN_0036FB18(void)
{
    return 0;
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00374568(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_0037B580(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_0037EFC8(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_003826A8(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_003EE068(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_004148E8(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_004302A0(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00431D10(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00431F60(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00432458(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_0043ACD0(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_0043B928(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00447730(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_00449AC0(void)
{
}


void LVL_30_INSOMNIAC_MUSEUM_FUN_0044B358(void)
{
}
void LVL_30_INSOMNIAC_MUSEUM_FUN_003B30C8(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003C2620(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003CD278(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003D71A8(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003DAC30(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003E0F10(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003E9A70(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003EAE18(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003FC7D8(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003FD498(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_004046F8(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_0040E750(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_00412010(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_00417098(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_0041CFB8(char *p)
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

extern u8 LVL_30_INSOMNIAC_MUSEUM_F62e6ff2b_D_00189E20[];
extern u8 LVL_30_INSOMNIAC_MUSEUM_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_0035FB08(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_30_INSOMNIAC_MUSEUM_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_30_INSOMNIAC_MUSEUM_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_30_INSOMNIAC_MUSEUM_F62e6ff2b_D_00188660;
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

extern u8 LVL_30_INSOMNIAC_MUSEUM_F1c0a2bbf_D_001ADD18[];
extern u8 LVL_30_INSOMNIAC_MUSEUM_F1c0a2bbf_D_001ADD38[];
extern void LVL_30_INSOMNIAC_MUSEUM_F1c0a2bbf_FUN_00115E38(void *, s32, void *);

void LVL_30_INSOMNIAC_MUSEUM_FUN_004302B8(u32 *p, u32 a1, u32 a2, u32 a3)
{
    if (a1 < 4u)
        LVL_30_INSOMNIAC_MUSEUM_F1c0a2bbf_FUN_00115E38(LVL_30_INSOMNIAC_MUSEUM_F1c0a2bbf_D_001ADD18, 37, LVL_30_INSOMNIAC_MUSEUM_F1c0a2bbf_D_001ADD38);
    p[0] = a2;
    p[1] = a3;
    p[2] = a1;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
void LVL_30_INSOMNIAC_MUSEUM_FUN_003A32F0(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_003E3D60(char *p)
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
extern Blob LVL_30_INSOMNIAC_MUSEUM_F6894d7c1_D_001A8E60;
int LVL_30_INSOMNIAC_MUSEUM_FUN_002FA438(int x)
{
    Blob b;
    int i;
    b = LVL_30_INSOMNIAC_MUSEUM_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_FUN_00115DA8(char *, char *, ...);
extern char LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_D_001AD5F0[];
extern char LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_D_001AD600[];
extern char LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_D_001AD608[];

void LVL_30_INSOMNIAC_MUSEUM_FUN_00375F68(char *dst, int value)
{
    if (value > 999999)
        LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_FUN_00115DA8(dst, LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_D_001AD5F0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_FUN_00115DA8(dst, LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_D_001AD600, value / 1000, value % 1000);
    else
        LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_FUN_00115DA8(dst, LVL_30_INSOMNIAC_MUSEUM_Fafab4c55_D_001AD608, value);
}
typedef struct {
    char *base;
    unsigned limit;
    unsigned size;
    unsigned cur;
    int count;
    void *free;
} Hdr;

extern int LVL_30_INSOMNIAC_MUSEUM_Fe77c6258_FUN_00115E38(char *, int, char *);
extern char LVL_30_INSOMNIAC_MUSEUM_Fe77c6258_D_001ADD18[];
extern char LVL_30_INSOMNIAC_MUSEUM_Fe77c6258_D_001ADD60[];

void *LVL_30_INSOMNIAC_MUSEUM_FUN_00430340(Hdr *p)
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
        LVL_30_INSOMNIAC_MUSEUM_Fe77c6258_FUN_00115E38(LVL_30_INSOMNIAC_MUSEUM_Fe77c6258_D_001ADD18, 83, LVL_30_INSOMNIAC_MUSEUM_Fe77c6258_D_001ADD60);
        return 0;
    }
    r = p->base + cur;
    p->cur = cur + p->size;
    p->count++;
    return r;
}
void LVL_30_INSOMNIAC_MUSEUM_FUN_00389448(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_0038BAD0(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_0041FAA8(char *p)
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

extern Entry *LVL_30_INSOMNIAC_MUSEUM_Fc68ad20a_D_0018C2B8;

int LVL_30_INSOMNIAC_MUSEUM_FUN_002D8D78(int key, int *out)
{
    Entry *e = LVL_30_INSOMNIAC_MUSEUM_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8;
extern short LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63AC;
extern int LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_FUN_00133688(void);
extern void LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_FUN_0011AEA0(int);

void LVL_30_INSOMNIAC_MUSEUM_FUN_0032F318(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_FUN_00133688()) {
        LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f6;
    q = LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f18;
    LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f1C;
        LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_30_INSOMNIAC_MUSEUM_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_30_INSOMNIAC_MUSEUM_Fa2dbe766_D_00189E20[];

int LVL_30_INSOMNIAC_MUSEUM_FUN_003C7110(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_30_INSOMNIAC_MUSEUM_Fa2dbe766_D_00189E20;
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_00346D38(void)
{
    *(short *)0x001AA802 = (*(unsigned char *)0x001A7BC9) ? 3 : 0;
    *(short *)0x001AA81A = (*(unsigned char *)0x001A7BCA) ? 3 : 0;
    *(short *)0x001AA832 = (*(unsigned char *)0x001A7BCB) ? 3 : 0;
    *(short *)0x001AA84A = (*(unsigned char *)0x001A7BCC) ? 3 : 0;
    *(short *)0x001AA862 = (*(unsigned char *)0x001A7BCE) ? 3 : 0;
}
void LVL_30_INSOMNIAC_MUSEUM_FUN_00406C10(char *object)
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
extern char LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20[];

void LVL_30_INSOMNIAC_MUSEUM_FUN_002D9E68(void)
{
    char *b = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
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
        char *c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_30_INSOMNIAC_MUSEUM_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_30_INSOMNIAC_MUSEUM_FUN_003E4438(char *p)
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
void LVL_30_INSOMNIAC_MUSEUM_FUN_0041FA40(char *p)
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
extern char LVL_30_INSOMNIAC_MUSEUM_F0be97c76_D_00189E20[];

void LVL_30_INSOMNIAC_MUSEUM_FUN_002ADE70(void)
{
    char *base = LVL_30_INSOMNIAC_MUSEUM_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_30_INSOMNIAC_MUSEUM_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_30_INSOMNIAC_MUSEUM_Fee2b87d1_D_00152CD0;

s32 LVL_30_INSOMNIAC_MUSEUM_FUN_0030B968(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_30_INSOMNIAC_MUSEUM_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_30_INSOMNIAC_MUSEUM_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_30_INSOMNIAC_MUSEUM_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_30_INSOMNIAC_MUSEUM_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A63E8;
extern unsigned char LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_30_INSOMNIAC_MUSEUM_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_30_INSOMNIAC_MUSEUM_F1157be91_FUN_00133230(void);
extern int LVL_30_INSOMNIAC_MUSEUM_F1157be91_FUN_00132028(void);

int LVL_30_INSOMNIAC_MUSEUM_FUN_0032F1A0(int a0, int a1, int a2) {
    CdMode mode = LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A7900[0];
    LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A7430[0] = 0;
    LVL_30_INSOMNIAC_MUSEUM_F1157be91_D_001A7434 = 0;
    LVL_30_INSOMNIAC_MUSEUM_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_30_INSOMNIAC_MUSEUM_F1157be91_FUN_00133230();
    LVL_30_INSOMNIAC_MUSEUM_F1157be91_FUN_00132028();
    return 1;
}
