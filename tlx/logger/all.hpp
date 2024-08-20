/*******************************************************************************
 * tlx/logger/all.hpp
 *
 * Part of tlx - http://panthema.net/tlx
 *
 * Copyright (C) 2017 Timo Bingmann <tb@panthema.net>
 *
 * All rights reserved. Published under the Boost Software License, Version 1.0
 ******************************************************************************/

#ifndef TLX_LOGGER_ALL_HEADER
#define TLX_LOGGER_ALL_HEADER

/*[[[perl
foreach (sort glob("tlx/logger/"."*.hpp")) {
  next if $_ eq "tlx/logger/all.hpp";
  print "#include <$_> // NOLINT(misc-include-cleaner)\n";
}
]]]*/
#include <tlx/logger/array.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/logger/core.hpp>             // NOLINT(misc-include-cleaner)
#include <tlx/logger/deque.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/logger/map.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/logger/set.hpp>              // NOLINT(misc-include-cleaner)
#include <tlx/logger/tuple.hpp>            // NOLINT(misc-include-cleaner)
#include <tlx/logger/unordered_map.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/logger/unordered_set.hpp>    // NOLINT(misc-include-cleaner)
#include <tlx/logger/wrap_unprintable.hpp> // NOLINT(misc-include-cleaner)
// [[[end]]]

#endif // !TLX_LOGGER_ALL_HEADER

/******************************************************************************/
