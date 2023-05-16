/*******************************************************************************
 * tests/sort_strings_test.hpp
 *
 * String sorting test program
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_TESTS_SORT_STRINGS_TEST_HEADER
#define TLX_TESTS_SORT_STRINGS_TEST_HEADER

#include <tlx/sort/strings/string_ptr.hpp>

#include <tlx/logger.hpp>
#include <tlx/simple_vector.hpp>
#include <tlx/timestamp.hpp>

#include <chrono>
#include <cstdint>
#include <random>

#if TLX_MORE_TESTS
static const bool tlx_more_tests = true;
#else
static const bool tlx_more_tests = false;
#endif

using namespace tlx::sort_strings_detail;

static const size_t seed = 1234567;

template <typename StringSet>
using StringSorter = void (*)(const StringPtr<StringSet>&, size_t, size_t);

template <typename StringSet, typename LcpType>
using StringLcpSorter =
    void (*)(const StringLcpPtr<StringSet, LcpType>&, size_t, size_t);

template <typename Random, typename Iterator>
void fill_random(Random& rng, const std::string& letters,
                 Iterator begin, Iterator end) {
    for (Iterator i = begin; i != end; ++i)
        *i = letters[(rng() / 100) % letters.size()];
}

template <typename Random, typename Iterator>
void fill_random_lognormal(Random& rng, const std::string& letters,
                           Iterator begin, Iterator end) {
    std::lognormal_distribution<double> lognorm(0.0, 1.0);
    for (Iterator i = begin; i != end; ++i) {
        while (true) {
            unsigned p = static_cast<unsigned>(
                lognorm(rng) * letters.size() / 2.0);
            if (p >= letters.size())
                continue;
            *i = letters[p];
            break;
        }
    }
}

//! calculate lcp by scanning
template <typename StringSet, typename LcpType>
static inline
bool check_lcp(const StringSet& ss, LcpType* lcp) {
    bool good = true;
    typename StringSet::Iterator s1 = ss.begin();
    for (size_t i = 0; i + 1 < ss.size(); ++i, ++s1) {
        typename StringSet::Iterator s2 = s1 + 1;

        typename StringSet::CharIterator c1 = ss.get_chars(*s1, 0);
        typename StringSet::CharIterator c2 = ss.get_chars(*s2, 0);

        LcpType h = 0;
        while (ss.is_equal(*s1, c1, *s2, c2))
            ++h, ++c1, ++c2;

        if (h != lcp[i + 1]) {
            good = false;
            LOG1 << "check_lcp() failed at position " << i + 1;
        }
    }
    return good;
}

template <typename StringSet, StringSorter<StringSet> sorter,
          typename LcpType, StringLcpSorter<StringSet, LcpType> lcp_sorter>
void TestUCharString(const char* name,
                     const size_t num_strings, const size_t num_chars,
                     const std::string& letters, bool with_lcp) {

    std::default_random_engine rng(seed);

    LOG1 << "Running " << name << " on " << num_strings
         << " uint8_t* strings" << (with_lcp ? " with lcps" : "");

    // array of string pointers
    tlx::simple_vector<std::uint8_t*> cstrings(num_strings);

    // generate random strings of length num_chars
    for (size_t i = 0; i < num_strings; ++i)
    {
        size_t slen = num_chars + (rng() >> 8) % (num_chars / 4);

        cstrings[i] = new std::uint8_t[slen + 1];
        fill_random_lognormal(rng, letters, cstrings[i], cstrings[i] + slen);
        cstrings[i][slen] = 0;
    }

    if (!with_lcp) {
        // run sorting algorithm
        double ts1 = tlx::timestamp();

        UCharStringSet ss(cstrings.data(), cstrings.data() + num_strings);
        sorter(StringPtr<UCharStringSet>(ss), /* depth */ 0, /* memory */ 0);
        if (0) ss.print();

        double ts2 = tlx::timestamp();
        LOG1 << "sorting took " << ts2 - ts1 << " seconds";

        // check result
        if (!ss.check_order()) {
            LOG1 << "Result is not sorted!";
            abort();
        }
    }
    else {
        // run sorting algorithm with lcp output
        double ts1 = tlx::timestamp();

        tlx::simple_vector<std::uint32_t> lcp(num_strings);

        UCharStringSet ss(cstrings.data(), cstrings.data() + num_strings);
        lcp_sorter(StringLcpPtr<UCharStringSet, std::uint32_t>(ss, lcp.data()),
                   /* depth */ 0, /* memory */ 0);
        if (0) ss.print();

        double ts2 = tlx::timestamp();
        LOG1 << "sorting+lcp took " << ts2 - ts1 << " seconds";

        // check result
        if (!ss.check_order()) {
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

template <typename StringSet, StringSorter<StringSet> sorter,
          typename LcpType, StringLcpSorter<StringSet, LcpType> lcp_sorter>
void TestVectorStdString(const char* name,
                         const size_t num_strings, const size_t num_chars,
                         const std::string& letters, bool with_lcp) {

    std::default_random_engine rng(seed);

    LOG1 << "Running " << name << " on " << num_strings
         << " std::vector<std::string> strings"
         << (with_lcp ? " with lcps" : "");

    // vector of std::string objects
    std::vector<std::string> strings(num_strings);

    // generate random strings of length num_chars
    for (size_t i = 0; i < num_strings / 4; ++i)
    {
        size_t slen = num_chars + (rng() >> 8) % (num_chars / 4);

        strings[i].resize(slen);
        fill_random(rng, letters, strings[i].begin(), strings[i].end());

        strings[i + 1 * num_strings / 4] = strings[i];
        strings[i + 2 * num_strings / 4] = strings[i];
        strings[i + 3 * num_strings / 4] = strings[i];
    }

    if (!with_lcp) {
        // run sorting algorithm
        double ts1 = tlx::timestamp();

        StdStringSet ss(strings.data(), strings.data() + strings.size());
        sorter(StringPtr<StdStringSet>(ss), /* depth */ 0, /* memory */ 0);
        if (0) ss.print();

        double ts2 = tlx::timestamp();
        LOG1 << "sorting took " << ts2 - ts1 << " seconds";

        // check result
        if (!ss.check_order()) {
            LOG1 << "Result is not sorted!";
            abort();
        }
    }
    else {
        // run sorting algorithm with lcp output
        double ts1 = tlx::timestamp();

        tlx::simple_vector<std::uint32_t> lcp(num_strings);

        StdStringSet ss(strings.data(), strings.data() + strings.size());
        lcp_sorter(StringLcpPtr<StdStringSet, std::uint32_t>(ss, lcp.data()),
                   /* depth */ 0, /* memory */ 0);
        if (0) ss.print();

        double ts2 = tlx::timestamp();
        LOG1 << "sorting+lcp took " << ts2 - ts1 << " seconds";

        // check result
        if (!ss.check_order()) {
            LOG1 << "Result is not sorted!";
            abort();
        }
        if (!check_lcp(ss, lcp.data())) {
            LOG1 << "LCP result is not correct!";
            abort();
        }
    }
}

template <typename StringSet, StringSorter<StringSet> sorter,
          typename LcpType, StringLcpSorter<StringSet, LcpType> lcp_sorter>
void TestUPtrStdString(const char* name,
                       const size_t num_strings, const size_t num_chars,
                       const std::string& letters, bool with_lcp) {

    std::default_random_engine rng(seed);

    LOG1 << "Running " << name << " on " << num_strings
         << " std::vector<std::unique_ptr<std::string>> strings"
         << (with_lcp ? " with lcps" : "");

    // vector of pointers to std::string objects
    typedef std::unique_ptr<std::string> unique_ptr_string;
    std::vector<unique_ptr_string> strings(num_strings);

    // generate random strings of length num_chars
    for (size_t i = 0; i < num_strings; ++i)
    {
        size_t slen = num_chars + (rng() >> 8) % (num_chars / 4);

        strings[i] = unique_ptr_string(new std::string(slen, 0));
        fill_random(rng, letters, strings[i]->begin(), strings[i]->end());
    }

    if (!with_lcp) {
        // run sorting algorithm
        double ts1 = tlx::timestamp();

        UPtrStdStringSet ss(strings.data(), strings.data() + strings.size());
        sorter(StringPtr<UPtrStdStringSet>(ss), /* depth */ 0, /* memory */ 0);
        if (0) ss.print();

        double ts2 = tlx::timestamp();
        LOG1 << "sorting took " << ts2 - ts1 << " seconds";

        // check result
        if (!ss.check_order()) {
            LOG1 << "Result is not sorted!";
            abort();
        }
    }
    else {
        // run sorting algorithm with lcp output
        double ts1 = tlx::timestamp();

        tlx::simple_vector<std::uint32_t> lcp(num_strings);

        UPtrStdStringSet ss(strings.data(), strings.data() + strings.size());
        lcp_sorter(StringLcpPtr<UPtrStdStringSet, std::uint32_t>(ss, lcp.data()),
                   /* depth */ 0, /* memory */ 0);
        if (0) ss.print();

        double ts2 = tlx::timestamp();
        LOG1 << "sorting+lcp took " << ts2 - ts1 << " seconds";

        // check result
        if (!ss.check_order()) {
            LOG1 << "Result is not sorted!";
            abort();
        }
        if (!check_lcp(ss, lcp.data())) {
            LOG1 << "LCP result is not correct!";
            abort();
        }
    }
}

template <typename StringSet, StringSorter<StringSet> sorter>
void TestStringSuffixString(const char* name, const size_t num_chars,
                            const std::string& letters) {

    std::default_random_engine rng(seed);

    LOG1 << "Running " << name << " on " << num_chars
         << " std::vector<size_t> suffixes";

    // std::string text object
    std::string text(num_chars, 0);
    fill_random(rng, letters, text.begin(), text.end());

    std::vector<size_t> suffixarray;
    auto ss = StringSuffixSet::Initialize(text, suffixarray);

    // run sorting algorithm
    double ts1 = tlx::timestamp();

    sorter(ss, /* depth */ 0, /* memory */ 0);
    if (0) ss.print();

    double ts2 = tlx::timestamp();
    LOG1 << "sorting took " << ts2 - ts1 << " seconds";

    // check result
    if (!ss.check_order()) {
        LOG1 << "Result is not sorted!";
        abort();
    }
}

static const char* letters_alnum
    = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
      "\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF";

// use macro because one cannot pass template functions as template parameters:
#define run_tests(func)                                             \
    TestUCharString<UCharStringSet, func, std::uint32_t, func>(     \
        #func, num_strings, 16, letters_alnum, /* lcp */ false);    \
    TestUCharString<UCharStringSet, func, std::uint32_t, func>(     \
        #func, num_strings, 17, letters_alnum, /* lcp */ true);     \
    TestVectorStdString<StdStringSet, func, std::uint32_t, func>(   \
        #func, num_strings, 16, letters_alnum, /* lcp */ false);    \
    TestVectorStdString<StdStringSet, func, std::uint32_t, func>(   \
        #func, num_strings, 17, letters_alnum, /* lcp */ true);     \
    TestUPtrStdString<UPtrStdStringSet, func, std::uint32_t, func>( \
        #func, num_strings, 16, letters_alnum, /* lcp */ false);    \
    TestUPtrStdString<UPtrStdStringSet, func, std::uint32_t, func>( \
        #func, num_strings, 18, letters_alnum, /* lcp */ true);     \
    TestStringSuffixString<StringSuffixSet, func>(                  \
        #func, num_strings, letters_alnum);                         \

#endif // !TLX_TESTS_SORT_STRINGS_TEST_HEADER

/******************************************************************************/
