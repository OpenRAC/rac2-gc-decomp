void FUN_0027CDC8(unsigned int param_1, unsigned int param_2)
{
  extern unsigned int D_001B1608;
  extern unsigned int D_001B9E40[64];
  extern unsigned int D_001B9F40[64];

  if (D_001B1608 < 0x40U) {
    unsigned int idx = D_001B1608;
    D_001B9E40[idx] = param_1;
    D_001B9F40[idx] = param_2;
    D_001B1608 = idx + 1U;
  }

  if (D_001B1608 >= 0x40U) {
    return;
  }
}