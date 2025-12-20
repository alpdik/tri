/**
 * @file DoublyLinkedList.h
 * @author Umut Ertuğrul Daşgın
 * @brief A pointer based vector-like container implementation as a doubly linked list.
 * @version 0.1
 * @date 2025-12-14
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace data_structures {
    template <typename T>
    class DoublyLinkedList {
    private:
        struct Node {
            T value;
            Node* prev;
            Node* next;

            explicit Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
            explicit Node(T&& v) : value(std::move(v)), prev(nullptr), next(nullptr) {}
        };

        Node* head_;
        Node* tail_;
        std::size_t size_;

        Node* node_at(std::size_t idx) {
            if (idx >= size_) throw std::out_of_range("DoublyLinkedList::node_at out of range");

            if (idx <= size_ / 2) {
                Node* temp = head_;
                for (std::size_t i = 0; i < idx; ++i) temp = temp -> next;
                return temp;
            } else {
                Node* temp = tail_;
                for (std::size_t i = size_ - 1; i > idx; --i) temp = temp -> prev;
                return temp;
            }
        }

        void delete_node(Node* node) {
            if (!node) return;

            Node* pr = node -> prev;
            Node* ne = node -> next;

            pr ? pr -> next = ne : head_ = ne;
            ne ? ne -> prev = pr : tail_ = pr;

            delete node;
            --size_;
        }

        void clear() noexcept {
            Node* temp = head_;
            while (temp) {
                Node* next = temp -> next;
                delete temp;
                temp = next;
            }
            head_ = tail_ = nullptr;
            size_ = 0;
        }

    public:
        class iterator {
            Node* p_;

        public:
            explicit iterator(Node* p = nullptr) : p_(p) {}

            T& operator*() const { return p_ -> value; }
            T* operator->() const { return &p_ -> value; }

            iterator& operator++() {
                if (p_) p_ = p_ -> next;
                return *this;
            }

            bool operator==(const iterator& other) const { return p_ == other.p_; }
            bool operator!=(const iterator& other) const { return p_ != other.p_; }

            friend class DoublyLinkedList;
        };

        DoublyLinkedList() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}
        ~DoublyLinkedList() { clear(); }

        // TODO: Revisit after reviewing the RULE OF FIVE
        DoublyLinkedList(const DoublyLinkedList&) = delete;
        DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

        DoublyLinkedList(DoublyLinkedList&& other) noexcept
            : head_(other.head_), tail_(other.tail_), size_(other.size_) {
            other.head_ = other.tail_ = nullptr;
            other.size_ = 0;
        }

        DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
            if (this == &other) return *this;
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = other.tail_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        std::size_t size() const noexcept { return size_; }
        bool empty() const noexcept { return size_ == 0; }

        iterator begin() { return iterator(head_); }
        iterator end() { return iterator(nullptr); }

        void insert(std::size_t idx, const T& v) {
            if (idx > size_) throw std::out_of_range("DoublyLinkedList::insert pos out of range");
            if (idx == size_) { push_back(v); return; }
            if (idx == 0) { push_front(v); return; }

            Node* temp = node_at(idx);
            Node* node = new Node(v);

            node -> next = temp;
            node -> prev = temp -> prev;

            if (temp -> prev) node -> prev -> next = node;
            else head_ = node;

            temp -> prev = node;
            ++size_;
        }

        void erase(std::size_t idx) {
            Node* node = node_at(idx);
            delete_node(node);
        }

        void push_back(const T& v) {
            Node* node = new Node(v);
            node -> prev = tail_;
            tail_ ? tail_ -> next = node : head_ = node;
            tail_ = node;
            ++size_;
        }

        void push_back(T&& v) {
            Node* node = new Node(std::move(v));
            node -> prev = tail_;
            tail_ ? tail_ -> next = node : head_ = node;
            tail_ = node;
            ++size_;
        }

        void push_front(const T& v) {
            Node* node = new Node(v);
            node -> next = head_;
            head_ ? head_ -> prev = node : tail_ = node;
            head_ = node;
            ++size_;
        }

        void push_front(T&& v) {
            Node* node = new Node(std::move(v));
            node -> next = head_;
            head_ ? head_ -> prev = node : tail_ = node;
            head_ = node;
            ++size_;
        }

        void pop_back() {
            if (empty()) throw std::out_of_range("DoublyLinkedList::pop_back empty");
            delete_node(tail_);
        }

        void pop_front() {
            if (empty()) throw std::out_of_range("DoublyLinkedList::pop_front empty");
            delete_node(head_);
        }

        const T& front() const {
            if (empty()) throw std::out_of_range("DoublyLinkedList::front empty");
            return head_ -> value;
        }

        T& front() {
            if (empty()) throw std::out_of_range("DoublyLinkedList::front empty");
            return head_ -> value;
        }

        const T& back() const {
            if (empty()) throw std::out_of_range("DoublyLinkedList::back empty");
            return tail_ -> value;
        }

        T& back() {
            if (empty()) throw std::out_of_range("DoublyLinkedList::back empty");
            return tail_ -> value;
        }
    };
}