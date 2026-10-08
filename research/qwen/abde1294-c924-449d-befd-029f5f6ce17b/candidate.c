typedef unsigned int uint32;
typedef unsigned char uint8;

extern uint8 D_00265290[];
extern uint8 D_001A7A08[];

uint32 FUN_00341558(uint32 *param_1)
{
    uint32 idx = *(uint32 *)((char *)param_1 + 0x2fc);
    uint32 *base = (uint32 *)D_00265290;
    uint32 *slot = base + idx * 9;
    uint32 res = *slot;

    if ((idx == 2) && (D_001A7A08[0] == 0) && (idx < *(uint32 *)((char *)param_1 + 0x1c0))) {
        res = 0;
    }

    return res;
}
