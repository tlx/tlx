/*******************************************************************************
 * tests/btree_speedtest.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2008-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <sys/time.h>

#include <set>
#include <tlx/btree_multiset.hpp>
#include <unordered_set>

#include <map>
#include <tlx/btree_multimap.hpp>
#include <unordered_map>

// *** Settings

/// starting number of items to insert
const size_t minitems = 125;

/// maximum number of items to insert
const size_t maxitems = 1024000 * 64;

const int randseed = 34234235;

/// b+ tree slot range to test
const int min_nodeslots = 4;
const int max_nodeslots = 256;

/// Time is measured using gettimeofday()
static inline double timestamp() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec + tv.tv_usec * 0.000001;
}

/// Traits used for the speed tests, BTREE_DEBUG is not defined.
template <int InnerSlots_, int LeafSlots_>
struct btree_traits_speed : tlx::btree_default_traits<size_t, size_t>{
    static const bool   selfverify = false;
    static const bool   debug = false;

    static const int    leafslots = InnerSlots_;
    static const int    innerslots = LeafSlots_;

    static const size_t binsearch_threshold = 256 * 1024 * 1024; // never
};

// -----------------------------------------------------------------------------

/// Test a generic set type with insertions
template <typename SetType>
struct Test_Set_Insert {
    Test_Set_Insert(size_t) { }

    inline void run(size_t items) {
        SetType set;

        srand(randseed);
        for (size_t i = 0; i < items; i++)
            set.insert(rand());

        assert(set.size() == items);
    }
};

/// Test a generic set type with insert, find and delete sequences
template <typename SetType>
struct Test_Set_InsertFindDelete {
    Test_Set_InsertFindDelete(size_t) { }

    inline void run(size_t items) {
        SetType set;

        srand(randseed);
        for (size_t i = 0; i < items; i++)
            set.insert(rand());

        assert(set.size() == items);

        srand(randseed);
        for (size_t i = 0; i < items; i++)
            set.find(rand());

        srand(randseed);
        for (size_t i = 0; i < items; i++)
            set.erase(set.find(rand()));

        assert(set.empty());
    }
};

/// Test a generic set type with insert, find and delete sequences
template <typename SetType>
struct Test_Set_Find {
    SetType set;

    Test_Set_Find(size_t items) {
        srand(randseed);
        for (size_t i = 0; i < items; i++)
            set.insert(rand());

        assert(set.size() == items);
    }

    void    run(size_t items) {
        srand(randseed);
        for (size_t i = 0; i < items; i++)
            set.find(rand());
    }
};

/// Construct different set types for a generic test class
template <template <typename SetType> class TestClass>
struct TestFactory_Set {
    /// Test the multiset red-black tree from STL
    typedef TestClass<std::multiset<size_t> > StdSet;

    /// Test the unordered_set from STL TR1
    typedef TestClass<std::unordered_multiset<size_t> > UnorderedSet;

    /// Test the B+ tree with a specific leaf/inner slots
    template <int Slots>
    struct BtreeSet
        : TestClass<tlx::btree_multiset<
                        size_t, std::less<size_t>,
                        struct btree_traits_speed<Slots, Slots> > >{
        BtreeSet(size_t n)
            : TestClass<tlx::btree_multiset<
                            size_t, std::less<size_t>,
                            struct btree_traits_speed<Slots, Slots> > >(n) { }
    };

    /// Run tests on all set types
    void call_testrunner(std::ostream& os, size_t items);
};

// -----------------------------------------------------------------------------

/// Test a generic map type with insertions
template <typename MapType>
struct Test_Map_Insert {
    Test_Map_Insert(size_t) { }

    inline void run(size_t items) {
        MapType map;

        srand(randseed);
        for (size_t i = 0; i < items; i++) {
            size_t r = rand();
            map.insert(std::make_pair(r, r));
        }

        assert(map.size() == items);
    }
};

/// Test a generic map type with insert, find and delete sequences
template <typename MapType>
struct Test_Map_InsertFindDelete {
    Test_Map_InsertFindDelete(size_t) { }

    inline void run(size_t items) {
        MapType map;

        srand(randseed);
        for (size_t i = 0; i < items; i++) {
            size_t r = rand();
            map.insert(std::make_pair(r, r));
        }

        assert(map.size() == items);

        srand(randseed);
        for (size_t i = 0; i < items; i++)
            map.find(rand());

        srand(randseed);
        for (size_t i = 0; i < items; i++)
            map.erase(map.find(rand()));

        assert(map.empty());
    }
};

/// Test a generic map type with insert, find and delete sequences
template <typename MapType>
struct Test_Map_Find {
    MapType map;

    Test_Map_Find(size_t items) {
        srand(randseed);
        for (size_t i = 0; i < items; i++) {
            size_t r = rand();
            map.insert(std::make_pair(r, r));
        }

        assert(map.size() == items);
    }

    void    run(size_t items) {
        srand(randseed);
        for (size_t i = 0; i < items; i++)
            map.find(rand());
    }
};

/// Construct different map types for a generic test class
template <template <typename MapType> class TestClass>
struct TestFactory_Map {
    /// Test the multimap red-black tree from STL
    typedef TestClass<std::multimap<size_t, size_t> > StdMap;

    /// Test the unordered_map from STL
    typedef TestClass<std::unordered_multimap<size_t, size_t> > UnorderedMap;

    /// Test the B+ tree with a specific leaf/inner slots
    template <int Slots>
    struct BtreeMap
        : TestClass<tlx::btree_multimap<
                        size_t, size_t, std::less<size_t>,
                        struct btree_traits_speed<Slots, Slots> > >{
        BtreeMap(size_t n)
            : TestClass<tlx::btree_multimap<
                            size_t, size_t, std::less<size_t>,
                            struct btree_traits_speed<Slots, Slots> > >(n) { }
    };

    /// Run tests on all map types
    void call_testrunner(std::ostream& os, size_t items);
};

// -----------------------------------------------------------------------------

size_t repeatuntil;

/// Repeat (short) tests until enough time elapsed and divide by the runs.
template <typename TestClass>
void testrunner_loop(std::ostream& os, size_t items) {
    size_t runs = 0;
    double ts1, ts2;

    do
    {
        // count repetition of timed tests
        runs = 0;

        {
            // initialize test structures
            TestClass test(items);

            ts1 = timestamp();

            for (size_t r = 0; r <= repeatuntil; r += items)
            {
                // run timed test procedure
                test.run(items);
                ++runs;
            }

            ts2 = timestamp();
        }

        std::cerr << "Insert " << items << " repeat " << (repeatuntil / items)
                  << " time " << (ts2 - ts1) << "\n";

        // discard and repeat if test took less than one second.
        if ((ts2 - ts1) < 1.0) repeatuntil *= 2;
    }
    while ((ts2 - ts1) < 1.0);

    os << std::fixed << std::setprecision(10) << ((ts2 - ts1) / runs) << " " << std::flush;
}

// Template magic to emulate a for_each slots. These templates will roll-out
// btree instantiations for each of the Low-High leaf/inner slot numbers.
template <template <int Slots> class functional, int Low, int High>
struct btree_range {
    inline void operator () (std::ostream& os, size_t items) {
        testrunner_loop<functional<Low> >(os, items);
        btree_range<functional, Low + 2, High>()(os, items);
    }
};

template <template <int Slots> class functional, int Low>
struct btree_range<functional, Low, Low>{
    inline void operator () (std::ostream& os, size_t items) {
        testrunner_loop<functional<Low> >(os, items);
    }
};

template <template <typename Type> class TestClass>
void TestFactory_Set<TestClass>::call_testrunner(std::ostream& os, size_t items) {
    os << items << " " << std::flush;

    testrunner_loop<StdSet>(os, items);
    testrunner_loop<UnorderedSet>(os, items);

#if 0
    btree_range<BtreeSet, min_nodeslots, max_nodeslots>()(os, items);
#else
    // just pick a few node sizes for quicker tests
    testrunner_loop<BtreeSet<4> >(os, items);
    for (int i = 6; i < 8; i += 2) os << "0 ";
    testrunner_loop<BtreeSet<8> >(os, items);
    for (int i = 10; i < 16; i += 2) os << "0 ";
    testrunner_loop<BtreeSet<16> >(os, items);
    for (int i = 18; i < 32; i += 2) os << "0 ";
    testrunner_loop<BtreeSet<32> >(os, items);
    for (int i = 34; i < 64; i += 2) os << "0 ";
    testrunner_loop<BtreeSet<64> >(os, items);
    for (int i = 66; i < 128; i += 2) os << "0 ";
    testrunner_loop<BtreeSet<128> >(os, items);
    for (int i = 130; i < 256; i += 2) os << "0 ";
    testrunner_loop<BtreeSet<256> >(os, items);
#endif

    os << "\n" << std::flush;
}

template <template <typename Type> class TestClass>
void TestFactory_Map<TestClass>::call_testrunner(std::ostream& os, size_t items) {
    os << items << " " << std::flush;

    testrunner_loop<StdMap>(os, items);
    testrunner_loop<UnorderedMap>(os, items);

#if 0
    btree_range<BtreeMap, min_nodeslots, max_nodeslots>()(os, items);
#else
    // just pick a few node sizes for quicker tests
    testrunner_loop<BtreeMap<4> >(os, items);
    for (int i = 6; i < 8; i += 2) os << "0 ";
    testrunner_loop<BtreeMap<8> >(os, items);
    for (int i = 10; i < 16; i += 2) os << "0 ";
    testrunner_loop<BtreeMap<16> >(os, items);
    for (int i = 18; i < 32; i += 2) os << "0 ";
    testrunner_loop<BtreeMap<32> >(os, items);
    for (int i = 34; i < 64; i += 2) os << "0 ";
    testrunner_loop<BtreeMap<64> >(os, items);
    for (int i = 66; i < 128; i += 2) os << "0 ";
    testrunner_loop<BtreeMap<128> >(os, items);
    for (int i = 130; i < 256; i += 2) os << "0 ";
    testrunner_loop<BtreeMap<256> >(os, items);
#endif

    os << "\n" << std::flush;
}

/// Speed test them!
int main() {
    {   // Set - speed test only insertion

        std::ofstream os("speed-set-insert.txt");

        repeatuntil = minitems;

        for (size_t items = minitems; items <= maxitems; items *= 2)
        {
            std::cerr << "set: insert " << items << "\n";
            TestFactory_Set<Test_Set_Insert>().call_testrunner(os, items);
        }
    }

    {   // Set - speed test insert, find and delete

        std::ofstream os("speed-set-all.txt");

        repeatuntil = minitems;

        for (size_t items = minitems; items <= maxitems; items *= 2)
        {
            std::cerr << "set: insert, find, delete " << items << "\n";
            TestFactory_Set<Test_Set_InsertFindDelete>().call_testrunner(os, items);
        }
    }

    {   // Set - speed test find only

        std::ofstream os("speed-set-find.txt");

        repeatuntil = minitems;

        for (size_t items = minitems; items <= maxitems; items *= 2)
        {
            std::cerr << "set: find " << items << "\n";
            TestFactory_Set<Test_Set_Find>().call_testrunner(os, items);
        }
    }

    {   // Map - speed test only insertion

        std::ofstream os("speed-map-insert.txt");

        repeatuntil = minitems;

        for (size_t items = minitems; items <= maxitems; items *= 2)
        {
            std::cerr << "map: insert " << items << "\n";
            TestFactory_Map<Test_Map_Insert>().call_testrunner(os, items);
        }
    }

    {   // Map - speed test insert, find and delete

        std::ofstream os("speed-map-all.txt");

        repeatuntil = minitems;

        for (size_t items = minitems; items <= maxitems; items *= 2)
        {
            std::cerr << "map: insert, find, delete " << items << "\n";
            TestFactory_Map<Test_Map_InsertFindDelete>().call_testrunner(os, items);
        }
    }

    {   // Map - speed test find only

        std::ofstream os("speed-map-find.txt");

        repeatuntil = minitems;

        for (size_t items = minitems; items <= maxitems; items *= 2)
        {
            std::cerr << "map: find " << items << "\n";
            TestFactory_Map<Test_Map_Find>().call_testrunner(os, items);
        }
    }

    return 0;
}

/******************************************************************************/
