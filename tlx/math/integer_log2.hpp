/*******************************************************************************
 * tlx/math/integer_log2.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_INTEGER_LOG2_HEADER
#define TLX_MATH_INTEGER_LOG2_HEADER

namespace tlx {

//! \addtogroup tlx_math
//! \{

/******************************************************************************/
// integer_log2_floor()

//! calculate the log2 floor of an integer type (by repeated bit shifts)
template <typename IntegerType>
static inline unsigned integer_log2_floor_template(IntegerType i) {
    unsigned p = 0;
    while (i >= 65536) i >>= 16, p += 16;
    while (i >= 256) i >>= 8, p += 8;
    while (i >>= 1) ++p;
    return p;
}

/******************************************************************************/
// integer_log2_floor()

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned integer_log2_floor(int i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned integer_log2_floor(unsigned int i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned integer_log2_floor(long i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned integer_log2_floor(unsigned long i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned integer_log2_floor(long long i);

//! calculate the log2 floor of an integer type (by repeated bit shifts)
unsigned integer_log2_floor(unsigned long long i);

/******************************************************************************/
// integer_log2_ceil()

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned integer_log2_ceil(int i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned integer_log2_ceil(unsigned int i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned integer_log2_ceil(long i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned integer_log2_ceil(unsigned long i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned integer_log2_ceil(long long i);

//! calculate the log2 ceiling of an integer type (by repeated bit shifts)
unsigned integer_log2_ceil(unsigned long long i);

//! \}

} // namespace tlx

#endif // !TLX_MATH_INTEGER_LOG2_HEADER

/******************************************************************************/
