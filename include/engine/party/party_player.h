#include "engine/party/party.h"

#ifndef PARTY_PLAYER_H
#define PARTY_PLAYER_H

#include <SDL.h>
#include <stdbool.h>

#define MAX_PLAYERS  4

typedef struct {
	int   id;
	int   controller_id;          /* index en SDL_GameController */
	bool  active;                 /* false si el slot está vacío  */
	bool  is_spectator;

	/* Estado de mundo (compartido, pero posición local del jugador) */
	float pos[3];
	float vel[3];
	float rot[3];                 /* yaw, pitch, roll             */
	int   hp;
	int   max_hp;
	int   weapon_id;
	int   ammo;
	int   suit_id;

	/* Cámara (propia de este jugador) */
	float cam_pos[3];
	float cam_rot[3];
	float cam_fov;
	float cam_aspect;             /* recalculado por viewport     */

	/* Viewport (calculado por party.c según layout) */
	int   vp_x, vp_y, vp_w, vp_h;

	/* Input (snapshot del frame) */
	float stick_lx, stick_ly;
	float stick_rx, stick_ry;
	Uint8 buttons[16];

	/* HUD */
	int   hud_theme;
	SDL_Color color_override;     /* tinte de personaje (opcional)*/
	bool  has_color_override;

	/* Flags de estado */
	bool  is_paused;
	bool  is_dead;
} PlayerState;

/* Inicializa N jugadores desde config. Devuelve 0 OK, -1 error. */
int  player_init_from_config(const char* config_path, int* out_count);
void player_shutdown(void);

/* Actualiza input de un jugador (lee SDL_GameController). */
void player_update_input(PlayerState* p);

/* Actualiza cámara y HUD. */
void player_update_camera(PlayerState* p, float dt);
void player_update_hud(PlayerState* p, float dt);

/* Reinicia el estado de un jugador (respawn). */
void player_reset(PlayerState* p, const float spawn_pos[3]);

extern PlayerState g_players[MAX_PLAYERS];
extern int         g_player_count;

#endif /* PARTY_PLAYER_H */