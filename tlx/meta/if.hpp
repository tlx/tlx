/*******************************************************************************
 * tlx/meta/if.hpp
 *
 * Template Metaprogramming Tools (from the Generative Programming book Krysztof
 * Czarnecki, Ulrich Eisenecker)
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2003 Roman Dementiev <dementiev@mpi-sb.mpg.de>
 * Copyright (C) 2008 Andreas Beckmann <beckmann@cs.uni-frankfurt.de>
 * Copyright (C) 2010-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_META_IF_HEADER
#define TLX_META_IF_HEADER

namespace tlx {

/******************************************************************************/
// If<true/false, TypeTrue, TypeFalse>::type

/*!
 * If template metaprogramming statement.
 *
 * If \c Flag is \c true then \c If<>::type is of type TypeTrue, otherwise of \c
 * If<>::type is of type TypeFalse
 */
template <bool Flag, typename TypeTrue, typename TypeFalse>
struct If {
    using type = TypeTrue;
};

template <typename TypeTrue, typename TypeFalse>
struct If<false, TypeTrue, TypeFalse>{
    using type = TypeFalse;
};

} // namespace tlx

#endif // !TLX_META_IF_HEADER

/******************************************************************************/
