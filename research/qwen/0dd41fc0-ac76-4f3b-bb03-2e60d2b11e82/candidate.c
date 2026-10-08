void SndIopCommandContinuing(int param_1, int param_2, void* param_3, int param_4, int param_5);

void FUN_001329B0(int param_1, int param_2, int param_3)
{
    int uStack_20;
    int uStack_1c;
    int uStack_18;
    register int v0 = param_2;

    uStack_20 = param_1;
    uStack_18 = param_3;
    uStack_1c = v0;
    SndIopCommandContinuing(0x4e, 0xc, &uStack_20, 0, 0);
}