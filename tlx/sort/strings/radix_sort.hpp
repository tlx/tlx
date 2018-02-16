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

#include <string.h>

#include <stack>

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

    if (strptr.size() < g_inssort_threshold)
        return insertion_sort(strptr.copy_back(), depth);

    const StringSet& ss = strptr.active();

    // count character occurrences
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

    // count character occurrences
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
// Stack-Based Variants
/******************************************************************************/

template <typename StringSet>
struct RadixStep_CE0_sb {
    StringShadowPtr<StringSet> strptr;
    size_t                     idx, pos;
    size_t                     bkt_size[256];

    typedef typename StringSet::Iterator Iterator;

    RadixStep_CE0_sb(const StringShadowPtr<StringSet>& in_strptr, size_t depth)
        : strptr(in_strptr) {

        const StringSet& ss = strptr.active();

        // count character occurrences
        memset(bkt_size, 0, sizeof(bkt_size));
        for (Iterator i = ss.begin(); i != ss.end(); ++i)
            ++bkt_size[ss.get_uint8(ss[i], depth)];

        // prefix sum
        Iterator bkt_index[256];
        bkt_index[0] = strptr.shadow().begin();
        for (size_t i = 1; i < 256; ++i)
            bkt_index[i] = bkt_index[i - 1] + bkt_size[i - 1];

        // distribute
        for (Iterator i = ss.begin(); i != ss.end(); ++i)
            *(bkt_index[ss.get_uint8(ss[i], depth)]++) = std::move(ss[i]);

        idx = 0; // will increment to 1 on first process
        pos = bkt_size[0];

        // copy back finished strings in zeroth bucket
        strptr.flip(0, pos).copy_back();
    }
};

template <typename StringSet>
static inline void
bingmann_msd_CE0_sb(const StringSet& ss, size_t depth) {

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(ss, depth);

    typename StringSet::Container shadow = ss.allocate(ss.size());
    StringShadowPtr<StringSet> strptr(ss, StringSet(shadow));

    typedef RadixStep_CE0_sb<StringSet> RadixStep;

    std::stack<RadixStep, std::vector<RadixStep> > radixstack;
    radixstack.emplace(strptr, depth);

    while (radixstack.size())
    {
        while (radixstack.top().idx < 255)
        {
            RadixStep& rs = radixstack.top();

            // process the bucket rs.idx
            size_t bkt_size = rs.bkt_size[++rs.idx];

            if (bkt_size == 0) {
                // done
            }
            else if (bkt_size < g_inssort_threshold) {
                insertion_sort(
                    rs.strptr.flip(rs.pos, rs.pos + bkt_size).copy_back(),
                    depth + radixstack.size());
                rs.pos += bkt_size;
            }
            else
            {
                rs.pos += bkt_size;
                radixstack.emplace(rs.strptr.flip(rs.pos - bkt_size, rs.pos),
                                   depth + radixstack.size());
                // cannot add here, because rs may have invalidated
            }
        }
        radixstack.pop();
    }

    StringSet::deallocate(shadow);
}

/******************************************************************************/

template <typename StringSet>
struct RadixStep_CE2_sb {
    StringShadowPtr<StringSet> strptr;
    size_t                     idx, pos;
    size_t                     bkt_size[256];

    typedef typename StringSet::Iterator Iterator;

    RadixStep_CE2_sb(const StringShadowPtr<StringSet>& in_strptr, size_t depth,
                     uint8_t* charcache) : strptr(in_strptr) {

        const StringSet& ss = strptr.active();
        const size_t n = ss.size();

        // read characters and count character occurrences
        memset(bkt_size, 0, sizeof(bkt_size));
        uint8_t* cc = charcache;
        for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
            *cc = ss.get_uint8(ss[i], depth);
        for (cc = charcache; cc != charcache + n; ++cc)
            ++bkt_size[static_cast<uint8_t>(*cc)];

        // prefix sum
        Iterator bkt_index[256];
        bkt_index[0] = strptr.shadow().begin();
        for (size_t i = 1; i < 256; ++i)
            bkt_index[i] = bkt_index[i - 1] + bkt_size[i - 1];

        // distribute
        cc = charcache;
        for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
            *(bkt_index[static_cast<uint8_t>(*cc)]++) = std::move(ss[i]);

        idx = 0; // will increment to 1 on first process
        pos = bkt_size[0];

        // copy back finished strings in zeroth bucket
        strptr.flip(0, pos).copy_back();
    }
};

