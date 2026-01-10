 /**
 * @file ReferenceManager.h
 * @brief Manages branches and the HEAD reference.
 *
 * @details ReferenceManager is responsible for managing branch objects and 
 * tracking the currently checked-out branch
 * (HEAD). It provides functionality for creating branches, switching
 * between them, and updating the HEAD reference as new commits are created.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */


#pragma once

#include <string>
#include <stdexcept>
#include "../data_structures/HashTable.h"
#include "../data_structures/DoublyLinkedList.h"
#include "../entities/Branch.h"

namespace core {
    /**
    * @brief Manages branch references and the current HEAD state.
    *
    * @details Branches are stored in a hash table to be able to do fast lookup by name, meanwhile a
    * doubly linked list is used to manage branch lifetimes and be sure about 
    * memory cleanup for destruction.
    *
    * This class models the reference-handling behavior of a version control
    * system, similar to branch and HEAD management in Git.
    */

    class ReferenceManager {
        private:
        data_structures::HashTable<std::string, entities::Branch*> branches_;
        data_structures::DoublyLinkedList<entities::Branch*> managed_branches_;
        entities::Branch* current_branch_;

        public:
        /**
        * @brief Constructs a ReferenceManager with no active branch.
        *
        * Initially, the HEAD reference is detached and no branch is selected.
        */  

        ReferenceManager() : current_branch_(nullptr) {}
        
        /**
        * @brief Destroys the ReferenceManager and releases all managed branches.
        *
        * @details All Branch objects owned by the manager are deallocated to prevent
        * memory leaks.
        */

        ~ReferenceManager() {
            for (auto it = managed_branches_.begin(); it != managed_branches_.end(); ++it) {
                delete *it;
            }
        }
        /**
        * @brief Creates a new branch pointing to the given commit.
        *
        * @details The function crates a branch with the specified name given and initialized. 
        * The branch points to the provided target commit.
        *
        * @param[in] name Name of the new branch.
        * @param[in] target_commit Commit that the new branch will point to.
        *
        * @throws std::runtime_error if a branch with the same name already exists.
        *
        * @pre target_commit != nullptr
        */

        void create_branch(const std::string& name, entities::Commit* target_commit) {
            if (branches_.contains(name)) {
                throw std::runtime_error("Branch already exists: " + name);
            }

            entities::Branch* new_branch = new entities::Branch(name, target_commit);

            branches_.put(name, new_branch);
            managed_branches_.push_back(new_branch);
        }
        /**
        * @brief Changes the current HEAD to the specified branch.
        *
        * @details After a successful check the specified branch becomes the active
        * branch and HEAD points to it.
        *
        * @param[in] name Name of the branch to check out.
        *
        * @throws std::runtime_error with a message "Branch not found" if the branch does not exist.
        */

        void checkout_branch(const std::string name) {
            if (!branches_.contains(name)) {
                throw std::runtime_error("Branch not found: " + name);
            }

            current_branch_ = branches_.get(name);
        }
        /**
        * @brief Updates the HEAD reference to point to a new commit.
        *
        * @details The currently active branch is updated to reference the given commit.
        *
        * @param[in] new_commit Commit that HEAD will point to.
        *
        * @throws std::runtime_error if no branch is currently checked out.
        *
        * @pre new_commit != nullptr
        */

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
