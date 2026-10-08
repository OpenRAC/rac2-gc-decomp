extern void FUN_00123028(long long, long long);

int FUN_00131400(void *arg)
{
    long long val = *(long long *)(arg + 8);
    FUN_00123028(val, val);
    return 0;
}