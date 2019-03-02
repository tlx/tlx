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
class d_ary_heap
{
    static_assert(std::numeric_limits<KeyType>::is_integer &&
                  !std::numeric_limits<KeyType>::is_signed,
                  "KeyType must be an unsigned integer type.");
    static_assert(Arity, "Arity must be greater than zero.");

public:
    using key_type = KeyType;
    using compare = Compare;

    static constexpr size_t arity = Arity;

protected:
    std::vector<key_type> heap;
    //! Positions of the keys in the heap vector.
    std::vector<key_type> handles;

    //! Compare function.
    compare c_;

    //! Marks a key that is not in the heap.
    static constexpr key_type not_present() { return static_cast<key_type>(-1); }

public:
    //! Allocates an empty heap.
    explicit d_ary_heap(compare c = compare{ }) : heap(0), handles(0), c_(c) { }

    //! Allocates space for \c new_size items.
    void reserve(size_t new_size) {
        if (handles.size() < new_size) {
            handles.resize(new_size, not_present());
            heap.reserve(new_size);
        }
    }

    //! Copy.
    d_ary_heap(const d_ary_heap&) = default;
    d_ary_heap& operator = (const d_ary_heap&) = default;

    //! Move.
    d_ary_heap(d_ary_heap&&) = default;
    d_ary_heap& operator = (d_ary_heap&&) = default;

    //! Empties the heap.
    void clear() {
        std::fill(handles.begin(), handles.end(), not_present());
        heap.clear();
    }

    //! Returns the number of items in the heap.
    size_t size() const noexcept { return heap.size(); }

    //! Returns the capacity of the heap.
    size_t capacity() const noexcept { return heap.capacity(); }

    //! Returns true if the heap has no items, false otherwise.
    bool empty() const noexcept { return heap.empty(); }

    //! Inserts a new item.
    void push(key_type new_key) {
        // Avoid to add the key that we use to mark non present keys.
        assert(new_key != not_present());
        if (new_key >= handles.size()) {
            handles.resize(new_key + 1, not_present());
        }
        else {
            assert(handles[new_key] == not_present());
        }

        // Insert the new item at the end of the heap.
        handles[new_key] = static_cast<key_type>(heap.size());
        heap.push_back(std::move(new_key));
        siftup(heap.size() - 1);
    }

    //! Removes the item with key \c key.
    void remove(key_type key) {
        assert(contains(key));
        auto h = handles[key];
        std::swap(heap[h], heap.back());
        handles[heap[h]] = h;
        handles[heap.back()] = not_present();
        heap.pop_back();
        // If we did not remove the last item in the heap vector.
        if (h < size()) {
            if (h && c_(heap[h], heap[parent(h)])) {
                siftup(h);
            }
            else {
                siftdown(h);
            }
        }
    }

    //! Returns the top item.
    key_type const& top() const {
        assert(!empty());
        return heap[0];
    }

    //! Removes the top item.
    void pop() { remove(heap[0]); }

    //! Removes and returns the top item.
    key_type extract_top() {
        key_type top_item = top();
        pop();
        return top_item;
    }

    //! Updates the priority queue after the priority associated to the item
    //! with key \c key has been changed; if the key \c key is not present in the
    //! the priority queue, it will be added.
    //!
    //! Note: if not called after a priority is changed, the behavior of the data
    //! structure is undefined.
    void update(key_type key) {
        if (key >= handles.size() || handles[key] == not_present()) {
            push(key);
        }
        else if (handles[key] &&
                 c_(heap[handles[key]], heap[parent(handles[key])])) {
            siftup(handles[key]);
        }
        else {
            siftdown(handles[key]);
        }
    }

    //! Returns true if the key \c key is in the heap, false otherwise.
    bool contains(key_type key) const {
        return key < handles.size() ? handles[key] != not_present() : false;
    }

    //! For debugging: runs a BFS from the root node and verifies that the heap
    //! property is respected.
    void sanity_check() {
        if (empty()) {
            return;
        }
        std::queue<size_t> q;
        // Explore from the root.
        q.push(0);
        while (!q.empty()) {
            auto s = q.front();
            q.pop();
            auto l = left(s);
            for (size_t i = 0; i < arity && l < heap.size(); ++i) {
                // Check that the priority of the children is not strictly less than
                // their parent.
                assert(!c_(heap[l], heap[s]));
                q.push(l++);
            }
        }
    }

private:
    //! Returns the position of the left child of the node at position \c k.
    size_t left(size_t k) const { return arity * k + 1; }

    //! Returns the position of the parent of the node at position \c k.
    size_t parent(size_t k) const { return (k - 1) / arity; }

    //! Pushes the node at position \c k up until  either it becomes the root or
    //! its parent has lower or equal priority.
    void siftup(size_t k) {
        while (k) {
            auto p = parent(k);
            if (!c_(heap[k], heap[p])) {
                break;
            }
            std::swap(heap[p], heap[k]);
            std::swap(handles[heap[p]], handles[heap[k]]);
            k = p;
        }
    }

    //! Pushes the item at position \c k down until either it becomes a leaf or
    //! all its children have higher priority
    void siftdown(size_t k) {
        while (true) {
            auto l = left(k);
            if (l >= heap.size()) {
                break;
            }
            // Get the min child.
            auto c = l;
            auto right = std::min(heap.size(), c + arity);
            while (++l < right) {
                if (c_(heap[l], heap[c])) {
                    c = l;
                }
            }

            // Current item has lower or equal priority than the child with minimum
            // priority, stop.
            if (!c_(heap[c], heap[k])) {
                break;
            }

            // Swap current item with the child with minimum priority.
            std::swap(heap[k], heap[c]);
            std::swap(handles[heap[k]], handles[heap[c]]);
            k = c;
        }
    }
};

} // namespace tlx

#endif // !TLX_CONTAINER_D_ARY_HEAP_HEADER

/******************************************************************************/
