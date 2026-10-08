typedef struct {
    int kind;
    unsigned int sign;
    int exponent;
    int pad;
    unsigned long long fraction;
} DoubleParts44;

unsigned long long FUN_00122630(DoubleParts44 *param_1)
{
    unsigned int uVar1;
    unsigned long long uVar2;
    unsigned long long uVar3;
    unsigned long long uVar4;

    uVar1 = param_1->kind;
    uVar4 = 0;
    uVar2 = param_1->fraction;
    if (uVar1 < 2) {
        uVar4 = 0x7ff;
        uVar3 = uVar2 | 0x8000000000000ULL;
        goto LAB_00122700;
    }
    if (uVar1 == 4) {
LAB_001226bc:
        uVar4 = 0x7ff;
        uVar3 = 0;
    }
    else {
        if (uVar1 == 2) {
            uVar3 = 0;
            goto LAB_00122700;
        }
        uVar3 = 0;
        if (uVar2 == 0) goto LAB_00122700;
        uVar1 = param_1->exponent;
        if (uVar1 < -0x3fe) {
            uVar2 = uVar2 >> (unsigned long long)(int)(-0x3fe - uVar1);
            if (0x38 < (unsigned long long)(int)(-0x3fe - uVar1)) {
                uVar2 = 0;
            }
        }
        else {
            uVar4 = (unsigned long long)(int)(uVar1 + 0x3ff);
            if (0x3ff < uVar1) goto LAB_001226bc;
            if ((uVar2 & 0xffULL) == 0x80ULL) {
                if ((uVar2 & 0x100ULL) != 0ULL) {
                    uVar2 = uVar2 + 0x80ULL;
                }
            }
            else {
                uVar2 = uVar2 + 0x7fULL;
            }
            if (uVar2 < 0x2000000000000000ULL) {
                uVar3 = uVar2 >> 8;
                goto LAB_00122700;
            }
            uVar2 = uVar2 >> 1;
            uVar4 = (unsigned long long)(int)(uVar1 + 0x400);
        }
        uVar3 = uVar2 >> 8;
    }
LAB_00122700:
    {
        unsigned long long mantissa = uVar3 & 0xfffffffffffffULL;
        unsigned long long exponent_field = (uVar4 & 0x7ffULL) << 20;
        unsigned long long sign_bit = (unsigned long long)(int)param_1->sign << 63;
        return mantissa | exponent_field | sign_bit;
    }
}