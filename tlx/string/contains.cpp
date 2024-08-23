/*******************************************************************************
 * tlx/string/contains.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/contains.hpp>
#include <string>

namespace tlx {

bool contains(tlx::string_view str, tlx::string_view pattern)
{
    return str.find(pattern) != std::string::npos;
}

bool contains(tlx::string_view str, const char ch)
{
    return str.find(ch) != std::string::npos;
}

} // namespace tlx

/******************************************************************************/
