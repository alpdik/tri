/**
 * @file Repository.h
 * @author Umut Ertuğrul Daşgın
 * @brief The main interface for the Version Control System (Facade Pattern).
 * @version 1.0
 * @date 2025-12-25
 */

#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

#include "GraphManager.h"
#include "ReferenceManager.h"
#include "StagingArea.h"
#include "GraphAlgorithms.h"
#include "MerkleTree.h"
#include "StorageEngine.h"
#include "MergeEngine.h"
#include "../entities/File.h"

namespace core {
    class Repository {
        private:
        GraphManager graph_manager_;
        ReferenceManager reference_manager_;
        StagingArea staging_area_;

        StorageEngine storage_engine_;
        MergeEngine merge_engine_;

        std::string calculate_tree_hash() const {
            if (staging_area_.is_empty()) return "empty_tree";

            MerkleTree tree(staging_area_.get_files());

            return tree.get_root_hash();
        }

        public:
        Repository() {
            try {
                if (!reference_manager_.get_branch("master")) {
                    reference_manager_.create_branch("master", nullptr);
                }

                reference_manager_.checkout_branch("master");
            } catch (const std::exception& e) {
                std::cerr << "Initialization warning:" << e.what() << std::endl;
            }
        }

        void add(const std::string& path, const std::string& content) {
            entities::File file(path, content);
            staging_area_.add_file(file);
            std::cout << "File staged: " << path << std::endl;
        }

        std::string commit(const std::string& message, const std::string author) {
            if (staging_area_.is_empty()) {
                throw std::runtime_error("Nothing to commit (Staging area is empty).");
            }

            std::string tree_hash = calculate_tree_hash();

            data_structures::DoublyLinkedList<entities::File> commit_files;
            const auto& staged = staging_area_.get_files();

            for (auto it = staged.begin(); it != staged.end(); ++it) {
                graph_manager_.save_blob(it->get_hash(), it->get_content());

                entities::File lightweight_file(it->get_path(), "");

                lightweight_file.set_hash_manual(it->get_hash());

                commit_files.push_back(lightweight_file);
            }

            entities::Commit* parent = nullptr;
            entities::Branch* current_branch = reference_manager_.get_current_branch();

            if (current_branch) parent = current_branch->get_last_commit();

            entities::Commit* new_commit = new entities::Commit(message, author, tree_hash, commit_files, parent);

            graph_manager_.add_commit(new_commit);
            reference_manager_.update_head(new_commit);
            staging_area_.clear();

            std::cout << "[" << current_branch->get_name() << " " << new_commit->get_id().substr(0, 7) << "] " << message << std::endl;

            return new_commit->get_id();
        }
        
        void checkout(const std::string& name) {
            reference_manager_.checkout_branch(name);
            
            entities::Branch* branch = reference_manager_.get_branch(name);
            entities::Commit* commit = branch->get_last_commit();

            std::cout << "Switched to branch '" << name << "'" << std::endl;

            if (commit) {
                storage_engine_.restore_files(commit, graph_manager_);
                std::cout << "Files restored from commit " << commit->get_id().substr(0,7) << std::endl;
            }
        }

        void merge(const std::string& branch_name) {
            entities::Branch* current = reference_manager_.get_current_branch();
            entities::Branch* target = reference_manager_.get_branch(branch_name);

            if (!target) throw std::runtime_error("Branch not found: " + branch_name);

            entities::Commit* head_c = current->get_last_commit();
            entities::Commit* target_c = target->get_last_commit();

            if (!head_c || !target_c) {
                std::cout << "Nothing to merge." << std::endl;
                return;
            }

            if (head_c->get_id() == target_c->get_id()) {
                std::cout << "Already up to date." << std::endl;
                return;
            }

            std::cout << "Merging " << branch_name << " into " << current->get_name() << "..." << std::endl;

            std::string conflict_msg = "";

            auto merged_files = merge_engine_.merge_commits(head_c, target_c, graph_manager_, conflict_msg);

            staging_area_.clear();
            
            for (auto it = merged_files.begin(); it != merged_files.end(); ++it) {
                staging_area_.add_file(*it);
                
                std::string content_to_write = it->get_content();
                if (content_to_write.empty()) {
                    content_to_write = graph_manager_.get_blob_content(it->get_hash());
                }
                storage_engine_.save_file_to_disk(it->get_path(), content_to_write);
            }

            if (!conflict_msg.empty()) {
                std::cout << "MERGE CONFLICT! Fix conflicts manually." << std::endl << conflict_msg << std::endl;
            } else {
                std::string msg = "Merge branch '" + branch_name + "'";
                std::string tree_hash = calculate_tree_hash();
                
                data_structures::DoublyLinkedList<entities::File> commit_files;
                const auto& staged = staging_area_.get_files();
                
                for (auto it = staged.begin(); it != staged.end(); ++it) {
                    graph_manager_.save_blob(it->get_hash(), it->get_content());
                    entities::File lf(it->get_path(), "");
                    lf.set_hash_manual(it->get_hash());
                    commit_files.push_back(lf);
                }

                entities::Commit* merge_commit = new entities::Commit(
                    msg, "MergeUser", tree_hash, commit_files, head_c, target_c
                );

                graph_manager_.add_commit(merge_commit);
                reference_manager_.update_head(merge_commit);
                staging_area_.clear();
                
                std::cout << "Merge successful." << std::endl;
            }
        }

        void log() {
            entities::Branch* current = reference_manager_.get_current_branch();

            if (!current || !current->get_last_commit()) {
                std::cout << "No history yet" << std::endl;
                return;
            }

            auto history_stack = GraphAlgorithms::get_history_dfs(current->get_last_commit());

            std::cout << std::endl << "===== Commit History for '" << current->get_name() << "' =====" << std::endl;
            
            while (!history_stack.empty()) {
                entities::Commit* c = history_stack.top();
                history_stack.pop();

                std::cout << "Commit: " << c->get_id() << std::endl;
                std::cout << "Author: " << c->get_author() << std::endl;
                std::time_t t = c->get_time();
                std::cout << "Date:   " << std::ctime(&t); 
                std::cout << "Tree:   " << c->get_tree_hash().substr(0, 10) << "..." << std::endl;
                if (c->is_merge_commit()) {
                    std::cout << "Merge:  " << c->get_parent1()->get_id().substr(0,7) << " " << c->get_parent2()->get_id().substr(0,7) << std::endl;
                }
                std::cout << std::endl << "    " << c->get_message() << std::endl;
                std::cout << "------------------------------------------" << std::endl;
            }
        }


        void create_branch(const std::string& name) {
            entities::Branch* current = reference_manager_.get_current_branch();

            if (!current || !current->get_last_commit()) {
                throw std::runtime_error("Cannot create branch: No commits exist yet (HEAD is empty).");
            }

            reference_manager_.create_branch(name, current->get_last_commit());
            std::cout << "Branch created: " << name << std::endl;
        }

        std::string get_current_branch_name() {
            auto b = reference_manager_.get_current_branch();
            return b ? b->get_name() : "Detached";
        }

    };
}