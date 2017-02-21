/*******************************************************************************
 * tests/cmdline_parser_test.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013-2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <sstream>
#include <string>

#include <tlx/cmdline_parser.hpp>
#include <tlx/die.hpp>

void test1() {
    int a_int = 0;
    std::string a_str;

    tlx::CmdlineParser cp;
    cp.add_int('i', "int", "<N>", a_int, "an integer");
    cp.add_string('f', "filename", "<F>", a_str, "a filename");

    cp.set_description("Command Line Parser Test");
    cp.set_author("Timo Bingmann <tb@panthema.net>");

    // good command line
    const char* cmdline1[] = { "test", "-i", "42", "-f", "somefile", nullptr };

    std::ostringstream os1;
    die_unless(cp.process(5, cmdline1, os1));

    die_unequal(a_int, 42);
    die_unequal(a_str, "somefile");

    // bad command line
    const char* cmdline2[] = { "test", "-i", "dd", "-f", "somefile", nullptr };

    std::ostringstream os2;
    die_if(cp.process(5, cmdline2, os2));
}

int main() {
    test1();

    return 0;
}

/******************************************************************************/
