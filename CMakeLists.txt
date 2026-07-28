cmake_minimum_required(VERSION 3.10)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

project(LanEngine)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
)

FetchContent_Declare(
    vk-bootstrap
    GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap.git
    GIT_TAG v1.4.328
)

FetchContent_Declare(
    volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG vulkan-sdk-1.4.328.0
)

FetchContent_MakeAvailable(glfw vk-bootstrap volk)
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE ENGINE_FILES src/Engine/**.cpp src/Engine/**.h)
file(GLOB_RECURSE EDITOR_FILES src/Editor/**.cpp)


add_library(${PROJECT_NAME} STATIC ${ENGINE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw volk vk-bootstrap::vk-bootstrap Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC src/Engine/)

add_executable(Editor ${EDITOR_FILES})
target_link_libraries(Editor ${PROJECT_NAME})