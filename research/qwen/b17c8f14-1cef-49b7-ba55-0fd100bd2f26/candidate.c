void FUN_00132938(undefined4 param_1);

void FUN_00132938(undefined4 param_1) {
  undefined4 auStack_20[4];

  auStack_20[0] = param_1;
  SndIopCommandContinuing(0xb, 4, auStack_20, 0, 0);
  return;
}