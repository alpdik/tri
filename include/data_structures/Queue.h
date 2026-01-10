/**
 * @file Queue.h
 * @brief Queue implementation based on a doubly linked list.
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
 * @brief FIFO queue container.
 * * Uses a doubly linked list internally
 * * Provides constant-time enqueue and dequeue
 */
template <typename T>
class Queue {
private:
    DoublyLinkedList<T> dll;

public:
    /**
     * @brief Adds an element to the back of the queue.
     * @param item Element to enqueue.
     */
    void enqueue(const T& item) { dll.push_back(item); }

    /**
     * @brief Removes the front element from the queue.
     */
    void dequeue() { if (!empty()) { dll.pop_front(); } }

    /**
     * @brief Returns the front element.
     * @return Reference to the front element.
     */
    T& front() { return dll.front(); }

    /**
     * @brief Checks whether the queue is empty.
     * @return True if the queue is empty.
     */
    bool empty() const { return dll.empty(); }

    /**
     * @brief Returns the number of elements.
     * @return Queue size.
     */
    size_t size() const { return dll.size(); }
};

} // namespace data_structures

