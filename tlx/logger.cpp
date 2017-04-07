/*******************************************************************************
 * tlx/logger.cpp
 *
 * Simple logging methods using ostream output.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/logger.hpp>

#include <iostream>
#include <mutex>
#include <string>

namespace tlx {

/******************************************************************************/

//! mutex for log output
static std::mutex s_logger_mutex;

void Logger::Output(const char* str) {
    // lock the global mutex of logger for serialized output in multi-threaded
    // programs.
    std::unique_lock<std::mutex> lock(s_logger_mutex);
    std::cout << str;
}

void Logger::Output(const std::string& str) {
    // lock the global mutex of logger for serialized output in multi-threaded
    // programs.
    std::unique_lock<std::mutex> lock(s_logger_mutex);
    std::cout << str;
}

Logger::~Logger() {
    oss_ << '\n';
    Output(oss_.str());
}

SpacingLogger::~SpacingLogger() {
    oss_ << '\n';
    Logger::Output(oss_.str());
}

} // namespace tlx

/******************************************************************************/
