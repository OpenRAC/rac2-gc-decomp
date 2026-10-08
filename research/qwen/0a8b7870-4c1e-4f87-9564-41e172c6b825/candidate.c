extern unsigned int D_00214F90;
extern unsigned int D_00214F9C;

extern int FUN_002ED688(int);

void FUN_002ED9C0(void)
{
  int ret = FUN_002ED688(D_00214F90);
  if (ret != 0 && *(int*)(ret + 0x118) == 1) {
    *(int*)(ret + 0x118) = 0;
    D_00214F9C--;
  }
}
