#pragma once

#include <pathfinder/prelude.h>

#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

namespace revector {

inline void split_path(const std::string &str, std::string &file_directory) {
    size_t found = str.find_last_of("/\\");

    //    std::cout << "Splitting: " << str << std::endl;
    //    std::cout << "Folder: " << str.substr(0, found) << std::endl;
    //    std::cout << "File: " << str.substr(found + 1) << std::endl;

    file_directory = str.substr(0, found);
}

} // namespace revector
