/**
 * @file DoublyLinkedList.h
 * @brief Pointer-based doubly linked list container for sequential storage.
 * @author [Your Names]
 * @date [YYYY-MM-DD]
 */

#pragma once

#include <cstddef>     // size_t
#include <stdexcept>   // std::out_of_range
#include <utility>     // std::move

namespace data_structures {

/**
 * @brief Pointer-based doubly linked list with sequential access.
 *
 * This container stores elements in dynamically allocated nodes connected
 * via next and previous pointers. It does not rely on any array-based storage
 * and is fully pointer-based.
 *
 * Index-based access is provided for convenience, but runs in O(n) time.
 *
 * @tparam T Type of elements stored in the list.
 */
template <typename T>
class DoublyLinkedList {
private:
  /**
   * @brief Internal node used by the list.
   *
   * Each node owns a value and links to its neighbors.
   */
  struct Node {
    T value;       ///< Stored value
    Node* prev;    ///< Previous node (nullptr if head)
    Node* next;    ///< Next node (nullptr if tail)

    /**
     * @brief Constructs a node by copying a value.
     *
     * @pre None
     * @post value is a copy of v
     * @post prev == nullptr and next == nullptr
     *
     * @param v Value to copy into the node.
     */
    explicit Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}

    /**
     * @brief Constructs a node by moving a value.
     *
     * @pre None
     * @post value is move-constructed from v
     * @post prev == nullptr and next == nullptr
     *
     * @param v Value to move into the node.
     */
    explicit Node(T&& v) : value(std::move(v)), prev(nullptr), next(nullptr) {}
  };

  Node* head_;          ///< Pointer to first node (nullptr if empty)
  Node* tail_;          ///< Pointer to last node (nullptr if empty)
  std::size_t size_;    ///< Number of elements

  /**
   * @brief Returns the node located at a given index.
   *
   * @pre i < size()
   * @post Returned pointer is not nullptr
   *
   * @param i Zero-based index.
   * @return Pointer to the node at index i.
   * @throws std::out_of_range If i >= size().
   */
  Node* node_at(std::size_t i) const {
    if (i >= size_) throw std::out_of_range("DoublyLinkedList::node_at out of range");

    // Walk from the closer end.
    if (i <= size_ / 2) {
      Node* cur = head_;
      for (std::size_t k = 0; k < i; ++k) cur = cur->next;
      return cur;
    } else {
      Node* cur = tail_;
      for (std::size_t k = size_ - 1; k > i; --k) cur = cur->prev;
      return cur;
    }
  }

  /**
   * @brief Unlinks a node from the list and deletes it.
   *
   * @pre n != nullptr
   * @post size() == old_size - 1
   * @post List links are updated correctly (head_/tail_ may change)
   *
   * @param n Pointer to the node to remove (must be a node from this list).
   */
  void unlink_and_delete(Node* n) {
    if (!n) return;

    Node* p = n->prev;
    Node* q = n->next;

    if (p) p->next = q;
    else head_ = q;

    if (q) q->prev = p;
    else tail_ = p;

    delete n;
    --size_;
  }

  /**
   * @brief Copies elements from another list by pushing to the back.
   *
   * @pre This list is empty OR caller accepts that existing elements remain
   *      (in this implementation, we call this only after clear()).
   * @post size() == old_size + other.size()
   *
   * @param other List to copy from.
   */
  void copy_from(const DoublyLinkedList& other) {
    for (Node* cur = other.head_; cur != nullptr; cur = cur->next) {
      push_back(cur->value);
    }
  }

public:
  /**
   * @brief Minimal forward iterator for the list.
   *
   * Iterates from head to tail.
   */
  class iterator {
  private:
    Node* p_;

  public:
    /**
     * @brief Constructs an iterator from a node pointer.
     *
     * @param p Node pointer (nullptr means end()).
     */
    explicit iterator(Node* p = nullptr) : p_(p) {}

    /**
     * @brief Dereferences the iterator.
     *
     * @pre p_ != nullptr
     * @return Reference to the current element.
     */
    T& operator*() const { return p_->value; }

    /**
     * @brief Member access operator.
     *
     * @pre p_ != nullptr
     * @return Pointer to the current element.
     */
    T* operator->() const { return &p_->value; }

    /**
     * @brief Advances iterator to the next node.
     *
     * @post Iterator points to the next node (or end()).
     * @return Reference to this iterator.
     */
    iterator& operator++() {
      if (p_) p_ = p_->next;
      return *this;
    }

    /**
     * @brief Equality comparison.
     */
    bool operator==(const iterator& other) const { return p_ == other.p_; }

    /**
     * @brief Inequality comparison.
     */
    bool operator!=(const iterator& other) const { return p_ != other.p_; }

    friend class DoublyLinkedList;
  };

  /**
   * @brief Constructs an empty list.
   *
   * @post size() == 0
   * @post empty() == true
   */
  DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

  /**
   * @brief Destructor.
   *
   * @post All resources released.
   */
  ~DoublyLinkedList() { clear(); }

  // ------------------- Rule of Five -------------------

  /**
   * @brief Copy constructor.
   *
   * @post This list contains copies of other's elements, in the same order.
   *
   * @param other List to copy.
   */
  DoublyLinkedList(const DoublyLinkedList& other)
      : head_(nullptr), tail_(nullptr), size_(0) {
    copy_from(other);
  }

