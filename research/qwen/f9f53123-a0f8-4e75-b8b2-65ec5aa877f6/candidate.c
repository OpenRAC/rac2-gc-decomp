typedef unsigned long long undefined8;
typedef unsigned int undefined4;

extern undefined4 FUN_002DE618(undefined4);

undefined8 FUN_002DBB48(int param_1)
{
  undefined4 uVar1;
  uVar1 = FUN_002DE618(*(undefined4 *)(param_1 + 0x54));
  *(undefined4 *)(param_1 + 0x54) = uVar1;
  return 0;
}
