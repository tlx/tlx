/*******************************************************************************
 * tests/meta/has_member_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/meta/has_member.hpp>
#include <tlx/meta/has_method.hpp>
#include <tlx/unused.hpp>
#include <cmath>
#include <string>

/******************************************************************************/
// has_member test

class ClassA
{
public:
    int attr1;

    void func123()
    {
    }

    template <typename Type>
    void tfunc123(const Type& x)
    {
        tlx::unused(x);
    }

    static double func42(const std::string& s)
    {
        tlx::unused(s);
        return 42.0;
    }
};

TLX_MAKE_HAS_MEMBER(attr1);
TLX_MAKE_HAS_MEMBER(attr2);

static_assert(has_member_attr1<ClassA>::value, "has_member test failed.");
static_assert(!has_member_attr2<ClassA>::value, "has_member test failed.");

TLX_MAKE_HAS_MEMBER(func123);
TLX_MAKE_HAS_MEMBER(func456);

static_assert(has_member_func123<ClassA>::value, "has_member test failed.");
static_assert(!has_member_func456<ClassA>::value, "has_member test failed.");

TLX_MAKE_HAS_TEMPLATE_MEMBER(tfunc123);
TLX_MAKE_HAS_TEMPLATE_MEMBER(tfunc456);

static_assert(has_member_tfunc123<ClassA, int>::value,
              "has_member test failed.");
static_assert(!has_member_tfunc456<ClassA, int>::value,
              "has_member test failed.");

/******************************************************************************/
// has_method test

// the following code does not work with gcc 4.8
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 8)) ||                 \
    defined(__clang__)

class ClassC
{
public:
    // NOLINTNEXTLINE(readability-convert-member-functions-to-static)
    void func123(int i)
    {
        tlx::unused(i);
    }

    static double func456(const std::string& s)
    {
        tlx::unused(s);
        return 42.0;
    }

    template <typename Type>
    void tfunc123(int i, const Type& t)
    {
        tlx::unused(i, t);
    }
};

class ClassD
{
public:
    void func123(const std::string&)
    {
    }
};

TLX_MAKE_HAS_METHOD(func123);

static_assert(has_method_func123<ClassC, void(int)>::value,
              "has_method_func123 test failed.");
static_assert(!has_method_func123<ClassC, void(std::string)>::value,
              "has_method_func123 test failed.");

static_assert(!has_method_func123<ClassD, void(int)>::value,
              "has_method_func123 test failed.");
static_assert(!has_method_func123<ClassC, std::string(int)>::value,
              "has_method_func123 test failed.");

TLX_MAKE_HAS_STATIC_METHOD(func456);

static_assert(has_method_func456<ClassC, double(const std::string&)>::value,
              "has_method_func123 test failed.");
static_assert(!has_method_func456<ClassC, double(int)>::value,
              "has_method_func123 test failed.");

TLX_MAKE_HAS_TEMPLATE_METHOD(tfunc123);

static_assert(!has_method_tfunc123<ClassC, void(std::string, double)>::value,
              "has_method_tfunc123 test failed.");
static_assert(!has_method_tfunc123<ClassC, void(int, double)>::value,
              "has_method_tfunc123 test failed.");

static_assert(!has_method_tfunc123<ClassD, void(int, double)>::value,
              "has_method_tfunc123 test failed.");

#endif

/******************************************************************************/

int main()
{
    return 0;
}

/******************************************************************************/
