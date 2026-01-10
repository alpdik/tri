/**
 * @file Branch.h
 * @brief Represents a branch pointing to a commit.
 *
 * @details
 * This file defines the Branch class, which models a version control branch
 * by associating a branch name with the latest commit it references.
 * The class provides basic accessors and update functionality.
 *
 * @author Umut Ertuğrul Daşgın
 * @version 1.0
 * @date 2025-12-25
 * @lastModified 2026-01-10
 * @note Documentation maintained by Deniz Kayra Aydın.
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <string>
#include "Commit.h"

namespace entities {

/**
 * @brief Represents a branch in the repository.
 *
 * @details
 * A Branch stores a human-readable name and a pointer to the most recent
 * commit it refers to. It is used to track different lines of development
 * within the repository.
 */
class Branch {
private:
    std::string name_;      ///< Name of the branch
    Commit* last_commit_;   ///< Pointer to the latest commit of the branch

public:
    /**
     * @brief Constructs a branch with a given name and initial commit.
     *
     * @param[in] name Name of the branch.
     * @param[in] last_commit Pointer to the initial commit.
     */
    Branch(const std::string& name, Commit* last_commit)
        : name_(name), last_commit_(last_commit) {}

    /**
     * @brief Constructs an empty branch.
     */
    Branch() : name_(""), last_commit_(nullptr) {}

    /**
     * @brief Returns the name of the branch.
     *
     * @return Reference to the branch name.
     */
    const std::string& get_name() const { return name_; }

    /**
     * @brief Returns the latest commit referenced by the branch.
     *
     * @return Pointer to the last commit.
     */
    Commit* get_last_commit() const { return last_commit_; }

    /**
     * @brief Updates the branch to point to a new commit.
     *
     * @param[in] new_last_commit Pointer to the new latest commit.
     */
    void set_last_commit(Commit* new_last_commit) {
        last_commit_ = new_last_commit;
    }
};

} // namespace entities
