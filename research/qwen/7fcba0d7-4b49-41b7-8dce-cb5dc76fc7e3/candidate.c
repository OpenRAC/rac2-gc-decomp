void FUN_002d5e38(int param_1, int param_2)
{
  int iVar1;
  int iVar2;

  iVar1 = *(int *)(param_1 + 0x40);
  iVar2 = *(int *)(param_1 + 0x34);
  FUN_002d5b58(param_2, *(int *)((iVar1 * 12 + iVar2) + 4));
  return;
}