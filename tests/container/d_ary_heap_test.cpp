/*******************************************************************************
 * tests/container/d_ary_heap_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Eugenio Angriman <angrimae@hu-berlin.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <algorithm>
#include <random>
#include <set>
#include <vector>

#include <tlx/container/d_ary_heap.hpp>
#include <tlx/die.hpp>

// Force instantiation.
namespace tlx {

template class d_ary_heap<uint8_t>;
template class d_ary_heap<uint16_t>;
template class d_ary_heap<uint32_t>;
template class d_ary_heap<uint64_t>;

} // namespace tlx

/******************************************************************************/

template <class T>
struct Comparator {
public:
    Comparator(const std::vector<double>& vec) : prio(vec) { }
    bool operator () (const T& x, const T& y) const { return prio[x] < prio[y]; }

private:
    const std::vector<double>& prio;
};

template <typename KeyType>
std::vector<KeyType> get_shuffled_vector(size_t size, uint32_t r_seed) {
    std::vector<KeyType> keys(size);
    for (KeyType key = 0; key < size; ++key) {
        keys[key] = key;
    }

    std::mt19937 gen(r_seed);
    std::shuffle(keys.begin(), keys.end(), gen);
    return keys;
}

//! Verifies the heap property, the size of the heap, and the top item.
template <typename DAryHeap, typename Set>
void check_heap(DAryHeap& heap, Set& set) {
    die_unequal(heap.size(), set.size());
    if (!heap.empty()) {
        die_unequal(heap.top(), *(set.begin()));
    }
}

template <typename DAryHeap, typename Set, typename KeysVector>
void fill_heap_and_set(DAryHeap& heap, Set& set, KeysVector& keys) {
    for (const typename KeysVector::value_type& key : keys) {
        heap.push(key);
        set.insert(key);
        check_heap(heap, set);
    }
}

//! Basic APIs: push(), top(), pop(), and remove().
template <typename KeyType, unsigned Arity = 2,
          class Compare = std::less<KeyType> >
void d_ary_heap_test(size_t size, uint32_t r_seed = 42) {
    tlx::d_ary_heap<KeyType, Arity, Compare> x;
    die_unequal(x.size(), 0u);
    die_if(!x.empty());

    // To check the correctness of the heap.
    std::set<KeyType, Compare> s;

    auto keys = get_shuffled_vector<KeyType>(size, r_seed);
    fill_heap_and_set(x, s, keys);

    // Test pop().
    while (!x.empty()) {
        x.pop();
        s.erase(s.begin());
        check_heap(x, s);
    }

    std::mt19937 gen(r_seed);
    std::shuffle(keys.begin(), keys.end(), gen);
    fill_heap_and_set(x, s, keys);

    // Test remove().
    for (auto key : keys) {
        x.remove(key);
        s.erase(s.find(key));
        check_heap(x, s);
    }
}

//! Tests update().
template <typename KeyType, unsigned Arity = 2>
void d_ary_heap_test_update(size_t size, std::vector<double>& prio,
                            uint32_t r_seed = 42) {
    tlx::d_ary_heap<KeyType, Arity, Comparator<KeyType> > x{
        Comparator<KeyType>(prio) };
    die_unequal(x.size(), 0u);
    die_if(!x.empty());

    std::set<KeyType, Comparator<KeyType> > s{ Comparator<KeyType>(prio) };

    // Keep a backup of the priorities because we are going to modify them.
    std::vector<double> backup(prio);

    auto keys = get_shuffled_vector<KeyType>(size, r_seed);

    // Test update() as a way to insert new items.
    for (auto key : keys) {
        x.update(key);
        s.insert(key);
        check_heap(x, s);
    }

    // Update all the priorities at random.
    std::mt19937 gen(r_seed);
    std::shuffle(keys.begin(), keys.end(), gen);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (auto key : keys) {
        prio[key] = dis(gen);
        x.update(key);
        x.sanity_check();
    }

    // Update the set.
    s.clear();
    for (auto key : keys) {
        s.insert(key);
    }
    check_heap(x, s);

    // Restore initial priorities.
    prio = backup;
}

int main() {
    // Size of the tested heaps and random seed.
    size_t size = 100;
    uint32_t r_seed = 42;

    // Basic heap APIs with default compare functions.
    d_ary_heap_test<uint8_t, 1>(size, r_seed);
    d_ary_heap_test<uint8_t, 2>(size, r_seed);
    d_ary_heap_test<uint8_t, 3>(size, r_seed);
    d_ary_heap_test<uint16_t, 1>(size, r_seed);
    d_ary_heap_test<uint16_t, 2>(size, r_seed);
    d_ary_heap_test<uint16_t, 3>(size, r_seed);
    d_ary_heap_test<uint32_t, 1>(size, r_seed);
    d_ary_heap_test<uint32_t, 2>(size, r_seed);
    d_ary_heap_test<uint32_t, 3>(size, r_seed);
    d_ary_heap_test<uint32_t, 2>(size, r_seed);
    d_ary_heap_test<uint32_t, 3>(size, r_seed);
    d_ary_heap_test<uint64_t, 1>(size, r_seed);
    d_ary_heap_test<uint64_t, 2>(size, r_seed);
    d_ary_heap_test<uint64_t, 3>(size, r_seed);

    d_ary_heap_test<uint8_t, 2, std::greater<uint8_t> >(size, r_seed);
    d_ary_heap_test<uint16_t, 2, std::greater<uint16_t> >(size, r_seed);
    d_ary_heap_test<uint32_t, 2, std::greater<uint32_t> >(size, r_seed);
    d_ary_heap_test<uint64_t, 2, std::greater<uint64_t> >(size, r_seed);

    // Custom compare function.
    std::vector<double> prio(size);
    std::mt19937 gen(r_seed);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (size_t i = 0; i < size; ++i) {
        prio[i] = dis(gen);
    }

    // Test heaps with custom priorities.
    d_ary_heap_test_update<uint8_t, 1>(size, prio, r_seed);
    d_ary_heap_test_update<uint8_t, 2>(size, prio, r_seed);
    d_ary_heap_test_update<uint8_t, 3>(size, prio, r_seed);
    d_ary_heap_test_update<uint16_t>(size, prio, r_seed);
    d_ary_heap_test_update<uint32_t>(size, prio, r_seed);
    d_ary_heap_test_update<uint64_t>(size, prio, r_seed);

    return 0;
}

/******************************************************************************/
