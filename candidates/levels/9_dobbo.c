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


void LVL_9_DOBBO_FUN_002AA4D8(void)
{
}


void LVL_9_DOBBO_FUN_002AA9C0(void)
{
}


void LVL_9_DOBBO_FUN_002AA9C8(void)
{
}


unsigned int LVL_9_DOBBO_FUN_002D2920(void)
{
    return 0;
}


void LVL_9_DOBBO_FUN_002E97A8(void)
{
}


void LVL_9_DOBBO_FUN_002F0780(void)
{
}


void LVL_9_DOBBO_FUN_002F1530(void)
{
}


void LVL_9_DOBBO_FUN_002F89F0(void)
{
}


void LVL_9_DOBBO_FUN_002FC960(void)
{
}


void LVL_9_DOBBO_FUN_003056B8(void)
{
}


unsigned int LVL_9_DOBBO_FUN_0036C800(void)
{
    return 0;
}


void LVL_9_DOBBO_FUN_00371318(void)
{
}


void LVL_9_DOBBO_FUN_00378330(void)
{
}


void LVL_9_DOBBO_FUN_0037BDC0(void)
{
}


void LVL_9_DOBBO_FUN_0037F4A0(void)
{
}


void LVL_9_DOBBO_FUN_003F28D0(void)
{
}


void LVL_9_DOBBO_FUN_004214A8(void)
{
}


void LVL_9_DOBBO_FUN_0043B3B0(void)
{
}


void LVL_9_DOBBO_FUN_0043CE20(void)
{
}


void LVL_9_DOBBO_FUN_0043D070(void)
{
}


void LVL_9_DOBBO_FUN_0043D568(void)
{
}


void LVL_9_DOBBO_FUN_00445DE0(void)
{
}


void LVL_9_DOBBO_FUN_00446A38(void)
{
}


void LVL_9_DOBBO_FUN_00452840(void)
{
}


void LVL_9_DOBBO_FUN_00454BD0(void)
{
}


void LVL_9_DOBBO_FUN_00456468(void)
{
}
void LVL_9_DOBBO_FUN_003B5A00(char *p)
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
void LVL_9_DOBBO_FUN_003C3E48(char *p)
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
void LVL_9_DOBBO_FUN_003D1C10(char *p)
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
void LVL_9_DOBBO_FUN_003D9060(char *p)
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
void LVL_9_DOBBO_FUN_003DC608(char *p)
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
void LVL_9_DOBBO_FUN_003E2A28(char *p)
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
void LVL_9_DOBBO_FUN_003ED550(char *p)
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
void LVL_9_DOBBO_FUN_003EE8F8(char *p)
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
void LVL_9_DOBBO_FUN_0041ED80(char *p)
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
void LVL_9_DOBBO_FUN_00426EE8(char *p)
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

