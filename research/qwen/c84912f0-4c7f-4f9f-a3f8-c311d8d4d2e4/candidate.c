extern void FUN_003465d0(void *param_1);
extern volatile int D_001A8CFC;

void FUN_0029c1d8(void)
{
    int v = D_001A8CFC;
    if (v != 0) {
        FUN_003465d0((void *)(v + 0x39a98));
    }
}
