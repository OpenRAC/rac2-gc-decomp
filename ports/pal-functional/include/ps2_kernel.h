// src/ps2_kernel.h
#ifndef PS2_KERNEL_H
#define PS2_KERNEL_H

#include "types.h"
#include <stddef.h> // size_t

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------
 * Utilidades de bajo nivel del Emotion Engine (equivalentes a funciones de
 * libc, reimplementadas para dejar explícito que sustituyen a la versión
 * nativa de la PS2).
 * ------------------------------------------------------------------------ */
int ee_memcmp(const void* ptr1, const void* ptr2, size_t num);
int ee_atoi(const char* str);

/* ------------------------------------------------------------------------
 * Semáforos del Kernel de la PS2 (o su emulación con SDL2 en el port a PC).
 * ------------------------------------------------------------------------ */
s32 sceWaitSema(s32 sema_id);
s32 sceSignalSema(s32 sema_id);
s32 iSignalSema(s32 sema_id);
s32 scePollSema(s32 sema_id);

/* ------------------------------------------------------------------------
 * Hilos del Kernel de la PS2.
 * ------------------------------------------------------------------------ */
s32 sceWakeupThread(s32 thread_id);
s32 iWakeupThread(s32 thread_id);
s32 sceReferThreadStatus(s32 thread_id, void* status_ptr);
s32 sceSleepThread(void);

/* ------------------------------------------------------------------------
 * Alarmas, caché y otras utilidades del Kernel.
 * ------------------------------------------------------------------------ */
s32  sceSetAlarm(u32 microseconds, void* alarm_callback, void* callback_arg);

#ifdef __cplusplus
}
#endif

#endif // PS2_KERNEL_H
