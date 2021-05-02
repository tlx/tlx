/*******************************************************************************
 * tests/thread_pool_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

// this makes sleep_for() available in older GCC versions
#define _GLIBCXX_USE_NANOSLEEP

#include <numeric>
#include <string>
#include <vector>

#include <tlx/die.hpp>
#include <tlx/thread_pool.hpp>

void test_loop_until_empty() {
    size_t job_num = 256;

    std::vector<size_t> result1(job_num, 0), result2(job_num, 0);

    {
        tlx::ThreadPool pool(8);

        for (size_t r = 0; r != 16; ++r) {

            for (size_t i = 0; i != job_num; ++i) {
                pool.enqueue(
                    [i, &result1, &result2, &pool]() {
                        // set flag
                        result1[i] = 1 + i;

                        // enqueue more work.
                        pool.enqueue(
                            [i, &result2]() {
                                result2[i] = 2 + i;
                            });
                    });
            }

            pool.loop_until_empty();
        }
    }

    // check that the threads have run
    for (size_t i = 0; i != job_num; ++i) {
        die_unequal(result1[i], 1 + i);
        die_unequal(result2[i], 2 + i);
    }
}

void test_loop_until_terminate(size_t sleep_msec) {
    size_t job_num = 256;

    std::vector<int> result1(job_num, 0), result2(job_num, 0);

    std::chrono::milliseconds sleep_time(sleep_msec);

    tlx::ThreadPool pool(8);

    for (size_t i = 0; i != job_num; ++i) {
        pool.enqueue(
            [i, &result1, &result2, &pool, &sleep_time]() {
                // set flag
                result1[i] = 1;
                std::this_thread::sleep_for(sleep_time);

                // enqueue more work: how to call this lambda again?
                pool.enqueue(
                    [i, &result2, &sleep_time]() {
                        result2[i] = 1;
                        std::this_thread::sleep_for(sleep_time);
                    });
            });
    }

    using steady_clock = std::chrono::steady_clock;
    steady_clock::time_point tp_start = steady_clock::now();

    // start thread which will stop the thread pool (if we would enqueue this as
    // job, it would be no different from the first test).
    std::thread stopper_thr = std::thread(
        [&pool]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            pool.terminate();
        });

    pool.loop_until_terminate();

    stopper_thr.join();

    // check that it didn't terminate immediately.
    auto tp_delta = std::chrono::duration_cast<std::chrono::milliseconds>(
        steady_clock::now() - tp_start);
    die_unless(tp_delta.count() >= 90);

    // check result: count number of flags set.
    size_t sum = std::accumulate(result1.begin(), result1.end(), 0);
    sum += std::accumulate(result2.begin(), result2.end(), 0);
    die_unequal(sum, pool.done());
}

void test_init_thread() {

    std::atomic<size_t> count { 0 };

    {
        tlx::ThreadPool pool(
            /* num_threads */ 8,
            /* thread initializer */
            [&](size_t i) { count += i; });

        pool.loop_until_empty();
    }

    die_unequal(count.load(), (7 * 8) / 2u);
}

int main() {
    test_loop_until_empty();

    for (size_t i = 0; i < 10; ++i)
        test_loop_until_terminate(i);

    test_init_thread();
    return 0;
}

/******************************************************************************/
