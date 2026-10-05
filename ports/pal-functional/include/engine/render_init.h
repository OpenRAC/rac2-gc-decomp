#ifndef RENDER_INIT_H
#define RENDER_INIT_H

/* [CONFIRM] Two binary semaphores (initCount=1, maxCount=1) that the
 *   engine creates when starting the render pipeline (SIF double buffer).
 *   Provisional names until the caller FUN_0011f828 is decoded. */
int g_render_sema_a;   /* DAT_00134e38 */
int g_render_sema_b;   /* DAT_00134e3c */

/* Creates both semaphores (FUN_0011f640). Calls the sceSemaCreate stub
 * with a binary SemaParam_t. */
void render_init_semas(void);

#endif
