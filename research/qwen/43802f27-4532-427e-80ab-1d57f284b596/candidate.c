extern void Deci2Call(long long, void *);

void FUN_0011BA58(unsigned int param_1, unsigned int param_2, unsigned int param_3)
{
    struct {
        unsigned int a;
        unsigned int b;
        unsigned int c;
        unsigned int pad;
    } s;

    s.a = param_1;
    s.b = param_2;
    s.c = param_3 & 0xffff;

    Deci2Call(-6LL, &s);
}