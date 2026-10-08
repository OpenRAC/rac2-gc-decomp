typedef unsigned int undefined4;

void SndIopCommandContinuing(undefined4 param_1, undefined4 param_2, undefined4 *param_3, undefined4 param_4, undefined4 param_5);

void FUN_00133400(undefined4 param_1)

{
  undefined4 *pvStack_20;
  
  pvStack_20 = (undefined4 *)(-0x20 + (undefined8)__builtin_frame_address(0));
  pvStack_20[0] = param_1;
  SndIopCommandContinuing(0x2e,4,pvStack_20,0,0);
  return;
}