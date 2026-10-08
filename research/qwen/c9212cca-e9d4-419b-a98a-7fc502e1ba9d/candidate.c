extern unsigned int FUN_002ED688(void *arg);

void FUN_002EE678(void *arg, unsigned int value) {
    unsigned int ret = FUN_002ED688(arg);
    if (ret != 0) {
        *(unsigned int *)(ret + 0x100) = value;
    }
}