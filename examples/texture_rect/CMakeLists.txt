add_executable(revector-texture-rect ${SOURCE_FILES} main.cpp)

target_include_directories(revector-texture-rect PUBLIC "../../src")

target_link_libraries(revector-texture-rect revector)
