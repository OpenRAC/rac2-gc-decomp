typedef float f32;
typedef int s32;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct {
    char pad0[0x8];
    int unk08;
    int *unk0C;
    char pad10[0x38];
    short unk48;
    short unk4A;
    char pad4C[0x4];
    int unk50;
    int unk54;
    int w;
    int h;
    int flags;
    char pad64[0x8];
    int unk6C;
    unsigned char cnt[4];
    int unk74;
    int unk78;
    int unk7C;
    void *unk80;
} HudElem;

typedef struct { int pos; int used; int size; } Ring;

typedef struct {
    char pad0[0x20];
    unsigned char state;
} Level16VendorMoby;

extern void *D_00133E74;
extern void *D_0013A308;
extern unsigned char D_00137E00;
extern int D_00134688;

struct IndirectWord
{
    unsigned char reserved[64];
    unsigned int *value;
};

void *FUN_00115200(void)
{
    return D_00133E74;
}

void **FUN_00115210(void)
{
    return &D_0013A308;
}

void FUN_0011B0A0(void)
{
    D_00134688 = 0;
}

void FUN_00120BC8(void)
{
}

void *FUN_00125960(void)
{
    return &D_00137E00;
}

unsigned int FUN_0012F9A8(struct IndirectWord *resource)
{
    return *resource->value;
}

unsigned int FUN_0026F710(void)
{
    return 0;
}

void FUN_0026F718(void)
{
}

void FUN_001163A0(unsigned int value)
{
    *(unsigned int *)((unsigned char *)D_00133E74 + 0x58) = value;
}

int FUN_0028B740(HudElem *rec, int *x, int *y) {
    int w = rec->w;
    int h = rec->h;
    int flags = rec->flags;

    if ((flags ^ 1) & 1) {
        if (!(flags & 2)) {
            *y -= h >> 1;
        }
    }
    if (!(rec->flags & 4)) {
        if (rec->flags & 8) {
            *x -= w;
        } else {
            *x -= w >> 1;
        }
    }
    return 0;
}

float FUN_002A7AA8(float a, float b, float c, float d, float t) {
    float p = (d - c) - (a - b);
    float q = (a - b) - p;
    float t2 = t * t;
    float t3 = t2 * t;
    return p * t3 + q * t2 + (c - a) * t + b;
}

int FUN_002A8860(int *p, int b) {
    int w = *p;
    int v = (w >> 24) - b;
    if (v < 0) v = 0;
    *p = (w & 0xFFFFFF) | (v << 24);
    return v == 0;
}

int FUN_002A8AF0(float *p, float *v, int n) {
    int r = 0;
    int i = 0;
    int k;
    for (k = 0; k < n; k++) {
        float y1 = v[k * 4 + 1];
        i++;
        if (i == n) i = 0;
        if ((y1 < p[1] && p[1] <= v[i * 4 + 1]) || (v[i * 4 + 1] < p[1] && p[1] <= y1)) {
            if (v[k * 4] + (p[1] - v[k * 4 + 1]) / (v[i * 4 + 1] - v[k * 4 + 1]) * (v[i * 4] - v[k * 4]) < p[0]) {
                r = !r;
            }
        }
    }
    return r;
}

f32 FUN_002AA140(f32 a, f32 b, f32 t) { return a + (b - a) * t; }

void FUN_002AAF40(int *a, int *b, int *c, int mask) {
    int x, y;
    if (mask & 1) { x = *b; y = *a; *a = x; *b = y; }
    if (mask & 2) { x = *c; y = *b; *b = x; *c = y; }
    if (mask & 4) { x = *a; y = *c; *c = x; *a = y; }
}

s32 FUN_002CC6A0(u8 *p) { *(s32 *)(p + 0x44) = -1; return 0; }

int FUN_00312B58(Level16VendorMoby *moby) {
    return moby->state == 6;
}

int FUN_00312E10(unsigned char *p) { return p[0x20] == 1; }

void FUN_003505E0(char *base, int n) {
    Ring *r = (Ring *)(base + 0x50000);
    int avail = r->size - r->used;
    if (n < avail) avail = n;
    r->pos = (r->pos + avail) % r->size;
    r->used += avail;
}

/* Third lot: nine further bodies shared with RAC1 and measured identical in
   RAC2's boot. Each was located by exact byte search, compiled with the
   qualified RAC2 profile, and compared byte for byte before entering the
   catalogue. The bodies carry no data or call reference; all operands are
   parameters, so nothing needed re-anchoring. */

s32 FUN_0011C880(s32 *a0, s32 *a1) {
    s32 *dst;
    s32 index;
    s32 value;

    index = *(s32 *)((u8 *)a0 + 0x10);
    dst = *(s32 **)((u8 *)a1 + 0x1C);
    value = *(s32 *)((u8 *)a0 + 0x14);
    dst[index] = value;
}

s32 FUN_0011C8A0(s32 *a0, s32 *a1) {
    s32 value;

    value = *(s32 *)((u8 *)a0 + 0x10);
    *(s32 *)((u8 *)a1 + 0x8) = value;
    return value;
}

typedef struct {
    char pad0[0x8];
    int unk008;
    char pad1[0xAC - 0xC];
    int unk0AC;
    char pad2[0x118 - 0xB0];
    int unk118;
    char pad3[0x820 - 0x11C];
    int unk820;
} Obj40;

s32 FUN_0012DA98(void *arg0) {
    Obj40 *s = (Obj40 *)arg0;
    s32 r = 1;

    if (s->unk008 != 2) {
        s32 v = s->unk118;
        s->unk008 = 2;
        s->unk0AC = v;
    }
    s->unk820 = r;
    return r;
}

/* The 16-byte store is written through a volatile quad pointer. Without the
   qualifier the compiler moves the quad store into the return's delay slot,
   which the retail build does not do; the qualifier is what keeps the store
   ahead of the return. The alternative idiom used by the RAC1 corpus is inline
   assembly, which the candidate gate refuses. */
/* The copy itself goes through a 128-bit integer type, not the aggregate:
   the reconstructed 2.9-ee compiler reaches lq/sq only through TImode (the
   aggregate path builds ld/sd pairs or a memcpy call). Same bytes as the
   retail (`lq $v0,0($a3); sq $v0,0($a0)`). */

typedef struct { int a, b, c, d; } __attribute__((aligned(16))) Quad16;
typedef int TI __attribute__((mode(TI)));

void FUN_002A8C00(char *a, int b, int c, float d, Quad16 *q) {
    *(int *)(a + 0x10) = b;
    *(int *)(a + 0x14) = c;
    *(float *)(a + 0x1C) = d;
    *(int *)(a + 0x20) = 1;
    *(volatile TI *)a = *(TI *)q;
}

void FUN_002B6770(int arg0, long arg1) {
    int *p = (int *)(int)arg1;

    if (p != 0) {
        *p = arg0;
    }
}

void FUN_002B7DF8(int arg0, long arg1) {
    short *p = (short *)(int)arg1;

    if (p != 0 && arg0 != 0 && p[5] == 2) {
        p[5] = 3;
    }
}

void FUN_002B7FB0(int arg0, long arg1) {
    short *p = (short *)(int)arg1;

    if (p != 0) {
        *(int *)p = arg0;
        if (arg0 != 0) {
            if (p[5] == 1) {
                p[5] = 8;
            }
        } else {
            p[5] = 0;
        }
    }
}

void FUN_002E60A0(int arg0, long arg1) {
    unsigned char *p = (unsigned char *)(int)arg1;

    if (p != 0) {
        *(int *)p = arg0;
        if (arg0 != 0) {
            if (p[4] == 1) {
                p[4] = 2;
            }
        } else {
            *(int *)(p + 0x18) = 0;
            *(int *)(p + 0x1C) = 0;
            p[4] = 0;
        }
    }
}

void FUN_002E60E8(int arg0, long arg1) {
    int *p = (int *)(int)arg1;

    if (p != 0) {
        *p = arg0;
        if (arg0 == 0) {
            *(int *)((char *)p + 0x18) = 0;
            *(int *)((char *)p + 0x1C) = 0;
            *(unsigned char *)((char *)p + 4) = 0;
        }
    }
}

typedef struct { unsigned long long lo; unsigned long long hi; } Quad;

/* 8-BYTE LOT -- bare getters and setters, identical across all 27 levels. */
/* Measured target: byte-identical across all 27 levels. */
f32 FUN_00282C48(f32 a0) { return __builtin_fabsf(a0); }

/* Measured target: byte-identical across all 27 levels. */
f32 FUN_002A7790(f32 a0) { return a0 * a0; }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00335E10(u8 *a0) { return *(s32 *)(a0 + 0x0); }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00335E18(u8 *a0) { return *(s32 *)(a0 + 0x4); }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00335E20(u8 *a0) { return *(s32 *)(a0 + 0xc); }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00336318(u8 *a0) { return *(s32 *)(a0 + 0x38); }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_003367B8(u8 *a0) { return *(s32 *)(a0 + 0x34); }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00336CC0(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x38) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00339790(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x2f8) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0033A9C8(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x204) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0033A9F8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x20c) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0033AE18(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x290) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0033AE20(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x294) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0033B0A8(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x274) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00341550(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x220) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00341CD8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x8) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_003423A8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x170) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00343038(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x4) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00343058(u8 *a0) { *(s32 *)(a0 + 0x90) = 0; }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00343060(u8 *a0) { return *(s32 *)(a0 + 0x10); }

/* Measured target: byte-identical across all 27 levels. */
void FUN_003435A0(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x330) = a1; }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_003435A8(u8 *a0) { return *(s32 *)(a0 + 0x330); }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00348118(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x50) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00348120(u8 *a0, s32 a1) { *(s32 *)(a0 + 0xbc) = a1; }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00348128(u8 *a0) { return *(s32 *)(a0 + 0x50); }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_00348130(u8 *a0) { return *(s32 *)(a0 + 0xb0); }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00348570(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x448) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00348578(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x44c) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00349490(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x144) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00349590(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x2c) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_003495C0(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x80) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00349678(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x24) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00349B18(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x18) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0034A4A8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x0) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_0034CE98(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x2bc) = a1; }

/* Measured target: byte-identical across all 27 levels. */
void FUN_00351888(u8 *a0) { *(s32 *)(a0 + 0xa8) = 0; }

/* Measured target: byte-identical across all 27 levels. */
s32 FUN_003518D8(u8 *a0) { return *(s32 *)(a0 + 0xa8); }

/* SECOND LOT -- 16 to 32 bytes. */
/* integer-to-float conversion; byte-identical across all 27 levels. */
f32 FUN_00283CE0(s32 a0) { return (f32)a0; }

/* reads a float through double indirection and returns its integer value; byte-identical across all 27 levels. */
s32 FUN_00336950(u8 *a0) { return (s32)*(f32 *)(*(u8 **)(a0 + 0x34)); }

/* writes the value if it is below the maximum; byte-identical across all 27 levels. */
void FUN_0033A9E0(u8 *a0, s32 a1) { if (a1 < *(s32 *)(a0 + 0x1F8)) *(s32 *)(a0 + 0x208) = a1; }

/* multiplies the index by 20 and adds it to a base; byte-identical across all 27 levels. */
s32 FUN_003423B0(u8 *a0) { return *(s32 *)(a0 + 0x250) + *(s32 *)(a0 + 0x16C) * 20; }

/* true when the counter has reached 0x1000; byte-identical across all 27 levels. */
s32 FUN_0034FAE8(u8 *a0) { return *(s32 *)(a0 + 0x50) >= 0x1000; }

/* stores two words and returns 1; byte-identical across all 27 levels. */
s32 FUN_00350698(u8 *a0, s32 a1, s32 a2) { *(s32 *)(a0 + 0x4) = a1; *(s32 *)a0 = a2; return 1; }

/* stores three consecutive words; byte-identical across all 27 levels. */
void FUN_00341540(u8 *a0, s32 a1, s32 a2, s32 a3) { *(s32 *)(a0 + 0x2F0) = a1; *(s32 *)(a0 + 0x2F4) = a2; *(s32 *)(a0 + 0x2F8) = a3; }

/* lot6 -- measured bodies with sizes recorded in the catalogue. */
void FUN_0033A9D0(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x1B8) = a2; }

void FUN_00348630(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x418) = a2; }

void FUN_00349A60(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x20) = a2; }

void FUN_00349AA8(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x8C) = a2; }


/* lot7 -- measured bodies with sizes recorded in the catalogue. */
extern Quad16 D_00189EA0;
void FUN_00351E48(u8 *a0) {
    volatile s32 *p = (volatile s32 *)a0;
    p[3] = 0;
    p[2] = 0;
}


/* lot11 -- measured bodies with sizes recorded in the catalogue. */


void FUN_00343028(u8 *a0, f32 a1, f32 a2) { *(f32 *)(a0 + 0x8) = a1; *(f32 *)(a0 + 0xc) = a2; }

void FUN_003480D8(u8 *a0, s32 a1, s32 a2) { *(s32 *)(a0 + 0xa8) = a1; *(s32 *)(a0 + 0xac) = a2; }

s32 FUN_003495C8(u8 *a0, s32 a1) { s32 vieux = *(s32 *)(a0 + 0x28); *(s32 *)(a0 + 0x28) = a1; return vieux; }

s32 FUN_003518E0(u8 *a0, s32 a1) { s32 vieux = *(s32 *)(a0 + 0xa8); *(s32 *)(a0 + 0xa8) = a1; return vieux; }

s32 FUN_00351F28(u8 *a0) { return *(u32 *)(a0 + 0xc) < 1; }

void FUN_003363C0(u8 *a0, f32 a1) { f32 *p = *(f32 **)(a0 + 0x38); *p = a1; }

extern u8 D_00139648;
s32 FUN_002B0D60(void) { return D_00139648; }

/* lot12 -- measured bodies with sizes recorded in the catalogue. */
void FUN_00336498(u8 *a0, s32 a1, s32 a2) {
    *(s32 *)*(u8 **)(a0 + 0xc) = a1;
    *(s32 *)(*(u8 **)(a0 + 0xc) + 0x4) = a2;
}

void FUN_003364B0(u8 *a0, s32 a1, s32 a2) {
    *(s32 *)(*(u8 **)(a0 + 0xc) + 0x8) = a1;
    *(s32 *)(*(u8 **)(a0 + 0xc) + 0xc) = a2;
}

void FUN_0034AFE8(u8 *a0, f32 a1, f32 a2) {
    *(f32 *)*(u8 **)(a0 + 0x8) = a1;
    *(f32 *)(*(u8 **)(a0 + 0x8) + 0x4) = a2;
}

void FUN_003435B0(u8 *a0, u32 a1) {
    if (a1 < 4) *(u32 *)(a0 + 0x294) = a1;
    *(s32 *)(a0 + 0x1a0) = 0;
}





/* lot13 -- measured bodies with sizes recorded in the catalogue. */
s32 FUN_00351E58(u8 *a0) {
    return (*(u32 *)(a0 + 0xC) ^ *(u32 *)(a0 + 0x10)) < 1;
}

void FUN_00336ED0(u8 *a0, u8 *a1) {
    *(u8 **)(a1 + 0) = *(u8 **)(a0 + 0x14);
    *(u8 **)(a0 + 0x14) = a1;
    *(s32 *)(a0 + 0x10) = *(s32 *)(a0 + 0x10) - 1;
}

void FUN_0033B030(u8 *a0, f32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6) {
    *(f32 *)(a0 + 0x278) = a1;
    *(f32 *)(a0 + 0x27C) = a2;
    *(f32 *)(a0 + 0x288) = a3;
    *(f32 *)(a0 + 0x28C) = a4;
    *(f32 *)(a0 + 0x280) = a5;
    *(f32 *)(a0 + 0x284) = a6;
}

void FUN_002934B8(u8 *a0, u8 *a1) {
    *(u8 *)(a0 + 4) = *(u8 *)(a1 + 0);
    *(u8 *)(a0 + 5) = *(u8 *)(a1 + 1);
    *(u8 *)(a0 + 6) = *(u8 *)(a1 + 2);
    *(u8 *)(a0 + 7) = *(u8 *)(a1 + 3);
    *(u8 **)(a0 + 0x0) = a1 + *(s32 *)(a1 + 4);
    *(u8 **)(a0 + 0x20) = a1 + *(s32 *)(a1 + 8);
}

void FUN_003364C8(u8 *a0, s32 a1, s32 a2) {
    u8 *o = a0;
    *(u32 *)*(u8 **)(o + 0xC) = (*(u32 *)*(u8 **)(o + 0xC) & 0x00FFFFFF) | (u32)(a1 << 24);
    *((u32 *)*(u8 **)(o + 0xC) + 1) = (*((u32 *)*(u8 **)(o + 0xC) + 1) & 0x00FFFFFF) | (u32)(a2 << 24);
}

void FUN_00336508(u8 *a0, s32 a1, s32 a2) {
    u8 *o = a0;
    *((u32 *)*(u8 **)(o + 0xC) + 2) = (*((u32 *)*(u8 **)(o + 0xC) + 2) & 0x00FFFFFF) | (u32)(a1 << 24);
    *((u32 *)*(u8 **)(o + 0xC) + 3) = (*((u32 *)*(u8 **)(o + 0xC) + 3) & 0x00FFFFFF) | (u32)(a2 << 24);
}


/* lot14 -- measured bodies with sizes recorded in the catalogue. */
extern u8 D_0018C0B0[];
void FUN_002AB650(u8 *a0) {
    u8 *p = *(u8 **)D_0018C0B0;
    *(long *)(a0 + 0x38) = *(long *)(p + 0x38);
}

/* lot15 -- first CALL-BEARING body: the call family is now open. */
extern void FUN_0011AAD0(s32);
void FUN_0034F3B8(void) {
    FUN_0011AAD0(1);
}

/* lot15 -- two twins of the first call-bearing body (same form, 32 bytes).
   The matched wrappers use a `nop` delay slot and return 1; the slot alone does not establish the callee parameter list. */
extern void FUN_001338C8();
s32 FUN_0034F898(void) {
    FUN_001338C8();
    return 1;
}

extern void FUN_0012EE28();
s32 FUN_00351828(void) {
    FUN_0012EE28();
    return 1;
}

/* The campaign: call-bearing bodies reproduced by the reconstructed toolchain
   (2.9-ee + refined gas) -- sd saves in 8-byte slots,
   no sibcall, the s32 return prototype determines v0 versus v1, and || chains
   with a shared exit. Measurements and evidence: docs/COMPILER-NOTES.md. */

/* FUN_002A77E0 : random draw bounded by the argument */
extern s32 FUN_001163B0(void);
s32 FUN_002A77E0(s32 a0) {
    return (FUN_001163B0() >> 16 & 0x7fff) % a0;
}

/* FUN_002A7940 : random angle: 12 bits centered around zero, then converted to radians */
extern s32 FUN_001163B0(void);
f32 FUN_002A7940(void) {
    return (f32)((FUN_001163B0() >> 16 & 0xfff) - 0x800) * 0.0015339808f;
}

/* FUN_002A7820 : random draw within a closed interval */
extern s32 FUN_001163B0(void);
s32 FUN_002A7820(s32 a0, s32 a1) {
    return (FUN_001163B0() >> 16 & 0x7fff) % ((a1 - a0) + 1) + a0;
}

/* FUN_002889B8 : clears three fields, initializes a block, then sets two flags */
extern s32 FUN_00115484(u8 *, s32, s32);
void FUN_002889B8(u8 *a0) {
    *(s32 *)(a0 + 0x30) = 0;
    *(s32 *)(a0 + 0x34) = 0;
    *(s32 *)(a0 + 0x38) = 0;
    FUN_00115484(a0 + 8, 0xcd, 0x28);
    *(s32 *)(a0 + 0x44) = 0;
    *(s32 *)(a0 + 0x40) = 1;
}

