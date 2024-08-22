/*******************************************************************************
 * tlx/math/div_ceil.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_DIV_CEIL_HEADER
#define TLX_MATH_DIV_CEIL_HEADER

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// div_ceil() - calculate n div k with rounding up

//! calculate n div k with rounding up, for n and k positive!
template <typename IntegralN, typename IntegralK>
static constexpr auto div_ceil(const IntegralN& n,
                               const IntegralK& k) -> decltype(n + k)
{
    return (n + k - 1) / k;
}

//! \}

} // namespace tlx

#endif // !TLX_MATH_DIV_CEIL_HEADER

/******************************************************************************/
