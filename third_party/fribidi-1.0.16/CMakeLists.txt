cmake_minimum_required(VERSION 3.16)

project(fribidi LANGUAGES C)

file(GLOB FRIBIDI_SOURCE_FILES CONFIGURE_DEPENDS ${FRIBIDI_SOURCE_FILES} "lib/*.c")

add_library(fribidi STATIC ${FRIBIDI_SOURCE_FILES})

target_include_directories(fribidi PUBLIC "lib")
target_include_directories(fribidi PUBLIC "gen.tab")
target_include_directories(fribidi PUBLIC "build")
target_compile_definitions(fribidi PUBLIC HAVE_STRINGIZE=1 HAVE_STDLIB_H)
target_compile_definitions(fribidi PUBLIC FRIBIDI_BUILD)

if (APPLE)
    target_compile_definitions(fribidi PUBLIC HAVE_STRINGS_H)
endif ()
