int FUN_002b0d60(void);
int FUN_002b0db8(void);

int FUN_002b0d70(void)
{
  int a = FUN_002b0db8();
  int b = FUN_002b0d60();
  int d = a - b;

  if (d < 0) {
    d = 0;
  }

  if (d < 41) {
    return d;
  }

  return 40;
}
