/*******************************************************************************
 * tests/math_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

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

    die_unequal(tlx::clz_template<std::uint8_t>(0), 8u);
    die_unequal(tlx::clz_template<std::uint16_t>(0), 16u);
    die_unequal(tlx::clz_template<std::uint32_t>(0), 32u);
    die_unequal(tlx::clz_template<std::uint64_t>(0), 64u);

    die_unequal(tlx::clz<std::uint32_t>(0), 32u);
    die_unequal(tlx::clz<std::uint64_t>(0), 64u);

    unsigned bitpos = 0;
    for (std::uint64_t i = 1llu << 63; i != 0; i >>= 1, ++bitpos)
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

    die_unequal(tlx::clz<std::uint32_t>(0x0100), 31u - 8u);
    die_unequal(tlx::clz<std::uint64_t>(0x0100), 63u - 8u);

    die_unequal(tlx::clz_template<std::uint32_t>(0x0100), 31u - 8u);
    die_unequal(tlx::clz_template<std::uint64_t>(0x0100), 63u - 8u);
}

static void test_ctz() {

    die_unequal(tlx::ctz_template<std::uint8_t>(0), 8u);
    die_unequal(tlx::ctz_template<std::uint16_t>(0), 16u);
    die_unequal(tlx::ctz_template<std::uint32_t>(0), 32u);
    die_unequal(tlx::ctz_template<std::uint64_t>(0), 64u);

    die_unequal(tlx::ctz<std::uint32_t>(0), 32u);
    die_unequal(tlx::ctz<std::uint64_t>(0), 64u);

    unsigned bitpos = 0;
    for (std::uint64_t i = (~0llu); i != 0; i <<= 1, ++bitpos)
    {
        die_unequal(tlx::ctz(i), bitpos);
        die_unequal(tlx::ctz_template(i), bitpos);
    }

    die_unequal(tlx::ctz<std::uint32_t>(0x0100), 8u);
    die_unequal(tlx::ctz<std::uint64_t>(0x0100), 8u);

    die_unequal(tlx::ctz_template<std::uint32_t>(0x0100), 8u);
    die_unequal(tlx::ctz_template<std::uint64_t>(0x0100), 8u);
}

static void test_ffs() {

    die_unequal(tlx::ffs(0), 0u);
    die_unequal(tlx::ffs_template(0), 0u);

    unsigned power = 0;
    for (std::uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
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
    for (std::uint32_t i = 1; i < (1lu << 31); i <<= 1, ++power)
    {
        if (i > 1) {
            die_unequal(tlx::integer_log2_floor(i - 1), power - 1);
            die_unequal(tlx::integer_log2_floor_template(i - 1), power - 1);
        }

        die_unequal(tlx::integer_log2_floor(i), power);
        die_unequal(tlx::integer_log2_floor(i + 1), power + (i == 1 ? 1 : 0));

        die_unequal(tlx::integer_log2_floor_template(i), power);
        die_unequal(tlx::integer_log2_floor_template(i + 1),
                    power + (i == 1 ? 1 : 0));

        if (i > 1) {
            die_unequal(
                tlx::integer_log2_ceil(i - 1), power - (i == 2 ? 1 : 0));
        }

        die_unequal(tlx::integer_log2_ceil(i), power);
        die_unequal(tlx::integer_log2_ceil(i + 1), power + 1);
    }

    power = 0;
    for (std::int64_t i = 1; i < (1ll << 62); i <<= 1, ++power)
    {
        if (i > 1) {
            die_unequal(tlx::integer_log2_floor(i - 1), power - 1);
            die_unequal(tlx::integer_log2_floor_template(i - 1), power - 1);
        }

        die_unequal(tlx::integer_log2_floor(i), power);
        die_unequal(tlx::integer_log2_floor(i + 1), power + (i == 1 ? 1 : 0));

        die_unequal(tlx::integer_log2_floor_template(i), power);
        die_unequal(tlx::integer_log2_floor_template(i + 1),
                    power + (i == 1 ? 1 : 0));

        if (i > 1) {
            die_unequal(
                tlx::integer_log2_ceil(i - 1), power - (i == 2 ? 1 : 0));
        }

        die_unequal(tlx::integer_log2_ceil(i), power);
        die_unequal(tlx::integer_log2_ceil(i + 1), power + 1);
    }

    power = 0;
    for (std::uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        if (i > 1) {
            die_unequal(tlx::integer_log2_floor(i - 1), power - 1);
            die_unequal(tlx::integer_log2_floor_template(i - 1), power - 1);
        }

        die_unequal(tlx::integer_log2_floor(i), power);
        die_unequal(tlx::integer_log2_floor(i + 1), power + (i == 1 ? 1 : 0));

        die_unequal(tlx::integer_log2_floor_template(i), power);
        die_unequal(tlx::integer_log2_floor_template(i + 1),
                    power + (i == 1 ? 1 : 0));

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
    for (std::uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
    {
        die_if(tlx::is_power_of_two(i - 1) && i != 2);
        die_unless(tlx::is_power_of_two(i));
        die_if(tlx::is_power_of_two(i + 1) && i != 1);
    }
}

static void test_popcount() {

    die_unequal(tlx::popcount(0x11111111u), 8u);
    die_unequal(tlx::popcount_generic32(0x11111111u), 8u);

    for (size_t i = 0; i < 0xFF; ++i)
        die_unequal(tlx::popcount(i), tlx::popcount_generic8(i));

    for (size_t i = 0; i < 0xFFFF; ++i)
        die_unequal(tlx::popcount(i), tlx::popcount_generic16(i));

    for (size_t i = 0; i < 0xFFFFFF; ++i)
        die_unequal(tlx::popcount(i), tlx::popcount_generic32(i));

    // for (size_t i = 0; i < 0xFFFFFFFFFF; ++i)
    //     die_unequal(tlx::popcount(i), tlx::popcount_generic64(i));

    std::vector<std::uint8_t> data;
    for (size_t i = 0; i < 20; ++i) {
        die_unequal(tlx::popcount(data.data(), data.size()), 2 * i);
        data.push_back(0x11);
    }
}

template <unsigned D = 7>
static void test_power_to_the_real() {
    using T = double;
    std::mt19937_64 prng(D);
    std::uniform_real_distribution<T> dist(-1e2, 1e2);

    for (int i = 0; i < 1000; ++i) {
        const T x = dist(prng);
        const auto tested = tlx::power_to_the<D>(x);

        auto ref = T{ 1 };
        for (int j = 0; j < static_cast<int>(D); ++j)
            ref *= x;

        die_unequal_eps(tested, ref, fabs(ref / 1e10));
    }

    if (D > 0)
        test_power_to_the_real < (D > 0) ? D - 1 : 0 > ();
}

template <unsigned D = 7>
static void test_power_to_the_int() {
    using T = std::int64_t;

    for (auto x = T{ -100 }; x < 100; ++x) {
        const auto tested = tlx::power_to_the<D>(x);

        auto ref = T{ 1 };
        for (int j = 0; j < static_cast<int>(D); ++j)
            ref *= x;

        die_unequal(tested, ref);
    }

    if (D > 0)
        test_power_to_the_real < (D > 0) ? D - 1 : 0 > ();
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
    for (std::uint64_t i = 1; i < (1llu << 63); i <<= 1, ++power)
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

static void test_round_up() {
    for (size_t i = 0; i < 1000; ++i) {
        for (size_t j = 1; j < 1000; ++j) {
            die_unequal(tlx::round_up(i, j),
                        std::ceil(static_cast<double>(i) / j) * j);
        }
    }
}

static void test_sgn() {
    die_unequal(tlx::sgn(42), +1);
    die_unequal(tlx::sgn(42.0), +1);
    die_unequal(tlx::sgn(0), 0);
    die_unequal(tlx::sgn(0.0), 0);
    die_unequal(tlx::sgn(-42), -1);
    die_unequal(tlx::sgn(-42.0), -1);
}

int main() {
    test_bswap();
    test_clz();
    test_ctz();
    test_ffs();
    test_integer_log2();
    test_is_power_of_two();
    test_popcount();
    test_power_to_the_real<>();
    test_power_to_the_int<>();
    test_rol();
    test_ror();
    test_round_to_power_of_two();
    test_round_up();
    test_sgn();

    return 0;
}

/******************************************************************************/
