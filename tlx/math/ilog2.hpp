/*******************************************************************************
 * tlx/math/ilog2.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_ILOG2_HEADER
#define TLX_MATH_ILOG2_HEADER

namespace tlx {

/******************************************************************************/
// ilog2_floor()

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned ilog2_floor(int i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned ilog2_floor(unsigned int i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned ilog2_floor(long i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned ilog2_floor(unsigned long i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned ilog2_floor(long long i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned ilog2_floor(unsigned long long i);

/******************************************************************************/
// ilog2_ceil()

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned ilog2_ceil(int i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned ilog2_ceil(unsigned int i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned ilog2_ceil(long i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned ilog2_ceil(unsigned long i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned ilog2_ceil(long long i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned ilog2_ceil(unsigned long long i);

} // namespace tlx

#endif // !TLX_MATH_ILOG2_HEADER

/******************************************************************************/
