/* Target: FUN_002A9568 (28 bytes) — wrapper that saves RA on stack, calls FUN_00283D38, restores RA and returns */
typedef unsigned long long u64;
typedef unsigned int u32;

extern u64 FUN_00283D38(u32, u64, u64);

void FUN_002A9568(void) {
  u64 ra;
  ra = 0;
  FUN_00283D38(0, 0, 0);
  ra = 0;
}