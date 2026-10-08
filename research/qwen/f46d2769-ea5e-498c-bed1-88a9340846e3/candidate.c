void FUN_0028b950(void);

void FUN_0028ba50(volatile int param_1)
{
  *(int *)(param_1 + 0x7c) = 0xd2;
  do {
    *(short *)(param_1 + 0x48) = 0;
    *(short *)(param_1 + 0x4a) = 0;
  } while(0);
  FUN_0028b950();
}