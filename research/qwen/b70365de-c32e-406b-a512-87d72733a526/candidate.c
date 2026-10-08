void FUN_00132C48();

void FUN_001338F0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5)
{
  unsigned int stack_save[12];

  stack_save[0] = param_1;
  stack_save[1] = param_2;
  stack_save[2] = param_3;
  stack_save[3] = param_4;
  stack_save[4] = param_5;

  FUN_00132C48(0x3e, 0x14, &stack_save[0]);
}