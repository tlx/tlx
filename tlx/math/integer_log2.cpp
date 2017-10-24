/*******************************************************************************
 * tlx/math/integer_log2.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/integer_log2.hpp>

namespace tlx {

/******************************************************************************/

unsigned integer_log2_floor(int i) {
    return integer_log2_floor_template(i);
}

unsigned integer_log2_floor(unsigned int i) {
    return integer_log2_floor_template(i);
}

unsigned integer_log2_floor(long i) {
    return integer_log2_floor_template(i);
}

unsigned integer_log2_floor(unsigned long i) {
    return integer_log2_floor_template(i);
}

unsigned integer_log2_floor(long long i) {
    return integer_log2_floor_template(i);
}

unsigned integer_log2_floor(unsigned long long i) {
    return integer_log2_floor_template(i);
}

/******************************************************************************/

unsigned integer_log2_ceil(int i) {
    if (i <= 1) return 0;
    return integer_log2_floor(i - 1) + 1;
}

unsigned integer_log2_ceil(unsigned int i) {
    if (i <= 1) return 0;
    return integer_log2_floor(i - 1) + 1;
}

unsigned integer_log2_ceil(long i) {
    if (i <= 1) return 0;
    return integer_log2_floor(i - 1) + 1;
}

unsigned integer_log2_ceil(unsigned long i) {
    if (i <= 1) return 0;
    return integer_log2_floor(i - 1) + 1;
}

unsigned integer_log2_ceil(long long i) {
    if (i <= 1) return 0;
    return integer_log2_floor(i - 1) + 1;
}

unsigned integer_log2_ceil(unsigned long long i) {
    if (i <= 1) return 0;
    return integer_log2_floor(i - 1) + 1;
}

} // namespace tlx

/******************************************************************************/
