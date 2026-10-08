void FUN_002D5B58(short param_1, int param_2, int *param_3);

void FUN_002D5E98(int param_1)
{
  int auStack_20[4];

  auStack_20[0] = 0;
  FUN_002D5B58(*(short *)(param_1 + 2), *(int *)(param_1 + 4), &auStack_20[0]);
  return;
}