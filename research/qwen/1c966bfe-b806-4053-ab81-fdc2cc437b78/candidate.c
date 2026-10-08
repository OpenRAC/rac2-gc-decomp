void FUN_0028AFE8(int param_1);

extern int D_001B1888[];
extern int D_001B1894[];
extern int D_001B1898[];

void FUN_0028AFE8(int param_1)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  unsigned int *puVar5;
  int iVar6;

  iVar1 = D_001B1888[(param_1 * 4) + 29];
  if (param_1 == 0) {
    iVar3 = 0;
    iVar6 = 0;
    if (0 < D_001B1888[13]) {
      do {
        iVar2 = iVar3 * 8;
        iVar3 = iVar3 + 1;
        ((int *)(((char *)&D_001B1894) + 4))[iVar2 / 4] = 0;
      } while (iVar3 < D_001B1888[13]);
    }
    iVar3 = 0;
  }
  else {
    iVar6 = param_1 << 2;
    iVar3 = D_001B1888[(param_1 - 1) * 4 + 5];
  }
  iVar2 = D_001B1888[iVar6 + 5];
  for (; iVar3 < iVar2; iVar3 = iVar3 + 1) {
    piVar4 = (int *)(((char *)&D_001B1898) + iVar3 * 8);
    *piVar4 = *piVar4 - iVar1;
    puVar5 = (unsigned int *)(((char *)&D_001B1898) + iVar3 * 8);
    *puVar5 = *puVar5 | 0x80000000;
  }
  if (param_1 == 0) {
    iVar3 = 0;
  }
  else {
    iVar3 = D_001B1888[(param_1 - 1) * 4 + 13];
  }
  iVar2 = D_001B1888[iVar6 + 13];
  for (; iVar3 < iVar2; iVar3 = iVar3 + 1) {
    piVar4 = (int *)(((char *)&D_001B1894) + iVar3 * 8);
    *piVar4 = *piVar4 - iVar1;
    puVar5 = (unsigned int *)(((char *)&D_001B1894) + iVar3 * 8);
    *puVar5 = *puVar5 | 0x80000000;
  }
  D_001B1888[iVar6 + 29] = 0;
}