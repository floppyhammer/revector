add_executable(revector-tree ${SOURCE_FILES} main.cpp)

target_include_directories(revector-tree PUBLIC "../../src")

target_link_libraries(revector-tree revector)
