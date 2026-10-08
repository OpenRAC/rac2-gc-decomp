extern unsigned short *D_001B1700;

int FUN_0027A0F8(int param_1, int param_2, int param_3)
{
  unsigned short *base = D_001B1700;
  int result = 0;

  if (param_3 >= 0 && param_3 < (int)base[1] && base[2 + param_3] != 0) {
    unsigned short *sub = base + base[2 + param_3];
    if (param_2 >= 0 && param_2 < (int)sub[1] && sub[2 + param_2] != 0) {
      unsigned short *sub2 = sub + sub[2 + param_2];
      if (param_1 >= 0 && param_1 < (int)sub2[1]) {
        unsigned short val = sub2[2 + param_1];
        if (val != 0xffff) {
          result = (int)(base + (val * 0x80)) + (int)*base;
        }
      }
    }
  }

  return result;
}
