/*******************************************************************************
 * tests/container/loser_tree_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include <tlx/cmdline_parser.hpp>
#include <tlx/container/loser_tree.hpp>
#include <tlx/die.hpp>
#include <tlx/logger.hpp>

static long ctor_dtor_counter = 0;

//! object to track construction and destruction
struct MyTracker {
    MyTracker() {
        ++ctor_dtor_counter;
    }
    MyTracker(const MyTracker&) noexcept { // NOLINT
        ++ctor_dtor_counter;
    }
    MyTracker& operator = (const MyTracker&) noexcept { // NOLINT
        // no change
        return *this;
    }
    ~MyTracker() {
        --ctor_dtor_counter;
    }
};

//! struct with one integer
struct MyInt {
    size_t value_ = 1;

    //! LoserTreeCopy needs default constructor
    MyInt() = default;

    explicit MyInt(size_t value) : value_(value) { }

    bool operator == (const MyInt& b) const {
        return value_ == b.value_;
    }
};

struct MyIntCompare {
    bool operator () (const MyInt& a, const MyInt& b) const {
        return a.value_ < b.value_;
    }
};

//! struct with two integers
struct MyIntPair {
    size_t key_ = 1;
    size_t value_ = 42;

    //! tracker to count constructions/destructions
    MyTracker track_;

    //! LoserTreeCopy needs default constructor
    MyIntPair() = default;

    MyIntPair(size_t value, size_t dummy)
        : key_(value), value_(dummy) { }

    bool operator == (const MyIntPair& b) const {
        return key_ == b.key_ && value_ == b.value_;
    }
};

struct MyIntPairCompare {
    bool operator () (const MyIntPair& a, const MyIntPair& b) const {
        return a.key_ < b.key_;
    }
};

// force instantiation
namespace tlx {

template class LoserTreeCopy<false, MyIntPair, MyIntPairCompare>;
template class LoserTreeCopy<true, MyIntPair, MyIntPairCompare>;
template class LoserTreeCopyBase<MyIntPair, MyIntPairCompare>;

template class LoserTreeCopyUnguarded<false, MyIntPair, MyIntPairCompare>;
template class LoserTreeCopyUnguarded<true, MyIntPair, MyIntPairCompare>;
template class LoserTreeCopyUnguardedBase<MyIntPair, MyIntPairCompare>;

template class LoserTreePointer<false, MyIntPair, MyIntPairCompare>;
template class LoserTreePointer<true, MyIntPair, MyIntPairCompare>;
template class LoserTreePointerBase<MyIntPair, MyIntPairCompare>;

template class LoserTreePointerUnguarded<false, MyIntPair, MyIntPairCompare>;
template class LoserTreePointerUnguarded<true, MyIntPair, MyIntPairCompare>;
template class LoserTreePointerUnguardedBase<MyIntPair, MyIntPairCompare>;

} // namespace tlx

/******************************************************************************/
// test_losertree

