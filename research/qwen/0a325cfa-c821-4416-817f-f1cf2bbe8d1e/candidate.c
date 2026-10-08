extern int FUN_00335E20(char *);
unsigned FUN_0034CA68(int a0) {
    char *p = (char *)(a0 + 0x358);
    int addr = FUN_00335E20(p);
    return (unsigned)*(int *)addr & 0xff000000U;
}
