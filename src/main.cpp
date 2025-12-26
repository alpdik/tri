/**
 * @file main.cpp
 * @author Umut Ertuğrul Daşgın
 * @co-conturbuted Alp Dikmen
 * @brief Entry point for the VCS Project. Contains Demo and Interactive modes.
 * @version 1.0
 * @date 2025-12-25
 */

// main.cpp (edit: remove run_demo() definition, keep call)
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Keep this as-is for now to minimize changes; you may later switch to <core/Repository.h>
#include <core/Repository.h>

#include <tests/demo_scenarios.h>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

std::vector<std::string> split_input(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void interactive_shell() {
    core::Repository repo;
    std::string line;

    std::cout << GREEN << "=== VCS INTERACTIVE SHELL ===" << RESET << std::endl;
    std::cout << "Type 'help' for commands.\n" << std::endl;

    while (true) {
        std::cout << YELLOW << repo.get_current_branch_name() << "> " << RESET;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        auto args = split_input(line);
        std::string command = args[0];

        try {
            if (command == "exit" || command == "quit") {
                break;
            }
            else if (command == "clear") {
                system(CLEAR_SCREEN);
            }
            else if (command == "help") {
                std::cout << "Commands:\n"
                          << "  add <file> <content>   : Stage a file (use quotes for content logic not impl in parser)\n"
                          << "                           (Tip: For this shell, content is single word or handled simply)\n"
                          << "  commit <msg> <author>  : Commit changes\n"
                          << "  log                    : Show history\n"
                          << "  branch <name>          : Create new branch\n"
                          << "  checkout <name>        : Switch branch\n"
                          << "  merge <branch>         : Merge branch into current\n"
                          << "  demo                   : Run automated demo\n"
                          << "  exit                   : Exit program\n";
            }
            else if (command == "add") {
                if (args.size() < 3) {
                    std::cout << "Usage: add <filename> <content>\n";
                    std::cout << "Interactive mode: Enter content for " << args[1] << ": ";
                    std::string content;
                    std::getline(std::cin, content);
                    repo.add(args[1], content);
                } else {
                    repo.add(args[1], args[2]);
                }
            }
            else if (command == "commit") {
                std::string msg, author;
                std::cout << "Enter commit message: ";
                std::getline(std::cin, msg);
                std::cout << "Enter author: ";
                std::getline(std::cin, author);
                repo.commit(msg, author);
            }
            else if (command == "branch") {
                if (args.size() < 2) std::cout << "Usage: branch <name>\n";
                else repo.create_branch(args[1]);
            }
            else if (command == "checkout") {
                if (args.size() < 2) std::cout << "Usage: checkout <name>\n";
                else repo.checkout(args[1]);
            }
            else if (command == "merge") {
                if (args.size() < 2) std::cout << "Usage: merge <branch_name>\n";
                else repo.merge(args[1]);
            }
            else if (command == "log") {
                repo.log();
            }
            else if (command == "demo") {
                run_demo();
            }
            else {
                std::cout << "Unknown command. Type 'help'.\n";
            }

        } catch (const std::exception& e) {
            std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "demo") {
        run_demo();
    } else {
        interactive_shell();
    }
    return 0;
}