/* FUN_003512B8 : rounds a field to a multiple of 2048 under a semaphore */
extern s32 FUN_0011AC60(s32);
extern s32 FUN_0011AC40(s32);
void FUN_003512B8(u8 *a0) {
    FUN_0011AC60(*(s32 *)(a0 + 0x40));
    *(s32 *)(a0 + 0x14) = (*(s32 *)(a0 + 0x14) + 0x7ff) / 0x800 * 0x800;
    FUN_0011AC40(*(s32 *)(a0 + 0x40));
}

/* FUN_00300540 : dispatch: weapon category selected by the identifier */
s32 FUN_00300540(s32 a0) {
    s32 v = 0;
    if (a0 == 0 || a0 == 14 || a0 == 13) v = 2;
    else if (a0 == 1 || a0 == 3 || a0 == 11 || a0 == 12) v = 1;
    return v;
}

/* FUN_00351268 : waits, then combines two fields shifted by 11 */
extern s32 FUN_0011AC60(s32 a0);
extern s32 FUN_0011AC40(s32 a0);
s32 FUN_00351268(u8 *a0) {
    s32 x;
    FUN_0011AC60(*(s32 *)(a0 + 0x40));
    x = (*(s32 *)(a0 + 0x10) << 11) + *(s32 *)(a0 + 0x14);
    FUN_0011AC40(*(s32 *)(a0 + 0x40));
    return x;
}

/* Ninth lot: privately byte-gated call-bearing bodies. */
extern s32 FUN_001163B0(void);
f32 FUN_002A7878(f32 a0, f32 a1) {
    s32 r = FUN_001163B0();
    return a0 + (f32)(r >> 16 & 0x7FFF) * (a1 - a0) * 3.0517578125e-05f;
}

extern s32 FUN_001163B0(void);
f32 FUN_002A78D8(f32 a0, f32 a1) {
    s32 r = FUN_001163B0();
    f32 result = a0 + (f32)(r >> 16 & 0xFFF) * (a1 - a0) * 0.000244140625f;
    if ((r >> 16 & 1) != 0) result = -result;
    return result;
}

extern s32 FUN_00133890(void);
void FUN_0034F918(u8 *a0) {
    FUN_00133890();
    *(volatile s32 *)(a0 + 0x5c) = 0;
    *(volatile s32 *)(a0 + 0x00) = 0;
    *(volatile s32 *)(a0 + 0x30) = 0;
    *(volatile s32 *)(a0 + 0x38) = 0;
    *(volatile s32 *)(a0 + 0x3c) = 0;
    *(volatile s32 *)(a0 + 0x44) = 0;
    *(volatile s32 *)(a0 + 0x50) = 0;
    *(volatile s32 *)(a0 + 0x58) = 0;
}

extern void FUN_001338F0(s32, s32, s32, s32, s32);
void FUN_0034F8C0(u8 *a0) {
    FUN_001338F0(*(s32 *)(a0+0x48), *(s32 *)(a0+0x4c) / 1024 * 1024,
                *(s32 *)(a0+0x5c), *(s32 *)(a0+0x14), *(s32 *)(a0+0x18));
    *(s32 *)a0 = 2;
}

extern s32 FUN_0011AC60(s32);
extern s32 FUN_0011AC40(s32);
void FUN_00350B70(u8 *a0, s32 a1) {
    FUN_0011AC60(*(s32 *)(a0+0x40));
    *(s32 *)(a0+0x14) += (s32)a1;
    *(long *)(a0+0x48) = (long)a1 + *(long *)(a0+0x48);
    FUN_0011AC40(*(s32 *)(a0+0x40));
}

extern s32 FUN_0011F5E0(void);
extern s32 FUN_0011F628(void);
void FUN_00350798(u32 a0) {
    FUN_0011F5E0();
    *(volatile u32 *)0x1000f590 = *(volatile u32 *)0x1000f520 | 0x10000;
    *(volatile u32 *)0x1000b000 = a0;
    *(volatile u32 *)0x1000f590 = *(volatile u32 *)0x1000f520 & 0xfffeffff;
    FUN_0011F628();
}

extern s32 FUN_0011F5E0(void);
extern s32 FUN_0011F628(void);
void FUN_00350808(u32 a0) {
    FUN_0011F5E0();
    *(volatile u32 *)0x1000f590 = *(volatile u32 *)0x1000f520 | 0x10000;
    *(volatile u32 *)0x1000b400 = a0;
    *(volatile u32 *)0x1000f590 = *(volatile u32 *)0x1000f520 & 0xfffeffff;
    FUN_0011F628();
}

extern s32 FUN_001253A8(s32, s32, u8 *, u8 *);
extern s32 FUN_00124B88(s32);
s32 FUN_003506B0(u8 *a0, u8 *a1, s32 a2, s32 a3) {
    u8 mode[3];
    s32 sectors = a2 >> 11;
    s32 out = 0;
    mode[0] = 100; mode[1] = 1; mode[2] = 0;
    FUN_001253A8(*(s32 *)(a0+4), sectors, a1, mode);
    if (a3 == 0) {
        *(s32 *)(a0+4) += sectors;
        FUN_00124B88(0);
        out = a2;
    }
    return out;
}

extern s32 FUN_0011F5E0(void);
extern s32 FUN_0011F628(void);
void FUN_00351E70(u8 *a0) {
    FUN_0011F5E0();
    *(s32 *)(*(u8 **)(a0+4) + *(s32 *)(a0+8) * 0x138c0) = 2;
    *(volatile s32 *)(a0+0xc) += 1;
    *(volatile s32 *)(a0+8) = (*(volatile s32 *)(a0+8) + 1) % *(s32 *)(a0+0x10);
    FUN_0011F628();
}

/* lot10: privately byte-gated bodies. */
extern s32 FUN_00126470(u8 *, s32, s32, s32, s32, s32, s32, s32);
extern s32 FUN_0011AEA0(s32);
extern s32 FUN_00126730(u8 *, u8 *);
extern s32 FUN_001244B8(s32, s32);
void FUN_002FC8D0(u8 *a0, s32 a1, s32 a2) {
    u8 image[112];
    s32 remain = (a2 + 0x3fff) & ~0x3fff;
    s32 src = 0, dst = 0;
    for (; remain > 0; remain -= 0x4000) {
        FUN_00126470(image, (s32)(short)((a1+src)>>8), 1, 1, 0, 0, 64, 64);
        src += 0x4000;
        FUN_0011AEA0(0);
        FUN_00126730(image,a0+dst);
        dst += 0x3000;
        FUN_001244B8(0,0);
    }
}

extern s32 FUN_0011AC60(s32);
extern s32 FUN_0011AC40(s32);
void FUN_00350A78(s32 *a0, s32 *a1, s32 *a2, s32 *a3, s32 *a4) {
    s32 pos, left;
    FUN_0011AC60(a0[16]);
    left = (s32)((long)a0[2] - (long)(a0[4]+2)) * 2048 - a0[5];
    pos = ((a0[3] + a0[4]) * 2048 + a0[5]) % a0[6];
    if (left <= a0[6]-pos) {
        *a1 = a0[0]+pos;
        *a2 = left;
        *a3 = 0;
        *a4 = 0;
    } else {
        *a1 = a0[0]+pos;
        *a2 = a0[6]-pos;
        *a3 = a0[0];
        *a4 = left - (a0[6]-pos);
    }
    FUN_0011AC40(a0[16]);
}

s32 FUN_00288A00(u8 *a0,s32 *a1,s32 *a2) {
 s32 *values,*flags; s32 idx;
 if(*(s32 *)(a0+0x38)==0) return 0;
 if(--*(s32 *)(a0+0x38)==0) *(s32 *)(a0+0x44)=0;
 values=(s32 *)(a0+8); flags=(s32 *)(a0+12);
 *a1=*(s32 *)((u32)values+(*(s32 *)(a0+0x34)<<3));
 *a2=*(s32 *)((u32)flags+(*(s32 *)(a0+0x34)<<3));
 values=(s32 *)((u32)values+(*(s32 *)(a0+0x34)<<3)); *values=-1;
 flags=(s32 *)((u32)flags+(*(s32 *)(a0+0x34)<<3)); *flags=-1;
 idx=*(s32 *)(a0+0x34)+1;
 if(idx==5) idx=0;
 *(s32 *)(a0+0x34)=idx;
 return 1;
}

s32 FUN_00288AA0(u8 *a0,s32 a1) {
 s32 v;
 if(*(s32 *)(a0+0x38)==5 || *(s32 *)(a0+0x44)!=0) return 0;
 *(s32 *)((u32)a0+(*(s32 *)(a0+0x30)<<3)+8)=a1;
 *(u32 *)((u32)a0+(*(s32 *)(a0+0x30)<<3)+12)=(u32)(a1-0x20U)<0x91;
 v=*(s32 *)(a0+0x30)+1;
 if(v==5) v=0;
 *(s32 *)(a0+0x38)=*(s32 *)(a0+0x38)+1;
 *(s32 *)(a0+0x30)=v;
 return 1;
}

void FUN_00295BE8(u8 *a0,u8 *a1,u8 *a2,u8 *a3) {
 s32 i=0;
 do {
  s32 next=i+1;
  u32 mask=1;
  u8 *nextp=a3+1;
  s32 j=7;
  do {
   u8 value;
   j--;
   if(*a3&mask) value=*a1; else value=*a2;
   mask<<=1;
   *a0=value;
   a1++;a2++;a0++;
  } while(j>=0);
  i=next;
  a3=nextp;
 } while(i<0x8000);
}

void FUN_0029F978(u8 *a0,u32 a1,u8 *a2) {
 u8 *p;
 if(a2[1]!=0) return;
 a2[0]=a1; a2[1]=1;
 *(f32 *)(a2+0x1c)=1.0f;
 *(f32 *)(a2+0x20)=1.0f;
 *(f32 *)(a2+0x24)=1.0f;
 *(f32 *)(a2+0x28)=1.0f;
 p=*(u8 **)(*(u8 **)(*(u8 **)(a0+0x24)+0x1c)+a2[0]*4+4);
 *(u32 *)(a2+4)=p[p[0]+4]*0x40+0x70000000;
 *(u8 **)(a2+8)=*(u8 **)(a0+0x54);
 *(u8 **)(a0+0x54)=a2;
}

void FUN_00348068(u8 *a0,f32 *a1) {
 f32 *end;
 *(f32 **)(a0+0x58)=a1;
 *(s32 *)(a0+0xb0)=0;
 end=a1+0x50;
 if(a1[0]>0.0f) {
  do {
   a1+=5;
   (*(s32 *)(a0+0xb0))++;
  } while(*a1>0.0f && (s32)a1<(s32)end);
 }
 if(*(s32 *)(a0+0x50)>=*(s32 *)(a0+0xb0)) *(s32 *)(a0+0x50)=0;
}

s32 FUN_00350750(u8 *a2, s32 a1) {
    u32 v1 = ((*(u32 *)(a2 + 8) << 4) + *(u32 *)(a2 + 4) + 0x10) & 0x0FFFFFFF;
    if (a1 == v1) return 0;
    return (u32)(a1 - *(u32 *)a2) >> 11;
}

s32 FUN_0027F128(u8 *a0,s32 a1,u8 *a2) {
 s32 out=0,i=0;
 u8 *p;
 if(a1!=0 && a0[0]!=0) {
  p=a0;
  do {
   s32 value=*(signed char *)(a2+p[0]*4+3);
   i++; p++;
   if(value!=0) out+=value;
   if(i==a1) break;
  } while(p[0]!=0);
 }
 return out;
}

void FUN_002C9A98(s32 *a0) {
 s32 *base=a0+3; s32 v;
 do {
  v=--a0[1];
  if(v<=0) v=a0[0];
  a0[1]=v;
 } while(*(s32 *)((u32)base+(v<<2))==0);
}

void FUN_002C9AD8(s32 *a0) {
 s32 limit=a0[0]; s32 *base=a0+3; s32 v;
 do {
  v=a0[1]+1;
  if(limit<v) v=0;
  a0[1]=v;
 } while(*(s32 *)((u32)base+(v<<2))==0);
}

void FUN_002DE810(unsigned short *a0,u8 *a1) {
 a0[0]=0;
 a0[1]=*(unsigned short *)(a1+0x24);
 a0[2]=0;
 a0[3]=*(unsigned short *)(a1+0x20);
 a0[4]=*(s32 *)(a1+0x20)>>1;
 a0[5]=*(s32 *)(a1+0x24)>>1;
 a0[8]=0x10;a0[9]=0;
}

s32 FUN_00336D50(u8 *a0,s32 a1) {
 s32 count=*(s32 *)(a0+0x18); s32 i,out=0;u8 *base=a0+0x1c;
 for(i=0;i<count;i++) {
  s32 *p=(s32 *)((i<<3)+(u32)base);
  if(p[0]==a1) {out=p[1];break;}
 }
 return out;
}

void FUN_00349150(s32 *a0) {
 u8 *p=(u8 *)a0+0x50; s32 value=-1; s32 i=15;
 a0[0]=0;a0[0x52]=0;
 do {
  *(s32 *)(p-12)=value;*(s32 *)(p-8)=value;*(s32 *)(p-4)=value;*(s32 *)p=value;
  i--;p+=16;
 }while(i>=0);
 a0[0x54]=0;a0[0x55]=0;
}

void FUN_00349630(u8 *a0,s32 a1) {
 *(s32 *)(a0+0x1c)=a1;
 if(*(s32 *)(a0+0x28)==0) {
  if(a1==1) *(f32 *)(a0+0x18)=0.0f;
  else *(f32 *)(a0+0x18)=1.0f;
  *(s32 *)(a0+0x28)=1;
 }
}

u8 *FUN_00349918(u8 *a0) {
 s32 *p=(s32 *)(a0+0x2c); s32 i=1;
 do {
  s32 *end=p+12;
  s32 next=i-1;
  s32 j=2;
  do {
   p[0]=0;p[1]=0;p[2]=0;p[3]=0;
   j--;p+=4;
  } while(j!=-1);
  i=next;p=end;
 } while(i!=-1);
 return a0;
}

extern u8 D_001A63A8[];
extern s32 FUN_00133688(void);
extern s32 FUN_0011AEA0(s32);
void FUN_002B7D88(s32 a0) {
    if (a0 == 1) {
        if (FUN_00133688() != 0) { *(short *)(D_001A63A8+4) = 2; } else {
            void (*callback)(s32,s32);
            s32 data, flag;
            ((void (*)(s32))FUN_0011AEA0)(0);
            flag = D_001A63A8[6] == 0;
            callback = *(void (**)(s32,s32))(D_001A63A8+0x18);
            *(short *)(D_001A63A8+4) = 0;
            D_001A63A8[6] = 0;
            if (callback) {
                data = *(s32 *)(D_001A63A8+0x1c);
                *(void (**)(s32,s32))(D_001A63A8+0x18) = 0;
                *(s32 *)(D_001A63A8+0x1c) = 0;
                callback(data,flag);
            }
        }
    }
}

extern s32 D_001A72E0[] __attribute__((sda));
extern s32 D_001A7340[] __attribute__((sda));
extern s32 FUN_00126470(u8 *, s32, s32, s32, s32, s32, s32, s32);
extern s32 FUN_0011AEA0(s32);
extern s32 FUN_00126730(u8 *, u8 *);
void FUN_00285708(s32 a0, s32 a1, s32 a2, s32 a3, u8 *a4) {
    u8 image[112];
    FUN_00126470(image, (D_001A72E0[0]<<8)>>16, (D_001A7340[0]<<10)>>16,
                0x30,(short)a0,(short)a1,(short)a2,(short)a3);
    FUN_0011AEA0(0);
    FUN_00126730(image,a4);
}

typedef struct __attribute__((packed)) { u8 mode[4]; } CdMode;
extern CdMode D_001A63E8;
extern u8 D_001A7900[] __attribute__((sda));
extern s32 D_001A7430[] __attribute__((sda));
extern s32 D_001A7434 __attribute__((sda));
extern s32 FUN_001334B8(s32,s32,s32,CdMode *);
extern s32 FUN_00133230(void);
extern s32 FUN_00132028(void);
s32 FUN_002B7C10(s32 a0, s32 a1, s32 a2) {
    CdMode mode = D_001A63E8;
    mode.mode[1] = D_001A7900[0];
    D_001A7430[0] = 0; D_001A7434 = 0;
    FUN_001334B8(a1,a2,a0,&mode);
    FUN_00133230(); FUN_00132028();
    return 1;
}

extern s32 FUN_0011AEA0(s32);
extern s32 FUN_0011AFE0(s32 *, s32);
extern s32 FUN_0011AFC0(s32);
extern s32 FUN_00133930(s32, s32);
void FUN_0034FB20(u8 *a0, s32 a1, s32 a2, s32 a3) {
    s32 dma[4], id;
    ((void (*)(s32))FUN_0011AEA0)(0);
    dma[0] = a1; dma[1] = *(s32 *)(a0+0x48); dma[2] = a2; dma[3] = 0;
    do { id = FUN_0011AFE0(dma,1); } while (id == 0);
    while (FUN_0011AFC0(id) >= 0) {}
    FUN_00133930(a2,a3);
}

s32 FUN_0029AE78(s32 *a0) {
 s32 n=8;
 while(a0[0]!=0) {
  n+=8; n+=a0[1]; n=(n+3)&-4;
  a0+=4;
 }
 return n+8;
}

s32 FUN_002AB1F0(u8 *a0) {
 if(a0==0) return 0;
 if((*(unsigned short *)(a0+0x34)&0x20)!=0) return (*(s32 **)(a0+0x68))[0];
 return 0;
}

s32 FUN_002AB220(u8 *a0) {
 if(a0==0) return 0;
 if((*(unsigned short *)(a0+0x34)&0x20)!=0) return (*(s32 **)(a0+0x68))[4];
 return 0;
}

s32 FUN_002AD0B0(u8 *a0) {
 if(a0==0) return 0;
 if((*(unsigned short *)(a0+0x34)&0x20)==0) return 0;
 return (*(s32 **)(a0+0x68))[2];
}

u8 *FUN_00349498(u8 *a0) {
 s32 *p=(s32 *)(a0+0x30);s32 i=3;
 do {
  p[0]=0;p[1]=0;p[2]=0;p[3]=0;
  i--;p+=4;
 }while(i!=-1);
 return a0;
}

void FUN_0026FEB8(const char *format,...) {
}

void FUN_002802E0(unsigned short *a0,s32 a1,s32 a2,s32 a3,s32 a4,s32 a5,s32 a6,s32 a7,s32 a8) {
 a0[0]=a1;a0[1]=a2;a0[2]=a3;a0[3]=a4;
 a0[4]=a5;a0[5]=a6;a0[8]=a7;a0[9]=a8;
 a0[6]=0;a0[7]=0;a0[10]=0;a0[11]=0;
}

void FUN_0034F960(s32 *a0,s32 *a1,s32 *a2,s32 *a3,s32 *a4) {
 s32 offset,n;
 if(a0[0]==0) {
  if(a0[1]!=4) {
   *a1=(s32)((u8 *)a0+(a0[12]+8));
   *a2=0x28-a0[12]; *a3=a0[13]; *a4=a0[16];
   return;
  }
  *a1=a0[13]; *a2=a0[16];
  clear: *a3=0; *a4=0; return;
 }
 n=a0[16]-a0[15];offset=a0[14];
 if(a0[16]-offset>=n) {
  *a1=a0[13]+offset; *a2=n; goto clear;
 }
 *a1=a0[13]+offset;
 *a2=a0[16]-a0[14]; *a3=a0[13];
 *a4=n-(a0[16]-a0[14]);
}

