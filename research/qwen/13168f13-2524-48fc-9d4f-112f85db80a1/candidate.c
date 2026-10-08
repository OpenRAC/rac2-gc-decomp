extern void SndIopCommandContinuing(int a0, int a1, void* a2, int a3, int a4);

void FUN_00132AF8(int param_1)
{
  void *stack[4];
  stack[0] = (void*)param_1;
  SndIopCommandContinuing(0x16, 4, stack, 0, 0);
}
