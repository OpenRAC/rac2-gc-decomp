#ifndef PARTY_H
#define PARTY_H

#include <stdbool.h>

/* Inicializa el sistema party.
 *   config_path: "data/mods/party_mode/config.json"
 *   Devuelve true si OK. */
bool party_init(const char* config_path);

/* Libera recursos (GL, audio, controllers). */
void party_shutdown(void);

/* Game loop: se llama UNA vez por frame, antes de render. */
void party_update(float dt);

/* Render: se llama DESPUÉS de party_update, dibuja todos los viewports. */
void party_render(void);

/* Cuántos jugadores activos hay (1-4). */
int party_player_count(void);

/* Cambia layout en runtime (p. ej. en pausa). */
void party_set_layout(const char* layout);

int  party_get_viewport(int player_id, int* x, int* y, int* w, int* h);

#endif /* PARTY_H */
