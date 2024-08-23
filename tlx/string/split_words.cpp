/*******************************************************************************
 * tlx/string/split_words.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/split_words.hpp>
#include <string>
#include <vector>

namespace tlx {

std::vector<std::string> split_words(tlx::string_view str,
                                     std::string::size_type limit)
{
    std::vector<std::string> out;
    if (limit == 0)
        return out;

    tlx::string_view::const_iterator it = str.begin(), last = it;

    for (; it != str.end(); ++it)
    {
        if (*it == ' ' || *it == '\n' || *it == '\t' || *it == '\r')
        {
            if (it == last)
            { // skip over empty split substrings
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

std::vector<tlx::string_view> split_words_view(tlx::string_view str,
                                               std::string::size_type limit)
{
    std::vector<tlx::string_view> out;
    if (limit == 0)
        return out;

    tlx::string_view::const_iterator it = str.begin(), last = it;

    for (; it != str.end(); ++it)
    {
        if (*it == ' ' || *it == '\n' || *it == '\t' || *it == '\r')
        {
            if (it == last)
            { // skip over empty split substrings
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
