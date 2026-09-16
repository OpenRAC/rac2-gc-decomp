# src/engine/CMakeLists.txt
set(ENGINE_SRCS
    scene.c
    entity.c
    collision.c
    animation.c
)

add_library(engine ${ENGINE_SRCS})
