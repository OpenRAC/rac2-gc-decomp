/*
 * Disassembled at 0x002cd220
 * Signature: int FUN_002cd220(void)
 * Behavior:
 *   - Saves RA on stack
 *   - Calls FUN_0027C540(0)
 *   - Calls FUN_0029C138()
 *   - Calls FUN_0027C660()
 *   - Restores RA from stack
 *   - Returns 0
 */
extern int FUN_0027C540(int);
extern int FUN_0029C138(void);
extern int FUN_0027C660(void);

int FUN_002CD220(void)
{
  FUN_0027C540(0);
  FUN_0029C138();
  FUN_0027C660();
  return 0;
}