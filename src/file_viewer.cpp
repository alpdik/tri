//
// Created by alpdi on 9/1/2026.
//
// File: `src/file_viewer.cpp`
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
