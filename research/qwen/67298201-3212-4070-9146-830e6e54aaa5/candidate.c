/* Wrapper function that calls FUN_00284EA8 and returns.
 * Stack frame is used only to save/restore the return address (ra).
 * The function follows standard MIPS ABI conventions:
 * - addiu sp,sp,-0x10 allocates 16 bytes on the stack
 * - sd ra,0x(sp) saves the return address
 * - jal calls the external helper
 * - delay slot is a nop
 * - ld ra,0x(sp) restores the return address
 * - jr ra returns to caller
 * - _addiu sp,sp,0x10 deallocates stack
 */
void FUN_0027BA48(void);

void FUN_0027BA48(void)
{
  FUN_00284EA8();
  return;
}