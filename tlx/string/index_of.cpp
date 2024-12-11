/*******************************************************************************
 * tlx/string/index_of.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2024 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/equal_icase.hpp>
#include <tlx/string/index_of.hpp>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace tlx {

size_t index_of(const std::vector<std::string>& list, tlx::string_view str)
{
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list[i] == str)
            return i;
    }
    std::string reason = "Could not find index_of() ";
    reason.append(str.begin(), str.end());
    throw std::runtime_error(reason);
}

size_t index_of(const std::vector<tlx::string_view>& list, tlx::string_view str)
{
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (list[i] == str)
            return i;
    }
    std::string reason = "Could not find index_of() ";
    reason.append(str.begin(), str.end());
    throw std::runtime_error(reason);
}

size_t index_of_icase(const std::vector<std::string>& list,
                      tlx::string_view str)
{
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (tlx::equal_icase(list[i], str))
            return i;
    }
    std::string reason = "Could not find index_of_icase() ";
    reason.append(str.begin(), str.end());
    throw std::runtime_error(reason);
}

size_t index_of_icase(const std::vector<tlx::string_view>& list,
                      tlx::string_view str)
{
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (tlx::equal_icase(list[i], str))
            return i;
    }
    std::string reason = "Could not find index_of_icase() ";
    reason.append(str.begin(), str.end());
    throw std::runtime_error(reason);
}

} // namespace tlx

/******************************************************************************/
