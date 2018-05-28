/*******************************************************************************
 * tests/meta/fold_test.cpp
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
#include <tlx/meta/fold_left.hpp>
#include <tlx/meta/fold_left_tuple.hpp>
#include <tlx/meta/fold_right.hpp>
#include <tlx/meta/fold_right_tuple.hpp>
#include <tlx/unused.hpp>

/******************************************************************************/
// fold_left

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
void test_foldl_run(std::ostream& os, const Args& ... args) {

    auto r1 = tlx::fold_left(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r1, 48.0);

    auto r2 = tlx::fold_left(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r2, 48.0);

    auto r3 = tlx::fold_left(TakeLeftFunctor(), 0, args...);

    die_unequal(r3, 0);

    auto r4 = tlx::fold_left(TakeRightFunctor(), 0, args...);

    die_unequal(r4, true);
}

static void test_fold_left() {

    std::ostringstream oss;

    test_foldl_run(oss, static_cast<int>(42), static_cast<double>(5), true);

    // (((0 + 42) + 5) + 1)) where left operand is expected
    die_unequal("0\n42\n47\n0\n42\n47\n", oss.str());
}

/******************************************************************************/
// fold_left_tuple

template <typename... Args>
void test_foldl_tuple_run(std::ostream& os, const Args& ... args) {

    auto my_tuple = std::make_tuple(args...);

    auto r1 = tlx::fold_left_tuple(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r1, 48.0);

    auto r2 = tlx::fold_left_tuple(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r2, 48.0);

    auto r3 = tlx::fold_left_tuple(TakeLeftFunctor(), 0, my_tuple);

    die_unequal(r3, 0);

    auto r4 = tlx::fold_left_tuple(TakeRightFunctor(), 0, my_tuple);

    die_unequal(r4, true);
}

static void test_fold_left_tuple() {

    std::ostringstream oss;

    test_foldl_tuple_run(
        oss, static_cast<int>(42), static_cast<double>(5), true);

    // (((0 + 42) + 5) + 1)) where left operand is expected
    die_unequal("0\n42\n47\n0\n42\n47\n", oss.str());
}

/******************************************************************************/
// fold_right

template <typename... Args>
void test_foldr_run(std::ostream& os, const Args& ... args) {

    auto r1 = tlx::fold_right(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r1, 48.0);

    auto r2 = tlx::fold_right(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r2, 48.0);

    auto r3 = tlx::fold_right(TakeLeftFunctor(), 0, args...);

    die_unequal(r3, 42);

    auto r4 = tlx::fold_right(TakeRightFunctor(), 0, args...);

    die_unequal(r4, 0);
}

static void test_fold_right() {

    std::ostringstream oss;

    test_foldr_run(oss, static_cast<int>(42), static_cast<double>(5), true);

    // (42 + (5 + (1 + 0))) where left operand is expected
    die_unequal("1\n5\n42\n1\n5\n42\n", oss.str());
}

/******************************************************************************/
// fold_right_tuple

template <typename... Args>
void test_foldr_tuple_run(std::ostream& os, const Args& ... args) {

    auto my_tuple = std::make_tuple(args...);

    auto r1 = tlx::fold_right_tuple(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r1, 48.0);

    auto r2 = tlx::fold_right_tuple(
        [&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r2, 48.0);

    auto r3 = tlx::fold_right_tuple(TakeLeftFunctor(), 0, my_tuple);

    die_unequal(r3, 42);

    auto r4 = tlx::fold_right_tuple(TakeRightFunctor(), 0, my_tuple);

    die_unequal(r4, 0);
}

static void test_fold_right_tuple() {

    std::ostringstream oss;

    test_foldr_tuple_run(
        oss, static_cast<int>(42), static_cast<double>(5), true);

    // (42 + (5 + (1 + 0))) where left operand is expected
    die_unequal("1\n5\n42\n1\n5\n42\n", oss.str());
}

/******************************************************************************/

int main() {

    test_fold_left();
    test_fold_left_tuple();
    test_fold_right();
    test_fold_right_tuple();

    return 0;
}

/******************************************************************************/
