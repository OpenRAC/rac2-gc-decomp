typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_20_YEEDIL_D_001A8EB0;
void LVL_20_YEEDIL_FUN_002EB8E0(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_20_YEEDIL_FUN_00307F48(s32 index) {
    NativeTable20 values=LVL_20_YEEDIL_D_001A8EB0;
    return values.items[index];
}

u32 LVL_20_YEEDIL_FUN_002C0D60(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_20_YEEDIL_D_0018C0B4;
u32 LVL_20_YEEDIL_FUN_002EA888(void) {
    return LVL_20_YEEDIL_D_0018C0B4;
}

s32 LVL_20_YEEDIL_FUN_002F7A70(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_20_YEEDIL_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_20_YEEDIL_FUN_002C0BF8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_20_YEEDIL_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_20_YEEDIL_FUN_002C0C30(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_20_YEEDIL_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_20_YEEDIL_FUN_002C1850(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_20_YEEDIL_FUN_002EB638(f32, f32, f32, f32, s32, s32);

void LVL_20_YEEDIL_FUN_002EB0E0(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_20_YEEDIL_FUN_002EB638(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_20_YEEDIL_FUN_003047A0(int width, int height, int address, int mode);
extern void LVL_20_YEEDIL_FUN_003913B0(unsigned int reg, unsigned long value);
extern void LVL_20_YEEDIL_FUN_00304B08(int width, int height);

void LVL_20_YEEDIL_FUN_002F9810(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_20_YEEDIL_FUN_003047A0(width, height, address, 1);
    LVL_20_YEEDIL_FUN_003913B0(0x47, 0x30000UL);
    LVL_20_YEEDIL_FUN_003913B0(0x42, 0x8000000044UL);
    LVL_20_YEEDIL_FUN_00304B08(0x100, 0x100);
    LVL_20_YEEDIL_FUN_003913B0(0x42, 0x8000000044UL);
}

s32 LVL_20_YEEDIL_FUN_002C0C68(s32 index) {
 s32 value=LVL_20_YEEDIL_FUN_002C0C30(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_20_YEEDIL_FUN_002C0CA0(s32 index) {
 return LVL_20_YEEDIL_FUN_002C0C30(index)==47;
}

s32 LVL_20_YEEDIL_FUN_002C6340(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

typedef struct {
    u32 unknown;
    s32 data_offset;
    s32 length;
    s32 extra;
} PackedHeaderCountView;

s32 LVL_20_YEEDIL_FUN_00393950(const PackedHeaderCountView *header) {
    s32 fixed = header->extra + 16;
    s32 total = header->data_offset + fixed + header->length;
    return ((total + 15) / 16) * 4;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_20_YEEDIL_FUN_00357838(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_20_YEEDIL_D_00289100[13];
void LVL_20_YEEDIL_FUN_0030B050(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_20_YEEDIL_D_00289100[i].key == key) break;
    }
    if (i < 13) {
        LVL_20_YEEDIL_D_00289100[i].fields[9] = value;
        if (LVL_20_YEEDIL_D_00289100[i].busy == 0)
            LVL_20_YEEDIL_D_00289100[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_20_YEEDIL_D_001395B8[];
u32 LVL_20_YEEDIL_FUN_00319E50(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_20_YEEDIL_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_20_YEEDIL_D_00232540[];
s32 LVL_20_YEEDIL_FUN_00395F18(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_20_YEEDIL_D_00232540;
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
extern int LVL_20_YEEDIL_D_0022EE00[];
extern ListOverrideObject *LVL_20_YEEDIL_D_00227900[];
extern ListOverridePair LVL_20_YEEDIL_D_0022E800[];
void LVL_20_YEEDIL_FUN_00380CB8(void)
{
    int *selected = LVL_20_YEEDIL_D_0022EE00;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_20_YEEDIL_D_00227900[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_20_YEEDIL_D_0022E800[row->key];
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
extern NativeObjectSearchRecord32 LVL_20_YEEDIL_D_00256740[];
s32 LVL_20_YEEDIL_FUN_0041BBE8(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_20_YEEDIL_D_00256740[index].field14 == object) {
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

s32 LVL_20_YEEDIL_FUN_002EA720(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)LVL_20_YEEDIL_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)LVL_20_YEEDIL_D_00189E20)->fallback;
    if (((ClassFilterRoot *)LVL_20_YEEDIL_D_00189E20)->mode == 0x31) {
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
extern void *LVL_20_YEEDIL_D_0018C0B0;
extern void *LVL_20_YEEDIL_D_0018B134;
extern void *LVL_20_YEEDIL_D_0018B040;
void *LVL_20_YEEDIL_FUN_002B7C00(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_20_YEEDIL_D_0018C0B0;
    if (kind == 1)
        return LVL_20_YEEDIL_D_0018B134;
    if (kind == 6)
        return LVL_20_YEEDIL_D_0018B040;
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
extern unsigned char LVL_20_YEEDIL_D_00139568[];
extern MappedClassEntry LVL_20_YEEDIL_D_0026D970[];
unsigned int LVL_20_YEEDIL_FUN_00307C20(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_20_YEEDIL_D_0026D970[LVL_20_YEEDIL_D_00139568[i]];
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
extern s32 LVL_20_YEEDIL_FUN_00301FA0(s32 value);
NativeCompactHeader *LVL_20_YEEDIL_FUN_003982B0(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_20_YEEDIL_FUN_00301FA0(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_20_YEEDIL_FUN_00301FA0(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_20_YEEDIL_D_001A79F0;
s32 LVL_20_YEEDIL_FUN_002EA6A0(void) {
    s32 found = 0;
    if (LVL_20_YEEDIL_D_001A79F0 == 25 || LVL_20_YEEDIL_D_001A79F0 == 5 ||
        LVL_20_YEEDIL_D_001A79F0 == 10 || LVL_20_YEEDIL_D_001A79F0 == 15) {
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
void LVL_20_YEEDIL_FUN_002E70F8(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_20_YEEDIL_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_20_YEEDIL_FUN_002E7130(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)LVL_20_YEEDIL_D_00189E20;
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
extern ConditionalResetSlot LVL_20_YEEDIL_D_001B9E00[8];
void LVL_20_YEEDIL_FUN_002EAB10(void) {
    ConditionalResetSlot *slot = LVL_20_YEEDIL_D_001B9E00;
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
extern StateTransitionView LVL_20_YEEDIL_D_001C0100;
void LVL_20_YEEDIL_FUN_00307588(void) {
    if (LVL_20_YEEDIL_D_001C0100.mode == 7 && LVL_20_YEEDIL_D_001C0100.state == 1) {
        LVL_20_YEEDIL_D_001C0100.state = 2;
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
extern void LVL_20_YEEDIL_FUN_00338830(void *, void *, int, int);
extern void LVL_20_YEEDIL_FUN_0041CB08(void *, void *);
void LVL_20_YEEDIL_FUN_0041E7E8(DobboPartsObject252 *object)
{
    DobboPartsDescriptor252 *descriptor = object->descriptor;
    DobboPartsObject252 *parent = descriptor->parent;
    int i;
    if (!parent || parent->class_code != 0xd0e || parent->status == 0xfe || parent->status == 0xfd)
        return;
    for (i = 0; i < 2; ++i) {
        if (descriptor->parts[i]) {
            LVL_20_YEEDIL_FUN_00338830(descriptor->parent, descriptor->parts[i], i, i == 1 ? 6 : 0);
            if (object->disabled) {
                descriptor->parts[i]->flags &= ~1U;
                descriptor->parts[i]->disabled = 1;
            } else {
                descriptor->parts[i]->flags |= 1;
                descriptor->parts[i]->disabled = 0;
            }
            LVL_20_YEEDIL_FUN_0041CB08(object, descriptor->parts[i]);
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
extern DobboFormatRoot396 LVL_20_YEEDIL_D_001CA5A0;
extern const char LVL_20_YEEDIL_D_001A9A60[];
extern const char LVL_20_YEEDIL_D_001A9A68[];
extern const unsigned char *LVL_20_YEEDIL_FUN_003087E0(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_20_YEEDIL_FUN_0031D4D0(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_20_YEEDIL_FUN_003087E0(LVL_20_YEEDIL_D_001CA5A0.rows[index].text_id);
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
        int key = LVL_20_YEEDIL_D_001CA5A0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_20_YEEDIL_D_0026D970[LVL_20_YEEDIL_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_20_YEEDIL_D_001A9A60, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_20_YEEDIL_D_001A9A68);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}
