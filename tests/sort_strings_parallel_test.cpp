/*******************************************************************************
 * tests/sort_strings_parallel_test.cpp
 *
 * String sorting test program
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include "sort_strings_test.hpp"

#include <tlx/sort/strings/parallel_sample_sort.hpp>

/******************************************************************************/
// More PS5 Parameter Instantiations

class PS5ParametersTreeUnrollInterleave : public PS5ParametersDefault
{
public:
    //! key type for sample sort: 32-bit or 64-bit
    typedef size_t key_type;

    //! classification tree variant for sample sorts
    using Classify =
        SSClassifyTreeUnrollInterleave<key_type, /* TreeBits */ 13>;
};

template <typename StringPtr>
void parallel_sample_sort_unroll_interleave(
    const StringPtr& strptr, size_t depth, size_t memory) {
    return parallel_sample_sort_params<PS5ParametersTreeUnrollInterleave>(
        strptr, depth, memory);
}

/******************************************************************************/

void test_all(const size_t num_strings) {
    run_tests(parallel_sample_sort);
    run_tests(parallel_sample_sort_unroll_interleave);
}

int main() {
    // self verify calculations
    perfect_tree_calculations_self_verify();

    // run tests
    test_all(16);
    test_all(256);
    test_all(65550);
    if (tlx_more_tests) {
        test_all(1024 * 1024);
        test_all(16 * 1024 * 1024);
    }

    return 0;
}

/******************************************************************************/
