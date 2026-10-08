/* 
 * Function: FUN_002cd6f0
 * Address: 0x002cd6f0
 * Size: 48 bytes (12 instructions)
 * 
 * Disassembly summary:
 *  - addiu sp,sp,-0x10
 *  - sd ra,0x0(sp)
 *  - jal 0x0027c540
 *  - move a0,zero   (delay slot: sets arg0 = 0)
 *  - jal 0x0029cc20
 *  - nop            (delay slot)
 *  - jal 0x0027c660
 *  - nop            (delay slot)
 *  - ld ra,0x0(sp)
 *  - move v0,zero
 *  - jr ra
 *  - addiu sp,sp,0x10
 * 
 * Key observations:
 * - FUN_0027c540 receives argument 0 in a0 (delay slot)
 * - FUN_0029cc20 and FUN_0027c660 receive no arguments
 * - Function returns 0
 * 
 * Previous attempts failed because compiler optimization removed delay-slot instruction.
 * Here we use function pointer to prevent optimization of argument passing.
 */
extern void (*volatile FUN_0027c540_ptr)(void);
extern void (*volatile FUN_0029cc20_ptr)(void);
extern void (*volatile FUN_0027c660_ptr)(void);

long long FUN_002cd6f0(void)
{
    void (*f)(void);
    f = FUN_0027c540_ptr;
    f();
    FUN_0029cc20_ptr();
    FUN_0027c660_ptr();
    return 0;
}