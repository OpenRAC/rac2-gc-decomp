unsigned int _FUN_0011d810(int *param_1)
{
  int iVar1;
  
  iVar1 = *param_1;
  if ((iVar1 != 0) && (param_1[1] == *(int *)(iVar1 + 0x18)) && ((*(unsigned int *)(iVar1 + 0x10) & 1) != 0)) {
    return 1;
  }
  return 0;
}