/*******************************************************************************
 * tlx/algorithm.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2018 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_ALGORITHM_HEADER
#define TLX_ALGORITHM_HEADER

//! \defgroup tlx_algorithm Algorithms
//! Algorithms for iterators and ranges

/*[[[perl
foreach (sort glob("tlx/algorithm/"."*.hpp")) {
  print "#include <$_> // NOLINT(misc-include-cleaner)\n";
}
]]]*/
#include <tlx/algorithm/exclusive_scan.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/is_sorted_cmp.hpp>  // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/merge_advance.hpp>  // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/merge_combine.hpp>  // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/multisequence_partition.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/multisequence_selection.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/multiway_merge.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/multiway_merge_splitting.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/parallel_multiway_merge.hpp> // NOLINT(misc-include-cleaner)
#include <tlx/algorithm/random_bipartition_shuffle.hpp> // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_ALGORITHM_HEADER

/******************************************************************************/
