/*******************************************************************************
 * tlx/unused.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_UNUSED_HEADER
#define TLX_UNUSED_HEADER

namespace tlx {

/******************************************************************************/
// UNUSED(variable)

template <typename T>
void unused(T&&) { }

} // namespace tlx

#endif // !TLX_UNUSED_HEADER

/******************************************************************************/
