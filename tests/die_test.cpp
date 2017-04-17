/*******************************************************************************
 * tests/die_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/die.hpp>

#include <cmath>

int main() {

    // test some double comparisons
    die_unless(tlx::die_equal_compare(NAN, NAN));
    die_unless(!tlx::die_equal_compare(NAN, 0));
    die_unless(!tlx::die_equal_compare(0, NAN));

    die_unless(!tlx::die_equal_compare(NAN, INFINITY));
    die_unless(!tlx::die_equal_compare(INFINITY, NAN));
    die_unless(tlx::die_equal_compare(INFINITY, INFINITY));
    die_unless(!tlx::die_equal_compare(INFINITY, -INFINITY));

    die_unless(tlx::die_equal_compare(10, 10));

    return 0;
}

/******************************************************************************/
