/*******************************************************************************
 * tlx/string/split_words.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/string/split_words.hpp>

namespace tlx {

std::vector<std::string> split_words(
    const std::string& str, std::string::size_type limit) {
    std::vector<std::string> out;
    if (limit == 0) return out;

    std::string::const_iterator it = str.begin(), last = it;

    for ( ; it != str.end(); ++it)
    {
        if (*it == ' ' || *it == '\n' || *it == '\t' || *it == '\r')
        {
            if (it == last) { // skip over empty split substrings
                last = it + 1;
                continue;
            }

            if (out.size() + 1 >= limit)
            {
                out.emplace_back(last, str.end());
                return out;
            }

            out.emplace_back(last, it);
            last = it + 1;
        }
    }

    if (last != it)
        out.emplace_back(last, it);

    return out;
}

} // namespace tlx

/******************************************************************************/
