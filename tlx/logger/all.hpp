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
  print "#include <$_>\n";
}
]]]*/
#include <tlx/logger/array.hpp>
#include <tlx/logger/core.hpp>
#include <tlx/logger/deque.hpp>
#include <tlx/logger/map.hpp>
#include <tlx/logger/set.hpp>
#include <tlx/logger/tuple.hpp>
#include <tlx/logger/unordered_map.hpp>
#include <tlx/logger/unordered_set.hpp>
#include <tlx/logger/wrap_unprintable.hpp>
// [[[end]]]

#endif // !TLX_LOGGER_ALL_HEADER

/******************************************************************************/
