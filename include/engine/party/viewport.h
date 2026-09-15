#ifndef PARTY_VIEWPORT_H
#define PARTY_VIEWPORT_H

#include <stdbool.h>

typedef enum {
	VIEWPORT_LAYOUT_FULL,     /* 1x1 → 1 player   */
	VIEWPORT_LAYOUT_1X2,      /* horizontal → 2   */
	VIEWPORT_LAYOUT_2X1,      /* vertical → 2     */
	VIEWPORT_LAYOUT_2X2       /* cuadrícula → 4   */
} ViewportLayout;

typedef struct {
	int x, y, w, h;
} ViewportRect;

/* Aplica el layout y recalcula los rectángulos para N players.
 *   layout: FULL, 1X2, 2X1, 2X2
 *   player_count: cuántos slots activos (1-4)
 *   sw, sh: ancho/alto de la ventana SDL
 *   out: array de ViewportRect (tamaño MAX_PLAYERS) */
void viewport_compute(ViewportLayout layout, int player_count,
	int sw, int sh, ViewportRect* out);

/* Aplica glViewport + glScissor para un rect. */
void viewport_apply(const ViewportRect* r);

#endif /* PARTY_VIEWPORT_H */
