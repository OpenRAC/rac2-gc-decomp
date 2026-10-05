#include "engine/render_init.h"
#include "core/sce_compat.h"

/* Estructura del parámetro de sceSemaCreate (libkernel).
 * Campos según SDK PS2: initCount, maxCount, flags. */
typedef struct {
	int initCount;   /* 1: inicia liberado      */
	int maxCount;    /* 1: binario              */
	int flags;       /* 1: binario / atributos  */
} SemaParam_t;

void render_init_semas(void)
{
	SemaParam_t sp = { 1, 1, 1 };   /* semáforo binario, inicia señalizado */

	g_render_sema_a = sceSemaCreate(&sp);   /* -> DAT_00134e38 */
	g_render_sema_b = sceSemaCreate(&sp);   /* -> DAT_00134e3c */

	/* Regístralos en la tabla ID -> SDL_Semaphore para que
	   sema_wait/signal resuelvan el handle virtual a un SDL real. */
	   /* (va en engine/semaphore_map.c, ya propuesto antes) */
	   /* sema_register(g_render_sema_a);
		  sema_register(g_render_sema_b);  */
}
