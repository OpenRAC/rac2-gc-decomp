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

typedef struct { int a, b, c, d; } __attribute__((aligned(16))) Quad16;

void FUN_002A8C00(char *a, int b, int c, float d, Quad16 *q) {
    *(int *)(a + 0x10) = b;
    *(int *)(a + 0x14) = c;
    *(float *)(a + 0x1C) = d;
    *(int *)(a + 0x20) = 1;
    *(volatile Quad16 *)a = *q;
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

/* LOT 8 OCTETS -- getters et setters nus, identiques dans les 27 niveaux. */
/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
f32 FUN_00282C48(f32 a0) { return __builtin_fabsf(a0); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
f32 FUN_002A7790(f32 a0) { return a0 * a0; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00335E10(u8 *a0) { return *(s32 *)(a0 + 0x0); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00335E18(u8 *a0) { return *(s32 *)(a0 + 0x4); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00335E20(u8 *a0) { return *(s32 *)(a0 + 0xc); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00336318(u8 *a0) { return *(s32 *)(a0 + 0x38); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_003367B8(u8 *a0) { return *(s32 *)(a0 + 0x34); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00336CC0(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x38) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00339790(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x2f8) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0033A9C8(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x204) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0033A9F8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x20c) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0033AE18(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x290) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0033AE20(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x294) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0033B0A8(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x274) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00341550(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x220) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00341CD8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x8) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_003423A8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x170) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00343038(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x4) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00343058(u8 *a0) { *(s32 *)(a0 + 0x90) = 0; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00343060(u8 *a0) { return *(s32 *)(a0 + 0x10); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_003435A0(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x330) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_003435A8(u8 *a0) { return *(s32 *)(a0 + 0x330); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00348118(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x50) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00348120(u8 *a0, s32 a1) { *(s32 *)(a0 + 0xbc) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00348128(u8 *a0) { return *(s32 *)(a0 + 0x50); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_00348130(u8 *a0) { return *(s32 *)(a0 + 0xb0); }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00348570(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x448) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00348578(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x44c) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00349490(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x144) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00349590(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x2c) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_003495C0(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x80) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00349678(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x24) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00349B18(u8 *a0, f32 a1) { *(f32 *)(a0 + 0x18) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0034A4A8(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x0) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_0034CE98(u8 *a0, s32 a1) { *(s32 *)(a0 + 0x2bc) = a1; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
void FUN_00351888(u8 *a0) { *(s32 *)(a0 + 0xa8) = 0; }

/* Cible mesuree : identique a l'octet dans les 27 niveaux. */
s32 FUN_003518D8(u8 *a0) { return *(s32 *)(a0 + 0xa8); }

/* DEUXIEME LOT -- 16 a 32 octets. */
/* conversion entier vers flottant ; identique a l'octet dans les 27 niveaux. */
f32 FUN_00283CE0(s32 a0) { return (f32)a0; }

/* lit un flottant par double indirection, rend son entier ; identique a l'octet dans les 27 niveaux. */
s32 FUN_00336950(u8 *a0) { return (s32)*(f32 *)(*(u8 **)(a0 + 0x34)); }

/* ecrit la valeur si elle est plus petite que le maximum ; identique a l'octet dans les 27 niveaux. */
void FUN_0033A9E0(u8 *a0, s32 a1) { if (a1 < *(s32 *)(a0 + 0x1F8)) *(s32 *)(a0 + 0x208) = a1; }

/* index multiplie par 20, ajoute a une base ; identique a l'octet dans les 27 niveaux. */
s32 FUN_003423B0(u8 *a0) { return *(s32 *)(a0 + 0x250) + *(s32 *)(a0 + 0x16C) * 20; }

/* vrai si le compteur a atteint 0x1000 ; identique a l'octet dans les 27 niveaux. */
s32 FUN_0034FAE8(u8 *a0) { return *(s32 *)(a0 + 0x50) >= 0x1000; }

/* range deux mots, rend 1 ; identique a l'octet dans les 27 niveaux. */
s32 FUN_00350698(u8 *a0, s32 a1, s32 a2) { *(s32 *)(a0 + 0x4) = a1; *(s32 *)a0 = a2; return 1; }

/* range trois mots consecutifs ; identique a l'octet dans les 27 niveaux. */
void FUN_00341540(u8 *a0, s32 a1, s32 a2, s32 a3) { *(s32 *)(a0 + 0x2F0) = a1; *(s32 *)(a0 + 0x2F4) = a2; *(s32 *)(a0 + 0x2F8) = a3; }

/* lot6 -- corps mesures, tailles posees au catalogue. */
void FUN_0033A9D0(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x1B8) = a2; }

void FUN_00348630(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x418) = a2; }

void FUN_00349A60(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x20) = a2; }

void FUN_00349AA8(u8 *a0, s32 a1, s32 a2) { a0 += a1 * 4; *(s32 *)(a0 + 0x8C) = a2; }


/* lot7 -- corps mesures, tailles posees au catalogue. */
extern Quad16 D_00189EA0;
void FUN_00351E48(u8 *a0) {
    volatile s32 *p = (volatile s32 *)a0;
    p[3] = 0;
    p[2] = 0;
}


/* lot11 -- corps mesures, tailles posees au catalogue. */


void FUN_00343028(u8 *a0, f32 a1, f32 a2) { *(f32 *)(a0 + 0x8) = a1; *(f32 *)(a0 + 0xc) = a2; }

void FUN_003480D8(u8 *a0, s32 a1, s32 a2) { *(s32 *)(a0 + 0xa8) = a1; *(s32 *)(a0 + 0xac) = a2; }

s32 FUN_003495C8(u8 *a0, s32 a1) { s32 vieux = *(s32 *)(a0 + 0x28); *(s32 *)(a0 + 0x28) = a1; return vieux; }

s32 FUN_003518E0(u8 *a0, s32 a1) { s32 vieux = *(s32 *)(a0 + 0xa8); *(s32 *)(a0 + 0xa8) = a1; return vieux; }

s32 FUN_00351F28(u8 *a0) { return *(u32 *)(a0 + 0xc) < 1; }

void FUN_003363C0(u8 *a0, f32 a1) { f32 *p = *(f32 **)(a0 + 0x38); *p = a1; }

extern u8 D_00139648;
s32 FUN_002B0D60(void) { return D_00139648; }

/* lot12 -- corps mesures, tailles posees au catalogue. */
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





/* lot13 -- corps mesures, tailles posees au catalogue. */
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


/* lot14 -- corps mesures, tailles posees au catalogue. */
extern u8 D_0018C0B0[];
void FUN_002AB650(u8 *a0) {
    u8 *p = *(u8 **)D_0018C0B0;
    *(long *)(a0 + 0x38) = *(long *)(p + 0x38);
}

