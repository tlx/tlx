/*******************************************************************************
 * tlx/math/clz.hpp
 *
 * clz() count leading zeros - mainly for portability.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_CLZ_HEADER
#define TLX_MATH_CLZ_HEADER

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// clz() - count leading zeros

//! clz (count leading zeros) - generic implementation
template <typename Integral>
static inline unsigned clz_template(Integral x)
{
    if (x == 0)
        return 8 * sizeof(x);
    unsigned r = 0;
    while ((x & (static_cast<Integral>(1) << (8 * sizeof(x) - 1))) == 0)
        x <<= 1, ++r;
    return r;
}

/******************************************************************************/

template <typename Integral>
inline unsigned clz(Integral x);

#if defined(__GNUC__) || defined(__clang__)

//! clz (count leading zeros)
template <>
inline unsigned clz<unsigned>(unsigned x)
{
    if (x == 0)
        return 8 * sizeof(x);
    return static_cast<unsigned>(__builtin_clz(x));
}

//! clz (count leading zeros)
template <>
inline unsigned clz<int>(int x)
{
    return clz(static_cast<unsigned>(x));
}

//! clz (count leading zeros)
template <>
inline unsigned clz<unsigned long>(unsigned long x)
{
    if (x == 0)
        return 8 * sizeof(x);
    return static_cast<unsigned>(__builtin_clzl(x));
}

//! clz (count leading zeros)
template <>
inline unsigned clz<long>(long x)
{
    return clz(static_cast<unsigned long>(x));
}

//! clz (count leading zeros)
template <>
inline unsigned clz<unsigned long long>(unsigned long long x)
{
    if (x == 0)
        return 8 * sizeof(x);
    return static_cast<unsigned>(__builtin_clzll(x));
}

//! clz (count leading zeros)
template <>
inline unsigned clz<long long>(long long x)
{
    return clz(static_cast<unsigned long long>(x));
}

#elif defined(_MSC_VER)

//! clz (count leading zeros)
template <typename Integral>
inline unsigned clz<unsigned>(Integral x)
{
    unsigned long leading_zeros = 0;
    if (sizeof(x) > 4)
    {
#if defined(_WIN64)
        if (_BitScanReverse64(&leading_zeros, x))
            return 63 - leading_zeros;
        else
            return 8 * sizeof(x);
#else
        return clz_template(x);
#endif
    }
    else
    {
        if (_BitScanReverse(&leading_zeros, static_cast<unsigned>(x)))
            return 31 - leading_zeros;
        else
            return 8 * sizeof(x);
    }
}

#else

//! clz (count leading zeros)
template <>
inline unsigned clz<int>(int x)
{
    return clz_template(x);
}

//! clz (count leading zeros)
template <>
inline unsigned clz<unsigned>(unsigned x)
{
    return clz_template(x);
}

//! clz (count leading zeros)
template <>
inline unsigned clz<long>(long x)
{
    return clz_template(x);
}

//! clz (count leading zeros)
template <>
inline unsigned clz<unsigned long>(unsigned long x)
{
    return clz_template(x);
}

//! clz (count leading zeros)
template <>
inline unsigned clz<long long>(long long x)
{
    return clz_template(x);
}

//! clz (count leading zeros)
template <>
inline unsigned clz<unsigned long long>(unsigned long long x)
{
    return clz_template(x);
}

#endif

//! \}

} // namespace tlx

#endif // !TLX_MATH_CLZ_HEADER

/******************************************************************************/
