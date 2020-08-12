/*******************************************************************************
 * tests/container/simple_vector_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/simple_vector.hpp>
#include <tlx/die.hpp>

#include <algorithm>

struct MyInteger {
    size_t val_ = 1;
    size_t dummy_ = 42;
};

struct MyInteger2 {
    size_t val_ = 1;
    size_t dummy_ = 42;

    //! SimpleVector needs default constructor
    MyInteger2() = default;

    MyInteger2(size_t val, size_t dummy)
        : val_(val), dummy_(dummy) { }
};

// force instantiation
namespace tlx {

template class SimpleVector<int>;
template class SimpleVector<MyInteger>;
template class SimpleVector<MyInteger2>;

} // namespace tlx

int main() {
    tlx::SimpleVector<MyInteger> x;
    die_unequal(x.size(), 0u);

    tlx::SimpleVector<MyInteger> y(20);
    die_unequal(y.size(), 20u);

    for (size_t i = 0; i < y.size(); ++i)
        die_unequal(y[i].val_, 1u);

    // use iterators
    for (size_t i = 0; i < y.size(); ++i)
        y[i].val_ = i;

    std::reverse(y.begin(), y.end());

    for (size_t i = 0; i < y.size(); ++i)
        die_unequal(y[i].val_, y.size() - 1 - i);

    for (size_t i = 0; i < y.size(); ++i)
        die_unequal(y.at(i).val_, y.size() - 1 - i);

    die_unequal(y.front().val_, y.size() - 1);
    die_unequal(y.back().val_, 0u);

    std::sort(y.begin(), y.end(),
              [](const MyInteger& a, const MyInteger& b) {
                  return a.val_ < b.val_;
              });

    for (size_t i = 0; i < y.size(); ++i)
        die_unequal(y[i].val_, i);

    // move assignment
    x = std::move(y);

    die_unequal(x.size(), 20u);
    die_unequal(y.size(), 0u);

    // range iteration
    {
        size_t a = 0;
        for (auto b : x)
            die_unequal(a++, b.val_);

        a = 0;
        for (const MyInteger& b : x)
            die_unequal(a++, b.val_);
    }

    // resize test
    {
        x.resize(40);
        die_unequal(x.size(), 40u);

        // data still there?
        for (size_t i = 0; i < 20; ++i)
            die_unequal(x[i].val_, i);

        x.resize(10);
        die_unequal(x.size(), 10u);

        // data still there?
        for (size_t i = 0; i < x.size(); ++i)
            die_unequal(x[i].val_, i);
    }

    // std::swap test
    std::swap(x, y);

    die_unequal(x.size(), 0u);
    die_unequal(y.size(), 10u);

    // move construction
    {
        tlx::SimpleVector<MyInteger> z = std::move(y);
        die_unequal(y.size(), 0u);
        die_unequal(z.size(), 10u);

        // data still there?
        for (size_t i = 0; i < z.size(); ++i)
            die_unequal(z[i].val_, i);
    }

    // fill
    {
        x.resize(10);
        MyInteger mi;
        mi.val_ = 42, mi.dummy_ = 1;
        x.fill(mi);
    }

    return 0;
}

/******************************************************************************/
