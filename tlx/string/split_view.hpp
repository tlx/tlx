/*******************************************************************************
 * tlx/string/split_view.hpp
 *
 * Split a std::string and call a functor with tlx::string_view for each part.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_STRING_SPLIT_VIEW_HEADER
#define TLX_STRING_SPLIT_VIEW_HEADER

#include <tlx/container/string_view.hpp>
#include <string>
#include <vector>

namespace tlx {

//! \addtogroup tlx_string
//! \{
//! \name Split and Join
//! \{

/******************************************************************************/
// split_callback() with a Functor receiving string views

/*!
 * Split the given string at each separator character into distinct substrings,
 * and call the given callback with a StringView for each substring. Multiple
 * consecutive separators are considered individually and will result in empty
 * split substrings.
 *
 * \param str       string to split
 * \param sep       separator character
 * \param callback  callback taking StringView of substring
 * \param limit     maximum number of parts returned
 */
template <typename Functor>
static inline void split_callback(
    char sep, tlx::string_view str, Functor&& callback,
    std::string::size_type limit = std::string::npos)
{
    if (limit == 0)
    {
        callback(StringView(str.begin(), str.end()));
        return;
    }

    tlx::string_view::size_type count = 0;
    tlx::string_view::const_iterator it = str.begin(), last = it;

    for (; it != str.end(); ++it)
    {
        if (*it == sep)
        {
            if (count == limit)
            {
                callback(StringView(last, str.end()));
                return;
            }
            callback(StringView(last, it));
            ++count;
            last = it + 1;
        }
    }
    callback(StringView(last, it));
}

/******************************************************************************/
// split_view() returning std::vector<tlx::string_view>

/*!
 * Split the given string at each separator character into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.
 *
 * \param sep    separator character
 * \param str    string to split
 * \param limit  maximum number of parts returned
 * \return       vector containing each split substring
 */
std::vector<tlx::string_view> split_view(
    char sep, tlx::string_view str,
    tlx::string_view::size_type limit = tlx::string_view::npos);

/*!
 * Split the given string at each separator string into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.
 *
 * \param sep    separator string
 * \param str    string to split
 * \param limit  maximum number of parts returned
 * \return       vector containing each split substring
 */
std::vector<tlx::string_view> split_view(
    tlx::string_view sep, tlx::string_view str,
    tlx::string_view::size_type limit = tlx::string_view::npos);

/******************************************************************************/
// split_view() returning std::vector<tlx::string_view> with minimum fields

/*!
 * Split the given string at each separator character into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.  Returns a vector of strings with at least
 * min_fields and at most limit_fields, empty fields are added if needed.
 *
 * \param sep         separator string
 * \param str         string to split
 * \param min_fields  minimum number of parts returned
 * \param limit       maximum number of parts returned
 * \return            vector containing each split substring
 */
std::vector<tlx::string_view> split_view(char sep, tlx::string_view str,
                                         tlx::string_view::size_type min_fields,
                                         tlx::string_view::size_type limit);

/*!
 * Split the given string at each separator string into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.  Returns a vector of strings with at least
 * min_fields and at most limit_fields, empty fields are added if needed.
 *
 * \param sep         separator string
 * \param str         string to split
 * \param min_fields  minimum number of parts returned
 * \param limit       maximum number of parts returned
 * \return            vector containing each split substring
 */
std::vector<tlx::string_view> split_view(tlx::string_view sep,
                                         tlx::string_view str,
                                         tlx::string_view::size_type min_fields,
                                         tlx::string_view::size_type limit);

/******************************************************************************/
// split_view() into std::vector<tlx::string_view>

/*!
 * Split the given string at each separator character into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.
 *
 * \param into   destination std::vector
 * \param sep    separator character
 * \param str    string to split
 * \param limit  maximum number of parts returned
 * \return       vector containing each split substring
 */
std::vector<tlx::string_view>& split_view(
    std::vector<tlx::string_view>* into, char sep, tlx::string_view str,
    tlx::string_view::size_type limit = tlx::string_view::npos);

/*!
 * Split the given string at each separator string into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.
 *
 * \param into   destination std::vector
 * \param sep    separator string
 * \param str    string to split
 * \param limit  maximum number of parts returned
 * \return       vector containing each split substring
 */
std::vector<tlx::string_view>& split_view(
    std::vector<tlx::string_view>* into, tlx::string_view sep,
    tlx::string_view str,
    tlx::string_view::size_type limit = tlx::string_view::npos);

/******************************************************************************/
// split_view() into std::vector<tlx::string_view> with minimum fields

/*!
 * Split the given string at each separator character into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.  Returns a vector of strings with at least
 * min_fields and at most limit_fields, empty fields are added if needed.
 *
 * \param into        destination std::vector
 * \param sep         separator character
 * \param str         string to split
 * \param min_fields  minimum number of parts returned
 * \param limit       maximum number of parts returned
 * \return            vector containing each split substring
 */
std::vector<tlx::string_view>& split_view(
    std::vector<tlx::string_view>* into, char sep, tlx::string_view str,
    tlx::string_view::size_type min_fields, tlx::string_view::size_type limit);

/*!
 * Split the given string at each separator string into distinct substrings.
 * Multiple consecutive separators are considered individually and will result
 * in empty split substrings.  Returns a vector of strings with at least
 * min_fields and at most limit_fields, empty fields are added if needed.
 *
 * \param into        destination std::vector
 * \param sep         separator string
 * \param str         string to split
 * \param min_fields  minimum number of parts returned
 * \param limit       maximum number of parts returned
 * \return            vector containing each split substring
 */
std::vector<tlx::string_view>& split_view(
    std::vector<tlx::string_view>* into, tlx::string_view sep,
    tlx::string_view str, tlx::string_view::size_type min_fields,
    tlx::string_view::size_type limit);

//! \}
//! \}

} // namespace tlx

#endif // !TLX_STRING_SPLIT_VIEW_HEADER

/******************************************************************************/
