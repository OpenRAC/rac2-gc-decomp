extern unsigned *D_00133E74;

unsigned FUN_001163B0(void)
{
    unsigned *state_ptr = D_00133E74 + (0x58 / 4);
    unsigned state = *state_ptr;
    state = state * 0x41c64e6d + 0x3039;
    *state_ptr = state;
    return state & 0x7fffffff;
}
