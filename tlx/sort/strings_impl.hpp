/*******************************************************************************
 * tlx/sort/strings_impl.hpp
 *
 * Implementations of front-ends for string sorting algorithms.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_STRINGS_IMPL_HEADER
#define TLX_SORT_STRINGS_IMPL_HEADER

#include <tlx/sort/strings.hpp>
#include <tlx/sort/strings/insertion_sort.hpp>
#include <tlx/sort/strings/multikey_quicksort.hpp>
#include <tlx/sort/strings/radix_sort.hpp>

namespace tlx {

//! \addtogroup tlx_sort
//! \{
//! \name String Sorting Algorithms
//! \{

void sort_strings(char** strings, size_t size, size_t memory) {
    return sort_strings(
        reinterpret_cast<unsigned char**>(strings), size, memory);
}

void sort_strings(unsigned char** strings, size_t size, size_t memory) {
    sort_strings_detail::radixsort_CE3(
        sort_strings_detail::StringPtr<sort_strings_detail::UCharStringSet>(
            sort_strings_detail::UCharStringSet(strings, strings + size)),
        /* depth */ 0, memory);
}

void sort_strings(std::vector<char*>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

void sort_strings(std::vector<unsigned char*>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

/******************************************************************************/

void sort_strings(const char** strings, size_t size, size_t memory) {
    return sort_strings(
        reinterpret_cast<const unsigned char**>(strings), size, memory);
}

void sort_strings(const unsigned char** strings, size_t size, size_t memory) {
    sort_strings_detail::radixsort_CE3(
        sort_strings_detail::StringPtr<sort_strings_detail::CUCharStringSet>(
            sort_strings_detail::CUCharStringSet(strings, strings + size)),
        /* depth */ 0, memory);
}

void sort_strings(std::vector<const char*>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

void sort_strings(std::vector<const unsigned char*>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

/******************************************************************************/

void sort_strings(std::string* strings, size_t size, size_t memory) {
    sort_strings_detail::radixsort_CE3(
        sort_strings_detail::StringPtr<sort_strings_detail::StdStringSet>(
            sort_strings_detail::StdStringSet(strings, strings + size)),
        /* depth */ 0, memory);
}

void sort_strings(std::vector<std::string>& strings, size_t memory) {
    return sort_strings(strings.data(), strings.size(), memory);
}

//! \}
//! \}

} // namespace tlx

#endif // !TLX_SORT_STRINGS_IMPL_HEADER

/******************************************************************************/