template <typename LoserTree>
static inline
void test_losertree(bool stable, size_t num_vectors) {

    sLOG1 << "test_losertree:" << stable << num_vectors;

    using Vector = std::vector<MyIntPair>;
    std::vector<Vector> vecs;

    std::default_random_engine rng(std::random_device { }());

    std::vector<MyIntPair> correct;

    for (size_t i = 0; i < num_vectors; ++i) {
        std::vector<MyIntPair> vec1;
        for (size_t j = 0; j < 1000; ++j)
            vec1.emplace_back(MyIntPair(rng(), rng()));

        std::sort(vec1.begin(), vec1.end(), MyIntPairCompare());

        for (const MyIntPair& p : vec1)
            correct.emplace_back(MyIntPair(p));

        vecs.emplace_back(std::move(vec1));
    }

    if (stable)
    {
        // take first lists and replicate them with other second component
        for (size_t i = 0; i < num_vectors / 1; ++i) {
            std::vector<MyIntPair> vec1;
            for (size_t j = 0; j < 1000; ++j)
                vec1.emplace_back(MyIntPair(vecs[i][j].key_, rng()));

            std::sort(vec1.begin(), vec1.end(), MyIntPairCompare());

            for (const MyIntPair& p : vec1)
                correct.emplace_back(MyIntPair(p));

            vecs.emplace_back(std::move(vec1));
        }
    }

    std::stable_sort(correct.begin(), correct.end(), MyIntPairCompare());

    LoserTree lt(vecs.size());

    std::vector<Vector::const_iterator> lt_iter(vecs.size());
    size_t remaining_inputs = 0;

    for (size_t i = 0; i < vecs.size(); ++i) {
        lt_iter[i] = vecs[i].begin();

        if (lt_iter[i] == vecs[i].end()) {
            lt.insert_start(nullptr, i, true);
        }
        else {
            lt.insert_start(&*lt_iter[i], i, false);
            ++remaining_inputs;
        }
    }

    lt.init();

    std::vector<MyIntPair> result;

    while (remaining_inputs != 0)
    {
        // take next smallest element out
        unsigned top = lt.min_source();
        MyIntPair res = *lt_iter[top];
        // std::cout << res.key_ << std::endl;
        result.emplace_back(MyIntPair(res));

        ++lt_iter[top];

        if (lt_iter[top] != vecs[top].end()) {
            lt.delete_min_insert(&*lt_iter[top], false);
        }
        else {
            lt.delete_min_insert(nullptr, true);
            --remaining_inputs;
        }
    }

    die_unless(result == correct);
}

template <typename LoserTree>
static inline void test_losertree(bool stable) {

    test_losertree<LoserTree>(stable, 0);
    test_losertree<LoserTree>(stable, 1);
    test_losertree<LoserTree>(stable, 2);
    test_losertree<LoserTree>(stable, 3);
    test_losertree<LoserTree>(stable, 4);
    test_losertree<LoserTree>(stable, 5);
    test_losertree<LoserTree>(stable, 6);
    test_losertree<LoserTree>(stable, 7);
    test_losertree<LoserTree>(stable, 8);
    test_losertree<LoserTree>(stable, 9);
    test_losertree<LoserTree>(stable, 10);
    test_losertree<LoserTree>(stable, 11);
    test_losertree<LoserTree>(stable, 12);
}

static void test_losertree() {
    test_losertree<
        tlx::LoserTreeCopy<false, MyIntPair, MyIntPairCompare> >(
        /* stable */ false);
    test_losertree<
        tlx::LoserTreeCopy<true, MyIntPair, MyIntPairCompare> >(
        /* stable */ true);

    test_losertree<
        tlx::LoserTreePointer<false, MyIntPair, MyIntPairCompare> >(
        /* stable */ false);
    test_losertree<
        tlx::LoserTreePointer<true, MyIntPair, MyIntPairCompare> >(
        /* stable */ true);

    die_unequal(ctor_dtor_counter, 0);
}

/******************************************************************************/
// benchmark_losertree

double time_delta(const std::chrono::steady_clock::time_point& a,
                  const std::chrono::steady_clock::time_point& b) {
    return std::chrono::duration_cast<
        std::chrono::microseconds>(b - a).count() / 1e6;
}

