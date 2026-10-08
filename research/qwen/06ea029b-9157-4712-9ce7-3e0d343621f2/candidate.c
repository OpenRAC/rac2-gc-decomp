void FUN_00133750(unsigned int param_1, unsigned int param_2);

void FUN_00133750(unsigned int param_1, unsigned int param_2)
{
  unsigned int uStack_20;
  unsigned int uStack_1c;

  uStack_20 = param_1;
  uStack_1c = param_2;
  SndIopCommandContinuing(0x51, 8, &uStack_20, 0, 0);
}