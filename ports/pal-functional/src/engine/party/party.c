#include "engine/party/party.h"
#include "engine/party/party_player.h"
#include "engine/party/viewport.h"
#include "core/render/gl/gl_loader.h"
#include <stdio.h>
#include <string.h>

static ViewportLayout g_layout = VIEWPORT_LAYOUT_1X2;
static ViewportRect   g_viewports[MAX_PLAYERS];
static bool           g_party_active = false;

bool party_init(const char* config_path)
{
	if (player_init_from_config(config_path, NULL) != 0)
		return false;
	g_party_active = true;
	return true;
}

void party_shutdown(void)
{
	player_shutdown();
	g_party_active = false;
}

void party_update(float dt)
{
	if (!g_party_active) return;

	for (int i = 0; i < g_player_count; i++)
	{
		if (!g_players[i].active || g_players[i].is_paused)
			continue;
		player_update_input(&g_players[i]);
		player_update_camera(&g_players[i], dt);
		player_update_hud(&g_players[i], dt);
	}
	/* [TODO] Call world_update_physics / world_update_ai
	 *   of the original engine (to be decoded). ONCE, not per
	 *   player. The world is shared. */
}

void party_render(void)
{
	if (!g_party_active) return;

	/* Recompute the viewports with the current layout */
	int sw, sh;
	SDL_GetWindowSize(NULL, &sw, &sh);   /* [TODO] pass the window */
	viewport_compute(g_layout, g_player_count, sw, sh, g_viewports);

	/* [TODO] Full clear (once) */
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Per-player render */
	for (int i = 0; i < g_player_count; i++)
	{
		if (!g_players[i].active) continue;

		viewport_apply(&g_viewports[i]);

		/* [TODO] glClearColor(0.1 + i*0.1, 0.2, 0.3, 1); -- for testing */
		/* [TODO] render_world(g_players[i].cam_pos, g_players[i].cam_rot) */
		/* [TODO] render_hud(&g_players[i]) */

		/* Placeholder: draws a solid colour per viewport
		   to validate the screen split. */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glDisable(GL_SCISSOR_TEST);
}

int party_player_count(void)
{
	return g_player_count;
}

void party_set_layout(const char* layout)
{
	if (strcmp(layout, "full") == 0)      g_layout = VIEWPORT_LAYOUT_FULL;
	else if (strcmp(layout, "1x2") == 0)  g_layout = VIEWPORT_LAYOUT_1X2;
	else if (strcmp(layout, "2x1") == 0)  g_layout = VIEWPORT_LAYOUT_2X1;
	else if (strcmp(layout, "2x2") == 0)  g_layout = VIEWPORT_LAYOUT_2X2;
}
