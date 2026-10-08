void FUN_00119BF8(int arg0, int *arg1, char *arg2, unsigned long *arg3);

void FUN_00119BC8(void *param_1, void *param_2, void *param_3);

void FUN_00119BC8(void *param_1, void *param_2, void *param_3)
{
  FUN_00119BF8(*(int *)((char *)param_1 + 0x54), (int *)param_1, (char *)param_2, (unsigned long *)param_3);
}