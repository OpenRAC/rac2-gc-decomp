typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_0_ARANOS_TUTORIAL_D_001A8EB0;
void LVL_0_ARANOS_TUTORIAL_FUN_002D7940(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_0_ARANOS_TUTORIAL_FUN_002F3DD0(s32 index) {
    NativeTable20 values=LVL_0_ARANOS_TUTORIAL_D_001A8EB0;
    return values.items[index];
}

u32 LVL_0_ARANOS_TUTORIAL_FUN_002ADFD0(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_0_ARANOS_TUTORIAL_D_0018C0B4;
u32 LVL_0_ARANOS_TUTORIAL_FUN_002D68E8(void) {
    return LVL_0_ARANOS_TUTORIAL_D_0018C0B4;
}

s32 LVL_0_ARANOS_TUTORIAL_FUN_002E3A68(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}
