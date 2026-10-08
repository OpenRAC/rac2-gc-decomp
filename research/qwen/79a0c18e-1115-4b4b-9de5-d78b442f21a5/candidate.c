void SndIopCommandContinuing(unsigned long long, int, int, long, unsigned long long);

void FUN_00132A10(unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5, unsigned int p6, unsigned long long p7, unsigned long long p8)
{
  unsigned int buf[6];

  buf[0] = p1;
  buf[1] = p2;
  buf[2] = p3;
  buf[3] = p4;
  buf[4] = p5;
  buf[5] = p6;

  SndIopCommandContinuing(0x11, 0x18, (int)buf, (long)(unsigned int)p7, p8);
}