cmake_minimum_required(VERSION 3.16)
project(raytracer CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

add_compile_options(-O3 -ffast-math -march=native)

# ── Interface sources (compiled into binary and each plugin) ──────────────────
file(GLOB_RECURSE INTERFACE_SOURCES
    src/Exceptions/*.cpp
    src/Geometry/*.cpp
    src/Objects/Abstracts/*.cpp
    src/Utils/*.cpp
)

# ── Main binary ───────────────────────────────────────────────────────────────
file(GLOB         MAIN_ENTRY    src/*.cpp)          # picks up main.cpp when added
file(GLOB_RECURSE SYSTEM_SOURCES src/SceneSystem/*.cpp)

add_executable(raytracer
    ${MAIN_ENTRY}
    ${SYSTEM_SOURCES}
    ${INTERFACE_SOURCES}
)

target_include_directories(raytracer PRIVATE
    src
    ${LIBCONFIG_INCLUDE_DIRS}
)

target_link_libraries(raytracer PRIVATE
    ${LIBCONFIG_LIBRARIES}
    dl
)

# ── Plugin shared libraries (.so) ─────────────────────────────────────────────
file(GLOB PLUGIN_DIRS ${CMAKE_SOURCE_DIR}/src/Objects/Plugins/*)

foreach(PLUGIN_DIR ${PLUGIN_DIRS})
    if(IS_DIRECTORY ${PLUGIN_DIR})
        get_filename_component(PLUGIN_NAME ${PLUGIN_DIR} NAME)

        file(GLOB_RECURSE PLUGIN_SOURCES ${PLUGIN_DIR}/*.cpp)

        add_library(${PLUGIN_NAME} SHARED
            ${PLUGIN_SOURCES}
            ${INTERFACE_SOURCES}
        )

        set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")

        target_include_directories(${PLUGIN_NAME} PRIVATE
            src
            ${LIBCONFIG_INCLUDE_DIRS}
        )

        target_link_libraries(${PLUGIN_NAME} PRIVATE
            ${LIBCONFIG_LIBRARIES}
        )
    endif()
endforeach()
