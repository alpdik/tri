/**
 * @file Commit.h
 * @brief Represents a commit object in the version control system.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın / Ömer Kağan Zafer.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */


#pragma once

#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <functional>
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/File.h"

namespace entities {

/**
 * @brief Immutable commit record.
 * * Stores metadata, file snapshots, and parent links
 * * Supports merge commits with two parents
 */
class Commit {
private:
    std::string id_;
    std::string message_;
    std::string author_;
    std::time_t time_;
    std::string tree_hash_;
    data_structures::DoublyLinkedList<File> files_;
    Commit* parent1_;
    Commit* parent2_;

    /**
     * @brief Computes the commit identifier.
     * @return Deterministic commit hash.
     */
    std::string calculate_id() const {
        std::stringstream ss;

        ss << message_ << author_ << time_ << tree_hash_;

        if (parent1_) ss << parent1_->get_id();
        if (parent2_) ss << parent2_->get_id();

        std::hash<std::string> hasher;
        std::size_t hash_val = hasher(ss.str());

        std::stringstream out;
        out << std::hex << std::setw(16) << std::setfill('0') << hash_val;

        return out.str();
    }

public:
    /**
     * @brief Creates a new commit instance.
     * @param message Commit message.
     * @param author Commit author.
     * @param tree_hash Root tree hash.
     * @param files Snapshot of tracked files.
     * @param p1 First parent commit.
     * @param p2 Second parent commit.
     */
    Commit(const std::string& message,
           const std::string& author,
           const std::string& tree_hash,
           data_structures::DoublyLinkedList<File>& files,
           Commit* p1 = nullptr,
           Commit* p2 = nullptr)
        : message_(message),
          author_(author),
          tree_hash_(tree_hash),
          files_(files),
          parent1_(p1),
          parent2_(p2) {

        time_ = std::time(nullptr);
        id_ = calculate_id();
    }

    /**
     * @brief Returns the commit identifier.
     */
    const std::string& get_id() const { return id_; }

    /**
     * @brief Returns the commit message.
     */
    const std::string& get_message() const { return message_; }

    /**
     * @brief Returns the commit author.
     */
    const std::string& get_author() const { return author_; }

    /**
     * @brief Returns the commit timestamp.
     */
    const std::time_t& get_time() const { return time_; }

    /**
     * @brief Returns the root tree hash.
     */
    const std::string& get_tree_hash() const { return tree_hash_; }

    /**
     * @brief Returns tracked files in this commit.
     */
    const data_structures::DoublyLinkedList<File>& get_files() const { return files_; }

    /**
     * @brief Returns the first parent commit.
     */
    Commit* get_parent1() const { return parent1_; }

    /**
     * @brief Returns the second parent commit.
     */
    Commit* get_parent2() const { return parent2_; }

    /**
     * @brief Checks whether this is a merge commit.
     * @return True if two parents exist.
     */
    bool is_merge_commit() const { return parent1_ && parent2_; }
};

} // namespace entities
