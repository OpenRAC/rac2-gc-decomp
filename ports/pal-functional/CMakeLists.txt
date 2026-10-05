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
