/*******************************************************************************
 * tlx/sort/strings.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/sort/strings.hpp>
#include <tlx/sort/strings/insertion_sort.hpp>
#include <tlx/sort/strings/multikey_quicksort.hpp>
#include <tlx/sort/strings/radix_sort.hpp>
#include <tlx/sort/strings/string_set.hpp>

#include <tlx/unused.hpp>

namespace tlx {

namespace ss = tlx::sort_strings_detail;

void sort_strings(unsigned char** strings, size_t size, size_t memory) {
    tlx::unused(memory);
    ss::multikey_quicksort(
        ss::UCharStringSet(strings, strings + size), /* depth */ 0);
}

void sort_strings(char** strings, size_t size, size_t memory) {
    return sort_strings(
        reinterpret_cast<unsigned char**>(strings), size, memory);
}

void sort_strings(std::string* strings, size_t size, size_t memory) {
    tlx::unused(memory);
    ss::multikey_quicksort(
        ss::StdStringSet(strings, strings + size), /* depth */ 0);
}

void sort_strings(std::vector<std::string>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

} // namespace tlx

/******************************************************************************/
