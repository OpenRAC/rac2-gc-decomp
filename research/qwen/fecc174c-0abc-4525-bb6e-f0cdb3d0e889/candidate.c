void Deci2Call(int, void *);

void FUN_0011BA20(unsigned int param_1, unsigned int param_2, unsigned int param_3)
{
  volatile unsigned int local_20;
  volatile unsigned int local_1c;
  volatile unsigned int local_18;

  local_20 = param_1;
  local_1c = param_2;
  local_18 = param_3 & 0xffff;

  Deci2Call(-5, (void *)&local_20);

  local_20 = local_20;
  local_1c = local_1c;
  local_18 = local_18;
}