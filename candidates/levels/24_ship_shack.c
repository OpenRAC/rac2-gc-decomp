/* Authored C for the measured level-only clear-five-fields body. */
void LVL_24_SHIP_SHACK_FUN_002D6960(int *object) {
    object[0] = 0;
    object[1] = 0;
    object[2] = 0;
    object[3] = 0;
    object[4] = 0;
}

typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;
typedef long s64;
typedef unsigned long u64;
typedef char NativeWidths[(sizeof(s32)==4 && sizeof(s64)==8 && sizeof(void *)==4)?1:-1];
extern void LVL_24_SHIP_SHACK_FUN_002F5D18(void);
void LVL_24_SHIP_SHACK_FUN_002F5DE0(u8 *object) {
 LVL_24_SHIP_SHACK_FUN_002F5D18();
 *(s32 *)(object+0x58)=210;*(s32 *)(object+0x5c)=200;
 *(s32 *)(object+0x74)=-2;*(s32 *)(object+0x78)=30;
 *(u16 *)(object+0x48)=0;*(u16 *)(object+0x4a)=0;*(s32 *)(object+0x70)=0;
}

typedef struct {u32 items[5];} NativeTable20;
extern const NativeTable20 LVL_24_SHIP_SHACK_D_001A8EB0;
u32 LVL_24_SHIP_SHACK_FUN_002F2510(s32 index) {
 NativeTable20 values=LVL_24_SHIP_SHACK_D_001A8EB0;
 return values.items[index];
}

extern s32 LVL_24_SHIP_SHACK_D_001B1B70[] __attribute__((sda));
extern u32 LVL_24_SHIP_SHACK_D_001B1B74[] __attribute__((sda));
s32 LVL_24_SHIP_SHACK_FUN_0034F218(s32 key) {
 s32 count=0;
 u32 *flags=LVL_24_SHIP_SHACK_D_001B1B74;
 s32 *keys=LVL_24_SHIP_SHACK_D_001B1B70;
 do {
  count++;
  if(*keys!=key) {flags+=2;keys+=2;continue;}
  *flags|=4;return 0;
 }while(count<5);
 return 1;
}

typedef struct {
    u8 prefix[0xc40];
    s32 selected;
    s32 index;
    u8 gap[0x1648];
    u8 *object;
} NativeResidentView;
extern NativeResidentView LVL_24_SHIP_SHACK_D_00189E20;

void LVL_24_SHIP_SHACK_FUN_002D1570(s32 selected, s32 index) {
    if (selected >= 0) {
        u8 *object = LVL_24_SHIP_SHACK_D_00189E20.object;
        s32 offset = object[0x43] * 4;
        u8 *table = *(u8 **)(object + 0x24);
        u8 *entry = *(u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_24_SHIP_SHACK_D_00189E20.selected = selected;
            LVL_24_SHIP_SHACK_D_00189E20.index = index;
        }
    }
}

typedef struct {
 u8 prefix[0x30];s32 state;u8 gap0[0x1c];u8 *current;u8 gap1[0x138];s32 bank[2];
} NativeContext;
extern NativeContext LVL_24_SHIP_SHACK_D_001BC3C0;
extern u8 LVL_24_SHIP_SHACK_D_0028AAC0[];
void LVL_24_SHIP_SHACK_FUN_0035E348(void) {
 s32 index=-1;
 if(LVL_24_SHIP_SHACK_D_001BC3C0.state==9) index=1;
 if(index!=-1) {
  LVL_24_SHIP_SHACK_D_001BC3C0.current=LVL_24_SHIP_SHACK_D_0028AAC0;
  *(s32 *)(LVL_24_SHIP_SHACK_D_0028AAC0+0x68)=LVL_24_SHIP_SHACK_D_001BC3C0.bank[index];
 }
}

u32 LVL_24_SHIP_SHACK_FUN_002AD0D0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_24_SHIP_SHACK_D_0018C0B4;
u32 LVL_24_SHIP_SHACK_FUN_002D59A0(void) {
    return LVL_24_SHIP_SHACK_D_0018C0B4;
}

extern void LVL_24_SHIP_SHACK_FUN_002D25C0(f32);
extern s32 LVL_24_SHIP_SHACK_FUN_002D5860(s32);
extern u8 LVL_24_SHIP_SHACK_D_001B8500[];
typedef struct {u8 prefix[0x190];u8 *object;} NativeObjectRefView;
extern NativeObjectRefView LVL_24_SHIP_SHACK_D_001B8580;

void LVL_24_SHIP_SHACK_FUN_002D2878(f32 value) {
 LVL_24_SHIP_SHACK_FUN_002D25C0(-value);
}

void LVL_24_SHIP_SHACK_FUN_002D5820(void) {
 LVL_24_SHIP_SHACK_FUN_002D5860(0);
}

void LVL_24_SHIP_SHACK_FUN_002D5840(void) {
 LVL_24_SHIP_SHACK_FUN_002D5860(3);
}

s32 LVL_24_SHIP_SHACK_FUN_002D5A30(s32 index) {
 return LVL_24_SHIP_SHACK_D_001B8500[index*16]!=0;
}

void LVL_24_SHIP_SHACK_FUN_002D6CF8(void) {
 *(u16 *)(LVL_24_SHIP_SHACK_D_001B8580.object+0x7e)=1;
 LVL_24_SHIP_SHACK_D_001B8580.object[0x7d]=0;
}

s32 LVL_24_SHIP_SHACK_FUN_002E2A88(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}


extern s32 LVL_24_SHIP_SHACK_D_001BE840;
extern s32 LVL_24_SHIP_SHACK_D_001BE888;
extern u32 LVL_24_SHIP_SHACK_D_001BE894;
extern void LVL_24_SHIP_SHACK_FUN_002EEBE0(void);
extern void LVL_24_SHIP_SHACK_FUN_0033C720(s64);
extern void LVL_24_SHIP_SHACK_FUN_00372790(u32,u64);

s32 LVL_24_SHIP_SHACK_FUN_002EF578(void) {
 return LVL_24_SHIP_SHACK_D_001BE840;
}

s32 LVL_24_SHIP_SHACK_FUN_002EF588(void) {
 return LVL_24_SHIP_SHACK_D_001BE888;
}

void LVL_24_SHIP_SHACK_FUN_002EF5E8(u32 value) {
 LVL_24_SHIP_SHACK_D_001BE894=value;
}

s32 LVL_24_SHIP_SHACK_FUN_002F1B80(void) {
 return LVL_24_SHIP_SHACK_D_001BE840==7;
}

void LVL_24_SHIP_SHACK_FUN_002E48B0(void) {
 LVL_24_SHIP_SHACK_FUN_002EEBE0();
}

void LVL_24_SHIP_SHACK_FUN_002E53E0(void) {
 LVL_24_SHIP_SHACK_FUN_0033C720(0);
}

