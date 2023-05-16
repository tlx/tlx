/*******************************************************************************
 * tests/sort_strings_test.cpp
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

#include <tlx/sort/strings/insertion_sort.hpp>
#include <tlx/sort/strings/multikey_quicksort.hpp>
#include <tlx/sort/strings/radix_sort.hpp>

#include <cstdint>
#include <tlx/sort/strings.hpp>

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

        tlx::sort_strings(cstrings.data(), num_strings, /* memory */ 0);

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

        tlx::sort_strings(ccstrings.data(), num_strings, /* memory */ 0);

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

        tlx::sort_strings_lcp(
            ccstrings.data(), num_strings, lcp.data(), /* memory */ 0);

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

void test_all(const size_t num_strings) {
    if (num_strings <= 1024) {
        run_tests(insertion_sort);
    }

    if (num_strings <= 1024 * 1024) {
        run_tests(multikey_quicksort);
        run_tests(radixsort_CE0);
        run_tests(radixsort_CE2);
        run_tests(radixsort_CE3);
        run_tests(radixsort_CI2);
        run_tests(radixsort_CI3);

        TestFrontend(num_strings, 16, letters_alnum);
    }
}

int main() {
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
