/**
 * @file Branch.h
 * @author Umut Ertuğrul Daşgın
 * @brief Branch
 * @version 1.0
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <string>
#include "Commit.h"

namespace entities {
    class Branch {
    private:
        std::string name_;
        Commit* last_commit_;

    public:
        Branch(const std::string& name, Commit* last_commit) : name_(name), last_commit_(last_commit) {}
        Branch() : name_(""), last_commit_(nullptr) {}

        const std::string& get_name() const { return name_; }

        Commit* get_last_commit() const { return last_commit_; }

        void set_last_commit(Commit* new_last_commit) { 
            last_commit_ = new_last_commit;
        }
    };
}