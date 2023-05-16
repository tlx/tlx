/*******************************************************************************
 * tests/algorithm/multiway_merge_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2014-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <iostream>
#include <random>

#include <tlx/die.hpp>
#include <tlx/logger.hpp>

#include <tlx/algorithm/multiway_merge.hpp>
#include <tlx/algorithm/parallel_multiway_merge.hpp>

struct Something {
    unsigned int a, b;

    explicit Something(unsigned int x = 0) noexcept
        : a(x), b(x * x)
    { }

    bool operator < (const Something& other) const {
        return a < other.a;
    }

    bool operator == (const Something& other) const {
        return (a == other.a) && (b == other.b);
    }

    friend std::ostream& operator << (std::ostream& os, const Something& s) {
        return os << '(' << s.a << ',' << s.b << ')';
    }
};

template <typename ValueType, bool Parallel, bool Stable, bool Sentinels>
void test_vecs(unsigned int vecnum,
               const tlx::MultiwayMergeAlgorithm& mwma,
               const tlx::MultiwayMergeSplittingAlgorithm& mwmsa = tlx::MWMSA_DEFAULT) {

    static const bool debug = false;

    if (!Sentinels && mwma == tlx::MWMA_LOSER_TREE_SENTINEL)
        return;

    // construct many vectors of sorted random numbers

    std::mt19937 randgen(123456);
    std::uniform_int_distribution<unsigned int> distr_size(0, 127);
    std::uniform_int_distribution<unsigned int> distr_value(0, 20 * vecnum);

    std::vector<std::vector<ValueType> > vec(vecnum);

    size_t totalsize = 0;
    std::vector<ValueType> output, correct;

    for (size_t i = 0; i < vecnum; ++i)
    {
        // determine number of items in stream
        size_t inum = distr_size(randgen) + 64;
        if (i == 3) inum = 0; // add an empty sequence
        vec[i].resize(inum);
        totalsize += inum;

        // pick random items
        for (size_t j = 0; j < inum; ++j)
            vec[i][j] = ValueType(distr_value(randgen));

        std::sort(vec[i].begin(), vec[i].end());

        // put items into correct vector as well
        correct.insert(correct.end(), vec[i].begin(), vec[i].end());
    }

    // append sentinels
    if (Sentinels)
    {
        for (size_t i = 0; i < vecnum; ++i)
            vec[i].push_back(ValueType(std::numeric_limits<unsigned int>::max()));
    }

    // prepare output and correct vector
    output.resize(totalsize);
    std::sort(correct.begin(), correct.end());

    if (debug)
    {
        for (size_t i = 0; i < vecnum; ++i)
        {
            std::cout << "vec[" << i << "]:";
            for (size_t j = 0; j < vec[i].size(); ++j)
                std::cout << " " << vec[i][j];
            std::cout << "\n";
        }
    }

    // construct vector of input iterator ranges
    using input_iterator = typename std::vector<ValueType>::iterator;
    using difference_type = typename std::iterator_traits<input_iterator>::difference_type;

    std::vector<std::pair<input_iterator, input_iterator> > sequences(vecnum);

    for (size_t i = 0; i < vecnum; ++i)
    {
        sequences[i] = std::make_pair(vec[i].begin(), vec[i].end() - (Sentinels ? 1 : 0));

        die_unless(std::is_sorted(vec[i].cbegin(), vec[i].cend()));
    }

    if (Parallel) {
        tlx::parallel_multiway_merge_force_parallel = true;
        if (!Stable)
            tlx::parallel_multiway_merge(
                sequences.begin(), sequences.end(),
                output.begin(), static_cast<difference_type>(totalsize),
                std::less<ValueType>(), mwma, mwmsa);
        else
            tlx::stable_parallel_multiway_merge(
                sequences.begin(), sequences.end(),
                output.begin(), static_cast<difference_type>(totalsize),
                std::less<ValueType>(), mwma, mwmsa);
    }
    else if (!Sentinels) {
        if (!Stable)
            tlx::multiway_merge(
                sequences.begin(), sequences.end(),
                output.begin(), static_cast<difference_type>(totalsize),
                std::less<ValueType>(), mwma);
        else
            tlx::stable_multiway_merge(
                sequences.begin(), sequences.end(),
                output.begin(), static_cast<difference_type>(totalsize),
                std::less<ValueType>(), mwma);
    }
    else {
        if (!Stable)
            tlx::multiway_merge_sentinels(
                sequences.begin(), sequences.end(),
                output.begin(), static_cast<difference_type>(totalsize),
                std::less<ValueType>(), mwma);
        else
            tlx::stable_multiway_merge_sentinels(
                sequences.begin(), sequences.end(),
                output.begin(), static_cast<difference_type>(totalsize),
                std::less<ValueType>(), mwma);
    }

    if (debug || output != correct) {
        for (size_t i = 0; i < output.size(); ++i)
            std::cout << '(' << output[i] << '=' << correct[i] << ')' << ' ';
    }

    die_unless(output == correct);
}

void test_all(const tlx::MultiwayMergeAlgorithm& mwma) {
    // run multiway merge tests for 0..256 sequences
    for (unsigned int n = 0; n <= 128; n += 1 + n / 16 + n / 32 + n / 64)
    {
        LOG0 << "testing multiway_merge with " << n << " players";

        test_vecs<Something, /* Parallel */ false,
                  /* Stable */ false, /* Sentinels */ false>(n, mwma);
        test_vecs<Something, /* Parallel */ false,
                  /* Stable */ true, /* Sentinels */ false>(n, mwma);
        test_vecs<Something, /* Parallel */ false,
                  /* Stable */ false, /* Sentinels */ true>(n, mwma);
        test_vecs<Something, /* Parallel */ false,
                  /* Stable */ true, /* Sentinels */ true>(n, mwma);

        test_vecs<unsigned int, /* Parallel */ false,
                  /* Stable */ false, /* Sentinels */ false>(n, mwma);
        test_vecs<unsigned int, /* Parallel */ false,
                  /* Stable */ true, /* Sentinels */ false>(n, mwma);
        test_vecs<unsigned int, /* Parallel */ false,
                  /* Stable */ false, /* Sentinels */ true>(n, mwma);
        test_vecs<unsigned int, /* Parallel */ false,
                  /* Stable */ true, /* Sentinels */ true>(n, mwma);

        test_vecs<Something, /* Parallel */ true, /* Stable */ false,
                  /* Sentinels */ false>(n, mwma, tlx::MWMSA_EXACT);
        test_vecs<Something, /* Parallel */ true, /* Stable */ true,
                  /* Sentinels */ false>(n, mwma, tlx::MWMSA_EXACT);

        test_vecs<Something, /* Parallel */ true, /* Stable */ false,
                  /* Sentinels */ false>(n, mwma, tlx::MWMSA_SAMPLING);
        test_vecs<Something, /* Parallel */ true, /* Stable */ true,
                  /* Sentinels */ false>(n, mwma, tlx::MWMSA_SAMPLING);
    }
}

int main() {
    test_all(tlx::MWMA_BUBBLE);
    test_all(tlx::MWMA_LOSER_TREE);
    test_all(tlx::MWMA_LOSER_TREE_COMBINED);
    test_all(tlx::MWMA_LOSER_TREE_SENTINEL);

    return 0;
}

/******************************************************************************/
