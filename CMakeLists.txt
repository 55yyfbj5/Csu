cmake_minimum_required(VERSION 3.25)
project(Tosu VERSION 1.0.0 LANGUAGES CXX C)

# ------------------------------------------------------------------------------
# 1. Compiler Settings
# ------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------------------------------------
# 4. Target & Sources
# ------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
else()
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ------------------------------------------------------------------------------
# 5. Link Dependencies & Definitions
# ------------------------------------------------------------------------------
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

include(FetchContent)
FetchContent_Declare(
    miniaudio
    GIT_REPOSITORY https://github.com/mackron/miniaudio.git
    GIT_TAG        0.11.25
)
FetchContent_MakeAvailable(miniaudio)


if(UNIX AND NOT APPLE AND NOT ANDROID)
    target_link_libraries(miniaudio INTERFACE pthread m dl)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CURSES_LIBRARIES}
    miniaudio
)

if(CURSES_INCLUDE_DIRS)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
endif()

# ------------------------------------------------------------------------------
# 6. Assets Handling
# ------------------------------------------------------------------------------
if(NOT ANDROID)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Copying assets folder to binary directory..."
    )
endif()
