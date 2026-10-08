typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned char uint8_t;

extern int32_t FUN_002ED688(void *a0);

uint32_t FUN_002EE6A8(void *a0) {
    int32_t r = FUN_002ED688(a0);
    if (r == 0) return 0;
    return *(uint32_t *)((char *)r + 0x100);
}