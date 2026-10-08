void SndIopCommandContinuing(unsigned int param1, unsigned int param2, void *param3, unsigned int param4, unsigned int param5);

void FUN_00132888(unsigned int param1, unsigned int param2)
{
    struct {
        unsigned int pad[4];
    } locals;

    locals.pad[0] = param1;
    locals.pad[1] = param2;
    SndIopCommandContinuing(9U, 8U, &locals, 0U, 0U);
}