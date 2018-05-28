/*******************************************************************************
 * tests/algorithm/random_bipartition_shuffle.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Manuel Penschuck <tlx@manuel.jetzt>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include <tlx/algorithm/random_bipartition_shuffle.hpp>
#include <tlx/die.hpp>

// This unit test performs a large number (10000) of random_shuffles, each time
// classifying 100 elements into two groups of varying size. We then test, that
// the number of classifications matches the theoretical predictions (the number
// of times each element is put into a partition is distributed binomially)
// within a large confidence interval.

std::vector<unsigned> compute_histogram(unsigned size,
                                        unsigned size_left_part,
                                        unsigned iterations,
                                        std::mt19937_64& prng) {
    // compute histogram
    std::vector<unsigned> counts(size, 0);
    {
        std::vector<unsigned> data(size);

        for (unsigned iteration = 0; iteration < iterations; ++iteration) {
            // initialize data as increasing range
            for (size_t i = 0; i < size; i++) data[i] = i;

            tlx::random_bipartition_shuffle(data.begin(), data.end(),
                                            size_left_part, prng);

            // increase counter of all elements in left partition
            for (size_t i = 0; i < size_left_part; i++)
                counts[data[i]]++;
        }
    }

    return counts;
}

void check_with_100elements(unsigned left_size, unsigned confidence,
                            std::mt19937_64& prng) {
    auto counts = compute_histogram(100, left_size, 10000, prng);
    unsigned lb = 100 * left_size - confidence;
    unsigned ub = 100 * left_size + confidence;

    for (unsigned c : counts) {
        die_unless(lb < c && c < ub);
    }

    if (left_size < 50)
        check_with_100elements(100 - left_size, confidence, prng);
}

int main() {
    std::mt19937_64 prng(1);

    // confidence intervals are selected that the error prob for each element is
    // less than 1e-6; using a union bound, the error probability for each test
    // is less than 0.1%
    for (int i = 0; i < 10; i++) {
        check_with_100elements(1, 45, prng);
        check_with_100elements(2, 80, prng);
        check_with_100elements(5, 110, prng);
        check_with_100elements(10, 145, prng);
        check_with_100elements(20, 190, prng);
    }

    return 0;
}

/******************************************************************************/
