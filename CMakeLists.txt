# CMakeLists.txt principal
cmake_minimum_required(VERSION 3.15)
project(RAC2Decomp C)

# Configurar el estándar de C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Rutas de SDL2
set(SDL2_INCLUDE_DIR "C:/SDL2/include")
set(SDL2_LIBRARY_DIR "C:/SDL2/lib/x64")
set(SDL2_LIBRARIES "SDL2.lib" "SDL2main.lib")

# Incluir rutas de SDL2
include_directories(${SDL2_INCLUDE_DIR})

# Incluir directorios locales
include_directories(include)

# Agregar subdirectorios
add_subdirectory(src/core)
add_subdirectory(src/engine)
add_subdirectory(src/game)

# Definir el nombre del archivo ejecutable final (.exe)
add_executable(RAC2PC src/main.c)

# Vincular las bibliotecas estáticas y SDL2
target_link_libraries(RAC2PC core engine game ${SDL2_LIBRARIES})
