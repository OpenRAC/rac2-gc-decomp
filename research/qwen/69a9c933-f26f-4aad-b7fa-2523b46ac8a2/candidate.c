int FUN_00351E58(void);

int FUN_00351EE8(int *param_1)
{
  int iVar1;
  long lVar2;
  
  lVar2 = FUN_00351E58();
  if (lVar2 == 0) {
    iVar1 = *param_1 + param_1[2] * 0xd0000;
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}