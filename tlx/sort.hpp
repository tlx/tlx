/*******************************************************************************
 * tlx/sort.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_SORT_HEADER
#define TLX_SORT_HEADER

//! \defgroup tlx_sort Sorting Algorithms
//! Specialized Sorting Algorithms

/*[[[perl
foreach (sort grep(!/_impl/, glob("tlx/sort/"."*.hpp"))) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n";
}
]]]*/
#include <tlx/sort/parallel_mergesort.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/sort/strings.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/sort/strings_parallel.hpp>   // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_SORT_HEADER

/******************************************************************************/
