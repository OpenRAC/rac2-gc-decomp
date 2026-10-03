typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_8_TABORA_D_001A8EB0;
void LVL_8_TABORA_FUN_002E3728(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_8_TABORA_FUN_002FFAB8(s32 index) {
    NativeTable20 values=LVL_8_TABORA_D_001A8EB0;
    return values.items[index];
}

u32 LVL_8_TABORA_FUN_002BF680(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_8_TABORA_D_0018C0B4;
u32 LVL_8_TABORA_FUN_002E26D0(void) {
    return LVL_8_TABORA_D_0018C0B4;
}

s32 LVL_8_TABORA_FUN_002EF8B8(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}
