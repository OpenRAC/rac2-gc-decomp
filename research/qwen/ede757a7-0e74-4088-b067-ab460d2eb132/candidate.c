void FUN_00336ed0(int, unsigned int *);

void FUN_00335eb0(int param_1, int param_2)
{
  if (param_2 != *(unsigned int *)(param_1 + 4)) {
    if (*(unsigned int *)(param_1 + 0x2c) == 0) {
      *(unsigned int *)(param_1 + 4) = param_2;
    }
    else if (*(unsigned int *)(param_1 + 0x18) == 0) {
      FUN_00336ed0(param_1, (unsigned int *)(param_1 + 0x18));
      *(unsigned int *)(param_1 + 0x18) = 1;
      *(unsigned int *)(param_1 + 4) = param_2;
    }
    else {
      *(unsigned int *)(param_1 + 4) = param_2;
    }
  }
  return;
}
