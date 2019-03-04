/*******************************************************************************
 * tlx/container/d_ary_addressable_int_heap.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Eugenio Angriman <angrimae@hu-berlin.de>
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_CONTAINER_D_ARY_ADDRESSABLE_INT_HEAP_HEADER
#define TLX_CONTAINER_D_ARY_ADDRESSABLE_INT_HEAP_HEADER

#include <cassert>
#include <cstddef>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

namespace tlx {

//! \addtogroup tlx_container
//! \{

/*!
 * This class implements an addressable integer priority queue, precisely a
 * d-ary heap.
 *
 * Keys must be unique unsigned integers. The addressable heap holds an array
 * indexed by the keys, hence it requires a multiple of the highest integer key
 * as space!
 *
 * \tparam KeyType    Has to be an unsigned integer type.
 * \tparam Arity      A positive integer.
 * \tparam Compare    Function object.
 */
template <typename KeyType, unsigned Arity = 2,
          class Compare = std::less<KeyType> >
class DAryAddressableIntHeap
{
    static_assert(std::numeric_limits<KeyType>::is_integer &&
                  !std::numeric_limits<KeyType>::is_signed,
                  "KeyType must be an unsigned integer type.");
    static_assert(Arity, "Arity must be greater than zero.");

public:
    using key_type = KeyType;
    using compare_type = Compare;

    static constexpr size_t arity = Arity;

protected:
    //! Cells in the heap.
    std::vector<key_type> heap_;

    //! Positions of the keys in the heap vector.
    std::vector<key_type> handles_;

    //! Compare function.
    compare_type cmp_;

    //! Marks a key that is not in the heap.
    static constexpr key_type not_present() {
        return static_cast<key_type>(-1);
    }

public:
    //! Allocates an empty heap.
    explicit DAryAddressableIntHeap(compare_type cmp = compare_type())
        : heap_(0), handles_(0), cmp_(cmp) { }

    //! Allocates space for \c new_size items.
    void reserve(size_t new_size) {
        if (handles_.size() < new_size) {
            handles_.resize(new_size, not_present());
            heap_.reserve(new_size);
        }
    }

    //! Copy.
    DAryAddressableIntHeap(const DAryAddressableIntHeap&) = default;
    DAryAddressableIntHeap& operator = (const DAryAddressableIntHeap&) = default;

    //! Move.
    DAryAddressableIntHeap(DAryAddressableIntHeap&&) = default;
    DAryAddressableIntHeap& operator = (DAryAddressableIntHeap&&) = default;

    //! Empties the heap.
    void clear() {
        std::fill(handles_.begin(), handles_.end(), not_present());
        heap_.clear();
    }

    //! Returns the number of items in the heap.
    size_t size() const noexcept { return heap_.size(); }

    //! Returns the capacity of the heap.
    size_t capacity() const noexcept { return heap_.capacity(); }

    //! Returns true if the heap has no items, false otherwise.
    bool empty() const noexcept { return heap_.empty(); }

    //! Inserts a new item.
    void push(const key_type& new_key) {
        // Avoid to add the key that we use to mark non present keys.
        assert(new_key != not_present());
        if (new_key >= handles_.size()) {
            handles_.resize(new_key + 1, not_present());
        }
        else {
            assert(handles_[new_key] == not_present());
        }

        // Insert the new item at the end of the heap.
        handles_[new_key] = static_cast<key_type>(heap_.size());
        heap_.push_back(new_key);
        sift_up(heap_.size() - 1);
    }

    //! Inserts a new item.
    void push(key_type&& new_key) {
        // Avoid to add the key that we use to mark non present keys.
        assert(new_key != not_present());
        if (new_key >= handles_.size()) {
            handles_.resize(new_key + 1, not_present());
        }
        else {
            assert(handles_[new_key] == not_present());
        }

        // Insert the new item at the end of the heap.
        handles_[new_key] = static_cast<key_type>(heap_.size());
        heap_.push_back(std::move(new_key));
        sift_up(heap_.size() - 1);
    }

