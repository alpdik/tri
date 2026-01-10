/**
 * @file StorageEngine.h
 * @brief Handles persistent storage of file data.
 * @details Manages writing files to disk and restoring
 * tracked content from stored blob data.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın / Ömer Kağan Zafer.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
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

/**
 * @brief Manages disk-level file restoration and persistence.
 * * Supports restoring tracked files from commits
 * * Ensures required directories exist before writing files
 */
class StorageEngine {
public:
    StorageEngine() = default;

    /**
     * @brief Restores all files referenced by a commit.
     * @param commit Pointer to the commit object.
     * @param graph_manager Provides access to stored blob data.
     */
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

    /**
     * @brief Writes file content to disk at the given path.
     * @param path Target file path.
     * @param content File data to write.
     */
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

} // namespace core