s32 FUN_00350628(u8 *a0,s32 *a1) {
    u8 *p=a0+0x50000;
    s32 used=*(s32 *)(p+4);
    if(used!=0) {
        s32 size=*(s32 *)(p+8);
        *a1=(s32)a0+((*(s32 *)p-used)+size)%size;
    }
    return *(s32 *)(p+4);
}

extern u8 D_00188660[];
s32 FUN_002E59B0(s32 a0,s32 a1) {
    if(a1>=0) {
        u8 *p=D_00188660+a1*0x70;
        if(*(s32 *)(p+0x88)==a0 && (u32)p[0x74]-1<2) return 1;
    }
    return 0;
}

extern u8 D_00188660[];
void FUN_002E59F8(s32 a0) {
    if(a0>=0) {
        u8 *p=D_00188660+a0*0x70;
        s32 state=p[0x74];
        if(state==7) {
            *(s32 *)(p+0x88)=0;
            *(s32 *)(p+0x8c)=0;
            p[0x74]=0;
            return;
        }
        if(state!=0 && state!=6) p[0x74]=4;
    }
}

s32 FUN_002B3868(u8 *a0) {
 u8 flags=a0[0xbe];
 if(((flags^1)&1)!=0) {a0[0xbe]=flags|1;return 1;}
 return 0;
}

void FUN_00335E68(u8 *a0, s32 a1) {
 f32 *p=*(f32 **)(a0+0x10);
 *p=(a1!=0)?1.0f:0.0f;
}

s32 FUN_00335E88(u8 *a0) {
 return 0.0f<**(f32 **)(a0+0x10);
}

s32 FUN_003505B0(u8 *a0,u8 **a1) {
 s32 *p=(s32 *)(a0+0x50000);
 s32 n=p[2]-p[1];
 if(n!=0) *a1=a0+p[0];
 return n;
}

void FUN_00351FA0(u8 *a0) {
 if(*(volatile s32 *)(a0+0xc)>0) *(volatile s32 *)(a0+0xc)=*(volatile s32 *)(a0+0xc)-1;
}

void FUN_0034FA30(s32 *a0,s32 a1) {
    if(a0[0]==0) {
        if(a0[1]!=4) {
            s32 space=40-a0[12];
            s32 take=a1;
            if(space<a1) take=space;
            a0[12]+=take;
            if(a0[12]>39) a0[0]=1;
            a1-=take;
        } else a0[0]=1;
    }
    a0[16]=a0[16]/1024*1024;
    a0[14]=(a0[14]+a1)%a0[16];
    a0[15]+=a1;
    a0[17]+=a1;
}

typedef struct { f32 x,y,z,w; } MmiPoint;
s32 FUN_002A8A50(const f32 *a0,const MmiPoint *a1,s32 a2) {
    s32 i;
    for(i=0;i<a2;i++) {
        f32 x=a1[i].x,y=a1[i].y;
        if((a1[(i+1)%a2].x-x)*(a0[1]-y)-(a1[(i+1)%a2].y-y)*(a0[0]-x)>0.0f) return i+1;
    }
    return 0;
}

void FUN_0028B950(u8 *a0) {
    s32 *value=*(s32 **)(a0+0xc);
    s32 digits;
    s32 n;
    if (value!=0 && ((u32)value&3)==0) {
        s32 initial=*value;
        s32 maximum=*(s32 *)(a0+8);
        *(volatile s32 *)(a0+0x78)=initial;
        if (maximum<initial) *(volatile s32 *)(a0+0x78)=maximum;
        *(s32 *)(a0+0x74)=*(volatile s32 *)(a0+0x78);
    } else {
        *(volatile s32 *)(a0+0x74)=99999;
        *(volatile s32 *)(a0+0x78)=99999;
    }
    digits=0;
    for (n=*(volatile s32 *)(a0+8);n>9;n/=10) digits++;
    if ((*(u32 *)(a0+0x60)&3)==0 && (*(u32 *)(a0+0x60)&12)!=0) {
        *(s32 *)(a0+0x5c)+=(digits+1)*12;
        if (*(volatile s32 *)(a0+0x58)<14) { *(s32 *)(a0+0x58)=14; return; }
    } else {
        if (*(s32 *)(a0+0x5c)<12) *(s32 *)(a0+0x5c)=12;
        *(s32 *)(a0+0x58)=*(volatile s32 *)(a0+0x58)+(digits+1)*14;
    }
}

s32 FUN_002CB4E8(s32 a0) {
 s32 result=0;
 if(a0<0x15 || a0==0x18) result=1;
 return result;
}

s32 FUN_002ED688(u8 *a0) {
 if((*(unsigned short *)(a0+0x34)&0x20)==0) return 0;
 return (*(s32 **)(a0+0x68))[5];
}

void FUN_00335E38(u8 *a0,f32 f0,f32 f1,f32 f2,f32 f3) {
 (*(f32 **)a0)[0]=f0;(*(f32 **)a0)[1]=f1;
 (*(f32 **)a0)[2]=f2;(*(f32 **)a0)[3]=f3;
}

void FUN_00335F88(u8 *a0,f32 f0,f32 f1,f32 f2,f32 f3) {
 (*(f32 **)(a0+4))[0]=f0;(*(f32 **)(a0+4))[1]=f1;
 (*(f32 **)(a0+4))[2]=f2;(*(f32 **)(a0+4))[3]=f3;
}

s32 FUN_00342BC0(u8 *a0) {
 if(*(s32 *)(a0+0x10)!=0) return *(s32 *)((u32)a0+(*(s32 *)(a0+0x14)<<2)+0x28);
 return 0;
}

void FUN_003480F0(u8 *a0,f32 f0,f32 f1) {
 (*(f32 **)(a0+0x4c))[0]=f0;
 (*(f32 **)(a0+0x4c))[1]=f1;
 (*(s32 **)(a0+0x4c))[2]=0;
 (*(s32 **)(a0+0x4c))[3]=0;
}

void FUN_00350878(unsigned long *a0,unsigned long a1,unsigned long a2,unsigned long a3) {
 *a0=(a1<<32)|((a2<<32)>>4)|((a3<<32)>>32);
}

f32 FUN_002A7798(f32 a0) {
 return 1.0f-(1.0f-a0)*(1.0f-a0);
}

void FUN_00339760(u8 *a0) {s32 v=1;*(s32 *)(a0+0x300)=v;}

void FUN_00350590(u8 *a0) {
 a0+=0x50000;
 *(s32 *)(a0+8)=0x50000;
 *(s32 *)a0=0;
 *(s32 *)(a0+4)=0;
}

void FUN_003518C8(u8 *a0) {s32 v=1;*(s32 *)(a0+0xa8)=v;}

void FUN_00349AB8(u8 *a0,s32 a1) {
 if(a1!=0) *(f32 *)(a0+0x10)=1.0f;
 else *(f32 *)(a0+0x10)=0.0f;
 *(s32 *)(a0+0x14)=1;
 *(s32 *)(a0+0x1c)=1;
}

void FUN_00349AE0(u8 *a0,s32 a1) {
 if(a1!=0) *(f32 *)(a0+0x10)=0.0f;
 else *(f32 *)(a0+0x10)=1.0f;
 *(s32 *)(a0+0x14)=-1;
 *(s32 *)(a0+0x1c)=1;
}

s32 FUN_00350670(u8 *a0,s32 a1) {
 s32 original,value;
 a0+=0x50000;
 value=*(s32 *)(a0+4);original=value;
 if(a1<value) value=a1;
 *(s32 *)(a0+4)=original-value;
 return value;
}

void FUN_00336920(u8 *a0,s32 a1,s32 a2) {
 **(f32 **)(a0+0x34)=(f32)a1;
 (*(f32 **)(a0+0x34))[1]=(f32)a2;
}

void FUN_00336CC8(u8 *a0,s32 a1) {
 (*(f32 **)(a0+4))[1]=(f32)a1;
}

void FUN_00297550(u8 *out,s32 index,u8 *base,short *offsets) {
    s32 pending=0;
    u8 *start;
    short *table;
    s32 records,i;
    if (index!=0) start=base+(offsets+index)[-1];
    else start=base+0x200;
    table=(short *)(index*2+(s32)offsets);
    records=((base+*table-start)*2)/3;
    for(i=0;i<records;i++) {
        s32 position=i*2+i;
        u8 *p=start+(position>>1);
        u8 color=p[0],runByte=p[1],fill;
        s32 run;
        if ((position&1)!=0) color>>=4;
        else {
            runByte=(u8)((runByte<<4)|(color>>4));
            color&=15;
        }
        run=256;
        if(runByte!=0) run=runByte;
        fill=(u8)(color|(color<<4));
        if(pending) {
            pending=0;run--;
            *out |= color<<4;
            out++;
        }
        if(run!=0) {
            do {*out++=fill;run-=2;} while(run>0);
            if(run!=0) {out--;pending=1;*out=color;}
        }
    }
}

typedef struct { u8 before[0x68]; s32 active; u8 gap[6]; short state; } CallState;
extern u8 D_001A63A8[];
extern void FUN_00133400(s32);
s32 FUN_002B6D28(void) {
    if (((CallState *)D_001A63A8)->active==0) return 0;
    if (((CallState *)D_001A63A8)->state!=3) return 0;
    FUN_00133400(((CallState *)D_001A63A8)->active); ((CallState *)D_001A63A8)->state=4;
    return 1;
}

typedef struct { u8 before[0x158]; short a,b,c; u8 gap[10]; short d,e; } DisplayState;
extern DisplayState D_001A6480;
extern u8 *D_001A742C;
extern void FUN_00125A20(u8 *,short,short,short,short,short);
void FUN_00284E40(void) {
    FUN_00125A20(*(u8 **)0x001a742c,D_001A6480.c,D_001A6480.a,D_001A6480.b,D_001A6480.d,D_001A6480.e);
}

extern s32 D_001A7B90;
extern s32 D_001A7BB8 __attribute__((sda));
extern s32 FUN_00124418(void);
extern s32 FUN_001257D0(s32,s32,s32,s32);
void FUN_00284000(void) {
    FUN_00124418();
    if (D_001A7B90 != 0) D_001A7BB8 = 0;
    if (*(s32 *)0x001a7bb8 != 0) FUN_001257D0(0,0,0x50,1);
    else FUN_001257D0(0,1,D_001A7B90 != 0 ? 3 : 2,0);
}

void FUN_002A7750(u8 *a0,u8 a1,s32 a2) {
 u8 old=a0[0x20];a0[0x20]=a1;a0[0x94]=old;*(short *)(a0+0x96)=0;
 a0[0xbe]&=~1;
 if(a2!=-1){a0[0x95]=(u8)a2;a0[0xbe]&=~2;}
}

void FUN_002B8888(u8 *a0) {
 s32 *p=(s32 *)(a0+0x140);s32 count=15;
 *(s32 *)(a0+0x1b0)=0;*(volatile s32 *)(a0+0x1d4)=1;
 *(volatile s32 *)(a0+0x1a0)=0;*(volatile s32 *)(a0+0x1a4)=0;*(volatile s32 *)(a0+0x1a8)=0;
 *(volatile s32 *)(a0+0x1d0)=1;*(volatile s32 *)(a0+0x1b4)=0;*(volatile s32 *)(a0+0x1b8)=0;
 *(volatile s32 *)(a0+0x1c0)=0;*(volatile s32 *)(a0+0x1c4)=0;*(volatile s32 *)(a0+0x1c8)=0;*(volatile s32 *)(a0+0x1d8)=0;
 do { p[-16]=0;count--;p[0]=0;p++; }while(count>=0);
}

s32 FUN_00300280(s32 *a0,s32 a1) {
 s32 out=-1,i=0,flag=1; s32 *p=a0;
 for(;i<64;i++,p++) {
  if(*p==0) {
   *p=a1;
   a0[0x89]=flag;
   a0[0x88]++;
   out=i;
   p[0x40]=0;
   break;
  }
 }
 return out;
}

extern s32 FUN_00133230(void);
extern s32 FUN_00132028(void);
extern s32 FUN_00132AC8(void);
extern u8 D_00188660[];
void FUN_002E5FE0(void) {
    typedef s32 quad __attribute__((mode(__TI__)));
    s32 count;
    u8 *p;
    FUN_00133230(); FUN_00132028(); FUN_00132AC8();
    while (FUN_00132028() != 0) {}
    p=D_00188660; count=3;
    do { *(volatile quad *)p = 0; count--; p += 16; } while (count >= 0);


    p=D_00188660;
    *(s32 *)(p+0x40)=0;
    for(count=0;count<52;count++) {
        *(s32 *)(p+count*0x70+0x70)=0;
        *(u8 *)(p+count*0x70+0x74)=0;
    }
}

typedef struct { u8 before[0x1730]; s32 count; u8 *table; } CallbackGlobals;
extern u8 D_00188660[];
void FUN_002E5F60(void) {
    s32 i=0;
    for (i=0;i<((CallbackGlobals *)D_00188660)->count;i++) {
            u8 *entry=(u8 *)(i*0x90+(s32)((CallbackGlobals *)D_00188660)->table);
            void (*callback)(u8 *)=*(void (**)(u8 *))(entry+4);
            if (callback) callback(entry);
    }
}

typedef struct {
 u8 before[0x2c]; short clear2c; u8 gap1[14]; short mode,next; u8 gap2[4];
 u32 busy44; short value48; u8 gap3[2]; short step4c,step4e; u8 gap4[24];
 u32 busy68; u8 gap5[4]; short step70,step72; u8 gap6[24];
 u32 busy8c; u8 gap7[4]; short step94,step96;
} WaitState;
extern u8 D_001A63A8[];
extern s32 FUN_00133230(void);
extern s32 FUN_00132028(void);
extern s32 FUN_00133310(void);
extern s32 FUN_00133490(s32);
void FUN_002B7170(void) {
    ((void (*)(void))FUN_00133230)();
    while (((WaitState *)D_001A63A8)->busy44 == 0xffffffffU) FUN_00132028();
    while (((WaitState *)D_001A63A8)->busy8c == 0xffffffffU) FUN_00132028();
    while (((WaitState *)D_001A63A8)->busy68 == 0xffffffffU) FUN_00132028();
    FUN_00133310();
    while (FUN_00132028() != 0) {}
    FUN_00133490(1);
    ((WaitState *)D_001A63A8)->step4e = 0; ((WaitState *)D_001A63A8)->step4c = 0; ((WaitState *)D_001A63A8)->busy44 = 0;
    if (((WaitState *)D_001A63A8)->mode != -1) ((WaitState *)D_001A63A8)->value48 = ((WaitState *)D_001A63A8)->mode;
    ((WaitState *)D_001A63A8)->step72 = 0; ((WaitState *)D_001A63A8)->step70 = 0; ((WaitState *)D_001A63A8)->busy68 = 0;
    ((WaitState *)D_001A63A8)->step96 = 0; ((WaitState *)D_001A63A8)->step94 = 0; ((WaitState *)D_001A63A8)->busy8c = 0;
    ((WaitState *)D_001A63A8)->clear2c = 0; ((WaitState *)D_001A63A8)->mode = -1;
    ((WaitState *)D_001A63A8)->next = -1;
}

/* Clear one aligned 128-bit object through architectural zero. */
void FUN_00282C88(TI *a0) { *a0 = 0; }

/* Write the measured GS privileged 64-bit register configuration in order. */
typedef unsigned long long GsRegisterValue;
extern GsRegisterValue D_001A6488[3];

void FUN_0027B948(void)
{
    GsRegisterValue framebuffer, display;
    *(volatile GsRegisterValue *)0x120000e0 = 0;
    *(volatile GsRegisterValue *)0x12000000 = 0xffa1;
    *(volatile GsRegisterValue *)0x12000020 = D_001A6488[0];
    framebuffer = D_001A6488[1];
    *(volatile GsRegisterValue *)0x12000070 = framebuffer;
    *(volatile GsRegisterValue *)0x12000090 = framebuffer;
    display = D_001A6488[2];
    *(volatile GsRegisterValue *)0x12000080 = display;
    *(volatile GsRegisterValue *)0x120000a0 = display;
    *(volatile GsRegisterValue *)0x120000d0 = 0;
}

/* Set the value of each enabled resident channel in its observed update order. */
typedef struct ResidentChannel64 {
    short flags;
    short value;
    u8 fields4[0x20];
} ResidentChannel64;
typedef struct ResidentChannels64 {
    u8 fields0[0x50];
    ResidentChannel64 channels[3];
} ResidentChannels64;

void FUN_002B7340(s32 value)
{
    ResidentChannels64 *state = (ResidentChannels64 *)D_001A63A8;
    if (state->channels[0].flags & 0x8000)
        state->channels[0].value = value;
    if (state->channels[2].flags & 0x8000)
        state->channels[2].value = value;
    if (state->channels[1].flags & 0x8000)
        state->channels[1].value = value;
}

/* Construct a double through the original runtime packing routine. */
typedef struct DoubleParts44 {
    s32 kind;
    u32 sign;
    s32 exponent;
    unsigned long long fraction;
} DoubleParts44;
extern double FUN_00122630(DoubleParts44 *);

double FUN_00123268(s32 kind, u32 sign, s32 exponent, unsigned long long fraction)
{
    DoubleParts44 parts;
    parts.kind = kind;
    parts.sign = sign;
    parts.exponent = exponent;
    parts.fraction = fraction;
    return FUN_00122630(&parts);
}
/* libgcc single-precision software floating point, EE build - vendored copy.
 *
 * Modified distribution prepared by the RAC2 decompilation campaign on
 * 5 October 2026: upstream fp-bit.c was preprocessed for the EE single-float
 * configuration and one symbol was renamed. The original copyright,
 * GPL version 2-or-later notice, additional permissions, linking exception,
 * warranty disclaimer and author credits are retained below. Only
 * trailing whitespace on added comment lines is normalized.
 * See COPYING in this directory for the GNU GPL version 2 text. The root
 * MIT licence does not relicense this source or its generated copy within
 * candidates/boot.c.
 *
 * Provenance:
 *   upstream   rac1-decomp src/libgcc/fp-bit.c, commit
 *              cb22f0b0d3a171d1fd4b6851b86fe214a22c9822, sha256
 *              3069e3a1385e9b2d316929a676a71e2af8a834666b7388a89f10c52ae0e17336
 *   transform  one pass of the campaign's reconstructed 2.9-ee cpp:
 *              cpp -P -DFLOAT -DFLOAT_BIT_ORDER_MISMATCH -DNO_DENORMALS
 *                  -DUS_SOFTWARE_GOFAST
 *   rename     __unpack_f becomes FUN_00123400 at 16 occurrences in the
 *              preprocessed body. This is a mechanical, reversible rename;
 *              fptodp and every other body keep their original names.
 *
 * The published body is derived from the immutable two-symbol private trial;
 * it is independently qualified in the generated complete boot unit. The
 * refused fptodp body remains present and receives no matching credit.
 * The body between the markers is unchanged from the published PR13 source
 * at f39ac431876c1e05e88b9a2e28dce855a1e4f9cb. Only notices were restored.
 * See README.md in this directory and docs/RAC1-FP-BIT-EVIDENCE.md.
 */

/* This is a software floating point library which can be used instead of
   the floating point routines in libgcc1.c for targets without hardware
   floating point.
 Copyright (C) 1994, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.

This file is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file with other programs, and to distribute
those programs without any restriction coming from the use of this
file.  (The General Public License restrictions do apply in other
respects; for example, they cover modification of the file, and
distribution when not linked into another program.)

This file is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* As a special exception, if you link this library with other files,
   some of which are compiled with GCC, to produce an executable,
   this library does not by itself cause the resulting executable
   to be covered by the GNU General Public License.
   This exception does not however invalidate any other reasons why
   the executable file might be covered by the GNU General Public License.  */

