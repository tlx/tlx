/*******************************************************************************
 * tlx/math.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_HEADER
#define TLX_MATH_HEADER

//! \defgroup tlx_math Math Functions
//! Simple math functions

/*[[[perl
foreach (sort glob("tlx/math/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n"
}
]]]*/
#include <tlx/math/abs_diff.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/math/aggregate.hpp>             // NOLINT(misc-include-cleaner)
#include <tlx/math/aggregate_min_max.hpp>     // NOLINT(misc-include-cleaner)
#include <tlx/math/bswap.hpp>                 // NOLINT(misc-include-cleaner)
#include <tlx/math/bswap_be.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/math/bswap_le.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/math/clz.hpp>                   // NOLINT(misc-include-cleaner)
#include <tlx/math/ctz.hpp>                   // NOLINT(misc-include-cleaner)
#include <tlx/math/div_ceil.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/math/ffs.hpp>                   // NOLINT(misc-include-cleaner)
#include <tlx/math/integer_log2.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/math/is_power_of_two.hpp>       // NOLINT(misc-include-cleaner)
#include <tlx/math/polynomial_regression.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/math/popcount.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/math/power_to_the.hpp>          // NOLINT(misc-include-cleaner)
#include <tlx/math/rol.hpp>                   // NOLINT(misc-include-cleaner)
#include <tlx/math/ror.hpp>                   // NOLINT(misc-include-cleaner)
#include <tlx/math/round_to_power_of_two.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/math/round_up.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/math/sgn.hpp>                   // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_MATH_HEADER

/******************************************************************************/
