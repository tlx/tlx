/*******************************************************************************
 * tests/sort_strings_test.cpp
 *
 * String sorting test program
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/sort/strings/insertion_sort.hpp>
#include <tlx/sort/strings/multikey_quicksort.hpp>
#include <tlx/sort/strings/radix_sort.hpp>

#include <tlx/logger.hpp>

#include <random>

#if TLX_MORE_TESTS
static const bool tlx_more_tests = true;
#else
static const bool tlx_more_tests = false;
#endif

using namespace tlx::sort_strings_detail;

template <typename Set>
using StringSorter = void (*)(const Set&, size_t);

template <typename Random, typename Iterator>
void fill_random(Random& rng, const std::string& letters,
                 Iterator begin, Iterator end) {
    for (Iterator i = begin; i != end; ++i)
        *i = letters[(rng() / 100) % letters.size()];
}

template <typename StringSet, StringSorter<StringSet> sorter>
void TestUCharString(const char* name,
                     const size_t num_strings, const size_t num_chars,
                     const std::string& letters) {

    std::default_random_engine rng(123456);

    LOG1 << "Running " << name << " on " << num_strings
         << " uint8_t* strings";

    // array of string pointers
    uint8_t** cstrings = new uint8_t*[num_strings];

    // generate random strings of length num_chars
    for (size_t i = 0; i < num_strings; ++i)
    {
        size_t slen = num_chars + (rng() >> 8) % (num_chars / 4);

        cstrings[i] = new uint8_t[slen + 1];
        fill_random(rng, letters, cstrings[i], cstrings[i] + slen);
        cstrings[i][slen] = 0;
    }

    // run sorting algorithm
    UCharStringSet ss(cstrings, cstrings + num_strings);
    sorter(ss, 0);
    if (0) ss.print();

    // check result
    if (!ss.check_order()) {
        LOG1 << "Result is not sorted!";
        abort();
    }

    // free memory.
    for (size_t i = 0; i < num_strings; ++i)
        delete[] cstrings[i];

    delete[] cstrings;
}

template <typename StringSet, StringSorter<StringSet> sorter>
void TestVectorString(const char* name,
                      const size_t num_strings, const size_t num_chars,
                      const std::string& letters) {

    std::default_random_engine rng(123456);

    LOG1 << "Running " << name << " on " << num_strings
         << " std::vector<std::string> strings";

    // vector of std::string objects
    std::vector<std::string> strings(num_strings);

    // generate random strings of length num_chars
    for (size_t i = 0; i < num_strings; ++i)
    {
        size_t slen = num_chars + (rng() >> 8) % (num_chars / 4);

        strings[i].resize(slen);
        fill_random(rng, letters, strings[i].begin(), strings[i].end());
    }

    // run sorting algorithm
    StdStringSet ss(strings.data(), strings.data() + strings.size());
    sorter(ss, 0);
    if (0) ss.print();

    // check result
    if (!ss.check_order()) {
        LOG1 << "Result is not sorted!";
        abort();
    }
}

template <typename StringSet, StringSorter<StringSet> sorter>
void TestVectorPtrString(const char* name,
                         const size_t num_strings, const size_t num_chars,
                         const std::string& letters) {

    std::default_random_engine rng(123456);

    LOG1 << "Running " << name << " on " << num_strings
         << " std::vector<std::unique_ptr<std::string>> strings";

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

    // run sorting algorithm
    VectorStringUPtrSet ss(strings.begin(), strings.end());
    sorter(ss, 0);
    if (0) ss.print();

    // check result
    if (!ss.check_order()) {
        LOG1 << "Result is not sorted!";
        abort();
    }
}

template <typename StringSet, StringSorter<StringSet> sorter>
void TestStringSuffixString(const char* name, const size_t num_chars,
                            const std::string& letters) {

    std::default_random_engine rng(123456);

    LOG1 << "Running " << name << " on " << num_chars
         << " std::vector<size_t> suffixes";

    // std::string text object
    std::string text(num_chars, 0);
    fill_random(rng, letters, text.begin(), text.end());

    std::vector<size_t> suffixarray;
    auto ss = StringSuffixSet::Initialize(text, suffixarray);

    // run sorting algorithm
    sorter(ss, 0);
    if (0) ss.print();

    // check result
    if (!ss.check_order()) {
        LOG1 << "Result is not sorted!";
        abort();
    }
}

static const char* letters_alnum
    = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// use macro because one cannot pass template functions as template parameters:
#define run_tests(func)                             \
    TestUCharString<UCharStringSet, func>(          \
        #func, num_strings, 16, letters_alnum);     \
    TestVectorString<StdStringSet, func>(           \
        #func, num_strings, 16, letters_alnum);     \
    TestVectorPtrString<VectorStringUPtrSet, func>( \
        #func, num_strings, 16, letters_alnum);     \
    TestStringSuffixString<StringSuffixSet, func>(  \
        #func, num_strings, letters_alnum);         \

void test_all(const size_t num_strings) {
    if (num_strings <= 1024) {
        run_tests(insertion_sort);
    }
    run_tests(multikey_quicksort);
    run_tests(radix_sort_outofplace);
    run_tests(radix_sort_inplace);
}

int main() {
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
