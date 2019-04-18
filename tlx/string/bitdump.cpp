/*******************************************************************************
 * tlx/string/bitdump.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/string/bitdump.hpp>

namespace tlx {

/******************************************************************************/
// Bitdump 8-bit Little-Endian Methods

std::string bitdump_le8(const void* const data, size_t size) {
    const unsigned char* const cdata =
        static_cast<const unsigned char*>(data);

    std::string out;
    out.resize(size * 9 - 1);

    std::string::iterator oi = out.begin();
    for (const unsigned char* si = cdata; si != cdata + size; ++si) {
        *(oi + 0) = '0' + ((*si >> 7) & 1);
        *(oi + 1) = '0' + ((*si >> 6) & 1);
        *(oi + 2) = '0' + ((*si >> 5) & 1);
        *(oi + 3) = '0' + ((*si >> 4) & 1);
        *(oi + 4) = '0' + ((*si >> 3) & 1);
        *(oi + 5) = '0' + ((*si >> 2) & 1);
        *(oi + 6) = '0' + ((*si >> 1) & 1);
        *(oi + 7) = '0' + ((*si >> 0) & 1);
        oi += 8;
        if (si + 1 != cdata + size) {
            *oi++ = ' ';
        }
    }

    return out;
}

std::string bitdump_le8(const std::string& str) {
    return bitdump_le8(str.data(), str.size());
}

/******************************************************************************/
// Bitdump 8-bit Big-Endian Methods

std::string bitdump_be8(const void* const data, size_t size) {
    const unsigned char* const cdata =
        static_cast<const unsigned char*>(data);

    std::string out;
    out.resize(size * 9 - 1);

    std::string::iterator oi = out.begin();
    for (const unsigned char* si = cdata; si != cdata + size; ++si) {
        *(oi + 0) = '0' + ((*si >> 0) & 1);
        *(oi + 1) = '0' + ((*si >> 1) & 1);
        *(oi + 2) = '0' + ((*si >> 2) & 1);
        *(oi + 3) = '0' + ((*si >> 3) & 1);
        *(oi + 4) = '0' + ((*si >> 4) & 1);
        *(oi + 5) = '0' + ((*si >> 5) & 1);
        *(oi + 6) = '0' + ((*si >> 6) & 1);
        *(oi + 7) = '0' + ((*si >> 7) & 1);
        oi += 8;
        if (si + 1 != cdata + size) {
            *oi++ = ' ';
        }
    }

    return out;
}

std::string bitdump_be8(const std::string& str) {
    return bitdump_be8(str.data(), str.size());
}

/******************************************************************************/

} // namespace tlx

/******************************************************************************/
