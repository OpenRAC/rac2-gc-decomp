/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern long long FUN_0027C540(int);
extern void FUN_0029CED8(void);
extern void FUN_0027C660(void);

long long FUN_002D5540(void)
{
    FUN_0027C540(0);
    FUN_0029CED8();
    FUN_0027C660();
    return 0;
}