/* Target: FUN_0011BAC8
 * Receives a 32-bit argument in a0.
 * Returns pointer to static array of 4 unsigned ints at 0x13CA40.
 * No initializer on statics to avoid init code.
 */
unsigned int *FUN_0011BAC8(unsigned int p1)
{
  static unsigned int D_0013CA40;
  static unsigned int D_0013CA44;
  static unsigned int D_0013CA48;
  static unsigned int D_0013CA4C;
  static unsigned int D_0013CA50;

  D_0013CA40 = p1;
  D_0013CA44 = 0;
  D_0013CA48 = (unsigned int)&D_0013CA50;
  D_0013CA4C = (unsigned int)&D_0013CA50;
  return &D_0013CA40;
}