template <typename LoserTree>
void benchmark_losertree(
    const char* benchmark, size_t num_vectors, size_t vector_size) {

    using Vector = std::vector<MyInt>;
    using steady_clock = std::chrono::steady_clock;

    std::vector<Vector> vecs;
    size_t total_size = 0;

    std::minstd_rand rng(123456);

    steady_clock::time_point tp1 = steady_clock::now();

    for (size_t i = 0; i < num_vectors; ++i) {
        std::vector<MyInt> vec1;
        vec1.reserve(vector_size);

        for (size_t j = 0; j < vector_size; ++j)
            vec1.emplace_back(MyInt(rng()));

        std::sort(vec1.begin(), vec1.end(), MyIntCompare());
        total_size += vec1.size();
        vecs.emplace_back(std::move(vec1));
    }

    steady_clock::time_point tp2 = steady_clock::now();

    LoserTree lt(vecs.size());

    std::vector<Vector::const_iterator> lt_iter(vecs.size());
    size_t remaining_inputs = 0;

    for (size_t i = 0; i < vecs.size(); ++i) {
        lt_iter[i] = vecs[i].begin();

        if (lt_iter[i] == vecs[i].end()) {
            lt.insert_start(nullptr, i, true);
        }
        else {
            lt.insert_start(&*lt_iter[i], i, false);
            ++remaining_inputs;
        }
    }

    lt.init();

    std::vector<MyInt> result;
    result.reserve(total_size);

    while (remaining_inputs != 0)
    {
        // take next smallest element out
        unsigned top = lt.min_source();
        result.emplace_back(*lt_iter[top]++);

        if (lt_iter[top] != vecs[top].end()) {
            lt.delete_min_insert(&*lt_iter[top], false);
        }
        else {
            lt.delete_min_insert(nullptr, true);
            --remaining_inputs;
        }
    }

    steady_clock::time_point tp3 = steady_clock::now();

    die_unequal(result.size(), total_size);

    std::cout << "RESULT"
              << " benchmark=" << benchmark
              << " num_vectors=" << num_vectors
              << " vector_size=" << vector_size
              << " init_time=" << time_delta(tp1, tp2)
              << " merge_time=" << time_delta(tp2, tp3)
              << std::endl;
}

int main(int argc, char* argv[]) {
    tlx::CmdlineParser clp;

    size_t num_vectors = 10;
    clp.add_size_t('n', "num_vectors", num_vectors,
                   "Number of sequences to merge, default: 10");

    size_t outer_repeat = 1;
    clp.add_size_t('R', "outer_repeat", outer_repeat,
                   "Number of outer repetitions, default: 1");

    std::uint32_t vector_size = 1000000;
    clp.add_bytes('s', "vector_size", vector_size,
                  "Length of vectors to merge, default: 1000000");

    std::string benchmark;
    clp.add_opt_param_string(
        "benchmark", benchmark,
        "losertree type to benchmark: "
        "c = LoserTreeCopy, "
        "p = LoserTreePointer, "
        "d = LoserTreeCopyStable, "
        "q = LoserTreePointerStable, "
        "if empty = run tests");

    if (!clp.process(argc, argv))
        return -1;

    if (benchmark.empty()) {
        test_losertree();
        return 0;
    }

    // iterate over letters in benchmark
    for (const char& bench : benchmark) {
        switch (bench) {
        case 'c':
            for (size_t r = 0; r < outer_repeat; ++r) {
                benchmark_losertree<
                    tlx::LoserTreeCopy<false, MyInt, MyIntCompare> >(
                    "LoserTreeCopy", num_vectors, vector_size);
            }
            break;
        case 'd':
            for (size_t r = 0; r < outer_repeat; ++r) {
                benchmark_losertree<
                    tlx::LoserTreeCopy<true, MyInt, MyIntCompare> >(
                    "LoserTreeCopy", num_vectors, vector_size);
            }
            break;
        case 'p':
            for (size_t r = 0; r < outer_repeat; ++r) {
                benchmark_losertree<
                    tlx::LoserTreePointer<false, MyInt, MyIntCompare> >(
                    "LoserTreePointer", num_vectors, vector_size);
            }
            break;
        case 'q':
            for (size_t r = 0; r < outer_repeat; ++r) {
                benchmark_losertree<
                    tlx::LoserTreePointer<true, MyInt, MyIntCompare> >(
                    "LoserTreePointer", num_vectors, vector_size);
            }
            break;
        }
    }

    return 0;
}

/******************************************************************************/
