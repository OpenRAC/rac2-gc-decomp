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
