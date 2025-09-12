add_executable(revector-multiple-window ${SOURCE_FILES} main.cpp)

target_include_directories(revector-multiple-window PUBLIC "../../src")

target_link_libraries(revector-multiple-window revector)
