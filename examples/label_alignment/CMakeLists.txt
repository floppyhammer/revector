add_executable(revector-label-alignment ${SOURCE_FILES} main.cpp)

target_include_directories(revector-label-alignment PUBLIC "../../src")

target_link_libraries(revector-label-alignment revector)
