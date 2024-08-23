/*******************************************************************************
 * tlx/string/split_view.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/split_view.hpp>
#include <algorithm>
#include <cstring>
#include <vector>

namespace tlx {

/******************************************************************************/
// split_view() returning std::vector<tlx::string_view>

std::vector<tlx::string_view> split_view(char sep, tlx::string_view str,
                                         tlx::string_view::size_type limit)
{
    // call base method with new std::vector
    std::vector<tlx::string_view> out;
    split_view(&out, sep, str, limit);
    return out;
}

std::vector<tlx::string_view> split_view(tlx::string_view sep,
                                         tlx::string_view str,
                                         tlx::string_view::size_type limit)
{
    // call base method with new std::vector
    std::vector<tlx::string_view> out;
    split_view(&out, sep, str, limit);
    return out;
}

/******************************************************************************/
// split_view() returning std::vector<tlx::string_view> with minimum fields

std::vector<tlx::string_view> split_view(char sep, tlx::string_view str,
                                         tlx::string_view::size_type min_fields,
                                         tlx::string_view::size_type limit)
{
    // call base method with new std::vector
    std::vector<tlx::string_view> out;
    split_view(&out, sep, str, min_fields, limit);
    return out;
}

std::vector<tlx::string_view> split_view(tlx::string_view sep,
                                         tlx::string_view str,
                                         tlx::string_view::size_type min_fields,
                                         tlx::string_view::size_type limit)
{
    // call base method with new std::vector
    std::vector<tlx::string_view> out;
    split_view(&out, sep, str, min_fields, limit);
    return out;
}

/******************************************************************************/
// split_view() into std::vector<tlx::string_view>

std::vector<tlx::string_view>& split_view(std::vector<tlx::string_view>* into,
                                          char sep, tlx::string_view str,
                                          tlx::string_view::size_type limit)
{
    into->clear();
    if (limit == 0)
        return *into;

    tlx::string_view::const_iterator it = str.begin(), last = it;

    for (; it != str.end(); ++it)
    {
        if (*it == sep)
        {
            if (into->size() + 1 >= limit)
            {
                into->emplace_back(last, str.end());
                return *into;
            }

            into->emplace_back(last, it);
            last = it + 1;
        }
    }

    into->emplace_back(last, it);

    return *into;
}

std::vector<tlx::string_view>& split_view(std::vector<tlx::string_view>* into,
                                          tlx::string_view sep,
                                          tlx::string_view str,
                                          tlx::string_view::size_type limit)
{
    into->clear();
    if (limit == 0)
        return *into;

    if (sep.empty())
    {
        tlx::string_view::const_iterator it = str.begin();
        while (it != str.end())
        {
            into->emplace_back(it, it + 1);
            ++it;
        }
        return *into;
    }

    tlx::string_view::const_iterator it = str.begin(), last = it;

    for (; it + sep.size() < str.end(); ++it)
    {
        if (std::equal(sep.begin(), sep.begin() + sep.size(), it))
        {
            if (into->size() + 1 >= limit)
            {
                into->emplace_back(last, str.end());
                return *into;
            }

            into->emplace_back(last, it);
            last = it + sep.size();
        }
    }

    into->emplace_back(last, str.end());

    return *into;
}

/******************************************************************************/
// split_view() into std::vector<tlx::string_view> with minimum fields

std::vector<tlx::string_view>& split_view(
    std::vector<tlx::string_view>* into, char sep, tlx::string_view str,
    tlx::string_view::size_type min_fields, tlx::string_view::size_type limit)
{
    // call base method
    split_view(into, sep, str, limit);

    if (into->size() < min_fields)
        into->resize(min_fields);

    return *into;
}

std::vector<tlx::string_view>& split_view(
    std::vector<tlx::string_view>* into, tlx::string_view sep,
    tlx::string_view str, tlx::string_view::size_type min_fields,
    tlx::string_view::size_type limit)
{
    // call base method
    split_view(into, sep, str, limit);

    if (into->size() < min_fields)
        into->resize(min_fields);

    return *into;
}

} // namespace tlx

/******************************************************************************/
