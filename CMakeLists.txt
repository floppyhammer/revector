cmake_minimum_required(VERSION 3.16)

project(revector
        VERSION 0.1
        LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Identify Linux.
if (UNIX AND NOT APPLE AND NOT ANDROID)
    set(LINUX ON)
endif ()

option(REVECTOR_VULKAN "Use Vulkan instead of OpenGL" OFF)
option(REVECTOR_FRIBIDI "Use fribidi instead of icu" ON)

if (APPLE)
    set(REVECTOR_FRIBIDI ON)
endif ()

file(GLOB_RECURSE REVECTOR_SOURCE_FILES CONFIGURE_DEPENDS
        src/*.cpp
        third_party/tinyfiledialogs/tinyfiledialogs.c)

add_library(revector STATIC ${REVECTOR_SOURCE_FILES})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Has to be 17 or above.
target_compile_features(revector PUBLIC cxx_std_17)

# Include external and third-party headers.
target_include_directories(revector PUBLIC "third_party")

if (REVECTOR_VULKAN)
    set(PATHFINDER_VULKAN_SUPPORT ON CACHE BOOL "" FORCE)
else ()
    set(PATHFINDER_VULKAN_SUPPORT OFF CACHE BOOL "" FORCE)
endif ()
target_include_directories(revector PUBLIC "third_party/pathfinder-cpp")
add_subdirectory("third_party/pathfinder-cpp")

target_include_directories(revector PUBLIC "third_party/harfbuzz-8.4.0/src")
add_subdirectory("third_party/harfbuzz-8.4.0")

target_include_directories(revector PUBLIC "third_party/GSL-4.0.0/include")
add_subdirectory("third_party/GSL-4.0.0")

target_include_directories(revector PUBLIC "third_party/rapidcsv-8.84/src")
add_subdirectory("third_party/rapidcsv-8.84")

target_include_directories(revector PUBLIC "third_party/stduuid-1.2.3")

add_subdirectory("third_party/zlib")

target_link_libraries(revector PUBLIC pathfinder harfbuzz zlibstatic)

if (NOT REVECTOR_FRIBIDI)
    if (WIN32)
        # Fix "too many characters in constant" error when using CMake with MSVC.
        string(APPEND CMAKE_CXX_FLAGS " /utf-8")

        target_link_libraries(revector PUBLIC icu)
    elseif (LINUX)
        # icu4c.
        find_package(ICU 70.0 COMPONENTS uc i18n REQUIRED)

        target_link_libraries(revector PUBLIC ICU::uc ICU::i18n)
    endif ()
endif ()

if (REVECTOR_FRIBIDI)
    # Alternative to icu4c. If the target platform comes with ICU, we just use it.
    # Else, we build fribidi. It's too cumbersome to build ICU ourselves.
    add_subdirectory("third_party/fribidi")
    target_link_libraries(revector PUBLIC fribidi)

    target_compile_definitions(revector PUBLIC REVECTOR_USE_FRIBIDI)
endif ()

# Copy the assets to the binary directory.
file(COPY "assets" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Add examples.
add_subdirectory(examples/file_selection)
add_subdirectory(examples/container_layout)
add_subdirectory(examples/texture_rect)
add_subdirectory(examples/scrolling)
add_subdirectory(examples/tab)
add_subdirectory(examples/progress_bar)
add_subdirectory(examples/i18n)
add_subdirectory(examples/label_alignment)
add_subdirectory(examples/multiple_window)
add_subdirectory(examples/button)
add_subdirectory(examples/tree)
add_subdirectory(examples/popup_menu)
add_subdirectory(examples/collapse_containers)
