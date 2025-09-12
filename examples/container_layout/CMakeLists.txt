add_executable(revector-container-layout ${SOURCE_FILES} main.cpp)

target_include_directories(revector-container-layout PUBLIC "../../src")

target_link_libraries(revector-container-layout revector)
