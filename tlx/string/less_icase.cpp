/*******************************************************************************
 * tlx/string/less_icase.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/less_icase.hpp>
#include <tlx/string/to_lower.hpp>
#include <algorithm>

namespace tlx {

bool less_icase(const char* a, const char* b)
{
    while (*a != 0 && *b != 0 && to_lower(*a) == to_lower(*b))
        ++a, ++b;

    if (*a == 0 && *b == 0)
        return false;
    if (*a == 0)
        return true;
    if (*b == 0)
        return false;

    return to_lower(*a) < to_lower(*b);
}

bool less_icase(const char* a, tlx::string_view b)
{
    tlx::string_view::const_iterator bi = b.begin();

    while (*a != 0 && bi != b.end() && to_lower(*a) == to_lower(*bi))
        ++a, ++bi;

    if (*a == 0 && bi == b.end())
        return false;
    if (*a == 0)
        return true;
    if (bi == b.end())
        return false;

    return to_lower(*a) < to_lower(*bi);
}

bool less_icase(tlx::string_view a, const char* b)
{
    tlx::string_view::const_iterator ai = a.begin();

    while (ai != a.end() && *b != 0 && to_lower(*ai) == to_lower(*b))
        ++ai, ++b;

    if (ai == a.end() && *b == 0)
        return false;
    if (ai == a.end())
        return true;
    if (*b == 0)
        return false;

    return to_lower(*ai) < to_lower(*b);
}

bool less_icase(tlx::string_view a, tlx::string_view b)
{
    return std::lexicographical_compare(
        a.begin(), a.end(), b.begin(), b.end(),
        [](char c1, char c2) { return to_lower(c1) < to_lower(c2); });
}

} // namespace tlx

/******************************************************************************/
