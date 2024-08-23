/*******************************************************************************
 * tlx/string/split_quoted.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/split_quoted.hpp>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace tlx {

std::vector<std::string> split_quoted(tlx::string_view str, char sep,
                                      char quote, char escape)
{
    std::vector<std::string> out;

    tlx::string_view::const_iterator it = str.begin();
    std::string entry;

    for (; it != str.end();)
    {
        if (*it == sep)
        {
            // skip separator outside of fields
            ++it;
        }
        else if (*it == quote)
        {
            // parse quoted entry
            ++it;

            while (true)
            {
                if (it == str.end())
                    throw std::runtime_error(
                        "unmatched end quote in split_quoted().");

                if (*it == quote)
                {
                    ++it;
                    if (it == str.end())
                    {
                        // last quote and end-of-line
                        out.emplace_back(std::move(entry));
                        return out;
                    }
                    if (*it == sep)
                    {
                        // quote + sep -> end of this entry
                        out.emplace_back(std::move(entry));
                        ++it;
                        break;
                    }

                    throw std::runtime_error(
                        std::string(
                            "extra quote enclosed in entry, followed by ") +
                        *it);
                }
                if (*it == escape)
                {
                    ++it;
                    if (it == str.end())
                        throw std::runtime_error(
                            "escape as last character in string");

                    if (*it == quote)
                    {
                        // escape + quote -> quote
                        entry += *it++;
                    }
                    else if (*it == escape)
                    {
                        // escape + escape -> escape
                        entry += *it++;
                    }
                    else if (*it == 'n')
                    {
                        // escape + n -> new line
                        entry += '\n', ++it;
                    }
                    else if (*it == 'r')
                    {
                        // escape + r -> carriage return
                        entry += '\r', ++it;
                    }
                    else if (*it == 't')
                    {
                        // escape + t -> tab
                        entry += '\t', ++it;
                    }
                    else
                    {
                        throw std::runtime_error(
                            std::string("escape followed by "
                                        "unknown character") +
                            *it);
                    }
                }
                else
                {
                    // normal character
                    entry += *it++;
                }
            }
        }
        else
        {
            // parse unquoted entry
            while (true)
            {
                if (it == str.end())
                {
                    // end-of-line
                    out.emplace_back(std::move(entry));
                    return out;
                }

                if (*it == sep)
                {
                    // sep -> end of this entry
                    out.emplace_back(std::move(entry));
                    ++it;
                    break;
                }

                // normal character
                entry += *it++;
            }
        }
    }

    return out;
}

std::vector<std::string> split_quoted(tlx::string_view str)
{
    return split_quoted(str, ' ', '"', '\\');
}

} // namespace tlx

/******************************************************************************/
