cmake_minimum_required(VERSION 3.16)
project(GravitySimulator LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

# ---------------------------------------------------------------------------
# Dependencies (auto-downloaded via FetchContent)
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.9
)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
add_executable(GravitySimulator
    src/main.cpp

    # Vendor
    vendor/glad/glad.c
    vendor/stb/stb_image.cpp

    # View
    src/view/Window.cpp
    src/view/Mesh.cpp
    src/view/Shader.cpp
    src/view/TextureLoader.cpp
    src/view/Trail.cpp

    # Model
    src/model/Camera.cpp
    src/model/Simulation.cpp
    src/model/SphereGeometry.cpp

    # Controller
    src/controller/App.cpp
)

target_include_directories(GravitySimulator PRIVATE vendor/)
target_link_libraries(GravitySimulator PRIVATE glfw glm)

# Enable stricter warnings in Debug builds
target_compile_options(GravitySimulator PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
        -Wall -Wextra -Wpedantic
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    >
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
target_link_options(GravitySimulator PRIVATE
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<CONFIG:Debug>>:-fsanitize=address,undefined>
)

# Copy runtime assets next to the binary
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets  DESTINATION ${CMAKE_BINARY_DIR})
