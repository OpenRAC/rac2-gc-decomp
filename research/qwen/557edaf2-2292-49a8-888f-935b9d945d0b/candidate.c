void FUN_00348068(int, int);

void FUN_00342380(int param_1, int param_2) {
  *(int *)(param_1 + 0x250) = param_2;
  FUN_00348068(param_1 + 0x188, param_2);
  return;
}
