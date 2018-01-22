/*******************************************************************************
 * tests/meta_fold_test.cpp
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
#include <tlx/meta/foldl.hpp>
#include <tlx/meta/foldl_tuple.hpp>
#include <tlx/meta/foldr.hpp>
#include <tlx/meta/foldr_tuple.hpp>

/******************************************************************************/
// foldl

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

    auto r1 = tlx::foldl([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r1, 48.0);

    auto r2 = tlx::foldl([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r2, 48.0);

    auto r3 = tlx::foldl(TakeLeftFunctor(), 0, args...);

    die_unequal(r3, 0);

    auto r4 = tlx::foldl(TakeRightFunctor(), 0, args...);

    die_unequal(r4, true);
}

static void test_foldl() {

    std::ostringstream oss;

    test_foldl_run(oss, static_cast<int>(42), static_cast<double>(5), true);

    // (((0 + 42) + 5) + 1)) where left operand is expected
    die_unequal("0\n42\n47\n0\n42\n47\n", oss.str());
}

/******************************************************************************/
// foldl_tuple

template <typename... Args>
void test_foldl_tuple_run(std::ostream& os, const Args& ... args) {

    auto my_tuple = std::make_tuple(args...);

    auto r1 = tlx::foldl_tuple([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r1, 48.0);

    auto r2 = tlx::foldl_tuple([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r2, 48.0);

    auto r3 = tlx::foldl_tuple(TakeLeftFunctor(), 0, my_tuple);

    die_unequal(r3, 0);

    auto r4 = tlx::foldl_tuple(TakeRightFunctor(), 0, my_tuple);

    die_unequal(r4, true);
}

static void test_foldl_tuple() {

    std::ostringstream oss;

    test_foldl_tuple_run(
        oss, static_cast<int>(42), static_cast<double>(5), true);

    // (((0 + 42) + 5) + 1)) where left operand is expected
    die_unequal("0\n42\n47\n0\n42\n47\n", oss.str());
}

/******************************************************************************/
// foldr

template <typename... Args>
void test_foldr_run(std::ostream& os, const Args& ... args) {

    auto r1 = tlx::foldr([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r1, 48.0);

    auto r2 = tlx::foldr([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, args...);

    die_unequal(r2, 48.0);

    auto r3 = tlx::foldr(TakeLeftFunctor(), 0, args...);

    die_unequal(r3, 42);

    auto r4 = tlx::foldr(TakeRightFunctor(), 0, args...);

    die_unequal(r4, 0);
}

static void test_foldr() {

    std::ostringstream oss;

    test_foldr_run(oss, static_cast<int>(42), static_cast<double>(5), true);

    // (42 + (5 + (1 + 0))) where left operand is expected
    die_unequal("1\n5\n42\n1\n5\n42\n", oss.str());
}

/******************************************************************************/
// foldr_tuple

template <typename... Args>
void test_foldr_tuple_run(std::ostream& os, const Args& ... args) {

    auto my_tuple = std::make_tuple(args...);

    auto r1 = tlx::foldr_tuple([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r1, 48.0);

    auto r2 = tlx::foldr_tuple([&os](auto a, auto b) { os << a << '\n'; return a + b; }, 0, my_tuple);

    die_unequal(r2, 48.0);

    auto r3 = tlx::foldr_tuple(TakeLeftFunctor(), 0, my_tuple);

    die_unequal(r3, 42);

    auto r4 = tlx::foldr_tuple(TakeRightFunctor(), 0, my_tuple);

    die_unequal(r4, 0);
}

static void test_foldr_tuple() {

    std::ostringstream oss;

    test_foldr_tuple_run(
        oss, static_cast<int>(42), static_cast<double>(5), true);

    // (42 + (5 + (1 + 0))) where left operand is expected
    die_unequal("1\n5\n42\n1\n5\n42\n", oss.str());
}

/******************************************************************************/

int main() {

    test_foldl();
    test_foldl_tuple();
    test_foldr();
    test_foldr_tuple();

    return 0;
}

/******************************************************************************/
