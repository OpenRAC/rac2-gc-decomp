extern void FUN_00336DA0(void* a0);
extern void FUN_00336ED0(int a0, void* a1);
extern void* D_001ADA78;

void FUN_003360F8(void* arg0, int arg1) {
    int* base = (int*)arg0;
    base[0x0c] = (int)D_001ADA78;
    if (base[0x0b] != 0) {
        int iVar1;
        if (base[5] == 0) {
            FUN_00336ED0(base[0x0b], &base[0]);
            iVar1 = base[7];
        } else {
            iVar1 = base[7];
        }
        if (iVar1 == 0) {
            FUN_00336ED0(base[0x0b], &base[2]);
            iVar1 = base[6];
        } else {
            iVar1 = base[6];
        }
        if (iVar1 == 0) {
            FUN_00336ED0(base[0x0b], &base[1]);
            iVar1 = base[8];
        } else {
            iVar1 = base[8];
        }
        if (iVar1 == 0) {
            FUN_00336ED0(base[0x0b], &base[3]);
        }
    }
    if ((arg1 & 1) != 0) {
        FUN_00336DA0(arg0);
    }
}