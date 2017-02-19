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

namespace tlx {

bool starts_with(const std::string& str, const std::string& match) {
    if (match.size() > str.size())
        return false;

    return std::equal(match.begin(), match.end(), str.begin());
}

bool starts_with_icase(const std::string& str, const std::string& match) {
    if (match.size() > str.size())
        return false;

    return std::equal(match.begin(), match.end(), str.begin(),
                      [](const char& c1, const char& c2) {
                          return std::tolower(c1) == std::tolower(c2);
                      });
}

} // namespace tlx

/******************************************************************************/
