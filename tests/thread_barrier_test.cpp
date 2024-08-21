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

#include <tlx/cmdline_parser.hpp>
#include <tlx/container/simple_vector.hpp>
#include <tlx/die.hpp>
#include <tlx/thread_barrier_mutex.hpp>
#include <tlx/thread_barrier_spin.hpp>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <random>
#include <thread>

template <typename ThreadBarrier>
static void TestWaitFor(int thread_count, int slowThread = -1)
{
    int maxWaitTime = 10000;

    ThreadBarrier barrier(thread_count);

    tlx::simple_vector<std::uint8_t> flags(thread_count);
    tlx::simple_vector<std::thread> threads(thread_count);

    for (int i = 0; i < thread_count; i++)
        flags[i] = false;

    for (int t = 0; t < thread_count; t++)
    {
        threads[t] = std::thread(
            [maxWaitTime, thread_count, slowThread, &barrier, &flags, t] {
                for (size_t r = 0; r < 100; ++r)
                {
                    std::minstd_rand0 rng(t + r);
                    rng();

                    if (slowThread == -1)
                    {
                        std::this_thread::sleep_for(
                            std::chrono::microseconds(rng() % maxWaitTime));
                    }
                    else if (t == slowThread)
                    {
                        std::this_thread::sleep_for(
                            std::chrono::microseconds(rng() % maxWaitTime));
                    }

                    // every thread sets a flag
                    flags[t] = true;

                    barrier.wait();

                    for (int i = 0; i < thread_count; i++)
                    {
                        // check flags
                        die_unequal(flags[i], true);
                    }

                    barrier.wait_yield([&]() {
                        // reset flags
                        for (int i = 0; i < thread_count; i++)
                            flags[i] = false;
                    });
                }
            });
    }

    for (int t = 0; t < thread_count; t++)
        threads[t].join();
}

int main(int argc, char* argv[])
{
    tlx::CmdlineParser cp;

    // add description
    cp.set_description("Test program for tlx::ThreadBarrier.");

    int thread_count = 4;
    cp.add_int('t', "threads", thread_count, "number of threads");

    int high_thread_count = 16;
    cp.add_int('T', "high-threads", high_thread_count,
               "number of threads for high thread tests");

    // process command line
    if (!cp.process(argc, argv))
        return -1;

    // run with 4 threads, one slow one
    for (int i = 0; i < thread_count; i++)
        TestWaitFor<tlx::ThreadBarrierMutex>(thread_count, i);

    // run with 16 threads
    TestWaitFor<tlx::ThreadBarrierMutex>(high_thread_count);

#if !defined(TLX_HAVE_THREAD_SANITIZER)
    // run with 4 threads, one slow one
    for (int i = 0; i < thread_count; i++)
        TestWaitFor<tlx::ThreadBarrierSpin>(thread_count, i);

    // run with 16 threads
    TestWaitFor<tlx::ThreadBarrierSpin>(high_thread_count);
#endif // !defined(TLX_HAVE_THREAD_SANITIZER)

    return 0;
}

/******************************************************************************/