/* This implements IEEE 754 format arithmetic, but does not provide a
   mechanism for setting the rounding mode, or for generating or handling
   exceptions.

   The original code by Steve Chamberlain, hacked by Mark Eichin and Jim
   Wilson, all of Cygnus Support.  */

/* --- verbatim preprocessed fp-bit.c begins here --- */
 



























 






 






 


 
















































 

















 












typedef float SFtype __attribute__ ((mode (SF)));
typedef float DFtype __attribute__ ((mode (DF)));

typedef int HItype __attribute__ ((mode (HI)));
typedef int SItype __attribute__ ((mode (SI)));
typedef int DItype __attribute__ ((mode (DI)));

 




typedef unsigned int UHItype __attribute__ ((mode (HI)));
typedef unsigned int USItype __attribute__ ((mode (SI)));
typedef unsigned int UDItype __attribute__ ((mode (DI)));























	typedef USItype fractype;
	typedef UHItype halffractype;
	typedef SFtype FLO_type;
	typedef SItype intfrac;
























 


 
 










 

typedef enum
{
  CLASS_SNAN,
  CLASS_QNAN,
  CLASS_ZERO,
  CLASS_NUMBER,
  CLASS_INFINITY
} fp_class_type;

typedef struct
{

  fp_class_type class;
  unsigned int sign;
  int normal_exp;


  union
    {
      fractype ll;
      halffractype l[2];
    } fraction;
} fp_number_type;

typedef union
{
  FLO_type value;
  fractype value_raw;




  struct
    {
      fractype fraction: 23  __attribute__ ((packed));
      unsigned int exp: 8  __attribute__ ((packed));
      unsigned int sign:1 __attribute__ ((packed));
    }
  bits;



}
FLO_union_type;


 

 



__inline__ 
static fp_number_type *
nan ()
{
   




  extern fp_number_type __thenan_df;

  return &__thenan_df;
}

__inline__ 
static int
isnan ( fp_number_type *  x)
{
  return x->class == CLASS_SNAN || x->class == CLASS_QNAN;
}

__inline__ 
static int
isinf ( fp_number_type *  x)
{
  return x->class == CLASS_INFINITY;
}



__inline__ 
static int
iszero ( fp_number_type *  x)
{
  return x->class == CLASS_ZERO;
}

__inline__  
static void
flip_sign ( fp_number_type *  x)
{
  x->sign = !x->sign;
}

extern FLO_type __pack_f  ( fp_number_type * );


FLO_type
__pack_f  ( fp_number_type *  src)
{
  FLO_union_type dst;
  fractype fraction = src->fraction.ll;	 
  int sign = src->sign;
  int exp = 0;

  if (isnan (src))
    {
      exp = (0xff) ;
      if (src->class == CLASS_QNAN || 1)
	{
	  fraction |= 0x100000L ;
	}
    }
  else if (isinf (src))
    {
      exp = (0xff) ;
      fraction = 0;
    }
  else if (iszero (src))
    {
      exp = 0;
      fraction = 0;
    }
  else if (fraction == 0)
    {
      exp = 0;
    }
  else
    {
      if (src->normal_exp < (-(127 )+1) )
	{
	   



	  int shift = (-(127 )+1)  - src->normal_exp;

	  exp = 0;

	  if (shift > 32  - 7L )
	    {
	       
	      fraction = 0;
	    }
	  else
	    {
	       
	      fraction >>= shift;
	    }
	  fraction >>= 7L ;
	}
      else if (src->normal_exp > 127 )
	{
	  exp = (0xff) ;
	  fraction = 0;
	}
      else
	{
	  exp = src->normal_exp + 127 ;
	   


	  if ((fraction & 0x7f ) == 0x40 )
	    {
	      if (fraction & (1 << 7L ))
		fraction += 0x3f  + 1;
	    }
	  else
	    {
	       
	      fraction += 0x3f ;
	    }
	  if (fraction >= (1LL<<(23 +1+ 7L )) )
	    {
	      fraction >>= 1;
	      exp += 1;
	    }
	  fraction >>= 7L ;
	}
    }

   



  dst.bits.fraction = fraction;
  dst.bits.exp = exp;
  dst.bits.sign = sign;




  return dst.value;
}


extern void FUN_00123400  (FLO_union_type *, fp_number_type *);


void
FUN_00123400  (FLO_union_type * src, fp_number_type * dst)
{
   


  fractype fraction;
  int exp;
  int sign;


  

  fraction = src->bits.fraction;
  exp = src->bits.exp;
  sign = src->bits.sign;


  dst->sign = sign;
  if (exp == 0)
    {
       
       



      if (fraction == 0

	  || 1

	  )
	{
	   
	  dst->class = CLASS_ZERO;
	}
      else
	{
	   


	  dst->normal_exp = exp - 127  + 1;
	  fraction <<= 7L ;

	  dst->class = CLASS_NUMBER;

	  while (fraction < (1LL<<(23 + 7L )) )
	    {
	      fraction <<= 1;
	      dst->normal_exp--;
	    }

	  dst->fraction.ll = fraction;
	}
    }
  else if (exp == (0xff) )
    {
       
      if (fraction == 0)
	{
	   
	  dst->class = CLASS_INFINITY;
	}
      else
	{
	   
	  if (fraction & 0x100000L )
	    {
	      dst->class = CLASS_QNAN;
	    }
	  else
	    {
	      dst->class = CLASS_SNAN;
	    }
	   
	  dst->fraction.ll = fraction;
	}
    }
  else
    {
       
      dst->normal_exp = exp - 127 ;
      dst->class = CLASS_NUMBER;
      dst->fraction.ll = (fraction << 7L ) | (1LL<<(23 + 7L )) ;
    }
}



static fp_number_type *
_fpadd_parts (fp_number_type * a,
	      fp_number_type * b,
	      fp_number_type * tmp)
{
  intfrac tfraction;

   
  int a_normal_exp;
  int b_normal_exp;
  fractype a_fraction;
  fractype b_fraction;

  if (isnan (a))
    {
      return a;
    }
  if (isnan (b))
    {
      return b;
    }
  if (isinf (a))
    {
       
      if (isinf (b) && a->sign != b->sign)
	return nan ();
      return a;
    }
  if (isinf (b))
    {
      return b;
    }
  if (iszero (b))
    {
      if (iszero (a))
	{
	  *tmp = *a;
	  tmp->sign = a->sign & b->sign;
	  return tmp;
	}
      return a;
    }
  if (iszero (a))
    {
      return b;
    }

   

  {
    int diff;

    a_normal_exp = a->normal_exp;
    b_normal_exp = b->normal_exp;
    a_fraction = a->fraction.ll;
    b_fraction = b->fraction.ll;

    diff = a_normal_exp - b_normal_exp;

    if (diff < 0)
      diff = -diff;
    if (diff < 32 )
      {
	 
	while (a_normal_exp > b_normal_exp)
	  {
	    b_normal_exp++;
	    {  b_fraction  = ( b_fraction  & 1) | ( b_fraction  >> 1); } ;
	  }
	while (b_normal_exp > a_normal_exp)
	  {
	    a_normal_exp++;
	    {  a_fraction  = ( a_fraction  & 1) | ( a_fraction  >> 1); } ;
	  }
      }
    else
      {
	 
	if (a_normal_exp > b_normal_exp)
	  {
	    b_normal_exp = a_normal_exp;
	    b_fraction = 0;
	  }
	else
	  {
	    a_normal_exp = b_normal_exp;
	    a_fraction = 0;
	  }
      }
  }

  if (a->sign != b->sign)
    {
      if (a->sign)
	{
	  tfraction = -a_fraction + b_fraction;
	}
      else
	{
	  tfraction = a_fraction - b_fraction;
	}
      if (tfraction >= 0)
	{
	  tmp->sign = 0;
	  tmp->normal_exp = a_normal_exp;
	  tmp->fraction.ll = tfraction;
	}
      else
	{
	  tmp->sign = 1;
	  tmp->normal_exp = a_normal_exp;
	  tmp->fraction.ll = -tfraction;
	}
       

      while (tmp->fraction.ll < (1LL<<(23 + 7L ))  && tmp->fraction.ll)
	{
	  tmp->fraction.ll <<= 1;
	  tmp->normal_exp--;
	}
    }
  else
    {
      tmp->sign = a->sign;
      tmp->normal_exp = a_normal_exp;
      tmp->fraction.ll = a_fraction + b_fraction;
    }
  tmp->class = CLASS_NUMBER;
   


  if (tmp->fraction.ll >= (1LL<<(23 +1+ 7L )) )
    {
      {  tmp->fraction.ll  = ( tmp->fraction.ll  & 1) | ( tmp->fraction.ll  >> 1); } ;
      tmp->normal_exp++;
    }
  return tmp;

}

FLO_type
fpadd  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  res = _fpadd_parts (&a, &b, &tmp);

  return __pack_f  (res);
}

FLO_type
fpsub  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  b.sign ^= 1;

  res = _fpadd_parts (&a, &b, &tmp);

  return __pack_f  (res);
}



static __inline__  fp_number_type *
_fpmul_parts ( fp_number_type *  a,
	       fp_number_type *  b,
	       fp_number_type * tmp)
{
  fractype low = 0;
  fractype high = 0;

  if (isnan (a))
    {
      a->sign = a->sign != b->sign;
      return a;
    }
  if (isnan (b))
    {
      b->sign = a->sign != b->sign;
      return b;
    }
  if (isinf (a))
    {
      if (iszero (b))
	return nan ();
      a->sign = a->sign != b->sign;
      return a;
    }
  if (isinf (b))
    {
      if (iszero (a))
	{
	  return nan ();
	}
      b->sign = a->sign != b->sign;
      return b;
    }
  if (iszero (a))
    {
      a->sign = a->sign != b->sign;
      return a;
    }
  if (iszero (b))
    {
      b->sign = a->sign != b->sign;
      return b;
    }

   

  {

    {
       


      DItype answer = (DItype)(a->fraction.ll) * (DItype)(b->fraction.ll);
      
      high = answer >> 32;
      low = answer;
    }

  }

  tmp->normal_exp = a->normal_exp + b->normal_exp;
  tmp->sign = a->sign != b->sign;

  tmp->normal_exp += 2;		 

  while (high >= (1LL<<(23 +1+ 7L )) )
    {
      tmp->normal_exp++;
      if (high & 1)
	{
	  low >>= 1;
	  low |= 0x80000000L ;
	}
      high >>= 1;
    }
  while (high < (1LL<<(23 + 7L )) )
    {
      tmp->normal_exp--;

      high <<= 1;
      if (low & 0x80000000L )
	high |= 1;
      low <<= 1;
    }
   

  if ((high & 0x7f ) == 0x40 )
    {
      if (high & (1 << 7L ))
	{
	   
	  high += 0x3f  + 1;
	}
      else if (low)
	{
	   
	  high += 0x3f  + 1;
	}
    }
  tmp->fraction.ll = high;
  tmp->class = CLASS_NUMBER;
  return tmp;
}

FLO_type
fpmul  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type tmp;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  res = _fpmul_parts (&a, &b, &tmp);

  return __pack_f  (res);
}



static __inline__  fp_number_type *
_fpdiv_parts (fp_number_type * a,
	      fp_number_type * b)
{
  fractype bit;
  fractype numerator;
  fractype denominator;
  fractype quotient;

  if (isnan (a))
    {
      return a;
    }
  if (isnan (b))
    {
      return b;
    }

  a->sign = a->sign ^ b->sign;

  if (isinf (a) || iszero (a))
    {
      if (a->class == b->class)
	return nan ();
      return a;
    }

  if (isinf (b))
    {
      a->fraction.ll = 0;
      a->normal_exp = 0;
      return a;
    }
  if (iszero (b))
    {
      a->class = CLASS_INFINITY;
      return a;
    }

   

  {
     



    a->normal_exp = a->normal_exp - b->normal_exp;
    numerator = a->fraction.ll;
    denominator = b->fraction.ll;

    if (numerator < denominator)
      {
	 
	numerator *= 2;
	a->normal_exp--;
      }
    bit = (1LL<<(23 + 7L )) ;
    quotient = 0;
     
    while (bit)
      {
	if (numerator >= denominator)
	  {
	    quotient |= bit;
	    numerator -= denominator;
	  }
	bit >>= 1;
	numerator *= 2;
      }

    if ((quotient & 0x7f ) == 0x40 )
      {
	if (quotient & (1 << 7L ))
	  {
	     
	    quotient += 0x3f  + 1;
	  }
	else if (numerator)
	  {
	     
	    quotient += 0x3f  + 1;
	  }
      }

    a->fraction.ll = quotient;
    return (a);
  }
}

FLO_type
fpdiv  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;
  fp_number_type *res;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  res = _fpdiv_parts (&a, &b);

  return __pack_f  (res);
}


int __fpcmp_parts_f  (fp_number_type * a, fp_number_type *b);


 





int
__fpcmp_parts_f  (fp_number_type * a, fp_number_type * b)
{


  if (isnan (a) || isnan (b))
    {
      return 1;			 
    }
  if (isinf (a) && isinf (b))
    {
       
       







      return b->sign - a->sign;
    }
   
  if (isinf (a))
    {
      return a->sign ? -1 : 1;
    }
  if (isinf (b))
    {
      return b->sign ? 1 : -1;
    }
  if (iszero (a) && iszero (b))
    {
      return 0;
    }
  if (iszero (a))
    {
      return b->sign ? 1 : -1;
    }
  if (iszero (b))
    {
      return a->sign ? -1 : 1;
    }
   
  if (a->sign != b->sign)
    {
       
      return a->sign ? -1 : 1;
    }
   
  if (a->normal_exp > b->normal_exp)
    {
      return a->sign ? -1 : 1;
    }
  if (a->normal_exp < b->normal_exp)
    {
      return a->sign ? 1 : -1;
    }
   
  if (a->fraction.ll > b->fraction.ll)
    {
      return a->sign ? -1 : 1;
    }
  if (a->fraction.ll < b->fraction.ll)
    {
      return a->sign ? 1 : -1;
    }
   
  return 0;
}



SItype 
fpcmp  (FLO_type arg_a, FLO_type arg_b)
{
  fp_number_type a;
  fp_number_type b;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  FUN_00123400  ((FLO_union_type *) & arg_b, &b);

  return __fpcmp_parts_f  (&a, &b);
}





FLO_type
sitofp  (SItype arg_a)
{
  fp_number_type in;

  in.class = CLASS_NUMBER;
  in.sign = arg_a < 0;
  if (!arg_a)
    {
      in.class = CLASS_ZERO;
    }
  else
    {
      in.normal_exp = 23  + 7L ;
      if (in.sign) 
	{
	   

	  if (arg_a == (SItype) 0x80000000)
	    {
	      return -2147483648.0;
	    }
	  in.fraction.ll = (-arg_a);
	}
      else
	in.fraction.ll = arg_a;

      while (in.fraction.ll < (1LL << (23  + 7L )))
	{
	  in.fraction.ll <<= 1;
	  in.normal_exp -= 1;
	}
    }
  return __pack_f  (&in);
}



SItype
fptosi  (FLO_type arg_a)
{
  fp_number_type a;
  SItype tmp;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  if (iszero (&a))
    return 0;
  if (isnan (&a))
    return 0;
   
  if (isinf (&a))
    return a.sign ? (- ((SItype) ((unsigned) (~0)>>1)) )-1 : ((SItype) ((unsigned) (~0)>>1)) ;
   
  if (a.normal_exp < 0)
    return 0;
  if (a.normal_exp > 30)
    return a.sign ? (- ((SItype) ((unsigned) (~0)>>1)) )-1 : ((SItype) ((unsigned) (~0)>>1)) ;
  tmp = a.fraction.ll >> ((23  + 7L ) - a.normal_exp);
  return a.sign ? (-tmp) : (tmp);
}




 





USItype
fptoui  (FLO_type arg_a)
{
  fp_number_type a;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  if (iszero (&a))
    return 0;
  if (isnan (&a))
    return 0;
   
  if (a.sign)
    return 0;
   
  if (isinf (&a))
    return ((USItype) ~0) ;
   
  if (a.normal_exp < 0)
    return 0;
  if (a.normal_exp > 31)
    return ((USItype) ~0) ;
  else if (a.normal_exp > (23  + 7L ))
    return a.fraction.ll << (a.normal_exp - (23  + 7L ));
  else
    return a.fraction.ll >> ((23  + 7L ) - a.normal_exp);
}




FLO_type
__negsf2  (FLO_type arg_a)
{
  fp_number_type a;

  FUN_00123400  ((FLO_union_type *) & arg_a, &a);
  flip_sign (&a);
  return __pack_f  (&a);
}





SFtype
__make_fp(fp_class_type class,
	     unsigned int sign,
	     int exp, 
	     USItype frac)
{
  fp_number_type in;

  in.class = class;
  in.sign = sign;
  in.normal_exp = exp;
  in.fraction.ll = frac;
  return __pack_f  (&in);
}




 




extern DFtype __make_dp (fp_class_type, unsigned int, int, UDItype frac);


DFtype
fptodp  (SFtype arg_a)
{
  fp_number_type in;

  FUN_00123400  ((FLO_union_type *) & arg_a, &in);
  return __make_dp (in.class, in.sign, in.normal_exp,
		    ((UDItype) in.fraction.ll) << (52+8-(23+7)) );
}







/* --- verbatim preprocessed fp-bit.c ends here --- */

extern u8 BOOT_D_001395B8[];

typedef struct {
    u8 pad0000[0x0000];
} ResidentBase1395B8;

s32 FUN_00294E48(s32 index)
{
    s32 byte = index / 8;
    s32 bit = index % 8;
    s32 old;

    if ((u32)bit < 8) {
        old = (BOOT_D_001395B8[byte + 0xA7] >> bit) & 1;
    } else {
        old = 0;
    }
    if ((u32)bit < 8) {
        BOOT_D_001395B8[byte + 0xA7] |= 1 << bit;
    }
    return old;
}

extern unsigned char D_19B278[];

int FUN_002B0DB8(void)
{
    int count = 0;
    int i;

    for (i = 0; i < 0x1C; i++) {
        int j;

        for (j = 0; j < 4; j++) {
            if (D_19B278[i * 4 + j] != 0)
                count++;
        }
    }
    if (count < 0)
        count = 0;
    if (count > 0x28)
        count = 0x28;
    return count;
}

/* Adapted mechanically from Lombyte src/sdk/library/dual_prime_vector.c.
 * Source last change f18ea57e965bf7e88630871cc3b230b05b07c587;
 * current upstream2c4452dd03f5f7ebb2868dbe073ccdb1afb27f61 has identical source.
 * Source SHA256 e6df19f23bb23e15cf030b7d3c5222fae6d1fc4ba4411cbff4a03f31fc014b51.
 * Only the include of already-identical EE u8/s32 types and function name
 * change. Algorithm, parameter types, layout, expressions and order unchanged.
 * RAC1 matching source/object identity does not prove this RAC2 C candidate.
 */
