/*******************************************************************************
 * tests/loser_tree_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include <tlx/die.hpp>
#include <tlx/loser_tree.hpp>

struct MyInteger {
    size_t val_ = 1;
    size_t dummy_ = 42;

    //! SimpleVector needs default constructor
    MyInteger() = default;

    MyInteger(size_t val, size_t dummy)
        : val_(val), dummy_(dummy) { }

    bool operator == (const MyInteger& b) const {
        return val_ == b.val_ && dummy_ == b.dummy_;
    }
};

struct MyIntegerCompare {
    bool operator () (const MyInteger& a, const MyInteger& b) const {
        return a.val_ < b.val_;
    }
};

// force instantiation
namespace tlx {

template class LoserTreeCopy<false, MyInteger, MyIntegerCompare>;
template class LoserTreeCopy<true, MyInteger, MyIntegerCompare>;
template class LoserTreePointer<false, MyInteger, MyIntegerCompare>;
template class LoserTreePointer<true, MyInteger, MyIntegerCompare>;

template class LoserTreeCopyUnguarded<false, MyInteger, MyIntegerCompare>;
template class LoserTreeCopyUnguarded<true, MyInteger, MyIntegerCompare>;
template class LoserTreePointerUnguarded<false, MyInteger, MyIntegerCompare>;
template class LoserTreePointerUnguarded<true, MyInteger, MyIntegerCompare>;

} // namespace tlx

int main() {

    using Vector = std::vector<MyInteger>;
    std::vector<Vector> vecs;

    std::default_random_engine rng(std::random_device { } ());
    size_t num_vectors = 10;

    std::vector<MyInteger> correct;

    for (size_t i = 0; i < num_vectors; ++i) {
        std::vector<MyInteger> vec1;
        for (size_t j = 0; j < 1000; ++j) {
            vec1.emplace_back(rng(), i);
            correct.emplace_back(vec1.back());
        }

        std::sort(vec1.begin(), vec1.end(), MyIntegerCompare());
        vecs.emplace_back(vec1);
    }

    std::sort(correct.begin(), correct.end(), MyIntegerCompare());

    using LT = tlx::LoserTreeCopy<false, MyInteger, MyIntegerCompare>;
    LT lt(vecs.size());

    std::vector<Vector::const_iterator> lt_iter(num_vectors);
    size_t remaining_inputs = 0;

    for (size_t i = 0; i < num_vectors; ++i) {
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

    std::vector<MyInteger> result;

    while (remaining_inputs != 0)
    {
        // take next smallest element out
        unsigned top = lt.min_source();
        MyInteger res = std::move(*lt_iter[top]);
        // std::cout << res.val_ << std::endl;
        result.emplace_back(res);

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

    return 0;
}

/******************************************************************************/
