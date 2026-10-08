// Helper function: takes two floats and two ints; third arg is a pointer
extern int FUN_003022a0(float, float, void*, int);

void FUN_00302440(void* p) {
    unsigned char val = ((unsigned char*)p)[32];
    if (val == 3) {
        float z = 0.0f;
        FUN_003022a0(z, z, p, 0);
    }
}