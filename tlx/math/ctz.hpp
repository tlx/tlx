/*******************************************************************************
 * tlx/math/ctz.hpp
 *
 * ctz() count trailing zeros - mainly for portability.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_CTZ_HEADER
#define TLX_MATH_CTZ_HEADER

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// ctz() - count trailing zeros

//! ctz (count trailing zeros) - generic implementation
template <typename Integral>
static inline unsigned ctz_template(Integral x)
{
    if (x == 0)
        return 8 * sizeof(x);
    unsigned r = 0;
    while ((x & static_cast<Integral>(1)) == 0)
        x >>= 1, ++r;
    return r;
}

/******************************************************************************/

template <typename Integral>
inline unsigned ctz(Integral x);

#if defined(__GNUC__) || defined(__clang__)

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<unsigned>(unsigned x)
{
    if (x == 0)
        return 8 * sizeof(x);
    return static_cast<unsigned>(__builtin_ctz(x));
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<int>(int x)
{
    return ctz(static_cast<unsigned>(x));
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<unsigned long>(unsigned long x)
{
    if (x == 0)
        return 8 * sizeof(x);
    return static_cast<unsigned>(__builtin_ctzl(x));
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<long>(long x)
{
    return ctz(static_cast<unsigned long>(x));
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<unsigned long long>(unsigned long long x)
{
    if (x == 0)
        return 8 * sizeof(x);
    return static_cast<unsigned>(__builtin_ctzll(x));
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<long long>(long long x)
{
    return ctz(static_cast<unsigned long long>(x));
}

#elif defined(_MSC_VER)

//! ctz (count trailing zeros)
template <typename Integral>
inline unsigned ctz<unsigned>(Integral x)
{
    unsigned long trailing_zeros = 0;
    if (sizeof(x) > 4)
    {
#if defined(_WIN64)
        if (_BitScanForward64(&trailing_zeros, x))
            return trailing_zeros;
        else
            return 8 * sizeof(x);
#else
        return ctz_template(x);
#endif
    }
    else
    {
        if (_BitScanForward(&trailing_zeros, static_cast<unsigned>(x)))
            return trailing_zeros;
        else
            return 8 * sizeof(x);
    }
}

#else

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<int>(int x)
{
    return ctz_template(x);
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<unsigned>(unsigned x)
{
    return ctz_template(x);
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<long>(long x)
{
    return ctz_template(x);
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<unsigned long>(unsigned long x)
{
    return ctz_template(x);
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<long long>(long long x)
{
    return ctz_template(x);
}

//! ctz (count trailing zeros)
template <>
inline unsigned ctz<unsigned long long>(unsigned long long x)
{
    return ctz_template(x);
}

#endif

//! \}

} // namespace tlx

#endif // !TLX_MATH_CTZ_HEADER

/******************************************************************************/
