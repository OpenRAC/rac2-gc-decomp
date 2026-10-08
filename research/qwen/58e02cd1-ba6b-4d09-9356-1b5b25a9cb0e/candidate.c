extern void FUN_00348068(void *);

void FUN_0033E368(unsigned long base, unsigned long value)
{
  FUN_00348068((void *)(base + 0x2b0));
  *(unsigned long *)(base + 0x378) = value;
}
