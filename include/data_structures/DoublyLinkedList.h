/**
 * @file DoublyLinkedList.h
 * @brief A generic pointer-based doubly linked list container.
 *
 * @details DoublyLinkedList class that provides a dynamic container 
 * implementation using a self made doubly linked list structure.
 * It supports bidirectional traversal, indexed access, insertion, removal,
 * and iterator-based operations.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın and Ömer Kağan Zafer
 * @version 1.1
 * @date 2025-12-24
 * @lastModified 2026-01-10
 */

#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <functional>

namespace data_structures {

/**
 * @brief A templated doubly linked list container.
 *
 * @details The DoublyLinkedList class stores elements in dynamically allocated nodes
 * connected via forward and backward pointers. This structure allows efficient
 * insertion and removal at both ends of the list and supports bidirectional
 * iteration.
 *
 * @tparam T Type of elements stored in the list.
 */
template <typename T>
class DoublyLinkedList {
private:
    /**
     * @brief Internal node structure of the linked list.
     *
     * @details Each node stores a value and pointers to the previous and next nodes.
     */
    struct Node {
        T value;
        Node* prev;
        Node* next;

        explicit Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
        explicit Node(T&& v) : value(std::move(v)), prev(nullptr), next(nullptr) {}
    };

    Node* head_;          ///< Pointer to the first node
    Node* tail_;          ///< Pointer to the last node
    std::size_t size_;    ///< Number of elements in the list

    /**
     * @brief Retrieves the node at a given index.
     *
     * @details Traverses from the head or tail depending on the index position
     * to reduce traversal cost.
     *
     * @param[in] idx Index of the desired node.
     * @return Pointer to the node at the given index.
     *
     * @throws std::out_of_range if index is out of bounds.
     * @note Time complexity: O(min(idx, size()-idx))
     */
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

    /**
     * @brief Deletes a given node from the list.
     *
     * @details Updates neighboring node links and releases memory.
     *
     * @param[in] node Pointer to the node to be deleted.
     * @note Time complexity: O(1)
     */
    void delete_node(Node* node) {
        if (!node) return;

        Node* pr = node->prev;
        Node* ne = node->next;

        pr ? pr->next = ne : head_ = ne;
        ne ? ne->prev = pr : tail_ = pr;

        delete node;
        --size_;
    }

    /**
     * @brief Clears all nodes from the list.
     *
     * @details Deallocates all internal nodes and resets the list state.
     * @note Time complexity: O(n) where n is the number of elements.
     */
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
    /**
     * @brief Bidirectional iterator for the linked list.
     */
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

    /**
     * @brief Constructs an empty list.
     * @note Time complexity: O(1)
     */
    DoublyLinkedList() noexcept : head_(nullptr), tail_(nullptr), size_(0) {}

    /**
     * @brief Destroys the list and releases all allocated nodes.
     * @note Time complexity: O(n) where n is the number of elements.
     */
    ~DoublyLinkedList() { clear_internal(); }

    /**
     * @brief Copy constructor.
     *
     * @details Creates a deep copy of another list.
     *
     * @param[in] other List to copy from.
     * @note Time complexity: O(n) where n is the number of elements in other.
     */
    DoublyLinkedList(const DoublyLinkedList& other) : head_(nullptr), tail_(nullptr), size_(0) {
        for (Node* curr = other.head_; curr != nullptr; curr = curr->next) {
            push_back(curr->value);
        }
    }

