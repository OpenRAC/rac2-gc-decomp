unsigned int FUN_00126ED8(unsigned int param_1)
{
  /* If top 4 bits equal 0x7, clear bits 28-31 and set bit 31 */
  if ((param_1 >> 28) == 7U) {
    /* Force lui/ori by using volatile to prevent constant folding */
    volatile unsigned int mask = 0x0fff0000U;
    mask |= 0x0000ffffU;
    param_1 = (param_1 & mask) | 0x80000000U;
  }
  return param_1;
}