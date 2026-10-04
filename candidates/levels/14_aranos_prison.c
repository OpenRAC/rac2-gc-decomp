typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_14_ARANOS_PRISON_D_001A8EB0;
void LVL_14_ARANOS_PRISON_FUN_002DF240(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_14_ARANOS_PRISON_FUN_002FB958(s32 index) {
    NativeTable20 values=LVL_14_ARANOS_PRISON_D_001A8EB0;
    return values.items[index];
}

u32 LVL_14_ARANOS_PRISON_FUN_002B5750(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_14_ARANOS_PRISON_D_0018C0B4;
u32 LVL_14_ARANOS_PRISON_FUN_002DE1E8(void) {
    return LVL_14_ARANOS_PRISON_D_0018C0B4;
}

s32 LVL_14_ARANOS_PRISON_FUN_002EB368(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_14_ARANOS_PRISON_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_14_ARANOS_PRISON_FUN_002B55E8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_14_ARANOS_PRISON_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_14_ARANOS_PRISON_FUN_002B5620(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_14_ARANOS_PRISON_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_14_ARANOS_PRISON_FUN_002B6240(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_14_ARANOS_PRISON_FUN_002DEF98(f32, f32, f32, f32, s32, s32);

void LVL_14_ARANOS_PRISON_FUN_002DEA40(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_14_ARANOS_PRISON_FUN_002DEF98(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_14_ARANOS_PRISON_FUN_002F81B0(int width, int height, int address, int mode);
extern void LVL_14_ARANOS_PRISON_FUN_00385CB8(unsigned int reg, unsigned long value);
extern void LVL_14_ARANOS_PRISON_FUN_002F8518(int width, int height);

void LVL_14_ARANOS_PRISON_FUN_002ED190(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_14_ARANOS_PRISON_FUN_002F81B0(width, height, address, 1);
    LVL_14_ARANOS_PRISON_FUN_00385CB8(0x47, 0x30000UL);
    LVL_14_ARANOS_PRISON_FUN_00385CB8(0x42, 0x8000000044UL);
    LVL_14_ARANOS_PRISON_FUN_002F8518(0x100, 0x100);
    LVL_14_ARANOS_PRISON_FUN_00385CB8(0x42, 0x8000000044UL);
}

extern void LVL_14_ARANOS_PRISON_FUN_002DAFE0(f32, f32, f32, f32 *, s32);

void LVL_14_ARANOS_PRISON_FUN_002B5478(f32 *output) {
 LVL_14_ARANOS_PRISON_FUN_002DAFE0(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_14_ARANOS_PRISON_FUN_002B5658(s32 index) {
 s32 value=LVL_14_ARANOS_PRISON_FUN_002B5620(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_14_ARANOS_PRISON_FUN_002B5690(s32 index) {
 return LVL_14_ARANOS_PRISON_FUN_002B5620(index)==47;
}

s32 LVL_14_ARANOS_PRISON_FUN_002BAB48(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

f32 LVL_14_ARANOS_PRISON_FUN_0042FAE8(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}
