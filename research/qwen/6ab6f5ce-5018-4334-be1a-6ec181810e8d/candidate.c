float FUN_00283CC8(float x, float *out_int)
{
  int i = (int)x;
  float fi = *(float *)&i;
  *out_int = fi;
  return x - fi;
}
