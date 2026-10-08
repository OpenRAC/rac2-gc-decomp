void FUN_002B9180(void *);
int FUN_00282978(short *);

void FUN_002BF6F0(void *param_1)
{
  int lVar1;

  lVar1 = FUN_00282978((short *)((char *)param_1 + 10));
  if (lVar1 != 0) {
    FUN_002B9180(param_1);
  }
  return;
}