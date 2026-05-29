cmake_minimum_required(VERSION 3.20)
project(dashforge3d C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# cglm (header-only — no static lib needed)
FetchContent_Declare(
    cglm
    GIT_REPOSITORY https://github.com/recp/cglm.git
    GIT_TAG        v0.9.4
    GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(cglm)
if(NOT cglm_POPULATED)
    FetchContent_Populate(cglm)
endif()

# stb (single-header libraries — header-only)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
    GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(stb)
if(NOT stb_POPULATED)
    FetchContent_Populate(stb)
endif()

# GLAD (vendored pre-generated OpenGL 3.3 Core loader)
add_library(glad STATIC
    external/glad/src/glad.c
)
target_include_directories(glad PUBLIC external/glad/include)
if(WIN32)
    target_link_libraries(glad PUBLIC opengl32)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(glad PUBLIC OpenGL::GL ${CMAKE_DL_LIBS})
endif()

# Game source files
set(SOURCES
    src/main.c
    src/core/window.c
    src/core/input.c
    src/core/time.c
    src/renderer/shader.c
    src/renderer/mesh.c
    src/renderer/texture.c
    src/renderer/camera.c
    src/renderer/renderer.c
    src/physics/collision.c
    src/game/level_object.c
    src/game/level.c
    src/game/player.c
    src/game/skin.c
    src/game/game.c
    src/editor/grid.c
    src/editor/editor.c
    src/particles/particles.c
    src/ui/ui.c
)

# Embed exe icon on Windows (only when assets/icon.ico exists)
if(WIN32 AND EXISTS "${CMAKE_SOURCE_DIR}/assets/icon.ico")
    configure_file(
        "${CMAKE_SOURCE_DIR}/src/app.rc.in"
        "${CMAKE_BINARY_DIR}/app.rc"
        @ONLY
    )
    list(APPEND SOURCES "${CMAKE_BINARY_DIR}/app.rc")
endif()

add_executable(dashforge3d ${SOURCES})

target_include_directories(dashforge3d PRIVATE
    src/
    ${cglm_SOURCE_DIR}/include
    ${stb_SOURCE_DIR}
)

target_link_libraries(dashforge3d PRIVATE
    glad
    glfw
)

if(WIN32)
    target_compile_definitions(dashforge3d PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(MINGW)
        target_link_libraries(dashforge3d PRIVATE m)
        # Suppress the console window in Release builds
        target_link_options(dashforge3d PRIVATE $<$<CONFIG:Release>:-mwindows>)
    endif()
else()
    target_link_libraries(dashforge3d PRIVATE m)
endif()

# Copy assets to build directory after build
add_custom_command(TARGET dashforge3d POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:dashforge3d>/assets"
    COMMENT "Copying assets to build directory..."
)
