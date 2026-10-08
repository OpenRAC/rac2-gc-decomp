/* RATCHET: Candidate for FUN_0012E9D0 (0x0012e9d0)
 *
 * Hypothesis: To match delay-slot instructions:
 * - After first jal, original used "_move s0,a0"
 * - After second jal, original used "_li a1,0x1"
 *
 * This suggests param_1 was passed in a0, and compiler reused a0 as base for s0.
 * We force explicit use of a0-like behavior by avoiding stack copies.
 *
 * Helpers (externals from task):
 * - FUN_0012E8C8: takes (ulong *param_1, int param_2); returns (int)(*param_1 >> (long)(0x40 - param_2))
 * - FUN_0012E8E8: takes (ulong *param_1, long param_2); modifies *param_1, param_1[2], param_1[3]
 */

/* Helper declarations (exact aliases per task.externals) */
extern unsigned long long FUN_0012E8C8(unsigned long long *param_1, int param_2);
extern void FUN_0012E8E8(unsigned long long *param_1, long param_2);

unsigned long long FUN_0012E9D0(unsigned long long param_1) {
    unsigned long long saved_ret;
    
    saved_ret = FUN_0012E8C8(&param_1, 1);
    FUN_0012E8E8(&param_1, 1);
    return saved_ret;
}
