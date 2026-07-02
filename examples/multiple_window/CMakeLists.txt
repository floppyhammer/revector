add_executable(vecgui-multiple-window ${SOURCE_FILES} main.cpp)

target_include_directories(vecgui-multiple-window PUBLIC "../../src")

target_link_libraries(vecgui-multiple-window vecgui)
