void FUN_00133710(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5);

extern void SndIopCommandContinuing(unsigned long long arg1, int arg2, int arg3, long arg4, unsigned long long arg5);

void FUN_00133710(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5)
{
  unsigned int locals[5];

  locals[0] = param_1;
  locals[1] = param_2;
  locals[2] = param_3;
  locals[3] = param_4;
  locals[4] = param_5;
  SndIopCommandContinuing((unsigned long long)param_1, param_2, param_3, (long)locals, (unsigned long long)param_5);
}