# src/game/CMakeLists.txt
set(GAME_SRCS
    weapons.c
    enemies.c
    ai.c
    levels.c
    transformations.c
)

add_library(game ${GAME_SRCS})
