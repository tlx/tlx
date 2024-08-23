/*******************************************************************************
 * tlx/string/contains_word.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/contains_word.hpp>

namespace tlx {

static inline bool is_white(char c)
{
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

bool contains_word(tlx::string_view str, tlx::string_view word)
{
    // all strings contain the empty word
    if (word.empty())
        return true;

    tlx::string_view::const_iterator it = str.begin();

    while (it != str.end())
    {
        // skip over whitespace
        while (is_white(*it))
        {
            if (++it == str.end())
                return false;
        }

        // check if this non-whitespace matches the string
        tlx::string_view::const_iterator wi = word.begin();
        while (*it == *wi)
        {
            ++it, ++wi;
            if (wi == word.end())
            {
                if (it == str.end() || is_white(*it))
                    return true;
                break;
            }
            if (it == str.end())
                return false;
        }

        // skip over not matching whitespace
        while (!is_white(*it))
        {
            if (++it == str.end())
                return false;
        }
    }

    return false;
}

} // namespace tlx

/******************************************************************************/
