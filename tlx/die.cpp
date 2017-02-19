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

} // namespace tlx

/******************************************************************************/
