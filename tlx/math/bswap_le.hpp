/*******************************************************************************
 * tlx/math/bswap_le.hpp
 *
 * bswap16_le(), bswap32_le() and bswap64_le() to swap bytes to little-endian:
 * no-operations on little-endian systems, bswaps on big-endian systems.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_BSWAP_LE_HEADER
#define TLX_MATH_BSWAP_LE_HEADER

#include <cstdint>
#include <tlx/define/endian.hpp>
#include <tlx/math/bswap.hpp>

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// bswap16_le() - swap 16-bit integers to little-endian

#if TLX_LITTLE_ENDIAN
static inline std::uint16_t bswap16_le(const std::uint16_t& v) {
    return v;
}
#elif TLX_BIG_ENDIAN
static inline std::uint16_t bswap16_le(const std::uint16_t& v) {
    return bswap16(v);
}
#endif

/******************************************************************************/
// bswap32_le() - swap 32-bit integers to little-endian

#if TLX_LITTLE_ENDIAN
static inline std::uint32_t bswap32_le(const std::uint32_t& v) {
    return v;
}
#elif TLX_BIG_ENDIAN
static inline std::uint32_t bswap32_le(const std::uint32_t& v) {
    return bswap32(v);
}
#endif

/******************************************************************************/
// bswap64_le() - swap 64-bit integers to little-endian

#if TLX_LITTLE_ENDIAN
static inline std::uint64_t bswap64_le(const std::uint64_t& v) {
    return v;
}
#elif TLX_BIG_ENDIAN
static inline std::uint64_t bswap64_le(const std::uint64_t& v) {
    return bswap64(v);
}
#endif

/******************************************************************************/

//! \}

} // namespace tlx

#endif // !TLX_MATH_BSWAP_LE_HEADER

/******************************************************************************/
