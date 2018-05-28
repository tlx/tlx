/*******************************************************************************
 * tests/meta/vmap_for_test.cpp
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
#include <tlx/meta/vmap_for_range.hpp>
#include <tlx/meta/vmap_foreach.hpp>
#include <tlx/meta/vmap_foreach_tuple.hpp>
#include <tlx/meta/vmap_foreach_tuple_with_index.hpp>
#include <tlx/meta/vmap_foreach_with_index.hpp>

/******************************************************************************/
// vmap_foreach

struct SimpleMapFunctor {
    explicit SimpleMapFunctor(std::ostream& os) : os_(os) { }
    template <typename Arg>
    auto operator () (const Arg& a) const {
        os_ << a << '\n';
        return a + 1;
    }
    std::ostream& os_;
};

template <typename... Args>
void test_vmap_foreach_run(std::ostream& os, const Args& ... args) {

    auto r1 = tlx::vmap_foreach(
        [&os](auto a) {
            os << a << '\n';
            return a + 1;
        },
        args...);

    die_unequal(std::tuple_size<decltype(r1)>::value, 3u);
    die_unequal(std::get<0>(r1), 43);
    die_unequal(std::get<1>(r1), 6.0);
    die_unequal(std::get<2>(r1), std::string("ello"));

    auto r2 = tlx::vmap_foreach(SimpleMapFunctor(os), args...);

    die_unequal(std::tuple_size<decltype(r2)>::value, 3u);
    die_unequal(std::get<0>(r2), 43);
    die_unequal(std::get<1>(r2), 6.0);
    die_unequal(std::get<2>(r2), std::string("ello"));
}

static void test_vmap_foreach() {

    std::ostringstream oss;

    test_vmap_foreach_run(
        oss, static_cast<int>(42), static_cast<double>(5), "hello");

    die_unequal("42\n5\nhello\n42\n5\nhello\n", oss.str());
}

/******************************************************************************/
// vmap_foreach_tuple

template <typename... Args>
void test_vmap_foreach_tuple_run(std::ostream& os, const Args& ... args) {

    auto my_tuple = std::make_tuple(args...);

    auto r1 = tlx::vmap_foreach_tuple(
        [&os](auto a) {
            os << a << '\n';
            return a + 1;
        },
        my_tuple);

    die_unequal(std::tuple_size<decltype(r1)>::value, 3u);
    die_unequal(std::get<0>(r1), 43);
    die_unequal(std::get<1>(r1), 6.0);
    die_unequal(std::get<2>(r1), std::string("ello"));

    auto r2 = tlx::vmap_foreach_tuple(SimpleMapFunctor(os), my_tuple);

    die_unequal(std::tuple_size<decltype(r2)>::value, 3u);
    die_unequal(std::get<0>(r2), 43);
    die_unequal(std::get<1>(r2), 6.0);
    die_unequal(std::get<2>(r2), std::string("ello"));
}

static void test_vmap_foreach_tuple() {

    std::ostringstream oss;

    test_vmap_foreach_tuple_run(
        oss, static_cast<int>(42), static_cast<double>(5), "hello");

    die_unequal("42\n5\nhello\n42\n5\nhello\n", oss.str());
}

/******************************************************************************/
// vmap_foreach_with_index

struct SimpleMapFunctorWithIndex {
    explicit SimpleMapFunctorWithIndex(std::ostream& os) : os_(os) { }
    template <typename Index, typename Arg>
    auto operator () (const Index& i, const Arg& a) const {
        os_ << i << ' ' << a << '\n';
        return a + 1;
    }
    std::ostream& os_;
};

