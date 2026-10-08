extern unsigned int *D_001A6D40;
extern unsigned int *D_001A6E90;
extern unsigned int *D_001B22A8;

void FUN_00284F40(long param_1) {
    unsigned int *queue = D_001B22A8;
    if (queue != (unsigned int *)0) {
        queue[0] = 0x30000015;
        if (param_1 == 0) {
            queue[1] = (unsigned int)D_001A6D40;
        } else {
            queue[1] = (unsigned int)D_001A6E90;
        }
        queue[2] = 0;
        queue[3] = 0x50000015;
        D_001B22A8 = queue + 4;
    }
}