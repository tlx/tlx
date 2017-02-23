/*******************************************************************************
 * tlx/string/starts_with.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/string/starts_with.hpp>

#include <algorithm>

#include <tlx/string/to_lower.hpp>

namespace tlx {

bool starts_with(const std::string& str, const std::string& match) {
    if (match.size() > str.size())
        return false;

    return std::equal(match.begin(), match.end(), str.begin());
}

bool starts_with(const std::string& str, const char* match) {
    std::string::const_iterator s = str.begin();

    while (*match != 0) {
        if (s == str.end() || *s != *match) return false;
        ++s, ++match;
    }

    return true;
}

/******************************************************************************/

bool starts_with_icase(const std::string& str, const std::string& match) {
    if (match.size() > str.size())
        return false;

    return std::equal(match.begin(), match.end(), str.begin(),
                      [](const char& c1, const char& c2) {
                          return to_lower(c1) == to_lower(c2);
                      });
}

bool starts_with_icase(const std::string& str, const char* match) {
    std::string::const_iterator s = str.begin();

    while (*match != 0) {
        if (s == str.end() || to_lower(*s) != to_lower(*match))
            return false;
        ++s, ++match;
    }

    return true;
}

} // namespace tlx

/******************************************************************************/
