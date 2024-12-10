/*******************************************************************************
 * tlx/logger/core.cpp
 *
 * Simple logging methods using ostream output.
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2015-2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/container/string_view.hpp>
#include <tlx/logger/core.hpp>
#include <atomic>
#include <iostream>
#include <mutex>
#include <string>

namespace tlx {

/******************************************************************************/

//! default output logger to cout
class DefaultLoggerOutputCOut : public LoggerOutputHook
{
    //! the global mutex of logger and spacing logger
    std::mutex mutex_;

    //! method the receive log lines
    void append_log_line(tlx::string_view line) final
    {
        // lock the global mutex of logger for serialized output in
        // multi-threaded programs.
        std::unique_lock<std::mutex> lock(mutex_);
        std::cout.write(line.data(), line.size()).flush();
    }
};

//! default output logger to cerr
class DefaultLoggerOutputCErr : public LoggerOutputHook
{
    //! the global mutex of logger and spacing logger
    std::mutex mutex_;

    //! method the receive log lines
    void append_log_line(tlx::string_view line) final
    {
        // lock the global mutex of logger for serialized output in
        // multi-threaded programs.
        std::unique_lock<std::mutex> lock(mutex_);
        std::cerr.write(line.data(), line.size()).flush();
    }
};

//! default logger singletons
static DefaultLoggerOutputCOut s_default_logger_cout;

//! default logger singletons
static DefaultLoggerOutputCErr s_default_logger_cerr;

//! global logger output hook
static std::atomic<LoggerOutputHook*> s_logger_output_hook{
    &s_default_logger_cout};

LoggerOutputHook* set_logger_output_hook(LoggerOutputHook* hook)
{
    return s_logger_output_hook.exchange(hook);
}

LoggerOutputHook* set_logger_to_stderr()
{
    return set_logger_output_hook(&s_default_logger_cerr);
}

/******************************************************************************/

//! global logger prefix hook
static std::atomic<LoggerPrefixHook*> s_logger_prefix_hook{nullptr};

LoggerPrefixHook* set_logger_prefix_hook(LoggerPrefixHook* hook)
{
    return s_logger_prefix_hook.exchange(hook);
}

/******************************************************************************/

Logger::Logger()
{
    LoggerPrefixHook* prefix_hook = s_logger_prefix_hook.load();
    if (prefix_hook != nullptr)
        prefix_hook->add_log_prefix(oss_);
}

Logger::~Logger()
{
    oss_ << '\n';
    (*s_logger_output_hook).append_log_line(oss_.str());
}

SpacingLogger::SpacingLogger()
{
    LoggerPrefixHook* prefix_hook = s_logger_prefix_hook.load();
    if (prefix_hook != nullptr)
        prefix_hook->add_log_prefix(oss_);
}

SpacingLogger::~SpacingLogger()
{
    oss_ << '\n';
    (*s_logger_output_hook).append_log_line(oss_.str());
}

/******************************************************************************/

LoggerPrefixHook::~LoggerPrefixHook()
{
}

/******************************************************************************/

LoggerOutputHook::~LoggerOutputHook()
{
}

/*----------------------------------------------------------------------------*/

LoggerCollectOutput::LoggerCollectOutput(bool echo) : echo_(echo)
{
    // NOLINTNEXTLINE(cppcoreguidelines-prefer-member-initializer)
    next_ = set_logger_output_hook(this);
}

LoggerCollectOutput::~LoggerCollectOutput()
{
    // set old logger hook
    set_logger_output_hook(next_);
}

void LoggerCollectOutput::clear()
{
    oss_.str(std::string());
}

std::string LoggerCollectOutput::get()
{
    return oss_.str();
}

void LoggerCollectOutput::append_log_line(tlx::string_view line)
{
    oss_ << line;
    if (echo_)
    {
        // pass through
        next_->append_log_line(line);
    }
}

} // namespace tlx

/******************************************************************************/
