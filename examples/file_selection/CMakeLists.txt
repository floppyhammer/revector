add_executable(revector-file-selection ${SOURCE_FILES} main.cpp)

target_include_directories(revector-file-selection PUBLIC "../../src")

target_link_libraries(revector-file-selection revector)
