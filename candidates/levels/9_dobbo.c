typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_9_DOBBO_D_001A8EB0;
void LVL_9_DOBBO_FUN_002D39E8(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_9_DOBBO_FUN_002F0150(s32 index) {
    NativeTable20 values=LVL_9_DOBBO_D_001A8EB0;
    return values.items[index];
}

u32 LVL_9_DOBBO_FUN_002B0A20(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_9_DOBBO_D_0018C0B4;
u32 LVL_9_DOBBO_FUN_002D2990(void) {
    return LVL_9_DOBBO_D_0018C0B4;
}

s32 LVL_9_DOBBO_FUN_002DFB78(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_9_DOBBO_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_9_DOBBO_FUN_002B08B8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_9_DOBBO_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_9_DOBBO_FUN_002B08F0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_9_DOBBO_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_9_DOBBO_FUN_002B1510(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_9_DOBBO_FUN_002D3740(f32, f32, f32, f32, s32, s32);

void LVL_9_DOBBO_FUN_002D31E8(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_9_DOBBO_FUN_002D3740(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_9_DOBBO_FUN_002EC9A8(int width, int height, int address, int mode);
extern void LVL_9_DOBBO_FUN_00378928(unsigned int reg, unsigned long value);
extern void LVL_9_DOBBO_FUN_002ECD10(int width, int height);

void LVL_9_DOBBO_FUN_002E1918(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_9_DOBBO_FUN_002EC9A8(width, height, address, 1);
    LVL_9_DOBBO_FUN_00378928(0x47, 0x30000UL);
    LVL_9_DOBBO_FUN_00378928(0x42, 0x8000000044UL);
    LVL_9_DOBBO_FUN_002ECD10(0x100, 0x100);
    LVL_9_DOBBO_FUN_00378928(0x42, 0x8000000044UL);
}

s32 LVL_9_DOBBO_FUN_002B0928(s32 index) {
 s32 value=LVL_9_DOBBO_FUN_002B08F0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_9_DOBBO_FUN_002B0960(s32 index) {
 return LVL_9_DOBBO_FUN_002B08F0(index)==47;
}

s32 LVL_9_DOBBO_FUN_002B5C38(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

f32 LVL_9_DOBBO_FUN_00420460(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_9_DOBBO_FUN_0033EAE0(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_9_DOBBO_D_00280980[13];
void LVL_9_DOBBO_FUN_002F3258(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_9_DOBBO_D_00280980[i].key == key) break;
    }
    if (i < 13) {
        LVL_9_DOBBO_D_00280980[i].fields[9] = value;
        if (LVL_9_DOBBO_D_00280980[i].busy == 0)
            LVL_9_DOBBO_D_00280980[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_9_DOBBO_D_001395B8[];
u32 LVL_9_DOBBO_FUN_00301C80(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_9_DOBBO_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_9_DOBBO_D_00231F80[];
s32 LVL_9_DOBBO_FUN_0037B310(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_9_DOBBO_D_00231F80;
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
extern int LVL_9_DOBBO_D_0022E840[];
extern ListOverrideObject *LVL_9_DOBBO_D_00227340[];
extern ListOverridePair LVL_9_DOBBO_D_0022E240[];
void LVL_9_DOBBO_FUN_00367FD8(void)
{
    int *selected = LVL_9_DOBBO_D_0022E840;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_9_DOBBO_D_00227340[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_9_DOBBO_D_0022E240[row->key];
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
extern NativeObjectSearchRecord32 LVL_9_DOBBO_D_00254BF0[];
s32 LVL_9_DOBBO_FUN_003F5CF0(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_9_DOBBO_D_00254BF0[index].field14 == object) {
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

s32 LVL_9_DOBBO_FUN_002D2850(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_9_DOBBO_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_9_DOBBO_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_9_DOBBO_D_00189E20)->mode == 0x31) {
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
extern void *LVL_9_DOBBO_D_0018C0B0;
extern void *LVL_9_DOBBO_D_0018B134;
extern void *LVL_9_DOBBO_D_0018B040;
void *LVL_9_DOBBO_FUN_002A8940(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_9_DOBBO_D_0018C0B0;
    if (kind == 1)
        return LVL_9_DOBBO_D_0018B134;
    if (kind == 6)
        return LVL_9_DOBBO_D_0018B040;
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
extern unsigned char LVL_9_DOBBO_D_00139568[];
extern MappedClassEntry LVL_9_DOBBO_D_002651F0[];
unsigned int LVL_9_DOBBO_FUN_002EFE28(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_9_DOBBO_D_002651F0[LVL_9_DOBBO_D_00139568[i]];
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
extern s32 LVL_9_DOBBO_FUN_002EA1B0(s32 value);
NativeCompactHeader *LVL_9_DOBBO_FUN_0037D670(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_9_DOBBO_FUN_002EA1B0(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_9_DOBBO_FUN_002EA1B0(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_9_DOBBO_D_001A79F0;
s32 LVL_9_DOBBO_FUN_002D27D0(void) {
    s32 found = 0;
    if (LVL_9_DOBBO_D_001A79F0 == 25 || LVL_9_DOBBO_D_001A79F0 == 5 ||
        LVL_9_DOBBO_D_001A79F0 == 10 || LVL_9_DOBBO_D_001A79F0 == 15) {
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
extern ConditionalResetSlot LVL_9_DOBBO_D_001B9840[8];
void LVL_9_DOBBO_FUN_002D2C18(void) {
    ConditionalResetSlot *slot = LVL_9_DOBBO_D_001B9840;
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
extern StateTransitionView LVL_9_DOBBO_D_001BFB40;
void LVL_9_DOBBO_FUN_002EF790(void) {
    if (LVL_9_DOBBO_D_001BFB40.mode == 7 && LVL_9_DOBBO_D_001BFB40.state == 1) {
        LVL_9_DOBBO_D_001BFB40.state = 2;
    }
}

/* Update two linked parts from a valid parent and propagate object state and color. */
typedef struct DobboPartsObject252 DobboPartsObject252;
typedef struct DobboPartsDescriptor252 {
    unsigned char gap0[0x2f0];
    DobboPartsObject252 *parts[2];
    DobboPartsObject252 *parent;
} DobboPartsDescriptor252;
struct DobboPartsObject252 {
    unsigned char gap0[0x20];
    unsigned char status;
    unsigned char gap21[0x10];
    unsigned char disabled;
    unsigned char gap32[2];
    unsigned short flags;
    unsigned char gap36[0x32];
    DobboPartsDescriptor252 *descriptor;
    unsigned char gap6c[0x3e];
    short class_code;
};
extern void LVL_9_DOBBO_FUN_003205C8(void *, void *, int, int);
extern void LVL_9_DOBBO_FUN_003F7788(void *, void *);
void LVL_9_DOBBO_FUN_003F9468(DobboPartsObject252 *object)
{
    DobboPartsDescriptor252 *descriptor = object->descriptor;
    DobboPartsObject252 *parent = descriptor->parent;
    int i;
    if (!parent || parent->class_code != 0xd0e || parent->status == 0xfe || parent->status == 0xfd)
        return;
    for (i = 0; i < 2; ++i) {
        if (descriptor->parts[i]) {
            LVL_9_DOBBO_FUN_003205C8(descriptor->parent, descriptor->parts[i], i, i == 1 ? 6 : 0);
            if (object->disabled) {
                descriptor->parts[i]->flags &= ~1U;
                descriptor->parts[i]->disabled = 1;
            } else {
                descriptor->parts[i]->flags |= 1;
                descriptor->parts[i]->disabled = 0;
            }
            LVL_9_DOBBO_FUN_003F7788(object, descriptor->parts[i]);
        }
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
extern DobboFormatRoot396 LVL_9_DOBBO_D_001C9FE0;
extern const char LVL_9_DOBBO_D_001A9A60[];
extern const char LVL_9_DOBBO_D_001A9A68[];
extern const unsigned char *LVL_9_DOBBO_FUN_002F09E8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_9_DOBBO_FUN_00305300(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_9_DOBBO_FUN_002F09E8(LVL_9_DOBBO_D_001C9FE0.rows[index].text_id);
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
        int key = LVL_9_DOBBO_D_001C9FE0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_9_DOBBO_D_002651F0[LVL_9_DOBBO_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_9_DOBBO_D_001A9A60, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_9_DOBBO_D_001A9A68);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_9_DOBBO_FUN_002B6300(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)LVL_9_DOBBO_D_00189E20;
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
extern int LVL_9_DOBBO_FUN_0035C990(int, unsigned int, void *);
void LVL_9_DOBBO_FUN_00448778(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_9_DOBBO_FUN_0035C990(3, 0, 0);
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
        LVL_9_DOBBO_FUN_0035C990(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_9_DOBBO_FUN_0035C990(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_9_DOBBO_FUN_0035C990(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_9_DOBBO_D_001B27C0[16];
extern u32 LVL_9_DOBBO_D_001B2800[16];

int LVL_9_DOBBO_FUN_0030D698(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_9_DOBBO_D_001B27C0[index] == 0 ||
            LVL_9_DOBBO_D_001B27C0[index] == object) {
            LVL_9_DOBBO_D_001B27C0[index] = object;
            LVL_9_DOBBO_D_001B2800[index] = 0;
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
extern void LVL_9_DOBBO_FUN_002EA4A0(f32 *output, const f32 *left, const f32 *right);
extern void LVL_9_DOBBO_FUN_00321470(NativeAngleOwner *owner, f32 *output, const f32 *input, s32 mode);
extern f32 LVL_9_DOBBO_FUN_002EAB28(f32 x, f32 y);
extern f32 LVL_9_DOBBO_FUN_002EA5E8(const f32 *vector);
void LVL_9_DOBBO_FUN_004051F8(NativeAngleOwner *owner) {
    f32 direction[4] __attribute__((aligned(16)));
    NativeAngleState *state = owner->field68;
    f32 angle, xy_length;
    LVL_9_DOBBO_FUN_002EA4A0(direction, state->field1F0, owner->field10);
    LVL_9_DOBBO_FUN_00321470(owner, direction, direction, 0);
    angle = LVL_9_DOBBO_FUN_002EAB28(direction[0], direction[1]);
    state->field2E0 = angle;
    if (0.78539824f < angle) state->field2E0 = 0.78539824f;
    else if (angle < -0.78539824f) state->field2E0 = -0.78539824f;
    xy_length = LVL_9_DOBBO_FUN_002EA5E8(direction);
    angle = -LVL_9_DOBBO_FUN_002EAB28(xy_length, direction[2]);
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
extern void LVL_9_DOBBO_FUN_002D8BD8(OozlaAppendObject164 *, int, const float *);
extern void LVL_9_DOBBO_FUN_002EA4A0(float *, const float *, const float *);
extern void LVL_9_DOBBO_FUN_002EA930(float *, const float *, const float *);
extern void LVL_9_DOBBO_FUN_002D8F20(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_9_DOBBO_FUN_002D8B30(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_9_DOBBO_FUN_002D8BD8(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_9_DOBBO_FUN_002EA4A0(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_9_DOBBO_FUN_002EA930(difference, difference, &object->transform[0][0]);
        LVL_9_DOBBO_FUN_002D8F20(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_9_DOBBO_FUN_0044DBF0(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_9_DOBBO_FUN_0044E088(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

extern unsigned char D_19B278[];

int LVL_9_DOBBO_FUN_00323220(void)
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
extern u8 LVL_9_DOBBO_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_9_DOBBO_FUN_00329E60(void) {
    if (((CallState *)LVL_9_DOBBO_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_9_DOBBO_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_9_DOBBO_D_001A63A8)->active); ((CallState *)LVL_9_DOBBO_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_9_DOBBO_FUN_003169C8(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_9_DOBBO_FUN_00301C00(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_9_DOBBO_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_9_DOBBO_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

typedef struct {
    u8 pad0000[0x2294];
    u32 kind;
    u32 unknown2298;
    u32 mode;
} ResidentFlags2294;

s32 LVL_9_DOBBO_FUN_002B09D0(void)
{
    ResidentFlags2294 *root = (ResidentFlags2294 *)LVL_9_DOBBO_D_00189E20;

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

extern void LVL_9_DOBBO_FUN_0030C790(NativeUpdate775View *object);

void LVL_9_DOBBO_FUN_0039C630(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_9_DOBBO_FUN_0030C790(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_9_DOBBO_FUN_00325E48(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_9_DOBBO_FUN_0031B7F8(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}
