/*******************************************************************************
 * tests/counting_ptr_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/counting_ptr.hpp>
#include <tlx/die.hpp>

static unsigned int count_deletes = 0;

// derive from counted_object to include reference counter
struct MyIntegerRC : public tlx::ReferenceCounter {

    //! some value
    int i;

    explicit MyIntegerRC(int _i) : i(_i) { }

    //! default copy constructor for unify()
    MyIntegerRC(const MyIntegerRC&) = default;

    // count number of destructor calls
    ~MyIntegerRC() { ++count_deletes; }
};

using IntegerPtr = tlx::CountingPtr<MyIntegerRC>;
using IntegerCPtr = tlx::CountingPtr<const MyIntegerRC>;

template class tlx::CountingPtr<MyIntegerRC>;

static IntegerPtr MakeIntegerPtr() {
    return tlx::make_counting<MyIntegerRC>(24);
}

int main() {

    count_deletes = 0;
    {
        {
            // create object and pointer to it
            IntegerPtr i1 = tlx::make_counting<MyIntegerRC>(42);

            die_unequal(42, i1->i);
            die_unequal(42, (*i1).i);
            die_unequal(42, i1.get()->i);
            die_unless(i1->unique());

            // make pointer sharing same object
            IntegerPtr i2 = i1; // NOLINT

            die_unequal(42, i2->i);
            die_unless(!i1->unique());
            die_unless(i2 == i1);
            die_unequal(2u, i1->reference_count());

            // make another pointer sharing the same object
            IntegerPtr i3 = i2;

            die_unequal(42, i3->i);
            die_unequal(3u, i3->reference_count());

            // replace object in i3 with new integer
            i3 = IntegerPtr(new MyIntegerRC(5));
            die_unless(i3 != i1);
            die_unequal(2u, i1->reference_count());
        }

        // check number of objects destructed
        die_unequal(2u, count_deletes);

        // get a pointer from a function
        IntegerPtr i4 = MakeIntegerPtr();

        // quitting the block will release the ptr
    }

    die_unequal(3u, count_deletes);

    return 0;
}

/******************************************************************************/
