add_executable(split_container ${SOURCE_FILES} main.cpp)

target_include_directories(split_container PUBLIC "../../src")

target_link_libraries(split_container revector)
