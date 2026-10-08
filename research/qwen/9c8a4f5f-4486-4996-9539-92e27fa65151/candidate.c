/* 
 * Standalone C candidate for function FUN_0012FAE8.
 * Analysis based on Ghidra disassembly and decompilation.
 * 
 * Summary:
 *   - FUN_0012FAE8 allocates a local array of 8 uint32_t's (32 bytes).
 *   - Sets the first element to 1.
 *   - Calls FUN_0012FA98 with (arg0, &array[0]) and discards the return value.
 *   - Restores stack and returns.
 *   - FUN_0012FA98 signature is inferred from its own decompilation as:
 *       undefined8 FUN_0012FA98(long param_1, int *param_2)
 *   - The callee checks param_1, accesses a table at offset 0x40, then
 *     dereferences a function pointer indexed by *param_2 and calls it.
 * 
 * Compiler flags observed: -O2 -G0 -ffunction-sections
 * This candidate must match byte-for-byte the reference implementation.
 */

/* External function prototype, binding provided by task. */
extern unsigned long FUN_0012FA98(long, int *);

/* Helper aliases (exact symbol names as in externals). */
typedef int int32;
typedef unsigned int uint32;

unsigned long FUN_0012FAE8(int32 param_1)
{
  uint32 auStack_30[8];

  auStack_30[0] = 1;
  FUN_0012FA98((long)param_1, (int *)auStack_30);
  return 0;
}
