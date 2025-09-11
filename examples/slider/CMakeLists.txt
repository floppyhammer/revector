add_executable(revector-slider ${SOURCE_FILES} main.cpp)

target_include_directories(revector-slider PUBLIC "../../src")

target_link_libraries(revector-slider revector)
