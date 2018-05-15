/*******************************************************************************
 * tlx/sort/strings3.cpp
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

namespace tlx {

namespace ss = tlx::sort_strings_detail;

void sort_strings(std::string* strings, size_t size, size_t memory) {
    ss::radixsort_CE3(
        ss::StdStringSet(strings, strings + size), /* depth */ 0, memory);
}

void sort_strings(std::vector<std::string>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

} // namespace tlx

/******************************************************************************/
