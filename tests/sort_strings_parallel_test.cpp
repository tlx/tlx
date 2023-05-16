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

#include <tlx/sort/strings_parallel.hpp>

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

void TestFrontend(const size_t num_strings, const size_t num_chars,
                  const std::string& letters) {

    std::default_random_engine rng(seed);

    LOG1 << "Running sort_strings() on " << num_strings
         << " uint8_t* strings";

    // array of string pointers
    tlx::simple_vector<std::uint8_t*> cstrings(num_strings);

    // generate random strings of length num_chars
    for (size_t i = 0; i < num_strings; ++i)
    {
        size_t slen = num_chars + (rng() >> 8) % (num_chars / 4);

        cstrings[i] = new std::uint8_t[slen + 1];
        fill_random(rng, letters, cstrings[i], cstrings[i] + slen);
        cstrings[i][slen] = 0;
    }

    // run sorting algorithm
    {
        double ts1 = tlx::timestamp();

        tlx::sort_strings_parallel(cstrings.data(), num_strings);

        double ts2 = tlx::timestamp();
        LOG1 << "sorting took " << ts2 - ts1 << " seconds";

        // check result
        if (!UCharStringSet(cstrings.data(), cstrings.data() + num_strings)
            .check_order())
        {
            LOG1 << "Result is not sorted!";
            abort();
        }
    }

    // array of const string pointers
    tlx::simple_vector<const std::uint8_t*> ccstrings(num_strings);

    for (size_t i = 0; i < num_strings; ++i)
        ccstrings[i] = cstrings[i];
    std::shuffle(ccstrings.begin(), ccstrings.end(), rng);

    // run sorting algorithm
    {
        double ts1 = tlx::timestamp();

        tlx::sort_strings_parallel(ccstrings.data(), num_strings);

        double ts2 = tlx::timestamp();
        LOG1 << "sorting took " << ts2 - ts1 << " seconds";

        // check result
        if (!CUCharStringSet(ccstrings.data(), ccstrings.data() + num_strings)
            .check_order())
        {
            LOG1 << "Result is not sorted!";
            abort();
        }
    }

    // recreate array of const string pointers
    for (size_t i = 0; i < num_strings; ++i)
        ccstrings[i] = cstrings[i];
    std::shuffle(ccstrings.begin(), ccstrings.end(), rng);

    // run sorting algorithm with LCP output
    {
        double ts1 = tlx::timestamp();

        tlx::simple_vector<std::uint32_t> lcp(num_strings);

        tlx::sort_strings_parallel_lcp(
            ccstrings.data(), num_strings, lcp.data());

        double ts2 = tlx::timestamp();
        LOG1 << "sorting took " << ts2 - ts1 << " seconds";

        // check result
        CUCharStringSet ss(ccstrings.data(), ccstrings.data() + num_strings);
        if (!ss.check_order())
        {
            LOG1 << "Result is not sorted!";
            abort();
        }
        if (!check_lcp(ss, lcp.data())) {
            LOG1 << "LCP result is not correct!";
            abort();
        }
    }

    // free memory.
    for (size_t i = 0; i < num_strings; ++i)
        delete[] cstrings[i];
}

/******************************************************************************/

void test_all(const size_t num_strings) {
    run_tests(parallel_sample_sort);
    run_tests(parallel_sample_sort_unroll_interleave);

    TestFrontend(num_strings, 16, letters_alnum);
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
