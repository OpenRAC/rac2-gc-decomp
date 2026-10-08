extern char *D_001B1888;
extern char *D_001B1894;
extern char *D_001B1898;
extern const int C_00000074;

void FUN_0028AE88(int param_1, int param_2)
{
  int iVar1;
  unsigned int *puVar2;
  int *piVar3;
  int iVar4;
  unsigned int uVar5;
  int iVar6;
  int off_74;

  off_74 = C_00000074;
  iVar6 = param_1;
  puVar2 = (unsigned int *)(D_001B1888 + off_74 + iVar6 * 4);
  if (*puVar2 == 0) {
    uVar5 = (unsigned int)(param_2 + 0xf) & 0xfffffff0;
    *puVar2 = uVar5;
    if (param_1 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(D_001B1888 + (iVar6 + -1) * 4 + 0x14);
    }
    iVar1 = *(int *)(D_001B1888 + iVar6 * 4 + 0x14);
    for (; iVar4 < iVar1; iVar4 = iVar4 + 1) {
      puVar2 = (unsigned int *)(D_001B1898 + iVar4 * 8);
      *puVar2 = *puVar2 & 0x7fffffff;
      piVar3 = (int *)(D_001B1898 + iVar4 * 8);
      *piVar3 = *piVar3 + uVar5;
    }
    if (param_1 == 0) {
      iVar4 = 0;
    }
    else {
      iVar4 = *(int *)(D_001B1888 + (iVar6 + -1) * 4 + 0x34);
    }
    iVar6 = *(int *)(D_001B1888 + iVar6 * 4 + 0x34);
    for (; iVar4 < iVar6; iVar4 = iVar4 + 1) {
      puVar2 = (unsigned int *)(D_001B1894 + iVar4 * 8);
      *puVar2 = *puVar2 & 0x7fffffff;
      piVar3 = (int *)(D_001B1894 + iVar4 * 8);
      *piVar3 = *piVar3 + uVar5;
    }
  }
  return;
}