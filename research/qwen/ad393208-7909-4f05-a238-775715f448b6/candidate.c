void FUN_002acad0(int param_1, unsigned short *param_2) {
  unsigned short uVar1;
  unsigned short *puVar2;
  long lVar3;
  extern unsigned int D_001B1B5C;

  lVar3 = 1;
  uVar1 = *param_2;
  puVar2 = param_2;

  if (0 < (short)*param_2) {
    do {
      puVar2 = puVar2 + 1;
      if (D_001B1B5C + (short)*puVar2 * 0x100 == param_1) {
        *puVar2 = *(unsigned short *)((char *)param_2 + (short)uVar1);
        *param_2 = *param_2 - 1;
        return;
      }
      lVar3 = lVar3 + 1;
      uVar1 = *param_2;
    } while (lVar3 <= (short)*param_2);
  }
  return;
}