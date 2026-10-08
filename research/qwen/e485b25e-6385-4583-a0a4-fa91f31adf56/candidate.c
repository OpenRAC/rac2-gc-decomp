/* MIPS function: FUN_0033E480
 * Signature: undefined8 FUN_0033E480(undefined8 param_1)
 * Disassembly verified at 0x0033e480-0x0033e4b3.
 * Calls FUN_0033AB50(param_1 + 8) and FUN_00336968(param_1 + 0x2b0), returns param_1.
 */
typedef void (*fun335fb0_t)(int);
typedef void (*fun3361a8_t)(int);
typedef void (*fun336968_t)(int);
typedef void (*fun33ab50_t)(int);

extern fun335fb0_t FUN_00335fb0;
extern fun3361a8_t FUN_003361a8;
extern fun336968_t FUN_00336968;
extern fun33ab50_t FUN_0033ab50;

void FUN_00335fb0(int param_1)
{
  *(unsigned char **)(param_1 + 0x30) = (unsigned char *)0x001ada78;
}

void FUN_003361a8(int param_1)
{
  FUN_00335fb0(param_1);
  *(unsigned char **)(param_1 + 0x30) = (unsigned char *)0x001ada58;
}

void FUN_00336968(int param_1)
{
  FUN_00335fb0();
  *(unsigned char **)(param_1 + 0x30) = (unsigned char *)0x001ada18;
}

void FUN_0033ab50(int param_1)
{
  int i;
  int p = param_1 + 0xc;
  for (i = 0; i < 4; i++) {
    FUN_003361a8(p);
    p += 0x4c;
  }
  FUN_00336968(param_1 + 0x198);
  FUN_00336968(param_1 + 0x1e0);
  FUN_00336968(param_1 + 0x228);
}

int FUN_0033E480(int param_1)
{
  FUN_0033ab50(param_1 + 8);
  FUN_00336968(param_1 + 0x2b0);
  return param_1;
}
