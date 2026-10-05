# Mod: Party Mode — Local Multiplayer

## Goal

Allow 2 to 4 local players in split screen.
Each player controls an independent Ratchet (or Clank),
with their own input, camera and HUD, while sharing
the same world, the same AI and the same physics.

## Scope

| Yes | No (for now) |
|---|---|
| 2-4 local players | Online / network multiplayer |
| Split screen 2×1, 1×2, 2×2 | Full screen per player |
| Per-controller input | Keyboard+mouse input (optional, future) |
| Independent HUD per player | Shared pause menu (shared for now) |
| Same world, same AI | Separate worlds / asymmetric co-op |
| Shared weapons (same pool) | Unique weapons per player (optional) |

## Configuration

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
```

## Game loop (multi)

```
main_loop:
    dt = timer()

    // 1. Inputs (per player)
    for p in 0..N-1:
        party_update_input(p)

    // 2. World logic (ONCE, not per player)
    world_update_physics(dt)
    world_update_ai(dt)
    world_update_collisions()

    // 3. Per-player state (camera, HUD, animation)
    for p in 0..N-1:
        player_update_camera(p, dt)
        player_update_hud(p, dt)

    // 4. Render (per viewport)
    for p in 0..N-1:
        glViewport(vp[p].x, vp[p].y, vp[p].w, vp[p].h)
        glScissor(vp[p].x, vp[p].y, vp[p].w, vp[p].h)
        glEnable(GL_SCISSOR_TEST)
        render_world(player_camera[p])
        render_hud(p)
        glDisable(GL_SCISSOR_TEST)

    SDL_GL_SwapWindow()
```

## CMake (added to the root CMakeLists.txt)

```cmake
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
```
