void FUN_00348EC0(void *a0);

extern char D_001A8CFC;

void FUN_0029C138(void)
{
    char *p = &D_001A8CFC;
    if (p != (char *)0) {
        unsigned int off = 0x39140;
        FUN_00348EC0((void *)((unsigned int)p + off));
    }
    return;
}