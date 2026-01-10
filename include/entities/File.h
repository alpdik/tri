/**
 * @file File.h
 * @brief Represents a tracked file in the version control system.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın / Ömer Kağan Zafer.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */


#pragma once

#include <string>
#include <functional>
#include <sstream>
#include <iomanip>

namespace entities {

/**
 * @brief File snapshot with content-based hashing.
 * * Stores file path and content
 * * Computes a deterministic content hash
 */
class File {
private:
    std::string path_;
    std::string content_;
    std::string hash_;

    /**
     * @brief Computes the file hash.
     * @return Deterministic hash value.
     */
    std::string calculate_hash() const {
        std::hash<std::string> hasher;
        std::size_t hash_val = hasher(content_ + path_);

        std::stringstream ss;
        ss << std::hex << std::setw(16) << std::setfill('0') << hash_val;
        return ss.str();
    }

public:
    /**
     * @brief Creates a file with path and content.
     * @param path File path.
     * @param content File content.
     */
    File(const std::string& path, const std::string& content)
        : path_(path), content_(content) {
        hash_ = calculate_hash();
    }

    /**
     * @brief Creates an empty file object.
     */
    File() : path_(""), content_(""), hash_("") {}

    /**
     * @brief Returns the file path.
     */
    const std::string& get_path() const { return path_; }

    /**
     * @brief Returns the file content.
     */
    const std::string& get_content() const { return content_; }

    /**
     * @brief Returns the file hash.
     */
    const std::string& get_hash() const { return hash_; }

    /**
     * @brief Updates file content and recomputes hash.
     * @param new_content New file content.
     */
    void set_content(const std::string& new_content) {
        content_ = new_content;
        hash_ = calculate_hash();
    }

    /**
     * @brief Sets the hash value manually.
     * @param hash Hash value to assign.
     */
    void set_hash_manual(const std::string& hash) { hash_ = hash; }

    /**
     * @brief Compares files by hash value.
     * @param other File to compare.
     * @return True if hashes match.
     */
    bool operator==(const File& other) const { return hash_ == other.hash_; }
};

} // namespace entities
