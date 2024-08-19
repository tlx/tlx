/*******************************************************************************
 * tlx/math/bswap_be.hpp
 *
 * bswap16_be(), bswap32_be() and bswap64_be() to swap bytes to big-endian:
 * no-operations on big-endian systems, bswaps on little-endian systems.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_BSWAP_BE_HEADER
#define TLX_MATH_BSWAP_BE_HEADER

#include <tlx/define/endian.hpp>
#include <tlx/math/bswap.hpp>
#include <cstdint>

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// bswap16_be() - swap 16-bit integers to big-endian

#if TLX_LITTLE_ENDIAN
static inline std::uint16_t bswap16_be(const std::uint16_t& v)
{
    return bswap16(v);
}
#elif TLX_BIG_ENDIAN
static inline std::uint16_t bswap16_be(const std::uint16_t& v)
{
    return v;
}
#endif

/******************************************************************************/
// bswap32_be() - swap 32-bit integers to big-endian

#if TLX_LITTLE_ENDIAN
static inline std::uint32_t bswap32_be(const std::uint32_t& v)
{
    return bswap32(v);
}
#elif TLX_BIG_ENDIAN
static inline std::uint32_t bswap32_be(const std::uint32_t& v)
{
    return v;
}
#endif

/******************************************************************************/
// bswap64_be() - swap 64-bit integers to big-endian

#if TLX_LITTLE_ENDIAN
static inline std::uint64_t bswap64_be(const std::uint64_t& v)
{
    return bswap64(v);
}
#elif TLX_BIG_ENDIAN
static inline std::uint64_t bswap64_be(const std::uint64_t& v)
{
    return v;
}
#endif

/******************************************************************************/

//! \}

} // namespace tlx

#endif // !TLX_MATH_BSWAP_BE_HEADER

/******************************************************************************/
