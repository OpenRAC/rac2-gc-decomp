void FUN_00335870(int a0, int a1);

extern void FUN_00336DA0(void);

void FUN_00335870(int a0, int a1)
{
  *(void **)(a0 + 4) = (void *)0x001AD9C8;
  if ((a1 & 1) != 0) {
    FUN_00336DA0();
  }
}
