 /**
 * @file MergeEngine.h
 * @brief Handles merging logic between two commits using a 3-way merge strategy.
 *
 * @details This file contains the definition of the MergeEngine class and
 * supporting logic required to perform commit merge operations
 * within the repository system.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */


#pragma once

#include <string>
#include <iostream> 
#include "GraphAlgorithms.h"
#include "GraphManager.h"
#include "../entities/Commit.h"
#include "../entities/File.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../data_structures/HashTable.h"

namespace core {
    /**
    * @brief Provides functionality for merging commits using a 3-way merge algorithm.
    *
    * @details The MergeEngine class compares the common base commit with the
    * "ours" and "theirs" commits to determine the final merged state
    * and to identify merge conflicts when they occur.
    */


    class MergeEngine { 
    public:
        /**
        * @brief Constructs a MergeEngine instance.
        */

        MergeEngine() = default;

        /**
        * @brief Merges two commits using a 3-way merge strategy.
        *
        * @details This class provides an abstract representation of Git-like branch and HEAD management.
        *
        * @param[in] ours Pointer to the current branch commit.
        * @param[in] theirs Pointer to the commit being merged into the current branch.
        * @param[in] graph_mgr Reference to the GraphManager used for retrieving blob contents.
        * @param[out] out_conflict_msg Accumulates human-readable conflict descriptions.
        *
        * @return A list of merged files representing the merge result.
        *
        * @pre ours != nullptr
        * @pre theirs != nullptr
        * @post Returned file list represents the merged state of the two commits.
        */

        data_structures::DoublyLinkedList<entities::File> merge_commits(
            entities::Commit* ours, 
            entities::Commit* theirs, 
            GraphManager& graph_mgr,
            std::string& out_conflict_msg) 
        {
            data_structures::DoublyLinkedList<entities::File> result_files;

            entities::Commit* base = GraphAlgorithms::find_merge_base(ours, theirs);
            
            auto map_ours   = create_file_map(ours);
            auto map_theirs = create_file_map(theirs);
            auto map_base   = create_file_map(base);

            data_structures::HashTable<std::string, bool> all_paths;
            
            auto add_paths = [&](const data_structures::DoublyLinkedList<entities::File>& list) {
                for(auto it = list.begin(); it != list.end(); ++it) {
                    all_paths.put(it->get_path(), true);
                }
            };

            add_paths(ours->get_files());
            add_paths(theirs->get_files());
            if(base) add_paths(base->get_files());

            const auto& our_files_list = ours->get_files();
            for (auto it = our_files_list.begin(); it != our_files_list.end(); ++it) {
                std::string path = it->get_path();
                std::string hash_ours = it->get_hash();
                
                std::string hash_theirs = "";
                if (map_theirs.contains(path)) hash_theirs = map_theirs.get(path);

                std::string hash_base = "";
                if (map_base.contains(path)) hash_base = map_base.get(path);

                if (hash_theirs == "") {
                    if (hash_base == "") {
                        result_files.push_back(*it);
                    } else if (hash_base == hash_ours) { } else {
                        out_conflict_msg += "CONFLICT (Modify/Delete): " + path + "\n";
                        result_files.push_back(*it);
                    }
                } 
                else if (hash_ours == hash_theirs) {
                    result_files.push_back(*it);
                }
                else {
                    if (hash_ours == hash_base) {
                        entities::File file_from_theirs(path, "");
                        file_from_theirs.set_hash_manual(hash_theirs);
                        result_files.push_back(file_from_theirs);
                    }
                    else if (hash_theirs == hash_base) {
                        result_files.push_back(*it);
                    }
                    else {
                        out_conflict_msg += "CONFLICT (Content): " + path + "\n";

                        std::string content_ours = graph_mgr.get_blob_content(hash_ours);
                        std::string content_theirs = graph_mgr.get_blob_content(hash_theirs);

                        std::string conflict_text = 
                            "<<<<<<< HEAD\n" + content_ours + "\n" +
                            "=======\n" + content_theirs + "\n" +
                            ">>>>>>> " + theirs->get_id().substr(0,7) + "\n";

                        entities::File conflict_file(path, conflict_text);
                        
                        result_files.push_back(conflict_file);
                    }
                }
            }

            const auto& their_files_list = theirs->get_files();
            for (auto it = their_files_list.begin(); it != their_files_list.end(); ++it) {
                std::string path = it->get_path();
                
                if (map_ours.contains(path)) continue;

                std::string hash_base = "";
                if (map_base.contains(path)) hash_base = map_base.get(path);

                if (hash_base == "") {
                    result_files.push_back(*it);
                } else {
                    if (it->get_hash() == hash_base) { } else {
                        out_conflict_msg += "CONFLICT (Delete/Modify): " + path + "\n";
                        result_files.push_back(*it);
                    }
                }
            }

            return result_files;
        }

    private:
        
        /**
        * @brief Creates a mapping from file paths to their content hashes for a commit.
        *
        * @details This helper function extracts all files from the given commit and stores
        * their paths and corresponding content hashes in a hash table to enable
        * fast lookup during the merge process.
        *
        * @param[in] c Pointer to the commit whose files will be mapped.
        * @return A hash table mapping file paths to content hashes.
        */

        data_structures::HashTable<std::string, std::string> create_file_map(entities::Commit* c) {
            data_structures::HashTable<std::string, std::string> map;
            if (!c) return map;

            const auto& files = c->get_files();
            for (auto it = files.begin(); it != files.end(); ++it) {
                map.put(it->get_path(), it->get_hash());
            }
            return map;
        }
    };
}