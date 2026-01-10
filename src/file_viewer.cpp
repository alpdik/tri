/**
 * @file file_viewer.cpp
 * @author Alp Dikmen
 * @date 2026-01-09
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "file_viewer.hpp"

#include <fstream>
#include <iostream>
#include <string>

bool print_file_contents(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Failed to open file: " << path << "\n";
        return false;
    }

    std::cout << "=== " << path << " ===\n";
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    std::cout << "=== end ===\n";
    return true;
}
