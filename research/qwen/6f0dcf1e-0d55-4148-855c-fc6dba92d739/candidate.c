/* target: FUN_0029cba8 @ 0x0029cba8 */
/* called helper: FUN_0034ce98 @ 0x0034ce98 (writes to param_1+700) */
/* global reference: D_001A8CFC @ 0x001a8cfc (4-byte global, loaded with lw) */
/* ABI: a0 = param_1, v0 = condition value from D_001A8CFC, return via jr ra */
/* FUN_0034ce98 signature: void(int, int) — writes param_1+700 */

extern int D_001A8CFC;
extern void FUN_0034ce98(int, int);

void FUN_0029cba8(int param_1)
{
  if (D_001A8CFC != 0) {
    FUN_0034ce98(D_001A8CFC + 0x376c8, param_1);
  }
  return;
}