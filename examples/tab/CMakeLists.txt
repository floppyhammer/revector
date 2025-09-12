add_executable(revector-tab ${SOURCE_FILES} main.cpp)

target_include_directories(revector-tab PUBLIC "../../src")

target_link_libraries(revector-tab revector)
