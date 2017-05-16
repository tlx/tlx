/*******************************************************************************
 * tests/math_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>
#include <iostream>

#include <tlx/die.hpp>
#include <tlx/math.hpp>

static void test_clz() {

    die_unequal(tlx::clz_template<uint32_t>(0), 32u);

    unsigned bitpos = 0;
    for (uint64_t i = 1llu << 63; i != 0; i >>= 1, ++bitpos)
    {
        if (i > 1) {
            die_unequal(tlx::clz(i - 1), bitpos + 1);
            die_unequal(tlx::clz_template(i - 1), bitpos + 1);
        }

        die_unequal(tlx::clz(i), bitpos);
        die_unequal(tlx::clz_template(i), bitpos);

        if (i > 1) {
            die_unequal(tlx::clz(i + 1), bitpos);
            die_unequal(tlx::clz_template(i + 1), bitpos);
        }
    }

    die_unequal(tlx::clz<uint32_t>(0x0100), 31u - 8u);
    die_unequal(tlx::clz<uint64_t>(0x0100), 63u - 8u);

    die_unequal(tlx::clz_template<uint32_t>(0x0100), 31u - 8u);
    die_unequal(tlx::clz_template<uint64_t>(0x0100), 63u - 8u);
}

static void test_ffs() {

    die_unequal(tlx::ffs(0), 0u);
    die_unequal(tlx::ffs_template(0), 0u);

    unsigned power = 0;
    for (uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        if (i > 1) {
            die_unequal(tlx::ffs(i - 1), 1u);
            die_unequal(tlx::ffs_template(i - 1), 1u);
        }

        die_unequal(tlx::ffs(i), power + 1);
        die_unequal(tlx::ffs_template(i), power + 1);

        if (i > 1) {
            die_unequal(tlx::ffs(i + 1), 1u);
            die_unequal(tlx::ffs_template(i + 1), 1u);
        }
    }
}

static void test_integer_log2() {

    unsigned power = 0;
    for (uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        if (i > 1)
            die_unequal(tlx::integer_log2_floor(i - 1), power - 1);

        die_unequal(tlx::integer_log2_floor(i), power);
        die_unequal(tlx::integer_log2_floor(i + 1), power + (i == 1 ? 1 : 0));

        if (i > 1) {
            die_unequal(
                tlx::integer_log2_ceil(i - 1), power - (i == 2 ? 1 : 0));
        }

        die_unequal(tlx::integer_log2_ceil(i), power);
        die_unequal(tlx::integer_log2_ceil(i + 1), power + 1);
    }
}

static void test_is_power_of_two() {

    unsigned power = 0;
    for (uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        die_if(tlx::is_power_of_two(i - 1) && i != 2);
        die_unless(tlx::is_power_of_two(i));
        die_if(tlx::is_power_of_two(i + 1) && i != 1);
    }
}

static void test_round_to_power_of_two() {

    unsigned power = 0;
    for (uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        if (i > 2)
            die_unequal(tlx::round_up_to_power_of_two(i - 1), i);

        die_unequal(tlx::round_up_to_power_of_two(i), i);
        die_unequal(tlx::round_up_to_power_of_two(i + 1), i << 1);

        die_unequal(tlx::round_down_to_power_of_two(i - 1), i >> 1);
        die_unequal(tlx::round_down_to_power_of_two(i), i);

        if (i > 2)
            die_unequal(tlx::round_down_to_power_of_two(i + 1), i);
    }
}

int main() {

    test_clz();
    test_ffs();
    test_integer_log2();
    test_round_to_power_of_two();
    test_is_power_of_two();

    return 0;
}

/******************************************************************************/
