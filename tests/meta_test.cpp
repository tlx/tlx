/*******************************************************************************
 * tests/meta_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2008 Andreas Beckmann <beckmann@cs.uni-frankfurt.de>
 * Copyright (C) 2013-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>
#include <iostream>
#include <sstream>

#include <tlx/die.hpp>
#include <tlx/math/integer_log2.hpp>
#include <tlx/meta.hpp>

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

template <typename ... Args>
void test_call_foreach_run(std::ostream& os, const Args& ... args) {

    tlx::call_foreach(
        [&os](auto a) { os << a << '\n'; },
        args ...);

    tlx::call_foreach(SimpleFunctor(os), args ...);
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

template <typename ... Args>
void test_call_foreach_with_index_run(std::ostream& os, const Args& ... args) {

    // a variadic tuple to get indexes from using the compile-time index param
    auto my_tuple = std::make_tuple(1, 'x', "abc");

    tlx::call_foreach_with_index(
        [&os, &my_tuple](auto index, auto a) {
            os << index << ' ' << a << ' '
               << std::get<decltype(index)::index>(my_tuple) << '\n';
        },
        args ...);

    tlx::call_foreach_with_index(SimpleFunctorWithIndex(os), args ...);
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

    tlx::call_for_range<16>(
        [&](size_t index) { oss << index << ' '; });

    tlx::call_for_range<4, 8>(
        [&](auto index) { oss << index << ' '; });

    die_unequal("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 4 5 6 7 ", oss.str());
}

/******************************************************************************/
// Log2 and Log2Floor

template <unsigned long long Value>
void test_log_i(size_t floor_value, size_t ceil_value) {

    die_unequal(tlx::Log2Floor<Value>::value, tlx::integer_log2_floor(Value));
    die_unequal(tlx::Log2<Value>::floor, tlx::integer_log2_floor(Value));
    die_unequal(tlx::Log2<Value>::ceil, tlx::integer_log2_ceil(Value));

    if (Value <= 1)
    {
        die_unequal(tlx::Log2Floor<Value>::value, 0);
        die_unequal(tlx::Log2<Value>::floor, 0);
        die_unequal(tlx::Log2<Value>::ceil, 0);
    }
    else if (Value == 2)
    {
        die_unequal(tlx::Log2Floor<Value>::value, 1);
        die_unequal(tlx::Log2<Value>::floor, 1);
        die_unequal(tlx::Log2<Value>::ceil, 1);
    }
    else
    {
        die_unequal(tlx::Log2Floor<Value>::value, floor_value);
        die_unequal(tlx::Log2<Value>::floor, floor_value);
        die_unequal(tlx::Log2<Value>::ceil, ceil_value);
    }
}

template <unsigned long long Value>
void test_log2_value(size_t p) {
    test_log_i<Value - 1>(p - 1, p);
    test_log_i<Value>(p, p);
    test_log_i<Value + 1>(p, p + 1);
}

