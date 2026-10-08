unsigned long *FUN_00125960(void);

int FUN_00125E58(unsigned long param_1, short param_2, short param_3);

int FUN_00125E58(unsigned long param_1, short param_2, short param_3) {
    unsigned long *puVar1;
    int iVar2, iVar3, iVar4;
    short s0, s1, s2;

    s1 = (short)(param_1 >> 16);
    s0 = param_2;
    s2 = param_3;

    puVar1 = FUN_00125960();

    iVar4 = s2;
    iVar3 = s0 + 0x3f;
    iVar2 = s0 + 0x7e;
    if ((iVar3 ^ 0x80000000) > (0x7fffffff ^ 0x80000000)) {
        iVar2 = iVar3;
    }

    if ((s1 & 2) == 0) {
        iVar3 = iVar4 + 0x1f;
        if ((iVar3 ^ 0x80000000) > (0x7fffffff ^ 0x80000000)) {
            iVar2 = iVar3;
        } else {
            iVar2 = iVar4 + 0x3e;
        }
        iVar2 = iVar2 >> 5;
    } else {
        iVar3 = iVar4 + 0x3f;
        if ((iVar3 ^ 0x80000000) > (0x7fffffff ^ 0x80000000)) {
            iVar2 = iVar3;
        } else {
            iVar2 = iVar4 + 0x7e;
        }
        iVar2 = iVar2 >> 6;
    }

    iVar2 = (iVar2 >> 6) * iVar2;

    if ((*puVar1 & 0xffff0000ffffUL) == 1) {
        iVar2 = iVar2 * 0x10000;
    } else {
        iVar2 = iVar2 * 0x20000;
    }

    return iVar2 >> 16;
}
