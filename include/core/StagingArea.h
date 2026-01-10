/**
 * @file StagingArea.h
 * @brief Manages files staged for the next commit.
 *
 * @details The StagingArea class represents an intermediate
 * area where files are collected before being committed. It supports adding,
 * updating, removing, and clearing staged files.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 *
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <string>
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/File.h"

namespace core {

/**
 * @brief Represents a staging area for files awaiting commit.
 *
 * @details The StagingArea class temporarily stores file objects that are marked
 * for inclusion in the next commit. If a file with the same path is added
 * multiple times, the staged version is updated accordingly.
 */
class StagingArea {
private:
    data_structures::DoublyLinkedList<entities::File> staged_files_;

public:
    /**
     * @brief Constructs an empty StagingArea.
     */
    StagingArea() = default;

    /**
     * @brief Adds a file to the staging area.
     *
     * @details If a file with the same path already exists in the staging area,
     * it is replaced with the new version.
     *
     * @param[in] file File to be staged.
     */
    void add_file(const entities::File& file) {
        for (auto it = staged_files_.begin(); it != staged_files_.end(); ++it) {
            if (it->get_path() == file.get_path()) {
                *it = file;
                return;
            }
        }
        staged_files_.push_back(file);
    }

    /**
     * @brief Removes a file from the staging area by path.
     *
     * @param[in] file_path Path of the file to be removed.
     */
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

    /**
     * @brief Retrieves all staged files.
     *
     * @return Constant reference to the list of staged files.
     */
    const data_structures::DoublyLinkedList<entities::File>& get_files() const {
        return staged_files_;
    }

    /**
     * @brief Clears all staged files.
     */
    void clear() {
        staged_files_.clear();
    }

    /**
     * @brief Checks whether the staging area is empty.
     *
     * @return true if no files are staged, false otherwise.
     */
    bool is_empty() const {
        return staged_files_.empty();
    }
};

} // namespace core