template <typename StringSet>
static inline void
bingmann_msd_CE2_sb(const StringShadowPtr<StringSet>& strptr,
                    uint8_t* charcache, size_t depth) {

    typedef RadixStep_CE2_sb<StringSet> RadixStep;

    std::stack<RadixStep, std::vector<RadixStep> > radixstack;
    radixstack.emplace(strptr, depth, charcache);

    while (TLX_LIKELY(!radixstack.empty()))
    {
        while (TLX_LIKELY(radixstack.top().idx < 255))
        {
            RadixStep& rs = radixstack.top();

            // process the bucket rs.idx
            size_t bkt_size = rs.bkt_size[++rs.idx];

            if (TLX_UNLIKELY(bkt_size == 0)) {
                // done
            }
            else if (bkt_size < g_inssort_threshold) {
                insertion_sort(
                    rs.strptr.flip(rs.pos, rs.pos + bkt_size).copy_back(),
                    depth + radixstack.size());
                rs.pos += bkt_size;
            }
            else
            {
                // have to increment first, as rs may be invalidated
                rs.pos += bkt_size;
                radixstack.emplace(rs.strptr.flip(rs.pos - bkt_size, rs.pos),
                                   depth + radixstack.size(), charcache);
            }
        }
        radixstack.pop();
    }
}

template <typename StringSet>
static inline void
bingmann_msd_CE2_sb(const StringSet& ss, size_t depth) {

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(ss, depth);

    typename StringSet::Container shadow = ss.allocate(ss.size());
    StringShadowPtr<StringSet> strptr(ss, StringSet(shadow));

    uint8_t* charcache = new uint8_t[ss.size()];

    bingmann_msd_CE2_sb<StringSet>(strptr, charcache, depth);

    delete[] charcache;
    StringSet::deallocate(shadow);
}

/******************************************************************************/

template <typename StringSet>
struct RadixStep_CE3_sb {
    static const size_t        RADIX = 0x10000;

    StringShadowPtr<StringSet> strptr;
    size_t                     idx, pos;
    size_t                     bkt_size[RADIX];

    typedef typename StringSet::Iterator Iterator;

    RadixStep_CE3_sb(const StringShadowPtr<StringSet>& in_strptr, size_t depth,
                     uint16_t* charcache) : strptr(in_strptr) {

        const StringSet& ss = strptr.active();
        const size_t n = ss.size();

        // read characters and count character occurrences
        memset(bkt_size, 0, sizeof(bkt_size));
        uint16_t* cc = charcache;
        for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
            *cc = ss.get_uint16(ss[i], depth);
        for (cc = charcache; cc != charcache + n; ++cc)
            ++bkt_size[static_cast<uint16_t>(*cc)];

        // prefix sum
        Iterator bkt_index[RADIX];
        bkt_index[0] = strptr.shadow().begin();
        for (size_t i = 1; i < RADIX; ++i)
            bkt_index[i] = bkt_index[i - 1] + bkt_size[i - 1];

        // distribute
        cc = charcache;
        for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
            *(bkt_index[static_cast<uint16_t>(*cc)]++) = std::move(ss[i]);

        idx = 0; // will increment to 1 on first process
        pos = bkt_size[0];

        // copy back finished strings in zeroth bucket
        strptr.flip(0, pos).copy_back();
    }
};