extern u8 LVL_9_DOBBO_F62e6ff2b_D_00189E20[];
extern u8 LVL_9_DOBBO_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_9_DOBBO_FUN_0035C588(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_9_DOBBO_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_9_DOBBO_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_9_DOBBO_F62e6ff2b_D_00188660;
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
void LVL_9_DOBBO_FUN_003A4690(char *p)
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
void LVL_9_DOBBO_FUN_003E5B10(char *p)
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
extern Blob LVL_9_DOBBO_F6894d7c1_D_001A8E60;
int LVL_9_DOBBO_FUN_002EFA58(int x)
{
    Blob b;
    int i;
    b = LVL_9_DOBBO_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
void LVL_9_DOBBO_FUN_00343698(void)
{
    *(short *)0x1AA882 = *(unsigned char *)0x1A7BC9 ? 3 : 0;
    *(short *)0x1AA89A = *(unsigned char *)0x1A7BCA ? 3 : 0;
    *(short *)0x1AA8B2 = *(unsigned char *)0x1A7BCB ? 3 : 0;
    *(short *)0x1AA8CA = *(unsigned char *)0x1A7BCC ? 3 : 0;
    *(short *)0x1AA8E2 = *(unsigned char *)0x1A7BCE ? 3 : 0;
}
void LVL_9_DOBBO_FUN_00385490(char *p)
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
void LVL_9_DOBBO_FUN_00387B18(char *p)
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
void LVL_9_DOBBO_FUN_003AEFD0(char *p)
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
void LVL_9_DOBBO_FUN_00429D90(char *p)
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

extern Entry *LVL_9_DOBBO_Fc68ad20a_D_0018C2B8;

int LVL_9_DOBBO_FUN_002CE010(int key, int *out)
{
    Entry *e = LVL_9_DOBBO_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_9_DOBBO_F55a1acb8_D_001A63A8;
extern short LVL_9_DOBBO_F55a1acb8_D_001A63AC;
extern int LVL_9_DOBBO_F55a1acb8_FUN_00133688(void);
extern void LVL_9_DOBBO_F55a1acb8_FUN_0011AEA0(int);

void LVL_9_DOBBO_FUN_0032AF30(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_9_DOBBO_F55a1acb8_FUN_00133688()) {
        LVL_9_DOBBO_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_9_DOBBO_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_9_DOBBO_F55a1acb8_D_001A63A8.f6;
    q = LVL_9_DOBBO_F55a1acb8_D_001A63A8.f18;
    LVL_9_DOBBO_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_9_DOBBO_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_9_DOBBO_F55a1acb8_D_001A63A8.f1C;
        LVL_9_DOBBO_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_9_DOBBO_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern void LVL_9_DOBBO_Fc84472d1_FUN_00115E38(char *file, int line, char *message);
extern char LVL_9_DOBBO_Fc84472d1_D_001ADD98[];
extern char LVL_9_DOBBO_Fc84472d1_D_001ADDB8[];

void LVL_9_DOBBO_FUN_0043B3C8(int *p, int b, int c, int d)
{
    if ((unsigned)b < 4)
        LVL_9_DOBBO_Fc84472d1_FUN_00115E38(LVL_9_DOBBO_Fc84472d1_D_001ADD98, 37, LVL_9_DOBBO_Fc84472d1_D_001ADDB8);

    p[0] = c;
    p[1] = d;
    p[2] = b;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
typedef struct { int f00; int f04; int f08; int f0C; int f10; int f14; } Thing;
extern void LVL_9_DOBBO_Feab1f2e5_FUN_00115E38(char *file, int line, char *message);
extern char LVL_9_DOBBO_Feab1f2e5_D_001ADD98[];
extern char LVL_9_DOBBO_Feab1f2e5_D_001ADDE0[];
int LVL_9_DOBBO_FUN_0043B450(Thing *p)
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
            LVL_9_DOBBO_Feab1f2e5_FUN_00115E38(LVL_9_DOBBO_Feab1f2e5_D_001ADD98, 83, LVL_9_DOBBO_Feab1f2e5_D_001ADDE0);
            return 0;
        }
        p->f0C = a2 + p->f08;
        r = p->f00 + a2;
        p->f10 = p->f10 + 1;
        return r;
    }
}
extern char LVL_9_DOBBO_Fa2dbe766_D_00189E20[];

int LVL_9_DOBBO_FUN_003CBAA8(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_9_DOBBO_Fa2dbe766_D_00189E20;
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
void LVL_9_DOBBO_FUN_004164D8(char *object)
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
extern char LVL_9_DOBBO_Fea34650e_D_00189E20[];

void LVL_9_DOBBO_FUN_002CF100(void)
{
    char *b = LVL_9_DOBBO_Fea34650e_D_00189E20;
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
        char *c = LVL_9_DOBBO_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_9_DOBBO_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_9_DOBBO_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_9_DOBBO_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_9_DOBBO_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_9_DOBBO_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_9_DOBBO_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_9_DOBBO_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_9_DOBBO_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_9_DOBBO_FUN_003E61E8(char *p)
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
void LVL_9_DOBBO_FUN_00429D28(char *p)
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
extern char LVL_9_DOBBO_F0be97c76_D_00189E20[];

void LVL_9_DOBBO_FUN_002AA458(void)
{
    char *base = LVL_9_DOBBO_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_9_DOBBO_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_9_DOBBO_Fee2b87d1_D_00152CD0;

s32 LVL_9_DOBBO_FUN_00301340(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_9_DOBBO_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_9_DOBBO_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_9_DOBBO_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_9_DOBBO_Fee2b87d1_D_00152CD0.slots[value] == i) break;
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
   itself (`LVL_9_DOBBO_F490e2c59_D_001AA7F0[i]`), not through a local `int *` copy.  With a local the
   allocator keeps one register for the whole live range (26 instructions);
   with the global gcc loads it into $v1, hoists the loop-invariant value and
   emits the retail `move $a2,$v1` copy at the block boundary (28 instructions,
   byte-identical). */
extern int *LVL_9_DOBBO_F490e2c59_D_001AA7F0 __attribute__((sda));
extern int LVL_9_DOBBO_F490e2c59_D_001A79F0 __attribute__((sda));

int LVL_9_DOBBO_FUN_003027C8(int param_1)
{
    int index = -1;
    int i;

    for (i = 0; i < 28; i++)
        if (LVL_9_DOBBO_F490e2c59_D_001AA7F0[i] == param_1) { index = i; break; }

    if (LVL_9_DOBBO_F490e2c59_D_001AA7F0[index] == 0)
        index = LVL_9_DOBBO_F490e2c59_D_001A79F0 ? -1 : index;

    return index;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_9_DOBBO_F1157be91_D_001A63E8;
extern unsigned char LVL_9_DOBBO_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_9_DOBBO_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_9_DOBBO_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_9_DOBBO_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_9_DOBBO_F1157be91_FUN_00133230(void);
extern int LVL_9_DOBBO_F1157be91_FUN_00132028(void);

int LVL_9_DOBBO_FUN_0032ADB8(int a0, int a1, int a2) {
    CdMode mode = LVL_9_DOBBO_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_9_DOBBO_F1157be91_D_001A7900[0];
    LVL_9_DOBBO_F1157be91_D_001A7430[0] = 0;
    LVL_9_DOBBO_F1157be91_D_001A7434 = 0;
    LVL_9_DOBBO_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_9_DOBBO_F1157be91_FUN_00133230();
    LVL_9_DOBBO_F1157be91_FUN_00132028();
    return 1;
}
typedef struct { char *p[6]; } Sep6;
extern Sep6 LVL_9_DOBBO_Fd1b66aa7_D_001AA550;
extern char LVL_9_DOBBO_Fd1b66aa7_D_001AA568[];
extern char LVL_9_DOBBO_Fd1b66aa7_D_001AA578[];
extern char LVL_9_DOBBO_Fd1b66aa7_D_001AA588[];
extern int LVL_9_DOBBO_Fd1b66aa7_FUN_00115DA8(char *dst, char *fmt, ...);

void LVL_9_DOBBO_FUN_0032B260(char *dst, int x, int idx)
{
    Sep6 t;
    t = LVL_9_DOBBO_Fd1b66aa7_D_001AA550;
    if (x > 999999) {
        int m = x / 1000000;
        int w = x % 1000000;
        LVL_9_DOBBO_Fd1b66aa7_FUN_00115DA8(dst, LVL_9_DOBBO_Fd1b66aa7_D_001AA568, m, t.p[idx % 6], w / 1000,
                          t.p[idx % 6], w % 1000);
    } else if (x >= 1000)
        LVL_9_DOBBO_Fd1b66aa7_FUN_00115DA8(dst, LVL_9_DOBBO_Fd1b66aa7_D_001AA578, x / 1000,
                          t.p[idx % 6], x % 1000);
    else
        LVL_9_DOBBO_Fd1b66aa7_FUN_00115DA8(dst, LVL_9_DOBBO_Fd1b66aa7_D_001AA588, x);
}
/* attempt 1 - shape read straight off the disassembly:
   save object's word 152 from its owner's field 16, then repeat for a second
   object found through two pointer hops, skipping the sentinel byte values. */

void LVL_9_DOBBO_FUN_003F74A8(char *p)
{
    char *q;
    char *r;
    char *s;
    int v;

    if (p == 0)
        return;
    if (*(unsigned char *)(p + 32) == 254)
        return;
    if (*(unsigned char *)(p + 32) == 253)
        return;

    q = *(char **)(p + 36);
    r = *(char **)(p + 104);
    *(int *)(p + 152) = *(int *)(q + 16);

    s = *(char **)(r + 760);
    if (s == 0)
        return;
    if (*(short *)(s + 170) != 3342)
        return;
    if (*(unsigned char *)(s + 32) == 254)
        return;
    if (*(unsigned char *)(s + 32) == 253)
        return;

    v = *(int *)(s + 36);
    *(int *)(s + 152) = *(int *)(v + 16);
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_9_DOBBO_F4e5bde81_D_00189E20;
extern s32 LVL_9_DOBBO_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_9_DOBBO_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_9_DOBBO_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_9_DOBBO_FUN_002CF458(void) {
    s32 result = LVL_9_DOBBO_F4e5bde81_D_00189E20.field348;
    if (LVL_9_DOBBO_F4e5bde81_D_001A8FF0 != 0 && LVL_9_DOBBO_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_9_DOBBO_F4e5bde81_D_001A8FF4 != 0 || LVL_9_DOBBO_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_9_DOBBO_F4e5bde81_D_00189E20.field2294 == 110 && LVL_9_DOBBO_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_9_DOBBO_F4e5bde81_D_00189E20.field2294 == 109 || LVL_9_DOBBO_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_9_DOBBO_F4e5bde81_D_00189E20.field1497 != 0 && LVL_9_DOBBO_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_9_DOBBO_F4e5bde81_D_00189E20.field2294 == 0 && LVL_9_DOBBO_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_9_DOBBO_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}
extern void LVL_9_DOBBO_F250a61cf_FUN_00115DA8(char *buf, char *format, ...);
extern char LVL_9_DOBBO_F250a61cf_D_001AD670[];
extern char LVL_9_DOBBO_F250a61cf_D_001AD680[];
extern char LVL_9_DOBBO_F250a61cf_D_001AD688[];

void LVL_9_DOBBO_FUN_00372D18(char *buf, int value)
{
    if (value > 999999)
        LVL_9_DOBBO_F250a61cf_FUN_00115DA8(buf, LVL_9_DOBBO_F250a61cf_D_001AD670, value / 1000000,
                                 value / 1000 % 1000, value % 1000);
    else if (value >= 1000)
        LVL_9_DOBBO_F250a61cf_FUN_00115DA8(buf, LVL_9_DOBBO_F250a61cf_D_001AD680, value / 1000, value % 1000);
    else
        LVL_9_DOBBO_F250a61cf_FUN_00115DA8(buf, LVL_9_DOBBO_F250a61cf_D_001AD688, value);
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_9_DOBBO_Fabf21065e887d7a9_AT0030B208_ROLE00;

int LVL_9_DOBBO_FUN_0030B208(void)
{
    if ((LVL_9_DOBBO_Fabf21065e887d7a9_AT0030B208_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_9_DOBBO_Fabf21065e887d7a9_AT0030B208_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_9_DOBBO_Fabf21065e887d7a9_AT0030B208_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_9_DOBBO_Fabf21065e887d7a9_AT0030B208_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_9_DOBBO_F5b4b17178a13f443_AT0031F3F0_ROLE00(void *object);

void LVL_9_DOBBO_FUN_0031F3F0(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_9_DOBBO_F5b4b17178a13f443_AT0031F3F0_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_9_DOBBO_Facdcf1600d770d3b_AT0035CCC8_ROLE00[];

void LVL_9_DOBBO_FUN_0035CCC8(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_9_DOBBO_Facdcf1600d770d3b_AT0035CCC8_ROLE00;
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


extern void LVL_9_DOBBO_Fb1b523716b470b36_AT003845F0_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_9_DOBBO_Fb1b523716b470b36_AT003845F0_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_9_DOBBO_FUN_003845F0(void *object)
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
    LVL_9_DOBBO_Fb1b523716b470b36_AT003845F0_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_9_DOBBO_Fb1b523716b470b36_AT003845F0_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_9_DOBBO_Fb1b523716b470b36_AT00387098_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_9_DOBBO_Fb1b523716b470b36_AT00387098_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_9_DOBBO_FUN_00387098(void *object)
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
    LVL_9_DOBBO_Fb1b523716b470b36_AT00387098_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_9_DOBBO_Fb1b523716b470b36_AT00387098_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003960C8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_003960C8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003960C8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT0039AC00_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_0039AC00(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT0039AC00_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_9_DOBBO_F86f665335d9cb905_AT003C2998_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_9_DOBBO_FUN_003C2998(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_9_DOBBO_F86f665335d9cb905_AT003C2998_ROLE00(owner, owner->context_68);
}


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003C9370_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_003C9370(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003C9370_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003CE9E8_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_003CE9E8(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003CE9E8_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003E6610_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_003E6610(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003E6610_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003EB418_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_003EB418(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT003EB418_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT00409190_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_9_DOBBO_FUN_00409190(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_9_DOBBO_F9cdc323a4d0c2fbd_AT00409190_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_9_DOBBO_F6af85cabb56d3b41_AT004382E8_ROLE00;

void LVL_9_DOBBO_FUN_004382E8(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_9_DOBBO_F6af85cabb56d3b41_AT004382E8_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_9_DOBBO_F6af85cabb56d3b41_AT004382E8_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_9_DOBBO_FUN_0043A120(float factor, void *context,
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


void LVL_9_DOBBO_FUN_0043A2D0(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_9_DOBBO_FUN_0043F630(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_9_DOBBO_FUN_00447658(float first, float second, float **cell)
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


extern unsigned char LVL_9_DOBBO_F79744baad5ad7f65_AT0044EAC8_ROLE00[];

void LVL_9_DOBBO_FUN_0044EAC8(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_9_DOBBO_F79744baad5ad7f65_AT0044EAC8_ROLE00[0] == 0) {
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


extern Rac2Native_6b0741c38bf00fee_u8 LVL_9_DOBBO_F6b0741c38bf00fee_AT002A9608_ROLE00[];
extern void LVL_9_DOBBO_F6b0741c38bf00fee_AT002A9608_ROLE01(Rac2Native_6b0741c38bf00fee_u64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s64, Rac2Native_6b0741c38bf00fee_s32);
extern void LVL_9_DOBBO_F6b0741c38bf00fee_AT002A9608_ROLE02(short, short, short);

void LVL_9_DOBBO_FUN_002A9608(void) {
 LVL_9_DOBBO_F6b0741c38bf00fee_AT002A9608_ROLE01(LVL_9_DOBBO_F6b0741c38bf00fee_AT002A9608_ROLE00[1],0,1,0x32);
 LVL_9_DOBBO_F6b0741c38bf00fee_AT002A9608_ROLE02(0x16,7,0);
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


extern Rac2Native_28c929cadd7aca24_NativeResidentView LVL_9_DOBBO_F28c929cadd7aca24_AT002CE508_ROLE00;

void LVL_9_DOBBO_FUN_002CE508(Rac2Native_28c929cadd7aca24_s32 selected, Rac2Native_28c929cadd7aca24_s32 index) {
    if (selected >= 0) {
        Rac2Native_28c929cadd7aca24_u8 *object = LVL_9_DOBBO_F28c929cadd7aca24_AT002CE508_ROLE00.object;
        Rac2Native_28c929cadd7aca24_s32 offset = object[0x43] * 4;
        Rac2Native_28c929cadd7aca24_u8 *table = *(Rac2Native_28c929cadd7aca24_u8 **)(object + 0x24);
        Rac2Native_28c929cadd7aca24_u8 *entry = *(Rac2Native_28c929cadd7aca24_u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_9_DOBBO_F28c929cadd7aca24_AT002CE508_ROLE00.selected = selected;
            LVL_9_DOBBO_F28c929cadd7aca24_AT002CE508_ROLE00.index = index;
        }
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef float Rac2Native_5fc519c90e0e763a_f32;


extern void LVL_9_DOBBO_F5fc519c90e0e763a_AT002CF7D0_ROLE00(Rac2Native_5fc519c90e0e763a_f32);

void LVL_9_DOBBO_FUN_002CF7D0(Rac2Native_5fc519c90e0e763a_f32 value) {
 LVL_9_DOBBO_F5fc519c90e0e763a_AT002CF7D0_ROLE00(-value);
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


void LVL_9_DOBBO_FUN_0037D188(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[139];

void LVL_9_DOBBO_FUN_00387A40(void)
{
    int i;
    LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[138] = 5;
    LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[137] = 0;
    LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[i] = 0;
        LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_9_DOBBO_F03c444112283bc5f_AT00387A40_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_9_DOBBO_FUN_002A8848(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_9_DOBBO_FUN_002D0008(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_9_DOBBO_FUN_002D84E0(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_9_DOBBO_FUN_002D84E8(void) {
    return 1;
}


extern void LVL_9_DOBBO_QWEN_11a4c157d102_AT002F0500_ROLE000(unsigned char *);

void LVL_9_DOBBO_FUN_002F0500(void *owner)
{
    LVL_9_DOBBO_QWEN_11a4c157d102_AT002F0500_ROLE000(owner);
}



void LVL_9_DOBBO_QWEN_407ee6f17a73_AT002F05A0_ROLE001(int);
void LVL_9_DOBBO_QWEN_407ee6f17a73_AT002F05A0_ROLE000(void*);

void LVL_9_DOBBO_FUN_002F05A0(void *param)
{
  LVL_9_DOBBO_QWEN_407ee6f17a73_AT002F05A0_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_9_DOBBO_QWEN_407ee6f17a73_AT002F05A0_ROLE000(param);
}


void LVL_9_DOBBO_QWEN_5696fcf76f0c_AT0030DEA8_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_9_DOBBO_FUN_0030DEA8(void)
{
    LVL_9_DOBBO_QWEN_5696fcf76f0c_AT0030DEA8_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


unsigned int LVL_9_DOBBO_FUN_00328B00(unsigned int param_1)
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
extern void LVL_9_DOBBO_QWEN_523e38f49b74_AT003439E8_ROLE000(long);
extern void LVL_9_DOBBO_QWEN_523e38f49b74_AT003439E8_ROLE002(void);
extern void LVL_9_DOBBO_QWEN_523e38f49b74_AT003439E8_ROLE001(void);

int LVL_9_DOBBO_FUN_003439E8(void)
{
  LVL_9_DOBBO_QWEN_523e38f49b74_AT003439E8_ROLE000(0);
  LVL_9_DOBBO_QWEN_523e38f49b74_AT003439E8_ROLE002();
  LVL_9_DOBBO_QWEN_523e38f49b74_AT003439E8_ROLE001();
  return 0;
}


unsigned long long LVL_9_DOBBO_FUN_00343DE8(void);

extern void LVL_9_DOBBO_QWEN_f47929f95774_AT00343DE8_ROLE000(long);
extern void LVL_9_DOBBO_QWEN_f47929f95774_AT00343DE8_ROLE002(void);
extern void LVL_9_DOBBO_QWEN_f47929f95774_AT00343DE8_ROLE001(void);

unsigned long long LVL_9_DOBBO_FUN_00343DE8(void)
{
  LVL_9_DOBBO_QWEN_f47929f95774_AT00343DE8_ROLE000(0);
  LVL_9_DOBBO_QWEN_f47929f95774_AT00343DE8_ROLE002();
  LVL_9_DOBBO_QWEN_f47929f95774_AT00343DE8_ROLE001();
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
extern void LVL_9_DOBBO_QWEN_cb305b1f1210_AT00344088_ROLE000(long);
extern void LVL_9_DOBBO_QWEN_cb305b1f1210_AT00344088_ROLE002(void);
extern void LVL_9_DOBBO_QWEN_cb305b1f1210_AT00344088_ROLE001(void);

long long LVL_9_DOBBO_FUN_00344088(void)
{
    LVL_9_DOBBO_QWEN_cb305b1f1210_AT00344088_ROLE000(0LL);
    LVL_9_DOBBO_QWEN_cb305b1f1210_AT00344088_ROLE002();
    LVL_9_DOBBO_QWEN_cb305b1f1210_AT00344088_ROLE001();
    return 0LL;
}


extern void LVL_9_DOBBO_QWEN_c23b3406f982_AT00349480_ROLE000(long);
extern void LVL_9_DOBBO_QWEN_c23b3406f982_AT00349480_ROLE002(void);
extern void LVL_9_DOBBO_QWEN_c23b3406f982_AT00349480_ROLE001(void);

int LVL_9_DOBBO_FUN_00349480(void)
{
    LVL_9_DOBBO_QWEN_c23b3406f982_AT00349480_ROLE000(0);
    LVL_9_DOBBO_QWEN_c23b3406f982_AT00349480_ROLE002();
    LVL_9_DOBBO_QWEN_c23b3406f982_AT00349480_ROLE001();
    return 0;
}


void LVL_9_DOBBO_QWEN_68cf9ebb5ee2_AT003495F0_ROLE000(long);
void LVL_9_DOBBO_QWEN_68cf9ebb5ee2_AT003495F0_ROLE001(void);
void LVL_9_DOBBO_QWEN_68cf9ebb5ee2_AT003495F0_ROLE002(void);

unsigned long long LVL_9_DOBBO_FUN_003495F0(void)
{
    LVL_9_DOBBO_QWEN_68cf9ebb5ee2_AT003495F0_ROLE000(0);
    LVL_9_DOBBO_QWEN_68cf9ebb5ee2_AT003495F0_ROLE002();
    LVL_9_DOBBO_QWEN_68cf9ebb5ee2_AT003495F0_ROLE001();
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

extern void LVL_9_DOBBO_QWEN_68f040eb20c9_AT003496A8_ROLE000(long arg0);
extern void LVL_9_DOBBO_QWEN_68f040eb20c9_AT003496A8_ROLE002(void);
extern void LVL_9_DOBBO_QWEN_68f040eb20c9_AT003496A8_ROLE001(void);

unsigned long long LVL_9_DOBBO_FUN_003496A8(void)
{
    LVL_9_DOBBO_QWEN_68f040eb20c9_AT003496A8_ROLE000(0);
    LVL_9_DOBBO_QWEN_68f040eb20c9_AT003496A8_ROLE002();
    LVL_9_DOBBO_QWEN_68f040eb20c9_AT003496A8_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_9_DOBBO_QWEN_92ea7c2f4a5c_AT00349A58_ROLE000(long);
extern void LVL_9_DOBBO_QWEN_92ea7c2f4a5c_AT00349A58_ROLE001(void);
extern void LVL_9_DOBBO_QWEN_92ea7c2f4a5c_AT00349A58_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_9_DOBBO_FUN_00349A58(void)
{
  LVL_9_DOBBO_QWEN_92ea7c2f4a5c_AT00349A58_ROLE000(0);
  LVL_9_DOBBO_QWEN_92ea7c2f4a5c_AT00349A58_ROLE002();
  LVL_9_DOBBO_QWEN_92ea7c2f4a5c_AT00349A58_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_9_DOBBO_QWEN_d01c568afacc_AT00349B58_ROLE000(long param_1);
extern void LVL_9_DOBBO_QWEN_d01c568afacc_AT00349B58_ROLE001(void);
extern void LVL_9_DOBBO_QWEN_d01c568afacc_AT00349B58_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_9_DOBBO_FUN_00349B58(void)
{
    LVL_9_DOBBO_QWEN_d01c568afacc_AT00349B58_ROLE000(0);
    LVL_9_DOBBO_QWEN_d01c568afacc_AT00349B58_ROLE002();
    LVL_9_DOBBO_QWEN_d01c568afacc_AT00349B58_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_9_DOBBO_QWEN_093381cf82c3_AT0034BD08_ROLE000(long);
extern void LVL_9_DOBBO_QWEN_093381cf82c3_AT0034BD08_ROLE002(void);
extern void LVL_9_DOBBO_QWEN_093381cf82c3_AT0034BD08_ROLE001(void);

long long LVL_9_DOBBO_FUN_0034BD08(void)
{
    LVL_9_DOBBO_QWEN_093381cf82c3_AT0034BD08_ROLE000(0);
    LVL_9_DOBBO_QWEN_093381cf82c3_AT0034BD08_ROLE002();
    LVL_9_DOBBO_QWEN_093381cf82c3_AT0034BD08_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_9_DOBBO_FUN_003559D0(void) {
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



extern QwenRecovery_4ecc6b5a4034_ExtraResetRecord164 LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE000[100];
extern QwenRecovery_4ecc6b5a4034_u32 LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[139];

void LVL_9_DOBBO_FUN_00384FC8(void)
{
    int i;
    for (i = 99; i >= 0; --i)
        LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE000[i].word = 0;
    LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[138] = 5;
    LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[137] = 0;
    LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[i] = 0;
        LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_9_DOBBO_QWEN_4ecc6b5a4034_AT00384FC8_ROLE001[i + 128] = 0;
}



void LVL_9_DOBBO_FUN_0043B040(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_9_DOBBO_FUN_0043B048(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_9_DOBBO_FUN_0043B1A0(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_9_DOBBO_FUN_0043B2F0(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_9_DOBBO_FUN_0043B3C0(unsigned long param_1)
{
  return param_1;
}


extern void LVL_9_DOBBO_QWEN_df8fc8ba49cf_AT0043DD88_ROLE000(unsigned char *owner, int value);

void LVL_9_DOBBO_FUN_0043DD88(unsigned char *owner, int value)
{
    LVL_9_DOBBO_QWEN_df8fc8ba49cf_AT0043DD88_ROLE000(owner + 8, value);
}



void* LVL_9_DOBBO_FUN_00442788(void* param_1);

extern void LVL_9_DOBBO_QWEN_f353c206e726_AT00442788_ROLE000(int);
extern unsigned long long LVL_9_DOBBO_QWEN_f353c206e726_AT00442788_ROLE001(unsigned long long);

void* LVL_9_DOBBO_FUN_00442788(void* param_1)
{
  LVL_9_DOBBO_QWEN_f353c206e726_AT00442788_ROLE000((int)param_1 + 8);
  LVL_9_DOBBO_QWEN_f353c206e726_AT00442788_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_9_DOBBO_QWEN_f2f9288a4e34_AT00442928_ROLE000(int);

int LVL_9_DOBBO_FUN_00442928(int owner)
{
    int result;
    result = LVL_9_DOBBO_QWEN_f2f9288a4e34_AT00442928_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_9_DOBBO_QWEN_e41cd63258f5_AT00442960_ROLE000(unsigned char *);

int LVL_9_DOBBO_FUN_00442960(int owner)
{
    return LVL_9_DOBBO_QWEN_e41cd63258f5_AT00442960_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_9_DOBBO_QWEN_f29f80950ce7_AT00446080_ROLE000(int);

void LVL_9_DOBBO_FUN_00446080(int param_1)
{
  LVL_9_DOBBO_QWEN_f29f80950ce7_AT00446080_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_9_DOBBO_QWEN_bf824305b9f7_AT00446998_ROLE000(unsigned char *owner, float *records);

void LVL_9_DOBBO_FUN_00446998(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_9_DOBBO_QWEN_bf824305b9f7_AT00446998_ROLE000(owner + 0x188, records);
}



void LVL_9_DOBBO_QWEN_61faeca45963_AT00446C60_ROLE000(int param_1);

void LVL_9_DOBBO_FUN_00446C60(int param_1)
{
  LVL_9_DOBBO_QWEN_61faeca45963_AT00446C60_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_9_DOBBO_FUN_00446DC8(unsigned long param_1)
{
  return param_1;
}


extern int LVL_9_DOBBO_QWEN_6add11b33414_AT00447B78_ROLE000(unsigned char *owner);

int LVL_9_DOBBO_FUN_00447B78(unsigned char *owner)
{
    return LVL_9_DOBBO_QWEN_6add11b33414_AT00447B78_ROLE000(owner + 0x298);
}



extern void LVL_9_DOBBO_QWEN_de961518de48_AT00447B98_ROLE000(unsigned char *owner);

void LVL_9_DOBBO_FUN_00447B98(unsigned char *owner)
{
    LVL_9_DOBBO_QWEN_de961518de48_AT00447B98_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_9_DOBBO_QWEN_7ba3cdeeb16b_AT0044C4B8_ROLE000(unsigned long long);

unsigned long long LVL_9_DOBBO_FUN_0044C4B8(unsigned long long value)
{
    LVL_9_DOBBO_QWEN_7ba3cdeeb16b_AT0044C4B8_ROLE000(value);
    return value;
}



void LVL_9_DOBBO_FUN_0044C700(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_9_DOBBO_FUN_0044D760(unsigned long param_1)
{
  return param_1;
}


void LVL_9_DOBBO_FUN_0044DAA0(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_9_DOBBO_FUN_0044DC40(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_9_DOBBO_FUN_0044DC88(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_9_DOBBO_FUN_00452BE0(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_9_DOBBO_FUN_00454CD0(void) {
    return 1;
}


extern int LVL_9_DOBBO_QWEN_cc83cb329fcb_AT00456510_ROLE000(unsigned char *);

int LVL_9_DOBBO_FUN_00456510(int *owner)
{
    int result;
    long status;
    status = LVL_9_DOBBO_QWEN_cc83cb329fcb_AT00456510_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_003A40D8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_003B5C60(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_003C41C8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_003D2668(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_003E5558(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_003EF3D0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_0041F100(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(char *local, char *first, char *second);
extern void LVL_9_DOBBO_Fc936841d_FUN_002EA470(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void LVL_9_DOBBO_FUN_00426E80(Vec16_Fc936841d *object, int index)
{
    char local[16];

    LVL_9_DOBBO_Fc936841d_FUN_002EA4A0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    LVL_9_DOBBO_Fc936841d_FUN_002EA470((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void LVL_9_DOBBO_F01bd4546_FUN_0043A478(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC48(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC48(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC48(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC48(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC48(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC48(char *p, int v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC90(char *p, float v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC90(char *p, float v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC90(char *p, float v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC90(char *p, float v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC90(char *p, float v);
extern void LVL_9_DOBBO_F01bd4546_FUN_0044DC90(char *p, float v);

void LVL_9_DOBBO_FUN_004511A0(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        LVL_9_DOBBO_F01bd4546_FUN_0043A478(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        LVL_9_DOBBO_F01bd4546_FUN_0044DC48(p1, 1);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC48(p2, 1);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC48(p3, 1);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC48(p4, 1);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC48(p5, 1);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC48(p6, 1);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC90(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC90(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC90(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC90(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC90(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        LVL_9_DOBBO_F01bd4546_FUN_0044DC90(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void LVL_9_DOBBO_F70997ba9_FUN_00378928(int a, int b);
extern void LVL_9_DOBBO_F70997ba9_FUN_002E24F0(int a);
extern void *LVL_9_DOBBO_F70997ba9_FUN_002F09E8(int id);
extern int LVL_9_DOBBO_F70997ba9_FUN_002E51F8(void *p, int i);
extern void LVL_9_DOBBO_F70997ba9_FUN_002E5180(void);
extern void LVL_9_DOBBO_F70997ba9_FUN_002E5618(int a, int b, long c, void *d, int e);
extern void LVL_9_DOBBO_F70997ba9_FUN_002E5170(void);
extern void LVL_9_DOBBO_F70997ba9_FUN_002E2610(void);

int LVL_9_DOBBO_FUN_0034FF60(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    LVL_9_DOBBO_F70997ba9_FUN_00378928(66, 68);
    LVL_9_DOBBO_F70997ba9_FUN_00378928(71, 11);
    LVL_9_DOBBO_F70997ba9_FUN_002E24F0(0);
    min = LVL_9_DOBBO_F70997ba9_FUN_002E51F8(LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11613), -1);
    v = LVL_9_DOBBO_F70997ba9_FUN_002E51F8(LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = LVL_9_DOBBO_F70997ba9_FUN_002E51F8(LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11614), -1);
        if (v >= min)
            min = v;
    }
    v = LVL_9_DOBBO_F70997ba9_FUN_002E51F8(LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11626), -1);
    if (v >= min)
        min = v;
    v = LVL_9_DOBBO_F70997ba9_FUN_002E51F8(LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11627), -1);
    if (v >= min)
        min = v;
    v = LVL_9_DOBBO_F70997ba9_FUN_002E51F8(LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    LVL_9_DOBBO_F70997ba9_FUN_002E5180();
    off = count - 6;
    LVL_9_DOBBO_F70997ba9_FUN_002E5618(slot, off, 0x80FFA888L, LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11613), -1);
    off += count;
    LVL_9_DOBBO_F70997ba9_FUN_002E5618(slot, off, 0x80FFA888L, LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11625), -1);
    off += count;
    LVL_9_DOBBO_F70997ba9_FUN_002E5618(slot, off, 0x80FFA888L, LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11626), -1);
    off += count;
    LVL_9_DOBBO_F70997ba9_FUN_002E5618(slot, off, 0x80FFA888L, LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11627), -1);
    off += count;
    LVL_9_DOBBO_F70997ba9_FUN_002E5618(slot, off, 0x80FFA888L, LVL_9_DOBBO_F70997ba9_FUN_002F09E8(11599), -1);
    LVL_9_DOBBO_F70997ba9_FUN_002E5170();
    LVL_9_DOBBO_F70997ba9_FUN_002E2610();
    return 2;
}
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);

void LVL_9_DOBBO_FUN_003A3CA0(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[0], v[3]);
        v[1] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[1], v[4]);
        v[2] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[2], v[5]);
        v[9] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[9], v[12]);
        v[10] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[10], v[13]);
        v[11] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[11], v[14]);
        v[20] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(v[0]) * v[6];
        v[21] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[1]) * v[7];
        v[22] = -LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[2]) * v[8];
        v[24] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(v[9]) * v[15];
        v[25] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[10]) * v[16];
        v[26] = -LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[11]) * v[17];
        v += 32;
    } while (--count != 0);
}
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);
extern float LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(float);

void LVL_9_DOBBO_FUN_003E5120(int unused, float *p, int n)
{
    float *v;
    int count;
    if (n <= 0)
        return;
    v = p;
    count = n;
    do {
        v[0] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[0], v[3]);
        v[1] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[1], v[4]);
        v[2] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[2], v[5]);
        v[9] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[9], v[12]);
        v[10] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[10], v[13]);
        v[11] = LVL_9_DOBBO_F307ea0ee_FUN_002EB478(v[11], v[14]);
        v[20] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(v[0]) * v[6];
        v[21] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[1]) * v[7];
        v[22] = -LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[2]) * v[8];
        v[24] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA60(v[9]) * v[15];
        v[25] = LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[10]) * v[16];
        v[26] = -LVL_9_DOBBO_F307ea0ee_FUN_002EAA78(v[11]) * v[17];
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


extern float LVL_9_DOBBO_F9e2cd219_FUN_003169A0(float value);
extern void LVL_9_DOBBO_F9e2cd219_FUN_0031CF78(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_9_DOBBO_F9e2cd219_FUN_00384C88(void *owner, void *out);

void LVL_9_DOBBO_FUN_00384BA0(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_9_DOBBO_F9e2cd219_FUN_0031CF78((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_9_DOBBO_F9e2cd219_FUN_003169A0(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_9_DOBBO_F9e2cd219_FUN_00384C88(self, (char *)child + 48);

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


extern float LVL_9_DOBBO_F9e2cd219_FUN_003169A0(float value);
extern void LVL_9_DOBBO_F9e2cd219_FUN_0031CF78(void *out, void *in, void *tmp, int mode, float value);
extern void LVL_9_DOBBO_F9e2cd219_FUN_00387728(void *owner, void *out);

void LVL_9_DOBBO_FUN_00387640(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    LVL_9_DOBBO_F9e2cd219_FUN_0031CF78((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            LVL_9_DOBBO_F9e2cd219_FUN_003169A0(child->field54));

    child->field54 = child->field54 + child->field58;
    LVL_9_DOBBO_F9e2cd219_FUN_00387728(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
extern void LVL_9_DOBBO_F6df9730c_FUN_002EA2E8(char *dst, int *src, int count);

void LVL_9_DOBBO_FUN_00304ED8(char *dst, unsigned char *src)
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
        LVL_9_DOBBO_F6df9730c_FUN_002EA2E8(dst, tmp, 64);
        dst = next;
        LVL_9_DOBBO_F6df9730c_FUN_002EA2E8(dst, tmp, 64);
        dst += 64;
        LVL_9_DOBBO_F6df9730c_FUN_002EA2E8(dst, tmp, 64);
        dst += 64;
        LVL_9_DOBBO_F6df9730c_FUN_002EA2E8(dst, tmp, 64);
        dst += 64;
    }
}
extern void LVL_9_DOBBO_Fdb046c5d_FUN_002D86B0(void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_002D84F0(void *);
extern int LVL_9_DOBBO_Fdb046c5d_FUN_002D88C8(void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_002EA750(float, void *, void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_002EA5A0(void *, void *, void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_002D8E50(void *, void *, void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_00321670(void *, void *, int);
extern int LVL_9_DOBBO_Fdb046c5d_FUN_002D8F70(void *, void *, void *, float, float);
extern int LVL_9_DOBBO_Fdb046c5d_FUN_0035C830(int, int, void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_002D8B30(void *, int, void *);
extern int LVL_9_DOBBO_Fdb046c5d_FUN_0035C830(int, int, void *);
extern void LVL_9_DOBBO_Fdb046c5d_FUN_0035CB80(int, void *);
extern char LVL_9_DOBBO_Fdb046c5d_D_001BFE80[];

int LVL_9_DOBBO_FUN_002D80D8(char *obj)
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
        LVL_9_DOBBO_Fdb046c5d_FUN_002D86B0(obj);
    else
        LVL_9_DOBBO_Fdb046c5d_FUN_002D84F0(obj);

    s5 = LVL_9_DOBBO_Fdb046c5d_FUN_002D88C8(obj);
    if (s5 == -1)
        return 0;

    s1 = LVL_9_DOBBO_Fdb046c5d_D_001BFE80;
    s4 = -1;
    LVL_9_DOBBO_Fdb046c5d_FUN_002EA750(1.0f, s1, s1);
    LVL_9_DOBBO_Fdb046c5d_FUN_002EA5A0(buf, s1, p);
    LVL_9_DOBBO_Fdb046c5d_FUN_002D8E50(obj, p + 16, buf);
    LVL_9_DOBBO_Fdb046c5d_FUN_00321670(obj + 16, out, 1);
    r = LVL_9_DOBBO_Fdb046c5d_FUN_002D8F70(p, s1, out, *(float *)(p + 56), *(float *)(p + 60));
    if (r == 0) {
        if (*(unsigned char *)(p + 94) != 255 && *(unsigned char *)(p + 92) == 0)
            s4 = LVL_9_DOBBO_Fdb046c5d_FUN_0035C830(*(unsigned char *)(p + 94), 0, obj);
        LVL_9_DOBBO_Fdb046c5d_FUN_002D8B30(obj, s5, out);
    } else {
        if (*(unsigned char *)(p + 94) != 255)
            s4 = LVL_9_DOBBO_Fdb046c5d_FUN_0035C830(*(unsigned char *)(p + 94), 0, obj);
    }
    if (s4 != -1)
        LVL_9_DOBBO_Fdb046c5d_FUN_0035CB80(s4, p + 32);
    return 0;
}
extern void LVL_9_DOBBO_F7242f0a4_FUN_002EA4F8(char *out, void *source, float value);
extern void LVL_9_DOBBO_F7242f0a4_FUN_0042F300(char *buffer, int mode);
extern void LVL_9_DOBBO_F7242f0a4_FUN_0042F1A0(int value, char *buffer);
extern void LVL_9_DOBBO_F7242f0a4_FUN_002EA470(char *first, char *second, char *third);
extern float LVL_9_DOBBO_F7242f0a4_FUN_002EA578(void *owner, char *buffer);

extern int LVL_9_DOBBO_F7242f0a4_D_001B9A60[];

void LVL_9_DOBBO_FUN_0042F9C8(void *owner, int flag, float value)
{
    char buffer[32];
    int *root = LVL_9_DOBBO_F7242f0a4_D_001B9A60;

    LVL_9_DOBBO_F7242f0a4_FUN_002EA4F8(buffer, root + 8, value);
    if (flag)
        LVL_9_DOBBO_F7242f0a4_FUN_0042F300(buffer + 16, 1);
    else
        LVL_9_DOBBO_F7242f0a4_FUN_0042F1A0(root[-4], buffer + 16);
    LVL_9_DOBBO_F7242f0a4_FUN_002EA470(buffer, buffer, buffer + 16);
    LVL_9_DOBBO_F7242f0a4_FUN_002EA4F8(owner, root + 12, LVL_9_DOBBO_F7242f0a4_FUN_002EA578(root + 12, buffer));
}
extern char LVL_9_DOBBO_F45821cfb_D_001C7780[];
extern void LVL_9_DOBBO_F45821cfb_FUN_002EA438(char *);

void LVL_9_DOBBO_FUN_00453208(void)
{
    float *p = (float *)LVL_9_DOBBO_F45821cfb_D_001C7780;

    p[224] = 0.4f;
    p[225] = 0.8f;
    p[226] = 1.2f;
    *(int *)&p[227] = 0;
    p[228] = 0.577f;
    p[229] = 0.577f;
    p[230] = -0.577f;
    *(int *)&p[231] = 0;
    LVL_9_DOBBO_F45821cfb_FUN_002EA438((char *)&p[232]);
    LVL_9_DOBBO_F45821cfb_FUN_002EA438((char *)&p[236]);
}
extern char LVL_9_DOBBO_Fd8e166aa_D_001BCE40[];

void LVL_9_DOBBO_FUN_002DF848(void)
{
    char *g = LVL_9_DOBBO_Fd8e166aa_D_001BCE40;
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
extern char LVL_9_DOBBO_F7cb419c1_D_001FF880[];

extern int LVL_9_DOBBO_F7cb419c1_FUN_00354DE0(int arg);

int LVL_9_DOBBO_FUN_0034B5D8(void)
{
    char *p = LVL_9_DOBBO_F7cb419c1_D_001FF880;

    *(int *)(p + 460) = LVL_9_DOBBO_F7cb419c1_FUN_00354DE0(*(int *)(p + 460));
    return 0;
}
extern char LVL_9_DOBBO_F0a76d85b_D_001B98C0[];

extern void LVL_9_DOBBO_F0a76d85b_FUN_002EAC40(char *p);
extern void LVL_9_DOBBO_F0a76d85b_FUN_002E0010(void);

void LVL_9_DOBBO_FUN_00453298(void)
{
    char *p = LVL_9_DOBBO_F0a76d85b_D_001B98C0;

    *(int *)(p + 320) = 0;
    *(int *)(p + 324) = 0;
    *(int *)(p + 328) = 0;
    LVL_9_DOBBO_F0a76d85b_FUN_002EAC40(p + 880);
    LVL_9_DOBBO_F0a76d85b_FUN_002E0010();
}
extern char LVL_9_DOBBO_F391de845_D_001B9A00[];
extern float LVL_9_DOBBO_F391de845_FUN_002EA630(char *a, char *b);
extern float LVL_9_DOBBO_F391de845_FUN_0035B280(void *self, float d, float x, float y);

float LVL_9_DOBBO_FUN_0035B378(char *self, char *p)
{
    float v = LVL_9_DOBBO_F391de845_FUN_002EA630(p, LVL_9_DOBBO_F391de845_D_001B9A00);
    float *q = *(float **)(self + 8);

    return LVL_9_DOBBO_F391de845_FUN_0035B280(q, v, q[0], q[1]);
}
extern int LVL_9_DOBBO_F2f080549_D_001A8FF4 __attribute__((sda));
extern char LVL_9_DOBBO_F2f080549_D_001B24D0[] __attribute__((sda));
extern char LVL_9_DOBBO_F2f080549_D_001A9000[] __attribute__((sda));
extern float LVL_9_DOBBO_F2f080549_FUN_002EA630(char *a, char *b);

float LVL_9_DOBBO_FUN_00321A48(char *p)
{
    float v;

    if (LVL_9_DOBBO_F2f080549_D_001A8FF4 == 0) {
        v = LVL_9_DOBBO_F2f080549_FUN_002EA630(p, LVL_9_DOBBO_F2f080549_D_001B24D0);
    } else {
        v = 100.0f - LVL_9_DOBBO_F2f080549_FUN_002EA630(p, LVL_9_DOBBO_F2f080549_D_001A9000);
    }
    return v;
}
extern char LVL_9_DOBBO_Fa76f1772_D_001BFE60[];
extern void LVL_9_DOBBO_Fa76f1772_FUN_002EA4A0(char *local, char *data);
extern float LVL_9_DOBBO_Fa76f1772_FUN_002EA578(char *local, char *p);

int LVL_9_DOBBO_FUN_00432EA0(int a0, int a1)
{
    char local[16];
    int s0;
    int r;

    if (a1 != 1) {
        r = 0;
    } else {
        s0 = *(int *)(a0 + 112) + 352;
        LVL_9_DOBBO_Fa76f1772_FUN_002EA4A0(local, LVL_9_DOBBO_Fa76f1772_D_001BFE60);
        r = LVL_9_DOBBO_Fa76f1772_FUN_002EA578(local, (char *)s0) < 0.0f ? 1 : 0;
    }
    return r;
}
extern void LVL_9_DOBBO_F46b43b72_FUN_0032ACC0(int value, char *target);
extern void LVL_9_DOBBO_F46b43b72_FUN_0032AE88(int value);

extern int LVL_9_DOBBO_F46b43b72_D_001BD700[];
extern int LVL_9_DOBBO_F46b43b72_D_0014B540[];

int LVL_9_DOBBO_FUN_00300A20(int index)
{
    int j = index + 1;
    int *d = LVL_9_DOBBO_F46b43b72_D_001BD700;
    int *b = LVL_9_DOBBO_F46b43b72_D_0014B540;
    int *t = (int *)((char *)b + 25416);
    int n = d[12] * 332;
    int hold = d[28];
    int j4 = j * 4;
    int cur = *(int *)((char *)t + (n + index * 4));
    int next = *(int *)((char *)t + (j4 + n));

    n = next - cur;
    if (n > 0) {
        LVL_9_DOBBO_F46b43b72_FUN_0032ACC0(hold, (char *)(cur + b[6341]));
        LVL_9_DOBBO_F46b43b72_FUN_0032AE88(0);
    }
    return 1;
}
extern void LVL_9_DOBBO_Fa2d20de7_FUN_00316BD0(void *object, float first, float second);
extern void LVL_9_DOBBO_Fa2d20de7_FUN_002EA470(char *first, char *second, void *third);
extern int LVL_9_DOBBO_Fa2d20de7_FUN_002DA5C0(void *first, char *second, int mode, int value, int extra);
extern void LVL_9_DOBBO_Fa2d20de7_FUN_002EA4A0(void *first, void *second, void *third);
extern void LVL_9_DOBBO_Fa2d20de7_FUN_002EA4F8(void *first, void *second, float value);

extern short LVL_9_DOBBO_Fa2d20de7_D_001B9A00[];
extern short LVL_9_DOBBO_Fa2d20de7_D_001BFE60[];
extern int LVL_9_DOBBO_Fa2d20de7_D_001886CC[];

void LVL_9_DOBBO_FUN_0035B130(void *object)
{
    LVL_9_DOBBO_Fa2d20de7_FUN_00316BD0(object, 0.5f, 6.0f);
    LVL_9_DOBBO_Fa2d20de7_FUN_002EA470(object, object, LVL_9_DOBBO_Fa2d20de7_D_001B9A00);
    if (LVL_9_DOBBO_Fa2d20de7_FUN_002DA5C0(LVL_9_DOBBO_Fa2d20de7_D_001B9A00, object, 130, LVL_9_DOBBO_Fa2d20de7_D_001886CC[0], 0)) {
        LVL_9_DOBBO_Fa2d20de7_FUN_002EA4A0(object, LVL_9_DOBBO_Fa2d20de7_D_001BFE60, LVL_9_DOBBO_Fa2d20de7_D_001B9A00);
        LVL_9_DOBBO_Fa2d20de7_FUN_002EA4F8(object, object, 0.75f);
        LVL_9_DOBBO_Fa2d20de7_FUN_002EA470(object, object, LVL_9_DOBBO_Fa2d20de7_D_001B9A00);
    }
}
extern char LVL_9_DOBBO_F15d5f4fb_D_001B98C0[];
extern char LVL_9_DOBBO_F15d5f4fb_D_00189E20[];
extern float LVL_9_DOBBO_F15d5f4fb_FUN_002EAB28(float a, float b);
extern float LVL_9_DOBBO_F15d5f4fb_FUN_002EB560(float a, float b);
extern float LVL_9_DOBBO_F15d5f4fb_FUN_002EA630(char *a, char *b);

void LVL_9_DOBBO_FUN_004373F0(char *o)
{
    char *B = LVL_9_DOBBO_F15d5f4fb_D_001B98C0;
    char *D = LVL_9_DOBBO_F15d5f4fb_D_00189E20;
    float *p = *(float **)(D + 8848);
    float r = LVL_9_DOBBO_F15d5f4fb_FUN_002EB560(*(float *)(B + 344),
                      LVL_9_DOBBO_F15d5f4fb_FUN_002EAB28(p[4] - *(float *)(B + 320),
                              p[5] - *(float *)(B + 324)));

    if (r < 0.5585054f) {
        r = LVL_9_DOBBO_F15d5f4fb_FUN_002EA630(D + 128, B + 320);
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





extern void LVL_9_DOBBO_F4778f810_FUN_003217C0(char *a, char *b, char *c, f32 d);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA470(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA930(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EB238(char *a, char *b);
extern void LVL_9_DOBBO_F4778f810_FUN_002EB018(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA930(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA4A0(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA4A0(char *a, char *b, char *c);

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


void LVL_9_DOBBO_FUN_002D84F0(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_9_DOBBO_F4778f810_FUN_003217C0((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_9_DOBBO_F4778f810_FUN_002EA470((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_9_DOBBO_F4778f810_FUN_002EA930((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_9_DOBBO_F4778f810_FUN_002EB238((char *)p + 0x10, (char *)&tmp[3]);
    LVL_9_DOBBO_F4778f810_FUN_002EB018((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_9_DOBBO_F4778f810_FUN_002EA930((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_9_DOBBO_F4778f810_FUN_002EA4A0((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_9_DOBBO_F4778f810_FUN_002EA4A0((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
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





extern void LVL_9_DOBBO_F4778f810_FUN_003217C0(char *a, char *b, char *c, f32 d);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA470(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA930(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EB238(char *a, char *b);
extern void LVL_9_DOBBO_F4778f810_FUN_002EB018(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA930(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA4A0(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F4778f810_FUN_002EA4A0(char *a, char *b, char *c);

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


void LVL_9_DOBBO_FUN_002D85D0(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    LVL_9_DOBBO_F4778f810_FUN_003217C0((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    LVL_9_DOBBO_F4778f810_FUN_002EA470((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    LVL_9_DOBBO_F4778f810_FUN_002EA930((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_9_DOBBO_F4778f810_FUN_002EB238((char *)p + 0x10, (char *)&tmp[3]);
    LVL_9_DOBBO_F4778f810_FUN_002EB018((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    LVL_9_DOBBO_F4778f810_FUN_002EA930((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    LVL_9_DOBBO_F4778f810_FUN_002EA4A0((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    LVL_9_DOBBO_F4778f810_FUN_002EA4A0((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}


extern void LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(int arg0, int arg1, int arg2, int arg3);
extern int LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(float value);
extern void LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(int arg0, int arg1, int arg2, int arg3);
extern int LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(float value);
extern void LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(int arg0, int arg1, int arg2, int arg3);
extern int LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(float value);
extern void LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(int arg0, int arg1, int arg2, int arg3);
extern int LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(float value);
extern void LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(int arg0, int arg1, int arg2, int arg3);
extern int LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(float value);
extern void LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(int arg0, int arg1, int arg2, int arg3);

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


extern ResidentState_Fd160fb9c LVL_9_DOBBO_Fd160fb9c_D_00189E20;

void LVL_9_DOBBO_FUN_002A98A0(void)
{
    int selector;

    if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.f1B8 < 15)
            return;
    }

    selector = LVL_9_DOBBO_Fd160fb9c_D_00189E20.b149D;

    if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.f2294 == 2) {
        if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.f1B8 == 22)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 0, 1, 30);
    }

    switch (LVL_9_DOBBO_Fd160fb9c_D_00189E20.p2290[67]) {
    case 3:
        if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(49.5f) != 0)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 0, 1, 30);
        if (LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(17.0f) != 0)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 1, 1, 30);
        return;
    case 4:
        if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(12.5f) != 0)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 0, 1, 30);
        if (LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(1.0f) != 0)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 1, 1, 30);
        return;
    case 20:
        if (LVL_9_DOBBO_Fd160fb9c_D_00189E20.fC2C != 0)
            return;
        if (LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(8.0f) != 0)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 0, 1, 30);
        if (LVL_9_DOBBO_Fd160fb9c_FUN_002B6300(21.0f) != 0)
            LVL_9_DOBBO_Fd160fb9c_FUN_002A9648(selector, 1, 1, 30);
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


extern Persistent_F882f1178 LVL_9_DOBBO_F882f1178_D_00189E20;
extern int LVL_9_DOBBO_F882f1178_D_001A8F00 __attribute__((sda));

extern float LVL_9_DOBBO_F882f1178_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F882f1178_FUN_002EAA78(float);
extern int LVL_9_DOBBO_F882f1178_FUN_002EB618(int, int, float);
extern float LVL_9_DOBBO_F882f1178_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F882f1178_FUN_002EB478(float, float);
extern float LVL_9_DOBBO_F882f1178_FUN_002EAA78(float);
extern int LVL_9_DOBBO_F882f1178_FUN_002EB618(int, int, float);
extern void LVL_9_DOBBO_F882f1178_FUN_00322530(int, float *, int, int, float);
extern void LVL_9_DOBBO_F882f1178_FUN_00322530(int, float *, int, int, float);

void LVL_9_DOBBO_FUN_002D0AE0(void)
{
    Node_F882f1178 *node;

    node = (Node_F882f1178 *)LVL_9_DOBBO_F882f1178_D_00189E20.p1360;
    if (node != 0) {
        if (*(int *)0x1A8F00 == 2)
            LVL_9_DOBBO_F882f1178_D_00189E20.f250C = LVL_9_DOBBO_F882f1178_FUN_002EB478(LVL_9_DOBBO_F882f1178_D_00189E20.f250C, 0.020362177863717079f);
        else
            LVL_9_DOBBO_F882f1178_D_00189E20.f250C = LVL_9_DOBBO_F882f1178_FUN_002EB478(LVL_9_DOBBO_F882f1178_D_00189E20.f250C, 0.034906592220067978f);
        node->field120 = LVL_9_DOBBO_F882f1178_FUN_002EB618(0xd2d2d2, 0x285050,
                                 LVL_9_DOBBO_F882f1178_FUN_002EAA78(LVL_9_DOBBO_F882f1178_D_00189E20.f250C) * 0.5f + 0.5f);
    }
    node = (Node_F882f1178 *)LVL_9_DOBBO_F882f1178_D_00189E20.p1364;
    if (node != 0) {
        if (LVL_9_DOBBO_F882f1178_D_001A8F00 != 2) {
            float limit;
            int which;

            limit = LVL_9_DOBBO_F882f1178_FUN_002EB478(LVL_9_DOBBO_F882f1178_D_00189E20.f2510, 0.049451004713773727f);
            LVL_9_DOBBO_F882f1178_D_00189E20.f2510 = limit;
            which = 0x1ee628;
            if (limit > 0.0f && limit < 1.9198623895645142f) {
                which = 0x1e1ed2;
            }
            node->field120 = LVL_9_DOBBO_F882f1178_FUN_002EB618(node->field120, which, 0.070000000298023224f);
        } else {
            LVL_9_DOBBO_F882f1178_D_00189E20.f2510 = LVL_9_DOBBO_F882f1178_FUN_002EB478(LVL_9_DOBBO_F882f1178_D_00189E20.f2510, 0.026179943233728409f);
            node->field120 = LVL_9_DOBBO_F882f1178_FUN_002EB618(0x1e1ed2, 0x1e1e50,
                                     LVL_9_DOBBO_F882f1178_FUN_002EAA78(LVL_9_DOBBO_F882f1178_D_00189E20.f2510) * 0.5f + 0.5f);
        }
    }
    if (LVL_9_DOBBO_F882f1178_D_00189E20.p1368 != 0) {
        if (LVL_9_DOBBO_F882f1178_D_001A8F00 == 2)
            LVL_9_DOBBO_F882f1178_FUN_00322530((int)LVL_9_DOBBO_F882f1178_D_00189E20.p1368, &LVL_9_DOBBO_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.026179943233728409f);
        else
            LVL_9_DOBBO_F882f1178_FUN_00322530((int)LVL_9_DOBBO_F882f1178_D_00189E20.p1368, &LVL_9_DOBBO_F882f1178_D_00189E20.f2514, 0xdcdcdc, 0x323232,
                    0.040724355727434158f);
    }
}
extern void LVL_9_DOBBO_F8411efa9_FUN_003D9060(char *pkt);
extern long long LVL_9_DOBBO_F8411efa9_FUN_002E2828(char *p);
extern float LVL_9_DOBBO_F8411efa9_FUN_002EAB28(float x, float y);
extern float LVL_9_DOBBO_F8411efa9_FUN_002EA5E8(char *p);
extern float LVL_9_DOBBO_F8411efa9_FUN_002EAB28(float x, float y);
extern void LVL_9_DOBBO_F8411efa9_FUN_002EACF0(float *matrix, float *quat);
extern void LVL_9_DOBBO_F8411efa9_FUN_002EA750(float *off, char *src, float scale);
extern void LVL_9_DOBBO_F8411efa9_FUN_003D90E8(char *pkt, float angle);
extern void LVL_9_DOBBO_F8411efa9_FUN_002E6C68(char *pkt, float *matrix, int mode);

void LVL_9_DOBBO_FUN_003D9308(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_9_DOBBO_F8411efa9_FUN_003D9060(pkt);
    r = LVL_9_DOBBO_F8411efa9_FUN_002E2828(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_9_DOBBO_F8411efa9_FUN_002EAB28(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_9_DOBBO_F8411efa9_FUN_002EAB28(LVL_9_DOBBO_F8411efa9_FUN_002EA5E8(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_9_DOBBO_F8411efa9_FUN_002EACF0(m, quat);
    LVL_9_DOBBO_F8411efa9_FUN_002EA750(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_9_DOBBO_F8411efa9_FUN_003D90E8(pkt, f12);
    LVL_9_DOBBO_F8411efa9_FUN_002E6C68(pkt, m, 0);
}
extern void LVL_9_DOBBO_F8411efa9_FUN_003DC608(char *pkt);
extern long long LVL_9_DOBBO_F8411efa9_FUN_002E2828(char *p);
extern float LVL_9_DOBBO_F8411efa9_FUN_002EAB28(float x, float y);
extern float LVL_9_DOBBO_F8411efa9_FUN_002EA5E8(char *p);
extern float LVL_9_DOBBO_F8411efa9_FUN_002EAB28(float x, float y);
extern void LVL_9_DOBBO_F8411efa9_FUN_002EACF0(float *matrix, float *quat);
extern void LVL_9_DOBBO_F8411efa9_FUN_002EA750(float *off, char *src, float scale);
extern void LVL_9_DOBBO_F8411efa9_FUN_003DC690(char *pkt, float angle);
extern void LVL_9_DOBBO_F8411efa9_FUN_002E6C68(char *pkt, float *matrix, int mode);

void LVL_9_DOBBO_FUN_003DC6F8(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    LVL_9_DOBBO_F8411efa9_FUN_003DC608(pkt);
    r = LVL_9_DOBBO_F8411efa9_FUN_002E2828(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = LVL_9_DOBBO_F8411efa9_FUN_002EAB28(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -LVL_9_DOBBO_F8411efa9_FUN_002EAB28(LVL_9_DOBBO_F8411efa9_FUN_002EA5E8(src), *(float *)(src + 8));
    quat[0] = f13;

    LVL_9_DOBBO_F8411efa9_FUN_002EACF0(m, quat);
    LVL_9_DOBBO_F8411efa9_FUN_002EA750(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    LVL_9_DOBBO_F8411efa9_FUN_003DC690(pkt, f12);
    LVL_9_DOBBO_F8411efa9_FUN_002E6C68(pkt, m, 0);
}
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);

void LVL_9_DOBBO_FUN_003D1DB8(char *p, float scale)
{
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p, p, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 16, p + 16, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 32, p + 32, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 48, p + 48, scale);
}
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);

void LVL_9_DOBBO_FUN_003D90E8(char *p, float scale)
{
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p, p, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 16, p + 16, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 32, p + 32, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 48, p + 48, scale);
}
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);

void LVL_9_DOBBO_FUN_003DC690(char *p, float scale)
{
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p, p, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 16, p + 16, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 32, p + 32, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 48, p + 48, scale);
}
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);

void LVL_9_DOBBO_FUN_003E2AB0(char *p, float scale)
{
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p, p, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 16, p + 16, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 32, p + 32, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 48, p + 48, scale);
}
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);

void LVL_9_DOBBO_FUN_003ED5D8(char *p, float scale)
{
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p, p, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 16, p + 16, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 32, p + 32, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 48, p + 48, scale);
}
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);
extern void LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(char *dst, char *src, float scale);

void LVL_9_DOBBO_FUN_003EE980(char *p, float scale)
{
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p, p, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 16, p + 16, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 32, p + 32, scale);
    LVL_9_DOBBO_F2c74c194_FUN_002EA4F8(p + 48, p + 48, scale);
}




extern u8 LVL_9_DOBBO_F458c670e_D_00189E20[];

extern void LVL_9_DOBBO_F458c670e_FUN_00319EA8(f32 *a, f32 *b, f32 c, f32 d, f32 e, f32 f);
extern f32 LVL_9_DOBBO_F458c670e_FUN_002EA3E0(f32 v);
extern void LVL_9_DOBBO_F458c670e_FUN_00319DE8(f32 *p, f32 v, f32 w);

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


void LVL_9_DOBBO_FUN_002C75F0(void)
{
    f32 old, v, r, sum;

    if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i1B8 >= 11
        || ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790 < 0.0f
        || ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i22A4 == 17
        || ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i22B0 == 17) {
        if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i2294 == 53) {
            LVL_9_DOBBO_F458c670e_FUN_00319EA8(&((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790,
                    &((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->fA3C,
                    0.0f, 0.03f, 0.3f, 0.025000002f);
            ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794 = 0.0f;
        } else {
            old = ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790;
            v = ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794;
            ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794 =
                v - (old * 0.005f + v * 0.045f);
            r = LVL_9_DOBBO_F458c670e_FUN_002EA3E0(old);
            if (r < 0.001f) {
                if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794 < 0.0001f)
                    ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794 = 0.0f;
            }
        }
    }
    sum = ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790
        + ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794;
    ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790 = sum;
    if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i2294 == 53) {
        if (sum > 0.0f) {
            if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794 > 0.0f)
                ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f794 = 0.0f;
            LVL_9_DOBBO_F458c670e_FUN_00319DE8(&((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790,
                    0.0f, 0.011666667f);
        }
    }
    if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i2294 != 124) {
        f32 x = ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f330;
        if (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->i229C == 22)
            x = ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f9B0;
        LVL_9_DOBBO_F458c670e_FUN_00319EA8(&((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f79C,
                &((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f7A0,
                x, 0.027f, 0.3f, 0.0f);
    }
    ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f088 =
        (((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f79C + (-0.12f))
        + ((Res_F458c670e *)LVL_9_DOBBO_F458c670e_D_00189E20)->f790;
}
extern void LVL_9_DOBBO_F40487154_FUN_002EACD0(char *a, char *b);
extern float LVL_9_DOBBO_F40487154_FUN_002EAA78(float value);
extern void LVL_9_DOBBO_F40487154_FUN_002EA4F8(char *a, char *b, float value);
extern float LVL_9_DOBBO_F40487154_FUN_002EB478(float value, float scale);

void LVL_9_DOBBO_FUN_003DF2F0(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_9_DOBBO_F40487154_FUN_002EACD0(object + 192, object + 240);
    x = LVL_9_DOBBO_F40487154_FUN_002EAA78(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_9_DOBBO_F40487154_FUN_002EAA78(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_9_DOBBO_F40487154_FUN_002EAA78(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_9_DOBBO_F40487154_FUN_002EA4F8(object + 192, object + 192, x);
    LVL_9_DOBBO_F40487154_FUN_002EA4F8(object + 208, object + 208, y);
    LVL_9_DOBBO_F40487154_FUN_002EA4F8(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_9_DOBBO_F40487154_FUN_002EB478(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_9_DOBBO_F40487154_FUN_002EB478(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_9_DOBBO_F40487154_FUN_002EB478(*(float *)(data + 12), 0.5f);
}
extern void LVL_9_DOBBO_F40487154_FUN_002EACD0(char *a, char *b);
extern float LVL_9_DOBBO_F40487154_FUN_002EAA78(float value);
extern void LVL_9_DOBBO_F40487154_FUN_002EA4F8(char *a, char *b, float value);
extern float LVL_9_DOBBO_F40487154_FUN_002EB478(float value, float scale);

void LVL_9_DOBBO_FUN_003E9858(char *object)
{
    char *data;
    float x;
    float y;
    float z;

    data = *(char **)(object + 104);
    LVL_9_DOBBO_F40487154_FUN_002EACD0(object + 192, object + 240);
    x = LVL_9_DOBBO_F40487154_FUN_002EAA78(*(float *)(data + 4)) * 0.15f + 0.35000002f;
    y = LVL_9_DOBBO_F40487154_FUN_002EAA78(*(float *)(data + 8)) * 0.15f + 0.35000002f;
    z = LVL_9_DOBBO_F40487154_FUN_002EAA78(*(float *)(data + 12)) * 0.15f + 0.35000002f;
    LVL_9_DOBBO_F40487154_FUN_002EA4F8(object + 192, object + 192, x);
    LVL_9_DOBBO_F40487154_FUN_002EA4F8(object + 208, object + 208, y);
    LVL_9_DOBBO_F40487154_FUN_002EA4F8(object + 224, object + 224, z);
    *(float *)(data + 4) = LVL_9_DOBBO_F40487154_FUN_002EB478(*(float *)(data + 4), 0.5f);
    *(float *)(data + 8) = LVL_9_DOBBO_F40487154_FUN_002EB478(*(float *)(data + 8), 0.5f);
    *(float *)(data + 12) = LVL_9_DOBBO_F40487154_FUN_002EB478(*(float *)(data + 12), 0.5f);
}
extern int LVL_9_DOBBO_F6eb4f363_FUN_00316A28(int mode);
extern float LVL_9_DOBBO_F6eb4f363_FUN_00318A58(char *object, char *local);
extern void LVL_9_DOBBO_F6eb4f363_FUN_002EA4F8(char *local, int value, float scale);
extern float LVL_9_DOBBO_F6eb4f363_FUN_00316AC0(float low, float high);
extern void LVL_9_DOBBO_F6eb4f363_FUN_00333200(char *object, char *local, float amount, float base);

void LVL_9_DOBBO_FUN_003844E0(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_9_DOBBO_F6eb4f363_FUN_00316A28(5))
        return;
    base = LVL_9_DOBBO_F6eb4f363_FUN_00318A58(object + 16, local);
    LVL_9_DOBBO_F6eb4f363_FUN_002EA4F8(local, value, 0.25f);
    LVL_9_DOBBO_F6eb4f363_FUN_00333200(object + 16, local, LVL_9_DOBBO_F6eb4f363_FUN_00316AC0(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_9_DOBBO_F6eb4f363_FUN_00316A28(int mode);
extern float LVL_9_DOBBO_F6eb4f363_FUN_00318A58(char *object, char *local);
extern void LVL_9_DOBBO_F6eb4f363_FUN_002EA4F8(char *local, int value, float scale);
extern float LVL_9_DOBBO_F6eb4f363_FUN_00316AC0(float low, float high);
extern void LVL_9_DOBBO_F6eb4f363_FUN_00333200(char *object, char *local, float amount, float base);

void LVL_9_DOBBO_FUN_00386F88(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_9_DOBBO_F6eb4f363_FUN_00316A28(5))
        return;
    base = LVL_9_DOBBO_F6eb4f363_FUN_00318A58(object + 16, local);
    LVL_9_DOBBO_F6eb4f363_FUN_002EA4F8(local, value, 0.25f);
    LVL_9_DOBBO_F6eb4f363_FUN_00333200(object + 16, local, LVL_9_DOBBO_F6eb4f363_FUN_00316AC0(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_9_DOBBO_F6eb4f363_FUN_00316A28(int mode);
extern float LVL_9_DOBBO_F6eb4f363_FUN_00318A58(char *object, char *local);
extern void LVL_9_DOBBO_F6eb4f363_FUN_002EA4F8(char *local, int value, float scale);
extern float LVL_9_DOBBO_F6eb4f363_FUN_00316AC0(float low, float high);
extern void LVL_9_DOBBO_F6eb4f363_FUN_00333200(char *object, char *local, float amount, float base);

void LVL_9_DOBBO_FUN_0038D388(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (LVL_9_DOBBO_F6eb4f363_FUN_00316A28(5))
        return;
    base = LVL_9_DOBBO_F6eb4f363_FUN_00318A58(object + 16, local);
    LVL_9_DOBBO_F6eb4f363_FUN_002EA4F8(local, value, 0.25f);
    LVL_9_DOBBO_F6eb4f363_FUN_00333200(object + 16, local, LVL_9_DOBBO_F6eb4f363_FUN_00316AC0(0.05f, 0.1f) * 210000.0f, base);
}
extern int LVL_9_DOBBO_Fc10c1216_FUN_002EA110(char *);
extern void LVL_9_DOBBO_Fc10c1216_FUN_0032C890(char *);
extern int LVL_9_DOBBO_Fc10c1216_FUN_002EB5D0(float);
extern void LVL_9_DOBBO_Fc10c1216_FUN_002EA470(char *, char *, char *);

void LVL_9_DOBBO_FUN_00332490(char *p)
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

    if (q[1] <= 0.0244f || LVL_9_DOBBO_Fc10c1216_FUN_002EA110(p + 10) != 0) {
        LVL_9_DOBBO_Fc10c1216_FUN_0032C890(p);
        return;
    }

    if (q[1] >= 0.12f) {
        q[3] = -q[3];
        q[1] = q[1] + q[3] * 0.007f;
    }
    *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    *(int *)(p + 4) = (*(int *)(p + 4) & 0xffffff) | (LVL_9_DOBBO_Fc10c1216_FUN_002EB5D0(q[1] * 255.0f) << 24);
    q[2] = q[2] + 0.002f;
    if (q[2] > 1.0f)
        q[2] = q[2] - 1.0f;
    *(char *)(p + 8) = (char)(q[2] * 255.0f);
    LVL_9_DOBBO_Fc10c1216_FUN_002EA470(p + 16, p + 16, (char *)(q + 4));
}
extern int LVL_9_DOBBO_F250fbfa4_FUN_002EA110(char *p);
extern void LVL_9_DOBBO_F250fbfa4_FUN_0032C890(char *p);
extern void LVL_9_DOBBO_F250fbfa4_FUN_002EA470(char *p0, char *p1, char *p2);

void LVL_9_DOBBO_FUN_0033C488(char *a0)
{
    char local[16];
    char *s1 = a0;
    char *s0;
    int a1;
    int v1;
    char *p;

    if (LVL_9_DOBBO_F250fbfa4_FUN_002EA110(a0 + 10) != 0) {
        LVL_9_DOBBO_F250fbfa4_FUN_0032C890(a0);
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
    LVL_9_DOBBO_F250fbfa4_FUN_002EA470(p, p, local);
    *(float *)(s0 + 8) = *(float *)(s0 + 8) - *(float *)(s0 + 12);
    *(unsigned char *)(s1 + 8) = *(unsigned char *)(s1 + 8) + *(unsigned char *)(s0 + 21);
    *(float *)(s1 + 12) = *(float *)(s1 + 12) + *(float *)(s0 + 16);
}
extern int LVL_9_DOBBO_F250fbfa4_FUN_002EA110(char *p);
extern void LVL_9_DOBBO_F250fbfa4_FUN_0032C890(char *p);
extern void LVL_9_DOBBO_F250fbfa4_FUN_002EA470(char *p0, char *p1, char *p2);

void LVL_9_DOBBO_FUN_0033D350(char *a0)
{
    char local[16];
    char *s1 = a0;
    char *s0;
    int a1;
    int v1;
    char *p;

    if (LVL_9_DOBBO_F250fbfa4_FUN_002EA110(a0 + 10) != 0) {
        LVL_9_DOBBO_F250fbfa4_FUN_0032C890(a0);
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
    LVL_9_DOBBO_F250fbfa4_FUN_002EA470(p, p, local);
    *(float *)(s0 + 8) = *(float *)(s0 + 8) - *(float *)(s0 + 12);
    *(unsigned char *)(s1 + 8) = *(unsigned char *)(s1 + 8) + *(unsigned char *)(s0 + 21);
    *(float *)(s1 + 12) = *(float *)(s1 + 12) + *(float *)(s0 + 16);
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00440350(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00440790(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00440A48(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00440FD0(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00441A58(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00441D20(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_004420E8(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00442398(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Ffb202470_FUN_0043F168(char *p);

char *LVL_9_DOBBO_FUN_00442590(char *object)
{
    LVL_9_DOBBO_Ffb202470_FUN_0043F168(object + 8);
    return object;
}
extern void LVL_9_DOBBO_Fe524d931_FUN_0043F440(char *p);
extern void LVL_9_DOBBO_Fe524d931_FUN_0043F630(char *p, float x, float y);
extern void LVL_9_DOBBO_Fe524d931_FUN_0035C990(int a, int b, int c);
extern void LVL_9_DOBBO_Fe524d931_FUN_002EC7D8(void);
extern short LVL_9_DOBBO_Fe524d931_D_001A6480[];

int LVL_9_DOBBO_FUN_004404B0(char *object, int mask)
{
    char *sub = object + 8;
    short oldx;
    short oldy;
    int *v;

    LVL_9_DOBBO_Fe524d931_FUN_0043F440(sub);
    v = *(int **)(object + 684);
    LVL_9_DOBBO_Fe524d931_FUN_0043F630(sub, *(float *)v, *(float *)(v + 1));
    if ((mask & 0x40) != 0 && *(int *)(object + 680) == 0)
        LVL_9_DOBBO_Fe524d931_FUN_0035C990(4, 0, 0);
    if ((mask & 0xf000) != 0) {
        short *p = LVL_9_DOBBO_Fe524d931_D_001A6480;
        oldx = p[180];
        oldy = p[181];
        if ((mask & 0x1000) != 0) {
            ((unsigned short *)p)[181] = ((unsigned short *)p)[181] - 1;
            if ((short)((unsigned short *)p)[181] < -32)
                p[181] = -32;
        }
        if ((mask & 0x4000) != 0) {
            unsigned short *q = (unsigned short *)LVL_9_DOBBO_Fe524d931_D_001A6480;
            q[181] = q[181] + 1;
            if ((short)q[181] > 32)
                ((short *)q)[181] = 32;
        }
        if ((mask & 0x8000) != 0) {
            unsigned short *q = (unsigned short *)LVL_9_DOBBO_Fe524d931_D_001A6480;
            q[180] = q[180] - 1;
            if ((short)q[180] < -40)
                ((short *)q)[180] = -40;
        }
        if ((mask & 0x2000) != 0) {
            unsigned short *q = (unsigned short *)LVL_9_DOBBO_Fe524d931_D_001A6480;
            q[180] = q[180] + 1;
            if ((short)q[180] > 40)
                ((short *)q)[180] = 40;
        }
        {
            short *q = LVL_9_DOBBO_Fe524d931_D_001A6480;
            if (oldx != q[180] || oldy != q[181])
                LVL_9_DOBBO_Fe524d931_FUN_0035C990(4, 0, 0);
        }
        LVL_9_DOBBO_Fe524d931_FUN_002EC7D8();
    }
    return (mask >> 6) & 1;
}
extern void LVL_9_DOBBO_F0b028b34_FUN_00454EA0(char *a, unsigned int b, int c, int d);
extern void LVL_9_DOBBO_F0b028b34_FUN_00454EA0(char *a, unsigned int b, int c, int d);
extern void LVL_9_DOBBO_F0b028b34_FUN_00454E30(int a);

int LVL_9_DOBBO_FUN_00454F40(int *p)
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
        LVL_9_DOBBO_F0b028b34_FUN_00454EA0((char *)(p[1] + i * 16), (i * 2048 + p[0]) & 0x0FFFFFFF, 3, 128);
    }
    LVL_9_DOBBO_F0b028b34_FUN_00454EA0((char *)(p[1] + i * 16), p[1] & 0x0FFFFFFF, 2, 0);
    *(volatile unsigned int *)0x1000B420 = 0;
    *(volatile unsigned int *)0x1000B410 = p[0] & 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000B430 = p[1] & 0x0FFFFFFF;
    LVL_9_DOBBO_F0b028b34_FUN_00454E30(5);
    return 1;
}
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_0037CEC8(char *a, char *b);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA470(char *a, char *b, char *c);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA4F8(char *a, char *b, float f);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_0037ADA8(char *a, char *b);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA4F8(char *a, char *b, float f);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA470(char *a, char *b, char *c);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA4F8(char *a, char *b, float f);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA470(char *a, char *b, char *c);
extern int LVL_9_DOBBO_Fbdd4a4aa_FUN_002EB618(unsigned int a, int b, float f);
extern void LVL_9_DOBBO_Fbdd4a4aa_FUN_0032C890(char *a);

void LVL_9_DOBBO_FUN_0033E178(int *obj)
{
    char *a = (char *)obj + 16;
    char *b = (char *)obj + 32;
    char tmp[16];

    LVL_9_DOBBO_Fbdd4a4aa_FUN_0037CEC8(a, a);
    *(int *)(b + 16) = *(int *)(b + 16) - 1;
    if (*(int *)(b + 16) < 0)
        *(int *)(b + 16) = 0;
    *(unsigned char *)((char *)obj + 8) = *(unsigned char *)((char *)obj + 8) + 1;
    LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA470(a, b, a);
    *(float *)((char *)obj + 12) = *(float *)((char *)obj + 12) + 1.5750000000000000000000e+03f;
    LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA4F8(b, b, 9.9000000953674316406250e-01f);
    LVL_9_DOBBO_Fbdd4a4aa_FUN_0037ADA8(a, tmp);
    if ((*(int *)((char *)obj + 4) & 0xFF) < 32) {
        LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA4F8(tmp, tmp, 1.5000000130385160446167e-03f);
        LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA470(b, b, tmp);
    } else {
        LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA4F8(tmp, tmp, -7.5000000651925802230835e-04f);
        LVL_9_DOBBO_Fbdd4a4aa_FUN_002EA470(b, b, tmp);
    }
    *(int *)((char *)obj + 4) = LVL_9_DOBBO_Fbdd4a4aa_FUN_002EB618(*(unsigned int *)((char *)obj + 4) & 0x00FFFFFF, 0, 2.5000000372529029846191e-02f)
        | (*(int *)(b + 16) << 24);
    if (*(int *)(b + 16) == 0)
        LVL_9_DOBBO_Fbdd4a4aa_FUN_0032C890((char *)obj);
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


extern int *LVL_9_DOBBO_F7b2f1854_FUN_0043B450(int n);
extern int *LVL_9_DOBBO_F7b2f1854_FUN_0043B3B8(int size, int *p);
extern void LVL_9_DOBBO_F7b2f1854_FUN_0043A478(Obj_F7b2f1854 *o, int v);

void LVL_9_DOBBO_FUN_0043A5D8(Obj_F7b2f1854 *o, int a1, int count)
{
    int *p;
    int n;

    o->f44 = count;
    if (count != 0) {
        p = LVL_9_DOBBO_F7b2f1854_FUN_0043B3B8(16, LVL_9_DOBBO_F7b2f1854_FUN_0043B450(count));
        n = o->f44;
        o->f0 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_9_DOBBO_F7b2f1854_FUN_0043B3B8(16, LVL_9_DOBBO_F7b2f1854_FUN_0043B450(n));
        n = o->f44;
        o->f8 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_9_DOBBO_F7b2f1854_FUN_0043B3B8(16, LVL_9_DOBBO_F7b2f1854_FUN_0043B450(n));
        n = o->f44;
        o->f4 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_9_DOBBO_F7b2f1854_FUN_0043B3B8(16, LVL_9_DOBBO_F7b2f1854_FUN_0043B450(n));
        n = o->f44;
        o->f12 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = LVL_9_DOBBO_F7b2f1854_FUN_0043B3B8(16, LVL_9_DOBBO_F7b2f1854_FUN_0043B450(n));
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
    LVL_9_DOBBO_F7b2f1854_FUN_0043A478(o, 1);
}
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043ACE0(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);

char *LVL_9_DOBBO_FUN_00447FB8(char *p)
{
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 76);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 152);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 228);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 304);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 380);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 456);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 528);
    LVL_9_DOBBO_F449e2f67_FUN_0043ACE0(p + 600);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 664);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 736);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 808);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 880);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 952);
    return p;
}
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043ACE0(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);
extern void LVL_9_DOBBO_F449e2f67_FUN_0043AF78(char *p);

char *LVL_9_DOBBO_FUN_00449A18(char *p)
{
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 76);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 152);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 228);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 304);
    LVL_9_DOBBO_F449e2f67_FUN_0043A7B8(p + 380);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 456);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 528);
    LVL_9_DOBBO_F449e2f67_FUN_0043ACE0(p + 600);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 664);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 736);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 808);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 880);
    LVL_9_DOBBO_F449e2f67_FUN_0043AF78(p + 952);
    return p;
}
extern char *LVL_9_DOBBO_Ffc961fca_FUN_0031D2F8(char *a1);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EACF0(char *dst, char *src);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EA470(char *dst, char *src, char *tail);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EA4A0(char *dst, char *src, char *tail);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EACF0(char *dst, char *src);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EAF78(char *dst, char *src);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EA930(char *dst, char *src, char *tail);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EA930(char *dst, char *src, char *tail);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EA930(char *dst, char *src, char *tail);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EA470(char *dst, char *src, char *tail);
extern void LVL_9_DOBBO_Ffc961fca_FUN_002EB018(char *dst, char *src, char *tail);

void LVL_9_DOBBO_FUN_00398A28(char *o0, char *o1)
{
    char b0[64];
    char b1[64];
    char b2[64];
    char *r = LVL_9_DOBBO_Ffc961fca_FUN_0031D2F8(o1);

    if (r == 0)
        return;
    LVL_9_DOBBO_Ffc961fca_FUN_002EACF0(b0, r);
    LVL_9_DOBBO_Ffc961fca_FUN_002EA470(o0 + 16, o0 + 16, r + 16);
    LVL_9_DOBBO_Ffc961fca_FUN_002EA4A0(o0 + 16, o0 + 16, o1 + 16);
    if ((*(int *)(r + 60) & 2) != 0) {
        LVL_9_DOBBO_Ffc961fca_FUN_002EACF0(b2, r + 32);
        LVL_9_DOBBO_Ffc961fca_FUN_002EAF78(b1, b2);
        LVL_9_DOBBO_Ffc961fca_FUN_002EA930(o0 + 16, o0 + 16, b1);
        LVL_9_DOBBO_Ffc961fca_FUN_002EA930(o0 + 16, o0 + 16, o1 + 192);
    } else {
        LVL_9_DOBBO_Ffc961fca_FUN_002EA930(o0 + 16, o0 + 16, b0);
    }
    LVL_9_DOBBO_Ffc961fca_FUN_002EA470(o0 + 16, o0 + 16, o1 + 16);
    LVL_9_DOBBO_Ffc961fca_FUN_002EB018(o0 + 192, b0, o0 + 192);
}
extern int LVL_9_DOBBO_Fd1c348f5_FUN_002EA110(char *p);
extern int LVL_9_DOBBO_Fd1c348f5_FUN_002EB5D0(float v);
extern int LVL_9_DOBBO_Fd1c348f5_FUN_002EA110(char *p);
extern void LVL_9_DOBBO_Fd1c348f5_FUN_0032C890(char *p);
extern int LVL_9_DOBBO_Fd1c348f5_FUN_002EB5D0(float v);

void LVL_9_DOBBO_FUN_0032FFF8(char *p)
{
    char *q = p + 32;
    int r;
    int v;
    float f;

    *(unsigned char *)(p + 8) = *(unsigned char *)(p + 8) + *(unsigned char *)(q + 8);
    *(float *)(p + 24) = *(float *)(p + 24) + *(float *)(q + 16);
    *(float *)(p + 12) = *(float *)(p + 12) * *(float *)(q + 12);
    if (*(int *)(q + 4) == 0) {
        r = LVL_9_DOBBO_Fd1c348f5_FUN_002EA110(p + 10);
        if (r != 0) {
            *(int *)(q + 4) = 1;
            *(short *)(p + 10) = 30;
            *(int *)(p + 4) = *(int *)(q + 20) | 0x7f000000;
        } else {
            f = (float)(10 - *(short *)(p + 10)) * 9.6000003814697265625000e+00f;
            v = LVL_9_DOBBO_Fd1c348f5_FUN_002EB5D0(f);
            *(int *)(p + 4) = ((v + 32) << 24) | *(int *)(q + 20);
        }
    } else {
        r = LVL_9_DOBBO_Fd1c348f5_FUN_002EA110(p + 10);
        if (r != 0) {
            LVL_9_DOBBO_Fd1c348f5_FUN_0032C890(p);
        } else {
            f = (float)*(short *)(p + 10) * 4.2333333492279052734375e+00f;
            v = LVL_9_DOBBO_Fd1c348f5_FUN_002EB5D0(f);
            *(int *)(p + 4) = (v << 24) | *(int *)(q + 20);
        }
    }
}
extern void LVL_9_DOBBO_F2d5993bb_FUN_002EA1F8(char *p, int a1, int a2);
extern void LVL_9_DOBBO_F2d5993bb_FUN_002EA248(char *p, int a1, int a2);
extern int LVL_9_DOBBO_F2d5993bb_FUN_00308278(char *p, int a1);

int LVL_9_DOBBO_FUN_00308360(char *out, int mult, int *recs)
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
                LVL_9_DOBBO_F2d5993bb_FUN_002EA1F8(p, 0, *(int *)(r + 4));
            else
                LVL_9_DOBBO_F2d5993bb_FUN_002EA248(p, v, *(int *)(r + 4));
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
    v = LVL_9_DOBBO_F2d5993bb_FUN_00308278(out + 8, off);
    *(int *)(out + 4) = v;
    *(int *)(out + 0) = off;
    return off + 8;
}
extern int LVL_9_DOBBO_F77a1e64d_FUN_0031B7B8(char *object);
extern float *LVL_9_DOBBO_F77a1e64d_FUN_0031ACF0(char *object);

int LVL_9_DOBBO_FUN_003DF418(char *object)
{
    float *p;

    if (LVL_9_DOBBO_F77a1e64d_FUN_0031B7B8(object) != 0)
        return 0;
    p = LVL_9_DOBBO_F77a1e64d_FUN_0031ACF0(object);
    if (p == 0)
        return 0;
    if (0.0f < *p)
        return 0;
    return 1;
}
extern int LVL_9_DOBBO_F77a1e64d_FUN_0031B7B8(char *object);
extern float *LVL_9_DOBBO_F77a1e64d_FUN_0031ACF0(char *object);

int LVL_9_DOBBO_FUN_003E9980(char *object)
{
    float *p;

    if (LVL_9_DOBBO_F77a1e64d_FUN_0031B7B8(object) != 0)
        return 0;
    p = LVL_9_DOBBO_F77a1e64d_FUN_0031ACF0(object);
    if (p == 0)
        return 0;
    if (0.0f < *p)
        return 0;
    return 1;
}
extern int LVL_9_DOBBO_F77a1e64d_FUN_0031B7B8(char *object);
extern float *LVL_9_DOBBO_F77a1e64d_FUN_0031ACF0(char *object);

int LVL_9_DOBBO_FUN_0042F140(char *object)
{
    float *p;

    if (LVL_9_DOBBO_F77a1e64d_FUN_0031B7B8(object) != 0)
        return 0;
    p = LVL_9_DOBBO_F77a1e64d_FUN_0031ACF0(object);
    if (p == 0)
        return 0;
    if (0.0f < *p)
        return 0;
    return 1;
}
extern void LVL_9_DOBBO_Fb342d477_D_0011AC60(int value);
extern void LVL_9_DOBBO_Fb342d477_FUN_00454E30(int value);
extern void LVL_9_DOBBO_Fb342d477_FUN_00454DC0(int value);
extern void LVL_9_DOBBO_Fb342d477_D_0011AC40(int value);

int LVL_9_DOBBO_FUN_004553F0(int *p)
{
    LVL_9_DOBBO_Fb342d477_D_0011AC60(p[16]);
    p[17] = 0;
    LVL_9_DOBBO_Fb342d477_FUN_00454E30(5);
    p[7] = *(volatile int *)0x1000B410;
    p[8] = *(volatile int *)0x1000B430;
    p[9] = *(volatile int *)0x1000B420;
    p[10] = *(volatile int *)0x1000B400;
    if (*(volatile int *)0x10002010 & 0xF0)
        while (*(volatile int *)0x10002010 & 0xF0)
            ;
    LVL_9_DOBBO_Fb342d477_FUN_00454DC0(0);
    p[11] = *(volatile int *)0x1000B010;
    p[12] = *(volatile int *)0x1000B020;
    p[13] = *(volatile int *)0x1000B000;
    p[14] = *(volatile int *)0x10002020;
    p[15] = *(volatile int *)0x10002010;
    LVL_9_DOBBO_Fb342d477_D_0011AC40(p[16]);
    return 1;
}
/* Family 0e7bb6a8908d30dc -- 244 bytes, 28 placements (1 boot + 27 levels). */

extern char *LVL_9_DOBBO_F0e7bb6a8_FUN_0031D2F8(char *a);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EACF0(char *dst, char *src);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EAF78(char *dst, char *src);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EAF78(char *dst, char *src);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EA4A0(char *dst, char *a, char *b);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EA958(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EACF0(char *dst, char *src);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_002EB068(char *a, char *b, char *c);
extern void LVL_9_DOBBO_F0e7bb6a8_FUN_0031B2D0(char *a, char *b);

int LVL_9_DOBBO_FUN_0031D840(char *unused, char *obj, char *arg2, char *arg3, char *arg4, char *arg5)
{
    char buf0[64];
    char buf1[16];
    char buf2[64];
    char *p;

    p = LVL_9_DOBBO_F0e7bb6a8_FUN_0031D2F8(obj);
    if (p == 0)
        return 0;
    if (*(int *)(p + 60) & 0x40) {
        LVL_9_DOBBO_F0e7bb6a8_FUN_002EACF0(buf0, obj + 240);
        LVL_9_DOBBO_F0e7bb6a8_FUN_002EAF78(buf0, buf0);
    } else {
        LVL_9_DOBBO_F0e7bb6a8_FUN_002EAF78(buf0, obj + 192);
    }
    LVL_9_DOBBO_F0e7bb6a8_FUN_002EA4A0(buf1, arg2, obj + 16);
    LVL_9_DOBBO_F0e7bb6a8_FUN_002EA958(arg4, buf1, buf0);
    LVL_9_DOBBO_F0e7bb6a8_FUN_002EACF0(buf2, arg3);
    LVL_9_DOBBO_F0e7bb6a8_FUN_002EB068(buf2, buf0, buf2);
    LVL_9_DOBBO_F0e7bb6a8_FUN_0031B2D0(buf2, arg5);
    return 1;
}
extern void LVL_9_DOBBO_Fa2a84657_FUN_002EA510(float *tmp, char *v, float k);
extern void LVL_9_DOBBO_Fa2a84657_FUN_002EA488(float *tmp, char *a, char *v);
extern int LVL_9_DOBBO_Fa2a84657_FUN_002EA110(char *field);
extern void LVL_9_DOBBO_Fa2a84657_FUN_0032C890(unsigned char *p);

void LVL_9_DOBBO_FUN_00335DB0(unsigned char *p)
{
    float tmp[4];
    char *v;
    int n;

    v = (char *)p + 32;
    LVL_9_DOBBO_Fa2a84657_FUN_002EA510(tmp, v, 9.4999998807907104492188e-01f);
    *(float *)(v + 8) = *(float *)(v + 8) + 1.3888889225199818611145e-03f;
    LVL_9_DOBBO_Fa2a84657_FUN_002EA488(tmp, (char *)p + 16, v);

    n = *(int *)(p + 4) + (int)0xFE000000;
    *(int *)(p + 4) = n;
    if (((unsigned)(n & 0xFF000000) - 1) > 0x5EFFFFFFu || LVL_9_DOBBO_Fa2a84657_FUN_002EA110((char *)p + 10) != 0) {
        LVL_9_DOBBO_Fa2a84657_FUN_0032C890(p);
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
extern int LVL_9_DOBBO_F750245c6_D_001A7340 __attribute__((sda));
extern void LVL_9_DOBBO_F750245c6_FUN_002F78B8(int a, int b, int c, int d, char *e, int f);
void LVL_9_DOBBO_FUN_00345868(int a0, char *a1)
{
    int h, k, p, m;
    k = a0 / 2 + 5;
    h = LVL_9_DOBBO_F750245c6_D_001A7340 / 2;
    m = h - k;
    p = h + k;
    LVL_9_DOBBO_F750245c6_FUN_002F78B8(m - 2, 312, p + 4, 314, a1, 0);
    LVL_9_DOBBO_F750245c6_FUN_002F78B8(m - 2, 333, p + 4, 335, a1, 0);
    LVL_9_DOBBO_F750245c6_FUN_002F78B8(m - 2, 313, m, 334, a1, 0);
    LVL_9_DOBBO_F750245c6_FUN_002F78B8(p + 2, 313, p + 4, 334, a1, 0);
}
extern float LVL_9_DOBBO_Fe617c30b_FUN_002EB5C0(int a);
extern void LVL_9_DOBBO_Fe617c30b_FUN_002EA4F8(char *p, char *q, float f);
extern void LVL_9_DOBBO_Fe617c30b_FUN_002EA470(char *p, char *q, char *r);
extern int LVL_9_DOBBO_Fe617c30b_FUN_00318EE0(int a, int b, float f);
extern int LVL_9_DOBBO_Fe617c30b_FUN_002EA110(char *p);
extern void LVL_9_DOBBO_Fe617c30b_FUN_0032C890(char *p);

void LVL_9_DOBBO_FUN_0032E678(char *a0)
{
    char *s0 = a0 + 32;
    int x;
    int u;
    int t;
    float f;

    x = *(int *)(s0 + 28);
    t = *(short *)(a0 + 10);
    u = *(short *)(s0 + 24) * (x - t) / x + *(short *)(s0 + 26);

    f = LVL_9_DOBBO_Fe617c30b_FUN_002EB5C0(u) * 1000.0f;
    *(float *)(a0 + 12) = f;
    LVL_9_DOBBO_Fe617c30b_FUN_002EA4F8(s0, s0, 9.8000001907348632812500e-01f);
    LVL_9_DOBBO_Fe617c30b_FUN_002EA470(a0 + 16, a0 + 16, s0);
    *(unsigned char *)(a0 + 8) = *(unsigned char *)(a0 + 8) + 1;
    f = LVL_9_DOBBO_Fe617c30b_FUN_002EB5C0(*(int *)(s0 + 28));
    *(int *)(a0 + 4) = LVL_9_DOBBO_Fe617c30b_FUN_00318EE0(*(int *)(s0 + 20), *(int *)(s0 + 16),
                               (float)*(short *)(a0 + 10) / f);
    if (LVL_9_DOBBO_Fe617c30b_FUN_002EA110(a0 + 10) != 0)
        LVL_9_DOBBO_Fe617c30b_FUN_0032C890(a0);
}
extern int LVL_9_DOBBO_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_9_DOBBO_F4a4e68d9_FUN_00452AE0(int);

void LVL_9_DOBBO_FUN_00308C80(void)
{
    int value = LVL_9_DOBBO_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_9_DOBBO_F4a4e68d9_FUN_00452AE0(value + 0x36F28);
}
extern int LVL_9_DOBBO_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_9_DOBBO_F4a4e68d9_FUN_00452B00(int);

void LVL_9_DOBBO_FUN_003092B0(void)
{
    int value = LVL_9_DOBBO_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_9_DOBBO_F4a4e68d9_FUN_00452B00(value + 0x36F28);
}
extern int LVL_9_DOBBO_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_9_DOBBO_F4a4e68d9_FUN_00452AA0(int);

void LVL_9_DOBBO_FUN_00309500(void)
{
    int value = LVL_9_DOBBO_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_9_DOBBO_F4a4e68d9_FUN_00452AA0(value + 0x36F28);
}
extern int LVL_9_DOBBO_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_9_DOBBO_F4a4e68d9_FUN_00452B20(int);

void LVL_9_DOBBO_FUN_00309530(void)
{
    int value = LVL_9_DOBBO_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_9_DOBBO_F4a4e68d9_FUN_00452B20(value + 0x36F28);
}
extern int LVL_9_DOBBO_F4a4e68d9_D_001A904C __attribute__((sda));
extern void LVL_9_DOBBO_F4a4e68d9_FUN_0043C6B8(int);

void LVL_9_DOBBO_FUN_0030A140(void)
{
    int value = LVL_9_DOBBO_F4a4e68d9_D_001A904C;

    if (value != 0)
        LVL_9_DOBBO_F4a4e68d9_FUN_0043C6B8(value + 0x36F28);
}
extern void LVL_9_DOBBO_F42d2147f_FUN_002EA750(char *a0, char *a1, float f);
extern void LVL_9_DOBBO_F42d2147f_FUN_002EA4F8(char *a0, char *a1, float f);
extern void LVL_9_DOBBO_F42d2147f_FUN_002EA470(char *a0, char *a1, char *a2);
extern char LVL_9_DOBBO_F42d2147f_D_00189E20[];
extern char LVL_9_DOBBO_F42d2147f_D_00189EA0[];

void LVL_9_DOBBO_FUN_004352B0(char *object)
{
    char local[16];
    char buf[16];
    char *p = LVL_9_DOBBO_F42d2147f_D_00189E20;
    unsigned char v;

    LVL_9_DOBBO_F42d2147f_FUN_002EA750(local, (char *)(*(int *)(p + 8848) + 224), 1.0f);
    v = *(unsigned char *)(p + 8884);
    if (v == 2) {
        LVL_9_DOBBO_F42d2147f_FUN_002EA4F8(buf, local, 9.5f);
    } else if (v == 1) {
        LVL_9_DOBBO_F42d2147f_FUN_002EA4F8(buf, local, 0.75f);
    } else {
        LVL_9_DOBBO_F42d2147f_FUN_002EA4F8(buf, local, 1.6f);
    }
    LVL_9_DOBBO_F42d2147f_FUN_002EA470(object + 48, LVL_9_DOBBO_F42d2147f_D_00189EA0, buf);
    LVL_9_DOBBO_F42d2147f_FUN_002EA470(object + 48, LVL_9_DOBBO_F42d2147f_D_00189EA0 + 208, object + 48);
}
extern char LVL_9_DOBBO_F93479d13_D_00189E20[];
extern void LVL_9_DOBBO_F93479d13_FUN_0030C790(char *entry);
extern void LVL_9_DOBBO_F93479d13_FUN_0030C790(char *entry);

void LVL_9_DOBBO_FUN_002B3BE0(int slot, int value)
{
    char *e;
    void (*fn)(char *);

    {
        char *p = LVL_9_DOBBO_F93479d13_D_00189E20 + slot * 80;

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
        char *q = LVL_9_DOBBO_F93479d13_D_00189E20 + slot * 80;

        e = *(char **)(q + 4640);
        *(int *)(q + 4676) = 0;
        *(int *)(q + 4680) = 0;
        if (e != 0) {
            LVL_9_DOBBO_F93479d13_FUN_0030C790(e);
            *(char **)(q + 4640) = 0;
        }
        e = *(char **)(q + 4644);
        if (e != 0 && slot != 3) {
            LVL_9_DOBBO_F93479d13_FUN_0030C790(e);
            *(char **)(q + 4644) = 0;
        }
    }
}
extern char LVL_9_DOBBO_F5fa3e1af_D_00189E20[];
extern float LVL_9_DOBBO_F5fa3e1af_FUN_002EA438(float *buf);
extern float LVL_9_DOBBO_F5fa3e1af_FUN_002EA5B8(float *buf);

void LVL_9_DOBBO_FUN_003DA6D8(char *obj)
{
    float buf[2];
    char *d = LVL_9_DOBBO_F5fa3e1af_D_00189E20;
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
    LVL_9_DOBBO_F5fa3e1af_FUN_002EA438(buf);
    buf[0] = *(float *)(e + 16);
    buf[1] = *(float *)(e + 20);
    *(float *)(e + 48) = LVL_9_DOBBO_F5fa3e1af_FUN_002EA5B8(buf);
}


extern int LVL_9_DOBBO_F9a90bcc4_FUN_00316A28(int count);

int LVL_9_DOBBO_FUN_00323EE8(u8 *owner, int b, int *outIndex,
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
    k = LVL_9_DOBBO_F9a90bcc4_FUN_00316A28(n);
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


extern void LVL_9_DOBBO_Feda2e14e_FUN_002EA470(void *out, void *in, void *src);
extern int LVL_9_DOBBO_Feda2e14e_FUN_002EA110(void *p);
extern void LVL_9_DOBBO_Feda2e14e_FUN_0032C890(void *self);
extern float LVL_9_DOBBO_Feda2e14e_FUN_002EB5C0(int n);
extern int LVL_9_DOBBO_Feda2e14e_FUN_002EB618(int handle, int previous, float ratio);
extern char LVL_9_DOBBO_Feda2e14e_D_00189E20[];

void LVL_9_DOBBO_FUN_003370C8(u8 *self)
{
    char *s2 = (char *)self + 32;
    int n;
    float a;
    float b;

    if (*(int *)(s2 + 28) == 1) {
        char *base = LVL_9_DOBBO_Feda2e14e_D_00189E20;
        *(float *)(self + 16) = *(float *)(base + 128) + *(float *)(s2 + 16);
        *(float *)(self + 20) = *(float *)(base + 132) + *(float *)(s2 + 20);
        *(float *)(self + 24) = *(float *)(base + 136) + *(float *)(s2 + 24);
        LVL_9_DOBBO_Feda2e14e_FUN_002EA470(self + 16, self + 16, s2);
        *(float *)(s2 + 16) = *(float *)(self + 16) - *(float *)(base + 128);
        *(float *)(s2 + 20) = *(float *)(self + 20) - *(float *)(base + 132);
        *(float *)(s2 + 24) = *(float *)(self + 24) - *(float *)(base + 136);
    } else {
        LVL_9_DOBBO_Feda2e14e_FUN_002EA470(self + 16, self + 16, s2);
    }

    if (*(float *)(self + 16) < 2.0f || *(float *)(self + 16) > 1021.0f
        || *(float *)(self + 20) < 2.0f || *(float *)(self + 20) > 1021.0f
        || *(float *)(self + 24) < 2.0f || *(float *)(self + 24) > 1021.0f) {
        LVL_9_DOBBO_Feda2e14e_FUN_0032C890(self);
        return;
    }

    if (LVL_9_DOBBO_Feda2e14e_FUN_002EA110((char *)self + 10) != 0) {
        LVL_9_DOBBO_Feda2e14e_FUN_0032C890(self);
        return;
    }

    n = *(int *)(self + 4) & 0xFFFFFF;
    a = LVL_9_DOBBO_Feda2e14e_FUN_002EB5C0(*(short *)(self + 10) - 1);
    b = LVL_9_DOBBO_Feda2e14e_FUN_002EB5C0(*(short *)(self + 10));
    *(int *)(self + 4) = LVL_9_DOBBO_Feda2e14e_FUN_002EB618(n, *(int *)(self + 4), a / b);
}
extern void LVL_9_DOBBO_Feb99aa89_FUN_002CF690(char *target, int mode, float value, float zero, float scale);
extern int LVL_9_DOBBO_Feb99aa89_FUN_002DA5C0(char *first, char *second, int mode, int flag, int extra);
extern float LVL_9_DOBBO_Feb99aa89_FUN_002EA630(char *source, short *table);

extern short LVL_9_DOBBO_Feb99aa89_D_001BFE60[];
extern float LVL_9_DOBBO_Feb99aa89_D_0018A084;

int LVL_9_DOBBO_FUN_002AE390(float *out, float scale, float amount)
{
    char buffer[32];

    LVL_9_DOBBO_Feb99aa89_FUN_002CF690(buffer, 1, LVL_9_DOBBO_Feb99aa89_D_0018A084 - 0.02f, 0.0f, scale);
    LVL_9_DOBBO_Feb99aa89_FUN_002CF690(buffer + 16, 1, amount, 0.0f, scale);
    if (LVL_9_DOBBO_Feb99aa89_FUN_002DA5C0(buffer, buffer + 16, 2, 0, 0)) {
        if (out)
            *out = LVL_9_DOBBO_Feb99aa89_FUN_002EA630(buffer, LVL_9_DOBBO_Feb99aa89_D_001BFE60);
        return 1;
    }
    return 0;
}
