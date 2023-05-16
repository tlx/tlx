/*******************************************************************************
 * tests/container/d_ary_heap_speedtest.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2008-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

#include <queue>
#include <tlx/container/d_ary_addressable_int_heap.hpp>
#include <tlx/container/d_ary_heap.hpp>

#include <tlx/die.hpp>
#include <tlx/timestamp.hpp>

// *** Settings

//! starting number of items to insert
const size_t min_items = 125;

//! maximum number of items to insert
const size_t max_items = 1024000 * 128;

// -----------------------------------------------------------------------------

//! Fill a generic heap type with pushes
template <typename HeapType>
class Test_Heap_Fill
{
public:
    Test_Heap_Fill(size_t) { }

    static const char * op() { return "heap_fill"; }

    void run(size_t items) {
        HeapType heap;

        for (size_t i = 0; i < items; i++)
            heap.push(items - i);

        die_unless(heap.size() == items);
    }
};

//! Test a generic heap type by filling and emptying it
template <typename HeapType>
class Test_Heap_FillPopAll
{
public:
    Test_Heap_FillPopAll(size_t) { }

    static const char * op() { return "heap_fill_popall"; }

    void run(size_t items) {
        HeapType heap;

        for (size_t i = 0; i < items; i++)
            heap.push(items - i);

        die_unless(heap.size() == items);

        for (size_t i = 0; i < items; i++)
            heap.pop();

        die_unless(heap.empty());
    }
};

//! Test a generic heap type by filling and cycling it.
template <typename HeapType>
class Test_Heap_FillCycle
{
public:
    HeapType heap_;

    Test_Heap_FillCycle(size_t items) {
        // fill up heap
        for (size_t i = 0; i < items; i++)
            heap_.push(items - i);

        die_unless(heap_.size() == items);
    }

    static const char * op() { return "heap_fill_cycle"; }

    void run(size_t items) {
        HeapType heap = heap_;

        // cycle heap once
        for (size_t i = 0; i < items; i++) {
            heap.pop();
            heap.push(2 * items - i);
        }

        die_unless(heap.size() == items);
    }
};

// -----------------------------------------------------------------------------

//! Construct different heap types for a generic test class
template <template <typename HeapType> class TestClass>
struct TestFactory_Heap {
    //! Test the binary heap from STL
    using StdQueue = TestClass<
        std::priority_queue<std::uint32_t, std::vector<std::uint32_t>,
                            std::greater<std::uint32_t> > >;

    //! Test the d-ary heap with a specific arity
    template <int Arity>
    using DAryHeap = TestClass<tlx::DAryHeap<std::uint32_t, Arity> >;

    //! Test the d-ary heap with a specific arity
    template <int Arity>
    using DAryAIntHeap =
        TestClass<tlx::DAryAddressableIntHeap<std::uint32_t, Arity> >;

    //! Run tests on all heap types
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
              << std::endl;
}

template <template <typename Type> class TestClass>
void TestFactory_Heap<TestClass>::call_testrunner(size_t items) {

    testrunner_loop<StdQueue>(items, "std::priority_queue");

    testrunner_loop<DAryHeap<2> >(items, "tlx::DAryHeap<2> slots=2");
    testrunner_loop<DAryHeap<3> >(items, "tlx::DAryHeap<3> slots=3");
    testrunner_loop<DAryHeap<4> >(items, "tlx::DAryHeap<4> slots=4");
    testrunner_loop<DAryHeap<6> >(items, "tlx::DAryHeap<6> slots=6");
    testrunner_loop<DAryHeap<8> >(items, "tlx::DAryHeap<8> slots=8");
    testrunner_loop<DAryHeap<16> >(items, "tlx::DAryHeap<16> slots=16");
    testrunner_loop<DAryHeap<32> >(items, "tlx::DAryHeap<32> slots=32");

    testrunner_loop<DAryAIntHeap<2> >(items, "tlx::DAryAIntHeap<2> slots=2");
    testrunner_loop<DAryAIntHeap<3> >(items, "tlx::DAryAIntHeap<3> slots=3");
    testrunner_loop<DAryAIntHeap<4> >(items, "tlx::DAryAIntHeap<4> slots=4");
    testrunner_loop<DAryAIntHeap<6> >(items, "tlx::DAryAIntHeap<6> slots=6");
    testrunner_loop<DAryAIntHeap<8> >(items, "tlx::DAryAIntHeap<8> slots=8");
    testrunner_loop<DAryAIntHeap<16> >(items, "tlx::DAryAIntHeap<16> slots=16");
    testrunner_loop<DAryAIntHeap<32> >(items, "tlx::DAryAIntHeap<32> slots=32");
}

//! Speed test them!
int main() {
    // Heap - speed test fill
    {
        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2) {
            std::cout << "heap: fill " << items << "\n";
            TestFactory_Heap<Test_Heap_Fill>().call_testrunner(items);
        }
    }

    // Heap - speed test fill and pop all
    {
        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2) {
            std::cout << "heap: fill, pop all " << items << "\n";
            TestFactory_Heap<Test_Heap_FillPopAll>().call_testrunner(items);
        }
    }

    // Heap - speed test a pop-push cycle
    {
        repeat_until = min_items;

        for (size_t items = min_items; items <= max_items; items *= 2) {
            std::cout << "heap: cycle " << items << "\n";
            TestFactory_Heap<Test_Heap_FillCycle>().call_testrunner(items);
        }
    }

    return 0;
}

/******************************************************************************/
