/*******************************************************************************
 * tlx/string/ends_with.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/string/ends_with.hpp>

#include <algorithm>
#include <cstring>

#include <tlx/string/to_lower.hpp>

namespace tlx {

bool ends_with(const std::string& str, const std::string& match) {
    if (match.size() > str.size())
        return false;

    return std::equal(match.begin(), match.end(), str.end() - match.size());
}

bool ends_with(const std::string& str, const char* match) {
    size_t match_size = strlen(match);
    if (match_size > str.size())
        return false;

    std::string::const_iterator s = str.end() - match_size;

    while (*match != 0) {
        if (s == str.end() || *s != *match) return false;
        ++s, ++match;
    }

    return true;
}

/******************************************************************************/

bool ends_with_icase(const std::string& str, const std::string& match) {
    if (match.size() > str.size())
        return false;

    return std::equal(match.begin(), match.end(), str.end() - match.size(),
                      [](const char& c1, const char& c2) {
                          return to_lower(c1) == to_lower(c2);
                      });
}

bool ends_with_icase(const std::string& str, const char* match) {
    size_t match_size = strlen(match);
    if (match_size > str.size())
        return false;

    std::string::const_iterator s = str.end() - match_size;

    while (*match != 0) {
        if (s == str.end() || to_lower(*s) != to_lower(*match))
            return false;
        ++s, ++match;
    }

    return true;
}

} // namespace tlx

/******************************************************************************/
