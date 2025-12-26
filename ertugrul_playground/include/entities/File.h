/**
 * @file File.h
 * @author Umut Ertuğrul Daşgın
 * @brief File.h
 * @version 1.0
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <string>
#include <functional>
#include <sstream>
#include <iomanip>

namespace entities {
    class File {
    private:
        std::string path_;
        std::string content_;
        std::string hash_;

        std::string calculate_hash() const {
            std::hash<std::string> hasher;
            std::size_t hash_val = hasher(content_ + path_);

            std::stringstream ss;
            ss << std::hex << std::setw(16) << std::setfill('0') << hash_val;
            return ss.str();
        }

    public:
        File(const std::string& path, const std::string& content) : path_(path), content_(content) {
            hash_ = calculate_hash();
        }

        File() : path_(""), content_(""), hash_("") {}

        const std::string& get_path() const { return path_; }
        const std::string& get_content() const { return content_; }
        const std::string& get_hash() const { return hash_; }

        void set_content(const std::string& new_content) {
            content_ = new_content;
            hash_ = calculate_hash();
        }

        void set_hash_manual(const std::string& hash) { hash_ = hash; }

        bool operator==(const File& other) const { return hash_ == other.hash_; }
    };
}
