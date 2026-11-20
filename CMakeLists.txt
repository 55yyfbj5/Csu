cmake_minimum_required(VERSION 3.20)
project(Csu VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# 1. nlohmann_json
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# 2. SDL3
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.20
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(SDL3_mixer)

# 3. sokol (Header-only / source library framework)
FetchContent_Declare(
    sokol
    GIT_REPOSITORY https://github.com/floooh/sokol.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(sokol)

# Define an interface library target for sokol
add_library(sokol INTERFACE)
target_include_directories(sokol INTERFACE ${sokol_SOURCE_DIR})

# Platform-specific link libraries/frameworks often required by Sokol backends
if(APPLE)
    target_link_options(sokol INTERFACE "-framework" "Metal" "-framework" "QuartzCore" "-framework" "Cocoa" "-framework" "AudioToolbox")
elseif(UNIX AND NOT ANDROID)
    target_link_libraries(sokol INTERFACE X11 Xi Xcursor GL dl pthread m)
endif()

# 4. miniaudio (Single-file header/source library)
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(miniaudio)

# Define an interface library target for miniaudio
# add_library(miniaudio INTERFACE)
# target_include_directories(miniaudio INTERFACE ${miniaudio_SOURCE_DIR})


file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    src
)

# Link all dependencies to your target
target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
    SDL3::SDL3
    SDL3_mixer::SDL3_mixer
    sokol
    miniaudio
)

# Copy assets
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory..."
)
add_dependencies(${PROJECT_NAME} copy_assets)