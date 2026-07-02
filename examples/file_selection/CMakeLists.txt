add_executable(vecgui-file-selection ${SOURCE_FILES} main.cpp)

target_include_directories(vecgui-file-selection PUBLIC "../../src")

target_link_libraries(vecgui-file-selection vecgui)
