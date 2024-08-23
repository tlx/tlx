/*******************************************************************************
 * tlx/string/pad.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/pad.hpp>
#include <algorithm>
#include <cstddef>
#include <string>

namespace tlx {

std::string pad(tlx::string_view str, size_t len, char pad_char)
{
    std::string str_copy;
    str_copy.reserve(len);
    str_copy.assign(str.data(), std::min(str.size(), len));
    str_copy.resize(len, pad_char);
    return str_copy;
}

} // namespace tlx

/******************************************************************************/
