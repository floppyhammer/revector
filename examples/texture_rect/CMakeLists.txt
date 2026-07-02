add_executable(vecgui-texture-rect ${SOURCE_FILES} main.cpp)

target_include_directories(vecgui-texture-rect PUBLIC "../../src")

target_link_libraries(vecgui-texture-rect vecgui)
