/*******************************************************************************
 * tests/thread_barrier_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

// this makes sleep_for() available in older GCC versions
#define _GLIBCXX_USE_NANOSLEEP

#include <random>
#include <thread>

#include <tlx/die.hpp>
#include <tlx/simple_vector.hpp>
#include <tlx/thread_barrier_mutex.hpp>

static void TestWaitFor(int count, int slowThread = -1) {

    int maxWaitTime = 10000;

    tlx::ThreadBarrierMutex barrier(count);

    tlx::simple_vector<uint8_t> flags(count);
    tlx::simple_vector<std::thread> threads(count);

    for (int i = 0; i < count; i++) {
        flags[i] = false;
    }

    for (int t = 0; t < count; t++) {
        threads[t] = std::thread(
            [maxWaitTime, count, slowThread, &barrier, &flags, t] {
                for (size_t r = 0; r < 100; ++r) {
                    std::minstd_rand0 rng(t + r);
                    rng();

                    if (slowThread == -1) {
                        std::this_thread::sleep_for(
                            std::chrono::microseconds(rng() % maxWaitTime));
                    }
                    else if (t == slowThread) {
                        std::this_thread::sleep_for(
                            std::chrono::microseconds(rng() % maxWaitTime));
                    }

                    // every thread sets a flag
                    flags[t] = true;

                    barrier.wait();

                    for (int i = 0; i < count; i++) {
                        // check flags
                        die_unequal(flags[i], true);
                    }

                    barrier.wait(
                        [&]() {
                            // reset flags
                            for (int i = 0; i < count; i++) {
                                flags[i] = false;
                            }
                        });
                }
            });
    }

    for (int t = 0; t < count; t++) {
        threads[t].join();
    }
}

int main() {

    // run with 8 threads, one slow one
    int count = 8;
    for (int i = 0; i < count; i++) {
        TestWaitFor(count, i);
    }

    // run with 32 threads
    TestWaitFor(32);

    return 0;
}

/******************************************************************************/
