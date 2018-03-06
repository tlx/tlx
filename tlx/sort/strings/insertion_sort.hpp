/*******************************************************************************
 * tlx/sort/strings/insertion_sort.hpp
 *
 * Base insertion string sort. This is an internal implementation header, see
 * tlx/sort/strings.hpp for public front-end functions.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_STRINGS_INSERTION_SORT_HEADER
#define TLX_SORT_STRINGS_INSERTION_SORT_HEADER

#include <tlx/define/likely.hpp>
#include <tlx/sort/strings/string_set.hpp>

namespace tlx {
namespace sort_strings_detail {

/******************************************************************************/

//! Generic insertion sort for abstract string sets. This method only requires
//! O(1) additional memory for sorting n strings, but runs in time O(nD).
template <typename StringSet>
static inline void insertion_sort(
    const StringSet& ss, size_t depth, size_t /* memory */) {
    typedef typename StringSet::Iterator Iterator;
    typedef typename StringSet::String String;
    typedef typename StringSet::CharIterator CharIterator;

    // this stores the begin iterator and size n, making the loops faster
    const Iterator begin = ss.begin();
    Iterator j;
    size_t n = ss.size();

    for (Iterator i = begin + 1; TLX_UNLIKELY(--n != 0); ++i)
    {
        String tmp = std::move(ss[i]);
        j = i;

        while (TLX_LIKELY(j != begin))
        {
            CharIterator s = ss.get_chars(ss[j - 1], depth);
            CharIterator t = ss.get_chars(tmp, depth);

            while (TLX_LIKELY(ss.is_equal(ss[j - 1], s, tmp, t)))
                ++s, ++t;

            if (TLX_UNLIKELY(ss.is_leq(ss[j - 1], s, tmp, t))) {
                break;
            }

            ss[j] = std::move(ss[j - 1]);
            --j;
        }

        ss[j] = std::move(tmp);
    }
}

/******************************************************************************/

} // namespace sort_strings_detail
} // namespace tlx

#endif // !TLX_SORT_STRINGS_INSERTION_SORT_HEADER

/******************************************************************************/
