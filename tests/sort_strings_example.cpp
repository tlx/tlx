/*******************************************************************************
 * tests/sort_strings_example.cpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2020 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#include <tlx/cmdline_parser.hpp>
#include <tlx/container/simple_vector.hpp>
#include <tlx/sort/strings.hpp>
#include <tlx/sort/strings_parallel.hpp>
#include <tlx/string/format_iec_units.hpp>
#include <tlx/timestamp.hpp>
#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    tlx::CmdlineParser cp;

    // add description
    cp.set_description("Example program to read a file and sort its lines.");

    bool parallel = false;
    cp.add_flag('p', "parallel", parallel, "sort with parallel algorithm");

    // add a required parameter
    std::string file;
    cp.add_param_string("file", file, "A file to process");

    // process command line
    if (!cp.process(argc, argv))
        return -1;

    std::cerr << "Opening " << file << '\n';
    std::ifstream in(file.c_str());
    if (!in.good())
    {
        std::cerr << "Error opening file: " << strerror(errno) << '\n';
        return -1;
    }

    if (!in.seekg(0, std::ios::end).good())
    {
        std::cerr << "Error seeking to end of file: " << strerror(errno)
                  << '\n';
        return -1;
    }
    size_t size = in.tellg();

    // allocate uninitialized memory area and string pointer array
    tlx::simple_vector<std::uint8_t> data(size + 8);
    std::vector<std::uint8_t*> strings;

    // read file and make string pointer array
    double ts1_read = tlx::timestamp();

    std::cerr << "Reading " << size << " bytes\n";
    in.seekg(0, std::ios::beg);

    // first string pointer
    strings.push_back(data.data() + 0);

    size_t pos = 0;
    while (pos < size)
    {
        size_t rem = std::min<size_t>(2 * 1024 * 1024U, size - pos);
        in.read(reinterpret_cast<char*>(data.data() + pos), rem);

        std::uint8_t* chunk = data.data() + pos;

        for (size_t i = 0; i < rem; ++i)
        {
            if (chunk[i] == '\n')
            {
                chunk[i] = 0;
                if (pos + i + 1 < size)
                    strings.push_back(chunk + i + 1);
            }
            else if (chunk[i] == '\0')
            {
                if (pos + i + 1 < size)
                    strings.push_back(chunk + i + 1);
            }
        }
        pos += rem;
    }

    // final zero termination
    data[size] = 0;

    double ts2_read = tlx::timestamp();

    std::cerr << "Reading took " << ts2_read - ts1_read << " seconds at "
              << tlx::format_iec_units(size / (ts2_read - ts1_read)) << "B/s\n";

    std::cerr << "Found " << strings.size() << " strings to sort.\n";

    // sort
    double ts1_sort = tlx::timestamp();
    {
        if (parallel)
            tlx::sort_strings_parallel(strings);
        else
            tlx::sort_strings(strings);
    }
    double ts2_sort = tlx::timestamp();

    std::cerr << "Sorting took " << ts2_sort - ts1_sort << " seconds.\n";

    // output sorted strings
    double ts1_write = tlx::timestamp();

    for (size_t i = 0; i < strings.size(); ++i)
        std::cout << strings[i] << '\n';

    double ts2_write = tlx::timestamp();

    std::cerr << "Writing took " << ts2_write - ts1_write << " seconds at "
              << tlx::format_iec_units(size / (ts2_write - ts1_write))
              << "/s\n";

    return 0;
}

/******************************************************************************/
