/* Authored C for the measured level-only clear-five-fields body. */
void LVL_24_SHIP_SHACK_FUN_002D6960(int *object) {
    object[0] = 0;
    object[1] = 0;
    object[2] = 0;
    object[3] = 0;
    object[4] = 0;
}

typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef float f32;
typedef long s64;
typedef unsigned long u64;
typedef char NativeWidths[(sizeof(s32)==4 && sizeof(s64)==8 && sizeof(void *)==4)?1:-1];
extern void LVL_24_SHIP_SHACK_FUN_002F5D18(void);
void LVL_24_SHIP_SHACK_FUN_002F5DE0(u8 *object) {
 LVL_24_SHIP_SHACK_FUN_002F5D18();
 *(s32 *)(object+0x58)=210;*(s32 *)(object+0x5c)=200;
 *(s32 *)(object+0x74)=-2;*(s32 *)(object+0x78)=30;
 *(u16 *)(object+0x48)=0;*(u16 *)(object+0x4a)=0;*(s32 *)(object+0x70)=0;
}

typedef struct {u32 items[5];} NativeTable20;
extern const NativeTable20 LVL_24_SHIP_SHACK_D_001A8EB0;
u32 LVL_24_SHIP_SHACK_FUN_002F2510(s32 index) {
 NativeTable20 values=LVL_24_SHIP_SHACK_D_001A8EB0;
 return values.items[index];
}

extern s32 LVL_24_SHIP_SHACK_D_001B1B70[] __attribute__((sda));
extern u32 LVL_24_SHIP_SHACK_D_001B1B74[] __attribute__((sda));
s32 LVL_24_SHIP_SHACK_FUN_0034F218(s32 key) {
 s32 count=0;
 u32 *flags=LVL_24_SHIP_SHACK_D_001B1B74;
 s32 *keys=LVL_24_SHIP_SHACK_D_001B1B70;
 do {
  count++;
  if(*keys!=key) {flags+=2;keys+=2;continue;}
  *flags|=4;return 0;
 }while(count<5);
 return 1;
}

typedef struct {
    u8 prefix[0xc40];
    s32 selected;
    s32 index;
    u8 gap[0x1648];
    u8 *object;
} NativeResidentView;
extern NativeResidentView LVL_24_SHIP_SHACK_D_00189E20;

void LVL_24_SHIP_SHACK_FUN_002D1570(s32 selected, s32 index) {
    if (selected >= 0) {
        u8 *object = LVL_24_SHIP_SHACK_D_00189E20.object;
        s32 offset = object[0x43] * 4;
        u8 *table = *(u8 **)(object + 0x24);
        u8 *entry = *(u8 **)(table + offset + 0x48);
        if (index < entry[0x10]) {
            LVL_24_SHIP_SHACK_D_00189E20.selected = selected;
            LVL_24_SHIP_SHACK_D_00189E20.index = index;
        }
    }
}

typedef struct {
 u8 prefix[0x30];s32 state;u8 gap0[0x1c];u8 *current;u8 gap1[0x138];s32 bank[2];
} NativeContext;
extern NativeContext LVL_24_SHIP_SHACK_D_001BC3C0;
extern u8 LVL_24_SHIP_SHACK_D_0028AAC0[];
void LVL_24_SHIP_SHACK_FUN_0035E348(void) {
 s32 index=-1;
 if(LVL_24_SHIP_SHACK_D_001BC3C0.state==9) index=1;
 if(index!=-1) {
  LVL_24_SHIP_SHACK_D_001BC3C0.current=LVL_24_SHIP_SHACK_D_0028AAC0;
  *(s32 *)(LVL_24_SHIP_SHACK_D_0028AAC0+0x68)=LVL_24_SHIP_SHACK_D_001BC3C0.bank[index];
 }
}