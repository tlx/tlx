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

#include <tlx/die.hpp>
#include <tlx/math/integer_log2.hpp>
#include <tlx/meta/log2.hpp>
#include <cmath>
#include <cstddef>

/******************************************************************************/
// Log2 and Log2Floor

template <unsigned long long Value>
void test_log_i(size_t floor_value, size_t ceil_value)
{
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
void test_log2_value(size_t p)
{
    test_log_i<Value - 1>(p - 1, p);
    test_log_i<Value>(p, p);
    test_log_i<Value + 1>(p, p + 1);
}

static void test_log2()
{
    /*[[[perl
      print "    test_log2_value<1ULL << $_>($_);\n" for (0..63);
    ]]]*/
    test_log2_value<1ULL << 0>(0);
    test_log2_value<1ULL << 1>(1);
    test_log2_value<1ULL << 2>(2);
    test_log2_value<1ULL << 3>(3);
    test_log2_value<1ULL << 4>(4);
    test_log2_value<1ULL << 5>(5);
    test_log2_value<1ULL << 6>(6);
    test_log2_value<1ULL << 7>(7);
    test_log2_value<1ULL << 8>(8);
    test_log2_value<1ULL << 9>(9);
    test_log2_value<1ULL << 10>(10);
    test_log2_value<1ULL << 11>(11);
    test_log2_value<1ULL << 12>(12);
    test_log2_value<1ULL << 13>(13);
    test_log2_value<1ULL << 14>(14);
    test_log2_value<1ULL << 15>(15);
    test_log2_value<1ULL << 16>(16);
    test_log2_value<1ULL << 17>(17);
    test_log2_value<1ULL << 18>(18);
    test_log2_value<1ULL << 19>(19);
    test_log2_value<1ULL << 20>(20);
    test_log2_value<1ULL << 21>(21);
    test_log2_value<1ULL << 22>(22);
    test_log2_value<1ULL << 23>(23);
    test_log2_value<1ULL << 24>(24);
    test_log2_value<1ULL << 25>(25);
    test_log2_value<1ULL << 26>(26);
    test_log2_value<1ULL << 27>(27);
    test_log2_value<1ULL << 28>(28);
    test_log2_value<1ULL << 29>(29);
    test_log2_value<1ULL << 30>(30);
    test_log2_value<1ULL << 31>(31);
    test_log2_value<1ULL << 32>(32);
    test_log2_value<1ULL << 33>(33);
    test_log2_value<1ULL << 34>(34);
    test_log2_value<1ULL << 35>(35);
    test_log2_value<1ULL << 36>(36);
    test_log2_value<1ULL << 37>(37);
    test_log2_value<1ULL << 38>(38);
    test_log2_value<1ULL << 39>(39);
    test_log2_value<1ULL << 40>(40);
    test_log2_value<1ULL << 41>(41);
    test_log2_value<1ULL << 42>(42);
    test_log2_value<1ULL << 43>(43);
    test_log2_value<1ULL << 44>(44);
    test_log2_value<1ULL << 45>(45);
    test_log2_value<1ULL << 46>(46);
    test_log2_value<1ULL << 47>(47);
    test_log2_value<1ULL << 48>(48);
    test_log2_value<1ULL << 49>(49);
    test_log2_value<1ULL << 50>(50);
    test_log2_value<1ULL << 51>(51);
    test_log2_value<1ULL << 52>(52);
    test_log2_value<1ULL << 53>(53);
    test_log2_value<1ULL << 54>(54);
    test_log2_value<1ULL << 55>(55);
    test_log2_value<1ULL << 56>(56);
    test_log2_value<1ULL << 57>(57);
    test_log2_value<1ULL << 58>(58);
    test_log2_value<1ULL << 59>(59);
    test_log2_value<1ULL << 60>(60);
    test_log2_value<1ULL << 61>(61);
    test_log2_value<1ULL << 62>(62);
    test_log2_value<1ULL << 63>(63);
    // [[[end]]]
}

/******************************************************************************/

int main()
{
    test_log2();
    return 0;
}

/******************************************************************************/
