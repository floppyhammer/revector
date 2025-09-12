add_executable(revector-split-container ${SOURCE_FILES} main.cpp)

target_include_directories(revector-split-container PUBLIC "../../src")

target_link_libraries(revector-split-container revector)
