/*******************************************************************************
 * tlx/math/round_to_power_of_two.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/round_to_power_of_two.hpp>

#include <cstddef>

namespace tlx {

template <typename Integral>
static inline Integral round_up_to_power_of_two_template(Integral n) {
    --n;
    for (size_t k = 1; k != 8 * sizeof(n); k <<= 1) {
        n |= n >> k;
    }
    ++n;
    return n;
}

/******************************************************************************/

int round_up_to_power_of_two(int i) {
    return round_up_to_power_of_two_template(i);
}

unsigned int round_up_to_power_of_two(unsigned int i) {
    return round_up_to_power_of_two_template(i);
}

long round_up_to_power_of_two(long i) {
    return round_up_to_power_of_two_template(i);
}

unsigned long round_up_to_power_of_two(unsigned long i) {
    return round_up_to_power_of_two_template(i);
}

long long round_up_to_power_of_two(long long i) {
    return round_up_to_power_of_two_template(i);
}

unsigned long long round_up_to_power_of_two(unsigned long long i) {
    return round_up_to_power_of_two_template(i);
}

/******************************************************************************/

int round_down_to_power_of_two(int i) {
    return round_up_to_power_of_two(i + 1) >> 1;
}

unsigned int round_down_to_power_of_two(unsigned int i) {
    return round_up_to_power_of_two(i + 1) >> 1;
}

long round_down_to_power_of_two(long i) {
    return round_up_to_power_of_two(i + 1) >> 1;
}

unsigned long round_down_to_power_of_two(unsigned long i) {
    return round_up_to_power_of_two(i + 1) >> 1;
}

long long round_down_to_power_of_two(long long i) {
    return round_up_to_power_of_two(i + 1) >> 1;
}

unsigned long long round_down_to_power_of_two(unsigned long long i) {
    return round_up_to_power_of_two(i + 1) >> 1;
}

} // namespace tlx

/******************************************************************************/
