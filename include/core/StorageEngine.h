/**
 * @file StorageEngine.h
 * @author Umut Ertuğrul Daşgın
 * @brief Handles physical file I/O operations (Restoring files to disk).
 * @version 1.0
 * @date 2025-12-25
 */

#pragma once

#include <fstream>
#include <iostream>
#include <filesystem>
#include "../entities/Commit.h"
#include "../entities/File.h"
#include "GraphManager.h"

namespace fs = std::filesystem;

namespace core {
    class StorageEngine {
        public:
        StorageEngine() = default;

        void restore_files(entities::Commit* commit, GraphManager& graph_manager) {
            if (!commit) return;
            
            const auto& files = commit->get_files();

            if (files.empty()) return; 

            for (auto it = files.begin(); it != files.end(); ++it) {
                std::string hash = it->get_hash();
                std::string content = graph_manager.get_blob_content(hash);

                save_file_to_disk(it->get_path(), content);
            }
        }

        void save_file_to_disk(const std::string& path, const std::string& content) {
            fs::path file_path(path);
            if (file_path.has_parent_path()) {
                fs::create_directories(file_path.parent_path());
            }

            std::ofstream out_file(path, std::ios::trunc);
            if (out_file.is_open()) {
                out_file << content;
                out_file.close();

                std::cout << "Restored: " << path << std::endl;
            } else {
                std::cerr << "Error: Could not write to file " << path << std::endl;
            }
        }
    };
}
