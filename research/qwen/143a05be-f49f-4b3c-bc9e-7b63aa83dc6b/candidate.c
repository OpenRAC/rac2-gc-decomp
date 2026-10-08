extern int D_00262944;
extern int D_00262958;

int FUN_002ed8a0(int param_1)
{
  int iVar1;
  iVar1 = -1;
  while( true ) {
    if (param_1 < 0) {
      return iVar1;
    }
    if (*(int *)((int)&D_00262958 + param_1 * 0x28) == 0) {
      break;
    }
    iVar1 = param_1;
    param_1 = *(int *)((int)&D_00262944 + param_1 * 0x28);
  }
  return iVar1;
}
