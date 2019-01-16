/*******************************************************************************
 * tests/meta/function_stack_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015 Sebastian Lamm <seba.lamm@gmail.com>
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/meta/function_stack.hpp>

#include <tlx/die.hpp>

#include <iostream>
#include <string>
#include <vector>

static void test_stack1() {

    std::vector<double> elements;

    // User-defined functions
    auto fmap_fn =
        [=](double input, auto emit_func) {
            emit_func(input);
            emit_func(input);
        };

    auto map_fn =
        [=](double input) {
            return 2 * input;
        };

    auto filter_fn =
        [=](double input) {
            return input > 80;
        };

    double total = 0;

    auto save_fn =
        [&total](double input) {
            // elements.push_back(input);
            total += input;
        };

    // Converted emitter functions
    auto conv_map_fn =
        [=](double input, auto emit_func) {
            emit_func(map_fn(input));
        };

    auto conv_filter_fn =
        [=](double input, auto emit_func) {
            if (filter_fn(input)) emit_func(input);
        };

    auto empty_stack = tlx::FunctionStack<double>();
    static_assert(empty_stack.empty, "FunctionStack::empty is wrong");

    auto new_stack = tlx::make_function_stack<double>(fmap_fn);
    static_assert(!new_stack.empty, "FunctionStack::empty is wrong");

    auto new_stack2 = new_stack.push(conv_map_fn);
    auto new_stack3 = new_stack2.push(conv_filter_fn);
    auto new_stack4 = new_stack3.push(save_fn);
    auto composed_function = new_stack4.fold();

    for (size_t i = 0; i != 1000; ++i) {
        composed_function(42);
        composed_function(2);
        composed_function(50);
    }

    die_unequal(total, 368000u);
}

static void test_simple_deduction_test() {

    auto fmap_fn1 =
        [=](int input, auto emit_func) {
            emit_func(std::to_string(input));
        };

    auto fmap_fn2 =
        [=](const std::string& input, auto emit_func) {
            emit_func(input + " Hello");
            emit_func(10);
        };

    auto new_stack1 = tlx::make_function_stack<int>(fmap_fn1);
    auto new_stack2 = new_stack1.push(fmap_fn2);

    std::vector<std::string> output;

    auto save_output = [&](auto) {
                           output.emplace_back("123");
                       };

    auto new_stack3 = new_stack2.push(save_output);
    new_stack3.fold()(42);

    die_unequal(output.size(), 2u);
    die_unequal(output[0], "123");
    die_unequal(output[1], "123");
}

static void test_stack_maker() {

    std::vector<std::string> out;

    auto result =
        tlx::FunctionStack<double>()
        & ([](const int& d, auto emit) { emit(d + 1); })
        & ([](const int& d, auto emit) { emit(std::to_string(2 * d)); })
        & ([](const std::string& s, auto emit) { emit(s + "2"); })
        & ([&](const std::string& s) { out.emplace_back(s); });

    auto x = result.fold();
    x(5);
    die_unequal(out.front(), "122");
}

int main() {
    test_stack1();
    test_simple_deduction_test();
    test_stack_maker();

    return 0;
}

/******************************************************************************/
