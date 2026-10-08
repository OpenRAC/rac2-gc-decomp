/*
 * Target function: FUN_0012E980
 * Architecture: MIPS
 * Compilation flags: -O2 -G0 -ffunction-sections
 * External helpers: FUN_0012E8C8, FUN_0012E8E8
 *
 * Analysis:
 * The function FUN_0012E980 takes two 64-bit arguments (param_1, param_2).
 * It calls FUN_0012E8C8() with no arguments, stores its return value in a local variable.
 * Then it calls FUN_0012E8E8(param_1, param_2).
 * Finally, it returns the value obtained from FUN_0012E8C8().
 *
 * The disassembly confirms standard MIPS calling convention:
 * - Stack frame allocation: addiu sp,sp,-0x40
 * - Saved registers: s0-s2, ra
 * - Arguments passed in a0/a1 (first two args)
 * - Delay slots filled with move instructions
 * - jal calls to external functions at 0x0012e8c8 and 0x0012e8e8
 * - Restoration epilogue before jr ra
 */

/* External function declarations — aliases as per ratchet_task.externals */
unsigned long long FUN_0012E8C8(void);
void FUN_0012E8E8(unsigned long long, unsigned long long);

unsigned long long FUN_0012E980(unsigned long long param_1, unsigned long long param_2)
{
    unsigned long long saved_retval;
    saved_retval = FUN_0012E8C8();
    FUN_0012E8E8(param_1, param_2);
    return saved_retval;
}