int FUN_00351a38(long param_1)

{
  int bVar1;
  long lVar2;
  volatile long lVar3;
  
  lVar2 = FUN_00351938();
  bVar1 = 0;
  lVar3 = 0;
  if (lVar2 == 0) {
    lVar2 = FUN_0012f9b8(param_1);
    lVar3 = lVar2;
    bVar1 = (int)(lVar2 != 0);
  }
  return bVar1;
}
