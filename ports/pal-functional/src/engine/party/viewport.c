#include "engine/party/viewport.h"
#include "core/render/gl/gl_loader.h"

void viewport_compute(ViewportLayout layout, int n,
	int sw, int sh, ViewportRect* out)
{
	for (int i = 0; i < n; i++)
		out[i] = (ViewportRect){ 0, 0, sw, sh };   /* default: full */

	if (n <= 1) return;

	switch (layout)
	{
	case VIEWPORT_LAYOUT_1X2:   /* 2 players: left / right */
		out[0] = (ViewportRect){ 0,     0, sw / 2, sh };
		out[1] = (ViewportRect){ sw / 2,  0, sw - sw / 2, sh };
		break;

	case VIEWPORT_LAYOUT_2X1:   /* 2 players: top / bottom */
		out[0] = (ViewportRect){ 0, 0,     sw, sh / 2 };
		out[1] = (ViewportRect){ 0, sh / 2,  sw, sh - sh / 2 };
		break;

	case VIEWPORT_LAYOUT_2X2:   /* 4 players: grid */
		out[0] = (ViewportRect){ 0,     0,     sw / 2, sh / 2 };
		out[1] = (ViewportRect){ sw / 2,  0,     sw - sw / 2, sh / 2 };
		out[2] = (ViewportRect){ 0,     sh / 2,  sw / 2, sh - sh / 2 };
		out[3] = (ViewportRect){ sw / 2,  sh / 2,  sw - sw / 2, sh - sh / 2 };
		break;

	default:
		break;
	}
}

void viewport_apply(const ViewportRect* r)
{
	glViewport(r->x, r->y, r->w, r->h);
	glScissor(r->x, r->y, r->w, r->h);
	glEnable(GL_SCISSOR_TEST);
}
