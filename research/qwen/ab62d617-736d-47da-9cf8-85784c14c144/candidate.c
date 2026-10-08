void FUN_0012F950(int *param_1, unsigned int param_2, unsigned int param_3);

void FUN_0012F950(int *param_1, unsigned int param_2, unsigned int param_3)
{
  int base = *(int *)(param_1 + 0x10);
  *(unsigned int *)(base + 0xb0) = 1;
  *(unsigned int *)(base + 0xd8) = (param_2 & 0xfffffff) | 0x20000000;
  *(unsigned int *)(base + 0xe4) = param_3;
  *(unsigned int *)(base + 0xdc) = 0;
  *(unsigned int *)(base + 0xe0) = 0;
  MpegImageBufferAlignmentError();
}

extern int MpegImageBufferAlignmentError(void);