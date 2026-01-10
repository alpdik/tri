/**
 * @file HashTable.h
 * @brief A generic hash table container implementation.
 *
 * @details
 * This file defines a templated HashTable class that maps keys to values
 * using hashing with separate chaining. The container supports insertion,
 * lookup, removal, and automatic resizing based on load factor.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın and Ömer Kağan Zafer.
 * @version 1.1
 * @date 2025-12-24
 * @lastModified 2026-01-10
 * @copyright Copyright (c) 2025
 */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <functional>
#include <string>

namespace data_structures {

/**
 * @brief A templated hash table with separate chaining.
 *
 * @details
 * The HashTable class stores key–value pairs in buckets indexed by a hash
 * function. Collisions are handled using linked lists, and the table is
 * automatically rehashed when the load factor exceeds a predefined threshold.
 *
 * @tparam K Type of keys.
 * @tparam V Type of values.
 */
template <typename K, typename V>
class HashTable {
private:
    /**
     * @brief Internal hash table entry.
     *
     * @details
     * Represents a single key–value pair stored in the table. Entries within
     * the same bucket are linked together to resolve collisions.
     */
    struct HashEntry {
        K key;
        V value;
        HashEntry* next;

        explicit HashEntry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
        explicit HashEntry(K&& k, V&& v) : key(std::move(k)), value(std::move(v)), next(nullptr) {}
    };

    HashEntry** buckets_;          ///< Array of bucket pointers
    std::size_t capacity_;         ///< Number of buckets
    std::size_t size_;             ///< Number of stored elements
    const float load_factor_threshold_ = 0.75f; ///< Rehash threshold

    /**
     * @brief Computes the hash index for a given key.
     *
     * @details
     * Uses the standard library hash function and maps the result into
     * the valid bucket range.
     *
     * @param[in] key Key to hash.
     * @return Index of the corresponding bucket.
     */
    std::size_t hash_func(const K& key) const {
        return std::hash<K>{}(key) % capacity_;
    }

