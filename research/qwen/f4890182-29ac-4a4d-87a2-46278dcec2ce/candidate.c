typedef int s32;
typedef unsigned char u8;

extern s32 FUN_0027F128(u8 *, s32, u8 *);

void FUN_0027F1B8(void *a0, void *a1)

{
  volatile u8 *p = (u8 *)0x264290;
  FUN_0027F128(a0, (s32)a1, p);
}