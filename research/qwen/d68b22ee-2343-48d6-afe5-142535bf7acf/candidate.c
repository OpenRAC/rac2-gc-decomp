void FUN_00282C88(void);

struct S {
    char pad0[16];
    volatile int f1;
    volatile int f2;
    char pad1[4];
    volatile float f3;
    volatile int f4;
};

void FUN_002A8BD0(void *base, int val1, int val2, float fval)
{
    struct S *s = base;
    s->f1 = val1;
    s->f2 = val2;
    s->f3 = fval;
    s->f4 = 0;
    FUN_00282C88();
}