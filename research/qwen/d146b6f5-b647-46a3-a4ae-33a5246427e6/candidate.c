int FUN_0029af58(int *);

extern unsigned int FUN_0029aeb0(char *, long);
extern int FUN_0029ae78(int *);

int FUN_0029af58(int *param_1)
{
  int iVar1;
  int iVar3;
  int bVar2;
  
  bVar2 = 0;
  iVar1 = param_1[1];
  if (iVar1 != 0) {
    iVar3 = FUN_0029aeb0((char *)(param_1 + 2), (long)*param_1);
    bVar2 = iVar3 == iVar1;
  }
  return bVar2;
}