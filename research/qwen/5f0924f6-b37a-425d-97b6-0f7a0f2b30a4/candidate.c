unsigned int FUN_0012E8C8(unsigned long *param_1, int param_2)
{
  unsigned long val = *param_1;
  int shift = 0x40 - param_2;
  unsigned long result = val >> (unsigned long)shift;
  return (unsigned int)(result >> 32);
}
