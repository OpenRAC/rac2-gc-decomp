#ifndef SEMAPHORE_MAP_H
#define SEMAPHORE_MAP_H

/* Mapeo ID virtual (del stub sceSemaCreate) -> SDL_Semaphore real.
 * Lo rellena Sys_InitGraphicsSemaphore. El resto del motor llama a
 * sema_wait(id) / sema_signal(id) y esta tabla resuelve a SDL. */
int         sema_wait(int virtual_id);   /* SDL_SemaphoreWait */
int         sema_signal(int virtual_id); /* SDL_SemaphorePost */
void        sema_register(int virtual_id); /* crea el SDL_Semaphore(0) y lo asocia */

#endif /* SEMAPHORE_MAP_H */
