int FUN_00341e90(void);

int FUN_00341e58(int param_1)
{
  int res = FUN_00341e90();
  int val = *(int *)(param_1 + 0xd8);
  int scaled = res * (int)((unsigned int)0x140000 >> 16);
  return val + scaled;
}
