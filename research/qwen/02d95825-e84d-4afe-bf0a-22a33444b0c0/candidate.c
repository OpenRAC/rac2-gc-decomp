int FUN_00123930(int *param_1)
{
  int count = 0;
  int offset = *param_1;
  if (offset) {
    int val = param_1[1];
    int *next_base;
    int next_off;
    while (1) {
      if (val) {
        int cond = param_1[2];
        if (cond) count++;
      }
      next_base = (int*)((char*)param_1 + offset + 4);
      next_off = next_base[0];
      if (!next_off) break;
      param_1 = next_base;
      offset = next_off;
      val = param_1[1];
    }
  }
  return count;
}
