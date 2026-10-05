# English

# Memory Card Subsystem Documentation - Ratchet & Clank 2 (PAL)

This document compiles the text strings and calls to the Sony SDK (`libmc`) used to manage game progress.

## File Format Templates (Memory Card Path Template)

| Memory Address | Data Type | Static String Value | Purpose / Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001A9AA2`** | `char[]` (String) | `“BESCES-50916RATCHET/save%d.bin”` | Format mask used by vsnprintf to define the path of the game’s binary file in the slot (`mc0:` / `mc1:`). |

# Español

# Mod: Party Mode — Multi-Jugador Local

## Objetivo

Permitir 2 a 4 jugadores locales en pantalla dividida.
Cada jugador controla un Ratchet (o un Clank) independiente,
con input propio, cámara propia, HUD propio, pero comparten
el mismo mundo, la misma IA, la misma física.

## Alcance

| Sí | No (por ahora) |
|---|---|
| 2-4 jugadores locales | Multiplayer online / red |
| Split-screen 2×1, 1×2, 2×2 | Pantalla completa por jugador |
| Inputs por controlador | Inputs por teclado+mouse (opcional futuro) |
| HUD independiente por jugador | Menú de pausa compartido (por ahora sí) |
| Mismo mundo, misma IA | Mundos separados / co-op asimétrico |
| Armas compartidas (mismo pool) | Armas únicas por jugador (opcional) |

## Configuración

`data/mods/party_mode/config.json`:

```json
{
  "enabled": true,
  "max_players": 4,
  "layout": "2x2",
  "players": [
    {
      "id": 0,
      "controller": 0,
      "character": "ratchet",
      "color_override": null,
      "hud_theme": "default"
    },
    {
      "id": 1,
      "controller": 1,
      "character": "clank",
      "color_override": "#ff4444",
      "hud_theme": "clank"
    }
  ]
}

Game loop (multi)
main_loop:
    dt = timer()

    // 1. Inputs (por jugador)
    for p in 0..N-1:
        party_update_input(p)

    // 2. Lógica de mundo (UNA VEZ, no por jugador)
    world_update_physics(dt)
    world_update_ai(dt)
    world_update_collisions()

    // 3. Estados por jugador (cámara, HUD, animación)
    for p in 0..N-1:
        player_update_camera(p, dt)
        player_update_hud(p, dt)

    // 4. Render (por viewport)
    for p in 0..N-1:
        glViewport(vp[p].x, vp[p].y, vp[p].w, vp[p].h)
        glScissor(vp[p].x, vp[p].y, vp[p].w, vp[p].h)
        glEnable(GL_SCISSOR_TEST)
        render_world(player_camera[p])
        render_hud(p)
        glDisable(GL_SCISSOR_TEST)

    SDL_GL_SwapWindow()


CMake (se agrega a CMakeLists.txt raíz)
# --- engine/party ---
add_library(engine_party STATIC
    src/engine/party/party.c
    src/engine/party/player.c
    src/engine/party/viewport.c
)
target_include_directories(engine_party PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(engine_party
    engine_display      # display_init_channel, display_init_channel_b
    engine_semaphore    # semaphore_map
    core_sce_compat     # stubs
    SDL2::SDL2
    OpenGL::GL
)
