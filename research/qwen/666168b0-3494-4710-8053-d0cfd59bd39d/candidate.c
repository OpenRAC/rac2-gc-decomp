int * FUN_0011D350(int param_1, int param_2)
{
  int *piVar2;
  int iVar3;
  int iVar1;

  iVar3 = *(int *)(param_2 + 0x28);
  if (iVar3 != 0) {
    piVar2 = *(int **)(iVar3 + 8);
    while (1) {
      if (piVar2 == 0) {
        iVar3 = *(int *)(iVar3 + 0x14);
      }
      else {
        iVar1 = *piVar2;
        while (1) {
          if (iVar1 == param_1) {
            return piVar2;
          }
          piVar2 = (int *)piVar2[0xe];
          if (piVar2 == 0) break;
          iVar1 = *piVar2;
        }
        iVar3 = *(int *)(iVar3 + 0x14);
      }
      if (iVar3 == 0) break;
      piVar2 = *(int **)(iVar3 + 8);
    }
  }
  return 0;
}
