void FUN_002a0488(int param_1,long param_2,long param_3,long param_4)

{
  *(unsigned long *)(param_1 + 0x38) = *(unsigned long *)(param_1 + 0x38) & 0xffffffff | param_2 << 0x20 | param_3 << 0x28 | param_4 << 0x30;
  return;
}
