/* Reproduce exact control-flow and stack behavior of target */
extern void FUN_00132C48(int, int, void *);
void FUN_00133850(int, int, int, int, int, int);

void FUN_00133850(int p1, int p2, int p3, int p4, int p5, int p6)
{
  int stack[6];
  stack[0] = p1;
  stack[1] = p2;
  stack[2] = p3;
  stack[3] = p4;
  stack[4] = p5;
  stack[5] = p6;
  FUN_00132C48(0x3b, 0x18, stack);
}