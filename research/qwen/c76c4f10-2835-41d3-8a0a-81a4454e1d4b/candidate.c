struct S {
    char pad[0x190];
    unsigned int ptr;
};

void FUN_002704D0(void)
{
    extern struct S D_001B5200;
    unsigned int ptr = D_001B5200.ptr;
    *(unsigned short *)(ptr + 0x7EU) = 1U;
    ptr = D_001B5200.ptr;
    *(unsigned char *)(ptr + 0x7DU) = 0U;
}