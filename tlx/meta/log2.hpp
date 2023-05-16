/*******************************************************************************
 * tlx/meta/log2.hpp
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

#ifndef TLX_META_LOG2_HEADER
#define TLX_META_LOG2_HEADER

#include <cstdint>

namespace tlx {

//! \addtogroup tlx_meta
//! \{

/******************************************************************************/
// Log2Floor<Value>::value

template <std::uint64_t Input>
class Log2Floor
{
public:
    enum {
        value = Log2Floor<Input / 2>::value + 1
    };
};

template <>
class Log2Floor<1>
{
public:
    enum { value = 0 };
};

template <>
class Log2Floor<0>
{
public:
    enum { value = 0 };
};

/******************************************************************************/
// Log2<Value>::floor and Log2<Value>::ceil

template <std::uint64_t Input>
class Log2
{
public:
    enum {
        floor = Log2Floor<Input>::value,
        ceil = Log2Floor<Input - 1>::value + 1
    };
};

template <>
class Log2<1>
{
public:
    enum {
        floor = 0,
        ceil = 0
    };
};

template <>
class Log2<0>
{
public:
    enum {
        floor = 0,
        ceil = 0
    };
};

//! \}

} // namespace tlx

#endif // !TLX_META_LOG2_HEADER

/******************************************************************************/
