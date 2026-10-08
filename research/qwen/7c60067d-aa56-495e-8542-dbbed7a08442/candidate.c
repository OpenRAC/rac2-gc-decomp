void Deci2Call(int, void *);

void FUN_0011b978(unsigned int param_1, unsigned int param_2, unsigned int param_3)
{
  volatile unsigned int *p = (volatile unsigned int *)0x80000000;
  p[0] = param_1 & 0xffff;
  p[1] = param_2;
  p[2] = param_3;
  Deci2Call(1, &p[0]);
}