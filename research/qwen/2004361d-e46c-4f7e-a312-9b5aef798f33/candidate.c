void FUN_0028B950(void);
void FUN_0028DF38(int param_1)
{
  volatile int *p = (volatile int *)param_1;
  p[0x17] = 0x96;
  p[0x16] = 0x20;
  p[0x14] = 0x20;
  FUN_0028B950();
}