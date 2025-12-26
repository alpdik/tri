/**
 * @file Commit.h
 * @author Umut Ertuğrul Daşgın
 * @brief Commit
 * @version 1.0
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
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
        Commit(const std::string& message, const std::string& author, const std::string& tree_hash, data_structures::DoublyLinkedList<File>& files, Commit* p1 = nullptr, Commit* p2 = nullptr) : message_(message), author_(author), tree_hash_(tree_hash), files_(files), parent1_(p1), parent2_(p2) {
            time_ = std::time(nullptr);
            id_ = calculate_id();               
        }

        const std::string& get_id() const { return id_; }
        const std::string& get_message() const { return message_; }
        const std::string& get_author() const { return author_; }
        const std::time_t& get_time() const { return time_; }
        const std::string& get_tree_hash() const { return tree_hash_; }

        const data_structures::DoublyLinkedList<File>& get_files() const { return files_; }

        Commit* get_parent1() const { return parent1_; }
        Commit* get_parent2() const { return parent2_; }

        bool is_merge_commit() const { return parent1_ && parent2_; }
    };
}
