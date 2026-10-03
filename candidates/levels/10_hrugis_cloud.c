typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_10_HRUGIS_CLOUD_D_001A8EB0;
void LVL_10_HRUGIS_CLOUD_FUN_002F25E0(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_10_HRUGIS_CLOUD_FUN_0030E810(s32 index) {
    NativeTable20 values=LVL_10_HRUGIS_CLOUD_D_001A8EB0;
    return values.items[index];
}

u32 LVL_10_HRUGIS_CLOUD_FUN_002C8D50(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_10_HRUGIS_CLOUD_D_0018C0B4;
u32 LVL_10_HRUGIS_CLOUD_FUN_002F1620(void) {
    return LVL_10_HRUGIS_CLOUD_D_0018C0B4;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002FE708(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_10_HRUGIS_CLOUD_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_10_HRUGIS_CLOUD_FUN_002C8BE8(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_10_HRUGIS_CLOUD_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002C8C20(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_10_HRUGIS_CLOUD_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_10_HRUGIS_CLOUD_FUN_002C9840(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}
