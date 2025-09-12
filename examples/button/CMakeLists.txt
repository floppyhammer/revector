add_executable(revector-button ${SOURCE_FILES} main.cpp)

target_include_directories(revector-button PUBLIC "../../src")

target_link_libraries(revector-button revector)
