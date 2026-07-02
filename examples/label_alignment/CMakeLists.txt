add_executable(vecgui-label-alignment ${SOURCE_FILES} main.cpp)

target_include_directories(vecgui-label-alignment PUBLIC "../../src")

target_link_libraries(vecgui-label-alignment vecgui)
