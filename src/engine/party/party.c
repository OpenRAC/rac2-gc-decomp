#include "engine/party/party.h"
#include "engine/party/player.h"
#include "engine/party/viewport.h"
#include <GL/gl.h>
#include <stdio.h>

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
	/* [TODO] Llamar a world_update_physics / world_update_ai
	 *   del motor original (por descifrar). UNA vez, no por
	 *   jugador. El mundo es compartido. */
}

void party_render(void)
{
	if (!g_party_active) return;

	/* Recalcular viewports con el layout actual */
	int sw, sh;
	SDL_GetWindowSize(NULL, &sw, &sh);   /* [TODO] pasar la ventana */
	viewport_compute(g_layout, g_player_count, sw, sh, g_viewports);

	/* [TODO] Clear completo (una vez) */
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Render por jugador */
	for (int i = 0; i < g_player_count; i++)
	{
		if (!g_players[i].active) continue;

		viewport_apply(&g_viewports[i]);

		/* [TODO] glClearColor(0.1 + i*0.1, 0.2, 0.3, 1); -- para test */
		/* [TODO] render_world(g_players[i].cam_pos, g_players[i].cam_rot) */
		/* [TODO] render_hud(&g_players[i]) */

		/* Placeholder: dibuja un color sólido por viewport
		   para validar la división de pantalla. */
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
