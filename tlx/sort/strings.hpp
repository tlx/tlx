/*******************************************************************************
 * tlx/sort/strings.hpp
 *
 * Front-end for string sorting algorithms.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_STRINGS_HEADER
#define TLX_SORT_STRINGS_HEADER

#include <cstdint>
#include <string>
#include <vector>

namespace tlx {

//! \addtogroup tlx_sort
//! \{
//! \name String Sorting Algorithms
//! \{

/******************************************************************************/

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(char** strings, size_t size, size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(unsigned char** strings, size_t size, size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(const char** strings, size_t size, size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(
    const unsigned char** strings, size_t size, size_t memory = 0);

/******************************************************************************/

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(std::vector<char*>& strings, size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(std::vector<unsigned char>& strings, size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(std::vector<const char*>& strings, size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(
    std::vector<const unsigned char*>& strings, size_t memory = 0);

/******************************************************************************/

/*!
 * Sort a set of std::strings in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(std::string* strings, size_t size, size_t memory = 0);

/*!
 * Sort a vector of std::strings in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings(std::vector<std::string>& strings, size_t memory = 0);

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(char** strings, size_t size, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(unsigned char** strings, size_t size, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(const char** strings, size_t size, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(const unsigned char** strings, size_t size, uint32_t* lcp,
                      size_t memory = 0);

/******************************************************************************/

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(std::vector<char*>& strings, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(std::vector<unsigned char>& strings, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style char* in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(std::vector<const char*>& strings, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a set of strings represented by C-style uint8_t* in place.
 *
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(std::vector<const unsigned char*>& strings, uint32_t* lcp,
                      size_t memory = 0);

/******************************************************************************/

/*!
 * Sort a set of std::strings in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(std::string* strings, size_t size, uint32_t* lcp,
                      size_t memory = 0);

/*!
 * Sort a vector of std::strings in place.
 *
 * The strings are sorted as _unsigned_ 8-bit characters, not signed characters!
 * If the memory limit is non zero, possibly slower algorithms will be selected
 * to stay within the memory limit.
 */
void sort_strings_lcp(std::vector<std::string>& strings, uint32_t* lcp,
                      size_t memory = 0);

/******************************************************************************/

//! \}
//! \}

} // namespace tlx

#if !TLX_BUILD_STRING_SORTING
// include implementations of string sorting algorithms
#include <tlx/sort/strings_impl.hpp>
#include <tlx/sort/strings_lcp_impl.hpp>
#endif

#endif // !TLX_SORT_STRINGS_HEADER

/******************************************************************************/
