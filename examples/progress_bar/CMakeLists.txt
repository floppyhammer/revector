add_executable(revector-progress-bar ${SOURCE_FILES} main.cpp)

target_include_directories(revector-progress-bar PUBLIC "../../src")

target_link_libraries(revector-progress-bar revector)
