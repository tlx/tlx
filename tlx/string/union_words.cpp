/*******************************************************************************
 * tlx/string/union_words.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/string/contains_word.hpp>
#include <tlx/string/union_words.hpp>

namespace tlx {

std::string union_words(const std::string& wordsA, const std::string& wordsB) {
    std::string words = wordsA;

    std::string::const_iterator it = wordsB.begin();

    while (it != wordsB.end())
    {
        // skip over whitespace
        while (*it == ' ' || *it == '\n' || *it == '\t' || *it == '\r') {
            if (++it == wordsB.end()) break;
        }

        std::string::const_iterator i1 = it;

        // find first non-whitespace
        while (it != wordsB.end() &&
               *it != ' ' && *it != '\n' && *it != '\t' && *it != '\r')
            ++it;

        std::string w(i1, it);

        if (!contains_word(words, w)) {
            if (!words.empty())
                words += ' ';
            words += w;
        }
    }

    return words;
}

} // namespace tlx

/******************************************************************************/
