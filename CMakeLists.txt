cmake_minimum_required(VERSION 3.28)
project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE SFML::Graphics)

add_executable(UI_example main.cpp)
target_link_libraries(UI_example PRIVATE SFML::Graphics)
add_custom_command(TARGET UI_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:UI_example>/assets)