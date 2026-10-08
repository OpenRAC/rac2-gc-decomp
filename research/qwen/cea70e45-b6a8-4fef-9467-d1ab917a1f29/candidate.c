extern short D_001B7FA0;
extern int D_001B8308;
extern int D_001B830C;
extern int D_001B8310;
extern int D_001B8808;

int FUN_002798A0(int arg0, int* arg1, int* arg2)
{
    *arg1 = (int)D_001B7FA0;
    *arg2 = *(short*)((char*)&D_001B7FA0 + 2) + arg0;

    int i = *arg1;
    if (i >= 0 && i < D_001B8808) {
        do {
            int idx = i * 0x28;
            if (D_001B830C + idx) {
                int val1 = *(int*)((char*)&D_001B830C + idx);
                int val2 = *(int*)((char*)&D_001B8310 + idx);
                if (val1 + *arg2 <= val2) {
                    return 1;
                }
                int next = i + 1;
                if (next >= D_001B8808) {
                    return 0;
                }
                int prev_val = *(int*)((char*)&D_001B8308 + idx);
                int next_val = *(int*)((char*)&D_001B8308 + next * 0x28);
                if (prev_val != next_val) {
                    return 0;
                }
                *arg2 = (*arg2 - 1) - (val2 - val1);
                i = *arg1 + 1;
                *arg1 = i;
            } else {
                break;
            }
        } while (i >= 0 && i < D_001B8808);
    }
    return 0;
}