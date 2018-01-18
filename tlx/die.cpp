/*******************************************************************************
 * tlx/die.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2016-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/die.hpp>

#include <iostream>
#include <sstream>

namespace tlx {

void die_with_message(const std::string& msg) {
    std::cerr << msg << std::endl;
    std::terminate();
}

void die_with_message(const char* msg, const char* file, size_t line) {
    std::ostringstream oss;
    oss << msg << " @ " << file << ':' << line;
    die_with_message(oss.str());
}

void die_with_message(const std::string& msg, const char* file, size_t line) {
    return die_with_message(msg.c_str(), file, line);
}

/******************************************************************************/
/** \page tlx_die die() - Simple Invariant Testing

tlx contains a set of macros called `die_...` for simple invariant testing. They
test some condition and fail with nice output containing both the condition, and
file/line information where it occurred.

- `die(message)` - always terminates with given message.
- `die_unless(condition)` - terminates if condition is false
- `die_if(condition)` - terminates if condition is true
- `die_unequal(a,b)` - terminates unless a == b.
- `die_unless_throws(code,exception)` - terminate if code does not throw the exception

Furthermore, some additional assert macros are also available. These are only
active in Debug mode, if NDEBUG is defined they are compiled out.

- `assert_equal(a,b)` - checks if a == b.

 */

} // namespace tlx

/******************************************************************************/
