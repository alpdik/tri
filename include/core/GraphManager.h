/**
 * @file GraphManager.h
 * @author Umut Ertuğrul Daşgın
 * @brief Manages the storage and lifecycle of commits.
 * @version 1.0
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <string>
#include "../data_structures/HashTable.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/Commit.h"

namespace core {
    class GraphManager {
        private:
        data_structures::HashTable<std::string, entities::Commit*> commit_map_;
        data_structures::DoublyLinkedList<entities::Commit*> managed_commits_;

        data_structures::HashTable<std::string, std::string> blob_storage_;
        public:
        GraphManager() = default;

        ~GraphManager() {
            for (auto it = managed_commits_.begin(); it != managed_commits_.end(); ++it) {
                delete *it;
            }
        }

        void add_commit(entities::Commit* commit) {
            commit_map_.put(commit->get_id(), commit);
            managed_commits_.push_back(commit);
        }

        bool contains(const std::string& id) const { return commit_map_.contains(id); }

        entities::Commit* get_commit(const std::string& id) {
            if (contains(id)) {
                return commit_map_.get(id);
            }

            return nullptr;
        }

        void save_blob(const std::string& hash, const std::string& content) {
            if (!blob_storage_.contains(hash)) {
                blob_storage_.put(hash, content);
            }
        }

        std::string get_blob_content(const std::string& hash) {
            if (blob_storage_.contains(hash)) {
                return blob_storage_.get(hash);
            }

            return "";
        }
    };
}
