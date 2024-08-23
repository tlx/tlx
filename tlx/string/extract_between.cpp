/*******************************************************************************
 * tlx/string/extract_between.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/extract_between.hpp>
#include <string>

namespace tlx {

std::string extract_between(tlx::string_view str, tlx::string_view sep1,
                            tlx::string_view sep2)
{
    std::string::size_type start = str.find(sep1.data(), 0, sep1.size());
    if (start == std::string::npos)
        return std::string();

    start += sep1.size();

    std::string::size_type limit = str.find(sep2.data(), start, sep2.size());

    if (limit == std::string::npos)
        return std::string();

    return std::string(str.data() + start, limit - start);
}

} // namespace tlx

/******************************************************************************/
