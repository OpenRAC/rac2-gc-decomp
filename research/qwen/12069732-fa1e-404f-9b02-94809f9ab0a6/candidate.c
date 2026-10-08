void FUN_00132a70(unsigned int param_1)
{
  unsigned int local_20 [4];

  local_20[0] = param_1;
  SndIopCommandContinuing(0x15, 4, (int)local_20, 0, 0);
  return;
}