/* Function FUN_0029C3E0 at 0x0029c3e0 */
/* Reads global D_001A8CFC; if non-zero, calls FUN_0033F030 with (global + 0x3b7c0) */

/* External symbols referenced */
extern int D_001A8CFC;  /* Global variable at 0x001a8cfc */

extern void FUN_0033F030(int arg1);  /* External helper function at 0x0033f030 */

/* Implementation */
void FUN_0029C3E0(void)
{
    if (D_001A8CFC != 0) {
        FUN_0033F030(D_001A8CFC + 0x3b7c0);
    }
    return;
}
