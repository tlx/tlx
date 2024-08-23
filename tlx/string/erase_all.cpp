/*******************************************************************************
 * tlx/string/erase_all.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/erase_all.hpp>
#include <string>

namespace tlx {

/******************************************************************************/
// erase_all() in-place

std::string& erase_all(std::string* str, char drop)
{
    std::string::size_type pos1 = std::string::npos;
    std::string::size_type pos2{};

    while ((pos1 = str->find_last_of(drop, pos1)) != std::string::npos)
    {
        pos2 = str->find_last_not_of(drop, pos1);
        if (pos2 == std::string::npos)
        {
            str->erase(0, pos1 - pos2);
            return *str;
        }
        str->erase(pos2 + 1, pos1 - pos2);
        pos1 = pos2;
    }

    return *str;
}

std::string& erase_all(std::string* str, tlx::string_view drop)
{
    std::string::size_type pos1 = std::string::npos;
    std::string::size_type pos2{};

    while ((pos1 = str->find_last_of(drop.data(), pos1, drop.size())) !=
           std::string::npos)
    {
        pos2 = str->find_last_not_of(drop.data(), pos1, drop.size());
        if (pos2 == std::string::npos)
        {
            str->erase(0, pos1 - pos2);
            return *str;
        }
        str->erase(pos2 + 1, pos1 - pos2);
        pos1 = pos2;
    }

    return *str;
}

/******************************************************************************/
// erase_all() copy

std::string erase_all(tlx::string_view str, char drop)
{
    std::string out;
    out.reserve(str.size());

    tlx::string_view::const_iterator si = str.begin();
    while (si != str.end())
    {
        if (*si != drop)
            out += *si;
        ++si;
    }

    return out;
}

std::string erase_all(tlx::string_view str, tlx::string_view drop)
{
    std::string out;
    out.reserve(str.size());

    tlx::string_view::const_iterator si = str.begin();
    while (si != str.end())
    {
        // search for letter
        const char* d = drop.begin();
        while (d != drop.end())
        {
            if (*si == *d)
                break;
            ++d;
        }
        // append if not found
        if (d == drop.end())
            out += *si;
        ++si;
    }

    return out;
}

} // namespace tlx

/******************************************************************************/
