add_executable(revector-collapse-containers ${SOURCE_FILES} main.cpp)

target_include_directories(revector-collapse-containers PUBLIC "../../src")

target_link_libraries(revector-collapse-containers revector)