static void test_log2() {
/*[[[perl
  print "test_log2_value<1ull << $_>($_);\n" for (0..63);
]]]*/
    test_log2_value<1ull << 0>(0);
    test_log2_value<1ull << 1>(1);
    test_log2_value<1ull << 2>(2);
    test_log2_value<1ull << 3>(3);
    test_log2_value<1ull << 4>(4);
    test_log2_value<1ull << 5>(5);
    test_log2_value<1ull << 6>(6);
    test_log2_value<1ull << 7>(7);
    test_log2_value<1ull << 8>(8);
    test_log2_value<1ull << 9>(9);
    test_log2_value<1ull << 10>(10);
    test_log2_value<1ull << 11>(11);
    test_log2_value<1ull << 12>(12);
    test_log2_value<1ull << 13>(13);
    test_log2_value<1ull << 14>(14);
    test_log2_value<1ull << 15>(15);
    test_log2_value<1ull << 16>(16);
    test_log2_value<1ull << 17>(17);
    test_log2_value<1ull << 18>(18);
    test_log2_value<1ull << 19>(19);
    test_log2_value<1ull << 20>(20);
    test_log2_value<1ull << 21>(21);
    test_log2_value<1ull << 22>(22);
    test_log2_value<1ull << 23>(23);
    test_log2_value<1ull << 24>(24);
    test_log2_value<1ull << 25>(25);
    test_log2_value<1ull << 26>(26);
    test_log2_value<1ull << 27>(27);
    test_log2_value<1ull << 28>(28);
    test_log2_value<1ull << 29>(29);
    test_log2_value<1ull << 30>(30);
    test_log2_value<1ull << 31>(31);
    test_log2_value<1ull << 32>(32);
    test_log2_value<1ull << 33>(33);
    test_log2_value<1ull << 34>(34);
    test_log2_value<1ull << 35>(35);
    test_log2_value<1ull << 36>(36);
    test_log2_value<1ull << 37>(37);
    test_log2_value<1ull << 38>(38);
    test_log2_value<1ull << 39>(39);
    test_log2_value<1ull << 40>(40);
    test_log2_value<1ull << 41>(41);
    test_log2_value<1ull << 42>(42);
    test_log2_value<1ull << 43>(43);
    test_log2_value<1ull << 44>(44);
    test_log2_value<1ull << 45>(45);
    test_log2_value<1ull << 46>(46);
    test_log2_value<1ull << 47>(47);
    test_log2_value<1ull << 48>(48);
    test_log2_value<1ull << 49>(49);
    test_log2_value<1ull << 50>(50);
    test_log2_value<1ull << 51>(51);
    test_log2_value<1ull << 52>(52);
    test_log2_value<1ull << 53>(53);
    test_log2_value<1ull << 54>(54);
    test_log2_value<1ull << 55>(55);
    test_log2_value<1ull << 56>(56);
    test_log2_value<1ull << 57>(57);
    test_log2_value<1ull << 58>(58);
    test_log2_value<1ull << 59>(59);
    test_log2_value<1ull << 60>(60);
    test_log2_value<1ull << 61>(61);
    test_log2_value<1ull << 62>(62);
    test_log2_value<1ull << 63>(63);
// [[[end]]]
}

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

template <typename ... Args>
void test_vmap_foreach_run(std::ostream& os, const Args& ... args) {

    auto r1 = tlx::vmap_foreach(
        [&os](auto a) {
            os << a << '\n';
            return a + 1;
        },
        args ...);

    die_unequal(std::tuple_size<decltype(r1)>::value, 3u);
    die_unequal(std::get<0>(r1), 43);
    die_unequal(std::get<1>(r1), 6.0);
    die_unequal(std::get<2>(r1), std::string("ello"));

    auto r2 = tlx::vmap_foreach(SimpleMapFunctor(os), args ...);

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

template <typename ... Args>
void test_vmap_foreach_with_index_run(std::ostream& os, const Args& ... args) {

    // a variadic tuple to get indexes from using the compile-time index param
    auto my_tuple = std::make_tuple(1, 'x', "abc");

    auto r1 = tlx::vmap_foreach_with_index(
        [&os, &my_tuple](auto index, auto a) {
            os << index << ' ' << a << ' '
               << std::get<decltype(index)::index>(my_tuple) << '\n';
            return a + 1;
        },
        args ...);

    die_unequal(std::tuple_size<decltype(r1)>::value, 3u);
    die_unequal(std::get<0>(r1), 43);
    die_unequal(std::get<1>(r1), 6.0);
    die_unequal(std::get<2>(r1), std::string("ello"));

    auto r2 =
        tlx::vmap_foreach_with_index(SimpleMapFunctorWithIndex(os), args ...);

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
// vmap_for_range

static void test_vmap_for_range() {

    std::ostringstream oss;

    auto r1 = tlx::vmap_for_range<16>(
        [&](size_t index) {
            oss << index << ' ';
            return std::to_string(index + 42);
        });

    die_unequal(std::tuple_size<decltype(r1)>::value, 16u);
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

    die_unequal("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 4 5 6 7 ", oss.str());
}

/******************************************************************************/

int main() {

    test_call_for_range();
    test_call_foreach();
    test_call_foreach_with_index();
    test_log2();
    test_vmap_for_range();
    test_vmap_foreach();
    test_vmap_foreach_with_index();

    return 0;
}

/******************************************************************************/
