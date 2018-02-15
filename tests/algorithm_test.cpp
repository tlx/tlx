/*******************************************************************************
 * tests/algorithm_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <functional>
#include <iterator>
#include <vector>

#include <tlx/algorithm.hpp>
#include <tlx/die.hpp>
#include <tlx/logger.hpp>

static void test_merge_combine() {
    using Pair = std::pair<int, int>;

    std::vector<Pair> vec1 = {
        { 1, 10 }, { 2, 10 }, { 5, 10 }, { 7, 10 }, { 9, 10 }
    };
    std::vector<Pair> vec2 = {
        { 0, 5 }, { 2, 5 }, { 6, 5 }, { 7, 5 }, { 11, 5 }
    };

    std::vector<Pair> out;

    auto comp = [](const Pair& a, const Pair& b) {
                    return a.first - b.first;
                };

    die_unless(tlx::is_sorted_cmp(vec1.begin(), vec1.end(), comp));
    die_unless(tlx::is_sorted_cmp(vec2.begin(), vec2.end(), comp));

    tlx::merge_combine(
        vec1.begin(), vec1.end(),
        vec2.begin(), vec2.end(),
        std::back_inserter(out),
        comp,
        [](const Pair& a, const Pair& b) {
            return Pair(a.first, a.second + b.second);
        });

    std::vector<Pair> result = {
        { 0, 5 }, { 1, 10 }, { 2, 15 }, { 5, 10 }, { 6, 5 }, { 7, 15 },
        { 9, 10 }, { 11, 5 }
    };

    for (size_t i = 0; i < out.size(); ++i) {
        sLOG0 << out[i].first << out[i].second;
    }

    die_unless(tlx::is_sorted_cmp(out.begin(), out.end(), comp));

    die_unequal(result.size(), out.size());
    for (size_t i = 0; i < out.size(); ++i) {
        die_unequal(result[i].first, out[i].first);
        die_unequal(result[i].second, out[i].second);
    }
}

static void exclusive_scan() {
    {
        const std::vector<int> vec1;
        std::vector<int> vec2(1);
        int init = 1;
        const auto res1 = tlx::exclusive_scan(
            vec1.begin(), vec1.end(), vec2.begin(), init);
        LOG0 << vec2.end() - res1;
        LOG0 << vec2.front();
        die_unless(res1 == vec2.end());
        die_unless(vec2.front() == init);
    }

    {
        const std::vector<int> vec1 = { 1, 2, 3 };
        std::vector<int> vec2(5);
        const std::vector<int> vec_res = { 1, 2, 4, 7 };
        int init = 1;
        const auto res1 = tlx::exclusive_scan(
            vec1.begin(), vec1.end(), vec2.begin(), init);
        die_unless(res1 == vec2.begin() + 4);
        die_unless(std::equal(vec_res.begin(), vec_res.end(), vec2.begin()));
    }

    {
        const std::vector<int> vec1 = { 1, 2, 3 };
        std::vector<int> vec2(5);
        const std::vector<int> vec_res = { 1, 0, -2, -5 };
        int init = 1;
        const auto binary_op = std::minus<int>();
        const auto res1 = tlx::exclusive_scan(
            vec1.begin(), vec1.end(), vec2.begin(), init, binary_op);
        die_unless(res1 == vec2.begin() + 4);
        die_unless(std::equal(vec_res.begin(), vec_res.end(), vec2.begin()));
    }
}

int main() {

    test_merge_combine();
    exclusive_scan();

    return 0;
}

/******************************************************************************/
