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

/* lot15 -- premier corps qui APPELLE : la famille des appels s'ouvre. */
extern void FUN_0011AAD0(s32);
void FUN_0034F3B8(void) {
    FUN_0011AAD0(1);
}

/* lot15 -- deux jumeaux du premier corps qui appelle (meme forme, 32 octets).
   Le creneau de delai est un `nop` : l'appel n'a AUCUN argument, et le corps rend 1. */
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

/* La campagne : les corps qui appellent, reproduits par la chaine
   reconstruite (2.9-ee + gas raffine) -- sauvegardes sd en creneaux de 8,
   pas de sibcall, le prototype qui rend s32 decide v0 vs v1, chaines ||
   a sortie unique. Mesures et preuves : docs/COMPILER-NOTES.md. */

/* FUN_002A77E0 : tirage aleatoire borne par l'argument */
extern s32 FUN_001163B0(void);
s32 FUN_002A77E0(s32 a0) {
    return (FUN_001163B0() >> 16 & 0x7fff) % a0;
}

/* FUN_002A7940 : tirage d'un angle : 12 bits ramenes autour de zero, puis en radians */
extern s32 FUN_001163B0(void);
f32 FUN_002A7940(void) {
    return (f32)((FUN_001163B0() >> 16 & 0xfff) - 0x800) * 0.0015339808f;
}

/* FUN_002A7820 : tirage aleatoire dans un intervalle ferme */
extern s32 FUN_001163B0(void);
s32 FUN_002A7820(s32 a0, s32 a1) {
    return (FUN_001163B0() >> 16 & 0x7fff) % ((a1 - a0) + 1) + a0;
}

/* FUN_002889B8 : remet trois champs a zero, initialise un bloc, puis arme deux drapeaux */
extern s32 FUN_00115484(u8 *, s32, s32);
void FUN_002889B8(u8 *a0) {
    *(s32 *)(a0 + 0x30) = 0;
    *(s32 *)(a0 + 0x34) = 0;
    *(s32 *)(a0 + 0x38) = 0;
    FUN_00115484(a0 + 8, 0xcd, 0x28);
    *(s32 *)(a0 + 0x44) = 0;
    *(s32 *)(a0 + 0x40) = 1;
}

/* FUN_003512B8 : arrondit un champ au multiple de 2048, sous sema */
extern s32 FUN_0011AC60(s32);
extern s32 FUN_0011AC40(s32);
void FUN_003512B8(u8 *a0) {
    FUN_0011AC60(*(s32 *)(a0 + 0x40));
    *(s32 *)(a0 + 0x14) = (*(s32 *)(a0 + 0x14) + 0x7ff) / 0x800 * 0x800;
    FUN_0011AC40(*(s32 *)(a0 + 0x40));
}

/* FUN_00300540 : aiguillage : categorie d'arme selon l'identifiant */
s32 FUN_00300540(s32 a0) {
    s32 v = 0;
    if (a0 == 0 || a0 == 14 || a0 == 13) v = 2;
    else if (a0 == 1 || a0 == 3 || a0 == 11 || a0 == 12) v = 1;
    return v;
}

/* FUN_00351268 : attend puis combine deux champs decales de 11 */
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