void LVL_24_SHIP_SHACK_FUN_002EBB68(void) {
 LVL_24_SHIP_SHACK_FUN_00372790(0x47,0x513f1UL);
}

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_24_SHIP_SHACK_FUN_002ACF68(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_24_SHIP_SHACK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_24_SHIP_SHACK_FUN_002ACFA0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_24_SHIP_SHACK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_24_SHIP_SHACK_FUN_002ADBC0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern u8 LVL_24_SHIP_SHACK_D_0018B2BC[];
extern void LVL_24_SHIP_SHACK_FUN_002ADE68(void);
extern void LVL_24_SHIP_SHACK_FUN_002AE960(void);
extern void LVL_24_SHIP_SHACK_FUN_002ADC18(void);
extern void LVL_24_SHIP_SHACK_FUN_002A52C8(u64, s64, s64, s32);
extern void LVL_24_SHIP_SHACK_FUN_002B8D58(short, short, short);

void LVL_24_SHIP_SHACK_FUN_002A5288(void) {
 LVL_24_SHIP_SHACK_FUN_002A52C8(LVL_24_SHIP_SHACK_D_0018B2BC[1],0,1,0x32);
 LVL_24_SHIP_SHACK_FUN_002B8D58(0x16,7,0);
}

void LVL_24_SHIP_SHACK_FUN_002AECC0(void) {
 LVL_24_SHIP_SHACK_FUN_002ADE68();
 LVL_24_SHIP_SHACK_FUN_002AE960();
 LVL_24_SHIP_SHACK_FUN_002ADC18();
}

extern u8 LVL_24_SHIP_SHACK_D_0025B710[];
extern void LVL_24_SHIP_SHACK_FUN_002A4148(u8 *);

void LVL_24_SHIP_SHACK_FUN_002A4468(void) {
 u8 *p=LVL_24_SHIP_SHACK_D_0025B710;
 u8 *end=p+0x1b80;
 do {LVL_24_SHIP_SHACK_FUN_002A4148(p);p+=0xb0;}while((s32)p<(s32)end);
}

extern void LVL_24_SHIP_SHACK_FUN_002D66B8(f32, f32, f32, f32, s32, s32);

void LVL_24_SHIP_SHACK_FUN_002D6160(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_24_SHIP_SHACK_FUN_002D66B8(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_24_SHIP_SHACK_FUN_002EED68(int width, int height, int address, int mode);
extern void LVL_24_SHIP_SHACK_FUN_00372790(unsigned int reg, unsigned long value);
extern void LVL_24_SHIP_SHACK_FUN_002EF0D0(int width, int height);

void LVL_24_SHIP_SHACK_FUN_002E4828(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_24_SHIP_SHACK_FUN_002EED68(width, height, address, 1);
    LVL_24_SHIP_SHACK_FUN_00372790(0x47, 0x30000UL);
    LVL_24_SHIP_SHACK_FUN_00372790(0x42, 0x8000000044UL);
    LVL_24_SHIP_SHACK_FUN_002EF0D0(0x100, 0x100);
    LVL_24_SHIP_SHACK_FUN_00372790(0x42, 0x8000000044UL);
}

extern void LVL_24_SHIP_SHACK_FUN_002D27E0(f32, f32, f32, f32 *, s32);

void LVL_24_SHIP_SHACK_FUN_002ACDF8(f32 *output) {
 LVL_24_SHIP_SHACK_FUN_002D27E0(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_24_SHIP_SHACK_FUN_002ACFD8(s32 index) {
 s32 value=LVL_24_SHIP_SHACK_FUN_002ACFA0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_24_SHIP_SHACK_FUN_002AD010(s32 index) {
 return LVL_24_SHIP_SHACK_FUN_002ACFA0(index)==47;
}

s32 LVL_24_SHIP_SHACK_FUN_002B2348(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_24_SHIP_SHACK_FUN_003390C8(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_24_SHIP_SHACK_D_0027D600[13];
void LVL_24_SHIP_SHACK_FUN_002F5618(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_24_SHIP_SHACK_D_0027D600[i].key == key) break;
    }
    if (i < 13) {
        LVL_24_SHIP_SHACK_D_0027D600[i].fields[9] = value;
        if (LVL_24_SHIP_SHACK_D_0027D600[i].busy == 0)
            LVL_24_SHIP_SHACK_D_0027D600[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_24_SHIP_SHACK_D_001395B8[];
u32 LVL_24_SHIP_SHACK_FUN_00303C88(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_24_SHIP_SHACK_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_24_SHIP_SHACK_D_00230C40[];
s32 LVL_24_SHIP_SHACK_FUN_00375070(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_24_SHIP_SHACK_D_00230C40;
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
extern int LVL_24_SHIP_SHACK_D_0022D500[];
extern ListOverrideObject *LVL_24_SHIP_SHACK_D_00226000[];
extern ListOverridePair LVL_24_SHIP_SHACK_D_0022CF00[];
void LVL_24_SHIP_SHACK_FUN_003621A8(void)
{
    int *selected = LVL_24_SHIP_SHACK_D_0022D500;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_24_SHIP_SHACK_D_00226000[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_24_SHIP_SHACK_D_0022CF00[row->key];
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
extern NativeObjectSearchRecord32 LVL_24_SHIP_SHACK_D_00252CB0[];
s32 LVL_24_SHIP_SHACK_FUN_003DA6E8(void *object) {
    s32 result = -1;
    s32 index;
    for (index = 0; index < 30; index++) {
        if (LVL_24_SHIP_SHACK_D_00252CB0[index].field14 == object) {
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

s32 LVL_24_SHIP_SHACK_FUN_002D5860(s32 selection) {
    s32 found = 0;
    ClassFilterObject *primary = ((ClassFilterRoot *)&LVL_24_SHIP_SHACK_D_00189E20)->primary;
    ClassFilterObject *fallback = ((ClassFilterRoot *)&LVL_24_SHIP_SHACK_D_00189E20)->fallback;
    if (((ClassFilterRoot *)&LVL_24_SHIP_SHACK_D_00189E20)->mode == 0x31) {
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
extern void *LVL_24_SHIP_SHACK_D_0018C0B0;
extern void *LVL_24_SHIP_SHACK_D_0018B134;
extern void *LVL_24_SHIP_SHACK_D_0018B040;
void *LVL_24_SHIP_SHACK_FUN_002A40E8(int kind)
{
    if (kind == 0 || kind == 2 || kind == 3 || kind == 4 || kind == 5)
        return LVL_24_SHIP_SHACK_D_0018C0B0;
    if (kind == 1)
        return LVL_24_SHIP_SHACK_D_0018B134;
    if (kind == 6)
        return LVL_24_SHIP_SHACK_D_0018B040;
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
extern unsigned char LVL_24_SHIP_SHACK_D_00139568[];
extern MappedClassEntry LVL_24_SHIP_SHACK_D_00261E70[];
unsigned int LVL_24_SHIP_SHACK_FUN_002F21E8(unsigned int key)
{
    int i;
    for (i = 0; i < 56; i++) {
        MappedClassEntry *record = &LVL_24_SHIP_SHACK_D_00261E70[LVL_24_SHIP_SHACK_D_00139568[i]];
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
extern s32 LVL_24_SHIP_SHACK_FUN_002EC588(s32 value);
NativeCompactHeader *LVL_24_SHIP_SHACK_FUN_003773D0(NativeCompactHeader *header) {
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
            ((NativeCompactRow16 *)header->field18)[index].compact.field0C = LVL_24_SHIP_SHACK_FUN_002EC588(third);
            ((NativeCompactRow16 *)header->field18)[index].compact.field0E = LVL_24_SHIP_SHACK_FUN_002EC588(fourth);
            ((NativeCompactRow16 *)header->field18)[index].compact.field00 = 0;
            index++;
        } while (index < header->field06);
    }
    return header;
}

extern s32 LVL_24_SHIP_SHACK_D_001A79F0;
s32 LVL_24_SHIP_SHACK_FUN_002D57E0(void) {
    s32 found = 0;
    if (LVL_24_SHIP_SHACK_D_001A79F0 == 25 || LVL_24_SHIP_SHACK_D_001A79F0 == 5 ||
        LVL_24_SHIP_SHACK_D_001A79F0 == 10 || LVL_24_SHIP_SHACK_D_001A79F0 == 15) {
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
void LVL_24_SHIP_SHACK_FUN_002D22D0(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)&LVL_24_SHIP_SHACK_D_00189E20;
    FlagPairObjectView *primary = root->primary;
    FlagPairObjectView *secondary;
    primary->flags |= 1;
    secondary = root->secondary;
    if (secondary) secondary->flags |= 1;
}

void LVL_24_SHIP_SHACK_FUN_002D2308(void) {
    FlagPairResidentView *root = (FlagPairResidentView *)&LVL_24_SHIP_SHACK_D_00189E20;
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
void LVL_24_SHIP_SHACK_FUN_002D5C28(void) {
    ConditionalResetSlot *slot = (ConditionalResetSlot *)LVL_24_SHIP_SHACK_D_001B8500;
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
extern StateTransitionView LVL_24_SHIP_SHACK_D_001BE800;
void LVL_24_SHIP_SHACK_FUN_002F1B50(void) {
    if (LVL_24_SHIP_SHACK_D_001BE800.mode == 7 && LVL_24_SHIP_SHACK_D_001BE800.state == 1) {
        LVL_24_SHIP_SHACK_D_001BE800.state = 2;
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
extern DobboFormatRoot396 LVL_24_SHIP_SHACK_D_001C8CA0;
extern const char LVL_24_SHIP_SHACK_D_001A99E0[];
extern const char LVL_24_SHIP_SHACK_D_001A99E8[];
extern const unsigned char *LVL_24_SHIP_SHACK_FUN_002F2DA8(int);
extern void BOOT_FUN_00115DA8(char *, const char *, ...);
void LVL_24_SHIP_SHACK_FUN_00307308(int index, unsigned char *output)
{
    unsigned char temporary[80];
    const unsigned char *source = LVL_24_SHIP_SHACK_FUN_002F2DA8(LVL_24_SHIP_SHACK_D_001C8CA0.rows[index].text_id);
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
        int key = LVL_24_SHIP_SHACK_D_001C8CA0.rows[index].mapped_key;
        DobboFormatMapped396 *record = (DobboFormatMapped396 *)&LVL_24_SHIP_SHACK_D_00261E70[LVL_24_SHIP_SHACK_D_00139568[key]];
        BOOT_FUN_00115DA8((char *)temporary, LVL_24_SHIP_SHACK_D_001A99E0, record->amount);
    } else {
        BOOT_FUN_00115DA8((char *)temporary, LVL_24_SHIP_SHACK_D_001A99E8);
    }
    ++source;
    while (*p)
        *output++ = *p++;
    while (*source)
        *output++ = *source++;
    *output = 0;
}

typedef struct { u8 prefix[0xc38]; f32 plane; f32 depth; } GornFloatInterval64;

int LVL_24_SHIP_SHACK_FUN_002B2A98(f32 value)
{
    GornFloatInterval64 *root = (GornFloatInterval64 *)&LVL_24_SHIP_SHACK_D_00189E20;
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
extern int LVL_24_SHIP_SHACK_FUN_00356B70(int, unsigned int, void *);
void LVL_24_SHIP_SHACK_FUN_00418978(DobboGridState312 *state, unsigned int buttons)
{
    if (buttons & 0x1000) {
        LVL_24_SHIP_SHACK_FUN_00356B70(3, 0, 0);
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
        LVL_24_SHIP_SHACK_FUN_00356B70(3, 0, 0);
        row = state->row + 1;
        state->row = row > 1 ? 0 : row;
    } else if (buttons & 0x8000) {
        LVL_24_SHIP_SHACK_FUN_00356B70(3, 0, 0);
        --state->column;
        if (state->column < 0) {
            state->column = 3;
            state->row = 1;
            state->mode = 2;
        }
    } else if (buttons & 0x2000) {
        LVL_24_SHIP_SHACK_FUN_00356B70(3, 0, 0);
        ++state->column;
        if (state->column >= 2)
            state->column = 0;
    }
    state->index = state->column + state->row * 2;
}

extern void *LVL_24_SHIP_SHACK_D_001B1700[16];
extern u32 LVL_24_SHIP_SHACK_D_001B1740[16];

int LVL_24_SHIP_SHACK_FUN_0030F650(void *object)
{
    int index;
    for (index = 0; index < 16; ++index) {
        if (LVL_24_SHIP_SHACK_D_001B1700[index] == 0 ||
            LVL_24_SHIP_SHACK_D_001B1700[index] == object) {
            LVL_24_SHIP_SHACK_D_001B1700[index] = object;
            LVL_24_SHIP_SHACK_D_001B1740[index] = 0;
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
extern void LVL_24_SHIP_SHACK_FUN_002DBAE8(OozlaAppendObject164 *, int, const float *);
extern void LVL_24_SHIP_SHACK_FUN_002EC878(float *, const float *, const float *);
extern void LVL_24_SHIP_SHACK_FUN_002ECCF0(float *, const float *, const float *);
extern void LVL_24_SHIP_SHACK_FUN_002DBE30(OozlaAppendObject164 *, float *, const float *);
unsigned int LVL_24_SHIP_SHACK_FUN_002DBA40(OozlaAppendObject164 *object, unsigned int index, const float *direction)
{
    float difference[4];
    OozlaAppendDescriptor164 *descriptor = object->descriptor;
    descriptor->indices[descriptor->count] = index;
    ++descriptor->count;
    LVL_24_SHIP_SHACK_FUN_002DBAE8(object, (int)descriptor->count - 1, direction);
    if (descriptor->count != 1) {
        LVL_24_SHIP_SHACK_FUN_002EC878(difference, descriptor->points[descriptor->indices[0]], descriptor->points[descriptor->indices[1]]);
        LVL_24_SHIP_SHACK_FUN_002ECCF0(difference, difference, &object->transform[0][0]);
        LVL_24_SHIP_SHACK_FUN_002DBE30(object, descriptor->plane, difference);
    }
    return descriptor->count;
}

typedef struct { f32 x, y, z, w; } JammingParameterRow;
typedef struct {
    u8 prefix[0x30];
    JammingParameterRow rows[4];
    f32 weights[3];
} JammingParameterRows;
void LVL_24_SHIP_SHACK_FUN_0041DDF0(f32 weight, f32 x, f32 y, f32 z, f32 w, JammingParameterRows *object, s32 index) {
    object->rows[index].x = x;
    object->rows[index].y = y;
    object->rows[index].z = z;
    object->rows[index].w = w;
    object->weights[index] = weight;
}

typedef struct { f32 x, y, z, w; } JammingGridRow;
typedef struct { JammingGridRow rows[3]; } JammingGridGroup;
typedef struct { u8 prefix[0x2c]; JammingGridGroup groups[2]; } JammingGridObject;
void LVL_24_SHIP_SHACK_FUN_0041E288(f32 x, f32 y, f32 z, f32 w, JammingGridObject *object, s32 row, s32 group) {
    object->groups[group].rows[row].x = x;
    object->groups[group].rows[row].y = y;
    object->groups[group].rows[row].z = z;
    object->groups[group].rows[row].w = w;
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 LVL_24_SHIP_SHACK_D_001A63A8[];
extern void FUN_00133400(s32);
s32 LVL_24_SHIP_SHACK_FUN_00325138(void) {
    if (((CallState *)LVL_24_SHIP_SHACK_D_001A63A8)->active==0) return 0;
    if (((CallState *)LVL_24_SHIP_SHACK_D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)LVL_24_SHIP_SHACK_D_001A63A8)->active); ((CallState *)LVL_24_SHIP_SHACK_D_001A63A8)->state=4;
    return 1;
}

f32 LVL_24_SHIP_SHACK_FUN_00318948(f32 value, s32 count) {
    f32 factor = 1.0f - value;
    f32 product = factor;
    for (; count > 1; --count)
        product = product * factor;
    return 1.0f - product;
}

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 LVL_24_SHIP_SHACK_FUN_00303C08(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (LVL_24_SHIP_SHACK_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        LVL_24_SHIP_SHACK_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

extern unsigned char D_19B278[];

int LVL_24_SHIP_SHACK_FUN_003224F8(void)
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

extern void LVL_24_SHIP_SHACK_FUN_0030E798(NativeUpdate775View *object);

void LVL_24_SHIP_SHACK_FUN_00393B80(NativeUpdate775View *object)
{
    object->field2C *= 1.025f;
    object->field23 -= 3;
    if (object->field23 < 4)
        LVL_24_SHIP_SHACK_FUN_0030E798(object);
}


/* One fixed ordinary scalar source hypothesis. Byte offset and mask are
   measured; no original object/class/field name or allocation claim. The
   observed callers pass scalar one and ignore the result. */
void LVL_24_SHIP_SHACK_FUN_00323260(unsigned char *entity, int enabled) {
    if (enabled)
        entity[0xBE] |= 4;
    else
        entity[0xBE] &= 0xFB;
}


/* One fixed ordinary scalar query. Signed halfword width, offset and
   equality constant are measured; no original class or field-name claim. */
int LVL_24_SHIP_SHACK_FUN_0031CEA8(const unsigned char *entity) {
    return *(const signed short *)(entity + 0xAA) == 0x0CDB;
}


void LVL_24_SHIP_SHACK_FUN_002A61F0(void)
{
}


unsigned int LVL_24_SHIP_SHACK_FUN_002D5930(void)
{
    return 0;
}


void LVL_24_SHIP_SHACK_FUN_002EBB90(void)
{
}


void LVL_24_SHIP_SHACK_FUN_002F2B40(void)
{
}


void LVL_24_SHIP_SHACK_FUN_002F38F0(void)
{
}


void LVL_24_SHIP_SHACK_FUN_002FADB0(void)
{
}


void LVL_24_SHIP_SHACK_FUN_002FADB8(void)
{
}


void LVL_24_SHIP_SHACK_FUN_002FE968(void)
{
}


void LVL_24_SHIP_SHACK_FUN_003076C0(void)
{
}


unsigned int LVL_24_SHIP_SHACK_FUN_00366778(void)
{
    return 0;
}


void LVL_24_SHIP_SHACK_FUN_0036B1C8(void)
{
}


void LVL_24_SHIP_SHACK_FUN_003721E0(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00375B20(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00379200(void)
{
}


void LVL_24_SHIP_SHACK_FUN_003D72C8(void)
{
}


void LVL_24_SHIP_SHACK_FUN_003F4820(void)
{
}


void LVL_24_SHIP_SHACK_FUN_0040B5B0(void)
{
}


void LVL_24_SHIP_SHACK_FUN_0040D020(void)
{
}


void LVL_24_SHIP_SHACK_FUN_0040D270(void)
{
}


void LVL_24_SHIP_SHACK_FUN_0040D768(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00415FE0(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00416C38(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00422A40(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00424DD0(void)
{
}


void LVL_24_SHIP_SHACK_FUN_00426668(void)
{
}
void LVL_24_SHIP_SHACK_FUN_003A77C0(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003AF918(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003B9D30(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003C0BD8(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003C4180(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003CA460(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003D2CD0(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003D4078(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003F2A20(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003F89B0(char *p)
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

extern u8 LVL_24_SHIP_SHACK_F62e6ff2b_D_00189E20[];
extern u8 LVL_24_SHIP_SHACK_F62e6ff2b_D_00188660[];

typedef struct {
    u8 pad0[4640];
    u32 field4640;
    u8 gap[4204];
    u32 field8848;
} NativeView;

s32 LVL_24_SHIP_SHACK_FUN_00356768(s32 id)
{
    u8 *record;
    u8 *cursor;
    s32 limit;

    limit = 52;
    if (id == 0 || (((NativeView *)&LVL_24_SHIP_SHACK_F62e6ff2b_D_00189E20)->field8848 != (u32)id &&
                    ((NativeView *)&LVL_24_SHIP_SHACK_F62e6ff2b_D_00189E20)->field4640 != (u32)id))
        limit = 42;

    id = 0;
    if (limit != 0) {
        record = (u8 *)&LVL_24_SHIP_SHACK_F62e6ff2b_D_00188660;
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

extern u8 LVL_24_SHIP_SHACK_F1c0a2bbf_D_001ADD18[];
extern u8 LVL_24_SHIP_SHACK_F1c0a2bbf_D_001ADD38[];
extern void LVL_24_SHIP_SHACK_F1c0a2bbf_FUN_00115E38(void *, s32, void *);

void LVL_24_SHIP_SHACK_FUN_0040B5C8(u32 *p, u32 a1, u32 a2, u32 a3)
{
    if (a1 < 4u)
        LVL_24_SHIP_SHACK_F1c0a2bbf_FUN_00115E38(LVL_24_SHIP_SHACK_F1c0a2bbf_D_001ADD18, 37, LVL_24_SHIP_SHACK_F1c0a2bbf_D_001ADD38);
    p[0] = a2;
    p[1] = a3;
    p[2] = a1;
    p[5] = 0;
    p[3] = 0;
    p[4] = 0;
}
void LVL_24_SHIP_SHACK_FUN_003979E8(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003CCFC0(char *p)
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
extern Blob LVL_24_SHIP_SHACK_F6894d7c1_D_001A8E60;
int LVL_24_SHIP_SHACK_FUN_002F1E18(int x)
{
    Blob b;
    int i;
    b = LVL_24_SHIP_SHACK_F6894d7c1_D_001A8E60;
    for (i = 0; b.v[i]; i++)
        if (x == b.v[i])
            return 1;
    return 0;
}
extern int LVL_24_SHIP_SHACK_Fafab4c55_FUN_00115DA8(char *, char *, ...);
extern char LVL_24_SHIP_SHACK_Fafab4c55_D_001AD5F0[];
extern char LVL_24_SHIP_SHACK_Fafab4c55_D_001AD600[];
extern char LVL_24_SHIP_SHACK_Fafab4c55_D_001AD608[];

void LVL_24_SHIP_SHACK_FUN_0036CBC8(char *dst, int value)
{
    if (value > 999999)
        LVL_24_SHIP_SHACK_Fafab4c55_FUN_00115DA8(dst, LVL_24_SHIP_SHACK_Fafab4c55_D_001AD5F0, value / 1000000, (value / 1000) % 1000, value % 1000);
    else if (value >= 1000)
        LVL_24_SHIP_SHACK_Fafab4c55_FUN_00115DA8(dst, LVL_24_SHIP_SHACK_Fafab4c55_D_001AD600, value / 1000, value % 1000);
    else
        LVL_24_SHIP_SHACK_Fafab4c55_FUN_00115DA8(dst, LVL_24_SHIP_SHACK_Fafab4c55_D_001AD608, value);
}
typedef struct {
    char *base;
    unsigned limit;
    unsigned size;
    unsigned cur;
    int count;
    void *free;
} Hdr;

extern int LVL_24_SHIP_SHACK_Fe77c6258_FUN_00115E38(char *, int, char *);
extern char LVL_24_SHIP_SHACK_Fe77c6258_D_001ADD18[];
extern char LVL_24_SHIP_SHACK_Fe77c6258_D_001ADD60[];

void *LVL_24_SHIP_SHACK_FUN_0040B650(Hdr *p)
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
        LVL_24_SHIP_SHACK_Fe77c6258_FUN_00115E38(LVL_24_SHIP_SHACK_Fe77c6258_D_001ADD18, 83, LVL_24_SHIP_SHACK_Fe77c6258_D_001ADD60);
        return 0;
    }
    r = p->base + cur;
    p->cur = cur + p->size;
    p->count++;
    return r;
}
void LVL_24_SHIP_SHACK_FUN_0037E480(char *p)
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
void LVL_24_SHIP_SHACK_FUN_00380B08(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003FB0B0(char *p)
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

extern Entry *LVL_24_SHIP_SHACK_Fc68ad20a_D_0018C2B8;

int LVL_24_SHIP_SHACK_FUN_002D1078(int key, int *out)
{
    Entry *e = LVL_24_SHIP_SHACK_Fc68ad20a_D_0018C2B8;
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

extern Blk LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8;
extern short LVL_24_SHIP_SHACK_F55a1acb8_D_001A63AC;
extern int LVL_24_SHIP_SHACK_F55a1acb8_FUN_00133688(void);
extern void LVL_24_SHIP_SHACK_F55a1acb8_FUN_0011AEA0(int);

void LVL_24_SHIP_SHACK_FUN_00326198(int x)
{
    unsigned char c;
    int q;
    int arg;

    if (x != 1)
        return;

    if (LVL_24_SHIP_SHACK_F55a1acb8_FUN_00133688()) {
        LVL_24_SHIP_SHACK_F55a1acb8_D_001A63AC = 2;
        return;
    }

    LVL_24_SHIP_SHACK_F55a1acb8_FUN_0011AEA0(0);

    c = LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f6;
    q = LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f18;
    LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f4 = 0;
    arg = c < 1;
    LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f6 = 0;

    if (q != 0) {
        int cb = LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f1C;
        LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f18 = 0;
        LVL_24_SHIP_SHACK_F55a1acb8_D_001A63A8.f1C = 0;
        ((void (*)(int, int))q)(cb, arg);
    }
}
extern char LVL_24_SHIP_SHACK_Fa2dbe766_D_00189E20[];

int LVL_24_SHIP_SHACK_FUN_003B3BC8(float f12)
{
    char *b;

    if (f12 <= 0.0f)
        goto fail;
    b = LVL_24_SHIP_SHACK_Fa2dbe766_D_00189E20;
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
void LVL_24_SHIP_SHACK_FUN_0033D998(void)
{
    *(short *)0x001AA802 = (*(unsigned char *)0x001A7BC9) ? 3 : 0;
    *(short *)0x001AA81A = (*(unsigned char *)0x001A7BCA) ? 3 : 0;
    *(short *)0x001AA832 = (*(unsigned char *)0x001A7BCB) ? 3 : 0;
    *(short *)0x001AA84A = (*(unsigned char *)0x001A7BCC) ? 3 : 0;
    *(short *)0x001AA862 = (*(unsigned char *)0x001A7BCE) ? 3 : 0;
}
void LVL_24_SHIP_SHACK_FUN_003EA980(char *object)
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
extern char LVL_24_SHIP_SHACK_Fea34650e_D_00189E20[];

void LVL_24_SHIP_SHACK_FUN_002D2168(void)
{
    char *b = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
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
        char *c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
        p = *(char **)(c + 4892);
        if (p != 0)
            *(unsigned short *)(p + 52) &= 0xFFFE;
    }

    if (1) {
        char *c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
        if (*(unsigned char *)(c + 8884) == 1) {
            c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
            p = *(char **)(c + 6244);
            if (p != 0)
                *(unsigned short *)(p + 52) &= 0xFFFE;
        }
    }

    if (1) {
        char *c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
        if (*(short *)(c + 9468) != 0 || *(int *)(c + 8860) == 31) {
            c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4880);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4884);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
            c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
            p = *(char **)(c + 4892);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }

    if (1) {
        char *c = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
        if ((*(unsigned char *)(c + 8894) != 0 && *(int *)(c + 4680) == 10) ||
            *(unsigned char *)(c + 8895) != 0) {
            char *d = LVL_24_SHIP_SHACK_Fea34650e_D_00189E20;
            p = *(char **)(d + 4640);
            if (p != 0)
                *(unsigned short *)(p + 52) |= 0x41;
        }
    }
}
void LVL_24_SHIP_SHACK_FUN_003CD698(char *p)
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
void LVL_24_SHIP_SHACK_FUN_003FB048(char *p)
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
extern char LVL_24_SHIP_SHACK_F0be97c76_D_00189E20[];

void LVL_24_SHIP_SHACK_FUN_002A6170(void)
{
    char *base = LVL_24_SHIP_SHACK_F0be97c76_D_00189E20;
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

extern struct Table1 LVL_24_SHIP_SHACK_Fee2b87d1_D_0014B540;
extern struct Table2 LVL_24_SHIP_SHACK_Fee2b87d1_D_00152CD0;

s32 LVL_24_SHIP_SHACK_FUN_00303348(s32 value) {
    s32 i = 0;
    s32 *q;
    if (LVL_24_SHIP_SHACK_Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (LVL_24_SHIP_SHACK_Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (LVL_24_SHIP_SHACK_Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (LVL_24_SHIP_SHACK_Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}
/* attempt 3: the project's existing (boot-qualified) spelling: -G0 profile plus
   an explicit `sda` attribute on the one resident word that retail addresses
   through $gp.  Used here as a control, to measure what -G8 changes. */

typedef struct __attribute__((packed)) { unsigned char mode[4]; } CdMode;

extern CdMode LVL_24_SHIP_SHACK_F1157be91_D_001A63E8;
extern unsigned char LVL_24_SHIP_SHACK_F1157be91_D_001A7900[] __attribute__((sda));
extern int LVL_24_SHIP_SHACK_F1157be91_D_001A7430[] __attribute__((sda));
extern int LVL_24_SHIP_SHACK_F1157be91_D_001A7434 __attribute__((sda));
extern int LVL_24_SHIP_SHACK_F1157be91_FUN_001334B8(int, int, int, CdMode *);
extern int LVL_24_SHIP_SHACK_F1157be91_FUN_00133230(void);
extern int LVL_24_SHIP_SHACK_F1157be91_FUN_00132028(void);

int LVL_24_SHIP_SHACK_FUN_00326020(int a0, int a1, int a2) {
    CdMode mode = LVL_24_SHIP_SHACK_F1157be91_D_001A63E8;
    mode.mode[1] = LVL_24_SHIP_SHACK_F1157be91_D_001A7900[0];
    LVL_24_SHIP_SHACK_F1157be91_D_001A7430[0] = 0;
    LVL_24_SHIP_SHACK_F1157be91_D_001A7434 = 0;
    LVL_24_SHIP_SHACK_F1157be91_FUN_001334B8(a1, a2, a0, &mode);
    LVL_24_SHIP_SHACK_F1157be91_FUN_00133230();
    LVL_24_SHIP_SHACK_F1157be91_FUN_00132028();
    return 1;
}
typedef struct {
    u8 field00[0x218]; signed short field218; u8 field21A[0x12e];
    signed short field348; u8 field34A[4]; signed short field34E;
    u8 field350[0x1147]; u8 field1497; u8 field1498[2]; u8 field149A;
    u8 field149B[0xdf9]; s32 field2294;
} NativeModeResident;
extern NativeModeResident LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20;
extern s32 LVL_24_SHIP_SHACK_F4e5bde81_D_001A8FF0 __attribute__((sda));
extern volatile u8 LVL_24_SHIP_SHACK_F4e5bde81_D_001A7B08[4] __attribute__((sda));
extern s32 LVL_24_SHIP_SHACK_F4e5bde81_D_001A8FF4 __attribute__((sda));
s32 LVL_24_SHIP_SHACK_FUN_002D2500(void) {
    s32 result = LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field348;
    if (LVL_24_SHIP_SHACK_F4e5bde81_D_001A8FF0 != 0 && LVL_24_SHIP_SHACK_F4e5bde81_D_001A8FF4 == 0) result = 2;
    if (LVL_24_SHIP_SHACK_F4e5bde81_D_001A8FF4 != 0 || LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field2294 == 59 ||
        (LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field2294 == 110 && LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field149A == 0) ||
        LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field2294 == 109 || LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field218 != 0 ||
        (LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field1497 != 0 && LVL_24_SHIP_SHACK_F4e5bde81_D_001A7B08[3] != 0 &&
         LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field2294 == 0 && LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field34E < 4)) result = 1;
    if (result == 1 && LVL_24_SHIP_SHACK_F4e5bde81_D_00189E20.field149A != 0) result = 0;
    return result;
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_abf21065e887d7a9_FallbackU32;

typedef struct Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout {
    unsigned char unknown_00[0x90];
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_90;
    Rac2Native_abf21065e887d7a9_FallbackU32 unknown_94;
    Rac2Native_abf21065e887d7a9_FallbackU32 flags_98;
} Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout;


extern Rac2Native_abf21065e887d7a9_FallbackPredicate112Layout LVL_24_SHIP_SHACK_Fabf21065e887d7a9_AT0030D210_ROLE00;

int LVL_24_SHIP_SHACK_FUN_0030D210(void)
{
    if ((LVL_24_SHIP_SHACK_Fabf21065e887d7a9_AT0030D210_ROLE00.flags_90 & 0x10000u) == 0 &&
        (LVL_24_SHIP_SHACK_Fabf21065e887d7a9_AT0030D210_ROLE00.flags_98 & 0x04000000u) != 0)
        return 0;
    if ((LVL_24_SHIP_SHACK_Fabf21065e887d7a9_AT0030D210_ROLE00.flags_90 & 0x10000u) != 0 &&
        (LVL_24_SHIP_SHACK_Fabf21065e887d7a9_AT0030D210_ROLE00.flags_98 & 0x04000000u) != 0)
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


extern void LVL_24_SHIP_SHACK_F5b4b17178a13f443_AT003202A0_ROLE00(void *object);

void LVL_24_SHIP_SHACK_FUN_003202A0(Rac2Native_5b4b17178a13f443_SmallSlotReset128Owner *owner)
{
    int index;
    if (owner->active_14c != 0) {
        for (index = 0; index < owner->count_148; ++index) {
            if (owner->slots_120[index] != 0) {
                LVL_24_SHIP_SHACK_F5b4b17178a13f443_AT003202A0_ROLE00(owner->slots_120[index]);
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


extern Rac2Native_acdcf1600d770d3b_ScalarConfigure116State LVL_24_SHIP_SHACK_Facdcf1600d770d3b_AT00356EA8_ROLE00[];

void LVL_24_SHIP_SHACK_FUN_00356EA8(unsigned int value, unsigned int selector, int other,
                        unsigned int axis, unsigned int mode)
{
    Rac2Native_acdcf1600d770d3b_ScalarConfigure116State *state = LVL_24_SHIP_SHACK_Facdcf1600d770d3b_AT00356EA8_ROLE00;
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


extern void LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT0037D5E0_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT0037D5E0_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_24_SHIP_SHACK_FUN_0037D5E0(void *object)
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
    LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT0037D5E0_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT0037D5E0_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT00380088_ROLE01(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);
extern void LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT00380088_ROLE00(Rac2Native_b1b523716b470b36_FamilyNative192Vector *out,
    const Rac2Native_b1b523716b470b36_FamilyNative192Vector *first, const Rac2Native_b1b523716b470b36_FamilyNative192Vector *second);

void LVL_24_SHIP_SHACK_FUN_00380088(void *object)
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
    LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT00380088_ROLE01(&output,
        (const Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(source + 0x10), &input);
    LVL_24_SHIP_SHACK_Fb1b523716b470b36_AT00380088_ROLE00((Rac2Native_b1b523716b470b36_FamilyNative192Vector *)(base + 0x10), &output,
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


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT0038E1E0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_0038E1E0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT0038E1E0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT00392CC0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_00392CC0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT00392CC0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef struct Rac2Native_86f665335d9cb905_AnalysisOwner {
    unsigned char prefix_00[0x68];
    void *context_68;
} Rac2Native_86f665335d9cb905_AnalysisOwner;


extern void LVL_24_SHIP_SHACK_F86f665335d9cb905_AT003AE4A8_ROLE00(Rac2Native_86f665335d9cb905_AnalysisOwner *owner, void *context);

void LVL_24_SHIP_SHACK_FUN_003AE4A8(Rac2Native_86f665335d9cb905_AnalysisOwner *owner)
{
    LVL_24_SHIP_SHACK_F86f665335d9cb905_AT003AE4A8_ROLE00(owner, owner->context_68);
}


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003B1490_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_003B1490(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003B1490_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003B6B08_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_003B6B08(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003B6B08_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003CDAC0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_003CDAC0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003CDAC0_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003D0B98_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_003D0B98(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003D0B98_ROLE00(1.0f, context, context->vector_10, 0);
    }
}


extern void LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003DF9F0_ROLE00(float scale, Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context,
                                    float *vector, int mode);

void LVL_24_SHIP_SHACK_FUN_003DF9F0(Rac2Native_9cdc323a4d0c2fbd_AnalysisOwner *owner)
{
    Rac2Native_9cdc323a4d0c2fbd_AnalysisConditionalContext *context = owner->context_68;
    if (context->active_24 != 0) {
        LVL_24_SHIP_SHACK_F9cdc323a4d0c2fbd_AT003DF9F0_ROLE00(1.0f, context, context->vector_10, 0);
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


extern Rac2Native_6af85cabb56d3b41_SmallConditional52Global LVL_24_SHIP_SHACK_F6af85cabb56d3b41_AT00409138_ROLE00;

void LVL_24_SHIP_SHACK_FUN_00409138(Rac2Native_6af85cabb56d3b41_SmallConditional52Owner *owner)
{
    if (LVL_24_SHIP_SHACK_F6af85cabb56d3b41_AT00409138_ROLE00.identity_24a0 != owner->identity_86 &&
        LVL_24_SHIP_SHACK_F6af85cabb56d3b41_AT00409138_ROLE00.mode_2294 != 6) {
        owner->value_7d = 0;
        owner->value_7e = 3;
    }
}


void LVL_24_SHIP_SHACK_FUN_0040A320(float factor, void *context,
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


void LVL_24_SHIP_SHACK_FUN_0040A4D0(float factor,
                                     const Rac2Native_81be3b52930eb806_FallbackInterleavedFloat4Layout *source,
                                     float *destination)
{
    float complement = 1.0f - factor;
    destination[0] = complement * source->first_0 + factor * source->second_0;
    destination[1] = complement * source->first_1 + factor * source->second_1;
    destination[2] = complement * source->first_2 + factor * source->second_2;
    destination[3] = complement * source->first_3 + factor * source->second_3;
}


void LVL_24_SHIP_SHACK_FUN_0040F830(float first, float second, float **cell)
{
    (*cell)[0] = first;
    (*cell)[1] = second;
}


void LVL_24_SHIP_SHACK_FUN_00417858(float first, float second, float **cell)
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


extern unsigned char LVL_24_SHIP_SHACK_F79744baad5ad7f65_AT0041ECC8_ROLE00[];

void LVL_24_SHIP_SHACK_FUN_0041ECC8(Rac2Native_79744baad5ad7f65_ScalarChange48Owner *owner, int value)
{
    int previous = owner->value_04;
    if (previous != value && LVL_24_SHIP_SHACK_F79744baad5ad7f65_AT0041ECC8_ROLE00[0] == 0) {
        owner->previous_3fc = previous;
        owner->counter_3f8 = 180;
        owner->counter_3f4 = 300;
    }
    owner->value_04 = value;
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


void LVL_24_SHIP_SHACK_FUN_00376EE8(Rac2Native_74a5d71aacb394c6_ExtraIndexState48 *state, int index) {
    state->first[index] = 0;
    state->second[index] = 0;
    if (--state->count == 0) {
        state->current = 0;
    }
}


/* Measured partial layouts; original type and game roles are unknown. */
typedef unsigned int Rac2Native_03c444112283bc5f_u32;


extern Rac2Native_03c444112283bc5f_u32 LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[139];

void LVL_24_SHIP_SHACK_FUN_00380A30(void)
{
    int i;
    LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[138] = 5;
    LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[137] = 0;
    LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[i] = 0;
        LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_24_SHIP_SHACK_F03c444112283bc5f_AT00380A30_ROLE00[i + 128] = 0;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_24_SHIP_SHACK_FUN_002A3FF0(unsigned long param_1)
{
  return param_1;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_24_SHIP_SHACK_FUN_002D30B0(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_24_SHIP_SHACK_FUN_002DB3F0(void) {
    return 1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_24_SHIP_SHACK_FUN_002DB3F8(void) {
    return 1;
}


extern void LVL_24_SHIP_SHACK_QWEN_11a4c157d102_AT002F28C0_ROLE000(unsigned char *);

void LVL_24_SHIP_SHACK_FUN_002F28C0(void *owner)
{
    LVL_24_SHIP_SHACK_QWEN_11a4c157d102_AT002F28C0_ROLE000(owner);
}



void LVL_24_SHIP_SHACK_QWEN_407ee6f17a73_AT002F2960_ROLE001(int);
void LVL_24_SHIP_SHACK_QWEN_407ee6f17a73_AT002F2960_ROLE000(void*);

void LVL_24_SHIP_SHACK_FUN_002F2960(void *param)
{
  LVL_24_SHIP_SHACK_QWEN_407ee6f17a73_AT002F2960_ROLE001((int)param);
  *(unsigned int *)param = 0;
  LVL_24_SHIP_SHACK_QWEN_407ee6f17a73_AT002F2960_ROLE000(param);
}


void LVL_24_SHIP_SHACK_QWEN_5696fcf76f0c_AT0030FE60_ROLE000(unsigned int *dst, unsigned int val, int size);

void LVL_24_SHIP_SHACK_FUN_0030FE60(void)
{
    LVL_24_SHIP_SHACK_QWEN_5696fcf76f0c_AT0030FE60_ROLE000((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
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
extern void LVL_24_SHIP_SHACK_QWEN_523e38f49b74_AT0033DCE8_ROLE000(long);
extern void LVL_24_SHIP_SHACK_QWEN_523e38f49b74_AT0033DCE8_ROLE002(void);
extern void LVL_24_SHIP_SHACK_QWEN_523e38f49b74_AT0033DCE8_ROLE001(void);

int LVL_24_SHIP_SHACK_FUN_0033DCE8(void)
{
  LVL_24_SHIP_SHACK_QWEN_523e38f49b74_AT0033DCE8_ROLE000(0);
  LVL_24_SHIP_SHACK_QWEN_523e38f49b74_AT0033DCE8_ROLE002();
  LVL_24_SHIP_SHACK_QWEN_523e38f49b74_AT0033DCE8_ROLE001();
  return 0;
}


unsigned long long LVL_24_SHIP_SHACK_FUN_0033E0E8(void);

extern void LVL_24_SHIP_SHACK_QWEN_f47929f95774_AT0033E0E8_ROLE000(long);
extern void LVL_24_SHIP_SHACK_QWEN_f47929f95774_AT0033E0E8_ROLE002(void);
extern void LVL_24_SHIP_SHACK_QWEN_f47929f95774_AT0033E0E8_ROLE001(void);

unsigned long long LVL_24_SHIP_SHACK_FUN_0033E0E8(void)
{
  LVL_24_SHIP_SHACK_QWEN_f47929f95774_AT0033E0E8_ROLE000(0);
  LVL_24_SHIP_SHACK_QWEN_f47929f95774_AT0033E0E8_ROLE002();
  LVL_24_SHIP_SHACK_QWEN_f47929f95774_AT0033E0E8_ROLE001();
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
extern void LVL_24_SHIP_SHACK_QWEN_cb305b1f1210_AT0033E388_ROLE000(long);
extern void LVL_24_SHIP_SHACK_QWEN_cb305b1f1210_AT0033E388_ROLE002(void);
extern void LVL_24_SHIP_SHACK_QWEN_cb305b1f1210_AT0033E388_ROLE001(void);

long long LVL_24_SHIP_SHACK_FUN_0033E388(void)
{
    LVL_24_SHIP_SHACK_QWEN_cb305b1f1210_AT0033E388_ROLE000(0LL);
    LVL_24_SHIP_SHACK_QWEN_cb305b1f1210_AT0033E388_ROLE002();
    LVL_24_SHIP_SHACK_QWEN_cb305b1f1210_AT0033E388_ROLE001();
    return 0LL;
}


extern void LVL_24_SHIP_SHACK_QWEN_c23b3406f982_AT00343780_ROLE000(long);
extern void LVL_24_SHIP_SHACK_QWEN_c23b3406f982_AT00343780_ROLE002(void);
extern void LVL_24_SHIP_SHACK_QWEN_c23b3406f982_AT00343780_ROLE001(void);

int LVL_24_SHIP_SHACK_FUN_00343780(void)
{
    LVL_24_SHIP_SHACK_QWEN_c23b3406f982_AT00343780_ROLE000(0);
    LVL_24_SHIP_SHACK_QWEN_c23b3406f982_AT00343780_ROLE002();
    LVL_24_SHIP_SHACK_QWEN_c23b3406f982_AT00343780_ROLE001();
    return 0;
}


void LVL_24_SHIP_SHACK_QWEN_68cf9ebb5ee2_AT003438F0_ROLE000(long);
void LVL_24_SHIP_SHACK_QWEN_68cf9ebb5ee2_AT003438F0_ROLE001(void);
void LVL_24_SHIP_SHACK_QWEN_68cf9ebb5ee2_AT003438F0_ROLE002(void);

unsigned long long LVL_24_SHIP_SHACK_FUN_003438F0(void)
{
    LVL_24_SHIP_SHACK_QWEN_68cf9ebb5ee2_AT003438F0_ROLE000(0);
    LVL_24_SHIP_SHACK_QWEN_68cf9ebb5ee2_AT003438F0_ROLE002();
    LVL_24_SHIP_SHACK_QWEN_68cf9ebb5ee2_AT003438F0_ROLE001();
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

extern void LVL_24_SHIP_SHACK_QWEN_68f040eb20c9_AT003439A8_ROLE000(long arg0);
extern void LVL_24_SHIP_SHACK_QWEN_68f040eb20c9_AT003439A8_ROLE002(void);
extern void LVL_24_SHIP_SHACK_QWEN_68f040eb20c9_AT003439A8_ROLE001(void);

unsigned long long LVL_24_SHIP_SHACK_FUN_003439A8(void)
{
    LVL_24_SHIP_SHACK_QWEN_68f040eb20c9_AT003439A8_ROLE000(0);
    LVL_24_SHIP_SHACK_QWEN_68f040eb20c9_AT003439A8_ROLE002();
    LVL_24_SHIP_SHACK_QWEN_68f040eb20c9_AT003439A8_ROLE001();
    return 0;
}


/* External function declarations as per target specification */
extern void LVL_24_SHIP_SHACK_QWEN_92ea7c2f4a5c_AT00343D58_ROLE000(long);
extern void LVL_24_SHIP_SHACK_QWEN_92ea7c2f4a5c_AT00343D58_ROLE001(void);
extern void LVL_24_SHIP_SHACK_QWEN_92ea7c2f4a5c_AT00343D58_ROLE002(void);

/* Implementation matching decompiled output */
long LVL_24_SHIP_SHACK_FUN_00343D58(void)
{
  LVL_24_SHIP_SHACK_QWEN_92ea7c2f4a5c_AT00343D58_ROLE000(0);
  LVL_24_SHIP_SHACK_QWEN_92ea7c2f4a5c_AT00343D58_ROLE002();
  LVL_24_SHIP_SHACK_QWEN_92ea7c2f4a5c_AT00343D58_ROLE001();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void LVL_24_SHIP_SHACK_QWEN_d01c568afacc_AT00343E58_ROLE000(long param_1);
extern void LVL_24_SHIP_SHACK_QWEN_d01c568afacc_AT00343E58_ROLE001(void);
extern void LVL_24_SHIP_SHACK_QWEN_d01c568afacc_AT00343E58_ROLE002(void);

/* Candidate function: FUN_002d3390 */
unsigned long long LVL_24_SHIP_SHACK_FUN_00343E58(void)
{
    LVL_24_SHIP_SHACK_QWEN_d01c568afacc_AT00343E58_ROLE000(0);
    LVL_24_SHIP_SHACK_QWEN_d01c568afacc_AT00343E58_ROLE002();
    LVL_24_SHIP_SHACK_QWEN_d01c568afacc_AT00343E58_ROLE001();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void LVL_24_SHIP_SHACK_QWEN_093381cf82c3_AT00346008_ROLE000(long);
extern void LVL_24_SHIP_SHACK_QWEN_093381cf82c3_AT00346008_ROLE002(void);
extern void LVL_24_SHIP_SHACK_QWEN_093381cf82c3_AT00346008_ROLE001(void);

long long LVL_24_SHIP_SHACK_FUN_00346008(void)
{
    LVL_24_SHIP_SHACK_QWEN_093381cf82c3_AT00346008_ROLE000(0);
    LVL_24_SHIP_SHACK_QWEN_093381cf82c3_AT00346008_ROLE002();
    LVL_24_SHIP_SHACK_QWEN_093381cf82c3_AT00346008_ROLE001();
    return 0;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_24_SHIP_SHACK_FUN_0034FCD0(void) {
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



extern QwenRecovery_4ecc6b5a4034_ExtraResetRecord164 LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE000[100];
extern QwenRecovery_4ecc6b5a4034_u32 LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[139];

void LVL_24_SHIP_SHACK_FUN_0037DFB8(void)
{
    int i;
    for (i = 99; i >= 0; --i)
        LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE000[i].word = 0;
    LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[138] = 5;
    LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[137] = 0;
    LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[136] = 0;
    for (i = 0; i < 64; ++i) {
        LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[i] = 0;
        LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[i + 64] = 0;
    }
    for (i = 7; i >= 0; --i)
        LVL_24_SHIP_SHACK_QWEN_4ecc6b5a4034_AT0037DFB8_ROLE001[i + 128] = 0;
}



void LVL_24_SHIP_SHACK_FUN_0040B240(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void LVL_24_SHIP_SHACK_FUN_0040B248(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_24_SHIP_SHACK_FUN_0040B3A0(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


void LVL_24_SHIP_SHACK_FUN_0040B4F0(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_24_SHIP_SHACK_FUN_0040B5C0(unsigned long param_1)
{
  return param_1;
}


extern void LVL_24_SHIP_SHACK_QWEN_df8fc8ba49cf_AT0040DF88_ROLE000(unsigned char *owner, int value);

void LVL_24_SHIP_SHACK_FUN_0040DF88(unsigned char *owner, int value)
{
    LVL_24_SHIP_SHACK_QWEN_df8fc8ba49cf_AT0040DF88_ROLE000(owner + 8, value);
}



void* LVL_24_SHIP_SHACK_FUN_00412988(void* param_1);

extern void LVL_24_SHIP_SHACK_QWEN_f353c206e726_AT00412988_ROLE000(int);
extern unsigned long long LVL_24_SHIP_SHACK_QWEN_f353c206e726_AT00412988_ROLE001(unsigned long long);

void* LVL_24_SHIP_SHACK_FUN_00412988(void* param_1)
{
  LVL_24_SHIP_SHACK_QWEN_f353c206e726_AT00412988_ROLE000((int)param_1 + 8);
  LVL_24_SHIP_SHACK_QWEN_f353c206e726_AT00412988_ROLE001((int)param_1 + 0x2b0);
  return param_1;
}


extern int LVL_24_SHIP_SHACK_QWEN_f2f9288a4e34_AT00412B28_ROLE000(int);

int LVL_24_SHIP_SHACK_FUN_00412B28(int owner)
{
    int result;
    result = LVL_24_SHIP_SHACK_QWEN_f2f9288a4e34_AT00412B28_ROLE000(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int LVL_24_SHIP_SHACK_QWEN_e41cd63258f5_AT00412B60_ROLE000(unsigned char *);

int LVL_24_SHIP_SHACK_FUN_00412B60(int owner)
{
    return LVL_24_SHIP_SHACK_QWEN_e41cd63258f5_AT00412B60_ROLE000((unsigned char *)(owner + 0x2b0));
}



void LVL_24_SHIP_SHACK_QWEN_f29f80950ce7_AT00416280_ROLE000(int);

void LVL_24_SHIP_SHACK_FUN_00416280(int param_1)
{
  LVL_24_SHIP_SHACK_QWEN_f29f80950ce7_AT00416280_ROLE000(param_1 + 0x228);
  return;
}


extern void LVL_24_SHIP_SHACK_QWEN_bf824305b9f7_AT00416B98_ROLE000(unsigned char *owner, float *records);

void LVL_24_SHIP_SHACK_FUN_00416B98(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    LVL_24_SHIP_SHACK_QWEN_bf824305b9f7_AT00416B98_ROLE000(owner + 0x188, records);
}



void LVL_24_SHIP_SHACK_QWEN_61faeca45963_AT00416E60_ROLE000(int param_1);

void LVL_24_SHIP_SHACK_FUN_00416E60(int param_1)
{
  LVL_24_SHIP_SHACK_QWEN_61faeca45963_AT00416E60_ROLE000(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_24_SHIP_SHACK_FUN_00416FC8(unsigned long param_1)
{
  return param_1;
}


extern int LVL_24_SHIP_SHACK_QWEN_6add11b33414_AT00417D78_ROLE000(unsigned char *owner);

int LVL_24_SHIP_SHACK_FUN_00417D78(unsigned char *owner)
{
    return LVL_24_SHIP_SHACK_QWEN_6add11b33414_AT00417D78_ROLE000(owner + 0x298);
}



extern void LVL_24_SHIP_SHACK_QWEN_de961518de48_AT00417D98_ROLE000(unsigned char *owner);

void LVL_24_SHIP_SHACK_FUN_00417D98(unsigned char *owner)
{
    LVL_24_SHIP_SHACK_QWEN_de961518de48_AT00417D98_ROLE000(owner + 0x298);
}



extern unsigned long long LVL_24_SHIP_SHACK_QWEN_7ba3cdeeb16b_AT0041C6B8_ROLE000(unsigned long long);

unsigned long long LVL_24_SHIP_SHACK_FUN_0041C6B8(unsigned long long value)
{
    LVL_24_SHIP_SHACK_QWEN_7ba3cdeeb16b_AT0041C6B8_ROLE000(value);
    return value;
}



void LVL_24_SHIP_SHACK_FUN_0041C900(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_24_SHIP_SHACK_FUN_0041D960(unsigned long param_1)
{
  return param_1;
}


void LVL_24_SHIP_SHACK_FUN_0041DCA0(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void LVL_24_SHIP_SHACK_FUN_0041DE40(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void LVL_24_SHIP_SHACK_FUN_0041DE88(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long LVL_24_SHIP_SHACK_FUN_00422DE0(unsigned long param_1)
{
  return param_1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int LVL_24_SHIP_SHACK_FUN_00424ED0(void) {
    return 1;
}


extern int LVL_24_SHIP_SHACK_QWEN_cc83cb329fcb_AT00426710_ROLE000(unsigned char *);

int LVL_24_SHIP_SHACK_FUN_00426710(int *owner)
{
    int result;
    long status;
    status = LVL_24_SHIP_SHACK_QWEN_cc83cb329fcb_AT00426710_ROLE000((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}

