/*******************************************************************************
 * tests/sort_parallel_mergesort_test.cpp
 *
 * String sorting test program
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2014-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include <tlx/die.hpp>
#include <tlx/logger.hpp>

#include <tlx/sort/parallel_mergesort.hpp>

struct Something {
    int a, b;

    explicit Something(int x = 0) noexcept
        : a(x), b(x * x)
    { }

    bool operator < (const Something& other) const {
        return a < other.a;
    }

    friend std::ostream& operator << (std::ostream& os, const Something& s) {
        return os << '(' << s.a << ',' << s.b << ')';
    }
};

template <bool Stable>
void test_size(unsigned int size, tlx::MultiwayMergeSplittingAlgorithm mwmsa) {
    // std::cout << "testing parallel_mergesort with " << size << " items.\n";

    std::vector<Something> v(size);
    std::less<Something> cmp;

    std::mt19937 randgen(123456);
    std::uniform_int_distribution<unsigned int> distr;

    for (unsigned int i = 0; i < size; ++i)
        v[i] = Something(distr(randgen));

    if (Stable) {
        tlx::stable_parallel_mergesort(
            v.begin(), v.end(), cmp, /* num_threads */ 8, mwmsa);
    }
    else {
        tlx::parallel_mergesort(
            v.begin(), v.end(), cmp, /* num_threads */ 8, mwmsa);
    }

    die_unless(std::is_sorted(v.cbegin(), v.cend(), cmp));
}

int main() {
    // run multiway mergesort tests for 0..256 sequences
    for (unsigned int i = 0; i < 256; ++i)
    {
        test_size<false>(i, tlx::MWMSA_SAMPLING);
        test_size<true>(i, tlx::MWMSA_SAMPLING);

        test_size<false>(i, tlx::MWMSA_EXACT);
        test_size<true>(i, tlx::MWMSA_EXACT);
    }

    // run multiway mergesort tests for 0..256 sequences
    for (unsigned int i = 256; i <= 16 * 1024 * 1024; i = 2 * i - i / 2)
    {
        test_size<false>(i, tlx::MWMSA_SAMPLING);
        test_size<true>(i, tlx::MWMSA_SAMPLING);

        test_size<false>(i, tlx::MWMSA_EXACT);
        test_size<true>(i, tlx::MWMSA_EXACT);
    }

    return 0;
}

/******************************************************************************/
