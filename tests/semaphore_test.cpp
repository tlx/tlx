/*******************************************************************************
 * tests/semaphore_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/semaphore.hpp>

#include <tlx/die.hpp>

#include <thread>

static void test_semaphore() {

    static const size_t limit = 16;
    tlx::Semaphore sem;

    std::thread t1 = std::thread(
        [&]() {
            for (size_t i = 0; i < limit; ++i) {
                sem.signal(i);
            }
            sem.signal();
        });

    std::thread t2 = std::thread(
        [&]() {
            for (size_t i = 0; i < limit; ++i) {
                sem.wait(i);
            }
            sem.wait();
        });

    t1.join();
    t2.join();
}

int main() {
    test_semaphore();

    return 0;
}

/******************************************************************************/
