#ifndef PARTY_H
#define PARTY_H

#include <stdbool.h>

/* Initializes the party system.
 *   config_path: "data/mods/party_mode/config.json"
 *   Returns true on success. */
bool party_init(const char* config_path);

/* Releases resources (GL, audio, controllers). */
void party_shutdown(void);

/* Game loop: called ONCE per frame, before rendering. */
void party_update(float dt);

/* Render: called AFTER party_update, draws every viewport. */
void party_render(void);

/* Number of active players (1-4). */
int party_player_count(void);

/* Changes the layout at runtime (e.g. while paused). */
void party_set_layout(const char* layout);

int  party_get_viewport(int player_id, int* x, int* y, int* w, int* h);

#endif /* PARTY_H */
