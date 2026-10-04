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
