typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_4_BARLOW_D_001A8EB0;
void LVL_4_BARLOW_FUN_002F8130(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_4_BARLOW_FUN_00314B18(s32 index) {
    NativeTable20 values=LVL_4_BARLOW_D_001A8EB0;
    return values.items[index];
}

u32 LVL_4_BARLOW_FUN_002D44B8(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_4_BARLOW_D_0018C0B4 __attribute__((nosda));
u32 LVL_4_BARLOW_FUN_002F70D8(void) {
    return LVL_4_BARLOW_D_0018C0B4;
}

s32 LVL_4_BARLOW_FUN_003042C8(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_4_BARLOW_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_4_BARLOW_FUN_002D4350(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_4_BARLOW_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_4_BARLOW_FUN_002D4388(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_4_BARLOW_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_4_BARLOW_FUN_002D4FA8(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_4_BARLOW_FUN_002F7E88(f32, f32, f32, f32, s32, s32);

void LVL_4_BARLOW_FUN_002F7930(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_4_BARLOW_FUN_002F7E88(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_4_BARLOW_FUN_00311370(int width, int height, int address, int mode);
extern void LVL_4_BARLOW_FUN_0039EFA0(unsigned int reg, unsigned long value);
extern void LVL_4_BARLOW_FUN_003116D8(int width, int height);

void LVL_4_BARLOW_FUN_00306068(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_4_BARLOW_FUN_00311370(width, height, address, 1);
    LVL_4_BARLOW_FUN_0039EFA0(0x47, 0x30000UL);
    LVL_4_BARLOW_FUN_0039EFA0(0x42, 0x8000000044UL);
    LVL_4_BARLOW_FUN_003116D8(0x100, 0x100);
    LVL_4_BARLOW_FUN_0039EFA0(0x42, 0x8000000044UL);
}

s32 LVL_4_BARLOW_FUN_002D43C0(s32 index) {
 s32 value=LVL_4_BARLOW_FUN_002D4388(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_4_BARLOW_FUN_002D43F8(s32 index) {
 return LVL_4_BARLOW_FUN_002D4388(index)==47;
}

s32 LVL_4_BARLOW_FUN_002D96D0(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

typedef struct {
    u32 unknown;
    s32 data_offset;
    s32 length;
    s32 extra;
} PackedHeaderCountView;

s32 LVL_4_BARLOW_FUN_003A1540(const PackedHeaderCountView *header) {
    s32 fixed = header->extra + 16;
    s32 total = header->data_offset + fixed + header->length;
    return ((total + 15) / 16) * 4;
}

f32 LVL_4_BARLOW_FUN_004554F8(f32 value) {
    f32 squared = value * value;
    value = value * -2.0f;
    value = value + 3.0f;
    return squared * value;
}

/* Ordinary integer controller and queue-launch algorithm; static proposals. */
typedef struct {
    u8 prefix[0x30];
    s32 field30;
    s32 field34;
    s32 field38;
    s32 field3c;
    s32 field40;
    s32 field44;
} BarlowUtilityQueueView;

typedef struct {
    u8 prefix[0x20];
    u8 field20;
    u8 gap21[0x47];
    u8 *field68;
    u8 gap6c[0x44];
    u8 fieldb0;
} BarlowMoby4439View;

extern s32 LVL_4_BARLOW_D_001A8F00 __attribute__((sda));
extern s32 LVL_4_BARLOW_D_001A79F0;
extern u8 LVL_4_BARLOW_D_0019B2E8[];
extern u8 *LVL_4_BARLOW_D_001B24DC;
extern BarlowUtilityQueueView LVL_4_BARLOW_D_001BF880;

extern s32 LVL_4_BARLOW_FUN_00314DB0(BarlowUtilityQueueView *, s32 *, s32 *);
extern s32 LVL_4_BARLOW_FUN_0034E170(s32, s32, u32, u32, u8 *);
extern void LVL_4_BARLOW_FUN_00314EC8(BarlowUtilityQueueView *);
extern u32 LVL_4_BARLOW_FUN_0034C4C8(const u8 *);
extern void LVL_4_BARLOW_FUN_00333408(u32);
extern s32 LVL_4_BARLOW_FUN_00314E50(BarlowUtilityQueueView *, s32);
extern void LVL_4_BARLOW_FUN_00394268(void);
extern void LVL_4_BARLOW_FUN_003317E8(u8 *);

s32 LVL_4_BARLOW_FUN_00314F98(BarlowUtilityQueueView *queue) {
    if (queue->field38 != 0) {
        s32 request[2];
        s32 status;
        s32 mode;
        queue->field3c = 0;
        queue->field44 = 1;
        request[0] = 0;
        request[1] = 0;
        LVL_4_BARLOW_FUN_00314DB0(queue, &request[0], &request[1]);
        status = 0;
        mode = 1;
        if ((u32)(LVL_4_BARLOW_D_001A8F00 - 1) < 2) mode = 2;
        queue->field40 = mode;
        if (request[1] == 0)
            status = LVL_4_BARLOW_FUN_0034E170(1, mode, 0, request[0], 0);
        else if (request[1] == 1)
            status = LVL_4_BARLOW_FUN_0034E170(2, mode, request[0], 0, 0);
        if (status < 0) LVL_4_BARLOW_FUN_00314EC8(queue);
        return status == 0;
    }
    return 0;
}

/* Update the measured fields of the first matching key among13records. */
typedef struct { unsigned int fields[25]; int key; unsigned int busy; unsigned int tail[9]; } UpdateRecord;
typedef char UpdateRecordSize[(sizeof(UpdateRecord) == 144) ? 1 : -1];
extern UpdateRecord LVL_4_BARLOW_D_00296C00[13];
void LVL_4_BARLOW_FUN_00317C20(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_4_BARLOW_D_00296C00[i].key == key) break;
    }
    if (i < 13) {
        LVL_4_BARLOW_D_00296C00[i].fields[9] = value;
        if (LVL_4_BARLOW_D_00296C00[i].busy == 0)
            LVL_4_BARLOW_D_00296C00[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_4_BARLOW_D_001395B8[];
u32 LVL_4_BARLOW_FUN_00326290(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_4_BARLOW_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}
