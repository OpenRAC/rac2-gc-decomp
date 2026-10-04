typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_23_DAMOSEL_ORBIT_D_001A8EB0;
void LVL_23_DAMOSEL_ORBIT_FUN_002E2278(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_23_DAMOSEL_ORBIT_FUN_002FDE80(s32 index) {
    NativeTable20 values=LVL_23_DAMOSEL_ORBIT_D_001A8EB0;
    return values.items[index];
}

u32 LVL_23_DAMOSEL_ORBIT_FUN_002B6D88(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_23_DAMOSEL_ORBIT_D_0018C0B4;
u32 LVL_23_DAMOSEL_ORBIT_FUN_002E12B8(void) {
    return LVL_23_DAMOSEL_ORBIT_D_0018C0B4;
}

s32 LVL_23_DAMOSEL_ORBIT_FUN_002EE3A0(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_23_DAMOSEL_ORBIT_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_23_DAMOSEL_ORBIT_FUN_002B6C20(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_23_DAMOSEL_ORBIT_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_23_DAMOSEL_ORBIT_FUN_002B6C58(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_23_DAMOSEL_ORBIT_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_23_DAMOSEL_ORBIT_FUN_002B7878(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_23_DAMOSEL_ORBIT_FUN_002E1FD0(f32, f32, f32, f32, s32, s32);

void LVL_23_DAMOSEL_ORBIT_FUN_002E1A78(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_23_DAMOSEL_ORBIT_FUN_002E1FD0(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_23_DAMOSEL_ORBIT_FUN_002FA6D8(int width, int height, int address, int mode);
extern void LVL_23_DAMOSEL_ORBIT_FUN_003833A0(unsigned int reg, unsigned long value);
extern void LVL_23_DAMOSEL_ORBIT_FUN_002FAA40(int width, int height);

void LVL_23_DAMOSEL_ORBIT_FUN_002F0140(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_23_DAMOSEL_ORBIT_FUN_002FA6D8(width, height, address, 1);
    LVL_23_DAMOSEL_ORBIT_FUN_003833A0(0x47, 0x30000UL);
    LVL_23_DAMOSEL_ORBIT_FUN_003833A0(0x42, 0x8000000044UL);
    LVL_23_DAMOSEL_ORBIT_FUN_002FAA40(0x100, 0x100);
    LVL_23_DAMOSEL_ORBIT_FUN_003833A0(0x42, 0x8000000044UL);
}

extern void LVL_23_DAMOSEL_ORBIT_FUN_002DE0F8(f32, f32, f32, f32 *, s32);

void LVL_23_DAMOSEL_ORBIT_FUN_002B6AB0(f32 *output) {
 LVL_23_DAMOSEL_ORBIT_FUN_002DE0F8(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_23_DAMOSEL_ORBIT_FUN_002B6C90(s32 index) {
 s32 value=LVL_23_DAMOSEL_ORBIT_FUN_002B6C58(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_23_DAMOSEL_ORBIT_FUN_002B6CC8(s32 index) {
 return LVL_23_DAMOSEL_ORBIT_FUN_002B6C58(index)==47;
}

s32 LVL_23_DAMOSEL_ORBIT_FUN_002BC2C0(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_23_DAMOSEL_ORBIT_FUN_00349860(const s32 *count, s32 index, s32 delta, s32 wrap) {
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
extern UpdateRecord LVL_23_DAMOSEL_ORBIT_D_00282200[13];
void LVL_23_DAMOSEL_ORBIT_FUN_00300F88(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_23_DAMOSEL_ORBIT_D_00282200[i].key == key) break;
    }
    if (i < 13) {
        LVL_23_DAMOSEL_ORBIT_D_00282200[i].fields[9] = value;
        if (LVL_23_DAMOSEL_ORBIT_D_00282200[i].busy == 0)
            LVL_23_DAMOSEL_ORBIT_D_00282200[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_23_DAMOSEL_ORBIT_D_001395B8[];
u32 LVL_23_DAMOSEL_ORBIT_FUN_0030F5F8(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_23_DAMOSEL_ORBIT_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}
