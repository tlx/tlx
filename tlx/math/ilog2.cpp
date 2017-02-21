/*******************************************************************************
 * tlx/math/ilog2.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/ilog2.hpp>

namespace tlx {

template <typename IntegerType>
static inline unsigned ilog2_floor_template(IntegerType i) {
    unsigned p = 0;
    while (i >= 65536) i >>= 16, p += 16;
    while (i >= 256) i >>= 8, p += 8;
    while (i >>= 1) ++p;
    return p;
}

/******************************************************************************/

unsigned ilog2_floor(int i) {
    return ilog2_floor_template(i);
}

unsigned ilog2_floor(unsigned int i) {
    return ilog2_floor_template(i);
}

unsigned ilog2_floor(long i) {
    return ilog2_floor_template(i);
}

unsigned ilog2_floor(unsigned long i) {
    return ilog2_floor_template(i);
}

unsigned ilog2_floor(long long i) {
    return ilog2_floor_template(i);
}

unsigned ilog2_floor(unsigned long long i) {
    return ilog2_floor_template(i);
}

/******************************************************************************/

unsigned ilog2_ceil(int i) {
    if (i <= 1) return 0;
    return ilog2_floor(i - 1) + 1;
}

unsigned ilog2_ceil(unsigned int i) {
    if (i <= 1) return 0;
    return ilog2_floor(i - 1) + 1;
}

unsigned ilog2_ceil(long i) {
    if (i <= 1) return 0;
    return ilog2_floor(i - 1) + 1;
}

unsigned ilog2_ceil(unsigned long i) {
    if (i <= 1) return 0;
    return ilog2_floor(i - 1) + 1;
}

unsigned ilog2_ceil(long long i) {
    if (i <= 1) return 0;
    return ilog2_floor(i - 1) + 1;
}

unsigned ilog2_ceil(unsigned long long i) {
    if (i <= 1) return 0;
    return ilog2_floor(i - 1) + 1;
}

} // namespace tlx

/******************************************************************************/
