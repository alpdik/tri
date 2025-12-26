/**
 * @file HashTable.h
 * @author Umut Ertuğrul Daşgın
 * @brief 
 * @version 1.0
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <functional>
#include <string>

namespace data_structures {
    template <typename K, typename V>
    class HashTable {
    private:
        struct HashEntry {
            K key;
            V value;
            HashEntry* next;

            explicit HashEntry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
            explicit HashEntry(K&& k, V&& v) : key(std::move(k)), value(std::move(v)), next(nullptr) {}
        };
        
        HashEntry** buckets_;
        std::size_t capacity_;
        std::size_t size_;
        const float load_factor_threshold_ = 0.75f;

        std::size_t hash_func(const K& key) const {
            return std::hash<K>{}(key) % capacity_;
        }

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
                    size_++;
                }
            }
        }

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
        explicit HashTable(std::size_t cap = 101) : capacity_(cap), size_(0) {
            buckets_ = new HashEntry*[capacity_];
            for (std::size_t i = 0; i < capacity_; ++i) buckets_[i] = nullptr;
        }

        ~HashTable() {
            clear_internal();
            delete[] buckets_;
        }

        HashTable(const HashTable& other) : buckets_(nullptr), capacity_(0), size_(0) {
            copy_from(other);
        }
        
        HashTable& operator=(const HashTable& other) {
            if (this == &other) return *this;
            clear_internal();
            delete[] buckets_;
            copy_from(other);
            return *this;
        }

        HashTable(HashTable&& other) noexcept : buckets_(other.buckets_), capacity_(other.capacity_), size_(other.size_) {
            other.buckets_ = nullptr;
            other.capacity_ = 0;
            other.size_ = 0;
        }

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

        std::size_t size() const noexcept { return size_; }

        bool empty() const noexcept { return (size_ == 0); }

        void clear() { clear_internal(); }

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
            size_++;
        }

        void put(K&& key, V&& val) {
            if ((float)(size_ + 1) / capacity_ > load_factor_threshold_) {
                rehash(capacity_ * 2);
            }

            std::size_t idx = hash_func(key);
            HashEntry* entry = buckets_[idx];

            while (entry) {
                if (entry->key == key) {
                    entry->value = std::move(val);
                    return;
                }

                entry = entry->next;
            }

            HashEntry* new_node = new HashEntry(std::move(key), std::move(val));
            new_node->next = buckets_[idx];
            buckets_[idx] = new_node;
            size_++;
        }

        V& get(const K& key) {
            std::size_t idx = hash_func(key);
            HashEntry* entry = buckets_[idx];
            
            while (entry) {
                if (entry->key == key) return entry->value;
                entry = entry->next;
            }

            throw std::runtime_error("HashTable::t key not found");
        }

        const V& get(const K& key) const {
            std::size_t idx = hash_func(key);
            HashEntry* entry = buckets_[idx];
            
            while (entry) {
                if (entry->key == key) return entry->value;
                entry = entry->next;
            }

            throw std::runtime_error("HashTable::get key not found");
        }

        bool contains(const K& key) const {
            std::size_t idx = hash_func(key);
            HashEntry* entry = buckets_[idx];

            while (entry) {
                if (entry->key == key) return true;
                entry = entry->next;
            }

            return false;
        }

        void remove(const K& key) {
            std::size_t idx = hash_func(key);
            HashEntry* entry = buckets_[idx];
            HashEntry* prev = nullptr;

            while (entry) {
                if (entry->key == key) {
                    if (prev) {
                        prev->next = entry->next;
                    } else {
                        buckets_[idx] = entry->next;
                    }
                    
                    delete entry;
                    size_--;
                    return;
                }

                prev = entry;
                entry = entry->next;
            }
        }
    };
}