template <typename... Args>
void test_vmap_foreach_with_index_run(std::ostream& os, const Args& ... args) {

    // a variadic tuple to get indexes from using the compile-time index param
    auto my_tuple = std::make_tuple(1, 'x', "abc");

    auto r1 = tlx::vmap_foreach_with_index(
        [&os, &my_tuple](auto index, auto a) {
            os << index << ' ' << a << ' '
               << std::get<decltype(index)::index>(my_tuple) << '\n';
            return a + 1;
        },
        args...);

    die_unequal(std::tuple_size<decltype(r1)>::value, 3u);
    die_unequal(std::get<0>(r1), 43);
    die_unequal(std::get<1>(r1), 6.0);
    die_unequal(std::get<2>(r1), std::string("ello"));

    auto r2 = tlx::vmap_foreach_with_index(
        SimpleMapFunctorWithIndex(os), args...);

    die_unequal(std::tuple_size<decltype(r2)>::value, 3u);
    die_unequal(std::get<0>(r2), 43);
    die_unequal(std::get<1>(r2), 6.0);
    die_unequal(std::get<2>(r2), std::string("ello"));
}

static void test_vmap_foreach_with_index() {

    std::ostringstream oss;

    test_vmap_foreach_with_index_run(
        oss, static_cast<int>(42), static_cast<double>(5), "hello");

    die_unequal("0 42 1\n1 5 x\n2 hello abc\n"
                "0 42\n1 5\n2 hello\n", oss.str());
}

/******************************************************************************/
// vmap_foreach_tuple_with_index

template <typename Tuple>
void test_vmap_foreach_tuple_with_index_run(std::ostream& os, const Tuple& tuple) {

    // a variadic tuple to get indexes from using the compile-time index param
    auto my_tuple = std::make_tuple(1, 'x', "abc");

    auto r1 = tlx::vmap_foreach_tuple_with_index(
        [&os, &my_tuple](auto index, auto a) {
            os << index << ' ' << a << ' '
               << std::get<decltype(index)::index>(my_tuple) << '\n';
            return a + 1;
        },
        tuple);

    die_unequal(std::tuple_size<decltype(r1)>::value, 3u);
    die_unequal(std::get<0>(r1), 43);
    die_unequal(std::get<1>(r1), 6.0);
    die_unequal(std::get<2>(r1), std::string("ello"));

    auto r2 = tlx::vmap_foreach_tuple_with_index(
        SimpleMapFunctorWithIndex(os), tuple);

    die_unequal(std::tuple_size<decltype(r2)>::value, 3u);
    die_unequal(std::get<0>(r2), 43);
    die_unequal(std::get<1>(r2), 6.0);
    die_unequal(std::get<2>(r2), std::string("ello"));
}

static void test_vmap_foreach_tuple_with_index() {

    std::ostringstream oss;

    test_vmap_foreach_tuple_with_index_run(
        oss,
        std::make_tuple(static_cast<int>(42), static_cast<double>(5), "hello"));

    die_unequal("0 42 1\n1 5 x\n2 hello abc\n"
                "0 42\n1 5\n2 hello\n", oss.str());
}

/******************************************************************************/
// vmap_for_range

static void test_vmap_for_range() {

    std::ostringstream oss;

    auto r1 = tlx::vmap_for_range<6>(
        [&](size_t index) {
            oss << index << ' ';
            return std::to_string(index + 42);
        });

    die_unequal(std::tuple_size<decltype(r1)>::value, 6u);
    die_unequal(std::get<0>(r1), "42");
    die_unequal(std::get<1>(r1), "43");

    auto r2 = tlx::vmap_for_range<4, 8>(
        [&](auto index) {
            oss << index << ' ';
            return index + 42;
        });

    die_unequal(std::tuple_size<decltype(r2)>::value, 4u);
    die_unequal(std::get<0>(r2), 46u);
    die_unequal(std::get<1>(r2), 47u);

    die_unequal("0 1 2 3 4 5 4 5 6 7 ", oss.str());
}

/******************************************************************************/

int main() {

    test_vmap_for_range();
    test_vmap_foreach();
    test_vmap_foreach_tuple();
    test_vmap_foreach_with_index();
    test_vmap_foreach_tuple_with_index();

    return 0;
}

/******************************************************************************/
