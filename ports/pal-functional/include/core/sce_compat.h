#ifndef SCE_COMPAT_H
#define SCE_COMPAT_H

#include <stdint.h>

/* Stub: syscall 100 – cache flush (already present) */
int  sceFlushCache(int mode, void* addr, int size);

/* Stub: syscall 116 – SIF or DMA kick/notification */
int  sceSifCheckM_S(int op_code);   /* $a0 only */

/* Stub: syscall 131 – status polling (read/write head) */
int  sceSifSetM_S(void);

/* Stub: syscall 64 – sceSemaCreate(SemaParam_t*).
 * Creates a kernel semaphore. On PS2 it returns a kernel ID (handle);
 * on PC it returns a stable virtual ID. NEVER creates SDL resources here. */
int sceSemaCreate(void* param);   /* param = SemaParam_t* (ignored on PC) */

/* Stub: syscall 90 – event-set wait/signal or kernel queue flush.
 * Neighbour of the semaphore/event family. On PC: no-op. */
int sceSifSetRpcQueue(int a0, int a1, int a2);

/* Stub: syscall 91 – event-set signal (pair of syscall 90).
 * [CONFIRM] Confirm wait/signal when FUN_0011fa88 is decoded. */
int sceSifInitRpc(int op_code);


/* Stub: syscall 75 – GetOsdConfigParam (system configuration query).
 *   PS2: li v0, 0x4b ; syscall   (returns an int in $v0)
 *   PC:  there is no PS2 kernel; returns SYS_CONFIG_DEFAULT.
 * [CONFIRM] Refine the value and name after reading one of the 7 callers. */
/* System configuration get/set pair (syscalls 74/75).
 *   The value travels through a buffer passed by the caller (not through $v0). */
int GetOsdConfigParam(void* out_buf);        /* syscall 75 */
int SetOsdConfigParam(const void* in_buf);   /* syscall 74 */

/* FUN_0011f8d0: forces bit 13 of the config register and returns
 *   the 3-bit field (13-15) == 0 ? 1 : 0.  (video mode/config)
 *   PC: config=0 → always returns 1 (default mode). */
int sys_config_init(void);

/* Channel B display handshake (DAT_00134b48 holds its final state). */
extern int g_display_channel_b;
void display_init_channel_b(void);

#endif /* SCE_COMPAT_H */
