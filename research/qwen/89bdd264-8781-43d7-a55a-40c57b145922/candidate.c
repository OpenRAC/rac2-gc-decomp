static void FUN_002a0468(int param_1, long param_2, unsigned long param_3, long param_4, long param_5)
{
  *(unsigned long *)(param_1 + 0x38) = param_2 << 0x20 | param_3 | param_4 << 0x8 | param_5 << 0x10;
  return;
}