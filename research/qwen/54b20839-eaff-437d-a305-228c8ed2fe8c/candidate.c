int FUN_0012FA70(void* arg1, int index, int val1, int val2) {
    int* base = *(int**)((char*)arg1 + 0x40);
    int* addr1 = (int*)((char*)base + 0xc + index * 8);
    int* addr2 = (int*)((char*)base + 0x10 + index * 8);
    int temp;
    *addr2 = val2;
    temp = *addr1;
    *addr1 = val1;
    return temp;
}