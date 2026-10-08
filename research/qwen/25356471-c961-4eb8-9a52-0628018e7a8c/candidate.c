// Helper function declarations, using exact symbol names from externals
void FUN_002AFDD8(unsigned long, unsigned long, unsigned long, unsigned long);
unsigned int FUN_00282E20(unsigned char (*)[16]);

// Implementation of FUN_002AFF08
void FUN_002AFF08(unsigned long arg1, unsigned long arg2, unsigned long arg3) {
    unsigned char stack_buffer[16];

    FUN_002AFDD8(arg1, (unsigned long)stack_buffer, arg2, arg3);
    FUN_00282E20((unsigned char (*)[16])stack_buffer);
}