/*******************************************************************************
 * tlx/string/split.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/string/split.hpp>

namespace tlx {

std::vector<std::string> split(
    char sep, const std::string& str, std::string::size_type limit) {

    std::vector<std::string> out;
    if (limit == 0) return out;

    std::string::const_iterator it = str.begin(), last = it;

    for ( ; it != str.end(); ++it)
    {
        if (*it == sep)
        {
            if (out.size() + 1 >= limit)
            {
                out.emplace_back(last, str.end());
                return out;
            }

            out.emplace_back(last, it);
            last = it + 1;
        }
    }

    out.emplace_back(last, it);

    return out;
}

std::vector<std::string> split(
    const std::string& sep, const std::string& str,
    std::string::size_type limit) {

    std::vector<std::string> out;
    if (limit == 0) return out;
    if (sep.size() == 0)
    {
        std::string::const_iterator it = str.begin();
        while (it != str.end()) {
            out.emplace_back(it, it + 1);
            ++it;
        }
        return out;
    }

    std::string::const_iterator it = str.begin(), last = it;

    for ( ; it + sep.size() < str.end(); ++it)
    {
        if (std::equal(sep.begin(), sep.end(), it))
        {
            if (out.size() + 1 >= limit)
            {
                out.emplace_back(last, str.end());
                return out;
            }

            out.emplace_back(last, it);
            last = it + sep.size();
        }
    }

    out.emplace_back(last, str.end());

    return out;
}

} // namespace tlx

/******************************************************************************/
