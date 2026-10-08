int FUN_00283150(unsigned char *pbPtr, int iCount)
{
  int iAccum;
  unsigned char bLo;
  unsigned char bHi;

  iAccum = 0;
  while (0 < iCount) {
    bLo = *pbPtr;
    bHi = *(pbPtr + 1);
    pbPtr += 2;
    iCount -= 2;
    iAccum += (int)bLo + (int)bHi;
  }
  return iAccum;
}
