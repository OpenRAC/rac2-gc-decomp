extern unsigned char D_001A7BC8[16];
extern volatile unsigned short D_001AA580;
extern volatile unsigned short D_001AA598;
extern volatile unsigned short D_001AA5B0;
extern volatile unsigned short D_001AA5C8;
extern volatile unsigned short D_001AA5E0;

void FUN_002CCED0(void)
{
  unsigned char v0;
  unsigned short v1;

  v0 = D_001A7BC8[1];
  if (v0 != 0) {
    v1 = 3;
  } else {
    v1 = 0;
  }
  D_001AA580 = v1;

  v0 = D_001A7BC8[2];
  if (v0 != 0) {
    v1 = 3;
  } else {
    v1 = 0;
  }
  D_001AA598 = v1;

  v0 = D_001A7BC8[3];
  if (v0 != 0) {
    v1 = 0;
  } else {
    v1 = 3;
  }
  D_001AA5B0 = v1;

  v0 = D_001A7BC8[4];
  if (v0 != 0) {
    v1 = 0;
  } else {
    v1 = 3;
  }
  D_001AA5C8 = v1;

  v0 = D_001A7BC8[6];
  if (v0 != 0) {
    v1 = 0;
  } else {
    v1 = 3;
  }
  D_001AA5E0 = v1;
}