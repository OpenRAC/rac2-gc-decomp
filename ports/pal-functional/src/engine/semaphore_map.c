#include <SDL.h>
#include "engine/semaphore_map.h"

#define SEMA_MAX 64
static SDL_sem* sema_table[SEMA_MAX] = { 0 };

void sema_register(int id)
{
	if (id >= 0 && id < SEMA_MAX && !sema_table[id])
		sema_table[id] = SDL_CreateSemaphore(0);
}

int sema_wait(int id) { return (id > 0 && id < SEMA_MAX && sema_table[id]) ? SDL_SemWait(sema_table[id]) : 0; }
int sema_signal(int id) { return (id > 0 && id < SEMA_MAX && sema_table[id]) ? SDL_SemPost(sema_table[id]) : 0; }