    /**
     * @brief Clears all entries from the hash table.
     *
     * @details
     * Deallocates all internal nodes while keeping the bucket array intact.
     */
    void clear_internal() noexcept {
        for (std::size_t i = 0; i < capacity_; ++i) {
            HashEntry* entry = buckets_[i];
            while (entry) {
                HashEntry* temp = entry;
                entry = entry->next;
                delete temp;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }

    /**
     * @brief Copies the contents of another hash table.
     *
     * @details
     * Performs a deep copy of all buckets and chained entries.
     *
     * @param[in] other HashTable to copy from.
     */
    void copy_from(const HashTable& other) {
        buckets_ = new HashEntry*[other.capacity_];
        capacity_ = other.capacity_;
        size_ = 0;

        for (std::size_t i = 0; i < capacity_; ++i) buckets_[i] = nullptr;

        for (std::size_t i = 0; i < other.capacity_; ++i) {
            HashEntry* curr = other.buckets_[i];
            HashEntry** tail_ptr = &buckets_[i];

            while (curr) {
                *tail_ptr = new HashEntry(curr->key, curr->value);
                tail_ptr = &((*tail_ptr)->next);
                curr = curr->next;
                ++size_;
            }
        }
    }

    /**
     * @brief Rehashes the table to a new capacity.
     *
     * @details
     * Redistributes all existing entries into a new bucket array
     * according to the updated capacity.
     *
     * @param[in] new_capacity New number of buckets.
     */
    void rehash(std::size_t new_capacity) {
        HashEntry** new_buckets = new HashEntry*[new_capacity];
        for (std::size_t i = 0; i < new_capacity; ++i) new_buckets[i] = nullptr;

        for (std::size_t i = 0; i < capacity_; ++i) {
            HashEntry* entry = buckets_[i];
            while (entry) {
                HashEntry* next = entry->next;
                std::size_t new_idx = std::hash<K>{}(entry->key) % new_capacity;
                entry->next = new_buckets[new_idx];
                new_buckets[new_idx] = entry;
                entry = next;
            }
        }

        delete[] buckets_;
        buckets_ = new_buckets;
        capacity_ = new_capacity;
    }

public:
    /**
     * @brief Constructs a hash table with a given initial capacity.
     *
     * @param[in] cap Initial number of buckets.
     */
    explicit HashTable(std::size_t cap = 101) : capacity_(cap), size_(0) {
        buckets_ = new HashEntry*[capacity_];
        for (std::size_t i = 0; i < capacity_; ++i) buckets_[i] = nullptr;
    }

    /**
     * @brief Destroys the hash table and releases all allocated memory.
     */
    ~HashTable() {
        clear_internal();
        delete[] buckets_;
    }

    /**
     * @brief Copy constructor.
     */
    HashTable(const HashTable& other) : buckets_(nullptr), capacity_(0), size_(0) {
        copy_from(other);
    }

    /**
     * @brief Copy assignment operator.
     */
    HashTable& operator=(const HashTable& other) {
        if (this == &other) return *this;
        clear_internal();
        delete[] buckets_;
        copy_from(other);
        return *this;
    }

    /**
     * @brief Move constructor.
     */
    HashTable(HashTable&& other) noexcept
        : buckets_(other.buckets_), capacity_(other.capacity_), size_(other.size_) {
        other.buckets_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }

    /**
     * @brief Move assignment operator.
     */
    HashTable& operator=(HashTable&& other) noexcept {
        if (this == &other) return *this;
        clear_internal();
        delete[] buckets_;
        buckets_ = other.buckets_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        other.buckets_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        return *this;
    }

    /**
     * @brief Returns the number of stored elements.
     */
    std::size_t size() const noexcept { return size_; }

    /**
     * @brief Checks whether the hash table is empty.
     */
    bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief Removes all elements from the table.
     */
    void clear() { clear_internal(); }

    /**
     * @brief Inserts or updates a key–value pair.
     *
     * @param[in] key Key to insert.
     * @param[in] val Value associated with the key.
     */
    void put(const K& key, const V& val) {
        if ((float)(size_ + 1) / capacity_ > load_factor_threshold_) {
            rehash(capacity_ * 2);
        }

        std::size_t idx = hash_func(key);
        HashEntry* entry = buckets_[idx];

        while (entry) {
            if (entry->key == key) {
                entry->value = val;
                return;
            }
            entry = entry->next;
        }

        HashEntry* new_node = new HashEntry(key, val);
        new_node->next = buckets_[idx];
        buckets_[idx] = new_node;
        ++size_;
    }

    /**
     * @brief Retrieves the value associated with a key.
     *
     * @param[in] key Key to look up.
     * @return Reference to the associated value.
     *
     * @throws std::runtime_error if the key is not found.
     */
    V& get(const K& key) {
        std::size_t idx = hash_func(key);
        HashEntry* entry = buckets_[idx];
        while (entry) {
            if (entry->key == key) return entry->value;
            entry = entry->next;
        }
        throw std::runtime_error("HashTable::get key not found");
    }

    /**
     * @brief Checks whether a key exists in the table.
     *
     * @param[in] key Key to check.
     * @return true if the key exists, false otherwise.
     */
    bool contains(const K& key) const {
        std::size_t idx = hash_func(key);
        HashEntry* entry = buckets_[idx];
        while (entry) {
            if (entry->key == key) return true;
            entry = entry->next;
        }
        return false;
    }

    /**
     * @brief Removes a key–value pair from the table.
     *
     * @param[in] key Key to remove.
     */
    void remove(const K& key) {
        std::size_t idx = hash_func(key);
        HashEntry* entry = buckets_[idx];
        HashEntry* prev = nullptr;

        while (entry) {
            if (entry->key == key) {
                if (prev) prev->next = entry->next;
                else buckets_[idx] = entry->next;
                delete entry;
                --size_;
                return;
            }
            prev = entry;
            entry = entry->next;
        }
    }
};

} // namespace data_structures
