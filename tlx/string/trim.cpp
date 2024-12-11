/*******************************************************************************
 * tlx/string/trim.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/trim.hpp>
#include <cstring>
#include <string>

namespace tlx {

/******************************************************************************/

std::string& trim(std::string* str)
{
    return trim(str, " \r\n\t");
}

std::string& trim(std::string* str, tlx::string_view drop)
{
    std::string::size_type pos =
        str->find_last_not_of(drop.data(), std::string::npos, drop.size());
    if (pos != std::string::npos)
    {
        str->erase(pos + 1);
        pos = str->find_first_not_of(drop.data(), 0, drop.size());
        if (pos != std::string::npos)
            str->erase(0, pos);
    }
    else
        str->erase(str->begin(), str->end());

    return *str;
}

std::string& trim(std::string* str, char drop)
{
    std::string::size_type pos = str->find_last_not_of(drop);
    if (pos != std::string::npos)
    {
        str->erase(pos + 1);
        pos = str->find_first_not_of(drop);
        if (pos != std::string::npos)
            str->erase(0, pos);
    }
    else
        str->erase(str->begin(), str->end());

    return *str;
}

tlx::string_view& trim(tlx::string_view* str)
{
    return trim(str, " \r\n\t");
}

tlx::string_view& trim(tlx::string_view* str, tlx::string_view drop)
{
    tlx::string_view::size_type pos =
        str->find_last_not_of(drop.data(), std::string::npos, drop.size());
    if (pos == std::string::npos)
        str->clear();
    else
    {
        str->remove_suffix(str->size() - pos - 1);

        pos = str->find_first_not_of(drop.data(), 0, drop.size());
        if (pos != std::string::npos)
            str->remove_prefix(pos);
        else
            str->clear();
    }

    return *str;
}

tlx::string_view& trim(tlx::string_view* str, char drop)
{
    tlx::string_view::size_type pos = str->find_last_not_of(drop);
    if (pos == std::string::npos)
        str->clear();
    else
    {
        str->remove_suffix(str->size() - pos - 1);

        pos = str->find_first_not_of(drop);
        if (pos != std::string::npos)
            str->remove_prefix(pos);
        else
            str->clear();
    }

    return *str;
}

tlx::string_view trim(tlx::string_view str)
{
    return trim(str, " \r\n\t");
}

tlx::string_view trim(tlx::string_view str, tlx::string_view drop)
{
    tlx::string_view out = str;

    // trim beginning
    tlx::string_view::size_type pos =
        out.find_first_not_of(drop.data(), 0, drop.size());
    if (pos == tlx::string_view::npos)
        return tlx::string_view();
    out.remove_prefix(pos);

    // trim end
    pos = out.find_last_not_of(drop.data(), out.size(), drop.size());
    if (pos != tlx::string_view::npos)
        out.remove_suffix(out.size() - pos - 1);

    return out;
}

tlx::string_view trim(tlx::string_view str, char drop)
{
    tlx::string_view out = str;

    // trim beginning
    tlx::string_view::size_type pos = out.find_first_not_of(drop);
    if (pos == tlx::string_view::npos)
        return tlx::string_view();
    out.remove_prefix(pos);

    // trim end
    pos = out.find_last_not_of(drop);
    if (pos != tlx::string_view::npos)
        out.remove_suffix(out.size() - pos - 1);

    return out;
}

/******************************************************************************/

std::string& trim_right(std::string* str)
{
    return trim_right(str, " \r\n\t");
}

std::string& trim_right(std::string* str, tlx::string_view drop)
{
    str->erase(str->find_last_not_of(drop.data(), tlx::string_view::npos,
                                     drop.size()) +
                   1,
               std::string::npos);
    return *str;
}

std::string& trim_right(std::string* str, char drop)
{
    str->erase(str->find_last_not_of(drop) + 1, std::string::npos);
    return *str;
}

tlx::string_view& trim_right(tlx::string_view* str)
{
    return trim_right(str, " \r\n\t");
}

tlx::string_view& trim_right(tlx::string_view* str, tlx::string_view drop)
{
    tlx::string_view::size_type pos =
        str->find_last_not_of(drop.data(), tlx::string_view::npos, drop.size());
    if (pos != tlx::string_view::npos)
        str->remove_suffix(str->size() - pos - 1);
    else
        str->clear();

    return *str;
}

tlx::string_view& trim_right(tlx::string_view* str, char drop)
{
    tlx::string_view::size_type pos = str->find_last_not_of(drop);
    if (pos != tlx::string_view::npos)
        str->remove_suffix(str->size() - pos - 1);
    else
        str->clear();

    return *str;
}

tlx::string_view trim_right(tlx::string_view str)
{
    return trim_right(str, " \r\n\t");
}

tlx::string_view trim_right(tlx::string_view str, tlx::string_view drop)
{
    tlx::string_view::size_type pos =
        str.find_last_not_of(drop.data(), tlx::string_view::npos, drop.size());
    if (pos == tlx::string_view::npos)
        return tlx::string_view();

    return str.substr(0, pos + 1);
}

tlx::string_view trim_right(tlx::string_view str, char drop)
{
    tlx::string_view::size_type pos = str.find_last_not_of(drop);
    if (pos == tlx::string_view::npos)
        return tlx::string_view();

    return str.substr(0, pos + 1);
}

/******************************************************************************/

std::string& trim_left(std::string* str)
{
    return trim_left(str, " \r\n\t");
}

std::string& trim_left(std::string* str, tlx::string_view drop)
{
    str->erase(0, str->find_first_not_of(drop.data(), 0, drop.size()));
    return *str;
}

std::string& trim_left(std::string* str, char drop)
{
    str->erase(0, str->find_first_not_of(drop));
    return *str;
}

tlx::string_view& trim_left(tlx::string_view* str)
{
    return trim_left(str, " \r\n\t");
}

tlx::string_view& trim_left(tlx::string_view* str, tlx::string_view drop)
{
    tlx::string_view::size_type pos =
        str->find_first_not_of(drop.data(), 0, drop.size());
    if (pos != tlx::string_view::npos)
        str->remove_prefix(pos);
    else
        str->clear();

    return *str;
}

tlx::string_view& trim_left(tlx::string_view* str, char drop)
{
    tlx::string_view::size_type pos = str->find_first_not_of(drop);
    if (pos != tlx::string_view::npos)
        str->remove_prefix(pos);
    else
        str->clear();

    return *str;
}

tlx::string_view trim_left(tlx::string_view str)
{
    return trim_left(str, " \r\n\t");
}

tlx::string_view trim_left(tlx::string_view str, tlx::string_view drop)
{
    tlx::string_view::size_type pos =
        str.find_first_not_of(drop.data(), 0, drop.size());
    if (pos == tlx::string_view::npos)
        return tlx::string_view();

    return str.substr(pos, std::string::npos);
}

tlx::string_view trim_left(tlx::string_view str, char drop)
{
    tlx::string_view::size_type pos = str.find_first_not_of(drop);
    if (pos == tlx::string_view::npos)
        return tlx::string_view();

    return str.substr(pos, std::string::npos);
}

} // namespace tlx

/******************************************************************************/
