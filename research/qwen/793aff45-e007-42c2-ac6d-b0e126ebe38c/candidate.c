void FUN_00123028(unsigned long long param_1, unsigned long long param_2);

void FUN_00122760(unsigned long long *in_a0, unsigned int *in_a1);

int FUN_00122F10(unsigned int *in_a0, unsigned int *in_a1);

void FUN_00123028(unsigned long long param_1, unsigned long long param_2)
{
  struct {
    unsigned char auStack_70[32];
    unsigned char auStack_50[32];
    unsigned long long uStack_30;
    unsigned long long uStack_28;
  } local;

  local.uStack_30 = param_1;
  local.uStack_28 = param_2;
  FUN_00122760(&local.uStack_30, (unsigned int *)local.auStack_70);
  FUN_00122760(&local.uStack_28, (unsigned int *)local.auStack_50);
  FUN_00122F10((unsigned int *)local.auStack_70, (unsigned int *)local.auStack_50);
  return;
}