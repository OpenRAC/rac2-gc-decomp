void FUN_00126f00(void *param_1, long param_2)
{
  long iVar1;
  if (param_2 != 0) {
    iVar1 = param_2 + -1;
    do {
      *(unsigned char *)param_1 = 0;
      iVar1 = iVar1 + -1;
      param_1 = (unsigned char *)param_1 + 1;
    } while (iVar1 != -1);
  }
  return;
}
