/* Target: FUN_0033cd80
 * Behavior:
 *   - Accepts 64-bit argument param_1 (pointer or integer)
 *   - Calls FUN_0033ab50 with (param_1 + 8)
 *   - Calls FUN_003363d0 twice with (param_1 + 0x2ac) and (param_1 + 0x2f4)
 *   - Returns original param_1
 * Notes:
 *   - External functions are declared using provided aliases from task.externals
 *   - No includes used; pure standalone C
 *   - Delay-slot behavior handled implicitly via sequence
 *   - PS2 ABI: 32-bit pointers, stack alignment 16-byte
 */
void FUN_0033ab50(int arg0);
void FUN_003363d0(int arg0);

void *FUN_0033cd80(void *param_1) {
  int iVar1;
  iVar1 = (int)param_1;
  FUN_0033ab50(iVar1 + 8);
  FUN_003363d0(iVar1 + 0x2ac);
  FUN_003363d0(iVar1 + 0x2f4);
  return param_1;
}