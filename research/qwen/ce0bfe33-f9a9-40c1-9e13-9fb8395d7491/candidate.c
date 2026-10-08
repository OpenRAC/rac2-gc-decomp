void SndIopCommandContinuing(int param1, int param2, void* param3, int param4, int param5);

void FUN_00132858(int param_1)
{
    volatile int auStack_20[4];
    
    auStack_20[0] = param_1;
    SndIopCommandContinuing(6, 4, auStack_20, 0, 0);
}