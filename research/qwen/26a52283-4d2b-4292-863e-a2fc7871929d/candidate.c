extern int DAT_001A8CFC;
extern void FUN_0033B910(int);

void FUN_0029C370(void)
{
    int val = DAT_001A8CFC;
    if (val != 0) {
        const int offset = 0x3b490;
        FUN_0033B910(val + offset);
    }
    return;
}