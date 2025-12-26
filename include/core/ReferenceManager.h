/**
 * @file ReferenceManager.h
 * @author Umut Ertuğrul Daşgın
 * @brief Manages Branches and the HEAD reference.
 * @version 1.0
 * @date 2025-12-25
 */

#pragma once

#include <string>
#include <stdexcept>
#include "../data_structures/HashTable.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/Branch.h"

namespace core {
    class ReferenceManager {
        private:
        data_structures::HashTable<std::string, entities::Branch*> branches_;
        data_structures::DoublyLinkedList<entities::Branch*> managed_branches_;
        entities::Branch* current_branch_;

        public:
        ReferenceManager() : current_branch_(nullptr) {}
        ~ReferenceManager() {
            for (auto it = managed_branches_.begin(); it != managed_branches_.end(); ++it) {
                delete *it;
            }
        }

        void create_branch(const std::string& name, entities::Commit* target_commit) {
            if (branches_.contains(name)) {
                throw std::runtime_error("Branch already exists: " + name);
            }

            entities::Branch* new_branch = new entities::Branch(name, target_commit);

            branches_.put(name, new_branch);
            managed_branches_.push_back(new_branch);
        }

        void checkout_branch(const std::string name) {
            if (!branches_.contains(name)) {
                throw std::runtime_error("Branch not found: " + name);
            }

            current_branch_ = branches_.get(name);
        }

        void update_head(entities::Commit* new_commit) {
            if (current_branch_) {
                current_branch_->set_last_commit(new_commit);
            } else {
                throw std::runtime_error("HEAD is detached (No active branch)");
            }
        }

        entities::Branch* get_current_branch() const { return current_branch_; }

        entities::Branch* get_branch(const std::string& name) {
            if (branches_.contains(name)) {
                return branches_.get(name);
            }

            return nullptr;
        }

        const data_structures::DoublyLinkedList<entities::Branch*>& get_all_branches() const { return managed_branches_; }

    };
}
