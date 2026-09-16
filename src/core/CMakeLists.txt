# src/core/CMakeLists.txt
set(CORE_SRCS
    core.c
    input.c
    audio.c
    timer.c
)

add_library(core ${CORE_SRCS})
