typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_16_SNIVELAK_D_001A8EB0;
void LVL_16_SNIVELAK_FUN_002D0B60(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_16_SNIVELAK_FUN_002ED1C8(s32 index) {
    NativeTable20 values=LVL_16_SNIVELAK_D_001A8EB0;
    return values.items[index];
}

u32 LVL_16_SNIVELAK_FUN_002AE4B0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_16_SNIVELAK_D_0018C0B4;
u32 LVL_16_SNIVELAK_FUN_002CFB08(void) {
    return LVL_16_SNIVELAK_D_0018C0B4;
}

s32 LVL_16_SNIVELAK_FUN_002DCD48(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_16_SNIVELAK_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_16_SNIVELAK_FUN_002AE348(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_16_SNIVELAK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_16_SNIVELAK_FUN_002AE380(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_16_SNIVELAK_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_16_SNIVELAK_FUN_002AEFA0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_16_SNIVELAK_FUN_002D08B8(f32, f32, f32, f32, s32, s32);

void LVL_16_SNIVELAK_FUN_002D0360(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_16_SNIVELAK_FUN_002D08B8(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_16_SNIVELAK_FUN_002E9A20(int width, int height, int address, int mode);
extern void LVL_16_SNIVELAK_FUN_003731D0(unsigned int reg, unsigned long value);
extern void LVL_16_SNIVELAK_FUN_002E9D88(int width, int height);

void LVL_16_SNIVELAK_FUN_002DEAE8(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_16_SNIVELAK_FUN_002E9A20(width, height, address, 1);
    LVL_16_SNIVELAK_FUN_003731D0(0x47, 0x30000UL);
    LVL_16_SNIVELAK_FUN_003731D0(0x42, 0x8000000044UL);
    LVL_16_SNIVELAK_FUN_002E9D88(0x100, 0x100);
    LVL_16_SNIVELAK_FUN_003731D0(0x42, 0x8000000044UL);
}

s32 LVL_16_SNIVELAK_FUN_002AE3B8(s32 index) {
 s32 value=LVL_16_SNIVELAK_FUN_002AE380(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_16_SNIVELAK_FUN_002AE3F0(s32 index) {
 return LVL_16_SNIVELAK_FUN_002AE380(index)==47;
}

s32 LVL_16_SNIVELAK_FUN_002B3760(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

f32 LVL_16_SNIVELAK_FUN_0041C678(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_16_SNIVELAK_FUN_00339698(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_16_SNIVELAK_D_0027F530[13];
void LVL_16_SNIVELAK_FUN_002F02D0(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_16_SNIVELAK_D_0027F530[i].key == key) break;
    }
    if (i < 13) {
        LVL_16_SNIVELAK_D_0027F530[i].fields[9] = value;
        if (LVL_16_SNIVELAK_D_0027F530[i].busy == 0)
            LVL_16_SNIVELAK_D_0027F530[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_16_SNIVELAK_D_001395B8[];
u32 LVL_16_SNIVELAK_FUN_002FF0F8(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_16_SNIVELAK_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_16_SNIVELAK_D_00232080[];
s32 LVL_16_SNIVELAK_FUN_00375AB0(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_16_SNIVELAK_D_00232080;
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
extern int LVL_16_SNIVELAK_D_0022E940[];
extern ListOverrideObject *LVL_16_SNIVELAK_D_00227440[];
extern ListOverridePair LVL_16_SNIVELAK_D_0022E340[];
void LVL_16_SNIVELAK_FUN_00362A20(void)
{
    int *selected = LVL_16_SNIVELAK_D_0022E940;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_16_SNIVELAK_D_00227440[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_16_SNIVELAK_D_0022E340[row->key];
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
