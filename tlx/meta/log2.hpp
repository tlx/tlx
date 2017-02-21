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

#include <cstddef>

namespace tlx {

/******************************************************************************/
// Log2Floor<Value>::value

template <size_t Input>
class Log2Floor
{
public:
    enum {
        value = Log2Floor<Input / 2>::value + 1
    };
};

/*[[[perl
  for (my $i = 1, my $j = 0; $j <= 60; $i <<= 4, $j += 4) {
   print <<"EOF";
template <>
class Log2Floor<${i}ull>
{
public:
    enum { value = $j };
};

EOF
  }
]]]*/
template <>
class Log2Floor<1ull>
{
public:
    enum { value = 0 };
};

template <>
class Log2Floor<16ull>
{
public:
    enum { value = 4 };
};

template <>
class Log2Floor<256ull>
{
public:
    enum { value = 8 };
};

template <>
class Log2Floor<4096ull>
{
public:
    enum { value = 12 };
};

template <>
class Log2Floor<65536ull>
{
public:
    enum { value = 16 };
};

template <>
class Log2Floor<1048576ull>
{
public:
    enum { value = 20 };
};

template <>
class Log2Floor<16777216ull>
{
public:
    enum { value = 24 };
};

template <>
class Log2Floor<268435456ull>
{
public:
    enum { value = 28 };
};

template <>
class Log2Floor<4294967296ull>
{
public:
    enum { value = 32 };
};

template <>
class Log2Floor<68719476736ull>
{
public:
    enum { value = 36 };
};

template <>
class Log2Floor<1099511627776ull>
{
public:
    enum { value = 40 };
};

template <>
class Log2Floor<17592186044416ull>
{
public:
    enum { value = 44 };
};

template <>
class Log2Floor<281474976710656ull>
{
public:
    enum { value = 48 };
};

template <>
class Log2Floor<4503599627370496ull>
{
public:
    enum { value = 52 };
};

template <>
class Log2Floor<72057594037927936ull>
{
public:
    enum { value = 56 };
};

template <>
class Log2Floor<1152921504606846976ull>
{
public:
    enum { value = 60 };
};
// [[[end]]]

template <>
class Log2Floor<0>
{
public:
    enum { value = 0 };
};

/******************************************************************************/
// Log2<Value>::floor and Log2<Value>::ceil

template <size_t Input>
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

} // namespace tlx

#endif // !TLX_META_LOG2_HEADER

/******************************************************************************/
