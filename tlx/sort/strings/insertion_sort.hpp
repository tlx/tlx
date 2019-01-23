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
#include <tlx/sort/strings/string_ptr.hpp>

namespace tlx {
namespace sort_strings_detail {

/******************************************************************************/

template <typename StringPtr>
static inline
void lcp_insertion_sort(const StringPtr& strptr, size_t depth,
                        size_t /* memory */) {
    typedef typename StringPtr::StringSet StringSet;
    typedef typename StringSet::Iterator Iterator;
    typedef typename StringSet::String String;
    typedef typename StringSet::CharIterator CharIterator;

    // this stores the begin iterator and size n, making the loops faster
    const typename StringPtr::StringSet& ss = strptr.active();
    size_t n = ss.size();
    if (n <= 1) return;

    const Iterator begin = ss.begin();

    for (size_t j = 0; j < n - 1; ++j)
    {
        // insert strings[j] into sorted strings[0..j-1]

        String new_str = std::move(ss[begin + j]);
        size_t new_lcp = depth; // start with LCP depth

        size_t i = j;
        while (i > 0)
        {
            size_t prev_lcp = new_lcp;

            String cur_str = std::move(ss[begin + i - 1]);
            size_t cur_lcp = strptr.get_lcp(i);

            if (cur_lcp < new_lcp)
            {
                // CASE 1: lcp goes down -> insert string

                // move comparison string back
                ss[begin + i - 1] = std::move(cur_str);
                break;
            }
            else if (cur_lcp == new_lcp)
            {
                // CASE 2: compare more characters

                CharIterator c1 = ss.get_chars(new_str, new_lcp);
                CharIterator c2 = ss.get_chars(cur_str, new_lcp);

                while (ss.is_equal(new_str, c1, cur_str, c2))
                    ++c1, ++c2, ++new_lcp;

                // if (new_str >= curr_str) -> insert string
                if (!ss.is_less(new_str, c1, cur_str, c2))
                {
                    // update lcp of prev (smaller string) with inserted string
                    strptr.set_lcp(i, new_lcp);
                    // lcp of inserted string with next string
                    new_lcp = prev_lcp;

                    // move comparison string back
                    ss[begin + i - 1] = std::move(cur_str);
                    break;
                }
            }
            // else (cur_lcp > new_lcp), CASE 3: nothing to do

            ss[begin + i] = std::move(cur_str);
            strptr.set_lcp(i + 1, cur_lcp);

            --i;
        }

        ss[begin + i] = std::move(new_str);
        strptr.set_lcp(i + 1, new_lcp);
    }

    // last loop specialized with checks for out-of-bound access to lcp.
    {
        size_t j = n - 1;

        // insert strings[j] into sorted strings[0..j-1]

        String new_str = std::move(ss[begin + j]);
        size_t new_lcp = depth; // start with LCP depth

        size_t i = j;
        while (i > 0)
        {
            size_t prev_lcp = new_lcp;

            String cur_str = std::move(ss[begin + i - 1]);
            size_t cur_lcp = strptr.get_lcp(i);

            if (cur_lcp < new_lcp)
            {
                // CASE 1: lcp goes down -> insert string

                // move comparison string back
                ss[begin + i - 1] = std::move(cur_str);
                break;
            }
            else if (cur_lcp == new_lcp)
            {
                // CASE 2: compare more characters

                CharIterator c1 = ss.get_chars(new_str, new_lcp);
                CharIterator c2 = ss.get_chars(cur_str, new_lcp);

                while (ss.is_equal(new_str, c1, cur_str, c2))
                    ++c1, ++c2, ++new_lcp;

                // if (new_str >= curr_str) -> insert string
                if (!ss.is_less(new_str, c1, cur_str, c2))
                {
                    // update lcp of prev (smaller string) with inserted string
                    strptr.set_lcp(i, new_lcp);
                    // lcp of inserted string with next string
                    new_lcp = prev_lcp;

                    // move comparison string back
                    ss[begin + i - 1] = std::move(cur_str);
                    break;
                }
            }
            // else (cur_lcp > new_lcp), CASE 3: nothing to do

            ss[begin + i] = std::move(cur_str);
            if (i + 1 < n) // check out-of-bounds copy
                strptr.set_lcp(i + 1, cur_lcp);

            --i;
        }

        ss[begin + i] = std::move(new_str);
        if (i + 1 < n) { // check out-of-bounds save
            strptr.set_lcp(i + 1, new_lcp);
        }
    }
}

/******************************************************************************/

//! Generic insertion sort for abstract string sets. This method only requires
//! O(1) additional memory for sorting n strings, but runs in time O(nD).
template <typename StringPtr>
static inline void insertion_sort(
    const StringPtr& strptr, size_t depth, size_t memory) {

    if (strptr.with_lcp)
        return lcp_insertion_sort(strptr, depth, memory);

    typedef typename StringPtr::StringSet StringSet;
    typedef typename StringSet::Iterator Iterator;
    typedef typename StringSet::String String;
    typedef typename StringSet::CharIterator CharIterator;

    // this stores the begin iterator and size n, making the loops faster
    const typename StringPtr::StringSet& ss = strptr.active();
    size_t n = ss.size();
    if (n <= 1) return;

    const Iterator begin = ss.begin();
    Iterator j;

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
