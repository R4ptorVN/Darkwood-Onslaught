cmake_minimum_required(VERSION 3.22)
project(PROJECT_NAME)

set(CMAKE_CXX_STANDARD 14)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
add_executable(${PROJECT_NAME} scripts/main.cpp
        scripts/lib/RenderWindow.hpp
        scripts/lib/Entity.hpp
        scripts/lib/Player.hpp
        scripts/lib/Functions.hpp
        scripts/lib/Obstacle.hpp
        scripts/lib/Map.hpp
        scripts/lib/Enemy.hpp
        scripts/lib/Game.hpp
        scripts/lib/Projectile.hpp
        scripts/lib/Skeleton.hpp
        scripts/lib/Goblin.hpp
        scripts/lib/Orc.hpp
        scripts/lib/Necromancer.hpp
        scripts/RenderWindow.cpp
        scripts/Entity.cpp
        scripts/Player.cpp
        scripts/Functions.cpp
        scripts/Obstacle.cpp
        scripts/Map.cpp
        scripts/Enemy.cpp
        scripts/Game.cpp
        scripts/Goblin.cpp
        scripts/Orc.cpp
        scripts/Projectile.cpp
        scripts/Skeleton.cpp
        scripts/Necromancer.cpp
)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})