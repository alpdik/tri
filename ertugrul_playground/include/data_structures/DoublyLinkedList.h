/**
 * @file DoublyLinkedList.h
 * @author Umut Ertuğrul Daşgın
 * @brief A pointer based container implementation as a doubly linked list.
 * @version 1.1
 * @date 2025-12-24
 */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <functional>

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

        Node* node_at(std::size_t idx) const {
            if (idx >= size_) throw std::out_of_range("DoublyLinkedList::node_at out of range");

            if (idx <= size_ / 2) {
                Node* temp = head_;
                for (std::size_t i = 0; i < idx; ++i) temp = temp->next;
                return temp;
            } else {
                Node* temp = tail_;
                for (std::size_t i = size_ - 1; i > idx; --i) temp = temp->prev;
                return temp;
            }
        }

        void delete_node(Node* node) {
            if (!node) return;

            Node* pr = node->prev;
            Node* ne = node->next;

            pr ? pr->next = ne : head_ = ne;
            ne ? ne->prev = pr : tail_ = pr;

            delete node;
            --size_;
        }

        void clear_internal() noexcept {
            Node* temp = head_;
            while (temp) {
                Node* next = temp->next;
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
            T& operator*() const { return p_->value; }
            T* operator->() const { return &p_->value; }
            iterator& operator++() { if (p_) p_ = p_->next; return *this; }
            iterator& operator--() { if (p_) p_ = p_->prev; return *this; }
            bool operator==(const iterator& other) const { return p_ == other.p_; }
            bool operator!=(const iterator& other) const { return p_ != other.p_; }

            friend class DoublyLinkedList;
        };

        DoublyLinkedList() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}
        
        ~DoublyLinkedList() { clear_internal(); }

        DoublyLinkedList(const DoublyLinkedList& other) : head_(nullptr), tail_(nullptr), size_(0) {
            for (Node* curr = other.head_; curr != nullptr; curr = curr->next) {
                push_back(curr->value);
            }
        }

        DoublyLinkedList& operator=(const DoublyLinkedList& other) {
            if (this == &other) return *this;
            clear_internal();
            for (Node* curr = other.head_; curr != nullptr; curr = curr->next) {
                push_back(curr->value);
            }
            return *this;
        }

        DoublyLinkedList(DoublyLinkedList&& other) noexcept
            : head_(other.head_), tail_(other.tail_), size_(other.size_) {
            other.head_ = other.tail_ = nullptr;
            other.size_ = 0;
        }

        DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
            if (this == &other) return *this;
            clear_internal();
            head_ = other.head_;
            tail_ = other.tail_;
            size_ = other.size_;
            other.head_ = other.tail_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        std::size_t size() const noexcept { return size_; }
        bool empty() const noexcept { return size_ == 0; }
        void clear() { clear_internal(); }

        iterator begin() const { return iterator(head_); }
        iterator end() const { return iterator(nullptr); }

        // sample usage: list.find([](const File& f){ return f.name == "main.cpp"; });
        iterator find(std::function<bool(const T&)> predicate) {
            Node* temp = head_;
            while (temp) {
                if (predicate(temp->value)) return iterator(temp);
                temp = temp->next;
            }
            return end();
        }

        bool remove(const T& val) {
            Node* temp = head_;
            while (temp) {
                if (temp->value == val) {
                    delete_node(temp);
                    return true;
                }
                temp = temp->next;
            }
            return false;
        }

        void insert(std::size_t idx, const T& v) {
            if (idx > size_) throw std::out_of_range("DoublyLinkedList::insert pos out of range");
            if (idx == size_) { push_back(v); return; }
            if (idx == 0) { push_front(v); return; }

            Node* temp = node_at(idx);
            Node* node = new Node(v);

            node->next = temp;
            node->prev = temp->prev;
            if (temp->prev) node->prev->next = node;
            else head_ = node;
            temp->prev = node;
            ++size_;
        }

        void erase(std::size_t idx) {
            Node* node = node_at(idx);
            delete_node(node);
        }

        void push_back(const T& v) {
            Node* node = new Node(v);
            node->prev = tail_;
            tail_ ? tail_->next = node : head_ = node;
            tail_ = node;
            ++size_;
        }

        void push_back(T&& v) {
            Node* node = new Node(std::move(v));
            node->prev = tail_;
            tail_ ? tail_->next = node : head_ = node;
            tail_ = node;
            ++size_;
        }

        void push_front(const T& v) {
            Node* node = new Node(v);
            node->next = head_;
            head_ ? head_->prev = node : tail_ = node;
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

        T& front() {
            if (empty()) throw std::out_of_range("DoublyLinkedList::front empty");
            return head_->value;
        }

        const T& front() const {
            if (empty()) throw std::out_of_range("DoublyLinkedList::front empty");
            return head_->value;
        }

        T& back() {
            if (empty()) throw std::out_of_range("DoublyLinkedList::back empty");
            return tail_->value;
        }
        
        const T& back() const {
            if (empty()) throw std::out_of_range("DoublyLinkedList::back empty");
            return tail_->value;
        }
    };
}