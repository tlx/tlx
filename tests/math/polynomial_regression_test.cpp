/*******************************************************************************
 * tests/math/polynomial_regression_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/math/polynomial_regression.hpp>

#include <iomanip>
#include <iostream>

#include <tlx/die.hpp>
#include <tlx/logger.hpp>

void test1() {
    tlx::PolynomialRegression<double, /* WithStore */ true> pr(2);

    pr.add(0, 0);
    pr.add(1, 1);
    pr.add(2, 0);

    die_unequal_eps6(pr.coefficients()[0], 0.0);
    die_unequal_eps6(pr.coefficients()[1], 2.0);
    die_unequal_eps6(pr.coefficients()[2], -1.0);
    die_unequal_eps6(pr.r_square(), 1.0);
}

void test2() {
    tlx::PolynomialRegression<double, /* WithStore */ true> pr(2);

    pr.add(0, 1);
    pr.add(1, 2);
    pr.add(2, 2);
    pr.add(3, 1);

    die_unequal_eps6(pr.coefficients()[0], 1.0);
    die_unequal_eps6(pr.coefficients()[1], 1.5);
    die_unequal_eps6(pr.coefficients()[2], -0.5);
    die_unequal_eps6(pr.r_square(), 1.0);
}

void test3() {
    tlx::PolynomialRegression<double, /* WithStore */ true> pr(2);

    pr.add(0, 1);
    pr.add(1, 2);
    pr.add(2, 4);
    pr.add(3, 1);

    die_unequal_eps6(pr.coefficients()[0], 0.7);
    die_unequal_eps6(pr.coefficients()[1], 3.2);
    die_unequal_eps6(pr.coefficients()[2], -1.0);
    die_unequal_eps6(pr.r_square(), 0.7);
}

void test4() {
    tlx::PolynomialRegression<double, /* WithStore */ false> pr(2);

    pr.add(0, 1);
    pr.add(1, 2);
    pr.add(2, 4);
    pr.add(3, 1);

    die_unequal_eps6(pr.coefficients()[0], 0.7);
    die_unequal_eps6(pr.coefficients()[1], 3.2);
    die_unequal_eps6(pr.coefficients()[2], -1.0);
    die_unequal_eps6(pr.r_square(), NAN);
}

void test5() {
    tlx::PolynomialRegression<double, /* WithStore */ true> pr(2);

    pr.add(0, 1);
    pr.add(1, 2);
    pr.add(2, 4);
    pr.add(3, 1);
    pr.add(4, 8);
    pr.add(5, 2);
    pr.add(6, 1);
    pr.add(7, -3);
    pr.add(8, 6);

    die_unequal_eps6(pr.coefficients()[0], 1.818181);
    die_unequal_eps6(pr.coefficients()[1], 0.536797);
    die_unequal_eps6(pr.coefficients()[2], -0.0670996);
    die_unequal_eps6(pr.r_square(), 0.0168656);
}

template <typename PolynomialRegression>
void dump(PolynomialRegression& pr, double xmin, double xmax) {
    LOG1 << "# " << std::vector<double>(pr.coefficients())
         << ", r^2 " << pr.r_square();

    for (double x = xmin; x <= xmax; x += 0.1) {
        std::cout << x << '\t' << pr.evaluate(x) << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < pr.size(); ++i) {
        std::cout << pr.point(i).x << '\t' << pr.point(i).y << std::endl;
    }
}

void test6() {
    tlx::PolynomialRegression<double, /* WithStore */ false> pr0(0);

    pr0.add(0, 1);
    pr0.add(1, 2);
    pr0.add(2, 4);
    pr0.add(3, 1);
    die_unequal_eps6(pr0.coefficients()[0], 2.0);

    tlx::PolynomialRegression<double, /* WithStore */ false> pr1(1);

    pr1.add(0, 1);
    pr1.add(1, 2);
    pr1.add(2, 4);
    pr1.add(3, 1);
    die_unequal_eps6(pr1.coefficients()[0], 1.7);
    die_unequal_eps6(pr1.coefficients()[1], 0.2);

    tlx::PolynomialRegression<double, /* WithStore */ false> pr2(2);

    pr2.add(0, 1);
    pr2.add(1, 2);
    pr2.add(2, 4);
    pr2.add(3, 1);
    die_unequal_eps6(pr2.coefficients()[0], 0.7);
    die_unequal_eps6(pr2.coefficients()[1], 3.2);
    die_unequal_eps6(pr2.coefficients()[2], -1.0);

    tlx::PolynomialRegression<double, /* WithStore */ false> pr3(3);

    pr3.add(0, 1);
    pr3.add(1, 2);
    pr3.add(2, 4);
    pr3.add(3, 1);

    die_unequal_eps6(pr3.evaluate(0), 1.0);
    die_unequal_eps6(pr3.evaluate(1), 2.0);
    die_unequal_eps6(pr3.evaluate(2), 4.0);
    die_unequal_eps6(pr3.evaluate(3), 1.0);

    die_unequal_eps6(pr3.coefficients()[0], 1.0);
    die_unequal_eps6(pr3.coefficients()[1], -1.5);
    die_unequal_eps6(pr3.coefficients()[2], 3.5);
    die_unequal_eps6(pr3.coefficients()[3], -1.0);

    // underspecified polynomial
    tlx::PolynomialRegression<double, /* WithStore */ true> pr3u(3);

    pr3u.add(0, 1);
    pr3u.add(1, 2);
    pr3u.add(2, 4);

    die_unequal_eps6(pr3u.evaluate(0), 1.0);
    die_unequal_eps6(pr3u.evaluate(1), 2.0);
    die_unequal_eps6(pr3u.evaluate(2), 4.0);
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}

/******************************************************************************/
