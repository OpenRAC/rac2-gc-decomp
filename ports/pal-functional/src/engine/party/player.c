#include "engine/party/party_player.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

PlayerState g_players[MAX_PLAYERS];
int         g_player_count = 1;

/* [SIMPLIFIED] Config parser.
 * [MOD-PENDING] For production: use cJSON (single header) or
 * a custom parser. For now 2 players are hard-coded as the default
 * to validate the pipeline. */
int player_init_from_config(const char* config_path, int* out_count)
{
	(void)config_path;

	int n = 2;   /* default: 2 players */

	for (int i = 0; i < n; i++)
	{
		g_players[i].id = i;
		g_players[i].controller_id = i;
		g_players[i].active = true;
		g_players[i].is_spectator = false;
		g_players[i].is_paused = false;
		g_players[i].is_dead = false;
		g_players[i].hp = 100;
		g_players[i].max_hp = 100;
		g_players[i].weapon_id = 0;
		g_players[i].ammo = 99;
		g_players[i].suit_id = 0;
		g_players[i].cam_fov = 60.0f;
		g_players[i].has_color_override = false;
		memset(g_players[i].pos, 0, sizeof(float) * 3);
		memset(g_players[i].vel, 0, sizeof(float) * 3);
		memset(g_players[i].rot, 0, sizeof(float) * 3);
		memset(g_players[i].cam_pos, 0, sizeof(float) * 3);
		memset(g_players[i].cam_rot, 0, sizeof(float) * 3);
		memset(g_players[i].buttons, 0, sizeof(Uint8) * 16);
		g_players[i].stick_lx = 0; g_players[i].stick_ly = 0;
		g_players[i].stick_rx = 0; g_players[i].stick_ry = 0;
	}

	g_player_count = n;
	if (out_count) *out_count = n;
	return 0;
}

void player_shutdown(void)
{
	for (int i = 0; i < g_player_count; i++)
		g_players[i].active = false;
	g_player_count = 1;
}

/* SDL2 has no lookup by device index: open each controller once and keep it. */
static SDL_GameController* controller_for(int index)
{
	static SDL_GameController* opened[MAX_PLAYERS];
	if (index < 0 || index >= MAX_PLAYERS)
		return NULL;
	if (!opened[index] && index < SDL_NumJoysticks() && SDL_IsGameController(index))
		opened[index] = SDL_GameControllerOpen(index);
	return opened[index];
}

void player_update_input(PlayerState* p)
{
	SDL_GameController* gc = controller_for(p->controller_id);
	if (!gc) return;

	p->stick_lx = (float)SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f;
	p->stick_ly = (float)SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f;
	p->stick_rx = (float)SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0f;
	p->stick_ry = (float)SDL_GameControllerGetAxis(gc, SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0f;

	p->buttons[0] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_A) ? 1 : 0;
	p->buttons[1] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_B) ? 1 : 0;
	p->buttons[2] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_X) ? 1 : 0;
	p->buttons[3] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_Y) ? 1 : 0;
	p->buttons[4] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ? 1 : 0;
	p->buttons[5] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ? 1 : 0;
	p->buttons[6] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_LEFTSTICK) ? 1 : 0;
	p->buttons[7] = SDL_GameControllerGetButton(gc, SDL_CONTROLLER_BUTTON_RIGHTSTICK) ? 1 : 0;
}

void player_update_camera(PlayerState* p, float dt)
{
	(void)dt;
	/* [TODO] Derive cam_pos/rot from pos/rot + the camera offset.
	 *   Once the engine's camera function is decoded
	 *   (the one that reads the player state and sets the view
	 *   matrix), call it here per player. */
	p->cam_pos[0] = p->pos[0] + p->rot[0] * 2.0f;
	p->cam_pos[1] = p->pos[1] + 3.0f;
	p->cam_pos[2] = p->pos[2] - 4.0f;
}

void player_update_hud(PlayerState* p, float dt)
{
	(void)p;
	(void)dt;
	/* [TODO] Once the engine's HUD is decoded,
	 *   render one instance per player inside
	 *   its viewport. */
}

void player_reset(PlayerState* p, const float spawn_pos[3])
{
	p->pos[0] = spawn_pos[0];
	p->pos[1] = spawn_pos[1];
	p->pos[2] = spawn_pos[2];
	p->vel[0] = 0; p->vel[1] = 0; p->vel[2] = 0;
	p->hp = p->max_hp;
	p->ammo = 99;
	p->is_dead = false;
}
