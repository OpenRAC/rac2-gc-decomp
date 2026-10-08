void FUN_002a0488(void *, unsigned int, unsigned int, unsigned int);

void FUN_002aec18(void *param_1, unsigned int param_2)
{
  unsigned char b0 = param_2 & 0xFF;
  unsigned char b1 = (param_2 >> 8) & 0xFF;
  unsigned char b2 = (param_2 >> 16) & 0xFF;
  FUN_002a0488(param_1, b0, b1, b2);
}
