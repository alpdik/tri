/**
 * @file Stack.h
 * @author Umut Ertuğrul Daşgın
 * @brief Stack implementation with pointer based DLL.
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
    class Stack {
    private:
        DoublyLinkedList<T> dll;

    public:
        void push(const T& item) { dll.push_front(item); }
        void pop() { if (!empty()) { dll.pop_front(); } }
        T& top() { return dll.front(); }
        bool empty() const { return dll.empty(); }
        size_t size() const { return dll.size(); }
    };
}
