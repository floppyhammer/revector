add_executable(vecgui-tab ${SOURCE_FILES} main.cpp)

target_include_directories(vecgui-tab PUBLIC "../../src")

target_link_libraries(vecgui-tab vecgui)
