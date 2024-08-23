/*******************************************************************************
 * tlx/string/starts_with.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/starts_with.hpp>
#include <tlx/string/to_lower.hpp>
#include <algorithm>

namespace tlx {

/******************************************************************************/

bool starts_with(tlx::string_view str, tlx::string_view match)
{
    if (match.size() > str.size())
        return false;
    return std::equal(match.begin(), match.end(), str.begin());
}

/******************************************************************************/

bool starts_with_icase(tlx::string_view str, tlx::string_view match)
{
    if (match.size() > str.size())
        return false;
    return std::equal(match.begin(), match.end(), str.begin(),
                      [](const char& c1, const char& c2) {
                          return to_lower(c1) == to_lower(c2);
                      });
}

/******************************************************************************/

} // namespace tlx

/******************************************************************************/
