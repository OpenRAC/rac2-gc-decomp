extern signed short D_001bad04;

signed short FUN_00287e70(void)
{
    signed short *p = (signed short *)((char *)&D_001bad04 - (int)&D_001bad04 + (signed short)0x1c0000);
    return *p;
}