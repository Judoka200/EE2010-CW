cmake_minimum_required(VERSION 3.28)   

project(TextDungeon VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)             
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    SYSTEM)
set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
FetchContent_MakeAvailable(SFML)

# ---- game logic: everything in src/ except the two mains ----
file(GLOB SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SOURCES EXCLUDE REGEX ".*/main(_1)?\\.cpp$")

add_library(game STATIC ${SOURCES})
target_include_directories(game PUBLIC src)

# ---- SFML UI (src/main.cpp) ----
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE game SFML::Graphics)

# ---- console test build (src/main_1.cpp) ----
