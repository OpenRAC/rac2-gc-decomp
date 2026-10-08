/* Candidate: correct helper call uses arg1 as pointer, not arg0+0x10. */
extern void FUN_00336ED0(unsigned char *, unsigned char *);

void FUN_00335F20(unsigned char *arg0, int arg1)
{
  int val10;
  int p2c;
  int p24;

  val10 = *(int *)(arg0 + 0x10);

  if (arg1 == val10) {
    return;
  }

  p2c = *(int *)(arg0 + 0x2c);
  if (p2c == 0) {
    *(int *)(arg0 + 0x10) = arg1;
    return;
  }

  p24 = *(int *)(arg0 + 0x24);
  if (p24 == 0) {
    FUN_00336ED0(arg0, (unsigned char *)arg1);
    *(int *)(arg0 + 0x24) = 1;
  }

  *(int *)(arg0 + 0x10) = arg1;
}
