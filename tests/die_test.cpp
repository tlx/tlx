/*******************************************************************************
 * tests/die_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/die.hpp>

#include <cmath>
#include <iomanip>

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

    // test die with exception
    tlx::set_die_with_exception(true);

    die_unless_throws(die("hello"), tlx::DieException);

    die_unless_throws(die_unequal_eps6(0.01, -0.01), tlx::DieException);
    die_unequal_eps6(0.0000001, -0.0000001);
    die_unless_throws(die_unequal_eps6(NAN, -0.0001), tlx::DieException);
    die_unless_throws(die_unequal_eps6(-0.0001, NAN), tlx::DieException);
    die_unequal_eps6(NAN, NAN);

    die_unless_throws(die_verbose_unless(false, "bad"), tlx::DieException);
    die_unless_throws(die_verbose_if(true, "bad"), tlx::DieException);
    die_unless_throws(die_verbose_unequal(1, 2, "bad"), tlx::DieException);
    die_unless_throws(die_verbose_unequal_eps6(NAN, -0.0001, "bad"),
                      tlx::DieException);
    die_unless_throws(die_verbose_equal(2, 2, "bad"), tlx::DieException);

    return 0;
}

/******************************************************************************/
