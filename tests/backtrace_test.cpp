/*******************************************************************************
 * tests/backtrace_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2008-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/backtrace.hpp>

#include <map>

namespace Nu {

template <typename Type>
struct Alpha {
    struct Beta {
        void func() {
            tlx::print_cxx_backtrace();
        }
        void func(Type) {
            tlx::print_cxx_backtrace();
            tlx::print_raw_backtrace();
        }
    };
};

struct Gamma {
    template <int N>
    void unroll(double d) {
        unroll<N - 1>(d);
    }
};

template <>
void Gamma::unroll<0>(double) {
    tlx::print_cxx_backtrace();
}

} // namespace Nu

void test1() {
    Nu::Alpha<int>::Beta().func(42);
    Nu::Alpha<const char*>::Beta().func("42");
    Nu::Alpha<Nu::Alpha<std::map<int, double> > >::Beta().func();
    Nu::Gamma().unroll<5>(42.0);
}

int main() {

    test1();

    return 0;
}

/******************************************************************************/
