extern void LibmcMcservTooOld(void);
extern void FUN_0026feb8(void);

void MemcardInitFailed(void)
{
    void *lVar1;
    lVar1 = (void *)LibmcMcservTooOld();
    if (lVar1 != (void *)0)
    {
        FUN_0026feb8();
    }
}