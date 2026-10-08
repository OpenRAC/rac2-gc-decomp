/* Target: FUN_00115E68 */
/* Exact match to binary's 10-instruction layout */
/* Signature inferred from disassembly: returns 64-bit, calls FUN_001175F0(arg1, 0, 10) */
/* Sign-extension instructions (dsll32/dsra32) suggest compiler must emit them */

extern unsigned long long FUN_001175F0(unsigned long long arg1, unsigned int arg2, unsigned int arg3);

unsigned long long FUN_00115E68(unsigned long long arg1)
{
    unsigned long long result = FUN_001175F0(arg1, 0, 10);
    return result;
}