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

#include <tlx/container/d_ary_addressable_int_heap.hpp>
#include <tlx/container/d_ary_heap.hpp>
#include <tlx/die.hpp>

// Force instantiation.
namespace tlx {

template class DAryHeap<uint8_t>;
template class DAryHeap<uint16_t>;
template class DAryHeap<uint32_t>;
template class DAryHeap<uint64_t>;

template class DAryAddressableIntHeap<uint8_t>;
template class DAryAddressableIntHeap<uint16_t>;
template class DAryAddressableIntHeap<uint32_t>;
template class DAryAddressableIntHeap<uint64_t>;

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

struct TestData {
    unsigned int a, b;

    // required by the heap
    TestData()
        : a(0), b(0)
    { }

    // also used as implicit conversion constructor
    TestData(unsigned int _a)
        : a(_a), b(0)
    { }

    friend std::ostream& operator << (std::ostream& os, const TestData& d) {
        return os << d.a;
    }

    bool operator == (const TestData& o) const {
        return a == o.a && b == o.b;
    }
};

struct TestCompare {
    unsigned int somevalue;

    TestCompare(unsigned int sv = 0)
        : somevalue(sv)
    { }

    bool operator () (const struct TestData& a, const struct TestData& b) const {
        return a.a > b.a;
    }
};

template <typename KeyType>
std::vector<KeyType> get_shuffled_vector(size_t size, uint32_t r_seed) {
    std::vector<KeyType> keys(size);
    for (size_t key = 0; key < size; ++key) {
        keys[key] = KeyType(key);
    }

    std::mt19937 gen(r_seed);
    std::shuffle(keys.begin(), keys.end(), gen);
    return keys;
}

//! Verifies the heap property, the size of the heap, and the top item.
template <typename DAryHeap, typename Set>
void check_heap(DAryHeap& heap, Set& set) {
    die_unless(heap.sanity_check());
    die_unequal(heap.size(), set.size());
    if (!heap.empty()) {
        die_unequal(heap.top(), *(set.begin()));
    }
}

template <typename DAryAddressableIntHeap, typename Set>
void check_handles(DAryAddressableIntHeap& heap, Set& set) {
    for (auto elem = set.begin(); elem != set.end(); ++elem)
        die_unless(heap.contains(*elem));
}

template <typename DAryHeap, typename Set, typename KeysVector>
void fill_heap_and_set(DAryHeap& heap, Set& set, KeysVector& keys) {
    for (const typename KeysVector::value_type& key : keys) {
        heap.push(key);
        set.insert(key);
        check_heap(heap, set);
    }
}

//! Basic APIs: push(), top(), and pop().
template <typename KeyType, unsigned Arity = 2,
          class Compare = std::less<KeyType> >
void d_ary_heap_test(size_t size, uint32_t r_seed = 42) {
    tlx::DAryHeap<KeyType, Arity, Compare> x;
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

    // Test build_heap().
    x.clear();
    x.build_heap(s.begin(), s.end());
    check_heap(x, s);

    tlx::DAryHeap<KeyType, Arity, Compare> y, z;
    y.build_heap(keys);
    check_heap(y, s);

    z.build_heap(std::move(keys));
    check_heap(z, s);
}

//! Basic APIs: push(), top(), pop(), and remove().
template <typename KeyType, unsigned Arity = 2,
          class Compare = std::less<KeyType> >
void d_ary_addressable_int_heap_test(size_t size, uint32_t r_seed = 42) {
    tlx::DAryAddressableIntHeap<KeyType, Arity, Compare> x;
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
        check_handles(x, s);
    }

    std::mt19937 gen(r_seed);
    std::shuffle(keys.begin(), keys.end(), gen);
    fill_heap_and_set(x, s, keys);

    // Test remove().
    for (auto key : keys) {
        x.remove(key);
        s.erase(s.find(key));
        check_heap(x, s);
        check_handles(x, s);
    }


    // Test inserting and building afterwards
    fill_heap_and_set(x, s, keys);
    x.clear();
    for (auto key : keys) {
        x.push_without_update(key);
    }
    x.update_all();
    check_heap(x, s);
    check_handles(x, s);
    x.clear();
    s.clear();

    // Test build_heap().
    fill_heap_and_set(x, s, keys);
    x.clear();
    x.build_heap(keys);
    check_heap(x, s);
    check_handles(x, s);

    tlx::DAryAddressableIntHeap<KeyType, Arity, Compare> y, z;
    y.build_heap(s.begin(), s.end());
    check_heap(y, s);
    check_handles(y, s);

    z.build_heap(std::move(keys));
    check_heap(z, s);
    check_handles(z, s);
}

//! Tests update().
template <typename KeyType, unsigned Arity = 2>
void d_ary_heap_test_update(size_t size, std::vector<double>& prio,
                            uint32_t r_seed = 42) {
    tlx::DAryAddressableIntHeap<KeyType, Arity, Comparator<KeyType> > x{
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
        die_unless(x.sanity_check());
    }

    // Update the set.
    s.clear();
    for (auto key : keys) {
        s.insert(key);
    }
    check_heap(x, s);

    //! Test update_all().
    std::shuffle(prio.begin(), prio.end(), gen);
    x.update_all();

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
    d_ary_heap_test<uint8_t, 4>(size, r_seed);
    d_ary_heap_test<uint8_t, 6>(size, r_seed);
    d_ary_heap_test<uint8_t, 13>(size, r_seed);
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

    // Basic heap API with custom struct.
    d_ary_heap_test<TestData, 2, TestCompare>(size, r_seed);
    d_ary_heap_test<TestData, 3, TestCompare>(size, r_seed);

    // Basic heap APIs with default compare functions.
    d_ary_addressable_int_heap_test<uint8_t, 1>(size, r_seed);
    d_ary_addressable_int_heap_test<uint8_t, 2>(size, r_seed);
    d_ary_addressable_int_heap_test<uint8_t, 3>(size, r_seed);
    d_ary_addressable_int_heap_test<uint8_t, 4>(size, r_seed);
    d_ary_addressable_int_heap_test<uint8_t, 6>(size, r_seed);
    d_ary_addressable_int_heap_test<uint8_t, 13>(size, r_seed);
    d_ary_addressable_int_heap_test<uint16_t, 1>(size, r_seed);
    d_ary_addressable_int_heap_test<uint16_t, 2>(size, r_seed);
    d_ary_addressable_int_heap_test<uint16_t, 3>(size, r_seed);
    d_ary_addressable_int_heap_test<uint32_t, 1>(size, r_seed);
    d_ary_addressable_int_heap_test<uint32_t, 2>(size, r_seed);
    d_ary_addressable_int_heap_test<uint32_t, 3>(size, r_seed);
    d_ary_addressable_int_heap_test<uint32_t, 2>(size, r_seed);
    d_ary_addressable_int_heap_test<uint32_t, 3>(size, r_seed);
    d_ary_addressable_int_heap_test<uint64_t, 1>(size, r_seed);
    d_ary_addressable_int_heap_test<uint64_t, 2>(size, r_seed);
    d_ary_addressable_int_heap_test<uint64_t, 3>(size, r_seed);

    d_ary_addressable_int_heap_test<uint8_t, 2, std::greater<uint8_t> >(size, r_seed);
    d_ary_addressable_int_heap_test<uint16_t, 2, std::greater<uint16_t> >(size, r_seed);
    d_ary_addressable_int_heap_test<uint32_t, 2, std::greater<uint32_t> >(size, r_seed);
    d_ary_addressable_int_heap_test<uint64_t, 2, std::greater<uint64_t> >(size, r_seed);

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
