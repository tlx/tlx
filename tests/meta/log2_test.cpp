/*******************************************************************************
 * tests/meta/log2_test.cpp
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

#include <tlx/die.hpp>
#include <tlx/math/integer_log2.hpp>
#include <tlx/meta/log2.hpp>

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

int main() {
    test_log2();
    return 0;
}

/******************************************************************************/
