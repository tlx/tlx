/*******************************************************************************
 * tlx/math/power_to_the.hpp
 *
 * power_to_the<D>(x) raises x to the D-th power using log(D) unrolled
 * multiplications.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Manuel Penschuck <tlx@manuel.jetzt>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_POWER_TO_THE_HEADER
#define TLX_MATH_POWER_TO_THE_HEADER

#include <tlx/math/div_ceil.hpp>

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
//! power_to_the<D>(x)

//! returns x raised to the power of D using log(D) explicit multiplications.
template <unsigned D, typename T>
static constexpr T power_to_the(T x)
{
    // Compiler optimize two calls to the same recursion into one
    // Tested with GCC 4+, Clang 3+, MSVC 15+
    return D < 1  ? 1 :
           D == 1 ? x :
                    power_to_the<D / 2>(x) * power_to_the<div_ceil(D, 2)>(x);
}

//! \}

} // namespace tlx

#endif // !TLX_MATH_POWER_TO_THE_HEADER

/******************************************************************************/
