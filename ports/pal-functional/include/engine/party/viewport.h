#ifndef PARTY_VIEWPORT_H
#define PARTY_VIEWPORT_H

#include <stdbool.h>

typedef enum {
	VIEWPORT_LAYOUT_FULL,     /* 1x1 → 1 player   */
	VIEWPORT_LAYOUT_1X2,      /* horizontal → 2   */
	VIEWPORT_LAYOUT_2X1,      /* vertical → 2     */
	VIEWPORT_LAYOUT_2X2       /* grid → 4         */
} ViewportLayout;

typedef struct {
	int x, y, w, h;
} ViewportRect;

/* Applies the layout and recomputes the rectangles for N players.
 *   layout: FULL, 1X2, 2X1, 2X2
 *   player_count: number of active slots (1-4)
 *   sw, sh: width/height of the SDL window
 *   out: array of ViewportRect (MAX_PLAYERS entries) */
void viewport_compute(ViewportLayout layout, int player_count,
	int sw, int sh, ViewportRect* out);

/* Applies glViewport + glScissor for one rect. */
void viewport_apply(const ViewportRect* r);

#endif /* PARTY_VIEWPORT_H */
