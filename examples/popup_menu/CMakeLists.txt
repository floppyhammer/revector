add_executable(revector-popup-menu ${SOURCE_FILES} main.cpp)

target_include_directories(revector-popup-menu PUBLIC "../../src")

target_link_libraries(revector-popup-menu revector)
