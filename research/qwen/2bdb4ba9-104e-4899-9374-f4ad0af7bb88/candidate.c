extern int D_001F2840;
extern int D_001F2930;
extern int D_001F2A30;

void FUN_002C9A60(void)
{
  int *base = &D_001F2840;
  base[0xf0/sizeof(int)] = 0;
  base[0x1f0/sizeof(int)] = 0;
}