#include "engine/render_init.h"
#include "core/sce_compat.h"

/* Parameter structure of sceSemaCreate (libkernel).
 * Fields per the PS2 SDK: initCount, maxCount, flags. */
typedef struct {
	int initCount;   /* 1: starts released      */
	int maxCount;    /* 1: binary               */
	int flags;       /* 1: binary / attributes  */
} SemaParam_t;

void render_init_semas(void)
{
	SemaParam_t sp = { 1, 1, 1 };   /* binary semaphore, starts signalled */

	g_render_sema_a = sceSemaCreate(&sp);   /* -> DAT_00134e38 */
	g_render_sema_b = sceSemaCreate(&sp);   /* -> DAT_00134e3c */

	/* Register them in the ID -> SDL_Semaphore table so that
	   sema_wait/signal resolve the virtual handle to a real SDL one. */
	   /* (lives in engine/semaphore_map.c) */
	   /* sema_register(g_render_sema_a);
		  sema_register(g_render_sema_b);  */
}
