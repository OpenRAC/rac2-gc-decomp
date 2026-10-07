// src/ps2_kernel.h
#ifndef PS2_KERNEL_H
#define PS2_KERNEL_H

#include "types.h"
#include <stddef.h> // size_t

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------
 * Low-level Emotion Engine utilities (equivalents of libc functions,
 * reimplemented to make explicit that they replace the native PS2
 * version).
 * ------------------------------------------------------------------------ */
int ee_memcmp(const void* ptr1, const void* ptr2, size_t num);
int ee_atoi(const char* str);

/* ------------------------------------------------------------------------
 * PS2 kernel semaphores (or their SDL2 emulation in the PC port).
 * ------------------------------------------------------------------------ */
s32 sceWaitSema(s32 sema_id);
s32 sceSignalSema(s32 sema_id);
s32 iSignalSema(s32 sema_id);
s32 scePollSema(s32 sema_id);
s32 sceCreateSema(void);
s32 sceDeleteSema(s32 sema_id);

/* ------------------------------------------------------------------------
 * PS2 kernel threads.
 * ------------------------------------------------------------------------ */
s32 sceWakeupThread(s32 thread_id);
s32 iWakeupThread(s32 thread_id);
s32 sceReferThreadStatus(s32 thread_id, void* status_ptr);
s32 sceSleepThread(void);
s32 sceGetThreadId(void);

/* INTC line enable/disable (PS2 syscalls 20/21); no-ops on PC. */
s32 _EnableIntc(s32 intc_id);
s32 _DisableIntc(s32 intc_id);

/* ------------------------------------------------------------------------
 * Emotion Engine CP0 Status register and interrupt/sync instructions. On PC
 * Status stays 0 (no interrupt or diagnostic bit set) and the others are no-ops.
 * ------------------------------------------------------------------------ */
extern u32 Status;
void DI(void);
void EI(void);
void SYNC(int type);

/* ------------------------------------------------------------------------
 * Kernel alarms, cache and other utilities.
 * ------------------------------------------------------------------------ */
s32  sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg);

#ifdef __cplusplus
}
#endif

#endif // PS2_KERNEL_H
