/*******************************************************************************
 * tests/meta/call_for_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>

#include <tlx/die.hpp>
#include <tlx/meta/call_for_range.hpp>
#include <tlx/meta/call_foreach.hpp>
#include <tlx/meta/call_foreach_tuple.hpp>
#include <tlx/meta/call_foreach_tuple_with_index.hpp>
#include <tlx/meta/call_foreach_with_index.hpp>

/******************************************************************************/
// call_foreach

struct SimpleFunctor {
    explicit SimpleFunctor(std::ostream& os) : os_(os) { }
    template <typename Arg>
    void operator () (const Arg& a) const {
        os_ << a << '\n';
    }
    std::ostream& os_;
};

template <typename... Args>
void test_call_foreach_run(std::ostream& os, const Args& ... args) {

    tlx::call_foreach(
        [&os](auto a) { os << a << '\n'; },
        args...);

    tlx::call_foreach_tuple(SimpleFunctor(os), std::make_tuple(args...));
}

static void test_call_foreach() {

    std::ostringstream oss;

    test_call_foreach_run(
        oss, static_cast<int>(42), static_cast<double>(5), "hello");

    die_unequal("42\n5\nhello\n42\n5\nhello\n", oss.str());
}

/******************************************************************************/
// call_foreach_with_index

struct SimpleFunctorWithIndex {
    explicit SimpleFunctorWithIndex(std::ostream& os) : os_(os) { }
    template <typename Index, typename Arg>
    void operator () (const Index& i, const Arg& a) const {
        os_ << i << ' ' << a << '\n';
    }
    std::ostream& os_;
};

template <typename... Args>
void test_call_foreach_with_index_run(std::ostream& os, const Args& ... args) {

    // a variadic tuple to get indexes from using the compile-time index param
    auto my_tuple = std::make_tuple(1, 'x', "abc");

    tlx::call_foreach_with_index(
        [&os, &my_tuple](auto index, auto a) {
            os << index << ' ' << a << ' '
               << std::get<decltype(index)::index>(my_tuple) << '\n';
        },
        args...);

    tlx::call_foreach_tuple_with_index(
        SimpleFunctorWithIndex(os), std::make_tuple(args...));
}

static void test_call_foreach_with_index() {

    std::ostringstream oss;

    test_call_foreach_with_index_run(
        oss, static_cast<int>(42), static_cast<double>(5), "hello");

    die_unequal("0 42 1\n1 5 x\n2 hello abc\n"
                "0 42\n1 5\n2 hello\n", oss.str());
}

/******************************************************************************/
// call_for_range

static void test_call_for_range() {

    std::ostringstream oss;

    tlx::call_for_range<6>(
        [&](size_t index) { oss << index << ' '; });

    tlx::call_for_range<4, 8>(
        [&](auto index) { oss << index << ' '; });

    die_unequal("0 1 2 3 4 5 4 5 6 7 ", oss.str());
}

/******************************************************************************/

int main() {

    test_call_for_range();
    test_call_foreach();
    test_call_foreach_with_index();

    return 0;
}

/******************************************************************************/
