/*******************************************************************************
 * tlx/math/is_power_of_two.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2007-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/is_power_of_two.hpp>

namespace tlx {

template <typename Integral>
static inline bool is_power_of_two_template(Integral i) {
    if (i <= 0) return false;
    return !(i & (i - 1));
}

/******************************************************************************/

bool is_power_of_two(int i) {
    return is_power_of_two_template(i);
}

bool is_power_of_two(unsigned int i) {
    return is_power_of_two_template(i);
}

bool is_power_of_two(long i) {
    return is_power_of_two_template(i);
}

bool is_power_of_two(unsigned long i) {
    return is_power_of_two_template(i);
}

bool is_power_of_two(long long i) {
    return is_power_of_two_template(i);
}

bool is_power_of_two(unsigned long long i) {
    return is_power_of_two_template(i);
}

} // namespace tlx

/******************************************************************************/
