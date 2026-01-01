/**
 * @file StagingArea.h
 * @author Umut Ertuğrul Daşgın
 * @brief Manages files prepared for the next commit.
 * @version 1.0
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <string>
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/File.h"

namespace core {
    class StagingArea {
    private:
        data_structures::DoublyLinkedList<entities::File> staged_files_;

    public:
        StagingArea() = default;

        void add_file(const entities::File& file) {
            for (auto it = staged_files_.begin(); it != staged_files_.end(); ++it) {
                if (it->get_path() == file.get_path()) {
                    *it = file;
                    return;
                }
            }

            staged_files_.push_back(file);
        }

        void remove_file(const std::string& file_path) {
            std::size_t idx = 0;

            for (auto it = staged_files_.begin(); it != staged_files_.end(); ++it) {
                if (it->get_path() == file_path) {
                    staged_files_.erase(idx);
                    return;
                }

                ++idx;
            }
        }

        const data_structures::DoublyLinkedList<entities::File>& get_files() const { return staged_files_; }

        void clear() { staged_files_.clear(); }

        bool is_empty() const { return staged_files_.empty(); }
    };
}
