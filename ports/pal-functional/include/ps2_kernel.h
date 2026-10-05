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

/* ------------------------------------------------------------------------
 * PS2 kernel threads.
 * ------------------------------------------------------------------------ */
s32 sceWakeupThread(s32 thread_id);
s32 iWakeupThread(s32 thread_id);
s32 sceReferThreadStatus(s32 thread_id, void* status_ptr);
s32 sceSleepThread(void);

/* ------------------------------------------------------------------------
 * Kernel alarms, cache and other utilities.
 * ------------------------------------------------------------------------ */
s32  sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg);

#ifdef __cplusplus
}
#endif

#endif // PS2_KERNEL_H
