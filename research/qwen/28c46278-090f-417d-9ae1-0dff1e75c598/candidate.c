void FUN_002fcf90(int param_1)
{
  int *piVar1;
  int *piVar2;

  piVar1 = D_001B22A8;
  piVar2 = D_001B22A8 + 4;
  *D_001B22A8 = param_1 + -0x70000000;
  D_001B22A8 = piVar2;
  piVar1[3] = 0;
  piVar1[1] = 0;
  piVar1[2] = 0;
  return;
}
