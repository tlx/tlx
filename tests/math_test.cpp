/*******************************************************************************
 * tests/math_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math.hpp>

#include <tlx/die.hpp>

#include <cmath>
#include <cstddef>

static void test_integer_log2() {

    size_t power = 0;
    for (uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        if (i > 1)
            die_unequal(tlx::integer_log2_floor(i - 1), power - 1);

        die_unequal(tlx::integer_log2_floor(i), power);
        die_unequal(tlx::integer_log2_floor(i + 1), power + (i == 1 ? 1 : 0));

        if (i > 1)
            die_unequal(tlx::integer_log2_ceil(i - 1), power - (i == 2 ? 1 : 0));

        die_unequal(tlx::integer_log2_ceil(i), power);
        die_unequal(tlx::integer_log2_ceil(i + 1), power + 1);
    }
}

static void test_round_to_power_of_two() {

    size_t power = 0;
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

    test_integer_log2();
    test_round_to_power_of_two();

    return 0;
}

/******************************************************************************/
