typedef float f32;
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef struct { u32 items[5]; } NativeTable20;
extern const NativeTable20 LVL_11_JOBA_D_001A8EB0;
void LVL_11_JOBA_FUN_002E9738(s32 *object) {
    object[0]=0; object[1]=0; object[2]=0; object[3]=0; object[4]=0;
}
u32 LVL_11_JOBA_FUN_00306280(s32 index) {
    NativeTable20 values=LVL_11_JOBA_D_001A8EB0;
    return values.items[index];
}

u32 LVL_11_JOBA_FUN_002C6358(u8 *object,u32 value) {
    if(value==255) value=object[0xa9];
    return value;
}

extern u32 LVL_11_JOBA_D_0018C0B4;
u32 LVL_11_JOBA_FUN_002E86E0(void) {
    return LVL_11_JOBA_D_0018C0B4;
}

s32 LVL_11_JOBA_FUN_002F58D0(f32 a,f32 b,f32 t) {
 f32 difference=b-a;
 f32 squared=t*t;
 a=a*t;
 difference=difference*squared;
 a=a+difference;
 return (s32)a;
}

extern u8 LVL_11_JOBA_D_00189E20[];

typedef struct {
    u8 prefix[0x1220];
    u32 field1220;
    u8 gap[0x20];
    s32 field1244, field1248;
} NativeIndexedFieldsView;

u32 LVL_11_JOBA_FUN_002C61F0(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_11_JOBA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1220;
 return 0;
}

s32 LVL_11_JOBA_FUN_002C6228(s32 index) {
 NativeIndexedFieldsView *p=(NativeIndexedFieldsView *)((u8 *)&LVL_11_JOBA_D_00189E20+index*0x50);
 if(p->field1244==2) return p->field1248;
 return -1;
}

s32 LVL_11_JOBA_FUN_002C6E48(s32 value) {
 switch(value) {case 1:return 0x72;case 2:return 0x70;case 3:return 0x6e;default:return 0;}
}

extern void LVL_11_JOBA_FUN_002E9490(f32, f32, f32, f32, s32, s32);

void LVL_11_JOBA_FUN_002E8F38(u8 *object) {
 object[0x1d]=0;
 if (*(f32 *)(object+0x10)>=0.0f)
  LVL_11_JOBA_FUN_002E9490(1.1243411302566528f,0.005f,0.2f,0.0f,0,3);
}

/* Scalar GS setup wrapper; pinned standard integer arguments and helper calls. */
extern void LVL_11_JOBA_FUN_00302AD8(int width, int height, int address, int mode);
extern void LVL_11_JOBA_FUN_0038FD70(unsigned int reg, unsigned long value);
extern void LVL_11_JOBA_FUN_00302E40(int width, int height);

void LVL_11_JOBA_FUN_002F7670(int width, int height)
{
    int sum = width + height;
    int address;

    if (sum > 16)
        sum = 16;
    address = 0x3ff000 - (4 << sum);
    address = (address >> 13) << 13;
    LVL_11_JOBA_FUN_00302AD8(width, height, address, 1);
    LVL_11_JOBA_FUN_0038FD70(0x47, 0x30000UL);
    LVL_11_JOBA_FUN_0038FD70(0x42, 0x8000000044UL);
    LVL_11_JOBA_FUN_00302E40(0x100, 0x100);
    LVL_11_JOBA_FUN_0038FD70(0x42, 0x8000000044UL);
}

extern void LVL_11_JOBA_FUN_002E54D8(f32, f32, f32, f32 *, s32);

void LVL_11_JOBA_FUN_002C6080(f32 *output) {
 LVL_11_JOBA_FUN_002E54D8(0.3f,0.0f,1.34f,output,1);
}

s32 LVL_11_JOBA_FUN_002C6260(s32 index) {
 s32 value=LVL_11_JOBA_FUN_002C6228(index);
 if(value==12 || value==17) return 1;
 return 0;
}

s32 LVL_11_JOBA_FUN_002C6298(s32 index) {
 return LVL_11_JOBA_FUN_002C6228(index)==47;
}

s32 LVL_11_JOBA_FUN_002CB668(u8 *object) {
 if(object==0) return 0;
 return *(short *)(object+0xaa)==71;
}

/* Advances a signed index, wrapping or clamping to the measured count. */
s32 LVL_11_JOBA_FUN_00355E88(const s32 *count, s32 index, s32 delta, s32 wrap) {
    if (wrap != 0) {
        index = (index + delta + *count) % *count;
    } else {
        index += delta;
        if (delta < 0) {
            if (index < 0) index = 0;
        } else if (index > *count - 1) {
            index = *count - 1;
        }
    }
    return index;
}

/* Update the measured fields of the first matching key among13records. */
typedef struct { unsigned int fields[25]; int key; unsigned int busy; unsigned int tail[9]; } UpdateRecord;
typedef char UpdateRecordSize[(sizeof(UpdateRecord) == 144) ? 1 : -1];
extern UpdateRecord LVL_11_JOBA_D_00295330[13];
void LVL_11_JOBA_FUN_003095B0(int key, unsigned int value)
{
    int i;
    for (i = 0; i < 13; i++) {
        if (LVL_11_JOBA_D_00295330[i].key == key) break;
    }
    if (i < 13) {
        LVL_11_JOBA_D_00295330[i].fields[9] = value;
        if (LVL_11_JOBA_D_00295330[i].busy == 0)
            LVL_11_JOBA_D_00295330[i].fields[1] = value;
    }
}

