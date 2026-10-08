void FUN_00122760(unsigned long *a0, unsigned int *a1)
{
  unsigned long val;
  unsigned long mantissa;
  unsigned int exponent;
  unsigned int classification;

  val = *a0;
  mantissa = val & 0xfffffffffffffUL;
  a1[1] = (unsigned int)(val >> 63);

  exponent = (unsigned int)((val >> 52) & 0x7ffUL);

  if (exponent == 0) {
    classification = 2;
  } else if (exponent != 0x7ff) {
    mantissa |= 0x1000000000000000UL;
    a1[4] = (unsigned int)(mantissa >> 8);
    a1[2] = (int)exponent - 0x3ff;
    classification = 3;
  } else {
    if (mantissa == 0) {
      classification = 4;
    } else {
      if ((val & 0x8000000000000UL) == 0) {
        classification = 0;
      } else {
        classification = 1;
      }
      a1[4] = (unsigned int)mantissa;
    }
  }
  a1[0] = classification;
}