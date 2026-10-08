extern unsigned char D_001BF200[];

void* FUN_002A9640(void* a0, unsigned int a1, long a2)
{
    unsigned char idx;
    unsigned int offset;
    unsigned char* record;
    void* ptr_field;
    unsigned int val_field;

    idx = ((unsigned char*)a0)[0xa8];
    if (idx == 0xff) {
        return (void*)0;
    }
    offset = idx * 0x40;
    record = &D_001BF200[offset];
    ptr_field = *(void**)(record + 0x38);
    if (ptr_field != a0) {
        return (void*)0;
    }
    val_field = *(unsigned int*)(record + 0x24);
    if ((val_field & a1) == 0) {
        if (a2 == 0) {
            ((unsigned char*)a0)[0xa8] = 0xff;
        }
        return (void*)0;
    }
    return (void*)record;
}