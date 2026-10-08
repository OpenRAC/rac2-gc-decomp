typedef unsigned int undefined4;
typedef unsigned long undefined8;

void FUN_00132DF0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5);

void FUN_00132BC0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined8 param_7,undefined8 param_8)
{
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;

  uStack_30 = param_1;
  uStack_2c = param_2;
  uStack_28 = param_3;
  uStack_24 = param_4;
  uStack_20 = param_5;
  uStack_1c = param_6;
  uStack_18 = param_7;
  uStack_14 = param_8;
  FUN_00132DF0(0x21,0x18,(undefined4)&uStack_30,(undefined4)(param_7 & 0xffffffff),(undefined4)(param_7 >> 32));
  return;
}