template <typename StringSet>
static inline void
bingmann_msd_CE3_sb(const StringSet& ss, size_t depth) {
    static const size_t RADIX = 0x10000;

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(ss, depth);

    if (ss.size() < RADIX)
        return bingmann_msd_CE2_sb(ss, depth);

    typename StringSet::Container shadow = ss.allocate(ss.size());
    StringShadowPtr<StringSet> strptr(ss, StringSet(shadow));

    uint16_t* charcache = new uint16_t[ss.size()];

    typedef RadixStep_CE3_sb<StringSet> RadixStep;

    typedef std::stack<RadixStep, std::vector<RadixStep> > radixstack_type;
    radixstack_type radixstack;
    radixstack.emplace(strptr, depth, charcache);

    while (TLX_LIKELY(!radixstack.empty()))
    {
        while (TLX_LIKELY(radixstack.top().idx < RADIX - 1))
        {
            RadixStep& rs = radixstack.top();

            // process the bucket rs.idx
            size_t bkt_size = rs.bkt_size[++rs.idx];

            if (TLX_UNLIKELY(bkt_size == 0)) {
                // done
            }
            else if (TLX_UNLIKELY((rs.idx & 0xFF) == 0)) { // zero-termination
                rs.strptr.flip(rs.pos, rs.pos + bkt_size).copy_back();
                rs.pos += bkt_size;
            }
            else if (TLX_UNLIKELY(bkt_size < g_inssort_threshold))
            {
                insertion_sort(
                    rs.strptr.flip(rs.pos, rs.pos + bkt_size).copy_back(),
                    depth + 2 * radixstack.size());
                rs.pos += bkt_size;
            }
            else if (bkt_size < RADIX)
            {
                bingmann_msd_CE2_sb(rs.strptr.flip(rs.pos, rs.pos + bkt_size),
                                    reinterpret_cast<uint8_t*>(charcache),
                                    depth + 2 * radixstack.size());
                rs.pos += bkt_size;
            }
            else
            {
                // have to increment first, as rs may be invalidated
                rs.pos += bkt_size;
                radixstack.emplace(
                    rs.strptr.flip(rs.pos - bkt_size, rs.pos),
                    depth + 2 * radixstack.size(), charcache);
            }
        }
        radixstack.pop();
    }

    delete[] charcache;
    StringSet::deallocate(shadow);
}

/******************************************************************************/

template <typename StringSet>
struct RadixStep_CI2_sb {

    typedef typename StringSet::Iterator Iterator;
    typedef typename StringSet::String String;

    size_t   idx;
    Iterator pos;
    size_t   bkt_size[256];

    RadixStep_CI2_sb(const StringSet& ss, size_t depth, uint8_t* charcache) {
        const size_t n = ss.size();

        // read characters and count character occurrences
        memset(bkt_size, 0, sizeof(bkt_size));
        uint8_t* cc = charcache;
        for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
            *cc = ss.get_uint8(ss[i], depth);
        for (cc = charcache; cc != charcache + n; ++cc)
            ++bkt_size[static_cast<uint8_t>(*cc)];

        // inclusive prefix sum
        size_t bkt[256];
        bkt[0] = bkt_size[0];
        size_t last_bkt_size = bkt_size[0];
        for (size_t i = 1; i < 256; ++i) {
            bkt[i] = bkt[i - 1] + bkt_size[i];
            if (bkt_size[i]) last_bkt_size = bkt_size[i];
        }

        // premute in-place
        for (size_t i = 0, j; i < n - last_bkt_size; )
        {
            String perm = std::move(ss[ss.begin() + i]);
            uint8_t permch = charcache[i];
            while ((j = --bkt[permch]) > i)
            {
                std::swap(perm, ss[ss.begin() + j]);
                std::swap(permch, charcache[j]);
            }
            ss[ss.begin() + i] = std::move(perm);
            i += bkt_size[permch];
        }

        pos = ss.begin() + bkt_size[0];
        idx = 0; // will increment to 1 on first process, bkt 0 is not sorted further
    }
};

template <typename StringSet>
static inline void
bingmann_msd_CI2_sb(const StringSet& ss, uint8_t* charcache, size_t depth) {

    typedef RadixStep_CI2_sb<StringSet> RadixStep;

    std::stack<RadixStep, std::vector<RadixStep> > radixstack;
    radixstack.emplace(ss, depth, charcache);

    while (TLX_LIKELY(!radixstack.empty()))
    {
        while (TLX_LIKELY(radixstack.top().idx < 255))
        {
            RadixStep& rs = radixstack.top();

            // process the bucket rs.idx
            size_t bkt_size = rs.bkt_size[++rs.idx];

            if (TLX_UNLIKELY(bkt_size <= 1)) {
                // done
                rs.pos += bkt_size;
            }
            else if (bkt_size < g_inssort_threshold) {
                insertion_sort(
                    ss.sub(rs.pos, rs.pos + bkt_size),
                    depth + radixstack.size());
                rs.pos += bkt_size;
            }
            else
            {
                // have to increment first, as rs may be invalidated
                rs.pos += bkt_size;
                radixstack.emplace(ss.sub(rs.pos - bkt_size, rs.pos),
                                   depth + radixstack.size(), charcache);
            }
        }
        radixstack.pop();
    }
}

