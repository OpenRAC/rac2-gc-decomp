extern unsigned long D_0013CF64;
extern unsigned long D_0013CF6C;

void FUN_0011CB90(int index, unsigned int val1, unsigned int val2)
{
  unsigned int *ptr;

  if (index < 0)
    ptr = (unsigned int *)(D_0013CF64 + (index << 3));
  else
    ptr = (unsigned int *)(D_0013CF6C + (index << 3));

  *ptr = val1;
  ptr[1] = val2;
}
