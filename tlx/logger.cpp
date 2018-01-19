/*******************************************************************************
 * tlx/logger.cpp
 *
 * Simple logging methods using ostream output.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/logger.hpp>

#include <atomic>
#include <iostream>
#include <mutex>
#include <string>

namespace tlx {

/******************************************************************************/

//! default output logger
class DefaultLoggerOutput : public LoggerOutputHook
{
    //! the global mutex of logger and spacing logger
    std::mutex mutex_;

    //! method the receive log lines
    void append_log_line(const std::string& line) final {
        // lock the global mutex of logger for serialized output in
        // multi-threaded programs.
        std::unique_lock<std::mutex> lock(mutex_);
        std::cout << line;
        std::cout.flush();
    }
};

//! default logger singleton
static DefaultLoggerOutput s_default_logger;

//! global logger hook
static std::atomic<LoggerOutputHook*> s_logger_hook {
    &s_default_logger
};

LoggerOutputHook * set_logger_output_hook(LoggerOutputHook* hook) {
    return s_logger_hook.exchange(hook);
}

/******************************************************************************/

Logger::~Logger() {
    oss_ << '\n';
    (*s_logger_hook).append_log_line(oss_.str());
}

SpacingLogger::~SpacingLogger() {
    oss_ << '\n';
    (*s_logger_hook).append_log_line(oss_.str());
}

/******************************************************************************/

LoggerOutputHook::~LoggerOutputHook() { }

/*----------------------------------------------------------------------------*/

LoggerCollectOutput::LoggerCollectOutput(bool echo)
    : echo_(echo) {
    next_ = set_logger_output_hook(this);
}

LoggerCollectOutput::~LoggerCollectOutput() {
    // set old logger hook
    set_logger_output_hook(next_);
}

void LoggerCollectOutput::clear() {
    oss_.str(std::string());
}

std::string LoggerCollectOutput::get() {
    return oss_.str();
}

void LoggerCollectOutput::append_log_line(const std::string& line) {
    oss_ << line;
    if (echo_) {
        // pass through
        next_->append_log_line(line);
    }
}

} // namespace tlx

/******************************************************************************/
