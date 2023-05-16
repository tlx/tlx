/*******************************************************************************
 * tests/cmdline_parser_example.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2013 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

// [example]
#include <tlx/cmdline_parser.hpp>

#include <cstdint>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    tlx::CmdlineParser cp;

    // add description and author
    cp.set_description("This may some day be a useful program, which solves "
                       "many serious problems of the real world and achives "
                       "global peace.");
    cp.set_author("Timo Bingmann <tb@panthema.net>");

    // add an unsigned integer option --rounds <N>
    unsigned rounds = 0;
    cp.add_unsigned('r', "rounds", "N", rounds,
                    "Run N rounds of the experiment.");

    // add a byte size argument which the user can enter like '1gi'
    std::uint64_t a_size = 0;
    cp.add_bytes('s', "size", a_size,
                 "Number of bytes to process.");

    // add a required parameter
    std::string a_filename;
    cp.add_param_string("filename", a_filename,
                        "A filename to process");

    // process command line
    if (!cp.process(argc, argv))
        return -1; // some error occurred and help was always written to user.

    std::cout << "Command line parsed okay." << std::endl;

    // output for debugging
    cp.print_result();

    // do something useful

    return 0;
}
// [example]

/******************************************************************************/
