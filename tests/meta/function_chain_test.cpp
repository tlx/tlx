/*******************************************************************************
 * tests/meta/function_chain_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015 Sebastian Lamm <seba.lamm@gmail.com>
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/meta/function_chain.hpp>

#include <tlx/die.hpp>

#include <iostream>
#include <string>
#include <vector>

static void test_chain_maker() {

    auto chain = tlx::FunctionChain<>()
                 & ([](const int& d) { return d + 1; })
                 & ([](const int& d) { return d * 2; });

    // directly call function chain
    die_unequal(chain(5), (5 + 1) * 2);

    // fold lambda chain
    auto func = chain.fold();
    die_unequal(func(5), (5 + 1) * 2);
}

static void test_empty_chain_add() {

    auto chain = tlx::make_function_chain();
    die_unequal(chain(5), 5);

    auto chain2 = chain & ([](const int& d) { return std::to_string(d); });
    die_unequal(chain2(5), "5");

    auto chain3 = chain2 & ([](const std::string& d) { return d + "2"; });
    die_unequal(chain3(5), "52");
}

int main() {
    test_chain_maker();
    test_empty_chain_add();

    return 0;
}

/******************************************************************************/
