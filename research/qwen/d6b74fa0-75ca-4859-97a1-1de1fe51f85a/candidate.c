extern int D_001A8CFC;
extern void FUN_00345358(void*);

void FUN_0029C450(void)
{
    int cond = D_001A8CFC;
    if (cond != 0) {
        int offset = 0x39f38;
        void* arg = (void*)((char*)&D_001A8CFC + offset);
        FUN_00345358(arg);
    }
    return;
}