void FUN_002a04b8(int param_1, unsigned int *param_2, unsigned int *param_3, unsigned int *param_4)
{
  unsigned long long uVal;
  unsigned int uVar1;

  uVal = *(unsigned long long *)(param_1 + 0x38);
  uVar1 = (unsigned int)(uVal >> 32);
  *param_2 = uVar1 & 0xff;
  *param_3 = (uVar1 >> 8) & 0xff;
  *param_4 = (unsigned int)(uVal >> 48) & 0xff;
}