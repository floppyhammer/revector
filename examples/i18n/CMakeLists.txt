add_executable(revector-i18n ${SOURCE_FILES} main.cpp)

target_include_directories(revector-i18n PUBLIC "../../src")

target_link_libraries(revector-i18n revector)