  /**
   * @brief Copy assignment operator.
   *
   * @pre None
   * @post This list contains copies of other's elements, in the same order.
   *
   * @param other List to copy from.
   * @return Reference to this list.
   */
  DoublyLinkedList& operator=(const DoublyLinkedList& other) {
    if (this == &other) return *this;
    clear();
    copy_from(other);
    return *this;
  }

  /**
   * @brief Move constructor.
   *
   * @post This list takes ownership of other's nodes.
   * @post other becomes empty.
   *
   * @param other List to move from.
   */
  DoublyLinkedList(DoublyLinkedList&& other) noexcept
      : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  /**
   * @brief Move assignment operator.
   *
   * @post This list takes ownership of other's nodes.
   * @post other becomes empty.
   *
   * @param other List to move from.
   * @return Reference to this list.
   */
  DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
    if (this == &other) return *this;
    clear();
    head_ = other.head_;
    tail_ = other.tail_;
    size_ = other.size_;
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
    return *this;
  }

  // ------------------- Capacity -------------------

  /**
   * @brief Returns the number of elements.
   *
   * @return Current size of the list.
   */
  std::size_t size() const { return size_; }

  /**
   * @brief Checks whether the list is empty.
   *
   * @return true if size() == 0, otherwise false.
   */
  bool empty() const { return size_ == 0; }

  // ------------------- Element access -------------------

  /**
   * @brief Accesses the element at index i.
   *
   * @pre i < size()
   * @post Returned reference refers to the i-th element.
   *
   * @param i Zero-based index.
   * @return Reference to the element at index i.
   * @throws std::out_of_range If i >= size().
   */
  T& operator[](std::size_t i) { return node_at(i)->value; }

  /**
   * @brief Accesses the element at index i (const).
   *
   * @pre i < size()
   * @param i Zero-based index.
   * @return Const reference to the element at index i.
   * @throws std::out_of_range If i >= size().
   */
  const T& operator[](std::size_t i) const { return node_at(i)->value; }

  /**
   * @brief Returns the first element.
   *
   * @pre empty() == false
   * @return Reference to the first element.
   * @throws std::out_of_range If the list is empty.
   */
  T& front() {
    if (!head_) throw std::out_of_range("DoublyLinkedList::front on empty");
    return head_->value;
  }

  /**
   * @brief Returns the last element.
   *
   * @pre empty() == false
   * @return Reference to the last element.
   * @throws std::out_of_range If the list is empty.
   */
  T& back() {
    if (!tail_) throw std::out_of_range("DoublyLinkedList::back on empty");
    return tail_->value;
  }

  // ------------------- Modifiers -------------------

  /**
   * @brief Removes all elements from the list.
   *
   * @post size() == 0
   * @post empty() == true
   */
  void clear() {
    Node* cur = head_;
    while (cur) {
      Node* nxt = cur->next;
      delete cur;
      cur = nxt;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
  }

  /**
   * @brief Appends a value to the end of the list.
   *
   * @post size() == old_size + 1
   * @post back() equals the appended value
   *
   * @param v Value to append.
   */
  void push_back(const T& v) {
    Node* n = new Node(v);
    if (!tail_) {
      head_ = tail_ = n;
    } else {
      n->prev = tail_;
      tail_->next = n;
      tail_ = n;
    }
    ++size_;
  }

  /**
   * @brief Appends a value to the end of the list by moving it.
   *
   * @post size() == old_size + 1
   * @post back() equals the moved-in value
   *
   * @param v Value to append (moved).
   */
  void push_back(T&& v) {
    Node* n = new Node(std::move(v));
    if (!tail_) {
      head_ = tail_ = n;
    } else {
      n->prev = tail_;
      tail_->next = n;
      tail_ = n;
    }
    ++size_;
  }

  /**
   * @brief Removes the last element.
   *
   * @pre empty() == false
   * @post size() == old_size - 1
   *
   * @throws std::out_of_range If the list is empty.
   */
  void pop_back() {
    if (!tail_) throw std::out_of_range("DoublyLinkedList::pop_back on empty");
    unlink_and_delete(tail_);
  }

  /**
   * @brief Inserts a value before index pos.
   *
   * @pre pos <= size()
   * @post size() == old_size + 1
   *
   * @param pos Index to insert before (0..size()).
   * @param v Value to insert.
   * @throws std::out_of_range If pos > size().
   */
  void insert(std::size_t pos, const T& v) {
    if (pos > size_) throw std::out_of_range("DoublyLinkedList::insert pos out of range");
    if (pos == size_) {
      push_back(v);
      return;
    }
    Node* atp = node_at(pos);
    Node* n = new Node(v);

    n->next = atp;
    n->prev = atp->prev;

    if (atp->prev) atp->prev->next = n;
    else head_ = n;

    atp->prev = n;
    ++size_;
  }

  /**
   * @brief Erases the element at index pos.
   *
   * @pre pos < size()
   * @post size() == old_size - 1
   *
   * @param pos Index of element to erase.
   * @throws std::out_of_range If pos >= size().
   */
  void erase(std::size_t pos) {
    Node* n = node_at(pos);
    unlink_and_delete(n);
  }

  // ------------------- Iteration -------------------

  /**
   * @brief Returns iterator to the first element.
   *
   * @return Iterator pointing to the first element (or end() if empty).
   */
  iterator begin() { return iterator(head_); }

  /**
   * @brief Returns iterator past the last element.
   *
   * @return Iterator representing the end position.
   */
  iterator end() { return iterator(nullptr); }
};

} // namespace data_structures