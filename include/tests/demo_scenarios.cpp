
/**
 * @file demo.cpp
 * @author Umut Ertuğrul Daşgın
 * @co-conturbuted Alp Dikmen
 * @brief Entry point for the VCS Project. Contains Demo.
 * @version 1.0
 * @date 2025-12-25
 */
// File: `include/tests/demo_scenarios.cpp`
//
// Brief: Use demo-scoped paths so the repository tracks them under `tests/demo/`
// instead of looking like it modifies project-root files.

#include "demo_scenarios.h"

#include <iostream>
#include <core/Repository.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define CYAN    "\033[36m"

void run_demo() {
    std::cout << GREEN << "=== AUTOMATED DEMO SCENARIO STARTED ===" << RESET << std::endl;

    try {
        core::Repository repo;

        const std::string base = "tests/demo/";

        std::cout << CYAN << "\n[STEP 1] Initial Commit on Master" << RESET << std::endl;
        repo.add(base + "main.cpp", "int main() { return 0; }");
        repo.add(base + "readme.txt", "This is a VCS project.");
        repo.commit("Initial commit", "Umut");

        std::cout << CYAN << "\n[STEP 2] Create and Switch to 'feature-login'" << RESET << std::endl;
        repo.create_branch("feature-login");
        repo.checkout("feature-login");

        std::cout << CYAN << "\n[STEP 3] Work on Feature Branch" << RESET << std::endl;
        repo.add(base + "login.cpp", "void login() { /* logic */ }");
        repo.add(base + "main.cpp", "int main() { login(); return 0; }");
        repo.commit("Added login feature", "Alp");

        std::cout << CYAN << "\n[STEP 4] Switch back to Master and Create Conflict" << RESET << std::endl;
        repo.checkout("master");
        repo.add(base + "main.cpp", "int main() { std::cout << 'Hello'; return 0; }");
        repo.commit("Changed main on master", "Umut");

        std::cout << CYAN << "\n[STEP 5] Merge 'feature-login' into 'master' (Expect Conflict)" << RESET << std::endl;
        repo.merge("feature-login");

        std::cout << CYAN << "\n[STEP 6] Show History" << RESET << std::endl;
        repo.log();

    } catch (const std::exception& e) {
        std::cerr << RED << "Demo Error: " << e.what() << RESET << std::endl;
    }

    std::cout << GREEN << "\n=== DEMO FINISHED ===" << RESET << std::endl;
}
