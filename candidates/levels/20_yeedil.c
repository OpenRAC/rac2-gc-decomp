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
