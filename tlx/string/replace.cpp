/*******************************************************************************
 * tlx/string/replace.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/replace.hpp>
#include <cstring>
#include <string>

namespace tlx {

/******************************************************************************/
// replace_first() in-place

std::string& replace_first(std::string* str, tlx::string_view needle,
                           tlx::string_view instead)
{
    std::string::size_type firstpos =
        str->find(needle.data(), 0, needle.size());

    if (firstpos != std::string::npos)
        str->replace(firstpos, needle.size(), instead.data(), instead.size());

    return *str;
}

std::string& replace_first(std::string* str, char needle, char instead)
{
    std::string::size_type firstpos = str->find(needle);

    if (firstpos != std::string::npos)
        (*str)[firstpos] = instead;

    return *str;
}

/******************************************************************************/
// replace_first() copy

std::string replace_first(tlx::string_view str, tlx::string_view needle,
                          tlx::string_view instead)
{
    std::string newstr(str.data(), str.size());
    std::string::size_type firstpos =
        newstr.find(needle.data(), 0, needle.size());

    if (firstpos != std::string::npos)
        newstr.replace(firstpos, needle.size(), instead.data(), instead.size());

    return newstr;
}

std::string replace_first(tlx::string_view str, char needle, char instead)
{
    std::string newstr(str.data(), str.size());
    std::string::size_type firstpos = newstr.find(needle);

    if (firstpos != std::string::npos)
        newstr[firstpos] = instead;

    return newstr;
}

/******************************************************************************/
// replace_all() in-place

std::string& replace_all(std::string* str, tlx::string_view needle,
                         tlx::string_view instead)
{
    std::string::size_type lastpos = 0, thispos;

    while ((thispos = str->find(needle.data(), lastpos, needle.size())) !=
           std::string::npos)
    {
        str->replace(thispos, needle.size(), instead.data(), instead.size());
        lastpos = thispos + instead.size();
    }
    return *str;
}

std::string& replace_all(std::string* str, char needle, char instead)
{
    std::string::size_type lastpos = 0, thispos;

    while ((thispos = str->find(needle, lastpos)) != std::string::npos)
    {
        (*str)[thispos] = instead;
        lastpos = thispos + 1;
    }
    return *str;
}

/******************************************************************************/
// replace_all() copy

std::string replace_all(tlx::string_view str, tlx::string_view needle,
                        tlx::string_view instead)
{
    std::string newstr(str.data(), str.size());
    std::string::size_type lastpos = 0, thispos;

    while ((thispos = newstr.find(needle.data(), lastpos, needle.size())) !=
           std::string::npos)
    {
        newstr.replace(thispos, needle.size(), instead.data(), instead.size());
        lastpos = thispos + instead.size();
    }
    return newstr;
}

std::string replace_all(tlx::string_view str, char needle, char instead)
{
    std::string newstr(str.data(), str.size());
    std::string::size_type lastpos = 0, thispos;

    while ((thispos = newstr.find(needle, lastpos)) != std::string::npos)
    {
        newstr[thispos] = instead;
        lastpos = thispos + 1;
    }
    return newstr;
}

} // namespace tlx

/******************************************************************************/
