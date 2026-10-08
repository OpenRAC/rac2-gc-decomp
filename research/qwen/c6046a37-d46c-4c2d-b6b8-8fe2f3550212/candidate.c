/* Function at 0x002a01f0 is a minimal wrapper calling FUN_00282B50.
   It sets up:
   - a0 = 0x1cd7c0 (0x1d << 16 | 0xd7c0)
   - a1 = 0x70003a00 (0x7000 << 16 | 0x3a00)
   - a2 = 0x3c0 (960 bytes)
   The callee copies 960 bytes from 0x70003a00 to 0x1cd7c0.
   This wrapper attempts to match original instruction sequence.
*/

extern void FUN_00282B50(void *, const void *, int);

void FUN_002A01F0(void)
{
    FUN_00282B50((void *)0x1cd7c0, (const void *)0x70003a00, 0x3c0);
}
