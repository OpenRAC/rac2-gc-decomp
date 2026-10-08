extern unsigned int D_001A8CFC;

int FUN_0029B7C0(unsigned int a0, unsigned int a1, unsigned int a2)
{
    unsigned int base;
    base = D_001A8CFC;
    if (base == 0U) {
        return 0;
    }
    if (a2 >= 2U) {
        return 0;
    }
    base += 0x38000U;
    base += a2 * 8U;
    ((unsigned int*)(base + 0x7390U))[0] = a0;
    ((unsigned int*)(base + 0x7394U))[0] = a1;
    return 1;
}
