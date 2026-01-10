/**
 * @file Stack.h
 * @brief Stack implementation based on a doubly linked list.
 *
 * @author Umut Ertuğrul Daşgın
 * @note Documentation maintained by Deniz Kayra Aydın / Ömer Kağan Zafer.
 * @version 1.1
 * @date 2025-12-25
 * @lastModified 2026-01-10
 */

#pragma once
#include "DoublyLinkedList.h"

namespace data_structures {

/**
 * @brief LIFO stack container.
 * * Uses a doubly linked list internally
 * * Provides constant-time push and pop
 */
template <typename T>
class Stack {
private:
    DoublyLinkedList<T> dll;

public:
    /**
     * @brief Pushes an element onto the stack.
     * @param item Element to push.
     */
    void push(const T& item) { dll.push_front(item); }

    /**
     * @brief Removes the top element.
     */
    void pop() { if (!empty()) { dll.pop_front(); } }

    /**
     * @brief Returns the top element.
     * @return Reference to the top element.
     */
    T& top() { return dll.front(); }

    /**
     * @brief Checks whether the stack is empty.
     * @return True if the stack is empty.
     */
    bool empty() const { return dll.empty(); }

    /**
     * @brief Returns the number of elements.
     * @return Stack size.
     */
    size_t size() const { return dll.size(); }
};

} // namespace data_structures