/*
MIT License

Copyright (c) 2026 Mateusz Kłysz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
typedef struct {
    u8 pad0[0x174];
    s32 pictStruct;
    s32 topFieldFirst;
} MpegDec;

void FUN_0012B3E0(MpegDec *d, s32 DMV[][2], s32 *dmvector, s32 mvx, s32 mvy) {
    if (d->pictStruct == 3) {
        if (d->topFieldFirst) {
            DMV[0][0] = ((mvx + (mvx > 0)) >> 1) + dmvector[0];
            DMV[0][1] = ((mvy + (mvy > 0)) >> 1) + dmvector[1] - 1;
            DMV[1][0] = ((3 * mvx + (mvx > 0)) >> 1) + dmvector[0];
            DMV[1][1] = ((3 * mvy + (mvy > 0)) >> 1) + dmvector[1] + 1;
        } else {
            DMV[0][0] = ((3 * mvx + (mvx > 0)) >> 1) + dmvector[0];
            DMV[0][1] = ((3 * mvy + (mvy > 0)) >> 1) + dmvector[1] - 1;
            DMV[1][0] = ((mvx + (mvx > 0)) >> 1) + dmvector[0];
            DMV[1][1] = ((mvy + (mvy > 0)) >> 1) + dmvector[1] + 1;
        }
    } else {
        DMV[0][0] = ((mvx + (mvx > 0)) >> 1) + dmvector[0];
        DMV[0][1] = ((mvy + (mvy > 0)) >> 1) + dmvector[1];
        if (d->pictStruct == 1) {
            DMV[0][1]--;
        } else {
            DMV[0][1]++;
        }
    }
}

/* Copied mechanically from Lombyte sdk/library/update_temp_track_data.c; current source and actual
 * SDK-built full object/ref body pinned in donor-proof.json. Algorithm,
 * layout, signedness, expressions and source order preserved. Function
 * rename and duplicate EE scalar-type/include context removal only.
 * The donor SDK compiler b9aef69 differs; RAC2 acceptance requires its own qualification. */
/*
MIT License

Copyright (c) 2026 Mateusz Kłysz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
struct M2c_arg0
{
  u8 pad_0[0x150];
  s32 unk150;
  u8 pad_154[0x58];
  s32 unk1AC;
  u8 pad_1B0[0x69C];
  s32 unk84C;
  s32 unk850;
  s32 unk854;
};
void FUN_0012CFE8(struct M2c_arg0 *arg0, s32 arg1)
{
  s32 temp_2_30;
  s32 temp_3_21;
  s32 temp_4_32;
  s32 var_4_8;
  s32 var_7_4;
  var_7_4 = 0;
  var_4_8 = 0;
  if ((arg0->unk150 != 3) && (arg1 != 0))
  {
    if (arg1 < 0)
    {
      var_7_4 = arg0->unk854 == 0;
    }
    arg0->unk854 = 0;
    var_4_8 = arg1;
  }
  temp_3_21 = arg0->unk84C + arg1;
  arg0->unk1AC = temp_3_21;
  if ((var_7_4 != 0) && (var_4_8 >= arg1))
  {
    arg0->unk1AC = (s32) (temp_3_21 + 0x400);
  }
  ;
  temp_4_32 = arg0->unk1AC;
  arg0->unk850 = (s32) ((arg0->unk850 < temp_4_32) ? (temp_4_32) : (arg0->unk850));
}

/* Copied mechanically from Lombyte sdk/video/ipu/sce_ipu_sync.c; current source and actual
 * SDK-built full object/ref body pinned in donor-proof.json. Algorithm,
 * layout, signedness, expressions and source order preserved. Function
 * rename and duplicate EE scalar-type/include context removal only.
 * The donor SDK compiler b9aef69 differs; RAC2 acceptance requires its own qualification. */
/*
MIT License

Copyright (c) 2026 Mateusz Kłysz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
u32 FUN_00130DB8(s32 arg0) {
    u32 result;

    result = 0;
    switch (arg0) {
    case 0:
        while (*(volatile s32 *)0x10002010 < 0) {
        }
        result = 0;
        break;
    case 1:
        result = *(volatile u32 *)0x10002010 >> 31;
        break;
    default:
        break;
    }
    return result;
}


void FUN_0026F720(void)
{
}


void FUN_0026F728(void)
{
}


unsigned int FUN_0026F730(void)
{
    return 0;
}


unsigned int FUN_0026F738(void)
{
    return 0;
}


unsigned int FUN_0026F740(void)
{
    return 0;
}


unsigned int FUN_0026F748(void)
{
    return 0;
}


void FUN_0026F750(void)
{
}


void FUN_0026F758(void)
{
}


void FUN_0026F760(void)
{
}


void FUN_0026F768(void)
{
}


void FUN_0026F770(void)
{
}


void FUN_0026F778(void)
{
}


void FUN_0026F780(void)
{
}


void FUN_0026F788(void)
{
}


void FUN_0026F790(void)
{
}


void FUN_0026F798(void)
{
}


unsigned int FUN_0026F7A0(void)
{
    return 0;
}


void FUN_0026F7B0(void)
{
}


unsigned int FUN_0026F7B8(void)
{
    return 0;
}


unsigned int FUN_0026F7C0(void)
{
    return 0;
}


unsigned int FUN_0026F7C8(void)
{
    return 0;
}


unsigned int FUN_0026F7D0(void)
{
    return 0;
}


unsigned int FUN_0026F7D8(void)
{
    return 0;
}


unsigned int FUN_0026F7E0(void)
{
    return 0;
}


unsigned int FUN_0026F7E8(void)
{
    return 0;
}


unsigned int FUN_0026F7F0(void)
{
    return 0;
}


unsigned int FUN_0026F7F8(void)
{
    return 0;
}


unsigned int FUN_0026F800(void)
{
    return 0;
}


void FUN_0026F808(void)
{
}


void FUN_002820C8(void)
{
}


void FUN_00288D88(void)
{
}


void FUN_00289AA0(void)
{
}


void FUN_002912F0(void)
{
}


void FUN_00298170(void)
{
}


void FUN_002FF288(void)
{
}


void FUN_00336DA0(void)
{
}


void FUN_00338808(void)
{
}


void FUN_00338A58(void)
{
}


void FUN_00338F50(void)
{
}


void FUN_003417C8(void)
{
}


void FUN_00342420(void)
{
}


void FUN_0034E218(void)
{
}


void FUN_003505A8(void)
{
}


void FUN_00351E40(void)
{
}
void FUN_00316B40(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_0031E750(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_00328CA8(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_0032B2F0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -1.0f;
    *(float *)(p + 24) = 1.0f;
    *(float *)(p + 40) = -1.0f;
    *(float *)(p + 56) = 1.0f;
    *(float *)(p + 4) = -1.0f;
    *(float *)(p + 36) = 1.0f;
    *(float *)(p + 52) = 1.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 280993940374112LL;
    *(float *)(p + 20) = -1.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_00305838(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_00307C30(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_0031C8E0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_003220A8(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}
void FUN_003253A0(char *p)
{
    *(long long *)(p + 112) = 0LL;
    *(int *)(p + 80) = 0;
    *(int *)(p + 84) = 0;
    *(float *)(p + 88) = 1.0f;
    *(int *)(p + 92) = 0;
    *(int *)(p + 96) = 0;
    *(float *)(p + 100) = 1.0f;
    *(float *)(p + 104) = 1.0f;
    *(float *)(p + 108) = 1.0f;
    *(float *)(p + 8) = -2.0f;
    *(float *)(p + 24) = 2.0f;
    *(float *)(p + 40) = -2.0f;
    *(float *)(p + 56) = 2.0f;
    *(float *)(p + 4) = -2.0f;
    *(float *)(p + 36) = 2.0f;
    *(int *)(p + 0) = 0;
    *(int *)(p + 16) = 0;
    *(int *)(p + 32) = 0;
    *(int *)(p + 48) = 0;
    *(float *)(p + 12) = 1.0f;
    *(float *)(p + 28) = 1.0f;
    *(float *)(p + 44) = 1.0f;
    *(long long *)(p + 128) = 0x0000FF9000000260LL;
    *(float *)(p + 20) = -2.0f;
    *(float *)(p + 52) = 2.0f;
    *(float *)(p + 60) = 1.0f;
}

struct Slot { s32 w; s32 rest[4]; };
struct Table1 { char pad[19264]; struct Slot slots[48]; };
struct Table2 { char pad[52]; s32 slots[4]; };

extern struct Table1 Fee2b87d1_D_0014B540;
extern struct Table2 Fee2b87d1_D_00152CD0;

s32 FUN_00294630(s32 value) {
    s32 i = 0;
    s32 *q;
    if (Fee2b87d1_D_0014B540.slots[0].w == value) goto after1;
    while (++i < 48) {
        if (Fee2b87d1_D_0014B540.slots[i].w == value) break;
    }
after1:
    if (i == 48) return 1;
    value = 0;
    if (Fee2b87d1_D_00152CD0.slots[0] == i) goto after2;
    while (++value < 3) {
        if (Fee2b87d1_D_00152CD0.slots[value] == i) break;
    }
after2:
    return value != 3;
}


unsigned int FUN_0012EA70(unsigned int *param_1, int param_2)
{
  unsigned int uVar1;
  
  uVar1 = *(param_1 + 2) + (param_2 >> 3);
  if (*(param_1 + 9) <= uVar1) {
    uVar1 = uVar1 - *(param_1 + 10);
  }
  return uVar1;
}


/* Returns the constant integer 1. Matches FUN_0012F940 disassembly and decompilation. */
int FUN_0012F940(void) {
    return 1;
}


void FUN_0012FB70(void* param_1)
{
  *(int*)(param_1 + 8) = *(int*)(param_1 + 12);
}


extern int FUN_00132C48(int, int, void *);

int FUN_00133490(int value)
{
    int payload[4];
    payload[0] = value;
    return FUN_00132C48(0x36, 4, payload);
}



extern int FUN_00132C48(int, int, void *);

void FUN_001338C8(void)
{
  FUN_00132C48(0x3c, 0, 0);
}


extern int FUN_00132C48(int, int, void *);

int FUN_00133930(int first, int second)
{
    struct {
        unsigned int first;
        unsigned int second;
        unsigned int pad[2];
    } payload;
    payload.first = first;
    payload.second = second;
    return FUN_00132C48(0x5a, 8, &payload.first);
}



void FUN_001339F0(void *dest, const void *src, int n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    int i = 0;

    if (n <= 0) {
        return;
    }

    do {
        d[i] = s[i];
        i++;
    } while (i < n);
}



extern void FUN_002889B8(unsigned char *);

void FUN_00288B18(void *owner)
{
    FUN_002889B8(owner);
}



void FUN_00288B38(int);
void FUN_00288B18(void*);

void FUN_00288BB8(void *param)
{
  FUN_00288B38((int)param);
  *(unsigned int *)param = 0;
  FUN_00288B18(param);
}


extern void FUN_00291788(unsigned long long);
extern void FUN_00291640(unsigned long long);
void FUN_00291758(unsigned long long param_1)
{
    FUN_00291788(param_1);
    FUN_00291640(param_1);
}



void FUN_00282A60(unsigned int *dst, unsigned int val, int size);

void FUN_002A01C8(void)
{
    FUN_00282A60((unsigned int *)0x70003A00, 0x40000000, 0x3C0);
}


/* Measured partial C views; original types and object ownership are unknown. */
typedef unsigned char QwenRecovery_00beefd949f6_AbiByte15;
typedef unsigned long long QwenRecovery_00beefd949f6_AbiFlag15;




extern void FUN_002AFFD8(QwenRecovery_00beefd949f6_AbiByte15 *input, float *output, QwenRecovery_00beefd949f6_AbiFlag15 mode);

void FUN_002B00B0(QwenRecovery_00beefd949f6_AbiByte15 *param_1, float *output, QwenRecovery_00beefd949f6_AbiFlag15 mode)
{
    FUN_002AFFD8(param_1 + 0x10, output, mode);
    return;
}



/* Measured partial C views; original types and object ownership are unknown. */
typedef unsigned char QwenRecovery_84e23fd54f9f_AbiByte16;




extern int FUN_002B50E0(QwenRecovery_84e23fd54f9f_AbiByte16 *owner, QwenRecovery_84e23fd54f9f_AbiByte16 *input, float factor);

void FUN_002B5278(QwenRecovery_84e23fd54f9f_AbiByte16 *param_1, float factor)
{
    FUN_002B50E0(param_1, param_1 + 0x10, factor);
}



/*
 * Disassembled at 0x002cd220
 * Signature: int FUN_002cd220(void)
 * Behavior:
 *   - Saves RA on stack
 *   - Calls FUN_0027C540(0)
 *   - Calls FUN_0029C138()
 *   - Calls FUN_0027C660()
 *   - Restores RA from stack
 *   - Returns 0
 */
extern void FUN_0027C540(long);
extern void FUN_0029C138(void);
extern void FUN_0027C660(void);

int FUN_002CD220(void)
{
  FUN_0027C540(0);
  FUN_0029C138();
  FUN_0027C660();
  return 0;
}


unsigned long long FUN_002CD620(void);

extern void FUN_0027C540(long);
extern void FUN_0029C1D8(void);
extern void FUN_0027C660(void);

unsigned long long FUN_002CD620(void)
{
  FUN_0027C540(0);
  FUN_0029C1D8();
  FUN_0027C660();
  return 0;
}


/* Function at 0x002cd8c0: FUN_002CD8C0
 * 
 * Disassembly summary:
 * - 12 instructions, 48 bytes total
 * - Prologue: save ra, allocate 16-byte stack frame
 * - Three calls to external functions:
 *   1. FUN_0027C540(0)
 *   2. FUN_0029C450()
 *   3. FUN_0027C660()
 * - Epilogue: restore ra, return 0, deallocate stack
 * - No local variables used
 * - Return type is void-like; returns zero in v0
 * - Delay slots are_NOP/unused in this case
 */

/* External function declarations (exact addresses from task metadata) */
extern void FUN_0027C540(long);
extern void FUN_0029C450(void);
extern void FUN_0027C660(void);

long long FUN_002CD8C0(void)
{
    FUN_0027C540(0LL);
    FUN_0029C450();
    FUN_0027C660();
    return 0LL;
}


extern void FUN_0027C540(long);
extern void FUN_0029C680(void);
extern void FUN_0027C660(void);

int FUN_002D2CB8(void)
{
    FUN_0027C540(0);
    FUN_0029C680();
    FUN_0027C660();
    return 0;
}


void FUN_0027C540(long);
void FUN_0027C660(void);
void FUN_0029C730(void);

unsigned long long FUN_002D2E28(void)
{
    FUN_0027C540(0);
    FUN_0029C730();
    FUN_0027C660();
    return 0;
}



/* 
 * Function: FUN_002D2EE0
 * 
 * Disassembly summary:
 * - Saves return address on stack
 * - Calls FUN_0027C540(0) (with zero argument)
 * - Calls FUN_0029C7A0()
 * - Calls FUN_0027C660()
 * - Restores return address
 * - Returns 0
 *
 * Notes:
 * - The function follows standard MIPS ABI prologue/epilogue.
 * - Delay slots are filled with NOP or argument setup as seen in disassembly.
 * - No local variables used.
 * - All external function addresses come from the assigned externals list.
 */

extern void FUN_0027C540(long arg0);
extern void FUN_0029C7A0(void);
extern void FUN_0027C660(void);

unsigned long long FUN_002D2EE0(void)
{
    FUN_0027C540(0);
    FUN_0029C7A0();
    FUN_0027C660();
    return 0;
}


/* External function declarations as per target specification */
extern void FUN_0027C540(long);
extern void FUN_0027C660(void);
extern void FUN_0029C8F0(void);

/* Implementation matching decompiled output */
long FUN_002D3290(void)
{
  FUN_0027C540(0);
  FUN_0029C8F0();
  FUN_0027C660();
  return 0;
}


/* External functions declared with exact aliases from task.externals */
extern void FUN_0027C540(long param_1);
extern void FUN_0027C660(void);
extern void FUN_0029C960(void);

/* Candidate function: FUN_002d3390 */
unsigned long long FUN_002D3390(void)
{
    FUN_0027C540(0);
    FUN_0029C960();
    FUN_0027C660();
    return 0;
}


/* Target: FUN_002d5540 on MIPS (SCUS_972.68, -O2 -G0 -ffunction-sections)
 * Calls three externals: FUN_0027C540(int), FUN_0029CED8(void), FUN_0027C660(void)
 * Returns 0. Stack frame size 16 bytes. Delay slots filled.
 */
extern void FUN_0027C540(long);
extern void FUN_0029CED8(void);
extern void FUN_0027C660(void);

long long FUN_002D5540(void)
{
    FUN_0027C540(0);
    FUN_0029CED8();
    FUN_0027C660();
    return 0;
}


unsigned int FUN_0031AB08(unsigned int param_1)
{
    unsigned int inner_ptr;
    unsigned int result;

    inner_ptr = *(unsigned int *)(param_1 + 0x68);
    result = *(unsigned int *)(inner_ptr + 0x18);
    return result;
}


void FUN_00336A30(int param_1, unsigned int param_2)
{
  *(unsigned int*)(param_1 + 0x44) = param_2;
  return;
}


void FUN_00336A38(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0x40) = param_2;
  return;
}


extern void FUN_0033AE18(unsigned char *owner, int value);

void FUN_00339770(unsigned char *owner, int value)
{
    FUN_0033AE18(owner + 8, value);
}



void* FUN_0033E170(void* param_1);

extern void FUN_0033AB50(int);
extern unsigned long long FUN_00347EA0(unsigned long long);

void* FUN_0033E170(void* param_1)
{
  FUN_0033AB50((int)param_1 + 8);
  FUN_00347EA0((int)param_1 + 0x2b0);
  return param_1;
}


extern int FUN_0033E348(int);

int FUN_0033E310(int owner)
{
    int result;
    result = FUN_0033E348(owner);
    return *(int *)(owner + 0x378) + result * 0x14;
}



extern int FUN_00348128(unsigned char *);

int FUN_0033E348(int owner)
{
    return FUN_00348128((unsigned char *)(owner + 0x2b0));
}



void FUN_00348138(int);

void FUN_00341A68(int param_1)
{
  FUN_00348138(param_1 + 0x228);
  return;
}


extern void FUN_00348068(unsigned char *owner, float *records);

void FUN_00342380(unsigned char *owner, float *records)
{
    *(float **)(owner + 0x250) = records;
    FUN_00348068(owner + 0x188, records);
}



void FUN_00348138(int param_1);

void FUN_00342648(int param_1)
{
  FUN_00348138(param_1 + 0x188);
  return;
}


/* Target: FUN_003427B0 — identity function returning its first argument */
/* Disassembly: jr ra; move v0,a0 (MIPS delay slot) */
/* Compiled with: -O2 -G0 -ffunction-sections */

unsigned long FUN_003427B0(unsigned long param_1)
{
  return param_1;
}


/* Measured partial C views; original types and object ownership are unknown. */
typedef unsigned char QwenRecovery_a2544d80ec96_AbiByte22;




extern void FUN_00342F70(QwenRecovery_a2544d80ec96_AbiByte22 *owner, const QwenRecovery_a2544d80ec96_AbiByte22 *table);

void FUN_00343538(QwenRecovery_a2544d80ec96_AbiByte22 *p1, const QwenRecovery_a2544d80ec96_AbiByte22 *table)
{
    FUN_00342F70(p1 + 0x298, table);
}



extern int FUN_00342BC0(unsigned char *owner);

int FUN_00343560(unsigned char *owner)
{
    return FUN_00342BC0(owner + 0x298);
}



extern void FUN_00343058(unsigned char *owner);

void FUN_00343580(unsigned char *owner)
{
    FUN_00343058(owner + 0x298);
}



extern unsigned long long FUN_00336968(unsigned long long);

unsigned long long FUN_00347EA0(unsigned long long value)
{
    FUN_00336968(value);
    return value;
}



void FUN_003480E8(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 0xb8) = param_2;
  return;
}


void FUN_00349488(int *param_1)
{
    param_1[0x150 / sizeof(int)] = 0;
}


void FUN_00349628(int param_1, int param_2) {
    *(int *)(param_1 + 0x84) = param_2;
}