template <typename StringSet>
static inline void
bingmann_msd_CI2_sb(const StringSet& ss, size_t depth) {

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(ss, depth);

    uint8_t* charcache = new uint8_t[ss.size()];

    bingmann_msd_CI2_sb<StringSet>(ss, charcache, depth);

    delete[] charcache;
}

/******************************************************************************/

template <typename StringSet>
struct RadixStep_CI3_sb {
    static const size_t RADIX = 0x10000;

    typedef typename StringSet::Iterator Iterator;
    typedef typename StringSet::String String;

    size_t              idx;
    Iterator            pos;
    size_t              bkt_size[RADIX];

    RadixStep_CI3_sb(const StringSet& ss, size_t depth, uint16_t* charcache) {
        const size_t n = ss.size();

        // read characters and count character occurrences
        memset(bkt_size, 0, sizeof(bkt_size));
        uint16_t* cc = charcache;
        for (Iterator i = ss.begin(); i != ss.end(); ++i, ++cc)
            *cc = ss.get_uint16(ss[i], depth);
        for (cc = charcache; cc != charcache + n; ++cc)
            ++bkt_size[static_cast<uint16_t>(*cc)];

        // inclusive prefix sum
        size_t bkt[RADIX];
        bkt[0] = bkt_size[0];
        size_t last_bkt_size = bkt_size[0];
        for (size_t i = 1; i < RADIX; ++i) {
            bkt[i] = bkt[i - 1] + bkt_size[i];
            if (bkt_size[i]) last_bkt_size = bkt_size[i];
        }

        // premute in-place
        for (size_t i = 0, j; i < n - last_bkt_size; )
        {
            String perm = std::move(ss[ss.begin() + i]);
            uint16_t permch = charcache[i];
            while ((j = --bkt[permch]) > i)
            {
                std::swap(perm, ss[ss.begin() + j]);
                std::swap(permch, charcache[j]);
            }
            ss[ss.begin() + i] = std::move(perm);
            i += bkt_size[permch];
        }

        // will increment to 1 on first process, bkt 0 is not sorted further
        idx = 0;
        pos = ss.begin() + bkt_size[0];
    }
};

template <typename StringSet>
static inline void
bingmann_msd_CI3_sb(const StringSet& ss, size_t depth) {
    static const size_t RADIX = 0x10000;

    if (ss.size() < g_inssort_threshold)
        return insertion_sort(ss, depth);

    if (ss.size() < RADIX)
        return bingmann_msd_CI2_sb(ss, depth);

    typedef RadixStep_CI3_sb<StringSet> RadixStep;

    uint16_t* charcache = new uint16_t[ss.size()];

    std::stack<RadixStep, std::vector<RadixStep> > radixstack;
    radixstack.emplace(ss, depth, charcache);

    while (TLX_LIKELY(!radixstack.empty()))
    {
        while (TLX_LIKELY(radixstack.top().idx < RADIX - 1))
        {
            RadixStep& rs = radixstack.top();

            // process the bucket rs.idx
            size_t bkt_size = rs.bkt_size[++rs.idx];

            if (TLX_UNLIKELY(bkt_size <= 1)) {
                // done
                rs.pos += bkt_size;
            }
            else if (TLX_UNLIKELY((rs.idx & 0xFF) == 0)) { // zero-termination
                rs.pos += bkt_size;
            }
            else if (TLX_UNLIKELY(bkt_size < g_inssort_threshold))
            {
                insertion_sort(
                    ss.sub(rs.pos, rs.pos + bkt_size),
                    depth + 2 * radixstack.size());
                rs.pos += bkt_size;
            }
            else if (bkt_size < RADIX)
            {
                bingmann_msd_CI2_sb(ss.sub(rs.pos, rs.pos + bkt_size),
                                    reinterpret_cast<uint8_t*>(charcache),
                                    depth + 2 * radixstack.size());
                rs.pos += bkt_size;
            }
            else
            {
                // have to increment first, as rs may be invalidated
                rs.pos += bkt_size;
                radixstack.emplace(
                    ss.sub(rs.pos - bkt_size, rs.pos),
                    depth + 2 * radixstack.size(), charcache);
            }
        }
        radixstack.pop();
    }

    delete[] charcache;
}

/******************************************************************************/

} // namespace sort_strings_detail
} // namespace tlx

#endif // !TLX_SORT_STRINGS_RADIX_SORT_HEADER

/******************************************************************************/
