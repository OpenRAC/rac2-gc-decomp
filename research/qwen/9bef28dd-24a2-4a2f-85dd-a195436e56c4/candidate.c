void FUN_00133A28(int, int, int);

void FUN_00133A78(void)
{
  const int c2_high = 0x15;
  const int c2_low  = -0x4ac0;
  const int c2      = (c2_high << 16) + c2_low;
  FUN_00133A28(0x3e9, 0xb, c2);
}