typedef struct {
    u8 prefix[0xa7];
    u8 fielda7;
} OrbitalBitmapView;
extern u8 LVL_11_JOBA_D_001395B8[];
u32 LVL_11_JOBA_FUN_00317D08(s32 index) {
    s32 slot=index/8;
    u32 bit=index%8;
    u32 previous=0;
    if(bit<8) {
        OrbitalBitmapView *state=(OrbitalBitmapView *)(LVL_11_JOBA_D_001395B8+slot);
        previous=((s32)state->fielda7>>bit)&1;
    }
    return previous;
}

/* Consumes one positive counter selected by the measured class identifier. */
typedef struct {
    u8 prefix[0xe0];
    s32 fielde0, fielde4, fielde8, fieldec, fieldf0, fieldf4;
} JobaClassCounterView;

s32 LVL_11_JOBA_FUN_00441868(u8 *object, s32 class_id) {
    JobaClassCounterView *state = *(JobaClassCounterView **)(object + 0x68);
    switch (class_id) {
        case 0xd98:
            if (state->fielde0 > 0) { state->fielde0--; return 1; }
            break;
        case 0xdcf:
            if (state->fielde4 > 0) { state->fielde4--; return 1; }
            break;
        case 0xdf4:
            if (state->fielde8 > 0) { state->fielde8--; return 1; }
            break;
        case 0xdf7:
            if (state->fieldec > 0) { state->fieldec--; return 1; }
            break;
        case 0xdf9:
            if (state->fieldf0 > 0) { state->fieldf0--; return 1; }
            break;
        case 0xd37:
            if (state->fieldf4 > 0) { state->fieldf4--; return 1; }
            break;
    }
    return 0;
}

/* Increments the state counter selected by the measured class identifier. */
void LVL_11_JOBA_FUN_00441978(u8 *object, s32 class_id) {
    JobaClassCounterView *state = *(JobaClassCounterView **)(object + 0x68);
    switch (class_id) {
        case 0xd98: state->fielde0++; break;
        case 0xdcf: state->fielde4++; break;
        case 0xdf4: state->fielde8++; break;
        case 0xdf7: state->fieldec++; break;
        case 0xdf9: state->fieldf0++; break;
        case 0xd37: state->fieldf4++; break;
    }
}

/* Dispatches an indexed polygon query through the already integrated helper. */
typedef struct { s32 field00; u8 remainder[12]; } JobaPolygonHeaderView;
extern JobaPolygonHeaderView *LVL_11_JOBA_D_001FF580[];
extern s32 LVL_11_JOBA_FUN_0032E110(f32 *point, f32 *vertices, s32 count);

s32 LVL_11_JOBA_FUN_00441808(u8 *object, f32 *point) {
    u8 *state = *(u8 **)(object + 0x68);
    s32 index = *(s32 *)(state + 0x164);
    JobaPolygonHeaderView *header;
    s32 result;
    if (index == -1) {
        result = 0;
    } else {
        header = LVL_11_JOBA_D_001FF580[index];
        result = LVL_11_JOBA_FUN_0032E110(point, (f32 *)(header + 1), header->field00);
    }
    return result;
}

typedef struct {
    u32 field00,field04;
    s32 field08;
    u8 gap0c[4];
} OrbitalPairTableView16;
extern u8 LVL_11_JOBA_D_00232E40[];
s32 LVL_11_JOBA_FUN_00392650(u32 key,u32 owner) {
    OrbitalPairTableView16 *entry=(OrbitalPairTableView16 *)LVL_11_JOBA_D_00232E40;
    s32 checked=0;
    do {
        ++checked;
        if(entry->field04==key && entry->field00==owner) return entry->field08;
        ++entry;
    } while(checked<32);
    return -1;
}

/* Apply nonzero signed halfword overrides to indexed rows of selected objects. */
typedef struct {
    unsigned int first;
    unsigned char reserved04[0x1c];
    unsigned int second;
    unsigned char reserved24[0x0f];
    unsigned char key;
    unsigned char reserved34[0x1c];
} ListOverrideRow;
typedef struct {
    unsigned char reserved00[0x0f];
    unsigned char count;
    unsigned char reserved10[0x0c];
    ListOverrideRow *rows;
} ListOverrideObject;
typedef struct { short first; short second; } ListOverridePair;
extern int LVL_11_JOBA_D_0022F700[];
extern ListOverrideObject *LVL_11_JOBA_D_00228200[];
extern ListOverridePair LVL_11_JOBA_D_0022F100[];
void LVL_11_JOBA_FUN_0037F518(void)
{
    int *selected = LVL_11_JOBA_D_0022F700;
    while (*selected >= 0) {
        ListOverrideObject *object = LVL_11_JOBA_D_00228200[*selected];
        int i = 0;
        ListOverrideRow *row = object->rows;
        while (i < object->count) {
            ListOverridePair *pair = &LVL_11_JOBA_D_0022F100[row->key];
            if (pair->first != 0)
                row->first = (row->first & 0xffffc000u) | pair->first;
            if (pair->second != 0)
                row->second = (row->second & 0xffffc000u) | pair->second;
            i++;
            row++;
        }
        selected++;
    }
}
