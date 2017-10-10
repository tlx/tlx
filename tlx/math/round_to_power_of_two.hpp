/*******************************************************************************
 * tlx/math/round_to_power_of_two.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_ROUND_TO_POWER_OF_TWO_HEADER
#define TLX_MATH_ROUND_TO_POWER_OF_TWO_HEADER

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// round_up_to_power_of_two()

//! does what it says: round up to next power of two
int round_up_to_power_of_two(int i);

//! does what it says: round up to next power of two
unsigned int round_up_to_power_of_two(unsigned int i);

//! does what it says: round up to next power of two
long round_up_to_power_of_two(long i);

//! does what it says: round up to next power of two
unsigned long round_up_to_power_of_two(unsigned long i);

//! does what it says: round up to next power of two
long long round_up_to_power_of_two(long long i);

//! does what it says: round up to next power of two
unsigned long long round_up_to_power_of_two(unsigned long long i);

/******************************************************************************/
// round_down_to_power_of_two()

//! does what it says: round down to next power of two
int round_down_to_power_of_two(int i);

//! does what it says: round down to next power of two
unsigned int round_down_to_power_of_two(unsigned int i);

//! does what it says: round down to next power of two
long round_down_to_power_of_two(long i);

//! does what it says: round down to next power of two
unsigned long round_down_to_power_of_two(unsigned long i);

//! does what it says: round down to next power of two
long long round_down_to_power_of_two(long long i);

//! does what it says: round down to next power of two
unsigned long long round_down_to_power_of_two(unsigned long long i);

//! \}

} // namespace tlx

#endif // !TLX_MATH_ROUND_TO_POWER_OF_TWO_HEADER

/******************************************************************************/
