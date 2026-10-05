#ifndef SEMAPHORE_MAP_H
#define SEMAPHORE_MAP_H

/* Maps a virtual ID (from the sceSemaCreate stub) -> real SDL_Semaphore.
 * Filled by Sys_InitGraphicsSemaphore. The rest of the engine calls
 * sema_wait(id) / sema_signal(id) and this table resolves them to SDL. */
int         sema_wait(int virtual_id);   /* SDL_SemWait */
int         sema_signal(int virtual_id); /* SDL_SemPost */
void        sema_register(int virtual_id); /* creates the SDL_Semaphore(0) and associates it */

#endif /* SEMAPHORE_MAP_H */
