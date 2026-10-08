typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_11_JOBA_D_001A8EB0;
void LVL_11_JOBA_FUN_002E9738(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_11_JOBA_FUN_00306280(s32 index) {
    NativeTable20 values=LVL_11_JOBA_D_001A8EB0;
    return values.items[index];
}

u32 LVL_11_JOBA_FUN_002C6358(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_11_JOBA_D_0018C0B4;
u32 LVL_11_JOBA_FUN_002E86E0(void) {
    return LVL_11_JOBA_D_0018C0B4;
}

s32 LVL_11_JOBA_FUN_002F58D0(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_11_JOBA_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_11_JOBA_FUN_002C61F0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_11_JOBA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_11_JOBA_FUN_002C6228(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_11_JOBA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_11_JOBA_FUN_002C6E48(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_11_JOBA_FUN_002E9490(f32, f32, f32, f32, s32, s32);

void LVL_11_JOBA_FUN_002E8F38(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_11_JOBA_FUN_002E9490(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_11_JOBA_FUN_00302AD8(int width, int height, int address, int mode);
extern void LVL_11_JOBA_FUN_0038FD70(unsigned int reg, unsigned long value);
extern void LVL_11_JOBA_FUN_00302E40(int width, int height);

void LVL_11_JOBA_FUN_002F7670(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_11_JOBA_FUN_00302AD8(width, height, address, 1);
    LVL_11_JOBA_FUN_0038FD70(0x47, 0x30000UL);
    LVL_11_JOBA_FUN_0038FD70(0x42, 0x8000000044UL);
    LVL_11_JOBA_FUN_00302E40(0x100, 0x100);
    LVL_11_JOBA_FUN_0038FD70(0x42, 0x8000000044UL);
}

extern void LVL_11_JOBA_FUN_002E54D8(f32, f32, f32, f32 *, s32);

void LVL_11_JOBA_FUN_002C6080(f32 *output) {
 LVL_11_JOBA_FUN_002E54D8(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_11_JOBA_FUN_002C6260(s32 index) {
 s32 value=LVL_11_JOBA_FUN_002C6228(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_11_JOBA_FUN_002C6298(s32 index) {
 return LVL_11_JOBA_FUN_002C6228(index)==47;
}

s32 LVL_11_JOBA_FUN_002CB668(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_11_JOBA_FUN_00355E88(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_11_JOBA_D_00295330[13];
void LVL_11_JOBA_FUN_003095B0(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_11_JOBA_D_00295330[i].key == key) break;
    }
    if (i < 13) {
        LVL_11_JOBA_D_00295330[i].fields[9] = value;
        if (LVL_11_JOBA_D_00295330[i].busy == 0)
            LVL_11_JOBA_D_00295330[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_11_JOBA_D_001395B8[];
u32 LVL_11_JOBA_FUN_00317D08(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_11_JOBA_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

/* Consumes one positive counter selected by the measured class identifier. */
typedef struct {
    u8 prefix[0xe0];
    s32 fielde0, fielde4, fielde8, fieldec, fieldf0, fieldf4;
} JobaClassCounterView;

s32 LVL_11_JOBA_FUN_00441868(u8 *object, s32 class_id) {
    JobaClassCounterView *state = *(JobaClassCounterView **)(object + 0x68);
    switch (class_id) {
        case 0xd98:
            if (state->fielde0 > 0) { state->fielde0--; return 1; }
            break;
        case 0xdcf:
            if (state->fielde4 > 0) { state->fielde4--; return 1; }
            break;
        case 0xdf4:
            if (state->fielde8 > 0) { state->fielde8--; return 1; }
            break;
        case 0xdf7:
            if (state->fieldec > 0) { state->fieldec--; return 1; }
            break;
        case 0xdf9:
            if (state->fieldf0 > 0) { state->fieldf0--; return 1; }
            break;
        case 0xd37:
            if (state->fieldf4 > 0) { state->fieldf4--; return 1; }
            break;
    }
    return 0;
}

/* Increments the state counter selected by the measured class identifier. */
void LVL_11_JOBA_FUN_00441978(u8 *object, s32 class_id) {
    JobaClassCounterView *state = *(JobaClassCounterView **)(object + 0x68);
    switch (class_id) {
        case 0xd98: state->fielde0++; break;
        case 0xdcf: state->fielde4++; break;
        case 0xdf4: state->fielde8++; break;
        case 0xdf7: state->fieldec++; break;
        case 0xdf9: state->fieldf0++; break;
        case 0xd37: state->fieldf4++; break;
    }
}

/* Dispatches an indexed polygon query through the already integrated helper. */
typedef struct { s32 field00; u8 remainder[12]; } JobaPolygonHeaderView;
extern JobaPolygonHeaderView *LVL_11_JOBA_D_001FF580[];
extern s32 LVL_11_JOBA_FUN_0032E110(f32 *point, f32 *vertices, s32 count);

s32 LVL_11_JOBA_FUN_00441808(u8 *object, f32 *point) {
    u8 *state = *(u8 **)(object + 0x68);
    s32 index = *(s32 *)(state + 0x164);
    JobaPolygonHeaderView *header;
    s32 result;
    if (index == -1) {
        result = 0;
    } else {
        header = LVL_11_JOBA_D_001FF580[index];
        result = LVL_11_JOBA_FUN_0032E110(point, (f32 *)(header + 1), header->field00);
    }
    return result;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_11_JOBA_D_00232E40[];
s32 LVL_11_JOBA_FUN_00392650(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_11_JOBA_D_00232E40;
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
extern int LVL_11_JOBA_D_0022F700[];
extern ListOverrideObject *LVL_11_JOBA_D_00228200[];
extern ListOverridePair LVL_11_JOBA_D_0022F100[];
void LVL_11_JOBA_FUN_0037F518(void)
{
    int *selected = LVL_11_JOBA_D_0022F700;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_11_JOBA_D_00228200[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_11_JOBA_D_0022F100[row->key];
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
extern NativeObjectSearchRecord32 LVL_11_JOBA_D_00261410[];
s32 LVL_11_JOBA_FUN_0042E6A0(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_11_JOBA_D_00261410[index].field14 == object) {
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

s32 LVL_11_JOBA_FUN_002E85A0(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_11_JOBA_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_11_JOBA_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_11_JOBA_D_00189E20)->mode == 0x31) {
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
extern void *LVL_11_JOBA_D_0018C0B0;
extern void *LVL_11_JOBA_D_0018B134;
extern void *LVL_11_JOBA_D_0018B040;
void *LVL_11_JOBA_FUN_002BE258(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_11_JOBA_D_0018C0B0;
    if (kind == 1)
        return LVL_11_JOBA_D_0018B134;
    if (kind == 6)
        return LVL_11_JOBA_D_0018B040;
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
extern unsigned char LVL_11_JOBA_D_00139568[];
extern MappedClassEntry LVL_11_JOBA_D_00279BA0[];
unsigned int LVL_11_JOBA_FUN_00305F58(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_11_JOBA_D_00279BA0[LVL_11_JOBA_D_00139568[i]];
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
extern s32 LVL_11_JOBA_FUN_00300220(s32 value);
NativeCompactHeader *LVL_11_JOBA_FUN_003949B0(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_11_JOBA_FUN_00300220(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_11_JOBA_FUN_00300220(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_11_JOBA_D_001A79F0;
s32 LVL_11_JOBA_FUN_002E8520(void) {
    s32 found = 0;
    if (LVL_11_JOBA_D_001A79F0 == 25 || LVL_11_JOBA_D_001A79F0 == 5 ||
        LVL_11_JOBA_D_001A79F0 == 10 || LVL_11_JOBA_D_001A79F0 == 15) {
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
void LVL_11_JOBA_FUN_002E4FC8(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_11_JOBA_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_11_JOBA_FUN_002E5000(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_11_JOBA_D_00189E20;
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
extern ConditionalResetSlot LVL_11_JOBA_D_001BA700[8];
void LVL_11_JOBA_FUN_002E8968(void) {
    ConditionalResetSlot *slot = LVL_11_JOBA_D_001BA700;
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
extern StateTransitionView LVL_11_JOBA_D_001C0A00;
void LVL_11_JOBA_FUN_003058C0(void) {
    if (LVL_11_JOBA_D_001C0A00.mode == 7 && LVL_11_JOBA_D_001C0A00.state == 1) {
        LVL_11_JOBA_D_001C0A00.state = 2;
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
extern DobboFormatRoot396 LVL_11_JOBA_D_001CAEA0;
extern const char LVL_11_JOBA_D_001A9A60[];
extern const char LVL_11_JOBA_D_001A9A68[];
extern const unsigned char *LVL_11_JOBA_FUN_00306B18(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_11_JOBA_FUN_0031B388(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_11_JOBA_FUN_00306B18(LVL_11_JOBA_D_001CAEA0.rows[index].text_id);
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
        int key = LVL_11_JOBA_D_001CAEA0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_11_JOBA_D_00279BA0[LVL_11_JOBA_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_11_JOBA_D_001A9A60, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_11_JOBA_D_001A9A68);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_11_JOBA_FUN_002CBD60(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_11_JOBA_D_00189E20;
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
extern int LVL_11_JOBA_FUN_00373E20(int, unsigned int, void *);
void LVL_11_JOBA_FUN_0049B3E8(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_11_JOBA_FUN_00373E20(3, 0, 0);
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
        LVL_11_JOBA_FUN_00373E20(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_11_JOBA_FUN_00373E20(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_11_JOBA_FUN_00373E20(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_11_JOBA_D_001B3500[16];
extern u32 LVL_11_JOBA_D_001B3540[16];

int LVL_11_JOBA_FUN_00323720(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_11_JOBA_D_001B3500[index] == 0 ||
            LVL_11_JOBA_D_001B3500[index] == object) {
            LVL_11_JOBA_D_001B3500[index] = object;
            LVL_11_JOBA_D_001B3540[index] = 0;
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
extern void LVL_11_JOBA_FUN_002EE930(OozlaAppendObject164 *, int, const float *);
extern void LVL_11_JOBA_FUN_00300510(float *, const float *, const float *);
extern void LVL_11_JOBA_FUN_003009D8(float *, const float *, const float *);
extern void LVL_11_JOBA_FUN_002EEC78(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_11_JOBA_FUN_002EE888(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_11_JOBA_FUN_002EE930(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_11_JOBA_FUN_00300510(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_11_JOBA_FUN_003009D8(difference, difference, &object->transform[0][0]);
        LVL_11_JOBA_FUN_002EEC78(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_11_JOBA_FUN_004A0860(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_11_JOBA_FUN_004A0CF8(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_11_JOBA_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_11_JOBA_FUN_00341030(void) {
    if (((CallState *)LVL_11_JOBA_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_11_JOBA_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_11_JOBA_D_001A63A8)->active); ((CallState *)LVL_11_JOBA_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_11_JOBA_FUN_0032CA50(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_11_JOBA_FUN_00317C88(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_11_JOBA_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_11_JOBA_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

extern unsigned char D_19B278[];

int LVL_11_JOBA_FUN_00338DF0(void)
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

s32 LVL_11_JOBA_FUN_002C6308(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_11_JOBA_D_00189E20;

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

extern void LVL_11_JOBA_FUN_00322818(NativeUpdate775View *object);

void LVL_11_JOBA_FUN_003B3458(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_11_JOBA_FUN_00322818(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_11_JOBA_FUN_0033C2E0(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_11_JOBA_FUN_003318E0(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_11_JOBA_FUN_002C02E0(void)
{
}


void LVL_11_JOBA_FUN_002C0700(void)
{
}


void LVL_11_JOBA_FUN_002C0708(void)
{
}


unsigned int LVL_11_JOBA_FUN_002E8670(void)
{
    return 0;
}


void LVL_11_JOBA_FUN_002FF828(void)
{
}


void LVL_11_JOBA_FUN_003068B0(void)
{
}


void LVL_11_JOBA_FUN_00307660(void)
{
}


void LVL_11_JOBA_FUN_0030ED48(void)
{
}


void LVL_11_JOBA_FUN_0030ED50(void)
{
}


void LVL_11_JOBA_FUN_003129E8(void)
{
}


void LVL_11_JOBA_FUN_0031B740(void)
{
}


unsigned int LVL_11_JOBA_FUN_00383C48(void)
{
    return 0;
}


void LVL_11_JOBA_FUN_00388760(void)
{
}


void LVL_11_JOBA_FUN_0038F778(void)
{
}


void LVL_11_JOBA_FUN_00393100(void)
{
}


void LVL_11_JOBA_FUN_003967E0(void)
{
}


void LVL_11_JOBA_FUN_0042B280(void)
{
}


void LVL_11_JOBA_FUN_004741B0(void)
{
}


void LVL_11_JOBA_FUN_0048E020(void)
{
}


void LVL_11_JOBA_FUN_0048FA90(void)
{
}


void LVL_11_JOBA_FUN_0048FCE0(void)
{
}


void LVL_11_JOBA_FUN_004901D8(void)
{
}


void LVL_11_JOBA_FUN_00498A50(void)
{
}


void LVL_11_JOBA_FUN_004996A8(void)
{
}


void LVL_11_JOBA_FUN_004A54B0(void)
{
}


void LVL_11_JOBA_FUN_004A7840(void)
{
}


void LVL_11_JOBA_FUN_004A90D8(void)
{
}
void LVL_11_JOBA_FUN_00397DE0(char *p)
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
void LVL_11_JOBA_FUN_003CA1B0(char *p)
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
void LVL_11_JOBA_FUN_003D3768(char *p)
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
void LVL_11_JOBA_FUN_003DFAB8(char *p)
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
void LVL_11_JOBA_FUN_003F11D0(char *p)
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
void LVL_11_JOBA_FUN_003F4968(char *p)
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
void LVL_11_JOBA_FUN_003F9450(char *p)
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
void LVL_11_JOBA_FUN_0040B2C8(char *p)
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
void LVL_11_JOBA_FUN_004115A8(char *p)
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
void LVL_11_JOBA_FUN_0041D030(char *p)
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
void LVL_11_JOBA_FUN_0041E3D8(char *p)
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
void LVL_11_JOBA_FUN_004506C0(char *p)
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
void LVL_11_JOBA_FUN_00458D00(char *p)
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
void LVL_11_JOBA_FUN_0045FFB0(char *p)
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
void LVL_11_JOBA_FUN_00472008(char *p)
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
void LVL_11_JOBA_FUN_00478EF0(char *p)
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

extern u8 LVL_11_JOBA_F62e6ff2b_D_00189E20[];
extern u8 LVL_11_JOBA_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_11_JOBA_FUN_00373A18(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_11_JOBA_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_11_JOBA_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_11_JOBA_F62e6ff2b_D_00188660;
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
void LVL_11_JOBA_FUN_003BA3D8(char *p)
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
void LVL_11_JOBA_FUN_00415170(char *p)
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
extern Blob LVL_11_JOBA_F6894d7c1_D_001A8E60;
int LVL_11_JOBA_FUN_00305B88(int x)
{
    Blob b;
    int i;
    b = LVL_11_JOBA_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
void LVL_11_JOBA_FUN_0035AC48(void)
{
    *(short *)0x1AA882 = *(unsigned char *)0x1A7BC9 ? 3 : 0;
    *(short *)0x1AA89A = *(unsigned char *)0x1A7BCA ? 3 : 0;
    *(short *)0x1AA8B2 = *(unsigned char *)0x1A7BCB ? 3 : 0;
    *(short *)0x1AA8CA = *(unsigned char *)0x1A7BCC ? 3 : 0;
    *(short *)0x1AA8E2 = *(unsigned char *)0x1A7BCE ? 3 : 0;
}
void LVL_11_JOBA_FUN_0039CD50(char *p)
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
void LVL_11_JOBA_FUN_0039F3F0(char *p)
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
void LVL_11_JOBA_FUN_004336F0(char *p)
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
void LVL_11_JOBA_FUN_0044F1A8(char *p)
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
void LVL_11_JOBA_FUN_00452948(char *p)
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
void LVL_11_JOBA_FUN_00459EA0(char *p)
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
void LVL_11_JOBA_FUN_0047BE60(char *p)
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

extern Entry *LVL_11_JOBA_Fc68ad20a_D_0018C2B8;

int LVL_11_JOBA_FUN_002E3D70(int key, int *out)
{
    Entry *e = LVL_11_JOBA_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_11_JOBA_F55a1acb8_D_001A63A8;
extern short LVL_11_JOBA_F55a1acb8_D_001A63AC;
extern int LVL_11_JOBA_F55a1acb8_FUN_00133688(void);
extern void LVL_11_JOBA_F55a1acb8_FUN_0011AEA0(int);

void LVL_11_JOBA_FUN_00342100(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_11_JOBA_F55a1acb8_FUN_00133688()) {
        LVL_11_JOBA_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_11_JOBA_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_11_JOBA_F55a1acb8_D_001A63A8.f6;
    q = LVL_11_JOBA_F55a1acb8_D_001A63A8.f18;
    LVL_11_JOBA_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_11_JOBA_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_11_JOBA_F55a1acb8_D_001A63A8.f1C;
        LVL_11_JOBA_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_11_JOBA_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern void LVL_11_JOBA_Fc84472d1_FUN_00115E38(char *file, int line, char *message);
extern char LVL_11_JOBA_Fc84472d1_D_001ADD98[];
extern char LVL_11_JOBA_Fc84472d1_D_001ADDB8[];

void LVL_11_JOBA_FUN_0048E038(int *p, int b, int c, int d)
{
    if ((unsigned)b < 4)
        LVL_11_JOBA_Fc84472d1_FUN_00115E38(LVL_11_JOBA_Fc84472d1_D_001ADD98, 37, LVL_11_JOBA_Fc84472d1_D_001ADDB8);

    p[0] = c;
    p[1] = d;
    p[2] = b;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
typedef struct { int f00; int f04; int f08; int f0C; int f10; int f14; } Thing;
extern void LVL_11_JOBA_Feab1f2e5_FUN_00115E38(char *file, int line, char *message);
extern char LVL_11_JOBA_Feab1f2e5_D_001ADD98[];
extern char LVL_11_JOBA_Feab1f2e5_D_001ADDE0[];
int LVL_11_JOBA_FUN_0048E0C0(Thing *p)
{
    if (p->f14 != 0) {
        int v = p->f14;
        int w = *(int *)v;
        p->f14 = w;
        p->f10 = p->f10 + 1;
        return v;
    }
    {
        int a2 = p->f0C;
        int r;
        if ((unsigned)p->f04 < (unsigned)(a2 + p->f08)) {
            LVL_11_JOBA_Feab1f2e5_FUN_00115E38(LVL_11_JOBA_Feab1f2e5_D_001ADD98, 83, LVL_11_JOBA_Feab1f2e5_D_001ADDE0);
            return 0;
        }
        p->f0C = a2 + p->f08;
        r = p->f00 + a2;
        p->f10 = p->f10 + 1;
        return r;
    }
}
extern char LVL_11_JOBA_Fa2dbe766_D_00189E20[];

int LVL_11_JOBA_FUN_003D9050(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_11_JOBA_Fa2dbe766_D_00189E20;
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
void LVL_11_JOBA_FUN_004677C8(char *object)
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
extern char LVL_11_JOBA_Fea34650e_D_00189E20[];

void LVL_11_JOBA_FUN_002E4E60(void)
{
    char *b = LVL_11_JOBA_Fea34650e_D_00189E20;
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
        char *c = LVL_11_JOBA_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_11_JOBA_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_11_JOBA_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_11_JOBA_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_11_JOBA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_11_JOBA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_11_JOBA_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_11_JOBA_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_11_JOBA_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_11_JOBA_FUN_00415848(char *p)
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
void LVL_11_JOBA_FUN_0047BDF8(char *p)
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

extern struct Table1 LVL_11_JOBA_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_11_JOBA_Fee2b87d1_D_00152CD0;

s32 LVL_11_JOBA_FUN_003173C8(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_11_JOBA_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_11_JOBA_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_11_JOBA_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_11_JOBA_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* 28-entry table scan.  The table is reached through the resident pointer at
   0x1AA7F0; the caller's id is looked up from index 0, the first hit wins, and
   an index is rejected when its slot is empty while the small-data flag at
   0x1A79F0 is set.  The flag is read through $gp and the load sits in the delay
   slot of the bail-out branch, so the body needs the -G8 small-data profile.

   Measured shape note: the table pointer must be reached through the global
   itself (`LVL_11_JOBA_F490e2c59_D_001AA7F0[i]`), not through a local `int *` copy.  With a local the
   allocator keeps one register for the whole live range (26 instructions);
   with the global gcc loads it into $v1, hoists the loop-invariant value and
   emits the retail `move $a2,$v1` copy at the block boundary (28 instructions,
   byte-identical). */
extern int *LVL_11_JOBA_F490e2c59_D_001AA7F0 __attribute__((sda));
extern int LVL_11_JOBA_F490e2c59_D_001A79F0 __attribute__((sda));

int LVL_11_JOBA_FUN_00318850(int param_1)
{
    int index = -1;
    int i;

    for (i = 0; i < 28; i++)
        if (LVL_11_JOBA_F490e2c59_D_001AA7F0[i] == param_1) { index = i; break; }

    if (LVL_11_JOBA_F490e2c59_D_001AA7F0[index] == 0)
        index = LVL_11_JOBA_F490e2c59_D_001A79F0 ? -1 : index;

    return index;
}
/* Family 9b94f3cd1f5fac83 (372 B, 2 placements).
   Retail builds a 64-byte packet from the resident cursor global LVL_11_JOBA_F9b94f3cd_D_001B3D08 and
   then advances that global itself, so the closing store addresses the global
   through $gp: the qualified small-data profile is required.
   The four header words are written through the global expression, so each one
   carries its own reload of LVL_11_JOBA_F9b94f3cd_D_001B3D08 before its store. */

extern int *LVL_11_JOBA_F9b94f3cd_D_001B3D08 __attribute__((sda));
extern int LVL_11_JOBA_F9b94f3cd_D_001A7350 __attribute__((sda));
extern int LVL_11_JOBA_F9b94f3cd_D_001A7354 __attribute__((sda));

void LVL_11_JOBA_FUN_0030DDA0(int a0, int a1, int a2, int a3, long long a4, int a5, int a6)
{
    int *p;

    *(int *)LVL_11_JOBA_F9b94f3cd_D_001B3D08 = 0x10000003;
    *(int *)((char *)LVL_11_JOBA_F9b94f3cd_D_001B3D08 + 4) = 0;
    *(int *)((char *)LVL_11_JOBA_F9b94f3cd_D_001B3D08 + 8) = 0;
    *(int *)((char *)LVL_11_JOBA_F9b94f3cd_D_001B3D08 + 12) = 0x50000003;
    p = LVL_11_JOBA_F9b94f3cd_D_001B3D08;
    LVL_11_JOBA_F9b94f3cd_D_001B3D08 = p + 4;
    *(long long *)(p + 4) = 0x4400000000008001LL;
    *(long long *)(p + 6) = 17424;
    *(long long *)(p + 8) = 70;
    *(long long *)(p + 10) = a4;
    if (a6) {
        *(long long *)(p + 12) = (a0 + LVL_11_JOBA_F9b94f3cd_D_001A7350 - 8)
                               | ((long long)(a1 + LVL_11_JOBA_F9b94f3cd_D_001A7354 - 8) << 16)
                               | ((long long)a5 << 32);
        *(long long *)(p + 14) = (a2 + LVL_11_JOBA_F9b94f3cd_D_001A7350 - 8)
                               | ((long long)(a3 + LVL_11_JOBA_F9b94f3cd_D_001A7354 - 8) << 16)
                               | ((long long)a5 << 32);
    } else {
        *(long long *)(p + 12) = ((a0 << 4) + LVL_11_JOBA_F9b94f3cd_D_001A7350 - 16)
                               | ((long long)((a1 << 4) + LVL_11_JOBA_F9b94f3cd_D_001A7354 - 16) << 16)
                               | ((long long)a5 << 32);
        *(long long *)(p + 14) = ((a2 << 4) + LVL_11_JOBA_F9b94f3cd_D_001A7350 - 16)
                               | ((long long)((a3 << 4) + LVL_11_JOBA_F9b94f3cd_D_001A7354 - 16) << 16)
                               | ((long long)a5 << 32);
    }
    LVL_11_JOBA_F9b94f3cd_D_001B3D08 = (int *)((char *)LVL_11_JOBA_F9b94f3cd_D_001B3D08 + 48);
}
/* Family c1edda5cc3fa91ec — 148 bytes, levels/2_maktar_nebula and levels/11_joba.

   Write the four-word ring header and advance the resident pointer; when the
   pointer is null, hand the sibling buffer 48 bytes further on to the stop
   helper instead. Every reference reads the global again, and the last store
   addresses it through $gp (a delay-slot form the pinned -G0 profile cannot
   express). */
extern int *LVL_11_JOBA_Fc1edda5c_D_001B3D08 __attribute__((sda));
extern int LVL_11_JOBA_Fc1edda5c_D_001A742C __attribute__((sda));
extern void LVL_11_JOBA_Fc1edda5c_FUN_00126108(void *);

void LVL_11_JOBA_FUN_00302950(void)
{
    if (LVL_11_JOBA_Fc1edda5c_D_001B3D08 != 0) {
        *(int *)LVL_11_JOBA_Fc1edda5c_D_001B3D08 = 0x30000009;
        *(int *)((char *)LVL_11_JOBA_Fc1edda5c_D_001B3D08 + 4) = (LVL_11_JOBA_Fc1edda5c_D_001A742C + 48) & 0x0FFFFFFF;
        *(int *)((char *)LVL_11_JOBA_Fc1edda5c_D_001B3D08 + 8) = 0;
        *(int *)((char *)LVL_11_JOBA_Fc1edda5c_D_001B3D08 + 12) = 0x50000009;
        LVL_11_JOBA_Fc1edda5c_D_001B3D08 = (int *)((char *)LVL_11_JOBA_Fc1edda5c_D_001B3D08 + 16);
    } else {
        LVL_11_JOBA_Fc1edda5c_FUN_00126108((void *)(LVL_11_JOBA_Fc1edda5c_D_001A742C + 48));
    }
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_11_JOBA_F1157be91_D_001A63E8;
extern unsigned char LVL_11_JOBA_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_11_JOBA_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_11_JOBA_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_11_JOBA_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_11_JOBA_F1157be91_FUN_00133230(void);
extern int LVL_11_JOBA_F1157be91_FUN_00132028(void);

int LVL_11_JOBA_FUN_00341F88(int a0, int a1, int a2) {
    CdMode mode = LVL_11_JOBA_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_11_JOBA_F1157be91_D_001A7900[0];
    LVL_11_JOBA_F1157be91_D_001A7430[0] = 0;
    LVL_11_JOBA_F1157be91_D_001A7434 = 0;
    LVL_11_JOBA_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_11_JOBA_F1157be91_FUN_00133230();
    LVL_11_JOBA_F1157be91_FUN_00132028();
    return 1;
}
typedef struct { char *p[6]; } Sep6;
extern Sep6 LVL_11_JOBA_Fd1b66aa7_D_001AA550;
extern char LVL_11_JOBA_Fd1b66aa7_D_001AA568[];
extern char LVL_11_JOBA_Fd1b66aa7_D_001AA578[];
extern char LVL_11_JOBA_Fd1b66aa7_D_001AA588[];
extern int LVL_11_JOBA_Fd1b66aa7_FUN_00115DA8(char *dst, char *fmt, ...);

void LVL_11_JOBA_FUN_00342430(char *dst, int x, int idx)
{
    Sep6 t;
    t = LVL_11_JOBA_Fd1b66aa7_D_001AA550;
    if (x > 999999) {
        int m = x / 1000000;
        int w = x % 1000000;
        LVL_11_JOBA_Fd1b66aa7_FUN_00115DA8(dst, LVL_11_JOBA_Fd1b66aa7_D_001AA568, m, t.p[idx % 6], w / 1000,
                          t.p[idx % 6], w % 1000);
    } else if (x >= 1000)
        LVL_11_JOBA_Fd1b66aa7_FUN_00115DA8(dst, LVL_11_JOBA_Fd1b66aa7_D_001AA578, x / 1000,
                          t.p[idx % 6], x % 1000);
    else
        LVL_11_JOBA_Fd1b66aa7_FUN_00115DA8(dst, LVL_11_JOBA_Fd1b66aa7_D_001AA588, x);
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_11_JOBA_F4e5bde81_D_00189E20;
extern s32 LVL_11_JOBA_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_11_JOBA_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_11_JOBA_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_11_JOBA_FUN_002E51F8(void) {
    s32 result = LVL_11_JOBA_F4e5bde81_D_00189E20.field348;
    if (LVL_11_JOBA_F4e5bde81_D_001A8FF0 != 0 && LVL_11_JOBA_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_11_JOBA_F4e5bde81_D_001A8FF4 != 0 || LVL_11_JOBA_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_11_JOBA_F4e5bde81_D_00189E20.field2294 == 110 && LVL_11_JOBA_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_11_JOBA_F4e5bde81_D_00189E20.field2294 == 109 || LVL_11_JOBA_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_11_JOBA_F4e5bde81_D_00189E20.field1497 != 0 && LVL_11_JOBA_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_11_JOBA_F4e5bde81_D_00189E20.field2294 == 0 && LVL_11_JOBA_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_11_JOBA_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
extern void LVL_11_JOBA_F250a61cf_FUN_00115DA8(char *buf, char *format, ...);
extern char LVL_11_JOBA_F250a61cf_D_001AD670[];
extern char LVL_11_JOBA_F250a61cf_D_001AD680[];
extern char LVL_11_JOBA_F250a61cf_D_001AD688[];

void LVL_11_JOBA_FUN_0038A160(char *buf, int value)
{
    if (value > 999999)
        LVL_11_JOBA_F250a61cf_FUN_00115DA8(buf, LVL_11_JOBA_F250a61cf_D_001AD670, value / 1000000,
                                 value / 1000 % 1000, value % 1000);
    else if (value >= 1000)
        LVL_11_JOBA_F250a61cf_FUN_00115DA8(buf, LVL_11_JOBA_F250a61cf_D_001AD680, value / 1000, value % 1000);
    else
        LVL_11_JOBA_F250a61cf_FUN_00115DA8(buf, LVL_11_JOBA_F250a61cf_D_001AD688, value);
}
void LVL_11_JOBA_FUN_00332A98(int param_1, short *param_2, short param_3)
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

extern int LVL_11_JOBA_F489bf28e_D_001B3078 __attribute__((sda));   /* resident callback count */
extern fn_t LVL_11_JOBA_F489bf28e_D_001B3058[] __attribute__((sda)); /* callback table        */
extern int LVL_11_JOBA_F489bf28e_D_001B3068[] __attribute__((sda));  /* argument table        */

void LVL_11_JOBA_FUN_002F8DF0(void) {
    int i;
    for (i = 0; i < LVL_11_JOBA_F489bf28e_D_001B3078; i++) {
        LVL_11_JOBA_F489bf28e_D_001B3058[i](LVL_11_JOBA_F489bf28e_D_001B3068[i]);
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

extern int *volatile LVL_11_JOBA_Fd50cdefe_D_001B3D08 __attribute__((sda));
extern int *LVL_11_JOBA_Fd50cdefe_D_001B3D08_1 __attribute__((sda));

extern void LVL_11_JOBA_Fd50cdefe_FUN_00126288(int *a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7);

void LVL_11_JOBA_FUN_003901A0(int a0, int a1, int a2)
{
    int s0 = (a2 + 16383) & -16384;
    int s6 = 0;
    int s5 = 0;

    while (s0 > 0) {
        *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 0) = 0x10000006;
        *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 4) = 0;
        *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 8) = 0;
        *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 12) = 0x50000006;
        {
            int *next = (int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 16);
            LVL_11_JOBA_Fd50cdefe_D_001B3D08_1 = next;
            LVL_11_JOBA_Fd50cdefe_FUN_00126288(next, (short)((a1 + s6) >> 8), 1, 1, 0, 0, 64, 64);
        }
        s6 += 16384;
        s0 -= 16384;
        {
            int *p = LVL_11_JOBA_Fd50cdefe_D_001B3D08;
            int off = a0 + s5;
            s5 += 12288;
            LVL_11_JOBA_Fd50cdefe_D_001B3D08_1 = (int *)((char *)p + 96);
            *(int *)((char *)p + 96) = 0x30000300;
            *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 4) = off;
            *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 8) = 0;
            *(int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 12) = 0x50000300;
            LVL_11_JOBA_Fd50cdefe_D_001B3D08_1 = (int *)((char *)LVL_11_JOBA_Fd50cdefe_D_001B3D08 + 16);
        }
    }
}
extern volatile int * volatile LVL_11_JOBA_Fcfd2e0cd_D_001B3D08 __attribute__((sda));
extern volatile int * volatile LVL_11_JOBA_Fcfd2e0cd_D_001B3D08_1 __attribute__((sda));
extern char LVL_11_JOBA_Fcfd2e0cd_D_001A6D40[];
extern char LVL_11_JOBA_Fcfd2e0cd_D_001A6E90[];

void LVL_11_JOBA_FUN_003029E8(int a0)
{
    int *p = (int *)LVL_11_JOBA_Fcfd2e0cd_D_001B3D08;

    if (p == 0)
        return;
    p[0] = 0x30000015;
    if (a0 == 0)
        LVL_11_JOBA_Fcfd2e0cd_D_001B3D08[1] = (int)LVL_11_JOBA_Fcfd2e0cd_D_001A6D40;
    else
        LVL_11_JOBA_Fcfd2e0cd_D_001B3D08[1] = (int)LVL_11_JOBA_Fcfd2e0cd_D_001A6E90;
    LVL_11_JOBA_Fcfd2e0cd_D_001B3D08[2] = 0;
    LVL_11_JOBA_Fcfd2e0cd_D_001B3D08[3] = 0x50000015;
    LVL_11_JOBA_Fcfd2e0cd_D_001B3D08_1 = LVL_11_JOBA_Fcfd2e0cd_D_001B3D08 + 4;
}
int LVL_11_JOBA_FUN_00400AA0(int a0, int a1, float f12)
{
    int a2 = 0;

    if (*(int *)0x001A79F0 != 11) goto end;

    if (a0 == -1) {
        if (a1 >= 166) {
            if (a1 < 187) {
                if (f12 < 520.2f) {
                    a2 = 1;
                    goto end;
                }
            }
        }
    }

    if (a0 == 2) {
        if ((unsigned)(a1 - 1) < 6) {
            if (f12 < 537.0f) {
                a2 = 1;
                goto end;
            }
        }
        if ((unsigned)(a1 - 35) < 13) {
            if (f12 < 534.0f) {
                a2 = 1;
                goto end;
            }
        }
    }

end:
    return a2;
}
extern int LVL_11_JOBA_Fbef45185_D_001B359C __attribute__((sda));

void LVL_11_JOBA_FUN_00332B18(int param_1, unsigned short *param_2)
{
    int i;

    for (i = 1; i <= (short)param_2[0]; i++) {
        if (LVL_11_JOBA_Fbef45185_D_001B359C + ((short)param_2[i] << 8) == param_1) {
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


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_11_JOBA_Fabf21065e887d7a9_AT00321290_ROLE00;

int LVL_11_JOBA_FUN_00321290(void)
{
    if ((LVL_11_JOBA_Fabf21065e887d7a9_AT00321290_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_11_JOBA_Fabf21065e887d7a9_AT00321290_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_11_JOBA_Fabf21065e887d7a9_AT00321290_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_11_JOBA_Fabf21065e887d7a9_AT00321290_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_11_JOBA_F5b4b17178a13f443_AT003356D0_ROLE00(void *object);

void LVL_11_JOBA_FUN_003356D0(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_11_JOBA_F5b4b17178a13f443_AT003356D0_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_11_JOBA_Facdcf1600d770d3b_AT00374208_ROLE00[];

void LVL_11_JOBA_FUN_00374208(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_11_JOBA_Facdcf1600d770d3b_AT00374208_ROLE00;
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


extern void LVL_11_JOBA_Fb1b523716b470b36_AT0039BEB0_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_11_JOBA_Fb1b523716b470b36_AT0039BEB0_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_11_JOBA_FUN_0039BEB0(void *object)
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
    LVL_11_JOBA_Fb1b523716b470b36_AT0039BEB0_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_11_JOBA_Fb1b523716b470b36_AT0039BEB0_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_11_JOBA_Fb1b523716b470b36_AT0039E970_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_11_JOBA_Fb1b523716b470b36_AT0039E970_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_11_JOBA_FUN_0039E970(void *object)
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
    LVL_11_JOBA_Fb1b523716b470b36_AT0039E970_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_11_JOBA_Fb1b523716b470b36_AT0039E970_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003AD9B8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_003AD9B8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003AD9B8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003B2598_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_003B2598(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003B2598_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_11_JOBA_F86f665335d9cb905_AT003D2298_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_11_JOBA_FUN_003D2298(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_11_JOBA_F86f665335d9cb905_AT003D2298_ROLE00(owner, owner->context_68);
}


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003D68F0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_003D68F0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003D68F0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003DBF90_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_003DBF90(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT003DBF90_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT00415C70_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_00415C70(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT00415C70_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT0041AEF8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_0041AEF8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT0041AEF8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_11_JOBA_F9cdc323a4d0c2fbd_AT004575C8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_11_JOBA_FUN_004575C8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_11_JOBA_F9cdc323a4d0c2fbd_AT004575C8_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_11_JOBA_F6af85cabb56d3b41_AT0048A250_ROLE00;

void LVL_11_JOBA_FUN_0048A250(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_11_JOBA_F6af85cabb56d3b41_AT0048A250_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_11_JOBA_F6af85cabb56d3b41_AT0048A250_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_11_JOBA_FUN_0048CD90(float factor, void *context,
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


void LVL_11_JOBA_FUN_0048CF40(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_11_JOBA_FUN_004922A0(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_11_JOBA_FUN_0049A2C8(float first, float second, float **cell)
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


extern unsigned char LVL_11_JOBA_F79744baad5ad7f65_AT004A1738_ROLE00[];

void LVL_11_JOBA_FUN_004A1738(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_11_JOBA_F79744baad5ad7f65_AT004A1738_ROLE00[0] == 0) {
        owner->previous_3fc = previous;
        owner->counter_3f8 = 180;
        owner->counter_3f4 = 300;
    }
    owner->value_04 = value;
}
