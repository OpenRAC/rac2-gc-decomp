long GetMemorySize(void);
void TlbConfigReport(void);
void _InitTLB(void);

void FUN_0011F130(void)
{
  long memSize;

  memSize = GetMemorySize();
  if (memSize == 0x2000000L) {
    TlbConfigReport();
    goto done;
  }
  _InitTLB();
done:
  return;
}