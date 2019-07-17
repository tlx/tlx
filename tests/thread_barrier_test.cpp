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

#include <atomic>
#include <random>
#include <thread>
#include <vector>

#include <tlx/die.hpp>
#include <tlx/simple_vector.hpp>
#include <tlx/thread_barrier_mutex.hpp>

static void TestWaitFor(int count, int slowThread = -1) {

    int maxWaitTime = 100000;

    tlx::ThreadBarrierMutex barrier(count);

    // Need to use atomic here, since setting a bool might not be atomic.
    tlx::simple_vector<std::atomic<uint8_t> > flags(count);
    tlx::simple_vector<std::thread> threads(count);

    for (int i = 0; i < count; i++) {
        flags[i] = false;
    }

    for (int i = 0; i < count; i++) {
        threads[i] = std::thread(
            [maxWaitTime, count, slowThread, &barrier, &flags, i] {
                std::minstd_rand0 rng(i);
                rng();

                if (slowThread == -1) {
                    std::this_thread::sleep_for(
                        std::chrono::microseconds(rng() % maxWaitTime));
                }
                else if (i == slowThread) {
                    std::this_thread::sleep_for(
                        std::chrono::microseconds(rng() % maxWaitTime));
                }

                flags[i] = true;

                barrier.wait();

                for (int j = 0; j < count; j++) {
                    die_unequal(flags[j].load(), true);
                }
            });
    }

    for (int i = 0; i < count; i++) {
        threads[i].join();
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
