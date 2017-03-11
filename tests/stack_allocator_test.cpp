/*******************************************************************************
 * tests/stack_allocator_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <deque>
#include <string>
#include <vector>

#include <tlx/die.hpp>
#include <tlx/stack_allocator.hpp>

namespace tlx {

// forced instantiations
template class StackArena<128>;
template class StackAllocator<int, 128>;

} // namespace tlx

static void test_string_vector_deque() {

    using CharAlloc = tlx::StackAllocator<char, 128>;
    using IntAlloc = tlx::StackAllocator<int, 128>;

    using s_string = std::basic_string<char, std::char_traits<char>, CharAlloc>;

    {
        tlx::StackArena<128> arena;
        const char* text = "abcdefghijklmnopqrstuvwxyz";
        {
            s_string str(text, CharAlloc(arena));
            die_unless(arena.used() >= 27u);

            str = s_string("abc", CharAlloc(arena));
            die_unequal("abc", str);
        }
    }
    {
        tlx::StackArena<128> arena;
        std::vector<int, IntAlloc> my_vector(0, int(), IntAlloc(arena));

        // push more data than in our arena
        for (int i = 0; i < 100; ++i) {
            my_vector.push_back(i);
        }

        for (int i = 0; i < 100; ++i) {
            die_unequal(i, my_vector[i]);
        }
    }
    {
        tlx::StackArena<128> arena;
        std::deque<int, IntAlloc> my_deque(0, int(), IntAlloc(arena));

        // push more data than in our arena
        for (int i = 0; i < 100; ++i) {
            my_deque.push_back(i);
        }

        for (int i = 0; i < 100; ++i) {
            die_unequal(i, my_deque[i]);
        }
    }
}

int main() {
    test_string_vector_deque();

    return 0;
}

/******************************************************************************/
