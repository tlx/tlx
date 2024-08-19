/*******************************************************************************
 * tlx/logger/array.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_LOGGER_ARRAY_HEADER
#define TLX_LOGGER_ARRAY_HEADER

#include <tlx/logger/core.hpp>
#include <array>
#include <cstddef>
#include <ostream>

namespace tlx {

template <typename T, size_t N>
class LoggerFormatter<std::array<T, N> >
{
public:
    static void print(std::ostream& os, const std::array<T, N>& data)
    {
        os << '[';
        for (typename std::array<T, N>::const_iterator it = data.begin();
             it != data.end(); ++it)
        {
            if (it != data.begin())
                os << ',';
            LoggerFormatter<T>::print(os, *it);
        }
        os << ']';
    }
};

} // namespace tlx

#endif // !TLX_LOGGER_ARRAY_HEADER

/******************************************************************************/
