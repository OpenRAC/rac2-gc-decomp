void FUN_00132C48(unsigned int param_1, int param_2, unsigned int* param_3);

void FUN_00133930(unsigned int param_1, unsigned int param_2)
{
  struct {
    unsigned int local_20;
    unsigned int local_1c;
    unsigned int pad[2]; /* padding to match original stack usage */
  } stack;

  stack.local_20 = param_1;
  stack.local_1c = param_2;
  FUN_00132C48(0x5a, 8, &stack.local_20);
  return;
}
