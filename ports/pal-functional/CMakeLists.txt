# Native PC port of the functional Ratchet & Clank 2 reconstruction.
cmake_minimum_required(VERSION 3.16)
project(RAC2Decomp C)

# Configure the C standard (stdatomic.h and anonymous unions need C11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Release region: PAL (the reconstruction's source, SCES_516.07) or NTSC (USA, SCUS_972.68).
# It selects the boot executable name, GS output mode, frame clock and display height
# in include/core/region.h.
set(RAC2_REGION "PAL" CACHE STRING "Game release region: PAL or NTSC")
set_property(CACHE RAC2_REGION PROPERTY STRINGS PAL NTSC)
string(TOUPPER "${RAC2_REGION}" RAC2_REGION_UPPER)
if(NOT RAC2_REGION_UPPER MATCHES "^(PAL|NTSC)$")
    message(FATAL_ERROR "RAC2_REGION must be PAL or NTSC, not '${RAC2_REGION}'")
endif()
message(STATUS "RAC2 release region: ${RAC2_REGION_UPPER}")

# SDL2: prefer the package's own CMake config (SDL2 development packages, vcpkg,
# the official Windows development archive via SDL2_DIR). Otherwise point
# SDL2_INCLUDE_DIR at the directory holding SDL.h and SDL2_LIBRARY at the library.
find_package(SDL2 CONFIG QUIET)
if(TARGET SDL2::SDL2)
    set(RAC2_SDL2_TARGET SDL2::SDL2)
else()
    find_path(SDL2_INCLUDE_DIR SDL.h PATH_SUFFIXES SDL2 HINTS "C:/SDL2/include")
    find_library(SDL2_LIBRARY NAMES SDL2 SDL2-2.0 libSDL2-2.0.so.0 HINTS "C:/SDL2/lib/x64")
    if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
        message(FATAL_ERROR "SDL2 not found: install its development files, or set SDL2_DIR, "
                            "or SDL2_INCLUDE_DIR (the directory with SDL.h) and SDL2_LIBRARY")
    endif()
    add_library(rac2_sdl2 INTERFACE)
    # Both spellings appear in the sources: <SDL.h> and <SDL2/SDL.h>.
    get_filename_component(SDL2_INCLUDE_PARENT "${SDL2_INCLUDE_DIR}" DIRECTORY)
    target_include_directories(rac2_sdl2 INTERFACE "${SDL2_INCLUDE_DIR}" "${SDL2_INCLUDE_PARENT}")
    target_link_libraries(rac2_sdl2 INTERFACE "${SDL2_LIBRARY}")
    set(RAC2_SDL2_TARGET rac2_sdl2)
endif()

# OpenGL entry points are resolved at run time through SDL_GL_GetProcAddress
# (src/core/render/gl/gl_loader.c), so no OpenGL library or header is linked.
set(RAC2_SOURCES
    src/main.c
    src/boot_init.c
    src/cdvd.c
    src/graphics.c
    src/hud_ammo.c
    src/kernel_sys.c
    src/math_util.c
    src/ps2_graphics.c
    src/ps2_hal.c
    src/ps2_kernel.c
    src/ps2_sif.c
    src/ps2_string.c
    src/system.c
    src/core/boot.c
    src/core/core.c
    src/core/critical.c
    src/core/display.c
    src/core/gpu.c
    src/core/lifecycle.c
    src/core/mem.c
    src/core/sce_compat.c
    src/core/vsync.c
    src/core/render/render_dispatch.c
    src/core/render/gl/gl_backend.c
    src/core/render/gl/gl_loader.c
    src/core/sys/kernel_cache.c
    src/core/sys/kernel_interrupts.c
    src/engine/display_init.c
    src/engine/render_init.c
    src/engine/semaphore_map.c
    src/engine/party/party.c
    src/engine/party/player.c
    src/engine/party/viewport.c
)

# Name of the final executable (.exe)
add_executable(RAC2PC ${RAC2_SOURCES})
target_compile_definitions(RAC2PC PRIVATE RAC2_REGION_${RAC2_REGION_UPPER}=1)
target_include_directories(RAC2PC PRIVATE include include/core)
find_package(Threads REQUIRED)
target_link_libraries(RAC2PC PRIVATE ${RAC2_SDL2_TARGET} Threads::Threads)
if(NOT MSVC)
    target_link_libraries(RAC2PC PRIVATE m)
    target_compile_options(RAC2PC PRIVATE -Wall)
endif()
if(TARGET SDL2::SDL2main)
    target_link_libraries(RAC2PC PRIVATE SDL2::SDL2main)
endif()
