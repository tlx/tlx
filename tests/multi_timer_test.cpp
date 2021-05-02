/*******************************************************************************
 * tests/multi_timer_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2019 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

// this makes sleep_for() available in older GCC versions
#define _GLIBCXX_USE_NANOSLEEP

#include <tlx/multi_timer.hpp>

#include <thread>

#include <tlx/die.hpp>

int main() {
    tlx::MultiTimer mtimer;

    mtimer.start("first");
    std::this_thread::sleep_for(std::chrono::milliseconds(120));
    {
        tlx::ScopedMultiTimerSwitch sts(mtimer, "second");
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }
    mtimer.stop();

    die_unless(mtimer.get("first") >= 0.1);
    die_unless(mtimer.get("second") >= 0.1);

    return 0;
}

/******************************************************************************/
