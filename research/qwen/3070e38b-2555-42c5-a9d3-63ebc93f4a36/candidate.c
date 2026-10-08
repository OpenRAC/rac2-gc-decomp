/* 
 * Analyzing function FUN_0011EEA0 (address 0x0011eea0) from /SCUS_972.68
 * 
 * Logic inferred from Ghidra decompilation and disassembly:
 * - Calls sceSifGetReg(4) to fetch IOP register 4 (returns int)
 * - Masks bit 0x40000 (bit 18, used for status flag like DMA completion or interrupt)
 * - If set, calls FUN_0011b0a0 (which writes 0 to DAT_00134688, likely clearing a flag)
 * - Returns non-zero if bit 0x40000 was set, else zero
 */

/* External helpers as identified by Ghidra analysis */
extern int sceSifGetReg(int reg);
extern void FUN_0011b0a0(void);

int FUN_0011eea0(void)
{
    unsigned int reg_val;
    unsigned int mask_val;
    int result;

    reg_val = (unsigned int)sceSifGetReg(4);
    mask_val = 0x00040000U;
    result = (reg_val & mask_val) != 0;
    if (result) {
        FUN_0011b0a0();
    }
    return result;
}