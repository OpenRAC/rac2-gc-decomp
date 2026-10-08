void SndIopCommandContinuing(unsigned int, unsigned int, void *, unsigned int, unsigned int);

void FUN_001333d0(unsigned int param_1)
{
  unsigned int local_20[4];

  local_20[0] = param_1;
  SndIopCommandContinuing(0x2d, 4, local_20, 0, 0);
  return;
}