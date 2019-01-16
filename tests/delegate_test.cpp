/*******************************************************************************
 * tests/delegate_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/delegate.hpp>
#include <tlx/die.hpp>

using tlx::Delegate;

using TestDelegate = Delegate<int (int)>;
// TODO(tb): add tests with a different allocator

int func1(int a) {
    return a + 5;
}

int func2(int a) {
    return a + 10;
}

static void test_plain_functions() {
    {
        // construction from a immediate function with no object or pointer.
        TestDelegate d = TestDelegate::make<func1>();
        die_unequal(42, d(37));
    }
    {
        // construction from a plain function pointer.
        TestDelegate d = TestDelegate(func1);
        die_unequal(42, d(37));
    }
    {
        // construction from a plain function pointer.
        TestDelegate d = TestDelegate::make(func1);
        die_unequal(42, d(37));
    }
}

class A
{
public:
    int x;

    int func(int a) {
        return a + x;
    }

    int const_func(int a) const {
        return a + x;
    }

    int func2(int a) {
        return a + x + x;
    }
};

static void test_class_methods() {

    A a = { 2 };
    {
        // construction for an immediate class::method with class object
        TestDelegate d = TestDelegate::make<A, &A::func>(&a);
        die_unequal(42, d(40));
    }
    {
        // construction for an immediate class::method with class object
        TestDelegate d = TestDelegate::make<A, &A::const_func>(&a);
        die_unequal(42, d(40));
    }
    {
        // construction for an immediate class::method with class object by
        // reference
        TestDelegate d = TestDelegate::make<A, &A::func>(a);
        die_unequal(42, d(40));
    }
    {
        // construction for an immediate class::method with class object by
        // reference
        TestDelegate d = TestDelegate::make<A, &A::const_func>(a);
        die_unequal(42, d(40));
    }

    {
        // constructor from an indirect class::method with object pointer.
        TestDelegate d = TestDelegate(&a, &A::func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object pointer.
        TestDelegate d = TestDelegate(&a, &A::const_func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object reference.
        TestDelegate d = TestDelegate(a, &A::func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object reference.
        TestDelegate d = TestDelegate(a, &A::const_func);
        die_unequal(42, d(40));
    }

    {
        // constructor from an indirect class::method with object pointer.
        TestDelegate d = TestDelegate::make(&a, &A::func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object pointer.
        TestDelegate d = TestDelegate::make(&a, &A::const_func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object reference.
        TestDelegate d = TestDelegate::make(a, &A::func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object reference.
        TestDelegate d = TestDelegate::make(a, &A::const_func);
        die_unequal(42, d(40));
    }

    {
        // constructor from an indirect class::method with object pointer.
        TestDelegate d = tlx::make_delegate(&a, &A::func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object pointer.
        TestDelegate d = tlx::make_delegate(&a, &A::const_func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object reference.
        TestDelegate d = tlx::make_delegate(a, &A::func);
        die_unequal(42, d(40));
    }
    {
        // constructor from an indirect class::method with object reference.
        TestDelegate d = tlx::make_delegate(a, &A::const_func);
        die_unequal(42, d(40));
    }
}

class AddFunctor
{
public:
    int x;

    int operator () (int a) {
        return a + x;
    }
};

static void test_functor_class() {

    AddFunctor f = { 12 };

    {
        // calls general functor constructor
        TestDelegate d = TestDelegate(f);
        die_unequal(42, d(30));
    }
    {
        // calls general functor constructor
        TestDelegate d = TestDelegate::make(f);
        die_unequal(42, d(30));
    }
}

static void test_lambdas() {

    {
        TestDelegate d = TestDelegate([](int x) { return x + 1; });
        die_unequal(42, d(41));
    }
    {
        TestDelegate d = TestDelegate::make([](int x) { return x + 1; });
        die_unequal(42, d(41));
    }
    {
        // test a lambda with capture
        int val = 10;
        TestDelegate d = TestDelegate::make([&](int x) { return x + val; });
        die_unequal(42, d(32));
    }
    {
        TestDelegate d = [](int x) { return x + 1; };
        die_unequal(42, d(41));
    }
}

int main() {
    test_plain_functions();
    test_class_methods();
    test_functor_class();
    test_lambdas();

    return 0;
}

namespace tlx {

// force template instantiation
template class Delegate<int(int)>;
// TODO(tb): add tests with a different allocator

} // namespace tlx

/******************************************************************************/
