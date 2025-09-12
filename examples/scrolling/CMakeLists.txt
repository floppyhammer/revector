add_executable(revector-scrolling ${SOURCE_FILES} main.cpp)

target_include_directories(revector-scrolling PUBLIC "../../src")

target_link_libraries(revector-scrolling revector)
