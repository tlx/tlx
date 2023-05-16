/*******************************************************************************
 * tests/sort_networks_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2020 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include <tlx/die.hpp>
#include <tlx/logger.hpp>

#include <tlx/sort/networks/best.hpp>
#include <tlx/sort/networks/bose_nelson.hpp>
#include <tlx/sort/networks/bose_nelson_parameter.hpp>

struct Something {
    int a, b;

    explicit Something(int x = 0) noexcept
        : a(x), b(x * x)
    { }

    bool operator < (const Something& other) const {
        return a < other.a;
    }
    bool operator > (const Something& other) const {
        return a > other.a;
    }

    friend std::ostream& operator << (std::ostream& os, const Something& s) {
        return os << '(' << s.a << ',' << s.b << ')';
    }
};

void test_networks(unsigned int size, unsigned method) {
    std::vector<Something> v(size);
    std::greater<Something> cmp;

    std::mt19937 randgen(123456);
    std::uniform_int_distribution<unsigned int> distr;

    for (unsigned int i = 0; i < size; ++i)
        v[i] = Something(distr(randgen));

    // sort descending
    if (method == 0) {
        tlx::sort_networks::best::sort(v.begin(), v.end(), cmp);
    }
    else if (method == 1) {
        tlx::sort_networks::bose_nelson::sort(v.begin(), v.end(), cmp);
    }
    else if (method == 2) {
        tlx::sort_networks::bose_nelson_parameter::sort(v.begin(), v.end(), cmp);
    }

    die_unless(std::is_sorted(v.cbegin(), v.cend(), cmp));

    // sort (default) ascending
    if (method == 0) {
        tlx::sort_networks::best::sort(v.begin(), v.end());
    }
    else if (method == 1) {
        tlx::sort_networks::bose_nelson::sort(v.begin(), v.end());
    }
    else if (method == 2) {
        tlx::sort_networks::bose_nelson_parameter::sort(v.begin(), v.end());
    }

    die_unless(std::is_sorted(v.cbegin(), v.cend()));
}

int main() {
    // run multiway mergesort tests for 0..256 sequences
    for (unsigned int i = 0; i <= 16; ++i)
    {
        test_networks(i, /* method */ 0);
        test_networks(i, /* method */ 1);
        test_networks(i, /* method */ 2);
    }

    return 0;
}

/******************************************************************************/
