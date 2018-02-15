/*******************************************************************************
 * tests/math_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>
#include <iostream>

#include <tlx/die.hpp>
#include <tlx/math.hpp>

static void test_bswap() {
    die_unequal(tlx::bswap16_generic(0x1234u), 0x3412u);
    die_unequal(tlx::bswap16(0x1234u), 0x3412u);

    die_unequal(tlx::bswap32_generic(0x12345678u), 0x78563412u);
    die_unequal(tlx::bswap32(0x12345678u), 0x78563412u);

    die_unequal(tlx::bswap64_generic(0x1234567812345678llu),
                0x7856341278563412llu);
    die_unequal(tlx::bswap64(0x1234567812345678llu), 0x7856341278563412llu);
}

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

static void test_rol() {
    die_unequal(tlx::rol32_generic(0x12345678u, 1), 0x2468ACF0u);
    die_unequal(tlx::rol32(0x12345678u, 1), 0x2468ACF0u);

    die_unequal(tlx::rol32_generic(0x12345678u, 3), 0x91A2B3C0u);
    die_unequal(tlx::rol32(0x12345678u, 3), 0x91A2B3C0u);

    die_unequal(tlx::rol32_generic(0x12345678u, 8), 0x34567812u);
    die_unequal(tlx::rol32(0x12345678u, 8), 0x34567812u);

    die_unequal(tlx::rol64_generic(0x1234567812345678llu, 1),
                0x2468ACF02468ACF0llu);
    die_unequal(tlx::rol64(0x1234567812345678llu, 1), 0x2468ACF02468ACF0llu);

    die_unequal(tlx::rol64_generic(0x1234567812345678llu, 3),
                0x91A2B3C091A2B3C0llu);
    die_unequal(tlx::rol64(0x1234567812345678llu, 3), 0x91A2B3C091A2B3C0llu);

    die_unequal(tlx::rol64_generic(0x1234567812345678llu, 8),
                0x3456781234567812llu);
    die_unequal(tlx::rol64(0x1234567812345678llu, 8), 0x3456781234567812llu);
}

static void test_ror() {
    die_unequal(tlx::ror32_generic(0x12345678u, 1), 0x91A2B3Cu);
    die_unequal(tlx::ror32(0x12345678u, 1), 0x91A2B3Cu);

    die_unequal(tlx::ror32_generic(0x12345678u, 3), 0x2468ACFu);
    die_unequal(tlx::ror32(0x12345678u, 3), 0x2468ACFu);

    die_unequal(tlx::ror32_generic(0x12345678u, 8), 0x78123456u);
    die_unequal(tlx::ror32(0x12345678u, 8), 0x78123456u);

    die_unequal(tlx::ror64_generic(0x1234567812345678llu, 1),
                0x91A2B3C091A2B3Cllu);
    die_unequal(tlx::ror64(0x1234567812345678llu, 1), 0x91A2B3C091A2B3Cllu);

    die_unequal(tlx::ror64_generic(0x1234567812345678llu, 3),
                0x2468ACF02468ACFllu);
    die_unequal(tlx::ror64(0x1234567812345678llu, 3), 0x2468ACF02468ACFllu);

    die_unequal(tlx::ror64_generic(0x1234567812345678llu, 8),
                0x7812345678123456llu);
    die_unequal(tlx::ror64(0x1234567812345678llu, 8), 0x7812345678123456llu);
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

    test_bswap();
    test_clz();
    test_ffs();
    test_integer_log2();
    test_rol();
    test_ror();
    test_round_to_power_of_two();
    test_is_power_of_two();

    return 0;
}

/******************************************************************************/
