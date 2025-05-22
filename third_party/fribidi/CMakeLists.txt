cmake_minimum_required(VERSION 3.16)

project(fribidi LANGUAGES C)

file(GLOB FRIBIDI_SOURCE_FILES CONFIGURE_DEPENDS ${FRIBIDI_SOURCE_FILES} "src/*.c")

add_library(fribidi STATIC ${FRIBIDI_SOURCE_FILES})

# To fix "error: call to undeclared library function 'memset' with type 'void *(void *, int, unsigned long)'".
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_STANDARD 90)
    set(CMAKE_C_STANDARD_REQUIRED TRUE)
endif ()

target_include_directories(fribidi PUBLIC "src")
target_include_directories(fribidi PUBLIC "generated")
target_compile_definitions(fribidi PUBLIC HAVE_STRINGIZE=1 HAVE_STDLIB_H)
target_compile_definitions(fribidi PUBLIC FRIBIDI_BUILD)

if (APPLE)
    target_compile_definitions(fribidi PUBLIC HAVE_STRINGS_H)
endif ()
