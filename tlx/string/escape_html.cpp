/*******************************************************************************
 * tlx/string/escape_html.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/escape_html.hpp>
#include <cstring>
#include <string>

namespace tlx {

std::string escape_html(tlx::string_view str)
{
    std::string os;
    os.reserve(str.size() + str.size() / 16);

    for (tlx::string_view::const_iterator si = str.begin(); si != str.end();
         ++si)
    {
        if (*si == '&')
            os += "&amp;";
        else if (*si == '<')
            os += "&lt;";
        else if (*si == '>')
            os += "&gt;";
        else if (*si == '"')
            os += "&quot;";
        else
            os += *si;
    }

    return os;
}

} // namespace tlx

/******************************************************************************/
