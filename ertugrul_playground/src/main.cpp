/**
 * @file main.cpp
 * @author Umut Ertuğrul Daşgın
 * @brief Entry point for the VCS Project. Contains Demo and Interactive modes.
 * @version 1.0
 * @date 2025-12-25
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Core Repository (Facade)
#include "include/core/Repository.h"

// Ekran temizleme makrosu (Cross-platform)
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

// Renkli çıktılar için (Opsiyonel, desteklenmiyorsa normal text görünür)
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"

/**
 * @brief Kullanıcı girdisini boşluklara göre böler.
 */
std::vector<std::string> split_input(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * @brief Otomatik Test Senaryosu
 * Branch açma, dosya değiştirme, merge ve conflict durumlarını gösterir.
 */
void run_demo() {
    std::cout << GREEN << "=== AUTOMATED DEMO SCENARIO STARTED ===" << RESET << std::endl;
    
    try {
        core::Repository repo; // Otomatik olarak "master" branch oluşturur.

        std::cout << CYAN << "\n[STEP 1] Initial Commit on Master" << RESET << std::endl;
        repo.add("main.cpp", "int main() { return 0; }");
        repo.add("readme.txt", "This is a VCS project.");
        repo.commit("Initial commit", "Umut");

        std::cout << CYAN << "\n[STEP 2] Create and Switch to 'feature-login'" << RESET << std::endl;
        repo.create_branch("feature-login");
        repo.checkout("feature-login");

        std::cout << CYAN << "\n[STEP 3] Work on Feature Branch" << RESET << std::endl;
        repo.add("login.cpp", "void login() { /* logic */ }");
        repo.add("main.cpp", "int main() { login(); return 0; }"); // main.cpp değişti
        repo.commit("Added login feature", "Alp");

        std::cout << CYAN << "\n[STEP 4] Switch back to Master and Create Conflict" << RESET << std::endl;
        repo.checkout("master");
        // Master'da da main.cpp dosyasını farklı şekilde değiştiriyoruz
        repo.add("main.cpp", "int main() { std::cout << 'Hello'; return 0; }");
        repo.commit("Changed main on master", "Umut");

        std::cout << CYAN << "\n[STEP 5] Merge 'feature-login' into 'master' (Expect Conflict)" << RESET << std::endl;
        // Burada main.cpp hem master'da hem feature'da değiştiği için conflict çıkmalı.
        repo.merge("feature-login");

        std::cout << CYAN << "\n[STEP 6] Show History" << RESET << std::endl;
        repo.log();

    } catch (const std::exception& e) {
        std::cerr << RED << "Demo Error: " << e.what() << RESET << std::endl;
    }

    std::cout << GREEN << "\n=== DEMO FINISHED ===" << RESET << std::endl;
}

/**
 * @brief İnteraktif Komut Satırı (REPL)
 */
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
                    // Basit parser: 3. argümandan sonrasını içerik kabul et (tek kelime ise)
                    // Çok kelimeli içerik için yukarıdaki interactive input daha güvenli
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
    // Eğer program "demo" argümanı ile başlatılırsa direkt demoyu çalıştır
    if (argc > 1 && std::string(argv[1]) == "demo") {
        run_demo();
    } else {
        interactive_shell();
    }

    return 0;
}