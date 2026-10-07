#include "engine/party/party.h"

#ifndef PARTY_PLAYER_H
#define PARTY_PLAYER_H

#include <SDL.h>
#include <stdbool.h>

#define MAX_PLAYERS  4

typedef struct {
	int   id;
	int   controller_id;          /* index into SDL_GameController */
	bool  active;                 /* false if the slot is empty     */
	bool  is_spectator;

	/* World state (shared, but the player's local position) */
	float pos[3];
	float vel[3];
	float rot[3];                 /* yaw, pitch, roll             */
	int   hp;
	int   max_hp;
	int   weapon_id;
	int   ammo;
	int   suit_id;

	/* Camera (owned by this player) */
	float cam_pos[3];
	float cam_rot[3];
	float cam_fov;
	float cam_aspect;             /* recomputed per viewport      */

	/* Viewport (computed by party.c from the layout) */
	int   vp_x, vp_y, vp_w, vp_h;

	/* Input (snapshot of the frame) */
	float stick_lx, stick_ly;
	float stick_rx, stick_ry;
	Uint8 buttons[16];

	/* HUD */
	int   hud_theme;
	SDL_Color color_override;     /* character tint (optional)    */
	bool  has_color_override;

	/* State flags */
	bool  is_paused;
	bool  is_dead;
} PlayerState;

/* Initializes N players from config. Returns 0 OK, -1 error. */
int  player_init_from_config(const char* config_path, int* out_count);
void player_shutdown(void);

/* Updates one player's input (reads SDL_GameController). */
void player_update_input(PlayerState* p);

/* Updates the camera and HUD. */
void player_update_camera(PlayerState* p, float dt);
void player_update_hud(PlayerState* p, float dt);

/* Resets one player's state (respawn). */
void player_reset(PlayerState* p, const float spawn_pos[3]);

extern PlayerState g_players[MAX_PLAYERS];
extern int         g_player_count;

#endif /* PARTY_PLAYER_H */