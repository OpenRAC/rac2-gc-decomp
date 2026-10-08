int FUN_0011c000(unsigned long param_1) {
  int iVar1;
  unsigned long uVar2;
  long lVar3;

  lVar3 = ((param_1 & 0x7fffffffffffffffUL) >> 52) - 1075;
  if (lVar3 < -53) {
    return 0;
  }
  if (12 < lVar3) {
    return 9999;
  }
  uVar2 = param_1 & 0xfffffffffffffUL | 0x10000000000000UL;
  if (lVar3 < 0) {
    uVar2 = uVar2 >> ((int)-lVar3 - 2);
    if ((uVar2 & 3) == 3) {
      iVar1 = (int)(uVar2 >> 2) + 1;
    } else {
      iVar1 = (int)(uVar2 >> 2);
    }
  } else {
    iVar1 = (int)(uVar2 << lVar3);
  }
  return iVar1;
}