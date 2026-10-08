extern unsigned char D_00139568[16];
extern unsigned int D_0018b068;
extern unsigned char D_0018c0d4;
extern unsigned int D_00239ba0[];
extern unsigned short D_00239c28[];

int FUN_0028E260(void)
{
    unsigned char idx;
    unsigned int offset;
    unsigned short val;

    idx = D_0018b068;
    offset = (unsigned int)idx * 0xe0;
    val = ((unsigned short*)((unsigned char*)D_00239c28 + offset))[0];
    if (val == 0) {
        idx = 0;
    }
    if (D_0018c0d4 != 0) {
        idx = 0;
    }
    return (int)idx;
}