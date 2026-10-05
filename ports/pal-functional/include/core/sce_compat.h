#ifndef SCE_COMPAT_H
#define SCE_COMPAT_H

/* Stub: syscall 100 – flush de caché (ya tienes) */
int  sceFlushCache(int mode, void* addr, int size);

/* Stub: syscall 116 – kick/notificación SIF o DMA */
int  sceSifCheckM_S(void);

/* Stub: syscall 131 – polling de estado (read/write head) */
int  sceSifSetM_S(void);

/* Stub: syscall 64 – sceSemaCreate(SemaParam_t*).
 * Crea un semáforo de kernel. En PS2 devuelve un ID (handle) del kernel;
 * en PC devuelve un ID virtual estable. NUNCA crea recursos SDL aquí. */
int sceSemaCreate(void* param);   /* param = SemaParam_t* (se ignora en PC) */

/* Stub: syscall 90 – espera/señal de event-set o flush de cola del kernel.
 * Vecino de la familia de semáforos/eventos. En PC: no-op. */
int sceSifSetRpcQueue(void);

/* Stub: syscall 91 – signal de event-set (pareja de la 90).
 * [CONFIRM] Confirmar wait/signal al descifrar FUN_0011fa88. */
int sceSifInitRpc(void);

int sceSifCheckM_S(int op_code);   /* $a0 solo */
int sceSifSetRpcQueue(int a0, int a1, int a2);

/* Stub: syscall 75 – GetOsdConfigParam (consulta de config del sistema).
 *   PS2: li v0, 0x4b ; syscall   (devuelve int en $v0)
 *   PC:  no hay kernel PS2; devuelve SYS_CONFIG_DEFAULT.
 * [CONFIRM] Afinar valor y nombre al ver un caller de los 7. */
int GetOsdConfigParam(void);

/* Par get/set de config del sistema (syscalls 74/75).
 *   El valor viaja por un buffer que el caller pasa (no por $v0). */
int GetOsdConfigParam(void* out_buf);        /* syscall 75 */
int SetOsdConfigParam(const void* in_buf);   /* syscall 74 */

/* FUN_0011f8d0: fuerza el bit 13 del registro de config y devuelve
 *   el campo de 3 bits (13-15) == 0 ? 1 : 0.  (modo de video/config)
 *   PC: config=0 → devuelve siempre 1 (modo default). */
int sys_config_init(void);

#endif /* SCE_COMPAT_H */
