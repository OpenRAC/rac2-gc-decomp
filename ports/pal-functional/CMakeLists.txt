# CMakeLists.txt principal
cmake_minimum_required(VERSION 3.15)
project(RAC2Decomp C)

# Configure the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SDL2 paths
set(SDL2_INCLUDE_DIR "C:/SDL2/include")
set(SDL2_LIBRARY_DIR "C:/SDL2/lib/x64")
set(SDL2_LIBRARIES "SDL2.lib" "SDL2main.lib")

# Release region: PAL (the reconstruction's source, SCES_516.07) or NTSC (USA, SCUS_972.68).
# It selects the boot executable name, GS output mode, frame clock and display height
# in include/core/region.h.
set(RAC2_REGION "PAL" CACHE STRING "Game release region: PAL or NTSC")
set_property(CACHE RAC2_REGION PROPERTY STRINGS PAL NTSC)
string(TOUPPER "${RAC2_REGION}" RAC2_REGION_UPPER)
if(NOT RAC2_REGION_UPPER MATCHES "^(PAL|NTSC)$")
    message(FATAL_ERROR "RAC2_REGION must be PAL or NTSC, not '${RAC2_REGION}'")
endif()
add_compile_definitions(RAC2_REGION_${RAC2_REGION_UPPER}=1)
message(STATUS "RAC2 release region: ${RAC2_REGION_UPPER}")

# Add the SDL2 include paths
include_directories(${SDL2_INCLUDE_DIR})

# Add the local include directories
include_directories(include)

# Add the subdirectories
add_subdirectory(src/core)
add_subdirectory(src/engine)
add_subdirectory(src/game)

# Name of the final executable (.exe)
add_executable(RAC2PC src/main.c)

# Link the static libraries and SDL2
target_link_libraries(RAC2PC core engine game ${SDL2_LIBRARIES})
