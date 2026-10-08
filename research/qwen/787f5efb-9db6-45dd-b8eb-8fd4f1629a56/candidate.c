/* External helper declared with exact alias from ratchet_task.externals */
extern int FUN_0033E348(void);

/* 
 * Function at 0x0033e310:
 * - Accepts one integer parameter (e.g., pointer offset or struct pointer).
 * - Calls external FUN_0033E348 and multiplies its result by 0x14.
 * - Adds that to a value loaded from offset 0x378 of the parameter.
 * - Returns the computed sum.
 */
int FUN_0033E310(int param_1)
{
  int iVar1;
  
  iVar1 = FUN_0033E348();
  return *(int *)(param_1 + 0x378) + iVar1 * 0x14;
}