/*******************************************************************************
 * tlx/math/round_up.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_ROUND_UP_HEADER
#define TLX_MATH_ROUND_UP_HEADER

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// round_up() - round n up to the next multiple of k

//! round n up to the next multiple of k, for n and k positive!
template <typename IntegralN, typename IntegralK>
static constexpr auto round_up(const IntegralN& n,
                               const IntegralK& k) -> decltype(n + k)
{
    return ((n + k - 1) / k) * k;
}

//! \}

} // namespace tlx

#endif // !TLX_MATH_ROUND_UP_HEADER

/******************************************************************************/
