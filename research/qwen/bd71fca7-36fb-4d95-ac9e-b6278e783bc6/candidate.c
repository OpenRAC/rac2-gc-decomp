void SndIopCommandContinuing(long param_1, int param_2, int param_3, long param_4, long param_5);

void FUN_00133460(int param_1, long param_2, long param_3) {
  int local_stack[4];
  local_stack[0] = param_1;
  SndIopCommandContinuing(0x4f, 4, (int)local_stack, param_2, param_3);
}
