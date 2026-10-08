/* Stub for FUN_00132AC8 calling SndIopCommandContinuing(0x18,0,0,0,0) */
/* Helper signature with first arg as unsigned char (zero-extended), others as ints/longs per callee usage */
extern void SndIopCommandContinuing(unsigned char, int, int, long, unsigned long);

void FUN_00132AC8(void)
{
    SndIopCommandContinuing(0x18, 0, 0, 0, 0);
    return;
}