void FUN_00349670(int param_1, unsigned int param_2)
{
  *(unsigned int *)(param_1 + 32) = param_2;
  return;
}


extern void FUN_00350A78(int *owner, int *first_address, int *first_count,
                         int *second_address, int *second_count);

void FUN_00351848(unsigned char *owner, int *first_address, int *first_count,
                  int *second_address, int *second_count)
{
    FUN_00350A78((int *)(owner + 0x48), first_address, first_count,
                 second_address, second_count);
}



extern int FUN_00351268(unsigned char *owner);

int FUN_00351938(unsigned char *owner)
{
    return FUN_00351268(owner + 0x48);
}



extern int FUN_00351E58(unsigned char *);

int FUN_00351EE8(int *owner)
{
    int result;
    long status;
    status = FUN_00351E58((unsigned char *)owner);
    if (status == 0)
        result = *owner + owner[2] * 0xd0000;
    else
        result = 0;
    return result;
}


/* Owner-reviewed Qwen backlog: observed operations, not recovered original object boundaries. */

/* FUN_00285840: owner ABI/declaration review retained. */
/* extern declaration for global variable referenced in FUN_00285840 */
extern unsigned int D_001BACC0;

unsigned int FUN_00285840(void)
{
    return D_001BACC0;
}

/* FUN_00285850: owner ABI/declaration review retained. */
extern unsigned int D_001BAD08;

unsigned int FUN_00285850(void)
{
    return D_001BAD08;
}

/* FUN_002858B0: owner ABI/declaration review retained. */
extern int D_001BAD14;

void FUN_002858B0(int param_1)
{
  D_001BAD14 = param_1;
}

/* FUN_002ED870: owner ABI/declaration review retained. */
extern unsigned int D_00214FCC;

unsigned int FUN_002ED870(void)
{
    return D_00214FCC;
}

/* FUN_00287E48: owner ABI/declaration review retained. */
/* Compare the observed 32-bit cell with 7; original signedness is unknown. */
extern unsigned int D_001BACC0;
int FUN_00287E48(void)
{
    return D_001BACC0 == 7;
}

/* FUN_002D7AC8: owner ABI/declaration review retained. */
extern int D_001C51D4;
int FUN_002D7AC8(void)
{
    D_001C51D4 = -1;
    return 0;
}

/* FUN_002EE6A8: owner ABI/declaration review retained. */
extern int FUN_002ED688(unsigned char *owner);
unsigned int FUN_002EE6A8(unsigned char *owner)
{
    unsigned int address = (unsigned int)FUN_002ED688(owner);
    if (address == 0)
        return 0;
    return *(unsigned int *)(address + 0x100u);
}

/* FUN_0034CA68: owner ABI/declaration review retained. */
extern int FUN_00335E20(unsigned char *field);
unsigned int FUN_0034CA68(unsigned char *owner)
{
    unsigned int address = (unsigned int)FUN_00335E20(owner + 0x358);
    return *(unsigned int *)address & 0xff000000u;
}

/* FUN_002EE678: owner ABI/declaration review retained. */
extern int FUN_002ED688(unsigned char *owner);
void FUN_002EE678(unsigned char *owner, unsigned int value)
{
    unsigned int address = (unsigned int)FUN_002ED688(owner);
    if (address != 0)
        *(unsigned int *)(address + 0x100u) = value;
}

/* FUN_0011D118: owner ABI/declaration review retained. */
void FUN_0011CB58(void);

extern int D_001346A0;

void FUN_0011D118(void)
{
  FUN_0011CB58();
  D_001346A0 = 0;
  return;
}
extern void Fc936841d_FUN_00282CF0(char *local, char *first, char *second);
extern void Fc936841d_FUN_00282CC0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void FUN_00316DA0(Vec16_Fc936841d *object, int index)
{
    char local[16];

    Fc936841d_FUN_00282CF0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    Fc936841d_FUN_00282CC0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void Fc936841d_FUN_00282CF0(char *local, char *first, char *second);
extern void Fc936841d_FUN_00282CC0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void FUN_0031F1A8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    Fc936841d_FUN_00282CF0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    Fc936841d_FUN_00282CC0((char *)&object[index - 1], (char *)&object[index - 2], local);
}
extern void Fc936841d_FUN_00282CF0(char *local, char *first, char *second);
extern void Fc936841d_FUN_00282CC0(char *target, char *first, char *local);

#ifndef RAC2_T_VEC16_FC936841D
#define RAC2_T_VEC16_FC936841D
typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vec16_Fc936841d;
#endif


void FUN_0032BDC8(Vec16_Fc936841d *object, int index)
{
    char local[16];

    Fc936841d_FUN_00282CF0(local, (char *)&object[index - 2], (char *)&object[index - 3]);
    Fc936841d_FUN_00282CC0((char *)&object[index - 1], (char *)&object[index - 2], local);
}



extern void F01bd4546_FUN_00335E68(char *p, int v);
extern void F01bd4546_FUN_00349630(char *p, int v);
extern void F01bd4546_FUN_00349630(char *p, int v);
extern void F01bd4546_FUN_00349630(char *p, int v);
extern void F01bd4546_FUN_00349630(char *p, int v);
extern void F01bd4546_FUN_00349630(char *p, int v);
extern void F01bd4546_FUN_00349630(char *p, int v);
extern void F01bd4546_FUN_00349678(char *p, float v);
extern void F01bd4546_FUN_00349678(char *p, float v);
extern void F01bd4546_FUN_00349678(char *p, float v);
extern void F01bd4546_FUN_00349678(char *p, float v);
extern void F01bd4546_FUN_00349678(char *p, float v);
extern void F01bd4546_FUN_00349678(char *p, float v);

