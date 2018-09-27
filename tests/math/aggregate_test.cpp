/*******************************************************************************
 * tests/math/aggregate_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/aggregate.hpp>

#include <iomanip>

#include <tlx/die.hpp>

void test_integer() {
    tlx::Aggregate<int> agg;

    for (int i = 0; i < 30; ++i) {
        agg.add(i);
    }

    die_unequal(30u, agg.count());
    die_unequal((29 * 30) / 2, agg.total());
    die_unequal_eps6(14.5, agg.average());
    die_unequal(0, agg.min());
    die_unequal(29, agg.max());
    die_unequal_eps6(8.8034084308295046, agg.standard_deviation());
}

void test_double() {
    tlx::Aggregate<double> agg;

    for (size_t i = 1; i <= 1000; ++i) {
        agg.add(1.0 / static_cast<double>(i));
    }

    die_unequal(1000u, agg.count());
    die_unequal_eps6(7.4854708605503451, agg.total());
    die_unequal_eps6(0.0074854708605503447, agg.average());
    die_unequal(0.001, agg.min());
    die_unequal(1.0, agg.max());
    die_unequal_eps6(0.039868430925506362, agg.standard_deviation());
    die_unequal_eps6(0.039848491723996423, agg.standard_deviation(0));
}

int main() {
    test_integer();
    test_double();

    return 0;
}

/******************************************************************************/