    //! Removes the item with key \c key.
    void remove(key_type key) {
        assert(contains(key));
        key_type h = handles_[key];
        std::swap(heap_[h], heap_.back());
        handles_[heap_[h]] = h;
        handles_[heap_.back()] = not_present();
        heap_.pop_back();
        // If we did not remove the last item in the heap vector.
        if (h < size()) {
            if (h && cmp_(heap_[h], heap_[parent(h)])) {
                sift_up(h);
            }
            else {
                sift_down(h);
            }
        }
    }

    //! Returns the top item.
    const key_type& top() const noexcept {
        assert(!empty());
        return heap_[0];
    }

    //! Removes the top item.
    void pop() { remove(heap_[0]); }

    //! Removes and returns the top item.
    key_type extract_top() {
        key_type top_item = top();
        pop();
        return top_item;
    }

    /*!
     * Updates the priority queue after the priority associated to the item with
     * key \c key has been changed; if the key \c key is not present in the
     * priority queue, it will be added.
     *
     * Note: if not called after a priority is changed, the behavior of the data
     * structure is undefined.
     */
    void update(key_type key) {
        if (key >= handles_.size() || handles_[key] == not_present()) {
            push(key);
        }
        else if (handles_[key] &&
                 cmp_(heap_[handles_[key]], heap_[parent(handles_[key])])) {
            sift_up(handles_[key]);
        }
        else {
            sift_down(handles_[key]);
        }
    }

    //! Returns true if the key \c key is in the heap, false otherwise.
    bool contains(key_type key) const {
        return key < handles_.size() ? handles_[key] != not_present() : false;
    }

    //! For debugging: runs a BFS from the root node and verifies that the heap
    //! property is respected.
    bool sanity_check() {
        if (empty()) {
            return true;
        }
        std::vector<unsigned char> mark(handles_.size());
        std::queue<size_t> q;
        // Explore from the root.
        q.push(0);
        // mark first value as seen
        mark.at(heap_[0]) = 1;
        while (!q.empty()) {
            size_t s = q.front();
            q.pop();
            size_t l = left(s);
            for (size_t i = 0; i < arity && l < heap_.size(); ++i) {
                // check that the priority of the children is not strictly less
                // than their parent.
                if (cmp_(heap_[l], heap_[s]))
                    return false;
                // check handle
                if (handles_[heap_[l]] != l)
                    return false;
                // mark value as seen
                mark.at(heap_[l]) = 1;
                q.push(l++);
            }
        }
        // check not_present handles
        for (size_t i = 0; i < mark.size(); ++i) {
            if (mark[i] != (handles_[i] != not_present()))
                return false;
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
        key_type value = std::move(heap_[k]);
        size_t p = parent(k);
        while (k > 0 && !cmp_(heap_[p], value)) {
            heap_[k] = std::move(heap_[p]);
            handles_[heap_[k]] = k;
            k = p, p = parent(k);
        }
        handles_[value] = k;
        heap_[k] = std::move(value);
    }

    //! Pushes the item at position \c k down until either it becomes a leaf or
    //! all its children have higher priority
    void sift_down(size_t k) {
        key_type value = std::move(heap_[k]);
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
            if (!cmp_(heap_[c], value)) {
                break;
            }

            // Swap current item with the child with minimum priority.
            heap_[k] = std::move(heap_[c]);
            handles_[heap_[k]] = k;
            k = c;
        }
        handles_[value] = k;
        heap_[k] = std::move(value);
    }
};

//! make template alias due to similarity with std::priority_queue
template <typename KeyType, unsigned Arity = 2,
          typename Compare = std::less<KeyType> >
using d_ary_addressable_int_heap =
    DAryAddressableIntHeap<KeyType, Arity, Compare>;

//! \}

} // namespace tlx

#endif // !TLX_CONTAINER_D_ARY_ADDRESSABLE_INT_HEAP_HEADER

/******************************************************************************/