void FUN_0034CB88(char *self, int x)
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    *(s32 *)(self + 5492) = x;
    if (*(s32 *)(self + 5496) != 0) {
        *(s32 *)(self + 5496) = 0;
        F01bd4546_FUN_00335E68(self + 1464, 0);
        p1 = self + 3052;
        p2 = self + 3188;
        p3 = self + 3324;
        p4 = self + 3460;
        p5 = self + 3596;
        p6 = self + 3732;
        F01bd4546_FUN_00349630(p1, 1);
        F01bd4546_FUN_00349630(p2, 1);
        F01bd4546_FUN_00349630(p3, 1);
        F01bd4546_FUN_00349630(p4, 1);
        F01bd4546_FUN_00349630(p5, 1);
        F01bd4546_FUN_00349630(p6, 1);
        F01bd4546_FUN_00349678(p1, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        F01bd4546_FUN_00349678(p2, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        F01bd4546_FUN_00349678(p3, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        F01bd4546_FUN_00349678(p4, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        F01bd4546_FUN_00349678(p5, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
        F01bd4546_FUN_00349678(p6, *(s32 *)0x1A7B90 ? 0.14f : 0.116666675f);
    }
}
extern void F70997ba9_FUN_002FC560(int a, int b);
extern void F70997ba9_FUN_0027C540(int a);
extern void *F70997ba9_FUN_00288FB8(int id);
extern int F70997ba9_FUN_0027F198(void *p, int i);
extern void F70997ba9_FUN_0027F120(void);
extern void F70997ba9_FUN_0027F5B8(int a, int b, long c, void *d, int e);
extern void F70997ba9_FUN_0027F110(void);
extern void F70997ba9_FUN_0027C660(void);

int FUN_002D9798(int *arg0)
{
    int min;
    int v;
    int slot;
    int count;
    int off;

    F70997ba9_FUN_002FC560(66, 68);
    F70997ba9_FUN_002FC560(71, 11);
    F70997ba9_FUN_0027C540(0);
    min = F70997ba9_FUN_0027F198(F70997ba9_FUN_00288FB8(11613), -1);
    v = F70997ba9_FUN_0027F198(F70997ba9_FUN_00288FB8(11625), -1);
    if (v >= min)
        min = v;
    if (*(int *)0x1A79F0 != 0) {
        v = F70997ba9_FUN_0027F198(F70997ba9_FUN_00288FB8(11614), -1);
        if (v >= min)
            min = v;
    }
    v = F70997ba9_FUN_0027F198(F70997ba9_FUN_00288FB8(11626), -1);
    if (v >= min)
        min = v;
    v = F70997ba9_FUN_0027F198(F70997ba9_FUN_00288FB8(11627), -1);
    if (v >= min)
        min = v;
    v = F70997ba9_FUN_0027F198(F70997ba9_FUN_00288FB8(11599), -1);
    if (v >= min)
        min = v;
    slot = (arg0[8] - min) >> 1;
    if (slot <= 1)
        slot = 2;
    count = arg0[9] / (*(int *)0x1A79F0 != 0 ? 7 : 6);
    F70997ba9_FUN_0027F120();
    off = count - 6;
    F70997ba9_FUN_0027F5B8(slot, off, 0x80FFA888L, F70997ba9_FUN_00288FB8(11613), -1);
    off += count;
    F70997ba9_FUN_0027F5B8(slot, off, 0x80FFA888L, F70997ba9_FUN_00288FB8(11625), -1);
    off += count;
    F70997ba9_FUN_0027F5B8(slot, off, 0x80FFA888L, F70997ba9_FUN_00288FB8(11626), -1);
    off += count;
    F70997ba9_FUN_0027F5B8(slot, off, 0x80FFA888L, F70997ba9_FUN_00288FB8(11627), -1);
    off += count;
    F70997ba9_FUN_0027F5B8(slot, off, 0x80FFA888L, F70997ba9_FUN_00288FB8(11599), -1);
    F70997ba9_FUN_0027F110();
    F70997ba9_FUN_0027C660();
    return 2;
}
#ifndef RAC2_T_WORKER_F9E2CD219
#define RAC2_T_WORKER_F9E2CD219
typedef struct {
    unsigned char pad00[76];
    short field4C;
    unsigned char pad4E[2];
    int field50;
    float field54;
    float field58;
} Worker_F9e2cd219;
#endif


#ifndef RAC2_T_OWNER_F9E2CD219
#define RAC2_T_OWNER_F9E2CD219
typedef struct {
    unsigned char pad00[104];
    Worker_F9e2cd219 *child;
} Owner_F9e2cd219;
#endif


extern float F9e2cd219_FUN_002A7798(float value);
extern void F9e2cd219_FUN_002ACF68(void *out, void *in, void *tmp, int mode, float value);
extern void F9e2cd219_FUN_00305310(void *owner, void *out);

void FUN_00305228(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    F9e2cd219_FUN_002ACF68((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            F9e2cd219_FUN_002A7798(child->field54));

    child->field54 = child->field54 + child->field58;
    F9e2cd219_FUN_00305310(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
#ifndef RAC2_T_WORKER_F9E2CD219
#define RAC2_T_WORKER_F9E2CD219
typedef struct {
    unsigned char pad00[76];
    short field4C;
    unsigned char pad4E[2];
    int field50;
    float field54;
    float field58;
} Worker_F9e2cd219;
#endif


#ifndef RAC2_T_OWNER_F9E2CD219
#define RAC2_T_OWNER_F9E2CD219
typedef struct {
    unsigned char pad00[104];
    Worker_F9e2cd219 *child;
} Owner_F9e2cd219;
#endif


extern float F9e2cd219_FUN_002A7798(float value);
extern void F9e2cd219_FUN_002ACF68(void *out, void *in, void *tmp, int mode, float value);
extern void F9e2cd219_FUN_00307AD0(void *owner, void *out);

void FUN_003079E8(Owner_F9e2cd219 *self)
{
    Worker_F9e2cd219 *child = self->child;

    F9e2cd219_FUN_002ACF68((char *)child + 48, (char *)child + 16, (char *)child + 32, 0,
            F9e2cd219_FUN_002A7798(child->field54));

    child->field54 = child->field54 + child->field58;
    F9e2cd219_FUN_00307AD0(self, (char *)child + 48);

    if (child->field54 > 0.9f)
        child->field50 = 1;
    else
        child->field50 = 0;

    if (child->field54 >= 1.0f) {
        child->field54 = 1.0f;
        child->field4C = 1;
    }
}
extern void F6df9730c_FUN_00282B50(char *dst, int *src, int count);

void FUN_002979D8(char *dst, unsigned char *src)
{
    int table[256];
    int tmp[16];
    int *q;
    int i, j, k;

    for (i = 0; i < 256; i++) {
        table[i] = 0;
        if (i & 0x01) table[i] = 0x0000000f;
        if (i & 0x02) table[i] |= 0x000000f0;
        if (i & 0x04) table[i] |= 0x00000f00;
        if (i & 0x08) table[i] |= 0x0000f000;
        if (i & 0x10) table[i] |= 0x000f0000;
        if (i & 0x20) table[i] |= 0x00f00000;
        if (i & 0x40) table[i] |= 0x0f000000;
        if (i & 0x80) table[i] |= 0xf0000000;
    }
    for (j = 0; j < 128; j++) {
        char *next;
        q = tmp;
        next = dst + 64;
        for (k = 15; k >= 0; k--) {
            *q++ = table[*src++];
        }
        F6df9730c_FUN_00282B50(dst, tmp, 64);
        dst = next;
        F6df9730c_FUN_00282B50(dst, tmp, 64);
        dst += 64;
        F6df9730c_FUN_00282B50(dst, tmp, 64);
        dst += 64;
        F6df9730c_FUN_00282B50(dst, tmp, 64);
        dst += 64;
    }
}
extern void Fdb046c5d_FUN_002742E8(void *);
extern void Fdb046c5d_FUN_00274128(void *);
extern int Fdb046c5d_FUN_00274500(void *);
extern void Fdb046c5d_FUN_00282F20(float, void *, void *);
extern void Fdb046c5d_FUN_00282DD8(void *, void *, void *);
extern void Fdb046c5d_FUN_00274A88(void *, void *, void *);
extern void Fdb046c5d_FUN_002AFFD8(void *, void *, int);
extern int Fdb046c5d_FUN_00274BA8(void *, void *, void *, float, float);
extern int Fdb046c5d_FUN_002E5CF8(int, int, void *);
extern void Fdb046c5d_FUN_00274768(void *, int, void *);
extern int Fdb046c5d_FUN_002E5CF8(int, int, void *);
extern void Fdb046c5d_FUN_002E5F00(int, void *);
extern char Fdb046c5d_D_001BAFC0[];

int FUN_00273D10(char *obj)
{
    float buf[4];
    float out[4];
    char *p;
    char *s1;
    unsigned char k;
    int r;
    int s4;
    int s5;

    p = *(char **)(obj + 104);
    k = *(unsigned char *)(p + 92);
    if (k >= 3)
        return 1;
    if (k != 0)
        Fdb046c5d_FUN_002742E8(obj);
    else
        Fdb046c5d_FUN_00274128(obj);

    s5 = Fdb046c5d_FUN_00274500(obj);
    if (s5 == -1)
        return 0;

    s1 = Fdb046c5d_D_001BAFC0;
    s4 = -1;
    Fdb046c5d_FUN_00282F20(1.0f, s1, s1);
    Fdb046c5d_FUN_00282DD8(buf, s1, p);
    Fdb046c5d_FUN_00274A88(obj, p + 16, buf);
    Fdb046c5d_FUN_002AFFD8(obj + 16, out, 1);
    r = Fdb046c5d_FUN_00274BA8(p, s1, out, *(float *)(p + 56), *(float *)(p + 60));
    if (r == 0) {
        if (*(unsigned char *)(p + 94) != 255 && *(unsigned char *)(p + 92) == 0)
            s4 = Fdb046c5d_FUN_002E5CF8(*(unsigned char *)(p + 94), 0, obj);
        Fdb046c5d_FUN_00274768(obj, s5, out);
    } else {
        if (*(unsigned char *)(p + 94) != 255)
            s4 = Fdb046c5d_FUN_002E5CF8(*(unsigned char *)(p + 94), 0, obj);
    }
    if (s4 != -1)
        Fdb046c5d_FUN_002E5F00(s4, p + 32);
    return 0;
}
extern void F7242f0a4_FUN_00282D30(char *out, void *source, float value);
extern void F7242f0a4_FUN_00334730(char *buffer, int mode);
extern void F7242f0a4_FUN_00334710(int value, char *buffer);
extern void F7242f0a4_FUN_00282CC0(char *first, char *second, char *third);
extern float F7242f0a4_FUN_00282DB0(void *owner, char *buffer);

extern int F7242f0a4_D_001B53A0[];

void FUN_003349E8(void *owner, int flag, float value)
{
    char buffer[32];
    int *root = F7242f0a4_D_001B53A0;

    F7242f0a4_FUN_00282D30(buffer, root + 8, value);
    if (flag)
        F7242f0a4_FUN_00334730(buffer + 16, 1);
    else
        F7242f0a4_FUN_00334710(root[-4], buffer + 16);
    F7242f0a4_FUN_00282CC0(buffer, buffer, buffer + 16);
    F7242f0a4_FUN_00282D30(owner, root + 12, F7242f0a4_FUN_00282DB0(root + 12, buffer));
}
extern char F45821cfb_D_001C2740[];
extern void F45821cfb_FUN_00282C88(char *);

void FUN_0034EBE0(void)
{
    float *p = (float *)F45821cfb_D_001C2740;

    p[224] = 0.4f;
    p[225] = 0.8f;
    p[226] = 1.2f;
    *(int *)&p[227] = 0;
    p[228] = 0.577f;
    p[229] = 0.577f;
    p[230] = -0.577f;
    *(int *)&p[231] = 0;
    F45821cfb_FUN_00282C88((char *)&p[232]);
    F45821cfb_FUN_00282C88((char *)&p[236]);
}
extern char Fd8e166aa_D_001B7F80[];

void FUN_00279828(void)
{
    char *g = Fd8e166aa_D_001B7F80;
    int i = 0;
    int *table = *(int **)(g + 8);
    char *p = (char *)table + table[*(unsigned char *)0x1A7BB4];
    int n = *(int *)p;

    *(char **)(g + 12) = p + 8;
    *(int *)(g + 16) = n;
    for (i = 0; i < *(int *)(g + 16); i++) {
        char *q = *(char **)(g + 12);
        char *r = q - 8;

        *(int *)(q + i * 16) = *(int *)(q + i * 16) + (int)r;
    }
}
extern char F7cb419c1_D_001F2840[];

extern int F7cb419c1_FUN_002DE618(int arg);

int FUN_002D4E10(void)
{
    char *p = F7cb419c1_D_001F2840;

    *(int *)(p + 460) = F7cb419c1_FUN_002DE618(*(int *)(p + 460));
    return 0;
}
extern char F0a76d85b_D_001B5200[];

extern void F0a76d85b_FUN_00283360(char *p);
extern void F0a76d85b_FUN_00279D18(void);

void FUN_0034EC70(void)
{
    char *p = F0a76d85b_D_001B5200;

    *(int *)(p + 320) = 0;
    *(int *)(p + 324) = 0;
    *(int *)(p + 328) = 0;
    F0a76d85b_FUN_00283360(p + 880);
    F0a76d85b_FUN_00279D18();
}
extern char F391de845_D_001B5340[];
extern float F391de845_FUN_00282E48(char *a, char *b);
extern float F391de845_FUN_002E45C8(void *self, float d, float x, float y);

float FUN_002E46C0(char *self, char *p)
{
    float v = F391de845_FUN_00282E48(p, F391de845_D_001B5340);
    float *q = *(float **)(self + 8);

    return F391de845_FUN_002E45C8(q, v, q[0], q[1]);
}
extern void F46b43b72_FUN_002B7B18(int value, char *target);
extern void F46b43b72_FUN_002B7CE0(int value);

extern int F46b43b72_D_001B8840[];
extern int F46b43b72_D_0014B540[];

int FUN_00293CA8(int index)
{
    int j = index + 1;
    int *d = F46b43b72_D_001B8840;
    int *b = F46b43b72_D_0014B540;
    int *t = (int *)((char *)b + 25416);
    int n = d[12] * 332;
    int hold = d[28];
    int j4 = j * 4;
    int cur = *(int *)((char *)t + (n + index * 4));
    int next = *(int *)((char *)t + (j4 + n));

    n = next - cur;
    if (n > 0) {
        F46b43b72_FUN_002B7B18(hold, (char *)(cur + b[6341]));
        F46b43b72_FUN_002B7CE0(0);
    }
    return 1;
}
extern void Fa2d20de7_FUN_002A7988(void *object, float first, float second);
extern void Fa2d20de7_FUN_00282CC0(char *first, char *second, void *third);
extern int Fa2d20de7_FUN_002761F8(void *first, char *second, int mode, int value, int extra);
extern void Fa2d20de7_FUN_00282CF0(void *first, void *second, void *third);
extern void Fa2d20de7_FUN_00282D30(void *first, void *second, float value);

extern short Fa2d20de7_D_001B5340[];
extern short Fa2d20de7_D_001BAFA0[];
extern int Fa2d20de7_D_001886CC[];

void FUN_002E4478(void *object)
{
    Fa2d20de7_FUN_002A7988(object, 0.5f, 6.0f);
    Fa2d20de7_FUN_00282CC0(object, object, Fa2d20de7_D_001B5340);
    if (Fa2d20de7_FUN_002761F8(Fa2d20de7_D_001B5340, object, 130, Fa2d20de7_D_001886CC[0], 0)) {
        Fa2d20de7_FUN_00282CF0(object, Fa2d20de7_D_001BAFA0, Fa2d20de7_D_001B5340);
        Fa2d20de7_FUN_00282D30(object, object, 0.75f);
        Fa2d20de7_FUN_00282CC0(object, object, Fa2d20de7_D_001B5340);
    }
}

#ifndef RAC2_T_S8_F4778F810
#define RAC2_T_S8_F4778F810
typedef signed char s8_F4778f810;
#endif

#ifndef RAC2_T_S16_F4778F810
#define RAC2_T_S16_F4778F810
typedef signed short s16_F4778f810;
#endif

#ifndef RAC2_T_U16_F4778F810
#define RAC2_T_U16_F4778F810
typedef unsigned short u16_F4778f810;
#endif





extern void F4778f810_FUN_002B00D8(char *a, char *b, char *c, f32 d);
extern void F4778f810_FUN_00282CC0(char *a, char *b, char *c);
extern void F4778f810_FUN_00283098(char *a, char *b, char *c);
extern void F4778f810_FUN_00283958(char *a, char *b);
extern void F4778f810_FUN_00283738(char *a, char *b, char *c);
extern void F4778f810_FUN_00283098(char *a, char *b, char *c);
extern void F4778f810_FUN_00282CF0(char *a, char *b, char *c);
extern void F4778f810_FUN_00282CF0(char *a, char *b, char *c);

#ifndef RAC2_T_V4_F4778F810
#define RAC2_T_V4_F4778F810
typedef struct {
    f32 x, y, z, w;
} V4_F4778f810;
#endif


#ifndef RAC2_T_SUB_F4778F810
#define RAC2_T_SUB_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x10];
    u8 at020[0x28];
    f32 f048;
} Sub_F4778f810;
#endif


#ifndef RAC2_T_OBJ_F4778F810
#define RAC2_T_OBJ_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x58];
    char *p068;
    u8 pad06C[0xC0 - 0x6C];
    u8 at0C0[0x10];
} Obj_F4778f810;
#endif


void FUN_00274128(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    F4778f810_FUN_002B00D8((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    F4778f810_FUN_00282CC0((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    F4778f810_FUN_00283098((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    F4778f810_FUN_00283958((char *)p + 0x10, (char *)&tmp[3]);
    F4778f810_FUN_00283738((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    F4778f810_FUN_00283098((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    F4778f810_FUN_00282CF0((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    F4778f810_FUN_00282CF0((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}

#ifndef RAC2_T_S8_F4778F810
#define RAC2_T_S8_F4778F810
typedef signed char s8_F4778f810;
#endif

#ifndef RAC2_T_S16_F4778F810
#define RAC2_T_S16_F4778F810
typedef signed short s16_F4778f810;
#endif

#ifndef RAC2_T_U16_F4778F810
#define RAC2_T_U16_F4778F810
typedef unsigned short u16_F4778f810;
#endif





extern void F4778f810_FUN_002B00D8(char *a, char *b, char *c, f32 d);
extern void F4778f810_FUN_00282CC0(char *a, char *b, char *c);
extern void F4778f810_FUN_00283098(char *a, char *b, char *c);
extern void F4778f810_FUN_00283958(char *a, char *b);
extern void F4778f810_FUN_00283738(char *a, char *b, char *c);
extern void F4778f810_FUN_00283098(char *a, char *b, char *c);
extern void F4778f810_FUN_00282CF0(char *a, char *b, char *c);
extern void F4778f810_FUN_00282CF0(char *a, char *b, char *c);

#ifndef RAC2_T_V4_F4778F810
#define RAC2_T_V4_F4778F810
typedef struct {
    f32 x, y, z, w;
} V4_F4778f810;
#endif


#ifndef RAC2_T_SUB_F4778F810
#define RAC2_T_SUB_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x10];
    u8 at020[0x28];
    f32 f048;
} Sub_F4778f810;
#endif


#ifndef RAC2_T_OBJ_F4778F810
#define RAC2_T_OBJ_F4778F810
typedef struct {
    u8 pad000[0x10];
    u8 at010[0x58];
    char *p068;
    u8 pad06C[0xC0 - 0x6C];
    u8 at0C0[0x10];
} Obj_F4778f810;
#endif


void FUN_00274208(Obj_F4778f810 *obj)
{
    V4_F4778f810 tmp[6];
    Sub_F4778f810 *p = (Sub_F4778f810 *)obj->p068;

    F4778f810_FUN_002B00D8((char *)obj + 0x10, (char *)p, (char *)p, p->f048);
    F4778f810_FUN_00282CC0((char *)obj + 0x10, (char *)obj + 0x10, (char *)p);
    F4778f810_FUN_00283098((char *)&tmp[0], (char *)p + 0x20, (char *)obj + 0xC0);
    F4778f810_FUN_00283958((char *)p + 0x10, (char *)&tmp[3]);
    F4778f810_FUN_00283738((char *)obj + 0xC0, (char *)&tmp[3], (char *)obj + 0xC0);
    F4778f810_FUN_00283098((char *)&tmp[1], (char *)p + 0x20, (char *)obj + 0xC0);
    F4778f810_FUN_00282CF0((char *)&tmp[2], (char *)&tmp[1], (char *)&tmp[0]);
    F4778f810_FUN_00282CF0((char *)obj + 0x10, (char *)obj + 0x10, (char *)&tmp[2]);
}
extern char F669d6320_D_001BAFC0[];
extern void F669d6320_FUN_00282F20(void *out, void *in, float scale);
extern void F669d6320_FUN_00282CC0(void *out, void *in1, void *in2);
extern int F669d6320_FUN_002761F8(void *a, void *b, int mode, void *self, int flag);
extern void F669d6320_FUN_00282D30(void *out, void *in, float scale);

void FUN_0031A998(char *self, char *p1, int p2)
{
    float a[4];
    float b[4];

    F669d6320_FUN_00282F20(a, self + 224, 0.1f);
    F669d6320_FUN_00282F20(b, self + 224, -3.0f);
    F669d6320_FUN_00282CC0(a, a, self + 16);
    F669d6320_FUN_00282CC0(b, b, self + 16);

    if (F669d6320_FUN_002761F8(a, b, 2, self, 0))
        F669d6320_FUN_00282F20(p1, F669d6320_D_001BAFC0, 1.0f);
    else
        F669d6320_FUN_00282F20(p1, self + 224, -1.0f);

    if (p2 == 0)
        F669d6320_FUN_00282D30(p1, p1, -1.0f);
}
extern void F8411efa9_FUN_00328CA8(char *pkt);
extern long long F8411efa9_FUN_0027C878(char *p);
extern float F8411efa9_FUN_00283248(float x, float y);
extern float F8411efa9_FUN_00282E20(char *p);
extern float F8411efa9_FUN_00283248(float x, float y);
extern void F8411efa9_FUN_00283410(float *matrix, float *quat);
extern void F8411efa9_FUN_00282F20(float *off, char *src, float scale);
extern void F8411efa9_FUN_00328D30(char *pkt, float angle);
extern void F8411efa9_FUN_00280B90(char *pkt, float *matrix, int mode);

void FUN_00328D98(char *pos, int a1, int a2, char *a3, int t0,
                                        char *src, float f12, float f13)
{
    char pkt[144];
    float m[16];
    float quat[4];
    float off[4];
    long long r;

    F8411efa9_FUN_00328CA8(pkt);
    r = F8411efa9_FUN_0027C878(a3);

    *(int *)(pkt + 76) = a1 + (a2 << 24);
    *(int *)(pkt + 72) = a1 + (a2 << 24);
    *(int *)(pkt + 68) = a1 + (a2 << 24);
    *(int *)(pkt + 64) = a1 + (a2 << 24);
    *(long long *)(pkt + 112) = 0LL;
    *(long long *)(pkt + 120) = r;
    *(long long *)(pkt + 128) = 280993940374112LL;
    *(long long *)(pkt + 136) = ((long long)t0 << 2) | 0x8000000040LL;

    quat[3] = 0.0f;
    quat[2] = F8411efa9_FUN_00283248(*(float *)(src + 0), *(float *)(src + 4));
    quat[1] = -F8411efa9_FUN_00283248(F8411efa9_FUN_00282E20(src), *(float *)(src + 8));
    quat[0] = f13;

    F8411efa9_FUN_00283410(m, quat);
    F8411efa9_FUN_00282F20(off, src, 0.025f);

    m[12] = *(float *)(pos + 0) + off[0];
    m[13] = *(float *)(pos + 4) + off[1];
    m[14] = *(float *)(pos + 8) + off[2];
    m[15] = 1.0f;

    F8411efa9_FUN_00328D30(pkt, f12);
    F8411efa9_FUN_00280B90(pkt, m, 0);
}
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);

void FUN_0031E8F8(char *p, float scale)
{
    F2c74c194_FUN_00282D30(p, p, scale);
    F2c74c194_FUN_00282D30(p + 16, p + 16, scale);
    F2c74c194_FUN_00282D30(p + 32, p + 32, scale);
    F2c74c194_FUN_00282D30(p + 48, p + 48, scale);
}
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);

void FUN_00328D30(char *p, float scale)
{
    F2c74c194_FUN_00282D30(p, p, scale);
    F2c74c194_FUN_00282D30(p + 16, p + 16, scale);
    F2c74c194_FUN_00282D30(p + 32, p + 32, scale);
    F2c74c194_FUN_00282D30(p + 48, p + 48, scale);
}
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);
extern void F2c74c194_FUN_00282D30(char *dst, char *src, float scale);

void FUN_0032B378(char *p, float scale)
{
    F2c74c194_FUN_00282D30(p, p, scale);
    F2c74c194_FUN_00282D30(p + 16, p + 16, scale);
    F2c74c194_FUN_00282D30(p + 32, p + 32, scale);
    F2c74c194_FUN_00282D30(p + 48, p + 48, scale);
}
extern int F6eb4f363_FUN_002A77E0(int mode);
extern float F6eb4f363_FUN_002A90F8(char *object, char *local);
extern void F6eb4f363_FUN_00282D30(char *local, int value, float scale);
extern float F6eb4f363_FUN_002A7878(float low, float high);
extern void F6eb4f363_FUN_002BF728(char *object, char *local, float amount, float base);

void FUN_00304B68(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (F6eb4f363_FUN_002A77E0(5))
        return;
    base = F6eb4f363_FUN_002A90F8(object + 16, local);
    F6eb4f363_FUN_00282D30(local, value, 0.25f);
    F6eb4f363_FUN_002BF728(object + 16, local, F6eb4f363_FUN_002A7878(0.05f, 0.1f) * 210000.0f, base);
}
extern int F6eb4f363_FUN_002A77E0(int mode);
extern float F6eb4f363_FUN_002A90F8(char *object, char *local);
extern void F6eb4f363_FUN_00282D30(char *local, int value, float scale);
extern float F6eb4f363_FUN_002A7878(float low, float high);
extern void F6eb4f363_FUN_002BF728(char *object, char *local, float amount, float base);

void FUN_00307330(char *object)
{
    char local[16];
    float base;
    int value = *(int *)(object + 104);

    if (F6eb4f363_FUN_002A77E0(5))
        return;
    base = F6eb4f363_FUN_002A90F8(object + 16, local);
    F6eb4f363_FUN_00282D30(local, value, 0.25f);
    F6eb4f363_FUN_002BF728(object + 16, local, F6eb4f363_FUN_002A7878(0.05f, 0.1f) * 210000.0f, base);
}
extern int Fc10c1216_FUN_00282978(char *);
extern void Fc10c1216_FUN_002B9180(char *);
extern int Fc10c1216_FUN_00283CF0(float);
extern void Fc10c1216_FUN_00282CC0(char *, char *, char *);

void FUN_002BE9B8(char *p)
{
    float *q = (float *)(p + 32);

    if (0.0f < q[3])
        q[1] = q[1] + q[3] * 0.007f;
    else if (0.03f < q[1])
        q[1] = q[1] + q[3] * 0.007f;
    else {
        q[1] = q[1] + q[3] * 1.4000000664964318275452e-03f;
        *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    }

    if (q[1] <= 0.0244f || Fc10c1216_FUN_00282978(p + 10) != 0) {
        Fc10c1216_FUN_002B9180(p);
        return;
    }

    if (q[1] >= 0.12f) {
        q[3] = -q[3];
        q[1] = q[1] + q[3] * 0.007f;
    }
    *(float *)(p + 12) = *(float *)(p + 12) + 5460.0f;
    *(int *)(p + 4) = (*(int *)(p + 4) & 0xffffff) | (Fc10c1216_FUN_00283CF0(q[1] * 255.0f) << 24);
    q[2] = q[2] + 0.002f;
    if (q[2] > 1.0f)
        q[2] = q[2] - 1.0f;
    *(char *)(p + 8) = (char)(q[2] * 255.0f);
    Fc10c1216_FUN_00282CC0(p + 16, p + 16, (char *)(q + 4));
}
extern int F250fbfa4_FUN_00282978(char *p);
extern void F250fbfa4_FUN_002B9180(char *p);
extern void F250fbfa4_FUN_00282CC0(char *p0, char *p1, char *p2);

void FUN_002C7AE8(char *a0)
{
    char local[16];
    char *s1 = a0;
    char *s0;
    int a1;
    int v1;
    char *p;

    if (F250fbfa4_FUN_00282978(a0 + 10) != 0) {
        F250fbfa4_FUN_002B9180(a0);
        return;
    }
    s0 = a0 + 32;
    a1 = *(int *)(s1 + 4);
    v1 = (a1 >> 24) + *(unsigned char *)(s0 + 20);
    *(int *)(s1 + 4) = (a1 & 0x00ffffff) | (v1 << 24);
    p = s1 + 16;
    *(float *)(local + 0) = *(float *)(s1 + 32);
    *(float *)(local + 4) = *(float *)(s0 + 4);
    *(float *)(local + 8) = *(float *)(s0 + 8);
    *(int *)(local + 12) = 0;
    F250fbfa4_FUN_00282CC0(p, p, local);
    *(float *)(s0 + 8) = *(float *)(s0 + 8) - *(float *)(s0 + 12);
    *(unsigned char *)(s1 + 8) = *(unsigned char *)(s1 + 8) + *(unsigned char *)(s0 + 21);
    *(float *)(s1 + 12) = *(float *)(s1 + 12) + *(float *)(s0 + 16);
}
extern int F250fbfa4_FUN_00282978(char *p);
extern void F250fbfa4_FUN_002B9180(char *p);
extern void F250fbfa4_FUN_00282CC0(char *p0, char *p1, char *p2);

void FUN_002C87B0(char *a0)
{
    char local[16];
    char *s1 = a0;
    char *s0;
    int a1;
    int v1;
    char *p;

    if (F250fbfa4_FUN_00282978(a0 + 10) != 0) {
        F250fbfa4_FUN_002B9180(a0);
        return;
    }
    s0 = a0 + 32;
    a1 = *(int *)(s1 + 4);
    v1 = (a1 >> 24) + *(unsigned char *)(s0 + 20);
    *(int *)(s1 + 4) = (a1 & 0x00ffffff) | (v1 << 24);
    p = s1 + 16;
    *(float *)(local + 0) = *(float *)(s1 + 32);
    *(float *)(local + 4) = *(float *)(s0 + 4);
    *(float *)(local + 8) = *(float *)(s0 + 8);
    *(int *)(local + 12) = 0;
    F250fbfa4_FUN_00282CC0(p, p, local);
    *(float *)(s0 + 8) = *(float *)(s0 + 8) - *(float *)(s0 + 12);
    *(unsigned char *)(s1 + 8) = *(unsigned char *)(s1 + 8) + *(unsigned char *)(s0 + 21);
    *(float *)(s1 + 12) = *(float *)(s1 + 12) + *(float *)(s0 + 16);
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033BD38(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033C178(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033C430(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033C9B8(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033D440(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033D708(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033DAD0(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033DD80(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Ffb202470_FUN_0033AB50(char *p);

char *FUN_0033DF78(char *object)
{
    Ffb202470_FUN_0033AB50(object + 8);
    return object;
}
extern void Fe524d931_FUN_0033AE28(char *p);
extern void Fe524d931_FUN_0033B018(char *p, float x, float y);
extern void Fe524d931_FUN_002E5E58(int a, int b, int c);
extern void Fe524d931_FUN_00284E40(void);
extern short Fe524d931_D_001A6480[];

int FUN_0033BE98(char *object, int mask)
{
    char *sub = object + 8;
    short oldx;
    short oldy;
    int *v;

    Fe524d931_FUN_0033AE28(sub);
    v = *(int **)(object + 684);
    Fe524d931_FUN_0033B018(sub, *(float *)v, *(float *)(v + 1));
    if ((mask & 0x40) != 0 && *(int *)(object + 680) == 0)
        Fe524d931_FUN_002E5E58(4, 0, 0);
    if ((mask & 0xf000) != 0) {
        short *p = Fe524d931_D_001A6480;
        oldx = p[180];
        oldy = p[181];
        if ((mask & 0x1000) != 0) {
            ((unsigned short *)p)[181] = ((unsigned short *)p)[181] - 1;
            if ((short)((unsigned short *)p)[181] < -32)
                p[181] = -32;
        }
        if ((mask & 0x4000) != 0) {
            unsigned short *q = (unsigned short *)Fe524d931_D_001A6480;
            q[181] = q[181] + 1;
            if ((short)q[181] > 32)
                ((short *)q)[181] = 32;
        }
        if ((mask & 0x8000) != 0) {
            unsigned short *q = (unsigned short *)Fe524d931_D_001A6480;
            q[180] = q[180] - 1;
            if ((short)q[180] < -40)
                ((short *)q)[180] = -40;
        }
        if ((mask & 0x2000) != 0) {
            unsigned short *q = (unsigned short *)Fe524d931_D_001A6480;
            q[180] = q[180] + 1;
            if ((short)q[180] > 40)
                ((short *)q)[180] = 40;
        }
        {
            short *q = Fe524d931_D_001A6480;
            if (oldx != q[180] || oldy != q[181])
                Fe524d931_FUN_002E5E58(4, 0, 0);
        }
        Fe524d931_FUN_00284E40();
    }
    return (mask >> 6) & 1;
}
extern void F0b028b34_FUN_00350878(char *a, unsigned int b, int c, int d);
extern void F0b028b34_FUN_00350878(char *a, unsigned int b, int c, int d);
extern void F0b028b34_FUN_00350808(int a);

int FUN_00350918(int *p)
{
    int i;

    p[17] = 1;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[22] = 0;
    p[23] = 0;
    for (i = 0; i < p[21]; i++) {
        *(long long *)((char *)p[20] + i * 24) = -1;
        *(long long *)((char *)p[20] + i * 24 + 8) = -1;
        *(int *)((char *)p[20] + i * 24 + 16) = 0;
        *(int *)((char *)p[20] + i * 24 + 20) = 0;
    }
    for (i = 0; i < p[2]; i++) {
        F0b028b34_FUN_00350878((char *)(p[1] + i * 16), (i * 2048 + p[0]) & 0x0FFFFFFF, 3, 128);
    }
    F0b028b34_FUN_00350878((char *)(p[1] + i * 16), p[1] & 0x0FFFFFFF, 2, 0);
    *(volatile unsigned int *)0x1000B420 = 0;
    *(volatile unsigned int *)0x1000B410 = p[0] & 0x0FFFFFFF;
    *(volatile unsigned int *)0x1000B430 = p[1] & 0x0FFFFFFF;
    F0b028b34_FUN_00350808(5);
    return 1;
}
extern void Fbdd4a4aa_FUN_00300190(char *a, char *b);
extern void Fbdd4a4aa_FUN_00282CC0(char *a, char *b, char *c);
extern void Fbdd4a4aa_FUN_00282D30(char *a, char *b, float f);
extern void Fbdd4a4aa_FUN_002FE8D8(char *a, char *b);
extern void Fbdd4a4aa_FUN_00282D30(char *a, char *b, float f);
extern void Fbdd4a4aa_FUN_00282CC0(char *a, char *b, char *c);
extern void Fbdd4a4aa_FUN_00282D30(char *a, char *b, float f);
extern void Fbdd4a4aa_FUN_00282CC0(char *a, char *b, char *c);
extern int Fbdd4a4aa_FUN_00283D38(unsigned int a, int b, float f);
extern void Fbdd4a4aa_FUN_002B9180(char *a);

void FUN_002C8F40(int *obj)
{
    char *a = (char *)obj + 16;
    char *b = (char *)obj + 32;
    char tmp[16];

    Fbdd4a4aa_FUN_00300190(a, a);
    *(int *)(b + 16) = *(int *)(b + 16) - 1;
    if (*(int *)(b + 16) < 0)
        *(int *)(b + 16) = 0;
    *(unsigned char *)((char *)obj + 8) = *(unsigned char *)((char *)obj + 8) + 1;
    Fbdd4a4aa_FUN_00282CC0(a, b, a);
    *(float *)((char *)obj + 12) = *(float *)((char *)obj + 12) + 1.5750000000000000000000e+03f;
    Fbdd4a4aa_FUN_00282D30(b, b, 9.9000000953674316406250e-01f);
    Fbdd4a4aa_FUN_002FE8D8(a, tmp);
    if ((*(int *)((char *)obj + 4) & 0xFF) < 32) {
        Fbdd4a4aa_FUN_00282D30(tmp, tmp, 1.5000000130385160446167e-03f);
        Fbdd4a4aa_FUN_00282CC0(b, b, tmp);
    } else {
        Fbdd4a4aa_FUN_00282D30(tmp, tmp, -7.5000000651925802230835e-04f);
        Fbdd4a4aa_FUN_00282CC0(b, b, tmp);
    }
    *(int *)((char *)obj + 4) = Fbdd4a4aa_FUN_00283D38(*(unsigned int *)((char *)obj + 4) & 0x00FFFFFF, 0, 2.5000000372529029846191e-02f)
        | (*(int *)(b + 16) << 24);
    if (*(int *)(b + 16) == 0)
        Fbdd4a4aa_FUN_002B9180((char *)obj);
}
#ifndef RAC2_T_OBJ_F7B2F1854
#define RAC2_T_OBJ_F7B2F1854
typedef struct Obj_F7b2f1854 {
    int *f0;
    int *f4;
    int *f8;
    int *f12;
    int *f16;
    int f20;
    int f24;
    int f28;
    int f32;
    int f36;
    int f40;
    int f44;
} Obj_F7b2f1854;
#endif


extern int *F7b2f1854_FUN_00336E40(int n);
extern int *F7b2f1854_FUN_00336DA8(int size, int *p);
extern void F7b2f1854_FUN_00335E68(Obj_F7b2f1854 *o, int v);

void FUN_00335FC8(Obj_F7b2f1854 *o, int a1, int count)
{
    int *p;
    int n;

    o->f44 = count;
    if (count != 0) {
        p = F7b2f1854_FUN_00336DA8(16, F7b2f1854_FUN_00336E40(count));
        n = o->f44;
        o->f0 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = F7b2f1854_FUN_00336DA8(16, F7b2f1854_FUN_00336E40(n));
        n = o->f44;
        o->f8 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = F7b2f1854_FUN_00336DA8(16, F7b2f1854_FUN_00336E40(n));
        n = o->f44;
        o->f4 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = F7b2f1854_FUN_00336DA8(16, F7b2f1854_FUN_00336E40(n));
        n = o->f44;
        o->f12 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;

        p = F7b2f1854_FUN_00336DA8(16, F7b2f1854_FUN_00336E40(n));
        o->f16 = p;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[0] = 0;
    }
    o->f40 = a1;
    o->f32 = 0;
    o->f20 = 0;
    o->f28 = 0;
    o->f24 = 0;
    o->f36 = 0;
    F7b2f1854_FUN_00335E68(o, 1);
}
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_003366D0(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);

char *FUN_003439A0(char *p)
{
    F449e2f67_FUN_003361A8(p);
    F449e2f67_FUN_003361A8(p + 76);
    F449e2f67_FUN_003361A8(p + 152);
    F449e2f67_FUN_003361A8(p + 228);
    F449e2f67_FUN_003361A8(p + 304);
    F449e2f67_FUN_003361A8(p + 380);
    F449e2f67_FUN_00336968(p + 456);
    F449e2f67_FUN_00336968(p + 528);
    F449e2f67_FUN_003366D0(p + 600);
    F449e2f67_FUN_00336968(p + 664);
    F449e2f67_FUN_00336968(p + 736);
    F449e2f67_FUN_00336968(p + 808);
    F449e2f67_FUN_00336968(p + 880);
    F449e2f67_FUN_00336968(p + 952);
    return p;
}
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_003361A8(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_003366D0(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);
extern void F449e2f67_FUN_00336968(char *p);

char *FUN_00345400(char *p)
{
    F449e2f67_FUN_003361A8(p);
    F449e2f67_FUN_003361A8(p + 76);
    F449e2f67_FUN_003361A8(p + 152);
    F449e2f67_FUN_003361A8(p + 228);
    F449e2f67_FUN_003361A8(p + 304);
    F449e2f67_FUN_003361A8(p + 380);
    F449e2f67_FUN_00336968(p + 456);
    F449e2f67_FUN_00336968(p + 528);
    F449e2f67_FUN_003366D0(p + 600);
    F449e2f67_FUN_00336968(p + 664);
    F449e2f67_FUN_00336968(p + 736);
    F449e2f67_FUN_00336968(p + 808);
    F449e2f67_FUN_00336968(p + 880);
    F449e2f67_FUN_00336968(p + 952);
    return p;
}
extern char *Ffc961fca_FUN_002AD0B0(char *a1);
extern void Ffc961fca_FUN_00283410(char *dst, char *src);
extern void Ffc961fca_FUN_00282CC0(char *dst, char *src, char *tail);
extern void Ffc961fca_FUN_00282CF0(char *dst, char *src, char *tail);
extern void Ffc961fca_FUN_00283410(char *dst, char *src);
extern void Ffc961fca_FUN_00283698(char *dst, char *src);
extern void Ffc961fca_FUN_00283098(char *dst, char *src, char *tail);
extern void Ffc961fca_FUN_00283098(char *dst, char *src, char *tail);
extern void Ffc961fca_FUN_00283098(char *dst, char *src, char *tail);
extern void Ffc961fca_FUN_00282CC0(char *dst, char *src, char *tail);
extern void Ffc961fca_FUN_00283738(char *dst, char *src, char *tail);

void FUN_00310D78(char *o0, char *o1)
{
    char b0[64];
    char b1[64];
    char b2[64];
    char *r = Ffc961fca_FUN_002AD0B0(o1);

    if (r == 0)
        return;
    Ffc961fca_FUN_00283410(b0, r);
    Ffc961fca_FUN_00282CC0(o0 + 16, o0 + 16, r + 16);
    Ffc961fca_FUN_00282CF0(o0 + 16, o0 + 16, o1 + 16);
    if ((*(int *)(r + 60) & 2) != 0) {
        Ffc961fca_FUN_00283410(b2, r + 32);
        Ffc961fca_FUN_00283698(b1, b2);
        Ffc961fca_FUN_00283098(o0 + 16, o0 + 16, b1);
        Ffc961fca_FUN_00283098(o0 + 16, o0 + 16, o1 + 192);
    } else {
        Ffc961fca_FUN_00283098(o0 + 16, o0 + 16, b0);
    }
    Ffc961fca_FUN_00282CC0(o0 + 16, o0 + 16, o1 + 16);
    Ffc961fca_FUN_00283738(o0 + 192, b0, o0 + 192);
}
extern int Fd1c348f5_FUN_00282978(char *p);
extern int Fd1c348f5_FUN_00283CF0(float v);
extern int Fd1c348f5_FUN_00282978(char *p);
extern void Fd1c348f5_FUN_002B9180(char *p);
extern int Fd1c348f5_FUN_00283CF0(float v);

void FUN_002BC8E8(char *p)
{
    char *q = p + 32;
    int r;
    int v;
    float f;

    *(unsigned char *)(p + 8) = *(unsigned char *)(p + 8) + *(unsigned char *)(q + 8);
    *(float *)(p + 24) = *(float *)(p + 24) + *(float *)(q + 16);
    *(float *)(p + 12) = *(float *)(p + 12) * *(float *)(q + 12);
    if (*(int *)(q + 4) == 0) {
        r = Fd1c348f5_FUN_00282978(p + 10);
        if (r != 0) {
            *(int *)(q + 4) = 1;
            *(short *)(p + 10) = 30;
            *(int *)(p + 4) = *(int *)(q + 20) | 0x7f000000;
        } else {
            f = (float)(10 - *(short *)(p + 10)) * 9.6000003814697265625000e+00f;
            v = Fd1c348f5_FUN_00283CF0(f);
            *(int *)(p + 4) = ((v + 32) << 24) | *(int *)(q + 20);
        }
    } else {
        r = Fd1c348f5_FUN_00282978(p + 10);
        if (r != 0) {
            Fd1c348f5_FUN_002B9180(p);
        } else {
            f = (float)*(short *)(p + 10) * 4.2333333492279052734375e+00f;
            v = Fd1c348f5_FUN_00283CF0(f);
            *(int *)(p + 4) = (v << 24) | *(int *)(q + 20);
        }
    }
}
extern void F2d5993bb_FUN_00282A60(char *p, int a1, int a2);
extern void F2d5993bb_FUN_00282AB0(char *p, int a1, int a2);
extern int F2d5993bb_FUN_0029AEB0(char *p, int a1);

int FUN_0029AF98(char *out, int mult, int *recs)
{
    int off = 0;
    char *p = out + 8;
    char *r;
    int v;
    int w;

    if (*(int *)recs != 0) {
        r = (char *)recs;
        do {
            off += 8;
            v = *(int *)(r + 0) + mult * *(int *)(r + 4);
            *(int *)(p + 0) = *(int *)(r + 8);
            *(int *)(p + 4) = *(int *)(r + 4);
            p += 8;
            if (*(int *)(r + 8) == 6000)
                F2d5993bb_FUN_00282A60(p, 0, *(int *)(r + 4));
            else
                F2d5993bb_FUN_00282AB0(p, v, *(int *)(r + 4));
            w = *(int *)(r + 4);
            r += 16;
            p += w;
            off += w;
            p = (char *)(((int)p + 3) & ~3);
            off = (off + 3) & ~3;
        } while (*(int *)r != 0);
    }
    off += 8;
    *(int *)(p + 4) = 0;
    *(int *)(p + 0) = -1;
    v = F2d5993bb_FUN_0029AEB0(out + 8, off);
    *(int *)(out + 4) = v;
    *(int *)(out + 0) = off;
    return off + 8;
}
/* Family 0e7bb6a8908d30dc -- 244 bytes, 28 placements (1 boot + 27 levels). */

extern char *F0e7bb6a8_FUN_002AD0B0(char *a);
extern void F0e7bb6a8_FUN_00283410(char *dst, char *src);
extern void F0e7bb6a8_FUN_00283698(char *dst, char *src);
extern void F0e7bb6a8_FUN_00283698(char *dst, char *src);
extern void F0e7bb6a8_FUN_00282CF0(char *dst, char *a, char *b);
extern void F0e7bb6a8_FUN_002830C0(char *a, char *b, char *c);
extern void F0e7bb6a8_FUN_00283410(char *dst, char *src);
extern void F0e7bb6a8_FUN_00283788(char *a, char *b, char *c);
extern void F0e7bb6a8_FUN_002AB6D8(char *a, char *b);

int FUN_002AD5F8(char *unused, char *obj, char *arg2, char *arg3, char *arg4, char *arg5)
{
    char buf0[64];
    char buf1[16];
    char buf2[64];
    char *p;

    p = F0e7bb6a8_FUN_002AD0B0(obj);
    if (p == 0)
        return 0;
    if (*(int *)(p + 60) & 0x40) {
        F0e7bb6a8_FUN_00283410(buf0, obj + 240);
        F0e7bb6a8_FUN_00283698(buf0, buf0);
    } else {
        F0e7bb6a8_FUN_00283698(buf0, obj + 192);
    }
    F0e7bb6a8_FUN_00282CF0(buf1, arg2, obj + 16);
    F0e7bb6a8_FUN_002830C0(arg4, buf1, buf0);
    F0e7bb6a8_FUN_00283410(buf2, arg3);
    F0e7bb6a8_FUN_00283788(buf2, buf0, buf2);
    F0e7bb6a8_FUN_002AB6D8(buf2, arg5);
    return 1;
}
extern void Fa2a84657_FUN_00282D48(float *tmp, char *v, float k);
extern void Fa2a84657_FUN_00282CD8(float *tmp, char *a, char *v);
extern int Fa2a84657_FUN_00282978(char *field);
extern void Fa2a84657_FUN_002B9180(unsigned char *p);

void FUN_002C2080(unsigned char *p)
{
    float tmp[4];
    char *v;
    int n;

    v = (char *)p + 32;
    Fa2a84657_FUN_00282D48(tmp, v, 9.4999998807907104492188e-01f);
    *(float *)(v + 8) = *(float *)(v + 8) + 1.3888889225199818611145e-03f;
    Fa2a84657_FUN_00282CD8(tmp, (char *)p + 16, v);

    n = *(int *)(p + 4) + (int)0xFE000000;
    *(int *)(p + 4) = n;
    if (((unsigned)(n & 0xFF000000) - 1) > 0x5EFFFFFFu || Fa2a84657_FUN_00282978((char *)p + 10) != 0) {
        Fa2a84657_FUN_002B9180(p);
    } else {
        float f0;
        float f1;

        f1 = *(float *)(p + 12);
        f0 = 2.1000000000000000000000e+05f - f1;
        f0 = f0 * 7.0000000298023223876953e-02f;
        f1 = f1 + f0;
        *(float *)(p + 12) = f1;
        p[8] = p[8] + 1;
    }
}
extern int F750245c6_D_001A7340 __attribute__((sda));
extern void F750245c6_FUN_0028FC48(int a, int b, int c, int d, char *e, int f);
void FUN_002CF0A0(int a0, char *a1)
{
    int h, k, p, m;
    k = a0 / 2 + 5;
    h = F750245c6_D_001A7340 / 2;
    m = h - k;
    p = h + k;
    F750245c6_FUN_0028FC48(m - 2, 312, p + 4, 314, a1, 0);
    F750245c6_FUN_0028FC48(m - 2, 333, p + 4, 335, a1, 0);
    F750245c6_FUN_0028FC48(m - 2, 313, m, 334, a1, 0);
    F750245c6_FUN_0028FC48(p + 2, 313, p + 4, 334, a1, 0);
}
extern float Fe617c30b_FUN_00283CE0(int a);
extern void Fe617c30b_FUN_00282D30(char *p, char *q, float f);
extern void Fe617c30b_FUN_00282CC0(char *p, char *q, char *r);
extern int Fe617c30b_FUN_002A9568(int a, int b, float f);
extern int Fe617c30b_FUN_00282978(char *p);
extern void Fe617c30b_FUN_002B9180(char *p);

void FUN_002BAF68(char *a0)
{
    char *s0 = a0 + 32;
    int x;
    int u;
    int t;
    float f;

    x = *(int *)(s0 + 28);
    t = *(short *)(a0 + 10);
    u = *(short *)(s0 + 24) * (x - t) / x + *(short *)(s0 + 26);

    f = Fe617c30b_FUN_00283CE0(u) * 1000.0f;
    *(float *)(a0 + 12) = f;
    Fe617c30b_FUN_00282D30(s0, s0, 9.8000001907348632812500e-01f);
    Fe617c30b_FUN_00282CC0(a0 + 16, a0 + 16, s0);
    *(unsigned char *)(a0 + 8) = *(unsigned char *)(a0 + 8) + 1;
    f = Fe617c30b_FUN_00283CE0(*(int *)(s0 + 28));
    *(int *)(a0 + 4) = Fe617c30b_FUN_002A9568(*(int *)(s0 + 20), *(int *)(s0 + 16),
                               (float)*(short *)(a0 + 10) / f);
    if (Fe617c30b_FUN_00282978(a0 + 10) != 0)
        Fe617c30b_FUN_002B9180(a0);
}


extern int F9a90bcc4_FUN_002A77E0(int count);

int FUN_002B1A80(u8 *owner, int b, int *outIndex,
                                       float *o0, float *o1, float *o2, float *o3,
                                       float *o4, float *o5, float *o6)
{
    int n;
    int count;
    int i;
    int k;
    int left;
    u8 *p;

    n = 0;
    count = *(u8 *)(*(int *)(owner + 36) + 12);
    if (count != 0) {
        int *t = (int *)(*(int *)(owner + 36) + 72);
        u8 *q;
        left = count;
        do {
            q = *(u8 **)(*t + 20);
            if (q != 0 && *q == b)
                n++;
            t++;
            left--;
        } while (left != 0);
    }

    if (n != 0)
        goto second;

    return 0;

found:
    *o0 = *(float *)(p + 8) * 0.016666668f;
    *o1 = *(float *)(p + 12) * 0.016666668f;
    *o2 = *(float *)(p + 16) * 0.00027777778f;
    *o3 = *(float *)(p + 20) * 0.00027777778f;
    *o6 = *(float *)(p + 4) * 0.00027777778f;
    *o4 = *(float *)(p + 24);
    *o5 = *(float *)(p + 28);
    *outIndex = i;
    return 1;

second:
    k = F9a90bcc4_FUN_002A77E0(n);
    for (i = 0; i < *(u8 *)(*(int *)(owner + 36) + 12); i++) {
        char *tbl = (char *)(*(int *)(owner + 36) + 72);
        char *e = *(char **)(tbl + i * 4);
        p = *(u8 **)(e + 20);
        if (p == 0 || *p != b)
            continue;
        if (k == 0)
            goto found;
        k--;
    }
    return 0;
}
