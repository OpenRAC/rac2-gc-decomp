typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_26_JAMMING_ARRAY_D_001A8EB0;
void LVL_26_JAMMING_ARRAY_FUN_002D7FE0(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_26_JAMMING_ARRAY_FUN_002F4030(s32 index) {
    NativeTable20 values=LVL_26_JAMMING_ARRAY_D_001A8EB0;
    return values.items[index];
}

u32 LVL_26_JAMMING_ARRAY_FUN_002AE5D0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_26_JAMMING_ARRAY_D_0018C0B4;
u32 LVL_26_JAMMING_ARRAY_FUN_002D7020(void) {
    return LVL_26_JAMMING_ARRAY_D_0018C0B4;
}

s32 LVL_26_JAMMING_ARRAY_FUN_002E4108(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_26_JAMMING_ARRAY_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_26_JAMMING_ARRAY_FUN_002AE468(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_26_JAMMING_ARRAY_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_26_JAMMING_ARRAY_FUN_002AE4A0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_26_JAMMING_ARRAY_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_26_JAMMING_ARRAY_FUN_002AF0C0(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_26_JAMMING_ARRAY_FUN_002D7D38(f32, f32, f32, f32, s32, s32);

void LVL_26_JAMMING_ARRAY_FUN_002D77E0(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_26_JAMMING_ARRAY_FUN_002D7D38(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_26_JAMMING_ARRAY_FUN_002F0888(int width, int height, int address, int mode);
extern void LVL_26_JAMMING_ARRAY_FUN_0037ACB0(unsigned int reg, unsigned long value);
extern void LVL_26_JAMMING_ARRAY_FUN_002F0BF0(int width, int height);

void LVL_26_JAMMING_ARRAY_FUN_002E5EA8(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_26_JAMMING_ARRAY_FUN_002F0888(width, height, address, 1);
    LVL_26_JAMMING_ARRAY_FUN_0037ACB0(0x47, 0x30000UL);
    LVL_26_JAMMING_ARRAY_FUN_0037ACB0(0x42, 0x8000000044UL);
    LVL_26_JAMMING_ARRAY_FUN_002F0BF0(0x100, 0x100);
    LVL_26_JAMMING_ARRAY_FUN_0037ACB0(0x42, 0x8000000044UL);
}

extern void LVL_26_JAMMING_ARRAY_FUN_002D3E60(f32, f32, f32, f32 *, s32);

void LVL_26_JAMMING_ARRAY_FUN_002AE2F8(f32 *output) {
 LVL_26_JAMMING_ARRAY_FUN_002D3E60(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_26_JAMMING_ARRAY_FUN_002AE4D8(s32 index) {
 s32 value=LVL_26_JAMMING_ARRAY_FUN_002AE4A0(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_26_JAMMING_ARRAY_FUN_002AE510(s32 index) {
 return LVL_26_JAMMING_ARRAY_FUN_002AE4A0(index)==47;
}

s32 LVL_26_JAMMING_ARRAY_FUN_002B39C8(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}
