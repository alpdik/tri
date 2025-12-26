/**
 * @file Queue.h
 * @author Umut Ertuğrul Daşgın
 * @brief Queue implementation with pointer based DLL.
 * @version 1.0
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once
#include "DoublyLinkedList.h"

namespace data_structures {
    template <typename T>
    class Queue {
    private:
        DoublyLinkedList<T> dll;

    public:
        void enqueue(const T& item) { dll.push_back(item); }
        void dequeue() { if (!empty()) { dll.pop_front(); } }
        T& front() { return dll.front(); }
        bool empty() const { return dll.empty(); }
        size_t size() const { return dll.size(); }
    };
}
