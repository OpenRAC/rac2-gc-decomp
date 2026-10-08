typedef unsigned int uint;
typedef unsigned char uchar;

void Deci2Call(uint arg1, void *arg2);

void FUN_0011B9C8(uint param_1, uchar param_2)
{
    struct {
        uint uStack_20;
        int iStack_1c;
    } local;

    local.uStack_20 = param_1;
    local.iStack_1c = (int)(signed char)param_2;

    Deci2Call(3, &local.uStack_20);
}