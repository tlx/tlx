/*******************************************************************************
 * tests/container/string_view_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/string/split_view.hpp>

#include <tlx/die.hpp>

void construct_empty() {
    tlx::StringView empty(nullptr);
    die_unequal(empty.size(), 0u);
}

void assign_and_compare() {

    std::string input = "This is a string which does things and is our input.";
    std::string input2 = "is a string1";

    tlx::StringView other_str;
    tlx::StringView fast_str = tlx::StringView(&input[5], 11);  // "is a string"
    die_unequal(fast_str.size(), 11u);

    std::string cmp = "is a string";
    die_unless(fast_str == cmp);

    std::string different = "is another string";
    die_if(fast_str == different);

    std::string subset = "is a strin";
    die_if(fast_str == subset);

    other_str = tlx::StringView(&input[6], 11);             // "s a string "
    die_if(fast_str == other_str);
    die_unless(fast_str != other_str);
    tlx::StringView equal_str = tlx::StringView(&input2[0], 11); // "is a string"
    die_unless(fast_str == equal_str);
    die_if(fast_str != equal_str);
}

void split_view() {
    std::string input = "This is a string.";
    size_t i = 0;
    tlx::split_view(
        ' ', input,
        [&](const tlx::string_view& sv) {
            if (i == 0)
                die_unequal(sv, "This");
            if (i == 1)
                die_unequal(sv, "is");
            if (i == 2)
                die_unequal(sv, "a");
            if (i == 3)
                die_unequal(sv, "string.");
            ++i;
        });
    die_unequal(i, 4u);
}

int main() {
    construct_empty();
    assign_and_compare();
    split_view();

    return 0;
}

/******************************************************************************/
