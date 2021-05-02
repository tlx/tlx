/*******************************************************************************
 * tests/container/btree_speedtest.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2008-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include <set>
#include <tlx/container/btree_multiset.hpp>
#include <tlx/container/splay_tree.hpp>
#include <unordered_set>

#include <map>
#include <tlx/container/btree_multimap.hpp>
#include <unordered_map>

#include <tlx/die.hpp>
#include <tlx/timestamp.hpp>

// *** Settings

//! starting number of items to insert
const size_t min_items = 125;

//! maximum number of items to insert
const size_t max_items = 1024000 * 64;

//! random seed
const int seed = 34234235;

//! Traits used for the speed tests, BTREE_DEBUG is not defined.
template <int InnerSlots, int LeafSlots>
struct btree_traits_speed : tlx::btree_default_traits<size_t, size_t> {
    static const bool self_verify = false;
    static const bool debug = false;

    static const int leaf_slots = InnerSlots;
    static const int inner_slots = LeafSlots;

    static const size_t binsearch_threshold = 256 * 1024 * 1024; // never
};

// -----------------------------------------------------------------------------

//! Test a generic set type with insertions
template <typename SetType>
class Test_Set_Insert
{
public:
    Test_Set_Insert(size_t) { }

    static const char * op() { return "set_insert"; }

    void run(size_t items) {
        SetType set;

        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++)
            set.insert(rng());

        die_unless(set.size() == items);
    }
};

//! Test a generic set type with insert, find and delete sequences
template <typename SetType>
class Test_Set_InsertFindDelete
{
public:
    Test_Set_InsertFindDelete(size_t) { }

    static const char * op() { return "set_insert_find_delete"; }

    void run(size_t items) {
        SetType set;

        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++)
            set.insert(rng());

        die_unless(set.size() == items);

        rng.seed(seed);
        for (size_t i = 0; i < items; i++)
            set.find(rng());

        rng.seed(seed);
        for (size_t i = 0; i < items; i++)
            set.erase(set.find(rng()));

        die_unless(set.empty());
    }
};

//! Test a generic set type with insert, find and delete sequences
template <typename SetType>
class Test_Set_Find
{
public:
    SetType set;

    static const char * op() { return "set_find"; }

    Test_Set_Find(size_t items) {
        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++)
            set.insert(rng());

        die_unless(set.size() == items);
    }

    void run(size_t items) {
        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++)
            set.find(rng());
    }
};

//! Construct different set types for a generic test class
template <template <typename SetType> class TestClass>
struct TestFactory_Set {
    //! Test the multiset red-black tree from STL
    typedef TestClass<std::multiset<size_t> > StdSet;

    //! Test the multiset red-black tree from STL
    typedef TestClass<tlx::splay_multiset<size_t> > SplaySet;

    //! Test the unordered_set from STL TR1
    typedef TestClass<std::unordered_multiset<size_t> > UnorderedSet;

    //! Test the B+ tree with a specific leaf/inner slots
    template <int Slots>
    struct BtreeSet
        : TestClass<tlx::btree_multiset<
                        size_t, std::less<size_t>,
                        struct btree_traits_speed<Slots, Slots> > > {
        BtreeSet(size_t n)
            : TestClass<tlx::btree_multiset<
                            size_t, std::less<size_t>,
                            struct btree_traits_speed<Slots, Slots> > >(n) { }
    };

    //! Run tests on all set types
    void call_testrunner(size_t items);
};

// -----------------------------------------------------------------------------

//! Test a generic map type with insertions
template <typename MapType>
class Test_Map_Insert
{
public:
    Test_Map_Insert(size_t) { }

    static const char * op() { return "map_insert"; }

    void run(size_t items) {
        MapType map;

        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++) {
            size_t r = rng();
            map.insert(std::make_pair(r, r));
        }

        die_unless(map.size() == items);
    }
};

//! Test a generic map type with insert, find and delete sequences
template <typename MapType>
class Test_Map_InsertFindDelete
{
public:
    Test_Map_InsertFindDelete(size_t) { }

    static const char * op() { return "map_insert_find_delete"; }

    void run(size_t items) {
        MapType map;

        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++) {
            size_t r = rng();
            map.insert(std::make_pair(r, r));
        }

        die_unless(map.size() == items);

        rng.seed(seed);
        for (size_t i = 0; i < items; i++)
            map.find(rng());

        rng.seed(seed);
        for (size_t i = 0; i < items; i++)
            map.erase(map.find(rng()));

        die_unless(map.empty());
    }
};

//! Test a generic map type with insert, find and delete sequences
template <typename MapType>
class Test_Map_Find
{
public:
    MapType map;

    static const char * op() { return "map_find"; }

    Test_Map_Find(size_t items) {
        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++) {
            size_t r = rng();
            map.insert(std::make_pair(r, r));
        }

        die_unless(map.size() == items);
    }

    void run(size_t items) {
        std::default_random_engine rng(seed);
        for (size_t i = 0; i < items; i++)
            map.find(rng());
    }
};

//! Construct different map types for a generic test class
template <template <typename MapType> class TestClass>
struct TestFactory_Map {
    //! Test the multimap red-black tree from STL
    typedef TestClass<std::multimap<size_t, size_t> > StdMap;

    //! Test the unordered_map from STL
    typedef TestClass<std::unordered_multimap<size_t, size_t> > UnorderedMap;

    //! Test the B+ tree with a specific leaf/inner slots
    template <int Slots>
    struct BtreeMap
        : TestClass<tlx::btree_multimap<
                        size_t, size_t, std::less<size_t>,
                        struct btree_traits_speed<Slots, Slots> > > {
        BtreeMap(size_t n)
            : TestClass<tlx::btree_multimap<
                            size_t, size_t, std::less<size_t>,
                            struct btree_traits_speed<Slots, Slots> > >(n) { }
    };

    //! Run tests on all map types
    void call_testrunner(size_t items);
};

// -----------------------------------------------------------------------------

size_t repeat_until;

//! Repeat (short) tests until enough time elapsed and divide by the repeat.
template <typename TestClass>
void testrunner_loop(size_t items, const std::string& container_name) {

    size_t repeat = 0;
    double ts1, ts2;

    do
    {
        // count repetition of timed tests
        repeat = 0;

        {
            // initialize test structures
            TestClass test(items);

            ts1 = tlx::timestamp();

            for (size_t r = 0; r <= repeat_until; r += items)
            {
                // run timed test procedure
                test.run(items);
                ++repeat;
            }

            ts2 = tlx::timestamp();
        }

        std::cout << "Insert " << items << " repeat " << (repeat_until / items)
                  << " time " << (ts2 - ts1) << "\n";

        // discard and repeat if test took less than one second.
        if ((ts2 - ts1) < 1.0) repeat_until *= 2;
    }
    while ((ts2 - ts1) < 1.0);

    std::cout << "RESULT"
              << " container=" << container_name
              << " op=" << TestClass::op()
              << " items=" << items
              << " repeat=" << repeat
              << " time_total=" << (ts2 - ts1)
              << " time="
              << std::fixed << std::setprecision(10) << ((ts2 - ts1) / repeat)
              << " items_per_sec=" << items / (ts2 - ts1)
              << std::endl;
}

// Template magic to emulate a for_each slots. These templates will roll-out
// btree instantiations for each of the Low-High leaf/inner slot numbers.
template <template <int Slots> class Functional, int Low, int High>
struct btree_range {
    void operator () (size_t items, const std::string& container_name) {
        testrunner_loop<Functional<Low> >(
            items, container_name + "<" + std::to_string(Low) + ">"
            " slots=" + std::to_string(Low));
        btree_range<Functional, Low + 2, High>()(items, container_name);
    }
};

template <template <int Slots> class Functional, int Low>
struct btree_range<Functional, Low, Low> {
    void operator () (size_t items, const std::string& container_name) {
        testrunner_loop<Functional<Low> >(
            items, container_name + "<" + std::to_string(Low) + ">"
            " slots=" + std::to_string(Low));
    }
};

template <template <typename Type> class TestClass>
void TestFactory_Set<TestClass>::call_testrunner(size_t items) {

    testrunner_loop<StdSet>(items, "std::multiset");
    testrunner_loop<UnorderedSet>(items, "std::unordered_multiset");
    testrunner_loop<SplaySet>(items, "tlx::splay_multiset");

#if 0
    btree_range<BtreeSet, min_nodeslots, max_nodeslots>()(
        items, "tlx::btree_multiset");
#else
    // just pick a few node sizes for quicker tests
    testrunner_loop<BtreeSet<4> >(items, "tlx::btree_multiset<4> slots=4");
    testrunner_loop<BtreeSet<8> >(items, "tlx::btree_multiset<8> slots=8");
    testrunner_loop<BtreeSet<16> >(items, "tlx::btree_multiset<16> slots=16");
    testrunner_loop<BtreeSet<32> >(items, "tlx::btree_multiset<32> slots=32");
    testrunner_loop<BtreeSet<64> >(items, "tlx::btree_multiset<64> slots=64");
    testrunner_loop<BtreeSet<128> >(
        items, "tlx::btree_multiset<128> slots=128");
    testrunner_loop<BtreeSet<256> >(
        items, "tlx::btree_multiset<256> slots=256");
#endif
}

template <template <typename Type> class TestClass>
void TestFactory_Map<TestClass>::call_testrunner(size_t items) {

    testrunner_loop<StdMap>(items, "std::multimap");
    testrunner_loop<UnorderedMap>(items, "std::unordered_multimap");

#if 0
    btree_range<BtreeMap, min_nodeslots, max_nodeslots>()(
        items, "tlx::btree_multimap");
#else
    // just pick a few node sizes for quicker tests
    testrunner_loop<BtreeMap<4> >(items, "tlx::btree_multimap<4> slots=4");
    testrunner_loop<BtreeMap<8> >(items, "tlx::btree_multimap<8> slots=8");
    testrunner_loop<BtreeMap<16> >(items, "tlx::btree_multimap<16> slots=16");
    testrunner_loop<BtreeMap<32> >(items, "tlx::btree_multimap<32> slots=32");
    testrunner_loop<BtreeMap<64> >(items, "tlx::btree_multimap<64> slots=64");
    testrunner_loop<BtreeMap<128> >(
        items, "tlx::btree_multimap<128> slots=128");
    testrunner_loop<BtreeMap<256> >(
        items, "tlx::btree_multimap<256> slots=256");
#endif
}

//! Speed test them!
int main() {
    {   // Set - speed test only insertion

        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2)
        {
            std::cout << "set: insert " << items << "\n";
            TestFactory_Set<Test_Set_Insert>().call_testrunner(items);
        }
    }

    {   // Set - speed test insert, find and delete

        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2)
        {
            std::cout << "set: insert, find, delete " << items << "\n";
            TestFactory_Set<Test_Set_InsertFindDelete>().call_testrunner(items);
        }
    }

    {   // Set - speed test find only

        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2)
        {
            std::cout << "set: find " << items << "\n";
            TestFactory_Set<Test_Set_Find>().call_testrunner(items);
        }
    }

    {   // Map - speed test only insertion

        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2)
        {
            std::cout << "map: insert " << items << "\n";
            TestFactory_Map<Test_Map_Insert>().call_testrunner(items);
        }
    }

    {   // Map - speed test insert, find and delete

        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2)
        {
            std::cout << "map: insert, find, delete " << items << "\n";
            TestFactory_Map<Test_Map_InsertFindDelete>().call_testrunner(items);
        }
    }

    {   // Map - speed test find only

        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2)
        {
            std::cout << "map: find " << items << "\n";
            TestFactory_Map<Test_Map_Find>().call_testrunner(items);
        }
    }

    return 0;
}

/******************************************************************************/
