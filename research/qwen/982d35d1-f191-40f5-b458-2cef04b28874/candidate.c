void FUN_0011BAF0(int *param_1)
{
  int iVar1;
  int iVar2;

  iVar1 = param_1[1];
  iVar2 = param_1[3];
  iVar1 = iVar1 + 1;
  iVar2 = iVar2 + 1;
  param_1[1] = iVar1;
  param_1[3] = iVar2;
  if (iVar2 == (int)(param_1 + 4) + *param_1) {
    param_1[3] = (int)(param_1 + 4);
  }
  return;
}