    /**
     * @brief Copy assignment operator.
     *
     * @param[in] other List to assign from.
     * @return Reference to this list.
     * @note Time complexity: O(n + m) where n is the current size and m is other.size().
     */
    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this == &other) return *this;
        clear_internal();
        for (Node* curr = other.head_; curr != nullptr; curr = curr->next) {
            push_back(curr->value);
        }
        return *this;
    }

    /**
     * @brief Move constructor.
     *
     * @details Transfers ownership of nodes from another list.
     *
     * @param[in] other List to move from.
     * @note Time complexity: O(1)
     */
    DoublyLinkedList(DoublyLinkedList&& other) noexcept
        : head_(other.head_), tail_(other.tail_), size_(other.size_) {
        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
    }

    /**
     * @brief Move assignment operator.
     *
     * @param[in] other List to move from.
     * @return Reference to this list.
     * @note Time complexity: O(n) where n is the current size of this list.
     */
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

    /**
     * @brief Returns the number of elements in the list.
     * @note Time complexity: O(1)
     */
    std::size_t size() const noexcept { return size_; }

    /**
     * @brief Checks whether the list is empty.
     * @note Time complexity: O(1)
     */
    bool empty() const noexcept { return size_ == 0; }

    /**
     * @brief Removes all elements from the list.
     * @note Time complexity: O(n) where n is the number of elements.
     */
    void clear() { clear_internal(); }

    /**
     * @brief Returns an iterator to the beginning of the list.
     * @note Time complexity: O(1)
     */
    iterator begin() const { return iterator(head_); }

    /**
     * @brief Returns an iterator to the end of the list.
     * @note Time complexity: O(1)
     */
    iterator end() const { return iterator(nullptr); }

    /**
     * @brief Finds an element matching a predicate.
     *
     * @param[in] predicate Function used to test elements.
     * @return Iterator to the matching element or end().
     * @note Time complexity: O(n) where n is the number of elements.
     */
    iterator find(std::function<bool(const T&)> predicate) {
        Node* temp = head_;
        while (temp) {
            if (predicate(temp->value)) return iterator(temp);
            temp = temp->next;
        }
        return end();
    }

    /**
     * @brief Removes the first occurrence of a value.
     *
     * @param[in] val Value to remove.
     * @return true if the value was found and removed.
     * @note Time complexity: O(n) where n is the number of elements.
     */
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

    /**
     * @brief Inserts a value at the specified index.
     *
     * @param[in] idx Index at which to insert.
     * @param[in] v Value to insert.
     *
     * @throws std::out_of_range if index is invalid.
     * @note Time complexity: O(min(idx, size()-idx)) due to optimized traversal.
     */
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

    /**
     * @brief Erases the element at the given index.
     *
     * @param[in] idx Index of the element to remove.
     * @note Time complexity: O(min(idx, size()-idx)) due to optimized traversal.
     */
    void erase(std::size_t idx) {
        Node* node = node_at(idx);
        delete_node(node);
    }

    /**
     * @brief Appends an element to the end of the list.
     * @note Time complexity: O(1)
     */
    void push_back(const T& v) {
        Node* node = new Node(v);
        node->prev = tail_;
        tail_ ? tail_->next = node : head_ = node;
        tail_ = node;
        ++size_;
    }

    /**
     * @brief Appends a movable element to the end of the list.
     * @note Time complexity: O(1)
     */
    void push_back(T&& v) {
        Node* node = new Node(std::move(v));
        node->prev = tail_;
        tail_ ? tail_->next = node : head_ = node;
        tail_ = node;
        ++size_;
    }

    /**
     * @brief Inserts an element at the front of the list.
     * @note Time complexity: O(1)
     */
    void push_front(const T& v) {
        Node* node = new Node(v);
        node->next = head_;
        head_ ? head_->prev = node : tail_ = node;
        head_ = node;
        ++size_;
    }

    /**
     * @brief Removes the last element of the list.
     *
     * @throws std::out_of_range if the list is empty.
     * @note Time complexity: O(1)
     */
    void pop_back() {
        if (empty()) throw std::out_of_range("DoublyLinkedList::pop_back empty");
        delete_node(tail_);
    }

    /**
     * @brief Removes the first element of the list.
     *
     * @throws std::out_of_range if the list is empty.
     * @note Time complexity: O(1)
     */
    void pop_front() {
        if (empty()) throw std::out_of_range("DoublyLinkedList::pop_front empty");
        delete_node(head_);
    }

    /**
     * @brief Returns the first element.
     * @note Time complexity: O(1)
     */
    T& front() {
        if (empty()) throw std::out_of_range("DoublyLinkedList::front empty");
        return head_->value;
    }

    /**
     * @brief Returns the first element (const).
     * @note Time complexity: O(1)
     */
    const T& front() const {
        if (empty()) throw std::out_of_range("DoublyLinkedList::front empty");
        return head_->value;
    }

    /**
     * @brief Returns the last element.
     * @note Time complexity: O(1)
     */
    T& back() {
        if (empty()) throw std::out_of_range("DoublyLinkedList::back empty");
        return tail_->value;
    }

    /**
     * @brief Returns the last element (const).
     * @note Time complexity: O(1)
     */
    const T& back() const {
        if (empty()) throw std::out_of_range("DoublyLinkedList::back empty");
        return tail_->value;
    }
};

} // namespace data_structures
