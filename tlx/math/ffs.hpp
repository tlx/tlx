/*******************************************************************************
 * tlx/math/ffs.hpp
 *
 * ffs() find first set bit in integer - mainly for portability as ffs() is a
 * glibc extension and not available on Visual Studio.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_MATH_FFS_HEADER
#define TLX_MATH_FFS_HEADER

namespace tlx {

/******************************************************************************/
// ffs() - find first set bit in integer

//! find first set bit in integer, or zero if none are set.
unsigned ffs(int i);

//! find first set bit in integer, or zero if none are set.
unsigned ffs(unsigned int i);

//! find first set bit in integer, or zero if none are set.
unsigned ffs(long i);

//! find first set bit in integer, or zero if none are set.
unsigned ffs(unsigned long i);

//! find first set bit in integer, or zero if none are set.
unsigned ffs(long long i);

//! find first set bit in integer, or zero if none are set.
unsigned ffs(unsigned long long i);

} // namespace tlx

#endif // !TLX_MATH_FFS_HEADER

/******************************************************************************/
