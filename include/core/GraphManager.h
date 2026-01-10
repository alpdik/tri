/**
 * @file GraphManager.h
 * @brief Manages the storage and lifecycle of commit objects and blob data.
 * @details Responsible for owning dynamically allocated commits, indexing them for 
 * fast retrieval, and handling deduplicated content storage.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */

#pragma once

#include <string>
#include "../data_structures/HashTable.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/Commit.h"

namespace core {

/**
 * @brief Centralized controller for commit lifecycle and content deduplication.
 * * Implements two core architectural patterns:
 * 1. Object Ownership: Managed via DoublyLinkedList to ensure safe memory deallocation.
 * 2. Content-Addressable Storage (CAS): Maps unique content hashes to data strings.
 */
class GraphManager {
private:
    data_structures::HashTable<std::string, entities::Commit*> commit_map_;
    data_structures::DoublyLinkedList<entities::Commit*> managed_commits_;
    data_structures::HashTable<std::string, std::string> blob_storage_;

public:
    GraphManager() = default;

    /**
     * @brief Destructor ensures all managed Commit objects are properly deallocated.
     */
    ~GraphManager() {
        for (auto it = managed_commits_.begin(); it != managed_commits_.end(); ++it) {
            delete *it;
        }
    }

    /**
     * @brief Takes ownership of a dynamically allocated Commit.
     * @param commit Pointer to the commit object.
     * @pre commit must not be nullptr.
     */
    void add_commit(entities::Commit* commit) {
        if (!commit) return; 
        commit_map_.put(commit->get_id(), commit);
        managed_commits_.push_back(commit);
    }

    /**
     * @brief Retrieves a commit using its unique identifier.
     * @param id The unique hash of the commit.
     * @return Pointer to the commit if found; otherwise @c nullptr.
     */
    entities::Commit* get_commit(const std::string& id) {
        return commit_map_.contains(id) ? commit_map_.get(id) : nullptr;
    }

    /**
     * @brief Persists data using its unique content hash.
     * @note If the hash already exists, the storage operation is skipped (Deduplication).
     * @param hash Unique key representing the content.
     * @param content The actual data to store.
     */
    void save_blob(const std::string& hash, const std::string& content) {
        if (!blob_storage_.contains(hash)) {
            blob_storage_.put(hash, content);
        }
    }

    /**
     * @brief Retrieves stored blob content via its hash.
     * @param hash Unique key for the content.
     * @return The data string if found; otherwise an empty string.
     */
    std::string get_blob_content(const std::string& hash) const {
        return blob_storage_.contains(hash) ? blob_storage_.get(hash) : "";
    }
};

} // namespace core
