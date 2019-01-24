/*******************************************************************************
 * tlx/sort/strings_lcp_impl.hpp
 *
 * Implementations of front-ends for string sorting algorithms.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_STRINGS_LCP_IMPL_HEADER
#define TLX_SORT_STRINGS_LCP_IMPL_HEADER

#include <tlx/sort/strings.hpp>
#include <tlx/sort/strings/insertion_sort.hpp>
#include <tlx/sort/strings/multikey_quicksort.hpp>
#include <tlx/sort/strings/radix_sort.hpp>

namespace tlx {

//! \addtogroup tlx_sort
//! \{
//! \name String Sorting Algorithms
//! \{

void sort_strings_lcp(char** strings, size_t size, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(
        reinterpret_cast<unsigned char**>(strings), size, lcp, memory);
}

void sort_strings_lcp(unsigned char** strings, size_t size, uint32_t* lcp,
                      size_t memory) {
    sort_strings_detail::radixsort_CE3(
        sort_strings_detail::StringLcpPtr<
            sort_strings_detail::UCharStringSet, uint32_t>(
            sort_strings_detail::UCharStringSet(strings, strings + size), lcp),
        /* depth */ 0, memory);
}

void sort_strings_lcp(std::vector<char*>& strings, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(strings.data(), strings.size(), lcp, memory);
}

void sort_strings_lcp(std::vector<unsigned char*>& strings, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(strings.data(), strings.size(), lcp, memory);
}

/******************************************************************************/

void sort_strings_lcp(const char** strings, size_t size, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(
        reinterpret_cast<const unsigned char**>(strings), size, lcp, memory);
}

void sort_strings_lcp(const unsigned char** strings, size_t size, uint32_t* lcp,
                      size_t memory) {
    sort_strings_detail::radixsort_CE3(
        sort_strings_detail::StringLcpPtr<
            sort_strings_detail::CUCharStringSet, uint32_t>(
            sort_strings_detail::CUCharStringSet(strings, strings + size), lcp),
        /* depth */ 0, memory);
}

void sort_strings_lcp(std::vector<const char*>& strings, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(strings.data(), strings.size(), lcp, memory);
}

void sort_strings_lcp(std::vector<const unsigned char*>& strings, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(strings.data(), strings.size(), lcp, memory);
}

/******************************************************************************/

void sort_strings_lcp(std::string* strings, size_t size, uint32_t* lcp,
                      size_t memory) {
    sort_strings_detail::radixsort_CE3(
        sort_strings_detail::StringLcpPtr<
            sort_strings_detail::StdStringSet, uint32_t>(
            sort_strings_detail::StdStringSet(strings, strings + size), lcp),
        /* depth */ 0, memory);
}

void sort_strings_lcp(std::vector<std::string>& strings, uint32_t* lcp,
                      size_t memory) {
    return sort_strings_lcp(strings.data(), strings.size(), lcp, memory);
}

//! \}
//! \}

} // namespace tlx

#endif // !TLX_SORT_STRINGS_LCP_IMPL_HEADER

/******************************************************************************/
