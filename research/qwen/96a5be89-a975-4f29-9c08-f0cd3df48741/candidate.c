struct S {
  long long x0;
  long long x1;
  long long x2;
  long long x3;
  long long x4;
  long long x5;
};

void FUN_00119BC8(long, long, struct S *);

void FUN_00115CF0(long p1, long p2, long p3, long p4, long p5, long p6, long p7, long p8)
{
  char buf[80];
  struct S *s = (struct S *)(buf + 80);
  s->x0 = p3;
  s->x1 = p4;
  s->x2 = p5;
  s->x3 = p6;
  s->x4 = p7;
  s->x5 = p8;
  FUN_00119BC8(p1, p2, s);
}