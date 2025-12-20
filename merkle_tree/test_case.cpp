//
// Created by alpdi on 20/12/2025.
//


// main.cpp
#include <iostream>
#include <fstream>
#include <filesystem>
#include "merkle_tree.h" // Import the class

namespace fs = std::filesystem;

int main() {
    std::string projectPath = "./test_project";

    // --- SETUP: Create dummy files for testing ---
    if (fs::exists(projectPath)) fs::remove_all(projectPath); // Clean old run
    fs::create_directories(projectPath + "/src");
    std::ofstream(projectPath + "/README.md") << "Popoeyes menu items created";
    std::ofstream(projectPath + "/src/main.cpp") << "int main() { return 0; }";
    std::ofstream(projectPath + "/src/PopeyesMenu.md") << "Chicken sandwich \n Fries \n Nuggets";
    // ---------------------------------------------

    std::cout << "Building Merkle Tree for: " << projectPath << "\n" << std::endl;

    MerkleTree tree(projectPath);

    if (tree.root) {
        tree.printTree(tree.root);
        std::cout << "\nRoot Commit Hash: " << tree.root->hash << std::endl;
    }

    return 0;
}