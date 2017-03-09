/*******************************************************************************
 * tests/meta_has_member_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <cmath>
#include <cstddef>

#include <tlx/die.hpp>
#include <tlx/meta/has_member.hpp>

/******************************************************************************/
// has_member test

class ClassA
{
public:
    int attr1;

    void func123() { }

    template <typename Type>
    void tfunc123(const Type&) { }
};

TLX_MAKE_MEMBER_TEST(attr1)
TLX_MAKE_MEMBER_TEST(attr2)

static_assert(has_member_attr1<ClassA>::value == true,
              "has_member test failed.");
static_assert(has_member_attr2<ClassA>::value == false,
              "has_member test failed.");

TLX_MAKE_MEMBER_TEST(func123)
TLX_MAKE_MEMBER_TEST(func456)

static_assert(has_member_func123<ClassA>::value == true,
              "has_member test failed.");
static_assert(has_member_func456<ClassA>::value == false,
              "has_member test failed.");

TLX_MAKE_TEMPLATE_MEMBER_TEST(tfunc123)
TLX_MAKE_TEMPLATE_MEMBER_TEST(tfunc456)

static_assert(has_member_tfunc123<ClassA, int>::value == true,
              "has_member test failed.");
static_assert(has_member_tfunc456<ClassA, int>::value == false,
              "has_member test failed.");

/******************************************************************************/

int main() {
    return 0;
}

/******************************************************************************/
