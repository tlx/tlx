/*******************************************************************************
 * tests/container/ring_buffer_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/ring_buffer.hpp>
#include <tlx/die.hpp>

static void test_fill_circular(size_t rb_size) {

    tlx::RingBuffer<size_t> ring(rb_size);

    // put first element
    die_unequal(0u, ring.size());
    ring.push_back(0);

    // put nine more
    for (size_t i = 1; i < 10; ++i) {
        die_unequal(i, ring.size());
        ring.emplace_back(i);
    }

    die_unequal(10u, ring.size());

    // check contents of ring buffer
    for (size_t i = 0; i < ring.size(); ++i) {
        die_unequal(i, ring[i]);
    }

    // copy ring buffer and check contents
    {
        tlx::RingBuffer<size_t> ring2 = ring;
        die_unequal(ring.size(), ring2.size());

        // check contents of ring buffer
        for (size_t i = 0; i < ring2.size(); ++i) {
            die_unequal(i, ring2[i]);
        }

        tlx::RingBuffer<size_t> ring3 = std::move(ring2);
        die_unequal(ring.size(), ring3.size());

        // check contents of ring buffer
        for (size_t i = 0; i < ring3.size(); ++i) {
            die_unequal(i, ring3[i]);
        }
    }

    // copy assign ring buffer and check contents
    {
        tlx::RingBuffer<size_t> ring2;
        ring2 = ring;
        die_unequal(ring.size(), ring2.size());

        // check contents of ring buffer
        for (size_t i = 0; i < ring2.size(); ++i) {
            die_unequal(i, ring2[i]);
        }

        tlx::RingBuffer<size_t> ring3;
        ring3 = std::move(ring2);
        die_unequal(ring.size(), ring3.size());

        // check contents of ring buffer
        for (size_t i = 0; i < ring3.size(); ++i) {
            die_unequal(i, ring3[i]);
        }
    }

    // cycle ring buffer a few times
    for (size_t j = 0; j < 1000; ++j) {
        // check contents of ring buffer
        for (size_t i = 0; i < ring.size(); ++i) {
            die_unequal(j + i, ring[i]);
        }
        die_unequal(j, ring.front());
        die_unequal(j + 9u, ring.back());

        // append an item, and remove one
        ring.push_back(j + 10u);
        ring.pop_front();
        die_unequal(10u, ring.size());
    }
}

struct MyStruct {
    int i1, i2;

    MyStruct(int _i1, int _i2)
        : i1(_i1), i2(_i2) { }
};

static void test_non_default_constructible() {

    tlx::RingBuffer<MyStruct> ring(12);

    ring.push_back(MyStruct(0, 1));
    ring.emplace_back(MyStruct(1, 2));

    ring.push_front(MyStruct(2, 3));
    ring.emplace_front(MyStruct(3, 4));

    die_unequal(4u, ring.size());
    die_unequal(3, ring[0].i1);
    die_unequal(2, ring[1].i1);
    die_unequal(0, ring[2].i1);
    die_unequal(1, ring[3].i1);
}

namespace tlx {

template class RingBuffer<size_t>;
template class RingBuffer<MyStruct>;

} // namespace tlx

int main() {
    test_fill_circular(12);
    test_fill_circular(16);
    test_fill_circular(20);
    test_non_default_constructible();

    return 0;
}

/******************************************************************************/
