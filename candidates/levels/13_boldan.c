typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_13_BOLDAN_D_001A8EB0;
void LVL_13_BOLDAN_FUN_002E23C8(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_13_BOLDAN_FUN_002FE420(s32 index) {
    NativeTable20 values=LVL_13_BOLDAN_D_001A8EB0;
    return values.items[index];
}
