/*******************************************************************************
 * tlx/container/d_ary_heap.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Eugenio Angriman <angrimae@hu-berlin.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_CONTAINER_D_ARY_HEAP_HEADER
#define TLX_CONTAINER_D_ARY_HEAP_HEADER

#include <cassert>
#include <cstddef>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

namespace tlx {

/*!
 * This class implements an addressable integer priority queue, precisely a
 * d-ary heap.
 *
 * Keys are unique and need to be unsigned integers.
 *
 * \tparam KeyType    Has to be an unsigned integer type.
 * \tparam Arity      A positive integer.
 * \tparam Compare    Function object.
 */
template <typename KeyType, unsigned Arity = 2,
          class Compare = std::less<KeyType> >
class DAryHeap
{
    static_assert(Arity, "Arity must be greater than zero.");

public:
    using key_type = KeyType;
    using compare_type = Compare;

    static constexpr size_t arity = Arity;

protected:
    //! Cells in the heap.
    std::vector<key_type> heap_;

    //! Compare function.
    compare_type cmp_;

    //! Marks a key that is not in the heap.
    static constexpr key_type not_present() {
        return static_cast<key_type>(-1);
    }

public:
    //! Allocates an empty heap.
    explicit DAryHeap(compare_type cmp = compare_type())
        : heap_(0), cmp_(cmp) { }

    //! Allocates space for \c new_size items.
    void reserve(size_t new_size) {
        heap_.reserve(new_size);
    }

    //! Copy.
    DAryHeap(const DAryHeap&) = default;
    DAryHeap& operator = (const DAryHeap&) = default;

    //! Move.
    DAryHeap(DAryHeap&&) = default;
    DAryHeap& operator = (DAryHeap&&) = default;

    //! Empties the heap.
    void clear() {
        heap_.clear();
    }

    //! Returns the number of items in the heap.
    size_t size() const noexcept { return heap_.size(); }

    //! Returns the capacity of the heap.
    size_t capacity() const noexcept { return heap_.capacity(); }

    //! Returns true if the heap has no items, false otherwise.
    bool empty() const noexcept { return heap_.empty(); }

    //! Inserts a new item.
    void push(key_type new_key) {
        // Avoid to add the key that we use to mark non present keys.
        assert(new_key != not_present());

        // Insert the new item at the end of the heap.
        heap_.push_back(std::move(new_key));
        sift_up(heap_.size() - 1);
    }

    //! Returns the top item.
    const key_type& top() const noexcept {
        assert(!empty());
        return heap_[0];
    }

    //! Removes the top item.
    void pop() {
        assert(!empty());
        std::swap(heap_[0], heap_.back());
        heap_.pop_back();
        sift_down(0);
    }

    //! Removes and returns the top item.
    key_type extract_top() {
        key_type top_item = top();
        pop();
        return top_item;
    }

    //! For debugging: runs a BFS from the root node and verifies that the heap
    //! property is respected.
    bool sanity_check() {
        if (empty()) {
            return true;
        }
        std::queue<size_t> q;
        // Explore from the root.
        q.push(0);
        while (!q.empty()) {
            size_t s = q.front();
            q.pop();
            size_t l = left(s);
            for (size_t i = 0; i < arity && l < heap_.size(); ++i) {
                // Check that the priority of the children is not strictly less
                // than their parent.
                if (cmp_(heap_[l], heap_[s]))
                    return false;
                q.push(l++);
            }
        }
        return true;
    }

private:
    //! Returns the position of the left child of the node at position \c k.
    size_t left(size_t k) const { return arity * k + 1; }

    //! Returns the position of the parent of the node at position \c k.
    size_t parent(size_t k) const { return (k - 1) / arity; }

    //! Pushes the node at position \c k up until either it becomes the root or
    //! its parent has lower or equal priority.
    void sift_up(size_t k) {
        while (k) {
            size_t p = parent(k);
            if (!cmp_(heap_[k], heap_[p])) {
                break;
            }
            std::swap(heap_[p], heap_[k]);
            k = p;
        }
    }

    //! Pushes the item at position \c k down until either it becomes a leaf or
    //! all its children have higher priority
    void sift_down(size_t k) {
        while (true) {
            size_t l = left(k);
            if (l >= heap_.size()) {
                break;
            }
            // Get the min child.
            size_t c = l;
            size_t right = std::min(heap_.size(), c + arity);
            while (++l < right) {
                if (cmp_(heap_[l], heap_[c])) {
                    c = l;
                }
            }

            // Current item has lower or equal priority than the child with
            // minimum priority, stop.
            if (!cmp_(heap_[c], heap_[k])) {
                break;
            }

            // Swap current item with the child with minimum priority.
            std::swap(heap_[k], heap_[c]);
            k = c;
        }
    }
};

} // namespace tlx

#endif // !TLX_CONTAINER_D_ARY_HEAP_HEADER

/******************************************************************************/