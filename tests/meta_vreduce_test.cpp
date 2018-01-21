/*******************************************************************************
 * tests/meta_vmap_for_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Hung Tran <hung@ae.cs.uni-frankfurt.de>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>

#include <tlx/die.hpp>
#include <tlx/meta/vreduce.hpp>
#include <tlx/meta/vreduce_tuple.hpp>

/******************************************************************************/
// vreduce

struct TakeLeftFunctor {
    TakeLeftFunctor() = default;
    template <typename Arg1, typename Arg2>
    auto operator () (const Arg1& a, const Arg2& b) const {
        tlx::unused(b);
        return a;
    }
};

struct TakeRightFunctor {
    TakeRightFunctor() = default;
    template <typename Arg1, typename Arg2>
    auto operator () (const Arg1& a, const Arg2& b) const {
        tlx::unused(a);
        return b;
    }
};

template <typename... Args>
void test_vreduce_run(std::ostream& os, const Args& ... args) {

    auto r1 = tlx::vreduce([&os](auto a, auto b) { os << a << '\n'; return a + b; }, args...);

    die_unequal(r1, 48.0);

    auto r2 = tlx::vreduce([&os](auto a, auto b) { os << a << '\n'; return a + b; }, args...);

    die_unequal(r2, 48.0);

    auto r3 = tlx::vreduce(TakeLeftFunctor(), args...);

    die_unequal(r3, 42);

    auto r4 = tlx::vreduce(TakeRightFunctor(), args...);

    die_unequal(r4, true);
}

static void test_vreduce() {

    std::ostringstream oss;

    test_vreduce_run(
        oss, static_cast<int>(42), static_cast<double>(5), true);

    die_unequal("5\n42\n5\n42\n", oss.str());
}

/******************************************************************************/
// vreduce_tuple

template <typename... Args>
void test_vreduce_tuple_run(std::ostream& os, const Args& ... args) {

    auto my_tuple = std::make_tuple(args...);

    auto r1 = tlx::vreduce_tuple([&os](auto a, auto b) { os << a << '\n'; return a + b; }, my_tuple);

    die_unequal(r1, 48.0);

    auto r2 = tlx::vreduce_tuple([&os](auto a, auto b) { os << a << '\n'; return a + b; }, my_tuple);

    die_unequal(r2, 48.0);

    auto r3 = tlx::vreduce_tuple(TakeLeftFunctor(), my_tuple);

    die_unequal(r3, 42);

    auto r4 = tlx::vreduce_tuple(TakeRightFunctor(), my_tuple);

    die_unequal(r4, true);
}

static void test_vreduce_tuple() {

    std::ostringstream oss;

    test_vreduce_tuple_run(
        oss, static_cast<int>(42), static_cast<double>(5), true);

    die_unequal("5\n42\n5\n42\n", oss.str());
}

/******************************************************************************/

int main() {

    test_vreduce();
    test_vreduce_tuple();

    return 0;
}

/******************************************************************************/
