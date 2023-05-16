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
// this makes yield() available in older GCC versions
#define _GLIBCXX_USE_SCHED_YIELD

#include <cstdint>
#include <random>
#include <thread>

#include <tlx/die.hpp>
#include <tlx/simple_vector.hpp>
#include <tlx/thread_barrier_mutex.hpp>
#include <tlx/thread_barrier_spin.hpp>

template <typename ThreadBarrier>
static void TestWaitFor(int count, int slowThread = -1) {

    int maxWaitTime = 10000;

    ThreadBarrier barrier(count);

    tlx::simple_vector<std::uint8_t> flags(count);
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

                    barrier.wait_yield(
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
    int count = 8;

    // run with 8 threads, one slow one
    for (int i = 0; i < count; i++) {
        TestWaitFor<tlx::ThreadBarrierMutex>(count, i);
    }

    // run with 32 threads
    TestWaitFor<tlx::ThreadBarrierMutex>(32);

#if !defined(TLX_HAVE_THREAD_SANITIZER)
    // run with 8 threads, one slow one
    for (int i = 0; i < count; i++) {
        TestWaitFor<tlx::ThreadBarrierSpin>(count, i);
    }

    // run with 32 threads
    TestWaitFor<tlx::ThreadBarrierSpin>(32);
#endif // !defined(TLX_HAVE_THREAD_SANITIZER)

    return 0;
}

/******************************************************************************/
