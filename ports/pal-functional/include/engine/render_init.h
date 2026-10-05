#ifndef RENDER_INIT_H
#define RENDER_INIT_H

/* [CONFIRM] Dos semáforos binarios (initCount=1, maxCount=1) que el
 *   motor crea al arrancar el pipeline de render (doble buffer SIF).
 *   Nombres provisionales hasta descifrar el caller FUN_0011f828. */
int g_render_sema_a;   /* DAT_00134e38 */
int g_render_sema_b;   /* DAT_00134e3c */

/* Crea ambos semáforos (FUN_0011f640). Llama al stub sceSemaCreate
 * con un SemaParam_t binario. */
void render_init_semas(void);

#endif
