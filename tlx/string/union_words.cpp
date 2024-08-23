/*******************************************************************************
 * tlx/string/union_words.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/contains_word.hpp>
#include <tlx/string/union_words.hpp>
#include <string>

namespace tlx {

std::string union_words(tlx::string_view wordsA, tlx::string_view wordsB)
{
    std::string words(wordsA.data(), wordsA.size());

    tlx::string_view::const_iterator it = wordsB.begin();

    while (it != wordsB.end())
    {
        // skip over whitespace
        while (*it == ' ' || *it == '\n' || *it == '\t' || *it == '\r')
        {
            if (++it == wordsB.end())
                break;
        }

        tlx::string_view::const_iterator i1 = it;

        // find first non-whitespace
        while (it != wordsB.end() && *it != ' ' && *it != '\n' && *it != '\t' &&
               *it != '\r')
            ++it;

        tlx::string_view w(i1, it);

        if (!contains_word(words, w))
        {
            if (!words.empty())
                words += ' ';
            words.append(w.data(), w.size());
        }
    }

    return words;
}

} // namespace tlx

/******************************************************************************/
