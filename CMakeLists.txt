cmake_minimum_required(VERSION 3.16)

project(RubiksCubeSolver LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# The solver is far too slow without optimizations, so default to Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# ---------------------------------------------------------------------------
# Third-party dependencies
# ---------------------------------------------------------------------------
include(FetchContent)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "")
set(GLM_BUILD_TESTS     OFF CACHE BOOL "" FORCE)
set(GLM_BUILD_LIBRARY   OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)
FetchContent_Declare(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glfw glm)

find_package(Threads REQUIRED)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(stb_image STATIC external/stb/stb_image.cpp)
target_include_directories(stb_image PUBLIC external/stb)

# ---------------------------------------------------------------------------
# Application
# ---------------------------------------------------------------------------
add_executable(rubiks-cube-solver
    src/main.cpp

    src/cube/cubeIndexModel.cpp
    src/cube/cubeScramble.cpp
    src/cube/moveStore.cpp

    src/graphics/animation.cpp
    src/graphics/camera.cpp
    src/graphics/cube.cpp
    src/graphics/helper_functions.cpp
    src/graphics/shader.cpp

    src/solver/movePruner.cpp
    src/solver/simplifySolution.cpp

    src/solver/kociemba/g1DatabaseController.cpp
    src/solver/kociemba/g2DatabaseController.cpp
    src/solver/kociemba/kociembaSolver.cpp
    src/solver/kociemba/moveController.cpp
    src/solver/kociemba/patternIndexer.cpp
    src/solver/kociemba/symmetryIndexer.cpp

    src/util/math.cpp
    src/util/progressBar.cpp
)

target_include_directories(rubiks-cube-solver PRIVATE src)
target_compile_definitions(rubiks-cube-solver PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(rubiks-cube-solver PRIVATE glad glfw glm::glm stb_image Threads::Threads)

# Lets the compiler inline small functions across .cpp files (e.g. move table lookups in the search loop)
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR LANGUAGES CXX)
if(IPO_SUPPORTED)
    set_property(TARGET rubiks-cube-solver PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

# The program loads resources/... relative to the working directory, so make
# them reachable from the build directory as well as from the project root
if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/resources" "${CMAKE_CURRENT_BINARY_DIR}/resources" SYMBOLIC)
endif()
