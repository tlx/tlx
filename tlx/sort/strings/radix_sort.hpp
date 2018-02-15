/*******************************************************************************
 * tlx/sort/strings/radix_sort.hpp
 *
 * Out-of-place and in-place radix sort for strings. This is an internal
 * implementation header, see tlx/sort/strings.hpp for public front-end
 * functions.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_STRINGS_RADIX_SORT_HEADER
#define TLX_SORT_STRINGS_RADIX_SORT_HEADER

#include <tlx/define/likely.hpp>
#include <tlx/sort/strings/string_set.hpp>

namespace tlx {
namespace sort_strings_detail {

/******************************************************************************/

static const size_t g_inssort_threshold = 32;

/******************************************************************************/
//! out-of-place 8-bit radix sort without character caching

template <typename StringSet>
static inline void
radix_sort_outofplace(const StringShadowPtr<StringSet>& strptr, size_t depth) {
    typedef typename StringSet::Iterator Iterator;

    const StringSet& ss = strptr.active();

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(strptr.copy_back().active(), depth);

    // count character occurances
    size_t bkt_size[256] = { 0 };
    for (Iterator i = ss.begin(); i != ss.end(); ++i)
        ++bkt_size[ss.get_uint8(ss[i], depth)];

    {
        // prefix sum
        Iterator bkt_index[256];
        bkt_index[0] = strptr.shadow().begin();
        for (size_t i = 1; i < 256; ++i)
            bkt_index[i] = bkt_index[i - 1] + bkt_size[i - 1];

        // distribute
        for (Iterator i = ss.begin(); i != ss.end(); ++i)
            *(bkt_index[ss.get_uint8(ss[i], depth)]++) = std::move(ss[i]);
    }

    // recursion
    size_t bsum_begin = 0, bsum_end = bsum_begin + bkt_size[0];
    // copy back finished strings in zeroth bucket
    strptr.flip(bsum_begin, bsum_end).copy_back();
    for (size_t i = 1; i < 256; ++i) {
        bsum_begin = bsum_end, bsum_end += bkt_size[i];
        if (bkt_size[i] == 0) {
            // done
        }
        else if (bkt_size[i] == 1) {
            strptr.flip(bsum_begin, bsum_end).copy_back();
        }
        else {
            radix_sort_outofplace(strptr.flip(bsum_begin, bsum_end), depth + 1);
        }
    }
}

template <typename StringSet>
static inline void
radix_sort_outofplace(const StringSet& ss, size_t depth) {
    typename StringSet::Container shadow = ss.allocate(ss.size());
    StringShadowPtr<StringSet> strptr(ss, StringSet(shadow));
    radix_sort_outofplace(strptr, depth);
    StringSet::deallocate(shadow);
}

/******************************************************************************/
//! in-place 8-bit radix sort with character caching

template <typename StringSet>
static inline size_t*
radix_sort_inplace_bktsize(
    const StringSet& ss, typename StringSet::Char* charcache, size_t depth) {
    typedef typename StringSet::Iterator Iterator;
    typedef typename StringSet::String String;
    typedef typename StringSet::Char Char;

    // cache characters
    Char* cc = charcache;
    for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
        *cc = *ss.get_chars(ss[i], depth);

    // count character occurances
    size_t* bkt_size = new size_t[256]();
    for (cc = charcache; cc != charcache + ss.size(); ++cc)
        ++bkt_size[static_cast<size_t>(*cc)];

    // inclusive prefix sum
    size_t bkt[256];
    bkt[0] = bkt_size[0];
    size_t last_bkt_size = bkt_size[0];
    for (size_t i = 1; i < 256; ++i) {
        bkt[i] = bkt[i - 1] + bkt_size[i];
        if (bkt_size[i]) last_bkt_size = bkt_size[i];
    }

    // premute in-place
    for (size_t i = 0, j; i < ss.size() - last_bkt_size; )
    {
        String perm = std::move(ss[ss.begin() + i]);
        Char permch = charcache[i];
        while ((j = --bkt[static_cast<size_t>(permch)]) > i)
        {
            std::swap(perm, ss[ss.begin() + j]);
            std::swap(permch, charcache[j]);
        }
        ss[ss.begin() + i] = std::move(perm);
        i += bkt_size[static_cast<size_t>(permch)];
    }

    return bkt_size;
}

template <typename StringSet>
static inline void
radix_sort_inplace(
    const StringSet& ss, typename StringSet::Char* charcache, size_t depth) {
    typedef typename StringSet::Iterator Iterator;

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(ss, depth);

    size_t* bkt_size = radix_sort_inplace_bktsize(ss, charcache, depth);

    // recursion
    Iterator bsum = ss.begin() + bkt_size[0];
    for (size_t i = 1; i < 256; ++i) {
        Iterator bend = bsum + bkt_size[i];
        if (bkt_size[i] > 1)
            radix_sort_inplace(ss.sub(bsum, bend), charcache, depth + 1);
        bsum = bend;
    }

    delete[] bkt_size;
}

template <typename StringSet>
static inline void radix_sort_inplace(const StringSet& ss, size_t depth) {
    typedef typename StringSet::Char Char;
    Char* charcache = new Char[ss.size()];
    radix_sort_inplace(ss, charcache, depth);
    delete[] charcache;
}

/******************************************************************************/

} // namespace sort_strings_detail
} // namespace tlx

#endif // !TLX_SORT_STRINGS_RADIX_SORT_HEADER

/******************************************************************************/
