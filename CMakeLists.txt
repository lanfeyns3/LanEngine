cmake_minimum_required(VERSION 3.10)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LanEngine)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/lanfeyns3/glad.git
    GIT_TAG c
)



FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG main
)

FetchContent_MakeAvailable(
    glfw
    glad
    nlohmann_json
    EnTT
)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE ENGINE_FILES
    CONFIGURE_DEPENDS
    src/Engine/*.cpp
    src/Engine/*.h
    src/Engine/**/*.cpp
    src/Engine/**/*.h
)

file(GLOB_RECURSE EDITOR_FILES
    CONFIGURE_DEPENDS
    src/Editor/*.cpp
    src/Editor/**/*.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${ENGINE_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    src/Engine/
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    glad
    OpenGL::GL
    nlohmann_json::nlohmann_json
    EnTT::EnTT
)

add_executable(Editor
    ${EDITOR_FILES}
)

target_link_libraries(Editor
    ${PROJECT_NAME}
)