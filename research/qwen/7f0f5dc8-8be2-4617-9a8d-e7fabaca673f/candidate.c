int D_0013CF64;
int D_0013CF6C;

void FUN_0011CBC0(int param_1)
{
  int iVar1;
  int tmp;
  
  tmp = param_1 * 8;
  if (param_1 < 0) {
    iVar1 = D_0013CF64;
  } else {
    iVar1 = D_0013CF6C;
  }
  *(int *)(tmp + iVar1) = 0;
}