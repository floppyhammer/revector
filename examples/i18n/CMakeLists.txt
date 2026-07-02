add_executable(vecgui-i18n ${SOURCE_FILES} main.cpp)

target_include_directories(vecgui-i18n PUBLIC "../../src")

target_link_libraries(vecgui-i18n vecgui)
