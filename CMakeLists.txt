cmake_minimum_required(VERSION 3.16)
project(raytracer CXX)

cmake_policy(SET CMP0110 NEW)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

include(FetchContent)
FetchContent_Declare(
    libconfig
    GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS    OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(libconfig)

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

# SceneWriter is used directly in main.cpp but lives in the plugin tree
set(SCENE_WRITER_SOURCES
    src/Objects/Plugins/CorePlugin/Interfaces/SceneWriter/SceneWriter.cpp
)

add_executable(raytracer
    ${MAIN_ENTRY}
    ${SYSTEM_SOURCES}
    ${INTERFACE_SOURCES}
    ${SCENE_WRITER_SOURCES}
)

target_include_directories(raytracer PRIVATE src)

target_link_libraries(raytracer PRIVATE
    config++
    dl
)

# Export all symbols from the main binary so dlopen'd plugins can resolve them
target_link_options(raytracer PRIVATE -rdynamic)

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

        target_include_directories(${PLUGIN_NAME} PRIVATE src)

        target_link_libraries(${PLUGIN_NAME} PRIVATE config++)
    endif()
endforeach()

# ── Unit tests ───────────────────────────────────────────────────────────────

include(CTest)

enable_testing()

file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

add_executable(tests_run
    ${TEST_SOURCES}
    ${SYSTEM_SOURCES}
    ${INTERFACE_SOURCES}
    ${SCENE_WRITER_SOURCES}
)

target_compile_options(tests_run PRIVATE -O0 -g --coverage)
target_link_options(tests_run PRIVATE --coverage)

target_include_directories(tests_run PRIVATE src)

target_link_libraries(tests_run PRIVATE criterion config++)

foreach(TEST_FILE ${TEST_SOURCES})
    file(READ ${TEST_FILE} TEST_CONTENTS)

    string(REGEX MATCHALL "Test\\([^,]+,\\s*[^)]+\\)" TEST_MATCHES "${TEST_CONTENTS}")

    foreach(TEST_MATCH ${TEST_MATCHES})
        string(REGEX REPLACE "Test\\(\\s*([^,]+),.*" "\\1" TEST_SUITE "${TEST_MATCH}")
        string(STRIP "${TEST_SUITE}" TEST_SUITE)

        string(REGEX REPLACE "Test\\([^,]+,\\s*([^)]+)\\).*" "\\1" TEST_NAME "${TEST_MATCH}")
        string(STRIP "${TEST_NAME}" TEST_NAME)

        string(REPLACE "/" "_" CT_NAME "${TEST_SUITE}_${TEST_NAME}")
        string(REGEX REPLACE "[^A-Za-z0-9_.-]" "_" CT_NAME "${CT_NAME}")

        add_test(NAME ${CT_NAME} COMMAND tests_run --filter "${TEST_SUITE}/${TEST_NAME}" --timeout 10)
        set_tests_properties(${CT_NAME} PROPERTIES WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    endforeach()
endforeach()
