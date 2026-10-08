void SndIopCommandContinuing(unsigned long long, unsigned int, unsigned int, long, unsigned long long);

void FUN_00132978(unsigned int param_1, unsigned int param_2)
{
    char frame[32];
    unsigned int *uStack_20 = (unsigned int *)frame;

    uStack_20[0] = param_1;
    uStack_20[1] = param_2;
    SndIopCommandContinuing(0xd, 8, (unsigned int)uStack_20, 0, 0);
}